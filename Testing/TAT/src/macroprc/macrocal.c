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
* Functions:    main
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  Test Calculator module
*
* Notes:        None
*
******************************************************************************/
#include <portable.h>
#include <mscbase.h>
#include <strproc.h>
#include <outmess.h>
#include <tatcalc.h>
#include <qmemory.h>
#include <confproc.h>
#include <tatgdata.h>
#include "macroprc.h"

static UINT32_T             tat_MacroNumber;
static char           **tat_MacroStorage;

typedef enum
{
    OP_NE,
    OP_EQ,
    OP_LT,
    OP_LE,
    OP_GT,
    OP_GE
} tatm_OP;

typedef enum
{
    LOP_NONE = 0,
    LOP_AND,
    LOP_OR
} tatm_LP;


/***************************************************************************
 * Function : tatmacro_AddMacro
 *
 * Description: Add unique macro name to list
 *
 * Returns: None
 *
 * Notes: None
 *
 **************************************************************************/
static void tatmacro_AddMacro( char *m )
{
    UINT32_T                    i;

    for ( i = 0; i < tat_MacroNumber; i++ )
    {
        if ( !strcmp( m, tat_MacroStorage[i] ) )
            return;
    }

    tat_MacroStorage =
        MEM_Reallocate( tat_MacroStorage, ( tat_MacroNumber + 1 ) * sizeof ( tat_MacroStorage[0] ) );
    tat_MacroStorage[tat_MacroNumber] = MEM_CopyStr( m );
    tat_MacroNumber++;
}

/***************************************************************************
 * Function : tatmacro_Parse
 *
 * Description: parse string s according following grammar:
 *              <condition>::=<expression>[<oper><expression>] ;
 *              <expression>::=<relation> | ( <LPAREN> <expression> <RPAREN>) ;
 *              <relation> ::= <macro constant> <compare> [<macro constant> | <data>] ;
 *              <oper> :: = <OR><OR>  |  <AMPERSAND><AMPERSAND> ;
 *              <compare> ::= == | > | < | <= | >= | != ;
 *              <data> ::= <comment> | <float number> | <IDENTIFICATOR> ;
 *              if tat_ParseMode==0, generate C code to evaluate expression,
 *              if tat_ParseMode==1, extract macro names and check expression for correctness
 *              
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tatmacro_Parse( char **s, UINT32_T level )
{
    char                   *l;
    char                    l1[TAT_MAX_ID_LEN], l2[TAT_MAX_ID_LEN], op[TAT_MAX_ID_LEN], temp[TAT_MAX_ID_LEN];
    int                     expr = 0;  /* expression started */
    int                     comp_allowed = 1;

    l = *s;

    while ( *l )
    {
        StrSkipComments( &l );

        if ( *l == '(' )
        {
            if ( !comp_allowed )
                return RESULT_ERR;

            l++;

            if ( !tatmacro_Parse( &l, level + 1 ) )
                return RESULT_ERR;

            StrSkipComments( &l );

            if ( *l != ')' )
                return RESULT_ERR;

            l++;
            expr = 0;
            comp_allowed = 0;
        }
        else if ( *l == ')' )
        {
            if ( expr )
                return RESULT_ERR;

            *s = l;

            return ( LOGICAL ) ( level != 0 );
        }
        else if ( *l == '#' )
        {
            if ( !comp_allowed )
                return RESULT_ERR;

        /* start of macro expression */
            l1[0] = *l++;

            if ( !StrExtractLabel( &l, l1 + 1 ) || ( l1[1] == 0 ) )
                return RESULT_ERR;

            tatmacro_AddMacro( l1 );

            StrSkipComments( &l );

            if ( *l == '!' && *( l + 1 ) == '=' )
            {
                op[0] = '!';
                op[1] = '=';
                op[2] = 0;
                l += 2;
            }
            else if ( *l == '=' && *( l + 1 ) == '=' )
            {
                op[0] = '=';
                op[1] = '=';
                op[2] = 0;
                l += 2;
            }
            else if ( *l == '<' || *l == '>' )
            {
                op[0] = *l++;
                op[1] = 0;

                if ( *l == '=' )
                {
                    l++;
                    op[1] = '=';
                    op[2] = 0;
                }
            }
            else
                return RESULT_ERR;

            StrSkipComments( &l );

            if ( *l == '#' )
            {
                l2[0] = *l++;
                if ( !StrExtractLabel( &l, l2 + 1 ) || ( l2[1] == 0 ) )
                    return RESULT_ERR;
                tatmacro_AddMacro( l2 );
            }
            else if ( *l == '\"' )
            {
                if ( !StrExtractString( &l, '\"', temp ) || temp[0] == 0 )
                    return RESULT_ERR;
            }
            else
            {
            /* identifier or digit */

                if ( *l == '+' || *l == '-' || SymIsDigit( *l ) )
                {
                    if ( !StrExtractDigit( &l, temp ) || temp[0] == 0 )
                        return RESULT_ERR;
                }
                else
                {
                    if ( !StrExtractLabel( &l, temp ) || temp[0] == 0 )
                        return RESULT_ERR;
                }

                StrEmbedLineQuotes( l2, temp );
            };


            expr = 0;
            comp_allowed = 0;
        }
        else if ( *l == '|' && *( l + 1 ) == '|' )
        {
            if ( expr )
                return RESULT_ERR;

            l += 2;
            expr = 1;
            comp_allowed = 1;
            continue;
        }
        else if ( *l == '&' && *( l + 1 ) == '&' )
        {
            if ( expr )
                return RESULT_ERR;

            l += 2;
            expr = 1;
            comp_allowed = 1;
            continue;
        }
        else
            return RESULT_ERR;
    }

    *s = l;
    return ( LOGICAL ) ( ( expr == 0 ) && ( level == 0 ) );
}

