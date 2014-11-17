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
* Functions:    MSC_MapMessageID
*               MSC_FreeSigOpt
*               MSC_FreeSigMap
*               MSC_FillMessageMap
*               
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  Optimize signal definitions
*
* Notes:        None
*
******************************************************************************/

#include <portable.h>
#include <mscbase.h>
#include <qmemory.h>
#include <strproc.h>
#include <outmess.h>
#include <mscmess.h>
#include <tatgdata.h>
#include "sigopt.h"

size_t                    MSC_UniqMessCountATS = 0;
size_t                    MSC_UniqMapCountATS = 0;

MSC_UNIQUE_MESSAGES    *MSC_UniqMessStorage = NULL;
MSC_UNIQUE_MAP         *MSC_UniqMapStorage = NULL;

/***************************************************************************
 * Function : mess_par
 *
 * Description: generate string representation of message
 *
 * Returns: pointer to string
 *
 * Notes: None
 *
 **************************************************************************/
static char            *tat_MessageParams( MSC_STORAGE * M, size_t i, char *d )
{
    size_t                  k;
    char                   *p;
    char                    dest[TAT_MAX_LINE_LEN];

    *d = 0;
/*    p = d; */
    p=&dest[0];

    if ( M->messages[i].param_Count == 0 )
        return d;

    for ( k = 0; k < M->messages[i].param_Count - 1; k++ )
    {
        strcpy(p, MSC_GetStr( M->messages[i].message_Params[k], M ));
/*        StrEmbedLine( dest, MSC_GetStr( M->messages[i].message_Params[k], M ) );
        StrEmbedLine( d, dest ); */
        p += strlen( p );
        *p++ = ',';
    };
    strcpy(p, MSC_GetStr( M->messages[i].message_Params[k], M ));
    /* StrEmbedLine( dest, MSC_GetStr( M->messages[i].message_Params[k], M ) ); */

    StrEmbedLine( d, dest );

    return d;
}

/***************************************************************************
 * Function : MSC_MapMessageID
 *
 * Description: find appropriate number of message from list of unique messages
 *
 * Returns: number or -1 if not found
 *
 * Notes: None
 *
 **************************************************************************/
size_t MSC_MapMessageID( size_t key )
{
    size_t                    lo, hi, half;

    lo = 0;
    hi = MSC_UniqMapCountATS - 1;

    while ( lo <= hi )
    {
        half = ( lo + hi ) >> 1;

        if ( key == MSC_UniqMapStorage[half].original)
            return ( MSC_UniqMapStorage[half].new_num );
        else if ( key < MSC_UniqMapStorage[half].original )
        {
            hi = half - 1;
        }
        else
        {
            lo = half + 1;
        };
    }

    if ( MSC_UniqMapStorage[lo].original == key )
        return ( MSC_UniqMapStorage[lo].new_num );

    fprintf( MSG_ErrorStream, MSG_INTERNAL_PROBLEM );

    return (size_t) -1;
}

/***************************************************************************
 * Function : MSC_FreeSigOpt
 *
 * Description: clean signal defintions
 *
 * Returns: 
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_FreeSigOpt(  )
{
    size_t                    i;

    for ( i = 0; i < MSC_UniqMessCountATS; i++ )
    {
        MEM_Free( MSC_UniqMessStorage[i].signame );

        if ( MSC_UniqMessStorage[i].params )
            MEM_Free( MSC_UniqMessStorage[i].params );

        MSC_UniqMessStorage[i].signame = NULL;

    };

    if ( MSC_UniqMessStorage )
        MEM_Free( MSC_UniqMessStorage );

    MSC_UniqMessCountATS = 0;
    MSC_UniqMessStorage = NULL;

    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_FreeSigMap
 *
 * Description: clean signal mapping
 *
 * Returns: 
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_FreeSigMap(  )
{
    if ( MSC_UniqMapStorage )
        MEM_Free( MSC_UniqMapStorage );

    MSC_UniqMapStorage = NULL;

    MSC_UniqMapCountATS = 0;

    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_FillMessageMap
 *
 * Description: add message from MSC and fill message mapping
 *
 * Returns: RESULT_OK/RESULT_ERR
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_FillMessageMap( MSC_STORAGE * M )
{
    size_t                  i, j;
    char                   *sign;
    char                    dest[TAT_MAX_LINE_LEN];

    MSC_UniqMapStorage = MEM_Allocate( M->message_Count * sizeof ( MSC_UniqMapStorage[0] ) );

    for ( i = 0; i < M->message_Count; i++ )
    {
        sign = MSC_GetStr( M->messages[i].message_Name, M );
        /* StrToUpper( sign ); */
        tat_MessageParams( M, i, dest );

        for ( j = 0; j < MSC_UniqMessCountATS; j++ )
        {
            if ( !strcmp( sign, MSC_UniqMessStorage[j].signame ) &&
                MSC_UniqMessStorage[j].par_count == M->messages[i].param_Count )
            {
                if ( ( !MSC_UniqMessStorage[j].params && !dest[0] )
                    || ( MSC_UniqMessStorage[j].params && !strcmp( dest, MSC_UniqMessStorage[j].params ) ) )
                    break;
            }
        };

    /* add new parameter if not found in list */
        if ( j == MSC_UniqMessCountATS )
        {
            MSC_UniqMessStorage =
                MEM_Reallocate( MSC_UniqMessStorage, ( j + 1 ) * sizeof ( MSC_UniqMessStorage[0] ) );
            MSC_UniqMessStorage[j].par_count = M->messages[i].param_Count;
            MSC_UniqMessStorage[j].signame = MEM_CopyStr( sign );
            MSC_UniqMessStorage[j].params = MEM_CopyStr( dest );
            MSC_UniqMessCountATS++;
        };

    /* add entry to MSC_MapMessageIDping table */
        MSC_UniqMapStorage[MSC_UniqMapCountATS].original = M->messages[i].indicator;

        MSC_UniqMapStorage[MSC_UniqMapCountATS++].new_num = j;
    }

    return RESULT_OK;
}
