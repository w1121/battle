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
* Functions:    
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  TAT+ license manager
*
* Notes:        None
*
******************************************************************************/
#include <portable.h>
#include <fileproc.h>
#include <mscmess.h>
#include <outmess.h>
#include <tatgdata.h>
#include <strproc.h>
#include <time.h>
#include <qmemory.h>
#include <license.h>

/* special code characters which may appear in license file  */
char                    MSC_CodedChars[] = { "Z10HU2P53EK84WL7" };

#ifdef MSC_USE_LICENSE
/***************************************************************************
 * Function : tat_Decode
 *
 * Description: 
 *
 * Returns: 
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_Decode( unsigned char *d, char *src, char *hostname )
{
    char                   *pic1, *pic2;
    UINT32_T                    size = 0, cc, hostn_size=0;

    
    STR_SKIP_SPACES( src );
    while ( *src )
    {
        pic1 = strchr( MSC_CodedChars, *src );
        if ( !*src || !pic1 )
        {
            return RESULT_ERR;
        }
        src++;
        STR_SKIP_SPACES( src );
        pic2 = strchr( MSC_CodedChars, *src );
        if ( !*src || !pic2 )
        {
            return RESULT_ERR;
        }
        d[size] = (unsigned char)(( (UINT32_T)(( ( pic1 - &MSC_CodedChars[0] ) << 4 ) + ( pic2 - &MSC_CodedChars[0] )) - size) & 0xFF);

        if (hostname)
        {
            d[size]^=hostname[hostn_size++];
            if (!hostname[hostn_size]) hostn_size=0;
        }

        size++;
        src++;
        STR_SKIP_SPACES( src );
    }

    if ( size < MSC_LICENSE_SIZE )
    {
        return RESULT_ERR;
    }

    cc = ( ( ( ( UINT32_T ) ( d[size - 4] & 255 ) ) << 24 ) + ( ( ( UINT32_T ) ( d[size - 3] & 255 ) ) << 16 ) +
        ( ( ( UINT32_T ) ( d[size - 2] & 255 ) ) << 8 ) + ( ( UINT32_T ) ( d[size - 1] & 255 ) ) );

    return cc == MEM_CalcCRC32( d, size - 4 );
}
#endif

/***************************************************************************
 * Function : MSC_GetLicense
 *
 * Description: check license correctness and initialize data
 *
 * Returns: RESULT_OK / aborts execution
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_GetLicense( char *execpath )
{
#ifdef MSC_USE_LICENSE
    char                    fpath[TAT_MAX_FNAME_LEN];
    FILE                   *f;
    UINT32_T                    i;
    int                     time_val;
    char                   *buffer;

    unsigned char           binbuf[MSC_LICENSE_SIZE];
    char                    hostname[TAT_MAX_FNAME_LEN];

/*  tm, time_t  - system structures */
    struct tm              *t;
    time_t                  tm;
#if OSTYPE==WIN32
    WSADATA wsaData;
#endif

/* try to extract file path from executable name */
    FP_SplitPath( execpath, fpath, NULL, NULL );
    time( &tm );

    strcat( fpath, MSC_LICENSE_FILE );
    f = fopen( fpath, "r" );
    if ( !f )
    {
        execpath = getenv( MSC_ENV_VAR );
        if ( !execpath )
        {
            MSG_PrintError( MSG_NO_LICENSE_FILE );
            exit( SYS_ERROR_EXIT_CODE );
        }

        strcpy( fpath, execpath );
        i = strlen( fpath );
        if ( i && ( fpath[i - 1] != '\\' && fpath[i - 1] != '/' ) )
        {
            fpath[i] = '/';
            fpath[i + 1] = 0;
        }
        strcat( fpath, "bin/" MSC_LICENSE_FILE );
        FP_NormalizePath( fpath );
        f = fopen( fpath, "r" );
        if ( !f )
        {
            MSG_PrintError( MSG_NO_LICENSE_FILE );
            exit( SYS_ERROR_EXIT_CODE );
        }
    }

    i = FP_FileSize( f );

    if ( !i )
    {
        MSG_PrintError( MSG_LICENSE_CORRUPTED_s, fpath );
        exit( SYS_ERROR_EXIT_CODE );
    }

    buffer = MEM_Allocate( i + 1 );
    fread( buffer, i, 1, f );
    fclose( f );

    buffer[i] = 0;