/***************************************************************************
 * Function : TATMACRO_Extract
 *
 * Description: Extract macro names from expression
 *
 * Returns: RESULT_ERR/RESULT_OK; num-number of extracted name
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL TATMACRO_Extract( char *line, UINT32_T * num, char ***dest )
{
    LOGICAL                 res;

    tat_MacroStorage = NULL;
    tat_MacroNumber = 0;
    res = tatmacro_Parse( &line, 0 );

    if ( !res )
    {
        MEM_FreeStrArray( tat_MacroNumber, tat_MacroStorage );
    }
    else
    {
        *num = tat_MacroNumber;
        *dest = tat_MacroStorage;
    }

    return res;
}




static int tatmacro_Calc( char **s, UINT32_T level )
{
    char                   *l;
    char                    l1[TAT_MAX_ID_LEN], l2[TAT_MAX_ID_LEN], temp[TAT_MAX_ID_LEN];

    tatm_LP                 expr = LOP_NONE;  /* expression started */
    int                     comp_allowed = 1;
    MSC_MACRO_DATA          l1_data_Type = MSC_MACRODATA_UNDEFINED;
    int                     result = 0, lr, mnum;
    char                    * op1 = NULL, *op2=NULL;
    tatm_OP                 op;

    l = *s;

    while ( *l )
    {
        StrSkipComments( &l );

        if ( *l == '(' )
        {
            if ( !comp_allowed )
                return RESULT_ERR;

            l++;

            result  = tatmacro_Calc( &l, level + 1 );

            if ( result == -1 )
                return result;

            StrSkipComments( &l );

            if ( *l != ')' )
                return -1;

            l++;

            expr = 0;
            comp_allowed = 0;
        }
        else if ( *l == ')' )
        {
            if ( expr != LOP_NONE )
                return -1;

            *s = l;
            if (level == 0 )
            {
                return -1;
            }
            return result;
        }
        else if ( *l == '#' )
        {
            if ( !comp_allowed )
                return -1;

        /* start of macro expression */
            l++;
            if ( !StrExtractLabel( &l, l1 ) || ( l1[0] == 0 ) )
                return -1;

            mnum = MSC_GetSubstNumber(l1);

            /* get first value */
            op1 = MSC_StaticMacroTable[mnum].repl;
            l1_data_Type = MSC_StaticMacroTable[mnum].datatype;

            StrSkipComments( &l );

            /* determine operation type */
            if ( *l == '!' && *( l + 1 ) == '=' )
            {
                op = OP_NE;
                l += 2;
            }
            else if ( *l == '=' && *( l + 1 ) == '=' )
            {
                op = OP_EQ;
                l += 2;
            }
            else if ( *l == '<')
            {
                op = OP_LT;
                l++;
                if ( *l == '=' )
                {
                    l++;
                    op = OP_LE;
                }
            }
            else if (*l == '>' )
            {
                op = OP_GT;
                l++;
                if ( *l == '=' )
                {
                    l++;
                    op = OP_GE;
                }
            }
            else
                return -1;

            StrSkipComments( &l );

            if ( *l == '#' )
            {
                l++;

                if ( !StrExtractLabel( &l, l2 ) || ( l2[0] == 0 ) )
                    return -1;

                mnum = MSC_GetSubstNumber(l2);

                /* get second value */
                op2 = MSC_StaticMacroTable[mnum].repl;

                /* check datatype compliance */
                if ( l1_data_Type != MSC_StaticMacroTable[mnum].datatype )
                    return -1;
            }
            else if ( *l == '\"' )
            {

            /* string */

                if ( !StrExtractString( &l, '\"', temp ) || temp[0] == 0 )
                    return RESULT_ERR;

                strcpy(l2, temp);
                if ( ( l1_data_Type != MSC_MACRODATA_STRING ) &&
                     ( l1_data_Type != MSC_MACRODATA_UNDEF ) )
                {
                    return -1;
                }
                op2 = &l2;
            }
            else
            {
            /* identifier or digit */

                if ( *l == '+' || *l == '-' || SymIsDigit( *l ) )
                {
                    if ( !StrExtractDigit( &l, temp ) || temp[0] == 0 )
                        return -1;
                }
                else
                {
                    if ( !StrExtractLabel( &l, temp ) || temp[0] == 0 )
                        return -1;
                }

                strcpy(l2, temp);
                op2 = &l2;

                if ( l1_data_Type != MSC_MACRODATA_NUMBER &&
                    l1_data_Type != MSC_MACRODATA_UNDEF &&
                    StrIsAnyNumber( temp ) )
                {
                    return -1;
                }

                if ( l1_data_Type != MSC_MACRODATA_ID && l1_data_Type != MSC_MACRODATA_UNDEF &&
                    !StrIsAnyNumber( temp ) )
                {
                    return -1;
                }
            };

            lr = StrCompareSmart(op1, op2);
            switch (op)
            {
            case OP_NE:
                {
                    lr = (lr != 0);
                    break;
                }
            case OP_EQ:
                {
                    lr = (lr == 0);
                    break;
                }
            case OP_LE:
                {
                    lr = lr <= 0;
                    break;
                }
            case OP_LT:
                {
                    lr = lr < 0;
                    break;
                }
            case OP_GE:
                {
                    lr = lr >= 0;
                    break;
                }
            case OP_GT:
                {
                    lr = lr > 0;
                    break;
                }
            }

            switch (expr)
            {
            case LOP_NONE:
                {
                    result = lr;
                    break;
                }
            case LOP_AND:
                {
                    result = result && lr;
                    break;
                }
            case LOP_OR:
                {
                    result = result || lr;
                    break;
                }
            }

            expr = LOP_NONE;
            comp_allowed = 0;
        }
        else if ( *l == '|' && *( l + 1 ) == '|' )
        {
            if ( expr != LOP_NONE )
                return -1;

            l += 2;

            expr = LOP_OR;

            comp_allowed = 1;

            continue;
        }
        else if ( *l == '&' && *( l + 1 ) == '&' )
        {
            if ( expr != LOP_NONE)
                return -1;

            l += 2;

            expr = LOP_AND;

            comp_allowed = 1;

            continue;
        }
        else
            return -1;
    }

    *s = l;
    if (( ( expr == LOP_NONE ) && ( level == 0 ) ))
    {
        return result;
    }
    else
         return -1;
}


/***************************************************************************
 * Function : MSC_MacroCalc
 *
 * Description: calculate macro expression
 *
 * Returns: TRUE/FALSE/-1 if error
 *
 * Notes: None
 *
 **************************************************************************/
int MSC_MacroCalc(char * s)
{
    return tatmacro_Calc(&s, 0);
}
