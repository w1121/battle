/******************************************************************************
*
*       COPYRIGHT 2001-2002 MOTOROLA, ALL RIGHTS RESERVED
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
#include <qmemory.h>
#include <outmess.h>
#include <mscmess.h>
#include <tatgdata.h>
#include <crctable.h>
UINT32_T                    MSC_MemoryErrorOccured = 0;

#if defined(_WIN32) && !defined(__CYGWIN32__) && defined(__STDC__)
extern int              _msize( void * );
#endif

#if defined(_WIN32) && !defined(__CYGWIN32__)
#include <crtdbg.h>
#endif

#ifndef MEM_DEBUG
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
void                   *MEM_Allocate( size_t size )
{
    void                   *x;

    if ( size == 0 )
        return NULL;

    x = calloc( 1, size );
    if ( x == NULL )
    {
        MSC_MemoryErrorOccured = 1;
        MSG_PrintError( MSG_ALLOCATE_FAILED_u, size );
        exit( SYS_ERROR_EXIT_CODE );
    }

    return x;
}

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
void                   *MEM_Reallocate( void *z, size_t size )
{
    void                   *x;

    if ( z == NULL )
        size <<= 2;             /* simple optimization */
    x = realloc( z, size );

    if ( x == NULL )
    {
        MSC_MemoryErrorOccured = 1;
        MSG_PrintError( MSG_ALLOCATE_FAILED_u, size );
        exit( SYS_ERROR_EXIT_CODE );
    }

    return x;
}

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
void MEM_Free( void *x )
{
    if ( x == NULL )
    {
#ifdef _DEBUG
        MSG_PrintError( MSG_FREE_NULL_POINTER );
#endif

        return;
    }
    free( x );
    return;
}

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
void                   *MEM_Copy( const void *s, size_t size )
{
    void                   *x = NULL;

    if ( s == NULL )
        return NULL;

    if ( size == 0 )
        return NULL;

    x = malloc( size );

    if ( x == NULL )
    {
        MSC_MemoryErrorOccured = 1;
        MSG_PrintError( MSG_ALLOCATE_FAILED_u, size );
        exit( SYS_ERROR_EXIT_CODE );
    }

    memcpy( x, s, size );

    return x;
}

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
char                   *MEM_CopyStr( const char *s )
{
    char                   *d;
    size_t                 len;

    if ( !s || !( *s ) )
        return NULL;

    len = strlen( s );

    d = MEM_Copy( s, len + 1 );

    return d;
}

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
void                   *MEM_AddObject( void *z, size_t count, size_t el_size )
{
    void                   *x;
    int                     isf;

    isf = ( el_size <= 16 ) ? 3 : ( ( el_size <= 64 ) ? 2 : ( ( el_size <= 128 ) ? 1 : 0 ) );

    if ( !z )
    {
        x = malloc( el_size * ( count << isf ) );
    }
    else
    {
        if ( !( count & ( ( 1 << isf ) - 1 ) ) )
        {
            count += ( 1 << isf ) - 1;
            x = realloc( z, el_size * count );
        }
        else
            x = z;
    }

    if ( x == NULL )
    {
        MSC_MemoryErrorOccured = 1;
        MSG_PrintError( MSG_ALLOCATE_FAILED_u, count * el_size );
        abort(  );
    }

    return x;
}

#endif                         /* MEM_DEBUG */

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
LOGICAL MEM_FreeStrArray( const size_t n, char **store )
{
    size_t               i;

    if ( !store )
        return RESULT_ERR;

    for ( i = 0; i < n; i++ )
    {
        if ( store[i] )
        {
            MEM_Free( store[i] );
        }
    }
    memset(store, 0, sizeof(store[0])*n);
    MEM_Free( store );
    return RESULT_OK;
}

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
char                  **MEM_CopyStrArray( const size_t n, const char **store )
{
    size_t                  i;
    char                  **p;

    if ( !n || !store )
        return NULL;

    p = MEM_Allocate( n * sizeof ( p[0] ) );

    for ( i = 0; i < n; i++ )
    {
        p[i] = MEM_CopyStr( store[i] );
    }

    return p;
}

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
UINT32_T MEM_CalcCRC32( void *mem, int size )
{
    register unsigned long  crc;
    register unsigned char *d = mem;
    int                     c;

    crc = 0xFFFFFFFF;
    while ( size )
    {
        c = *d++;
        crc = ( ( crc >> 8 ) & 0x00FFFFFF ) ^ CRC32Table[( crc ^ c ) & 0xFF];
        size--;
    }
    return ( crc ^ 0xFFFFFFFF );
}