#if OSTYPE==WIN32
    {
        unsigned short wVersionRequested;
        int err;

        wVersionRequested = MAKEWORD( 2, 0 );

        err = WSAStartup( wVersionRequested, &wsaData );
        if ( err != 0 )
        {
            MSG_PrintError( MSG_NO_HOST_NAME );
            exit( SYS_ERROR_EXIT_CODE );
        }
    }

#endif
    if (gethostname(hostname,255)==-1)
    {
        MSG_PrintError( MSG_NO_HOST_NAME );
        exit( SYS_ERROR_EXIT_CODE );
    };

#if OSTYPE==WIN32
    WSACleanup();
#endif

    i = 0;
    while (hostname[i] && hostname[i]!='.' )
    {
        hostname[i]= (char) toupper(hostname[i]);
        i++;
    }

    if (hostname[i]=='.') hostname[i]=0;

    if ( !tat_Decode( binbuf, buffer, hostname ) )
    {
        MSG_PrintError( MSG_LICENSE_CORRUPTED_s, fpath );
        exit( 1 );
    }

    t = localtime( &tm );
    memset( buffer, 0, MSC_LICENSE_YEAR_SIZE + 1 );
    strncpy( buffer, ( char * ) &binbuf[MSC_LICENSE_YEAR], MSC_LICENSE_YEAR_SIZE );
    time_val = atoi( buffer );

    t->tm_year += 1900;
    t->tm_mon += 1;

    if ( t->tm_year > time_val )
    {
        MSG_PrintError( MSG_LICENSE_EXPIRED );
        exit( SYS_ERROR_EXIT_CODE );
    }
    else if ( t->tm_year == time_val )
    {
        memset( buffer, 0, MSC_LICENSE_MONTH_SIZE + 1 );
        strncpy( buffer, ( char * ) &binbuf[MSC_LICENSE_MONTH], MSC_LICENSE_MONTH_SIZE );
        time_val = atoi( buffer );
        if ( t->tm_mon > time_val )
        {
            MSG_PrintError( MSG_LICENSE_EXPIRED );
            exit( SYS_ERROR_EXIT_CODE );
        }
        else if ( t->tm_mon == time_val )
        {
            memset( buffer, 0, MSC_LICENSE_DAY_SIZE + 1 );
            strncpy( buffer, ( char * ) &binbuf[MSC_LICENSE_DAY], MSC_LICENSE_DAY_SIZE );
            time_val = atoi( buffer );
            if ( t->tm_mday > time_val )
            {
                MSG_PrintError( MSG_LICENSE_EXPIRED );
                exit( SYS_ERROR_EXIT_CODE );
            }
        }

    }

/* set-up internal constants */
    TAT_ZeroStr[0] = binbuf[MSC_LICENSE_MSC_ZeroStr];

    memset( buffer, 0, MSC_LICENSE_FNAME_LEN_SIZE + 1 );
    strncpy( buffer, ( char * ) &binbuf[MSC_LICENSE_FNAME_LEN], MSC_LICENSE_FNAME_LEN_SIZE );
    TAT_MaxFileNameLen = atoi( buffer );

    memset( buffer, 0, MSC_LICENSE_ID_LEN_SIZE + 1 );
    strncpy( buffer, ( char * ) &binbuf[MSC_LICENSE_ID_LEN], MSC_LICENSE_ID_LEN_SIZE );
    TAT_MaxIdLen = atoi( buffer );

    fprintf(stdout,"Registered to: %s/%s(%s), host '%s'\n\n",
        &binbuf[MSC_LICENSE_COMPANY],
        &binbuf[MSC_LICENSE_USER],
        &binbuf[MSC_LICENSE_EMAIL],
        hostname
        );

    MEM_Free( buffer );
#else /*MSC_USE_LICENSE*/
	TAT_MaxFileNameLen = TAT_MAX_FNAME_LEN;
	TAT_MaxIdLen = TAT_MAX_ID_LEN;
	TAT_ZeroStr[0] = 0;
	puts("");
#endif
    return RESULT_OK;
}
