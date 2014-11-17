/******************************************************************************
*
*       COPYRIGHT 2001-2004 MOTOROLA, ALL RIGHTS RESERVED
*
*       The code is the property of Motorola St.Petersburg Software Development
*       and is Motorola Confidential Proprietary Information.
*
*       The copyright notice above does not evidence any
*       actual or intended publication of such source code.
*
* Functions:    MSC_Calc
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  TAT+ integer symbolic calculator
*
* Notes:        Variables doesn't supported.
*
******************************************************************************/
#include <portable.h>
#include <mscbase.h>
#include <strproc.h>
#include <outmess.h>
#include <tatcalc.h>
#include <tatgdata.h>

typedef enum
{
    T_UNDEF = 0,
    T_EOL, T_CONST, T_LPAR,
    T_RPAR, T_COMMA, T_POINT, T_MUL,
    T_DIV, T_MOD, T_ADD, T_SUB,
    T_NEG, T_NOT, T_SHL, T_SHR,
    T_AND, T_XOR, T_IOR, T_LAND,
    T_LIOR, T_LNOT, T_EQ, T_LESS,
    T_GT, T_NE, T_LESSEQ, T_GTEQ, T_FUNC
}
TATCALC_TOKEN;                         /* tokens */

/*
 * Operator/Operand stack, base ptr and top of stack ptr
 */
#define MSC_STACK_SIZE 256
struct
{
    TATCALC_TOKEN           o_Token;
    double                  o_Value;
}
tat_OperationStack[MSC_STACK_SIZE];

static int              tat_OpBase, tat_OpSp;

/*
 * tat_Value (working) stack, base ptr and top of stack ptr
 */
static double           tat_ValueStack[MSC_STACK_SIZE];
static int              tat_ValueBase, tat_ValueSp;

/*
 * Built-in Functions and jump table
  int (*Functab[ 2 ])(); */

static int              tat_Level;     /* current recursion level in tatcalc_Calculate() */
static TATCALC_TOKEN    tat_Token;     /* current input token */
static double              tat_Value;     /* and its value */
static char             tat_Eol;       /* set when end of line encountered */
static char            *tat_LinePtr;   /* points to next character in input line */
static int              tat_ErrorFlag; /* set if parsing error occurred */

static double           tatcalc_Evaluate(  );
static int              tatcalc_GetToken(  );
static double           tatcalc_Push( double val );
static double           tatcalc_Pop(  );
static void             tatcalc_ParseExpression(  );
static void             tatcalc_Calculate( char *line );
static int              tatcalc_ParseOperatorPrimary( int precedence, int lvalue );
static void             tatcalc_ParseTokenValue( TATCALC_TOKEN tkn, double val );
static int              tatcalc_ParsePrimary(  );
static TATCALC_TOKEN    tatcalc_ParseToken( char **cpp );

/* static int isfunc( char **cpp); */
static int              tatcalc_GetPrecedence( TATCALC_TOKEN op );

#define                 MSC_CALC_MACRO_MODE 1
#define                 MSC_CALC_EXPR_MODE 0
static int              tat_Mode;

/***************************************************************************
 * Function : tat_SetError
 *
 * Description: set error flag
 *
 * Returns: None
 *
 * Notes: None
 *
 **************************************************************************/
static void tat_SetError(  )
{
    tat_ErrorFlag = 1;
}

