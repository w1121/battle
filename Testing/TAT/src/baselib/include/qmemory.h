#ifndef MEMORY_H
#define MEMORY_H
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
* Functions:    MEM_Allocate
*               MEM_Reallocate
*               MEM_Free
*               MEM_CopyStr
*               MEM_Copy
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  Memory allocation and checking
*
* Notes:        None
*
******************************************************************************/

#include <portable.h>

#ifdef __cplusplus
extern                  "C"
{
#endif                                 /* C++ */

    /***************************************************************************
     * Function : MEM_FreeStrArray
     *
     * Description: free allocated memory
     *
     * Returns: RESULT_ERR/RESULT_OK
     *
     * Notes: None
     *
     **************************************************************************/
    extern LOGICAL          MEM_FreeStrArray( const size_t n, char **store );

    /***************************************************************************
     * Function : MEM_CopyStrArray
     *
     * Description: copy array of strings
     *
     * Returns: pointer to a new array / NULL
     *
     * Notes: None
     *
     **************************************************************************/
    extern char           **MEM_CopyStrArray( const size_t n, const char **store );

    /***************************************************************************
    * Function : MEM_CalcCRC32
    *
    * Description: Calculate CRC for buffer
    *
    * Returns: CRC
    *
    * Notes: None
    *
    **************************************************************************/
    extern UINT32_T             MEM_CalcCRC32( void *d, int size );

#ifdef MEM_DEBUG
/* MEM_DEBUG is used for using BoundsChecker */
/***************************************************************************
 * Function : MEM_Allocate
 *
 * Description: allocate memory, clean memory with zero and check if memory was allocated
 *
 * Returns: pointer to allocated block
 *
 * Notes: works if MEM_DEBUG is set
 *
 **************************************************************************/
#define MEM_Allocate(a) calloc(a, 1)

/***************************************************************************
 * Function : MEM_Free
 *
 * Description: free allocated memory block
 *
 * Returns: none
 *
 * Notes: works if MEM_DEBUG is set
 *
**************************************************************************/
#define MEM_Free(a) free(a)

 /***************************************************************************
 * Function : MEM_Reallocate
 *
 * Description: resize memory block, check if memory was allocated
 *
 * Returns: pointer to block
 *
 * Notes: works if MEM_DEBUG is set
 *
 **************************************************************************/
#define MEM_Reallocate(a, b)  realloc(a, b)

 /***************************************************************************
 * Function : MEM_CopyStr
 *
 * Description: if string is not NULL or zero-length, allocate memory and copy it
 *
 * Returns: pointer to allocated block
 *
 * Notes: works if MEM_DEBUG is set
 *
 **************************************************************************/
#define MEM_CopyStr(a) (a && *(a))?_strdup(a):NULL

/***************************************************************************
 * Function : MEM_Copy
 *
 * Description: allocate memory of specified size and copy source in it
 *
 * Returns: pointer to allocated block
 *
 * Notes: works if MEM_DEBUG is set
 *
 **************************************************************************/
#define MEM_Copy(a, b) memcpy(malloc(b), a, b)

 /***************************************************************************
 * Function : MEM_AddObject
 *
 * Description: preallocate memory for future objects
 *              can be used only for incremental memory allocation!!!
 *
 * Returns: pointer to memory storage
 *
 * Notes: works if MEM_DEBUG is set
 *
 **************************************************************************/
#define MEM_AddObject(a, b, c) realloc(a, b*c)

#else
/* general implementation */

/***************************************************************************
 * Function : MEM_Allocate
 *
 * Description: allocate memory, clean memory with zero and check if memory was allocated
 *
 * Returns: pointer to allocated block
 *
 * Notes: None
 *
 **************************************************************************/
    extern void            *MEM_Allocate( size_t size );

/***************************************************************************
 * Function : MEM_Reallocate
 *
 * Description: resize memory block, check if memory was allocated
 *
 * Returns: pointer to block
 *
 * Notes: None
 *
 **************************************************************************/
    extern void            *MEM_Reallocate( void *z, size_t size );

/***************************************************************************
 * Function : MEM_AddObject
 *
 * Description: preallocate memory for future objects
 *              can be used only for incremental memory allocation!!!
 *
 * Returns: pointer to memory storage
 *
 * Notes: None
 *
 **************************************************************************/
    extern void            *MEM_AddObject( void *z, size_t count, size_t el_size );

/***************************************************************************
 * Function : MEM_Free
 *
 * Description: free allocated memory block
 *
 * Returns: none
 *
 * Notes: None
 *
**************************************************************************/
    extern void             MEM_Free( void *x );

/***************************************************************************
 * Function : MEM_Copy
 *
 * Description: allocate memory of specified size and copy source in it
 *
 * Returns: pointer to allocated block
 *
 * Notes: None
 *
 **************************************************************************/
    extern void            *MEM_Copy( const void *s, size_t size );

/***************************************************************************
 * Function : MEM_CopyStr
 *
 * Description: if string is not NULL or zero-length, allocate memory and copy it
 *
 * Returns: pointer to allocated block
 *
 * Notes: None
 *
 **************************************************************************/
    extern char            *MEM_CopyStr( const char *s );

#endif                                 /* MEMDEBUG */

#define MEM_TYPE_ALLOC(a) (a *)MEM_Allocate(sizeof(a));

    extern UINT32_T             MSC_MemoryErrorOccured;

#ifndef MEM_SYS_PAGE_SIZE
#define MEM_SYS_PAGE_SIZE   4096
#endif

#define MEM_ADD_OBJECT(obj, count)   obj=MEM_AddObject(obj, (count)+1, sizeof(obj[0])); memset( &obj[(count)], 0, sizeof ( obj[0] ) )

#define MEM_REALLOCATE(obj, count)   obj=MEM_Reallocate(obj,(count) * sizeof(obj[0]))

#ifdef __cplusplus
}

#endif                                 /* C++ */

#endif                                 /* MEMORY_h */
