/******************************************************************************
*
*       COPYRIGHT 2001-2004 MOTOROLA, ALL RIGHTS RESERVED
*
*       MOTOROLA CONFIDENTIAL PROPRIETARY
*
* Description:  Command-line processing
*
* $RCSfile: strcoll.c,v $
* $Revision: 1.3 $
* $Source: /home/cvsusr/cvssoft/tat_tools/baselib/src/strcoll.c,v $
* $Date: 2004/03/05 14:37:44 $
*
*
* CHANGE HISTORY
*                                                       
* Initial version 
*
******************************************************************************/
#include <portable.h>
#include <strcoll.h>
#include <mscmess.h>
#include <mscbase.h>
#include <tatgdata.h>
#include <qmemory.h>
#include <strproc.h>
#include <outmess.h>
#include <mscstrs.h>
#include <crctable.h>

#define PCHAR_COLL_EXTEND MEM_SYS_PAGE_SIZE/sizeof(char *)

/***************************************************************************
 * Function : StrCollInit
 *
 * Description: initialize storage for string collection
 *
 * Returns: pointer to storage
 *
 * Notes: None
 *
 **************************************************************************/
STRING_COLL           *StrCollInit( STRING_COLL * coll )
{
    if ( !coll )
    {
        coll = MEM_Allocate( sizeof ( *coll ) );
    }
    else
    {
        memset( coll, 0, sizeof ( *coll ) );
    }

    if ( StrCollAdd( ( char * ) STR_ZERO, coll ) != STRCOLL_STR_ID_0 )
    {
        return NULL;
    }

    if ( StrCollAdd( ( char * ) STR_ONE, coll ) != STRCOLL_STR_ID_1 )
    {
        return NULL;
    }

    if ( StrCollAdd( ( char * ) STR_INF, coll ) != STRCOLL_STR_ID_INF )
    {
        return NULL;
    }

    return coll;
}

/***************************************************************************
 * Function : StrCollClean
 *
 * Description: free memory, allocated for string collection
 *
 * Returns: LOGICAL_TRUE/LOGICAL_FALSE
 *
 * Notes:
 *
 **************************************************************************/
LOGICAL StrCollClean( STRING_COLL * x )
{
    size_t i;

    if ( !x )
        return RESULT_OK;

    for ( i = 0; i < x->string_Count; i++ )
    {
        if ( x->strings[i] && x->strings[i] != &TAT_ZeroStr[0] )
        {
            MEM_Free( x->strings[i] );
            x->strings[i]=NULL;
        }
    }

    memset( x->strings, 0, x->string_Count * sizeof ( char * ) );

    for ( i = 0; i < STRCOLL_HASH_TABLE_SIZE; i++ )
    {
        if ( x->hash_Array[i] )
            MEM_Free( x->hash_Array[i] );
        x->hash_Array[i]=NULL;
    }

    if ( x->strings )
        MEM_Free( x->strings );
    memset( x, 0, sizeof ( x[0] ) );
    MEM_Free( x );
    return RESULT_OK;
}

/***************************************************************************
 * Function : StrCollAdd
 *
 * Description: allocate memory in string collection
 *
 * Returns: index of string
 *
 * Notes:
 *
 **************************************************************************/
