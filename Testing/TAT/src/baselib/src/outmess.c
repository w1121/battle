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
* Functions:    MSG_PrintError
*               MSG_PrintSysError
*               MSG_PrintFileError
*               MSG_Abort
*               MSG_Copyright
*               MSG_SyntaxError
*               MSG_SemanticError
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  Message output functions
*
* Notes:        None
*
******************************************************************************/
#include <portable.h>
#include <errno.h>
#include <time.h>
#include <outmess.h>
#include <mscmess.h>
#include <qmemory.h>
#include <license.h>
#include <mscbase.h>
#include <stdarg.h>
#include <tatgdata.h>

/* stream for erroroneous message output */
FILE                   *MSG_ErrorStream = NULL;

/* current name of executable */
char                   *MSG_ProgramFileName = NULL;

UINT32_T                    TAT_MajorVersion = 2;
UINT32_T                    TAT_MinorVersion = 1;

static size_t           tat_ErrorCounter = 0;
static char *         *tat_ErrorMessage = NULL;

/***************************************************************************
 * Function : MSG_DumpMessages
 *
 * Description: Dump all collected error messages
 *  
 * Returns: None
 *
 * Notes: None
 *
 **************************************************************************/
void MSG_DumpMessages( void )
{
    size_t i;

    if ( tat_ErrorCounter )
    {
        for ( i = 0; i < tat_ErrorCounter; i++ )
        {
            fputs( tat_ErrorMessage[i], MSG_ErrorStream );
            fflush( MSG_ErrorStream );
            MEM_Free( tat_ErrorMessage[i] );
        }
        MEM_Free( tat_ErrorMessage );
    }
    tat_ErrorMessage = NULL;
    tat_ErrorCounter = 0;
    return;
}

/***************************************************************************
 * Function : MSC_PrintErrorMessage
 *
 * Description: process error message
 *
 * Returns: None
 *
 * Notes: None
 *
 **************************************************************************/
void MSG_PrintError( const char * format, ... )
{
    char                    buffer[TAT_MAX_LINE_LEN];
    size_t                  i;

    va_list                 arglist;

    if ( format )
    {
        va_start( arglist, format );
        vsprintf( buffer, format, arglist );
    }
    else
    {
        strcpy( buffer, MSG_INTERNAL_PROBLEM );
    }

    if ( MSC_MemoryErrorOccured )
    {
        MSG_DumpMessages(  );
        fflush( MSG_ErrorStream );
        fprintf( MSG_ErrorStream, buffer );
        fflush( MSG_ErrorStream );
    }
    else
    {
        i = tat_ErrorCounter;
        while ( i )
        {
            i--;
            if ( !strcmp( buffer, tat_ErrorMessage[i] ) )
            {
                i = tat_ErrorCounter+1;
                break;
            }
        }

        if ( i != tat_ErrorCounter+1 )
        {
            tat_ErrorMessage =
                MEM_Reallocate( tat_ErrorMessage, ( tat_ErrorCounter + 1 ) * sizeof ( tat_ErrorMessage[0] ) );
            tat_ErrorMessage[tat_ErrorCounter] = MEM_CopyStr( buffer );
            tat_ErrorCounter++;
        }
    }
}

/***************************************************************************
 * Function : MSG_PrintSysError
 *
 * Description: Output system error message
 *
 * Returns: None
 *
 * Notes: None
 *
 **************************************************************************/
void MSG_PrintSysError( const char * s )
{
    if ( !MSG_ErrorStream )
    {
        MSG_ErrorStream = stderr;
    }
    if ( s )
    {
        MSG_PrintError( "%s: %s\n", s, strerror( errno ) );
    }
    else
    {
        MSG_PrintError( "%s\n", strerror( errno ) );
    }
}

/***************************************************************************
 * Function : MSG_PrintFileError
 *
 * Description: Output file-related error
 *
 * Returns: None
 *
 * Notes: None
 *
 **************************************************************************/
void MSG_PrintFileError( const char * s )
{
    if ( !MSG_ErrorStream )
    {
        MSG_ErrorStream = stderr;
    }
    if ( s )
    {
        MSG_PrintError( MSG_FILE_RELATED_ERROR_ss, s, strerror( errno ) );
    }

}

/***************************************************************************
 * Function : MSG_Abort
 *
 * Description: Output error message and aborts execution
 *
 * Returns: None (exit)
 *
 * Notes: None
 *
 **************************************************************************/
void MSG_Abort( const char * s )
{
    if ( !MSG_ErrorStream )
    {
        MSG_ErrorStream = stderr;
    }
    fputs( s, MSG_ErrorStream );
    fflush( MSG_ErrorStream );
    abort(  );
}

static size_t               tat_ProcFileCount = 0;
static struct
{
    char *                 name;
    size_t                  line;
}
tat_FileArray[TAT_FILE_ARRAY];

/***************************************************************************
 * Function : MSG_PushProcessedFile
 *
 * Description: Push file name into stack
 *
 * Returns: None
 *
 * Notes: None
 *
 **************************************************************************/
void MSG_PushProcessedFile( const char * s )
{
    if ( tat_ProcFileCount < TAT_FILE_ARRAY )
    {
        tat_FileArray[tat_ProcFileCount].name = MEM_CopyStr( s );
        tat_FileArray[tat_ProcFileCount].line = 0;
        tat_ProcFileCount++;
    }
    else
    {
        MSG_PrintError( NULL );
        exit( SYS_ERROR_EXIT_CODE );
    }
}

/***************************************************************************
 * Function : MSG_PopProcessedFile
 *
 * Description: Pop file name from stack and free memory
 *
 * Returns: None
 *
 * Notes: None
 *
 **************************************************************************/