/***************************************************************************
 * Function : MSC_Calc
 *
 * Description: Compute, if possible, expression 'expr', save result in 'result'
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_Calc( char *expr, double *result )
{
    StrSkipComments( &expr );
    tat_ErrorFlag = 0;
    tat_Mode = MSC_CALC_EXPR_MODE;
    if ( *expr )
    {
        tatcalc_Calculate( expr );
    }
    else
        tat_SetError();
    if ( !tat_ErrorFlag )
        *result = tat_Value;
    return ( LOGICAL ) ( tat_ErrorFlag == 0 );
}

/***************************************************************************
 * Function : MSC_CalcMacro
 *
 * Description: check macro syntax
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_CalcMacro( char *expr )
{
    StrSkipComments( &expr );
    tat_ErrorFlag = 0;
    tat_Mode = MSC_CALC_MACRO_MODE;
    if ( *expr )
    {
        tatcalc_Calculate( expr );
    }
    else
        tat_SetError();

    return ( LOGICAL ) ( tat_ErrorFlag == 0 );
}

/***************************************************************************
 * Function : tatcalc_Calculate
 *
 * Description: 
 * Parse and calculate an arithmetic tatcalc_ParseExpression pointed to
 * by "line". This routine is fully re-entrant - a feature
 * that is not currently used, but may come in handy later
 * (for instance if a variable points to a character string
 * that contains an tatcalc_ParseExpression to be evaluated).
 * Returns: 
 *
 * Notes: None
 *
 **************************************************************************/
static void tatcalc_Calculate( char *line )
{
    char                   *savLineptr;
    int                     savValbase, savValsp, savOpbase, savOpsp;

    if ( tat_ErrorFlag )
        return;

    tat_Level++;
/*
   * Save all global variables that may change if tatcalc_Calculate()
   * is called recursively.
 */
    savLineptr = tat_LinePtr;
    savValbase = tat_ValueBase;
    savValsp = tat_ValueSp;
    savOpbase = tat_OpBase;
    savOpsp = tat_OpSp;

/*
   * Set up stack base ptrs and input line ptr
 */
    tat_ValueBase = tat_ValueSp;
    tat_OpBase = tat_OpSp;
    tat_LinePtr = line;

/*
   * Get the first token from input line and parse an
   * tatcalc_ParseExpression and then tatcalc_Evaluate it.
 */
    tat_Eol = 0;

    tatcalc_GetToken(  );
    if ( tat_ErrorFlag )
        return;

    if ( !tat_Eol )
    {
        tatcalc_ParseExpression(  );
        if ( !tat_ErrorFlag )
            tat_Value = tatcalc_Evaluate(  );
    }

/*
   * Restore previous values of globals
 */
    tat_LinePtr = savLineptr;
    tat_ValueBase = savValbase;
    tat_ValueSp = savValsp;
    tat_OpBase = savOpbase;
    tat_OpSp = savOpsp;

    --tat_Level;
}