STRING_ID StrCollAdd( const char * key, STRING_COLL * coll )
{
    size_t                  h;
    register char         **x;
    STRING_ID             *sh;
    size_t                 i, n, pos, z;
    char                  *s, *t;

/* check input parameters */
    if ( !key || !( *key ) || !coll )
    {
        return 0;
    }

/* hash calculation using CRC-32 */
    h = 0xFFFFFFFF;
    pos = 0;
    while ( key[pos] )
    {
        n = key[pos++];
        h = ( h >> 8 ) ^ CRC32Table[( h ^ n ) & 0xFF];
    }

#if STRCOLL_HASH_POWER_2
    h &= ( STRCOLL_HASH_TABLE_SIZE - 1 );
#else
    h %= STRCOLL_HASH_TABLE_SIZE;
#endif

    n = coll->hash_Length[h];
    x = coll->strings;
    sh = coll->hash_Array[h];
    i = 0;
    z = 0;

/* string binary search */
    while ( n >= 1 )
    {
        i = n >> 1;
        for ( s = key, t = x[sh[i]]; *s == *t; s++, t++ )
        {
            if ( *s == 0 )
            {
            /* string found */
                return sh[i] + 1;
            }
        }

        if ( *s < *t )
        {
            n = i;
        }
        else
        {
            sh += i + 1;
            n -= i + 1;
            z += i + 1;
        }
    }

/* position to insert */
    i &= ~1;
    i += z;

    n = coll->string_Count;
    coll->hash_Array[h] =
        MEM_Reallocate( coll->hash_Array[h], ( coll->hash_Length[h] + 1 ) * sizeof ( coll->hash_Length[0] ) );
    memmove( &coll->hash_Array[h][i + 1], &coll->hash_Array[h][i],
        ( coll->hash_Length[h] - i ) * sizeof ( coll->hash_Array[h][i] ) );
    coll->hash_Length[h]++;
    coll->hash_Array[h][i] = n;

/* increase string storage, if needed */
    if ( n >= coll->string_Count_Realloc )
    {
    /* reallocate new buffer */
        coll->string_Count_Realloc += PCHAR_COLL_EXTEND;
        coll->strings = MEM_Reallocate( coll->strings, coll->string_Count_Realloc * sizeof ( char * ) );
    }

/* add string to storage, pos - length of string */
    coll->strings[n] = MEM_Copy( key, pos + 1 );

    /* increase string counter */
    ( coll->string_Count )++;
    return n + 1;
}

/***************************************************************************
 * Function : StrCollCopy
 *
 * Description: Copy string collection
 *
 * Returns: pointer to storage/NULL
 *
 * Notes: None
 *
 **************************************************************************/
STRING_COLL           *StrCollCopy( STRING_COLL * src )
{
    size_t                  i;
    STRING_COLL           *dest;

    dest = MEM_Allocate( sizeof ( STRING_COLL ) );
    dest->string_Count = src->string_Count;
    dest->string_Count_Realloc = src->string_Count_Realloc;
    dest->strings = MEM_Allocate( sizeof ( dest->strings[0] ) * src->string_Count_Realloc );

    for ( i = 0; i < src->string_Count; i++ )
    {
        dest->strings[i] = MEM_CopyStr( src->strings[i] );
    }

    memcpy( dest->hash_Length, src->hash_Length, sizeof ( src->hash_Length[0] ) * STRCOLL_HASH_TABLE_SIZE );

    for ( i = 0; i < STRCOLL_HASH_TABLE_SIZE; i++ )
    {
        dest->hash_Array[i] = MEM_Copy( src->hash_Array[i], src->hash_Length[i] * sizeof ( src->hash_Array[0][0] ) );
    }

    return dest;
}

/***************************************************************************
 * Function : StrCollGet
 *
 * Description: returns pointer to string
 *
 * Returns: pointer to string / NULL
 *
 * Notes:
 *
 **************************************************************************/
char * StrCollGet( const STRING_ID x, const STRING_COLL * coll )
{
    if ( !coll || !x )
        return NULL;

    if ( x > coll->string_Count )
    {
#ifdef _DEBUG
        MSG_PrintError( MSG_INVALID_STRING_NUMBER_uu, x, coll->string_Count );
#else
        MSG_PrintError( MSG_INTERNAL_PROBLEM );
#endif
        exit(SYS_ERROR_EXIT_CODE);
    }
    return ( coll->strings[x - 1] );
}

/***************************************************************************
 * Function : StrCollProcess
 *
 * Description: update string in-place
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL StrCollProcess( STRING_ID * x, STRING_COLL * coll, STRCOLL_PROCESS_FUNC conv )
{
    char                    buffer[TAT_MAX_LINE_LEN];

    if ( !coll || !conv )
        return RESULT_ERR;

    if ( !*x )
        return RESULT_OK;

#if COMPILER == MSVC
    __try {
#endif
    if ( !conv( buffer, coll->strings[*x - 1] ) )
        return RESULT_ERR;
#if COMPILER==MSVC
    }
    /* handle critical errors in user's function */
    __except(1)
    {
        return RESULT_ERR;
    }
#endif

    *x = StrCollAdd( buffer, coll );
    return RESULT_OK;
}
