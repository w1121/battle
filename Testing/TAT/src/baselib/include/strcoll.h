#ifndef STRCOLL_H
#define STRCOLL_H
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
* Functions:    StrCollInit
*               StrCollClean
*               StrCollGet
*               StrCollAdd
*               StrCollCopy
*               StrCollProcess
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  string collection processing
*
* Notes:        None
*
******************************************************************************/
#include <portable.h>

#ifdef __cplusplus
extern                  "C"
{
#endif                                 /* C++ */

    typedef size_t          STRING_ID;

    typedef STRING_ID     *PSTRING_ID;


#define STRCOLL_HASH_TABLE_SIZE 256
#define STRCOLL_HASH_POWER_2    1

#define STRCOLL_STR_ID_0 (STRING_ID)1
#define STRCOLL_STR_ID_1 (STRING_ID)2
#define STRCOLL_STR_ID_INF (STRING_ID)3


    /* String collection */
    typedef struct _STRING_COLL
    {
    /* string collection data */
        size_t                  string_Count;   /* allocated dynamically */
        size_t                  string_Count_Realloc;   /* really allocated space */
        char *                  *strings;    /* all strings in MSC */
        PSTRING_ID              hash_Array[STRCOLL_HASH_TABLE_SIZE];    /* each element is ordered array of string ID */
        size_t                  hash_Length[STRCOLL_HASH_TABLE_SIZE];   /* length of hash cell */
    }
    STRING_COLL;


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
    extern STRING_COLL    *StrCollInit( STRING_COLL * );

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
    extern LOGICAL          StrCollClean( STRING_COLL * x );

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
    extern char *          StrCollGet( const STRING_ID x, const STRING_COLL * M );

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
    extern STRING_ID       StrCollAdd( const char * s, STRING_COLL * M );

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
    extern STRING_COLL    *StrCollCopy( STRING_COLL * src );

/***************************************************************************
 * Function : STRCOLL_PROCESS_FUNC
 *
 * Description: function type for string processing
 *              first parameter is destination, second is source
 *
 * Returns: function shall return NULL if error occured
 *
 * Notes: None
 *
 **************************************************************************/
    typedef char           *( *STRCOLL_PROCESS_FUNC ) ( char *, char * );

/***************************************************************************
 * Function : StrCollProcess
 *
 * Description: process string collection with user-defined function
 *
 * Returns: pointer to event data
 *
 * Notes:
 *
 **************************************************************************/
    extern LOGICAL          StrCollProcess( STRING_ID * x, STRING_COLL * coll, STRCOLL_PROCESS_FUNC conv );

#ifdef __cplusplus
}
#endif                                 /* C++ */

#endif                                 /* STRCOLL_H */