static double tatcalc_Evaluate(  )
{
/*
 * Evaluate an tatcalc_ParseExpression by popping operators and operands
 * from the Operator/Operand stack and performing each indicated
 * operation. Only the operators starting from current base ptr
 * (tat_OpBase) to top of stack (tat_OpSp) are evaluated, so that tatcalc_Evaluate()
 * may be called by any generation of tatcalc_Calculate().
 */
    int                     opsp, *ip;
    double                  val;
    TATCALC_TOKEN           op;

#define TOS (tat_ValueStack[tat_ValueSp-1]) /* top of stack macro */

    for ( opsp = tat_OpBase; opsp < tat_OpSp; ++opsp )
    {
        op = tat_OperationStack[opsp].o_Token;

        if ( op == T_CONST )
            tatcalc_Push( tat_OperationStack[opsp].o_Value );

        else if ( op == T_POINT )
        {
            int p;
        /*
         * Get the value of the integer pointed to by the
         * address on top of the stack. This usually follows
         * a T_SYMBOL when the symbol is not being used
         * as an "lvalue".
         */
            p = (int) tatcalc_Pop(  );
            ip = ( int * ) p;
            tatcalc_Push( *ip );
        }
        else if ( op == T_ADD )
        {
            val = tatcalc_Pop(  );
            TOS += val;
        }
        else if ( op == T_SUB )
        {
            val = tatcalc_Pop(  );
            TOS -= val;
        }
        else if ( op == T_NOT )
        {
            TOS = ~(unsigned int)TOS;
        }
        else if ( op == T_LNOT )
        {
            TOS = !(unsigned int)TOS;
        }
        else if ( op == T_NEG )
        {
            TOS = -TOS;
        }
        else if ( op == T_MUL )
        {
            val = tatcalc_Pop(  );
            TOS *= val;
        }
        else if ( op == T_DIV )
        {
            val = tatcalc_Pop(  );
            if ( (tat_Mode != MSC_CALC_MACRO_MODE) && val == 0 )
            {
                tat_SetError(  );
                break;
            }
            if (tat_Mode != MSC_CALC_MACRO_MODE) TOS /= val;
        }
        else if ( op == T_MOD )
        {
            val = tatcalc_Pop(  );
            if (tat_Mode != MSC_CALC_MACRO_MODE) TOS = fmod(TOS, val);
        }
        else if ( op == T_SHL )
        {
            val = tatcalc_Pop(  );
            TOS *= 1<<(unsigned int)val;
        }
        else if ( op == T_SHR )
        {
            val = tatcalc_Pop(  );
            TOS = (int)TOS >> (unsigned int)val;
        }
        else if ( op == T_AND )
        {
            val = tatcalc_Pop(  );
            TOS = (unsigned int)TOS & (unsigned int)val;
        }
        else if ( op == T_LAND )
        {
            val = tatcalc_Pop(  );
            TOS = (unsigned int)((TOS!=0) && (val!=0));
        }
        else if ( op == T_XOR )
        {
            val = tatcalc_Pop(  );
            TOS = (unsigned int)TOS ^ (unsigned int)val;
        }
        else if ( op == T_IOR )
        {
            val = tatcalc_Pop(  );
            TOS = (unsigned int)TOS | (unsigned int)val;
        }
        else if ( op == T_LIOR )
        {
            val = tatcalc_Pop(  );
            TOS = (unsigned int)((TOS!=0) || (val!=0.0));
        }
        else if ( op == T_EQ )
        {
            val = tatcalc_Pop(  );
            TOS = (unsigned int)(TOS == val);
        }
        else if ( op == T_NE )
        {
            val = tatcalc_Pop(  );
            TOS = (unsigned int)(TOS != val);
        }
        else if ( op == T_LESS )
        {
            val = tatcalc_Pop(  );
            TOS = (unsigned int)(TOS < val);
        }
        else if ( op == T_GT )
        {
            val = tatcalc_Pop(  );
            TOS = (unsigned int)(TOS > val);
        }
        else if ( op == T_LESSEQ )
        {
            val = tatcalc_Pop(  );
            TOS = (unsigned int)(TOS <= val);
        }
        else if ( op == T_GTEQ )
        {
            val = tatcalc_Pop(  );
            TOS = (unsigned int)(TOS >= val);
        }
        else if ( op == T_COMMA )
        {
            /* comma is not currently supported */
            tat_SetError(  );
        }
    /*      else if ( op == T_FUNC )
       tatcalc_Push( (*Functab[ tat_OperationStack[ opsp ].o_Value ])() ); */
        else
            tat_SetError(  );
    }
    return tat_ValueStack[tat_ValueBase];
}

static double tatcalc_Push( double val )
{
    if ( tat_ValueSp >= MSC_STACK_SIZE )
        tat_SetError(  );
    return tat_ValueStack[tat_ValueSp++] = val;
}

static double tatcalc_Pop(  )
{
    if ( --tat_ValueSp < 0 )
        tat_ValueSp = 0;
    return tat_ValueStack[tat_ValueSp];
}

/***************************************************************************
 * Function : tatcalc_ParseExpression
 *
 * Description: Parse an Expression. Expressions have the following syntax:
 *              <expression> := <primary> <operator> <primary>
 *              so the first thing to look for is a primary.
 *
 * Returns: None
 *
 * Notes: None
 *
 **************************************************************************/
static void tatcalc_ParseExpression(  )
{
    int                     lvalue;

    lvalue = tatcalc_ParsePrimary(  );
    if ( !lvalue )
        tat_SetError(  );
    else if ( tat_Eol )
    {
        if ( lvalue < 0 )
            tatcalc_ParseTokenValue( T_POINT, 0 );
    }
    else
        tatcalc_ParseOperatorPrimary( 0, lvalue );
}

