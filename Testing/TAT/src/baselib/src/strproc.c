/******************************************************************************
*
*       COPYRIGHT 2001-2004 MOTOROLA, ALL RIGHTS RESERVED
*
*       MOTOROLA CONFIDENTIAL PROPRIETARY
*
* Description:  Command-line processing
*
* $RCSfile: strproc.c,v $
* $Revision: 1.4 $
* $Source: /home/cvsusr/cvssoft/tat_tools/baselib/src/strproc.c,v $
* $Date: 2004/03/05 14:37:44 $
*
*
* CHANGE HISTORY
*                                                       
* Initial version 
*
******************************************************************************/
#include <portable.h>
#include <qmemory.h>
#include <strproc.h>
#include <mscstrs.h>
#include <mscbase.h>
#include <tatgdata.h>

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
int SymIsAlpha( const char c )
{
    return ( ( c >= 'A' && c <= 'Z' ) || ( c >= 'a' && c <= 'z' ) );
}

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
char SymToUpper( const char c )
{
    return ( c >= 'a' && c <= 'z' ) ? ( char ) ( c - ( char ) 'a' + ( char ) 'A' ) : c;
}

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
int SymIsDelim( const char c )
{
    return ( ( ( unsigned char ) c < 48 ) || ( c > 57 && c < 65 ) || ( c > 90 && c < 97 ) ||
        ( ( unsigned char ) c > 122 && ( unsigned char ) c < 128 ) );
}

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
int SymIsDigit( const char c )
{
    return ( c >= '0' && c <= '9' );
}

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
int SymIsHexDigit( const char c )
{
    return ( c >= '0' && c <= '9' ) || ( c >= 'A' && c <= 'F' ) || ( c >= 'a' && c <= 'f' );
}


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
LOGICAL StrParseCComment( char * * line, char * dest )
{
    char                   *s;
    int                     i;

    s = *line;

    if ( *s != '/' )
        return RESULT_OK;

    if ( *( s + 1 ) != '*' )
        return RESULT_OK;

    s += 2;

    STR_SKIP_SPACES( s );

    i = 0;

    if ( dest )
    {
        while ( *s && !( *s == '*' && *( s + 1 ) == '/' ) )
            dest[i++] = *s++;

        while ( i > 0 && SymIsSpace( dest[i - 1] ) )
            i--;

        dest[i] = 0;
    }
    else
    {
        while ( *s && !( *s == '*' && *( s + 1 ) == '/' ) )
            s++;
    }

    if ( *s == 0 )
    {
        *line = s;
        return RESULT_ERR;
    }

    s += 2;
    *line = s;
    return RESULT_OK;
}

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
LOGICAL StrSkipComments( char * * line )
{
    char                   *s;

    if ( !line )
        return RESULT_ERR;

    s = *line;

    if ( !s )
        return RESULT_ERR;

    STR_SKIP_SPACES( s );

    while ( *s && *s == '/' && *( s + 1 ) == '*' )
    {
        if ( !StrParseCComment( &s, NULL ) )
            return RESULT_ERR;

        STR_SKIP_SPACES( s );
    }

    *line = s;
    return RESULT_OK;
}

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
LOGICAL StrSkipMSCComments( char * * line )
{
    char                   *s, *p;

    if ( !line )
        return RESULT_ERR;

    s = *line;

    if ( !s )
        return RESULT_ERR;

    STR_SKIP_SPACES( s );

/* check for non-source-reference comments */
    while ( *s && *s == '/' && *( s + 1 ) == '*' )
    {
        if ( *( s + 2 ) == '[' )
        {
            p = s;
            if ( !StrParseCComment( &s, NULL ) )
            {
                return RESULT_ERR;
            }
            if ( *( s - 3 ) == ']' )
            {
                *line = p;
                return RESULT_OK;
            }
        }
        else
        {
            if ( !StrParseCComment( &s, NULL ) )
                return RESULT_ERR;
        }
        STR_SKIP_SPACES( s );
    }

    *line = s;
    return RESULT_OK;
}

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
LOGICAL StrExtractString( char * * line, char p, char * dest )
{
    char                   *s;

    if ( !line )
        return RESULT_ERR;

    s = *line;

    if ( !s )
        return RESULT_ERR;

    if ( *s != p )
        return RESULT_OK;

    if ( dest )
    {
        *dest++ = *s++;

        while ( *s )
        {
            while ( *s && *s != p )
                *dest++ = *s++;

            if ( ( *s == p ) && *( s + 1 ) == p )
            {
                *dest++ = p;
                s += 2;
            }
            else
                break;
        }

        *dest = 0;

        if ( *s == 0 )
            return RESULT_ERR;

        *dest++ = *s++;

        *dest = 0;
    }
    else
    {
        s++;

        while ( *s )
        {
            while ( *s && *s != p )
                s++;

            if ( ( *s == p ) && *( s + 1 ) == p )
            {
                s += 2;
            }
            else
                break;
        }

        if ( *s == 0 )
            return RESULT_ERR;

        s++;
    }

    *line = s;
    return RESULT_OK;
}

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
LOGICAL StrExtractCString( char * * line, char * dest )
{
    char                   *s;

    if ( !line )
        return RESULT_ERR;

    s = *line;

    if ( !s )
        return RESULT_ERR;

    if ( *s != '\"' )
        return RESULT_OK;

    s++;

    if ( dest )
    {
        while ( *s && *s != '\"' )
        {
            if ( *s == '\\' )
            {
                s++;

                if ( *s == 'n' )
                    *dest++ = '\n';
                else if ( *s == 't' )
                    *dest++ = '\t';
                else
                    *dest++ = *s;

                s++;
            }
            else
                *dest++ = *s++;
        };

        *dest = 0;
    }
    else
    {
        while ( *s && *s != '\"' )
        {
            if ( *s == '\\' )
            {
                s += 2;
            }
            else
                s++;
        };
    }

    if ( *s != '\"' )
        return RESULT_ERR;

    *line = s + 1;

    return RESULT_OK;
}

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
LOGICAL StrExtractXMLString( char * * line, char * dest )
{
    char                   *s, *b, ends, label[TAT_MAX_ID_LEN];
    UINT32_T                    character;

    if ( !line )
        return RESULT_ERR;

    s = *line;

    if ( !s )
        return RESULT_ERR;

    if ( *s != '\"' && *s != '\'' )
    {
        *dest = 0;
        return RESULT_OK;
    };

    ends = *s++;

    if ( dest )
    {
        while ( *s && *s != ends )
        {
            if ( *s == '<' || *s == '>' )
                return RESULT_ERR;

            if ( *s == '&' )
            {
                s++;

                if ( *s == 'l' && *( s + 1 ) == 't' && *( s + 2 ) == ';' )
                {
                    *dest++ = '<';
                    s += 3;
                }
                else if ( *s == 'g' && *( s + 1 ) == 't' && *( s + 2 ) == ';' )
                {
                    *dest++ = '>';
                    s += 3;
                }
                else if ( *s == 'a' && *( s + 1 ) == 'm' && *( s + 2 ) == 'p' && *( s + 3 ) == ';' )
                {
                    *dest++ = '&';
                    s += 4;
                }
                else if ( *s == 'a' && *( s + 1 ) == 'p' && *( s + 2 ) == 'o' && *( s + 3 ) == 's' &&
                    *( s + 4 ) == ';' )
                {
                    *dest++ = '\'';
                    s += 5;
                }
                else if ( *s == 'q' && *( s + 1 ) == 'u' && *( s + 2 ) == 'o' && *( s + 3 ) == 't' &&
                    *( s + 4 ) == ';' )
                {
                    *dest++ = '\"';
                    s += 5;
                }
                else if ( *s == '#' )
                {
                    if ( *( s + 1 ) == 'x' )
                    {
                        *s = '0';
                        b = s;

                        if ( !StrExtractLabel( &b, label ) )
                            return RESULT_ERR;

                        if ( !StrIsAnyNumber( label ) )
                            return RESULT_ERR;

                        *s = '#';

                        s = b;
                    }
                    else
                    {
                        s++;

                        if ( !StrExtractLabel( &s, label ) )
                            return RESULT_ERR;

                        if ( !StrIsIntegerNumber( label ) )
                            return RESULT_ERR;
                    }

                    if ( *s != ';' )
                        return RESULT_ERR;

                    s++;

                    character = ( UINT32_T ) StrToInt( label );

                    if ( character < 33 )
                        character = ' ';

                    if ( character > 127 )
                        character = '?';

                    *dest++ = ( char ) character;
                }
            }
            else if ( *s < 33 )
            {
                *dest++ = ' ';
                s++;
            }
            else
                *dest++ = *s++;
        };

        *dest = 0;
    }
    else
    {
        while ( *s && *s != ends )
            s++;
    }

    if ( *s != ends )
        return RESULT_ERR;

    *line = s + 1;

    return RESULT_OK;
}

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
char                   *StrFindCloseBrace( char * s )
{
    int                     c;

    if ( !s )
        return NULL;

    if ( *s == 0 )
        return ( char * ) s;

    c = 0;

    while ( *s )
    {
        if ( *s == '(' )
            c++;

        if ( *s == ')' )
            c--;

        if ( *s == '\'' )
        {
            if ( !StrExtractString( &s, '\'', NULL ) )
                return NULL;
            s--;
        }

        if ( *s == '"' )
        {
            if ( !StrExtractString( &s, '"', NULL ) )
                return NULL;
            s--;
        }

        if ( *s == '/' && *( s + 1 ) == '*' )
        {
            if ( !StrParseCComment( &s, NULL ) )
                return NULL;
            s--;
        }

        if ( c == 0 )
            break;

        s++;
    }

    if ( c != 0 )
        return NULL;

    if ( *s == ')' )
        s++;

    return ( char * ) s;
}


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
int StrReplace( char * src, const char * subst, const char * repl )
{
    size_t                     sl, sbl, rpl;
    int                     p;
    char                    first, *d;
    int                     r;

    if ( !src || !subst || !repl )
        return 0;

    r = 0;

    sl = strlen( src );
    sbl = strlen( subst );

    if ( sl < sbl )
        return 0;

    rpl = strlen( repl );
    first = subst[0];
    d = src;

    while ( *src )
    {
        while ( ( *src ) && ( *src != first ) )
            src++;

        p = strncmp( src, subst, sbl );

        if ( p == 0 )
        {
            memmove( src + rpl, src + sbl, sl - ( src - d ) );
            memmove( src, repl, rpl );
            src += rpl;
            r++;
        }
        else if ( *src )
            src++;
    }

    *src = 0;
    return r;
}

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
char * StrTrimSpaces( char * s )
{
    size_t                     i, k;

    if ( s )
    {
        i = 0;

        while ( s[i] && SymIsSpace( s[i] ) )
            i++;

        if ( s[i] == 0 )
        {
            *s = 0;
            return s;
        }

        k = strlen( s + i ) + i;    /* same as strlen(s)-1, but slightly faster */

        while ( k > 0 && SymIsSpace( s[k - 1] ) )
            k--;

        if ( k > 0 )
            s[k] = 0;

        memmove( s, s + i, k - i + 1 );
    }

    return s;
}

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
char                   *StrCleanEOL( char * line )
{
    size_t                     i;

    if ( line )
    {
        i = strlen( line );

        if ( i > 0 )
        {
            i--;

            while ( i > 0 && ( line[i] == CR || line[i] == LF ) )
                i--;

            line[i + 1] = 0;
        }
    }

    return line;
}

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
int StrReplaceLex( char * dest, char * src, const char * subs, const char * repl )
{
    size_t                     sl, sbl, rpl;
    int                        p;
    char                    first;
    int                     r;

    if ( !dest || !src )
        return 0;

    if ( !subs || !repl )
    {
        strcpy( dest, src );
        return 0;
    }

    r = 0;
    sl = strlen( src );
    sbl = strlen( subs );
/* subs > src */

    if ( sl < sbl )
    {
        memcpy( dest, src, sl + 1 );
        return 0;
    }

    rpl = strlen( repl );
    first = *subs;

    while ( *src )
    {
        while ( ( *src ) && ( *src != first ) )
        {
            *dest++ = *src++;
        }

        p = strncmp( src, subs, sbl );

        if ( ( p == 0 ) && SymIsDelim( *( src + sbl ) ) )
        {
            memcpy( dest, repl, rpl );
            src += sbl;
            dest += rpl;
            r++;
        }
        else
        {
            if ( *src )
            {
                src++;
                *dest++ = first;
            }
        }
    }

    *dest = 0;
    return r;
}

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
char                   *StrEmbedLine( char * d, char * s )
{
    if ( !d )
        return d;

    if ( !s )
    {
        *d = 0;
        return d;
    };

    while ( *s )
    {
        switch ( *s )
        {

        case '\n':
        {
            *d++ = '\\';
            *d++ = 'n';
            break;
        }

        case '\"':
        {
            *d++ = '\\';
            *d++ = '\"';
            break;
        }

        case '\\':
        {
            *d++ = '\\';
            *d++ = '\\';
            break;
        }

        case '$':
        {
            *d++ = '\\';
            *d++ = '$';
            break;
        }

        case '[':
        {
            *d++ = '\\';
            *d++ = '[';
            break;
        }

        case ']':
        {
            *d++ = '\\';
            *d++ = ']';
            break;
        }

        case '{':
        {
            *d++ = '\\';
            *d++ = '{';
            break;
        }

        case '}':
        {
            *d++ = '\\';
            *d++ = '}';
            break;
        }

        case '\t':
        {
            *d++ = '\\';
            *d++ = 't';
            break;
        }

        default:
            *d++ = *s;
        }

        s++;
    }

    *d = 0;
    return d;
}

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
char                   *StrEmbedXMLLine( char * d, char * s )
{
    if ( !d )
        return d;

    if ( !s )
    {
        *d = 0;
        return d;
    };

    while ( *s )
    {
        if ( *s == '\"' )
        {
            *d++ = '&';
            *d++ = 'q';
            *d++ = 'u';
            *d++ = 'o';
            *d++ = 't';
            *d++ = ';';
        }
        else if ( *s == '<' )
        {
            *d++ = '&';
            *d++ = 'l';
            *d++ = 't';
            *d++ = ';';
        }
        else if ( *s == '>' )
        {
            *d++ = '&';
            *d++ = 'g';
            *d++ = 't';
            *d++ = ';';
        }
        else if ( *s == '&' )
        {
            *d++ = '&';
            *d++ = 'a';
            *d++ = 'm';
            *d++ = 'p';
            *d++ = ';';
        }
        else if ( *s == '\'' )
        {
            *d++ = '&';
            *d++ = 'a';
            *d++ = 'p';
            *d++ = 'o';
            *d++ = 's';
            *d++ = ';';
        }
        else if ( *s >= 32 && *s <= 126 )
        {
            *d++ = *s;
        }
        else
        {
            *d++ = '&';
            *d++ = '#';
            sprintf( d, "%u", ( char ) *s );
            d += strlen( d );
            *d++ = ';';
        }

        s++;
    }

    *d = 0;
    return d;
}

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
char                   *StrEmbedLineQuotes( char * d, char * s )
{
    if ( !d )
        return NULL;

    *d++ = '\"';
    d = StrEmbedLine( d, s );
    *d++ = '\"';
    *d = 0;

    return d;
}

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
char                   *StrEmbedXMLLineQuotes( char * d, char * s )
{
    if ( !d )
        return NULL;

    *d++ = '\"';
    d = StrEmbedXMLLine( d, s );
    *d++ = '\"';
    *d = 0;

    return d;
}

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
LOGICAL StrMatch( const char * name, const char * mask )
{
    int                     i, last, im;

    for ( i = 0; mask[i] == name[i] || mask[i] == '?'; i++ )
        if ( name[i] == 0 )
            return ( mask[i] == 0 );

    if ( mask[i] != '*' )
        return RESULT_ERR;

    for ( last = i + 1, im = last;; )
    {
        if ( mask[im] == '*' )
        {
            if ( mask[++im] == 0 )
                return RESULT_OK;

            last = im;

            continue;
        }

        if ( name[i] == 0 )
            return ( mask[im] == 0 );

        if ( mask[im] != name[i] && mask[im] != '?' )
            i -= im - last, im = last;
        else
            ++im;

        ++i;
    }
}

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
int StrGetIndex( const UINT32_T dim, const char * * S_AR, const char * s )
{
    UINT32_T                    i;

    if ( !s )
        return -1;

    for ( i = 0; i < dim; i++ )
    {
        if ( !stricmp( S_AR[i], s ) )
            return i;
    }

    return -1;
}

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
size_t StrUnbrace( char * * l )
{
    size_t                  len;
    char                   *s;

    if ( !l )
        return (size_t)-1;

    s = *l;

    if ( s[0] == 0 )
        return 0;

    len = strlen( s );

    while ( s[0] == '(' && s[len - 1] == ')' && s[1] != '.' && StrFindCloseBrace( s ) == s + len )
    {
        s[len - 1] = 0;
        s++;
        len -= 2;
    }

    *l = s;
    return len;
}

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
LOGICAL StrIsNumber( const char * s )
{
    size_t   point = 0, i = 0, epoint = 0;

    if ( !s || *s == 0 )
        return RESULT_ERR;

/* skip first possible minus */
    if ( s[i] == '-' || s[i] == '+' )
    {
        i++;
    }

    while ( s[i] )
    {
        if (s[i]=='E' || s[i]=='e')
        {
            if (epoint)
                return RESULT_ERR;

            epoint = 1;
            if (s[i+1]=='-' || s[i+1]=='+')
                i++;
        } else if ( s[i] == '.' )
        {
        /* check for duplicated point */

            if ( point || epoint)
                return RESULT_ERR;

            point = 1;
        }
        else if ( !SymIsDigit( s[i] ) )
        {
            return RESULT_ERR;
        }

        i++;

        if ( i > TAT_MAX_ID_LEN )
            return RESULT_ERR;
    }

    return RESULT_OK;
}

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
LOGICAL StrIsAnyNumber( const char * s )
{
    size_t     point = 0, hex = 0, i = 0, epoint=0;

    if ( !s || *s == 0 )
        return RESULT_ERR;

/* skip first possible minus */
    if ( s[i] == '-' || s[i] == '+' )
        i++;

    if ( s[i] == '0' && s[i + 1] == 'x' )
    {
        hex = 1;
        i += 2;
    };

    while ( s[i] )
    {
        if (!hex && (s[i]=='E' || s[i]=='e'))
        {
            if (epoint)
                return RESULT_ERR;

            epoint=1;
            if (s[i+1]=='-' || s[i+1]=='+')
                i++;
        }
        if ( s[i] == '.' )
        {
        /* check for duplicated point */
            if ( point || hex || epoint)
                return RESULT_ERR;

            point = 1;
        }
        else if ( !SymIsDigit( s[i] ) )
        {                       /* check for hexadecimal digit */

            if ( !( hex && ( ( s[i] >= 'a' && s[i] <= 'f' ) || ( s[i] >= 'A' && s[i] <= 'F' ) ) ) )
                return RESULT_ERR;
        }

        i++;

        if ( i >= TAT_MAX_ID_LEN )
            return RESULT_ERR;
    }

    return RESULT_OK;
}

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
LOGICAL StrIsIntegerNumber( const char * s )
{
    size_t                    hex = 0, i = 0;

    if ( !s || *s == 0 )
        return RESULT_ERR;

/* skip first possible minus */
    if ( s[i] == '-' || s[i] == '+' )
        i++;

    if ( s[i] == '0' && s[i + 1] == 'x' )
    {
        hex = 1;
        i += 2;
    };

    while ( s[i] )
    {
        if ( !SymIsDigit( s[i] ) )
        {                       /* check for hexadecimal digit */

            if ( !( hex && ( ( s[i] >= 'a' && s[i] <= 'f' ) || ( s[i] >= 'A' && s[i] <= 'F' ) ) ) )
                return RESULT_ERR;
        }

        i++;
        if ( i > TAT_MAX_ID_LEN )
            return RESULT_ERR;
    }

    return RESULT_OK;
}

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
LOGICAL StrIsNaturalNumber( const char * s )
{
    size_t                    i = 0, hex = 0;

    if ( !s || *s == 0 )
        return RESULT_ERR;

/* skip first possible minus */
    if ( s[i] == '+' )
        i++;

    if ( s[i] == '0' && s[i + 1] == 'x' )
    {
        hex = 1;
        i += 2;
    };

    while ( s[i] )
    {
        if ( !SymIsDigit( s[i] ) )
        {                       /* check for hexadecimal digit */

            if ( !( hex && ( ( s[i] >= 'a' && s[i] <= 'f' ) || ( s[i] >= 'A' && s[i] <= 'F' ) ) ) )
                return RESULT_ERR;
        }

        i++;
        if ( i > TAT_MAX_ID_LEN )
            return RESULT_ERR;
    }

    return RESULT_OK;
}

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
char                   *StrFindCloseBraceEx( char * s, int flags )
{
    char                    stack[TAT_MAX_ID_LEN];
    int                     stack_point = 0;

    if ( !s || ( *s == 0 ) )
        return s;

    while ( *s )
    {
        switch ( *s )
        {
        case '(':
        {
            if ( flags & STR_USE_PARENTHESIS )
            {
                stack[stack_point++] = *s;
            }
            break;
        }
        case '[':
        {
            if ( flags & STR_USE_SQUARE_BRACKET )
            {
                stack[stack_point++] = *s;
            }
            break;
        }
        case '<':
        {
            if ( flags & STR_USE_ANGLE_BRACKET )
            {
                stack[stack_point++] = *s;
            }
            break;
        }
        case '{':
        {
            if ( flags & STR_USE_CURLY_BRACES )
            {
                stack[stack_point++] = *s;
            }
            break;
        }
        case '}':
        {
            if ( flags & STR_USE_CURLY_BRACES )
            {
                if ( !stack_point || stack[--stack_point] != '{' )
                    return NULL;
            }
            break;
        }
        case '>':
        {
            if ( flags & STR_USE_ANGLE_BRACKET )
            {
                if ( !stack_point || stack[--stack_point] != '<' )
                    return NULL;
            }
            break;
        }
        case ']':
        {
            if ( flags & STR_USE_SQUARE_BRACKET )
            {
                if ( !stack_point || stack[--stack_point] != '[' )
                    return NULL;
            }
            break;
        }
        case ')':
        {
            if ( flags & STR_USE_PARENTHESIS )
            {
                if ( !stack_point || stack[--stack_point] != '(' )
                    return NULL;
            }
            break;
        }
        case '\'':
        {
            if ( !StrExtractString( &s, '\'', NULL ) )
                return NULL;
            s--;
            break;
        }
        case '"':
        {
            if ( !StrExtractString( &s, '"', NULL ) )
                return NULL;
            s--;
            break;
        }
        case '/':
        {
            if ( *( s + 1 ) == '*' )
            {
                if ( !StrParseCComment( &s, NULL ) )
                    return NULL;
                s--;
            };
            break;
        }
        }                       /* switch */

        if ( !stack_point )
            break;
        s++;
    }

    if ( ( flags & STR_USE_PARENTHESIS ) && ( *s == ')' ) )
        s++;
    else if ( ( flags & STR_USE_SQUARE_BRACKET ) && ( *s == ']' ) )
        s++;
    else if ( ( flags & STR_USE_ANGLE_BRACKET ) && ( *s == '>' ) )
        s++;
    else if ( ( flags & STR_USE_CURLY_BRACES ) && ( *s == '}' ) )
        s++;

    return ( stack_point ) ? NULL : s;
}

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
LOGICAL StrCheckCName( char * s )
{
    size_t                    length = 0;

    if ( !s )
        return RESULT_ERR;

    if ( SymIsDigit( *s ) )
        return RESULT_ERR;

    while ( *s && ( *s == '_' || !SymIsDelim( *s ) ) )
    {
        s++;
        length++;

        if ( length >= TAT_MAX_ID_LEN )
            return RESULT_ERR;
    }

    return ( *s == 0 ) ? RESULT_OK : RESULT_ERR;
}

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
LOGICAL StrCheckIDName( char * s )
{
    size_t                    macro = 0;
    char                   *p;

    if ( !s )
        return RESULT_ERR;

    p = s;                      /* store original position */

/* ID name shouldn't start with digit */
    if ( SymIsDigit( *s ) || ( SymIsDelim( *s ) && *s != MSC_MACRO_START ) )
        return RESULT_ERR;

    s++;

    while ( *s && ( *s == '_' || *s == MSC_MACRO_START || !SymIsDelim( *s ) || ( macro && *s == '(' ) ) )
    {
        if ( *s == '#' )
            macro = 1;

    /* macro has parameters */
        if ( *s == '(' )
        {
            s = StrFindCloseBrace( s );
            macro = 0;

            if ( !s )
                return RESULT_ERR;
        }
        else
            s++;

        if ( ( UINT32_T ) ( s - p ) > TAT_MAX_ID_LEN )
            return RESULT_ERR;
    }

    return ( ( *s == 0 ) && ( ( UINT32_T ) ( s - p ) < TAT_MAX_ID_LEN ) ) ? RESULT_OK : RESULT_ERR;
}

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
LOGICAL StrExtractMacroID( char * * s, char * params )
{
    char                   *line = *s, *b;
    size_t                  macro = 0;
    size_t                  length = 0;

    StrSkipComments( &line );

    if ( !params )
        return RESULT_ERR;

    *params = 0;

/* identifier name can't start with digit */
    if ( SymIsDigit( *line ) )
    {
        *s = line;
        return RESULT_ERR;
    }

    while ( *line && ( !SymIsDelim( *line ) || *line == MSC_MACRO_START || *line == '_' ||
            *line == '@' || *line == '^' ||
            ( macro && *line == '(' ) || ( *line == '.' && *( line + 1 ) != '.' ) ) )
    {
        if ( *line == '#' )
            macro = 1;

    /* parse nested parameters */
        if ( *line == '(' )
        {
            b = StrFindCloseBrace( line );

            if ( !b )
            {
                return RESULT_ERR;
            }

            memcpy( params, line, b - line );
            params += b - line;
            length += b - line;
            line = b;
            macro = 0;
        }
        else
        {
            *params++ = *line++;
            length++;
        }

        if ( length >= TAT_MAX_ID_LEN )
            return RESULT_ERR;
    }

    *params = 0;
    *s = line;
    return RESULT_OK;
}

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
LOGICAL StrExtractLabel( char * * s, char * params )
{
    char                   *line;
    size_t                  length = 0;

    if ( !s )
        return RESULT_ERR;

    StrSkipComments( s );

    line = *s;

    if ( !line || ( *line == 0 ) )
        return RESULT_ERR;

    if ( params )
    {
        *params = 0;            /* to avoid further problems */

        while ( *line && ( !SymIsDelim( *line ) || ( *line == '_' ) || ( *line == '@' ) ) )
        {
            *params++ = *line++;
            length++;

            if ( length > TAT_MAX_ID_LEN )
                return RESULT_ERR;
        }

        *params = 0;
    }
    else
    {
        while ( *line && ( !SymIsDelim( *line ) || ( *line == '_' ) || ( *line == '@' ) ) )
        {
            line++;
            length++;

            if ( length > TAT_MAX_ID_LEN )
                return RESULT_ERR;
        }
    }

    *s = line;
    return RESULT_OK;
}


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
LOGICAL StrExtractFLabel( char * * s, char * params )
{
    char                   *line;
    size_t                  length = 0;

    if ( !s )
        return RESULT_ERR;

    StrSkipComments( s );

    line = *s;

    if ( !line || ( *line == 0 ) )
        return RESULT_ERR;

    if ( params )
    {
        *params = 0;            /* to avoid further problems */

        while ( *line && ( !SymIsDelim( *line ) || ( *line == '_' ) || ( *line == '@' ) || (*line=='.') ) )
        {
            *params++ = *line++;
            length++;

            if ( length > TAT_MAX_ID_LEN )
                return RESULT_ERR;
        }

        *params = 0;
    }
    else
    {
        while ( *line && ( !SymIsDelim( *line ) || ( *line == '_' ) || ( *line == '@' ) || ( *line == '.' ) ) )
        {
            line++;
            length++;

            if ( length > TAT_MAX_ID_LEN )
                return RESULT_ERR;
        }
    }

    *s = line;
    return RESULT_OK;
}

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
LOGICAL StrExtractDigit( char * * line, char * params )
{
    char                   *s;
    int                     point = 0, hex = 0, epoint = 0;

    if ( !line )
        return RESULT_ERR;

    StrSkipComments( line );

    s = *line;

    if ( !s || *s == 0 )
        return RESULT_ERR;

    if ( params )
        *params = 0;

/* skip first possible minus */
    if ( *s == '-' || *s == '+' )
    {
        if ( params )
            *params++ = *s;

        s++;
        STR_SKIP_SPACES(s);
    };

    if ( *s == '0' && *( s + 1 ) == 'x' )
    {
        if ( params )
        {
            *params++ = *s;
            *params++ = *( s + 1 );
        };

        hex = 1;

        s += 2;
    };

    while ( *s )
    {
        if ( ( size_t ) ( s - ( *line ) ) > TAT_MAX_ID_LEN )
            return RESULT_ERR;

        if (!hex && (*s=='e' || *s=='E'))
        {
            if (epoint)
                return RESULT_ERR;
            epoint=1;
            if ( params )
                *params++ = *s;
            s++;
            STR_SKIP_SPACES(s);
            if (*s=='+' || *s=='-')
            {
                if ( params )
                    *params++ = *s;
                s++;
            }
            STR_SKIP_SPACES(s);
        } else if ( *s == '.' )
        {
        /* check for duplicated point */

            if ( point || hex || epoint)
                return RESULT_ERR;

            point = 1;

            if ( params )
                *params++ = *s;

            s++;
        } 

        if ( !SymIsDigit( *s ) )
        {                       /* check for hexadecimal digit */
            if ( !( hex && ( ( *s >= 'a' && *s <= 'f' ) || ( *s >= 'A' && *s <= 'F' ) ) ) )
                break;
        }

        if ( params )
            *params++ = *s;

        s++;
    }

    if ( params )
    {
        *params = 0;
    }
    else
    {
        /* if no number is specified at all */
        if (s==*line)
            return RESULT_ERR;
    }

    *line = s;

    return RESULT_OK;
}

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
LOGICAL StrRemoveDupSpace( char * dest, char * src )
{
    int                     spc = 0;

    if ( !dest || !src )
        return RESULT_ERR;

    while ( *src )
    {
        if ( *src == SPACE || *src == TAB || *src == LF || *src == CR )
        {
            src++;

            if ( !spc )
            {
                spc = 1;
                *dest++ = SPACE;
            }
        }
        else
        {
            *dest++ = *src++;
            spc = 0;
        };
    };

    *dest = 0;
    return RESULT_OK;
}

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
LOGICAL SymIsExprDelim( const char c )
{
    return ( c == ',' ) || ( c == ')' ) || ( c == '}' ) || ( c == ']' ) || ( c < ' ' );
}

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
LOGICAL StrExtractExpr( char * * s, char * params )
{
    char                   *line = *s, *b;
    UINT32_T                    oper;
    char                    op;

    STR_SKIP_SPACES( line );
    oper = 0;
    op = 0;

    while ( *line &&
        ( ( !SymIsExprDelim( *line ) && *line != '.' ) || ( *line == '.' && *( line + 1 ) != '.' ) ) )
    {
        if ( *line == '+' || *line == '-' || *line == '*' || *line == '/' ||
            *line == '|' || *line == '&' || *line == '^' || *line == '~' || *line == '!' )
        {
            oper++;
            if ( oper == 2 )
            {
                if ( op != *line )
                {
                /* incorrect operator */
                    return RESULT_ERR;
                }

                if ( op == '*' || op == '^' || op == '!' || op == '/' )
                    return RESULT_ERR;
            }

            if ( oper == 1 )
                op = *line;

            if ( oper >= 3 )
                return RESULT_ERR;
        }
        else
            oper = 0;

        if ( *line == '(' )
        {
            b = StrFindCloseBrace( line );
            if ( !b )
                return RESULT_ERR;

            memcpy( params, line, b - line );
            params += b - line;
            line = b;
        }
        else
            *params++ = *line++;
    }

    *params = 0;
    *s = line;
    return RESULT_OK;
}

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
double StrToDouble( char * s )
{
    double                  minus = 1, sum;

    if ( !s )
        return 0;

    STR_SKIP_SPACES( s );

    if ( *s == '-' )
    {
        minus = -1;
        s++;
    }
    else if ( *s == '+' )
        s++;

    if ( *s == '0' && *( s + 1 ) == 'x' )   /* hexadecimal number */
    {
        s += 2;
        sum = 0;

        while ( *s )
        {
            sum *= 16;

            if ( *s >= '0' && *s <= '9' )
                sum += ( *s ) - '0';
            else if ( *s >= 'a' && *s <= 'f' )
                sum += ( *s ) - 'a' + 10;
            else if ( *s >= 'A' && *s <= 'F' )
                sum += ( *s ) - 'A' + 10;
            else
                return 0;

            s++;

        }

        return sum * minus;
    }
    else
        return minus * atof( s );
}

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
int StrToInt( char * s )
{
    int     minus = 1, sum;

    if ( !s )
        return 0;

    STR_SKIP_SPACES( s );

    if ( *s == '-' )
    {
        minus = -1;
        s++;
    }
    else if ( *s == '+' )
        s++;

    if ( *s == '0' && *( s + 1 ) == 'x' )   /* hexadecimal number */
    {
        s += 2;
        sum = 0;

        while ( *s )
        {
            sum *= 16;

            if ( *s >= '0' && *s <= '9' )
                sum += ( *s ) - '0';
            else if ( *s >= 'a' && *s <= 'f' )
                sum += ( *s ) - 'a' + 10;
            else if ( *s >= 'A' && *s <= 'F' )
                sum += ( *s ) - 'A' + 10;
            else
                return 0;

            s++;

        }

        return sum * minus;
    }
    else
        return minus * atoi( s );
}


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
int StrCompareSmart( char * s1, char * s2 )
{
    /*if (!s1 ||!s2) return -2; */
    if ( StrIsAnyNumber( s1 ) && StrIsAnyNumber( s2 ) )
    {
        double                  z;

        z = StrToDouble( s1 ) - StrToDouble( s2 );
        return ( ( z > 0 ) ? 1 : ( ( z < 0 ) ? -1 : 0 ) );
    };

    return strcmp( s1, s2 );
}

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
LOGICAL StrCheckSDLName( char * s )
{
    size_t                  j;

    if ( !s || s[0] == 0 )
    {
        return RESULT_ERR;
    };

    if ( SymIsDigit( s[0] ) || s[0] == '_' )
        return RESULT_ERR;

    j = 0;

    while ( s[j] )
    {
        if ( SymIsDelim( s[j] ) && s[j] != '_' )
            return RESULT_ERR;

        j++;
        if (j>=TAT_MAX_ID_LEN)
            return RESULT_ERR;
    };

    return RESULT_OK;
}

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
int StrToUpper( char * target )
{
    int                     i = 0;

    if ( !target )
        return 0;

    while ( 0 != *target )
    {
        *target = SymToUpper( *target );
        target++;
        i++;
    }

    return i;
}

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
char                   *StrUpCopy( char * target, const char * src )
{
    UINT32_T                    i = 0;

    if ( !target )
        return NULL;

    if ( src )
    {
        while ( src[i] )
        {
            *target++ = SymToUpper( src[i] );
            i++;
        }
    }

    *target = 0;
    return target;
}

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
LOGICAL StrEmbedMSCtext( char * dest, char * src )
{
    if ( !dest )
        return RESULT_ERR;
    if ( !src )
    {
        *dest = 0;
        return RESULT_OK;
    }
    while ( *src )
    {
        *dest++ = *src;
        if ( *src == '\'' )
            *dest++ = '\'';
        src++;
    }
    *dest = 0;
    return RESULT_OK;
}

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
int StrICmp( const char * c1, const char * c2 )
{
    return stricmp( c1, c2 );
}

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
LOGICAL StrExtractMSCString( char * * line, char * dest )
{
    char                   *s;
    int                    i;

    if ( !line )
        return RESULT_ERR;

    s = *line;

    if ( !s || *s!='\'')
        return RESULT_ERR;

    s++;

    /* remove leading spaces within quotes */
    STR_SKIP_SPACES(s);

    if ( dest )
    {
        i = 0;
        while ( *s )
        {
            while ( *s && *s != '\'' )
                dest[i++] = *s++;

            if ( ( *s == '\'' ) && *( s + 1 ) == '\'' )
            {
                dest[i++] = '\'';
                s += 2;
            }
            else
                break;
        }

        /* remove trailing spaces in parsed string */
        while (i>0 && SymIsSpace(dest[i-1])) i--;

        if ( *s == 0 )
            return RESULT_ERR;

        s++; /* skip trailing quote */

        dest[i] = 0;
    }
    else
    {
        s++;

        while ( *s )
        {
            while ( *s && *s != '\'' )
                s++;

            if ( ( *s == '\'' ) && *( s + 1 ) == '\'' )
            {
                s += 2;
            }
            else
                break;
        }

        if ( *s == 0 )
            return RESULT_ERR;

        s++;
    }

    *line = s;
    return RESULT_OK;
}

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
LOGICAL StrCheck01(const char * str)
{
    return (str && str[1]==0 && (str[0]=='0' || str[0]=='1'));
}
