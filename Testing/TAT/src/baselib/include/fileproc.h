#ifndef FILE_PROC_H
#define FILE_PROC_H
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
* Functions:    FP_SplitPath
*               FP_NormalizePath
*               FP_MakeDir
*               FP_OpenFile
*               FP_FileSize
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  File-processing functions
*
* Notes:        None
*
******************************************************************************/
#ifdef __cplusplus
extern                  "C"
{
#endif                                 /* C++ */

#include <portable.h>

    extern const char * FP_WRITETEXT;
    extern const char * FP_READTEXT;
    extern const char * FP_WRITEBIN;
    extern const char * FP_READBIN;

/***************************************************************************
 * Function : FP_SplitPath
 *
 * Description: splits  file name by path, name, extension
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes:
 *
 **************************************************************************/
    extern LOGICAL          FP_SplitPath( const char * path, char * dir, char * fname, char * ext );

/***************************************************************************
 * Function : FP_NormalizePath
 *
 * Description: change slashes to native for OS
 *
 * Returns: pointer to path
 *
 * Notes: normalize slashes
 *
 **************************************************************************/
    extern char            *FP_NormalizePath( char * path );

/***************************************************************************
 * Function : FP_MakeDir
 *
 * Description: create directories recursively
 *
 * Return: RESULT_ERR/RESULT_OK
 *
 * Notes:
 *
 **************************************************************************/
    extern LOGICAL          FP_MakeDir( const char * path );

/***************************************************************************
 * Function : FP_OpenFile
 *
 * Description: open file, check for error and allocate large in/out buffer
 *
 * Returns: file handle
 *
 * Notes:
 *
 **************************************************************************/
    extern FILE            *FP_OpenFile( const char * s, const char * mode );

/***************************************************************************
 * Function : FP_FileSize
 *
 * Description: return size of opened file
 *
 * Returns: size/0 if null
 *
 * Notes: None
 *
 **************************************************************************/
    extern UINT32_T             FP_FileSize( FILE * f );

/***************************************************************************
 * Function : FP_ExpandMask
 *
 * Description: Expand file mask into set of files
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          FP_ExpandMask( const char * path, size_t * count, char * ** arr );

/***************************************************************************
 * Function : FP_FullName
 *
 * Description: expand relative path to absolute one
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          FP_FullName( char * dest, const char * src );

/***************************************************************************
 * Function : FP_FileExist
 *
 * Description: check file for existence
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL FP_FileExist(const char * filename);

/***************************************************************************
 * Function : FP_FileSearch
 *
 * Description: search file for existence in specified directories
 * with specified extensions.
 * If pathcount==0, search will be conducted in the initial directory
 * If extcount==0, search will be conducted with initial extension
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL FP_FileSearch(size_t *resCount, char *** result, const char * filename, size_t pathcount,
                                 char * paths[], size_t extcount, char * exts[]);

/***************************************************************************
 * Function : FP_GetFileMtime
 *
 * Description: get file modification time
 *
 * Returns: time / 0 
 *
 * Notes: None
 *
 **************************************************************************/
    extern time_t FP_GetFileMtime(const char * fname);

/***************************************************************************
 * Function : FP_AddFileName
 *
 * Description: Add unique filename to array
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL FP_AddFileName(const char * name, size_t *count, char ***arr);

/***************************************************************************
 * Function : FP_AddDirectorySlash
 *
 * Description: add trailing slash to directory name.
 *              buffer shall have enough space.
 *
 * Returns: pointer to dirname
 *
 * Notes: None
 *
 **************************************************************************/
    extern char * FP_AddDirectorySlash(char * dirname);

    extern LOGICAL FP_IsWriteable(const char *filename);

#ifdef __cplusplus
}

#endif                                 /* C++ */

#endif                                 /* FILEPROC_H */