/***************************************************************************
 * Function : tatcalc_ParseOperatorPrimary
 *
 * Description: Parse the <operator> <primary> part of an expression.
 *              "precedence" is the PREVIOUS <operator>'s precedence level
 *              (0=low, +n=high).
 *
 * Returns: 
 *
 * Notes: None
 *
 **************************************************************************/
static int tatcalc_ParseOperatorPrimary( int precedence, int lvalue )
{

    TATCALC_TOKEN           tkn;
    int                     pr, lv;

    while ( !tat_Eol )
    {
    /*
     * Get the precedence level of current <operator> ("pr").
     * If it is greater than previous operator ("precedence"),
     * get the next <primary> and do another <operator> <primary>
     * NOTE: For left-to-right associativity, the condition
     *     pr > precedence
     * must be true. for right-to-left associativity,
     *     pr >= precedence
     * must be true (assignment operator only).
     */

        pr = tatcalc_GetPrecedence( tat_Token );

        if ( pr > precedence && ( pr > 0 ) )
        {
            if ( lvalue < 0 )
                tatcalc_ParseTokenValue( T_POINT, 0 );

        /*
         * Save the operator token and do a primary.
         */
            tkn = tat_Token;
            tatcalc_GetToken(  );
            lv = tatcalc_ParsePrimary(  );
            if ( !lv )
                tat_SetError(  );
        /*
         * Now look at the next operator. If its precedence
         * is greater than this one ("tkn" above), tatcalc_ParseTokenValue
         * code for it BEFORE this one.
         */
            lvalue = tatcalc_ParseOperatorPrimary( pr, lv );

            if ( lvalue < 0 )
            {
            /*
             * Next operator is not the assignment op.
             * and the current <primary> is an lvalue,
             * therefore tatcalc_ParseTokenValue a "load from address
             * on top of stack" instruction.
             */
                tatcalc_ParseTokenValue( T_POINT, 0 );
            /*
             * This makes it an rvalue now.
             */
                lvalue = 1;
            }
        /*
         * Generate the instruction for the current operator.
         */
            tatcalc_ParseTokenValue( tkn, 0 );
        }
        else
            break;
    }

    return lvalue;
}

/***************************************************************************
 * Function : tatcalc_ParsePrimary
 *
 * Description: Parse a primary. Primaries have the following syntax:
 *              <primary> := <constant> |
 *                           '(' <tatcalc_ParseExpression> ')' |
 *                           <unary op> <primary> |
 *                           <function> <primary>
 *
 * Returns: 
 *   -1   => the <primary> is an lvalue
 *    0   => not a <primary> (usually end of line or a syntax error)
 *    1   => the <primary> is an rvalue
 *
 *
 * Notes: None
 *
 **************************************************************************/
static int tatcalc_ParsePrimary(  )
{
    int                     rtn;
    double                     val;

    if ( tat_Eol )
        return 0;
    rtn = 1;

    switch ( tat_Token )
    {
    /*  case T_SYMBOL:
       rtn = -1; */
    case T_CONST:              /* a constant */
        tatcalc_ParseTokenValue( tat_Token, tat_Value );
        tatcalc_GetToken(  );
        if (tat_Token == T_CONST)
        {
            tat_SetError(  );
            rtn = 0;
        }
        break;
    case T_LPAR:               /* a parenthesized tatcalc_ParseExpression */
        tatcalc_GetToken(  );
        tatcalc_ParseExpression(  );
        if ( tat_Token != T_RPAR )
        {
            tat_SetError(  );
            rtn = 0;
        }
        else
            tatcalc_GetToken(  );
        break;
    case T_SUB:                /* unary - */
    /*
     * The lexical analyzer is not smart enough to recognize
     * unary operators (+ and -), that's why we have to do
     * it here
     */
        tatcalc_GetToken(  );
        tatcalc_ParseExpression(  );
        tatcalc_ParseTokenValue( T_NEG, 0 );
        break;
    case T_NOT:                /* unary ~ */
        tatcalc_GetToken(  );
        tatcalc_ParseExpression(  );
        tatcalc_ParseTokenValue( T_NOT, 0 );
        break;
    case T_LNOT:               /* unary ! */
        tatcalc_GetToken(  );
        tatcalc_ParseExpression(  );
        tatcalc_ParseTokenValue( T_LNOT, 0 );
        break;
    case T_ADD:                /* unary + */
        tatcalc_GetToken(  );
        tatcalc_ParseExpression(  );
        break;
    case T_FUNC:               /* built-in function */
        val = tat_Value;
        tatcalc_GetToken(  );
        tatcalc_ParseExpression(  );
        tatcalc_ParseTokenValue( T_FUNC, val );
        break;
    default:
    /*
     * Not a primary
     */
        rtn = 0;
    }
    return rtn;
}