void MSG_PopProcessedFile(  )
{
    if ( tat_ProcFileCount == 0 )
    {
        MSG_PrintError( MSG_INTERNAL_PROBLEM );
        exit( SYS_ERROR_EXIT_CODE );
    }
    tat_ProcFileCount--;
    if ( tat_FileArray[tat_ProcFileCount].name )
        MEM_Free( tat_FileArray[tat_ProcFileCount].name );
}

/***************************************************************************
 * Function : MSG_SetLineNumber
 *
 * Description: Update line number for current file
 *
 * Returns: None
 *
 * Notes: None
 *
 **************************************************************************/
void MSG_SetLineNumber( const size_t i )
{
    if ( tat_ProcFileCount == 0 )
    {
        MSG_PrintError( MSG_INTERNAL_PROBLEM );
        exit( SYS_ERROR_EXIT_CODE );
    }
    tat_FileArray[tat_ProcFileCount - 1].line = i;
}

/***************************************************************************
 * Function : MSG_GetLineNumber
 *
 * Description: return current line number of current file
 *
 * Returns: number
 *
 * Notes: None
 *
 **************************************************************************/
size_t MSG_GetLineNumber(  )
{
    if ( tat_ProcFileCount == 0 )
    {
        MSG_PrintError( MSG_INTERNAL_PROBLEM );
        exit( SYS_ERROR_EXIT_CODE );
    }
    return tat_FileArray[tat_ProcFileCount - 1].line;
}

/***************************************************************************
 * Function : MSG_GetFileName
 *
 * Description: return current file name
 *
 * Returns: name
 *
 * Notes: None
 *
 **************************************************************************/
char * MSG_GetFileName(  )
{
    if ( tat_ProcFileCount == 0 )
    {
        MSG_PrintError( MSG_INTERNAL_PROBLEM );
        exit( SYS_ERROR_EXIT_CODE );
    }
    return tat_FileArray[tat_ProcFileCount - 1].name;
}

/***************************************************************************
 * Function : MSG_SyntaxError()
 *
 * Description: Output syntax error position
 *
 * Returns: None
 *
 * Notes: None
 *
 **************************************************************************/
void MSG_SyntaxError(  )
{
    if ( !tat_ProcFileCount )
    {
        MSG_PrintError( MSG_INTERNAL_PROBLEM );
        exit( SYS_ERROR_EXIT_CODE );
    }

    MSG_PrintError( MSG_INCORRECT_SYNTAX_su, tat_FileArray[tat_ProcFileCount - 1].name,
        tat_FileArray[tat_ProcFileCount - 1].line );
    return;
}

/***************************************************************************
 * Function : MSG_SemanticError()
 *
 * Description: Output semantic error position, and description
 *
 * Returns: None
 *
 * Notes: None
 *
 **************************************************************************/
void MSG_SemanticError( const char * s, ... )
{
    char                    buffer[TAT_MAX_LINE_LEN];

    va_list                 arglist;

    if ( !tat_ProcFileCount || !s)
    {
        MSG_PrintError( MSG_INTERNAL_PROBLEM );
        exit( SYS_ERROR_EXIT_CODE );
    }

    va_start( arglist, s );
    vsprintf( buffer, s, arglist );

    MSG_PrintError( MSG_INCORRECT_SEMANTIC_sus, tat_FileArray[tat_ProcFileCount - 1].name,
        tat_FileArray[tat_ProcFileCount - 1].line, buffer );

    return;
}

static int              enable_warning = 1;

/***************************************************************************
 * Function : MSG_EnableSemanticWarning
 *
 * Description: enable / disable output of semantic warnings
 *
 * Returns: None
 *
 * Notes: None
 *
 **************************************************************************/
void MSG_EnableSemanticWarning( int i )
{
    enable_warning = i;
    return;
}

/***************************************************************************
 * Function : MSG_SemanticError()
 *
 * Description: Output semantic error position, and description
 *
 * Returns: None
 *
 * Notes: None
 *
 **************************************************************************/
void MSG_SemanticWarning( const char * s, ... )
{
    char                    buffer[TAT_MAX_LINE_LEN];

    va_list                 arglist;

    if ( !tat_ProcFileCount || !s)
    {
        MSG_PrintError( MSG_INTERNAL_PROBLEM );
        exit( SYS_ERROR_EXIT_CODE );
    }

    if ( enable_warning )
    {
        va_start( arglist, s );
        vsprintf( buffer, s, arglist );

        MSG_PrintError( MSG_WARNING_SEMANTIC_sus, tat_FileArray[tat_ProcFileCount - 1].name,
            tat_FileArray[tat_ProcFileCount - 1].line, buffer );
    }

    return;
}

/***************************************************************************
 * Function : MSG_Copyright
 *
 * Description: Output copyright message
 *
 * Returns: None
 *
 * Notes: None
 *
 **************************************************************************/
void MSG_Copyright( const char * mess, char * pname )
{
    MSG_ErrorStream = stderr;

    setlocale( LC_ALL, "" );

    setvbuf( stdout, NULL, _IONBF, 0 );
    
    MSG_ProgramFileName = MEM_CopyStr(pname);

    fprintf( stdout, "%s - version %u.%u build " BUILD "\n"
        "COPYRIGHT 2001-2004 MOTOROLA, ALL RIGHTS RESERVED\n", mess, TAT_MajorVersion, TAT_MinorVersion );

/* set-up error dumping procedure */
    if ( atexit( MSG_DumpMessages ) )
    {
        MSG_PrintError( MSG_INTERNAL_PROBLEM );
        MSG_DumpMessages(  );
        exit( SYS_ERROR_EXIT_CODE );
    };

    if ( pname )
    {
        MSC_GetLicense( pname );
    }
    else
        puts("");

    return;
}
