#ifndef STRPROCESS_H
#define STRPROCESS_H
/******************************************************************************
*
*       COPYRIGHT 2001-2004 MOTOROLA, ALL RIGHTS RESERVED
*
*       MOTOROLA CONFIDENTIAL PROPRIETARY
*
* Description:  Command-line processing
*
* $RCSfile: strproc.h,v $
* $Revision: 1.4 $
* $Source: /home/cvsusr/cvssoft/tat_tools/baselib/include/strproc.h,v $
* $Date: 2004/03/05 14:37:40 $
*
*
* CHANGE HISTORY
*                                                       
* Initial version 
*
******************************************************************************/

#ifdef __cplusplus
extern                  "C"
{
#endif
#include <portable.h>

#ifndef SymIsSpace
#define SymIsSpace(c) ( ( unsigned char ) c < 33 )
#endif

#ifndef STR_SKIP_SPACES
#define STR_SKIP_SPACES(a) { register char * __t=a; while (*__t && SymIsSpace(*__t) ) __t++; a=__t; }
#endif

/***************************************************************************
 * Function : SymIsDelim
 *
 * Description: Check character to be one of delimeters
 *
 * Returns: 0/1
 *
 * Notes: None
 *
 **************************************************************************/
    extern int              SymIsDelim( const char c );

/***************************************************************************
 * Function : SymIsDigit
 *
 * Description: check symbol to be decimal digit
 *
 * Returns: 0/1
 *
 * Notes: None
 *
 **************************************************************************/
    extern int              SymIsDigit( const char c );

/***************************************************************************
 * Function : SymIsHexDigit
 *
 * Description: check symbol to be hexadecimal digit
 *
 * Returns: 0/1
 *
 * Notes: None
 *
 **************************************************************************/
    extern int              SymIsHexDigit( const char c );


/***************************************************************************
 * Function : StrParseCComment
 *
 * Description: extract string from C-style comments
 *
 * Returns: RESULT_ERR/RESULT_OK, string saved in 'dest', *line points to end of comment
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          StrParseCComment( char * * line, char * dest );

/***************************************************************************
 * Function : StrSkipComments
 *
 * Description: skip spaces and C-style comments
 *
 * Returns: modified pointer
 *
 * Notes: original pointer shall be saved
 *
 **************************************************************************/
    extern LOGICAL          StrSkipComments( char * * line );

/***************************************************************************
 * Function : StrMSCSkipComments
 *
 * Description: skip spaces and C-style comments, but not MSC-source references
 *
 * Returns: modified pointer
 *
 * Notes: original pointer shall be saved
 *
 **************************************************************************/
    extern LOGICAL          StrSkipMSCComments( char * * line );

/***************************************************************************
 * Function : StrExtractString
 *
 * Description: extract string within specified delimiters
 *              doubling of delimiters treated as copying delimiter once
 *
 * Returns: RESULT_ERR/RESULT_OK, extracted string saved in 'dest'
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          StrExtractString( char * * line, char p, char * dest );

/***************************************************************************
 * Function : StrExtractCString
 *
 * Description: extract C style string within quotes
 *
 * Returns: RESULT_ERR/RESULT_OK, extracted string saved in 'dest'
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          StrExtractCString( char * * line, char * dest );

/***************************************************************************
 * Function : StrExtractXMLString
 *
 * Description: extract C style string within quotes
 *
 * Returns: RESULT_ERR/RESULT_OK, extracted string saved in 'dest'
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          StrExtractXMLString( char * * line, char * dest );

/***************************************************************************
 * Function : StrFindCloseBrace
 *
 * Description: find closing brace
 *
 * Returns: 
 *
 * Notes: None
 *
 **************************************************************************/
    extern char            *StrFindCloseBrace( char * s );

/***************************************************************************
 * Function : StrReplace
 *
 * Description: StrReplace occurences of 'subs' with 'repl'
 *
 * Returns: number of replaces
 *
 * Notes: None
 *
 **************************************************************************/
    extern int              StrReplace( char * src, const char * subst, const char * repl );

/***************************************************************************
 * Function : StrTrimSpaces
 *
 * Description: remove first and trail spaces
 *
 * Returns: string without first and trail spaces
 *
 * Notes: None
 *
 **************************************************************************/
    extern char            *StrTrimSpaces( char * s );

/***************************************************************************
 * Function : StrCleanEOL
 *
 * Description: remove trail EOL characters
 *
 * Returns: 
 *
 * Notes: None
 *
 **************************************************************************/
    extern char            *StrCleanEOL( char * line );

/***************************************************************************
 * Function : StrReplaceLex
 *
 * Description: Replace 'whole word' occurences of 'subs' with 'repl'
 *
 * Returns: number of replaces
 *
 * Notes: None
 *
 **************************************************************************/
    extern int              StrReplaceLex( char * dest, char * src, const char * subs, const char * repl );

/***************************************************************************
 * Function : StrEmbedLine
 *
 * Description: Replace special characters with codes
 *
 * Returns: string without illegal characters
 *
 * Notes: None
 *
 **************************************************************************/
    extern char            *StrEmbedLine( char * d, char * s );

/***************************************************************************
 * Function : StrEmbedXMLLine
 *
 * Description: Replace special characters with codes
 *
 * Returns: string without illegal characters
 *
 * Notes: None
 *
 **************************************************************************/
    extern char            *StrEmbedXMLLine( char * d, char * s );

/***************************************************************************
 * Function : StrEmbedLineQuotes
 *
 * Description: Replace special characters with codes and embrace string in quotes
 *
 * Returns: string without illegal characters
 *
 * Notes: None
 *
 **************************************************************************/
    extern char            *StrEmbedLineQuotes( char * d, char * s );

/***************************************************************************
 * Function : StrEmbedXMLLineQuotes
 *
 * Description: Replace special characters with codes and embrace string in quotes
 *
 * Returns: string without illegal characters
 *
 * Notes: None
 *
 **************************************************************************/
    extern char            *StrEmbedXMLLineQuotes( char * d, char * s );

/***************************************************************************
 * Function : StrMatch
 *
 * Description: Match string against wildcard
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          StrMatch( const char * name, const char * mask );

/***************************************************************************
 * Function : StrGetIndex
 *
 * Description: return index of string in array
 *
 * Returns: string
 *
 * Notes: None
 *
 **************************************************************************/
    extern int              StrGetIndex( const UINT32_T dim, const char * * S_AR, const char * s );

/***************************************************************************
 * Function : StrUnbrace
 *
 * Description: remove symmetrical braces
 *
 * Returns: string
 *
 * Notes: None
 *
 **************************************************************************/
    extern size_t              StrUnbrace( char * * l );

/***************************************************************************
 * Function : StrIsNumber
 *
 * Description: checks wherever string is floating number
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          StrIsNumber( const char * s );

/***************************************************************************
 * Function : StrIsAnyNumber
 *
 * Description: checks wherever string is float number or hexadecimal number
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          StrIsAnyNumber( const char * s );

/***************************************************************************
* Function : StrIsIntegerNumber
*
* Description: checks where string is integer number
*
* Returns: RESULT_ERR/RESULT_OK
*
* Notes: None
*
**************************************************************************/
    extern LOGICAL          StrIsIntegerNumber( const char * s );

/***************************************************************************
* Function : StrIsNaturalNumber
*
* Description: checks where string is natural (positive integer) number
*
* Returns: RESULT_ERR/RESULT_OK
*
* Notes: None
*
**************************************************************************/
    extern LOGICAL          StrIsNaturalNumber( const char * s );

/***************************************************************************
* Function : StrFindCloseBraceEx
*
* Description: find closing brace
*
* Returns: pointer to symbol after end / NULL
*
* Notes: None
*
**************************************************************************/
    extern char            *StrFindCloseBraceEx( char * s, int flags );

#define STR_USE_PARENTHESIS         0x00000001
#define STR_USE_SQUARE_BRACKET      0x00000002
#define STR_USE_ANGLE_BRACKET       0x00000004
#define STR_USE_CURLY_BRACES        0x00000008

/***************************************************************************
 * Function : StrCheckCName
 *
 * Description: check c function name for correctness
 *
 * Returns: RESULT_OK/RESULT_ERR
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          StrCheckCName( char * s );

/***************************************************************************
 * Function : StrCheckIDName
 *
 * Description: check identifier  name for correctness
 *
 * Returns: RESULT_OK/RESULT_ERR
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          StrCheckIDName( char * s );

/***************************************************************************
 * Function : StrExtractMacroID
 *
 * Description: extracts <MACRO_IDENTIFICATOR>
 *
 * Returns: RESULT_OK/RESULT_ERR
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          StrExtractMacroID( char * * s, char * params );

/***************************************************************************
 * Function : StrExtractLabel
 *
 * Description: extract text identifier
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          StrExtractLabel( char * * s, char * params );

/***************************************************************************
* Function : StrExtractDigit
*
* Description: extract a decimal/hexadecimal digit from line
*
* Returns: RESULT_ERR/RESULT_OK, filled params
*
* Notes: None
*
**************************************************************************/
    extern LOGICAL          StrExtractDigit( char * * line, char * params );

/***************************************************************************
 * Function : StrRemoveDupSpace
 *
 * Description: compress duplicate spaces in single
 *
 * Returns: compressed string
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          StrRemoveDupSpace( char * dest, char * src );

/***************************************************************************
 * Function : StrHash
 *
 * Description: calculates hash number for string
 *
 * Returns: number
 *
 * Notes: None
 *
 **************************************************************************/
    extern UINT32_T             StrHash( const char * str );

/***************************************************************************
 * Function : SymIsExprDelim
 *
 * Description: checks wherever symbol is delimeter of expressions
 *
 * Returns: RESULT_OK/RESULT_ERR
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          SymIsExprDelim( const char c );

/***************************************************************************
 * Function : StrExtractExpr
 *
 * Description: extract complex expression
 *
 * Returns: RESULT_OK/RESULT_ERR
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          StrExtractExpr( char * * s, char * params );

/***************************************************************************
 * Function : StrToDouble
 *
 * Description: extends atoi by hexadecimal numbers
 *
 * Returns: 
 *
 * Notes: None
 *
 **************************************************************************/
    extern double           StrToDouble( char * s );

/***************************************************************************
 * Function : StrCompareSmart
 *
 * Description: compares numbers as strings or strings
 *
 * Returns: -1/0/1
 *
 * Notes: None
 *
 **************************************************************************/
    extern int              StrCompareSmart( char * s1, char * s2 );

/***************************************************************************
 * Function : StrCheckSDLName
 *
 * Description: check if string is compliant with SDL naming conventions
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          StrCheckSDLName( char * s );

/***************************************************************************
 * Function : StrToUpper
 *
 * Description: convert string to upper case and return length
 *
 * Returns: 
 *
 * Notes: None
 *
 **************************************************************************/
    extern int              StrToUpper( char * target );

/***************************************************************************
 * Function : StrUpCopy
 *
 * Description: copy src to target in upper-case
 *
 * Returns: target
 *
 * Notes: None
 *
 **************************************************************************/
    extern char            *StrUpCopy( char * target, const char * src );

/***************************************************************************
 * Function : StrEmbedMSCtext
 *
 * Description: Embed MSC text in ''
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          StrEmbedMSCtext( char * dest, char * src );

/***************************************************************************
 * Function : SymIsAlpha
 *
 * Description: Check character to be one of alphabetic
 *
 * Returns: 0/1
 *
 * Notes: None
 *
 **************************************************************************/
    extern int              SymIsAlpha( const char c );

/***************************************************************************
 * Function : SymToUpper
 *
 * Description: convert to upper-case
 *
 * Returns: upper-case character
 *
 * Notes: None
 *
 **************************************************************************/
    extern char             SymToUpper( const char c );

/***************************************************************************
 * Function : StrICmp
 *
 * Description: portable wrapper for stricmp
 *
 * Returns: -1/0/1
 *
 * Notes: None
 *
 **************************************************************************/
    extern int              StrICmp( const char * c1, const char * c2 );

/***************************************************************************
 * Function : StrExtractFLabel
 *
 * Description: extract file-name like text identifier
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL StrExtractFLabel( char * * s, char * params );

/***************************************************************************
 * Function : StrExtractMSCString
 *
 * Description: extract string within specified delimiters
 *              doubling of delimiters treated as copying delimiter once
 *
 * Returns: RESULT_ERR/RESULT_OK, extracted string saved in 'dest'
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL StrExtractMSCString( char * * line, char * dest );

    /***************************************************************************
    * Function : StrToInt
    *
    * Description: extends atoi by hexadecimal numbers
    *
    * Returns: 
    *
    * Notes: None
    *
    **************************************************************************/
    extern int StrToInt( char * s );

/***************************************************************************
 * Function : StrCheck01
 *
 * Description: check if string is (0/1)
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL StrCheck01(const char * str);

#ifdef __cplusplus
}

#endif                                 /* C++ */
#endif                                 /* STRPROCESS_H */