/***************************************************************************
 * Function : tatcalc_GetPrecedence
 *
 * Description: Determine if "op" is a binary operator and return its
 *              precedence level if so. If not, return 0.
 *
 * Returns: precedence level. greater means higher
 *
 * Notes: None
 *
 **************************************************************************/
static int tatcalc_GetPrecedence( TATCALC_TOKEN op )
{
    switch ( op )
    {
    case T_COMMA:
        return 1;
    case T_LESS:
    case T_EQ:
    case T_GT:
    case T_GTEQ:
    case T_LESSEQ:
    case T_NE:
        return 3;
    case T_IOR:
    case T_LIOR:
        return 4;
    case T_XOR:
        return 5;
    case T_AND:
    case T_LAND:
        return 6;
    case T_SHL:
    case T_SHR:
        return 7;
    case T_ADD:
    case T_SUB:
        return 8;
    case T_MUL:
    case T_DIV:
    case T_MOD:
        return 9;
    case T_NOT:
    case T_LNOT:
        return 10;
    }
    return 0;
}

/***************************************************************************
 * Function : tatcalc_ParseTokenValue
 *
 * Description: Push the given token and value onto the Operator/Operand stack.
 *
 * Returns: None
 *
 * Notes: None
 *
 **************************************************************************/
static void tatcalc_ParseTokenValue( TATCALC_TOKEN tkn, double val )
{
    if ( tat_OpSp < MSC_STACK_SIZE )
    {
        tat_OperationStack[tat_OpSp].o_Token = tkn;
        tat_OperationStack[tat_OpSp].o_Value = val;
        ++tat_OpSp;
    }
    else
        tat_SetError(  );
}

/***************************************************************************
 * Function : tatcalc_GetToken
 *
 * Description: Lexical Analyzer. Gets next token from the input line
 *              pointed to by "tat_LinePtr" and advances "tat_LinePtr" to next
 *              character. If end of input line is encountered, the
 *              "tat_Eol" flag is set.         
 * Returns: 
 *
 * Notes: None
 *
 **************************************************************************/
static int tatcalc_GetToken(  )
{
    char                    buf[TAT_MAX_ID_LEN];
    char                    *end;
    StrSkipComments( &tat_LinePtr );

    if ( !*tat_LinePtr )
    {
        tat_Eol = 1;
        tat_Token = T_EOL;
    }
    else if (tat_Mode == MSC_CALC_MACRO_MODE && *tat_LinePtr==MSC_MACRO_START)
    {
        tat_Token = T_CONST;
        if ( StrExtractMacroID( &tat_LinePtr, buf ) )
        {
            tat_Value = 0;
        }
        else
            tat_SetError(  );
    }
    /* TO DO: handle variables */
    else if ((*tat_LinePtr>='A' && *tat_LinePtr<='Z') || (*tat_LinePtr>='a' && *tat_LinePtr<='z') || *tat_LinePtr=='_')
    {
        /* if we can parse it */
        if (StrExtractLabel(&tat_LinePtr, buf))
        {
            STR_SKIP_SPACES(tat_LinePtr);
            /* check for array index */
            if (*tat_LinePtr=='[')
            {
                end = StrFindCloseBraceEx( tat_LinePtr, STR_USE_PARENTHESIS | STR_USE_SQUARE_BRACKET );
                if (end)
                {
                    tat_LinePtr=end;
                }
                else
                    tat_SetError();
            }
            if (tat_Mode != MSC_CALC_MACRO_MODE)
                tat_SetError();
            tat_Token = T_CONST;
            tat_Value = 0;
        }
        else
            tat_SetError();
    }
    else if ( SymIsDigit( *tat_LinePtr ) )
    {
    /*
     * It's a numeric constant, "tat_Value" will be its value.
     */
        tat_Token = T_CONST;
        if ( StrExtractDigit( &tat_LinePtr, buf ) )
            tat_Value = StrToDouble( buf );
        else
            tat_SetError(  );
    }
    else if ( ( tat_Token = tatcalc_ParseToken( &tat_LinePtr ) ) )
    {
        ;
    }
/*  else if ( tat_Value = isfunc( &tat_LinePtr ) )
   {
   tat_Token = T_FUNC;
   --tat_Value;
   }
 */
    else
    {
    /*
     * Bad character in input line
     */
        tat_SetError(  );
        ++tat_LinePtr;
    }

    return tat_Token;
}

