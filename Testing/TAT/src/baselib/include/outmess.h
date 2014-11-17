#ifndef OUTMESS_H
#define OUTMESS_H
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

/* stream for error's output */
extern FILE            *MSG_ErrorStream;

/***************************************************************************
 * Function : MSG_PrintError
 *
 * Description: Output error message
 *
 * Returns: None
 *
 * Notes: None
 *
 **************************************************************************/
extern void             MSG_PrintError( const char * format, ... );

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
extern void             MSG_PrintSysError( const char * s );

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
extern void             MSG_PrintFileError( const char * s );

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
extern void             MSG_Abort( const char * s );

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
extern void             MSG_PushProcessedFile( const char * s );

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
extern void             MSG_PopProcessedFile(  );

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
extern void             MSG_SyntaxError(  );

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
extern void             MSG_SemanticError( const char * s, ... );

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
extern void             MSG_Copyright( const char * mess, char * pname );

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
extern void             MSG_SetLineNumber( const size_t i );

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
extern void             MSG_SemanticWarning( const char * s, ... );

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
extern void             MSG_EnableSemanticWarning( int i );

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
extern size_t             MSG_GetLineNumber(  );

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
extern char * MSG_GetFileName(  );

/***************************************************************************
 * Function : MSG_DumpMessages
 *
 * Description: Dump all error/warning messages
 *
 * Returns: None
 *
 * Notes: None
 *
 **************************************************************************/
extern void MSG_DumpMessages( void );

/* current name of executable */
extern char                   *MSG_ProgramFileName;

#endif