/***************************************************************************
 * Function : isfunc
 *
 * Description: Check if *cpp is the name of a built-function, return the
 *              unction jump table index+1 if so and bump *cpp to next character
 *
 * Returns: function number/Return 0 if not a function.
 *
 * Notes: None
 *
 **************************************************************************/
/*
static int isfunc( char **cpp)
{
    return funcno;
}
*/

/***************************************************************************
 * Function : tatcalc_ParseToken
 *
 * Description: Check if *cpp is a binary operator, return its token value
 *              and bump *cpp to next character.
 *
 * Returns: 
 *
 * Notes: None
 *
 **************************************************************************/
static TATCALC_TOKEN tatcalc_ParseToken( char **cpp )
{
    TATCALC_TOKEN           tkn = T_UNDEF;
    char                    c;
    char                   *s;

    s = *cpp;
    c = **cpp;

    switch ( c )
    {
    case ',':
        tkn = T_COMMA;
        break;
    case '=':
    {
        if ( *( s + 1 ) == '=' )
        {
            tkn = T_EQ;
            s++;
        };
        break;
    }
    case '<':
    {
        if ( *( s + 1 ) == '<' )
        {
            tkn = T_SHL;
            s++;
        }
        else if ( *( s + 1 ) == '=' )
        {
            tkn = T_LESSEQ;
            s++;
        }
        else
            tkn = T_LESS;
        break;
    }
    case '>':
    {
        if ( *( s + 1 ) == '>' )
        {
            tkn = T_SHR;
            s++;
        }
        else if ( *( s + 1 ) == '=' )
        {
            tkn = T_GTEQ;
            s++;
        }
        else
            tkn = T_GT;
        break;
    }
    case '(':
        tkn = T_LPAR;
        break;
    case ')':
        tkn = T_RPAR;
        break;
    case '*':
        tkn = T_MUL;
        break;
    case '/':
        tkn = T_DIV;
        break;
    case '%':
        tkn = T_MOD;
        break;
    case '+':
        tkn = T_ADD;
        break;
    case '-':
        tkn = T_SUB;
        break;
    case '^':
        tkn = T_XOR;
        break;
    case '&':
    {
        tkn = T_AND;
        if ( *( s + 1 ) == '&' )
        {
            tkn = T_LAND;
            s++;
        }
        break;
    }
    case '|':
    {
        tkn = T_IOR;
        if ( *( s + 1 ) == '|' )
        {
            tkn = T_LIOR;
            s++;
        }
        break;
    }
    case '~':
        tkn = T_NOT;
        break;
    case '!':
        {
            tkn = T_LNOT;
            if ( *( s + 1 ) == '=' )
            {
                tkn = T_NE;
                s++;
            }
            
            break;
        }
    }

    if ( tkn != T_UNDEF )
    {
        s++;
    }
    *cpp = s;
    return tkn;
}
