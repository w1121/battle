#ifndef PORTABLE_H
#define PORTABLE_H
/******************************************************************************
*
*       COPYRIGHT 2001-2004 MOTOROLA, ALL RIGHTS RESERVED
*
*       MOTOROLA CONFIDENTIAL PROPRIETARY
*
* Description:  Command-line processing
*
*
* CHANGE HISTORY
*                                                       
* Global macros defined in this header:
*   COMPILER :== 0 (MSVC) | 1 (GNU) | 2 (BCC) | 3 (ICC)
*   OSTYPE :== 0 (SUNOS) | 1 (WIN32) | 2 (WIN64) | 3 (HPUX)
*   LIBS :== 0 (MSVC) | 1 (POSIX) | 2 (BCC)
*   FILE_NAME_COMPARE :== stricmp | strcmp
*
******************************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include <time.h>

#define MSVC 0
#define GNU 1
#define BCC 2
#define ICC 3

#define POSIX 1
#define SUNOS 0
#define WIN32 1
#define WIN64 2
#define HPUX 3
#define LINUX 4



/*********************************************************************/
/*********************************************************************/
/* Configuration for Visual Studio Compiler / INTEL C for Win32      */
/*********************************************************************/
/*********************************************************************/

#if defined(_MSC_VER) || (defined(__INTEL_COMPILER) && defined(_WIN32))
#include <io.h>
#include <direct.h>
#include <string.h>
#include <signal.h>
#include <process.h>
/* avoid multiple inclusion of this file */
#pragma once

/* #include <windows.h> */
#define OSTYPE WIN32
#define COMPILER MSVC
#define LIBS MSVC

typedef unsigned __int64    UINT64_T;
typedef __int64             INT64_T;
#define atoi64(a) _atoi64(a)
#define _FAST  __fastcall
#define _INLINE __inline
#define _DLLIMPORT   __declspec( dllimport )
#define _DLLEXPORT   __declspec( dllexport )

#define wmkdir _wmkdir
#define wcsfnamecmp _wcsicmp

#if defined(__STDC__)
#define chdir _chdir
#define access _access
#define mkdir _mkdir
#define getcwd _getcwd
#define strcmpi _strcmpi
#define stricmp _stricmp
#define strnicmp _strnicmp
#define strrev  _strrev
#define putenv  _putenv
#define stat _stat
#endif /* __STDC__ */

/* support for access function */
#define R_OK   4
#define W_OK   2

#define NORMAL_FILE_SLASH '\\'
#define OTHER_FILE_SLASH '/'
#define FILE_NAME_COMPARE stricmp

#define MAKELANGID(p, s) ((((unsigned short)(s)) << 10) | (unsigned short )(p))
extern void            *__stdcall LoadLibraryA( const char *file );
extern void            *__stdcall GetProcAddress( void *handle,  const char *name );
extern int __stdcall    FreeLibrary( void *handle );
extern void * __stdcall LocalFree( void *handle );
extern int __stdcall    GetLastError();
extern void __stdcall   Sleep(unsigned int Milliseconds);

extern int __stdcall    FormatMessageA ( int dwFlags, void * lpSource,
                                        int dwMessageId, int dwLanguageId,
                                        char * lpBuffer, int nSize, va_list * );

extern void * __stdcall CreateFileMapping(
  void * hFile,                       /* handle to file */
  void * lpAttributes,                /* security (NULL) */
  unsigned int flProtect,                   /* protection */
  unsigned int dwMaximumSizeHigh,           /* high-order DWORD of size */
  unsigned int dwMaximumSizeLow,            /* low-order DWORD of size */
  char * lpName                       /* object name */
);

extern void * __stdcall  MapViewOfFileEx(
  void * hFileMappingObject,   /* handle to file-mapping object */
  unsigned int dwDesiredAccess,       /* access mode */
  unsigned int dwFileOffsetHigh,      /* high-order DWORD of offset */
  unsigned int dwFileOffsetLow,       /* low-order DWORD of offset */
  size_t dwNumberOfBytesToMap, /* number of bytes to map */
  void * lpBaseAddress         /* starting address */
);

extern int __stdcall CloseHandle(void * hObject );

extern int __stdcall __HrLoadAllImportsForDll (char * szDll);

#define WSADESCRIPTION_LEN      256
#define WSASYS_STATUS_LEN       128

typedef struct WSAData {
        unsigned short                    wVersion;
        unsigned short                    wHighVersion;
#ifdef _WIN64
        unsigned short          iMaxSockets;
        unsigned short          iMaxUdpDg;
        char *              lpVendorInfo;
        char                    szDescription[WSADESCRIPTION_LEN+1];
        char                    szSystemStatus[WSASYS_STATUS_LEN+1];
#else
        char                    szDescription[WSADESCRIPTION_LEN+1];
        char                    szSystemStatus[WSASYS_STATUS_LEN+1];
        unsigned short          iMaxSockets;
        unsigned short          iMaxUdpDg;
        char *              lpVendorInfo;
#endif
} WSADATA;

extern int __stdcall WSACleanup();

extern int __stdcall gethostname(char *name, int namelen);

extern int __stdcall WSAStartup(unsigned short *wVersionRequired, WSADATA *data);

#if !defined(MAKEWORD)
#define MAKEWORD(low,high) \
        ((unsigned short)(((unsigned char)(low)) | ((unsigned short)((unsigned char)(high))) << 8))
#endif

#endif /* _MSC_VER */

/*********************************************************************/
/*********************************************************************/
/* configuration for GNU C compiler                                  */
/*********************************************************************/
/*********************************************************************/
#if defined(__GNUC__)
#include <string.h>
#include <memory.h>
#include <unistd.h>
#include <dirent.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <sys/unistd.h>

#define COMPILER GNU

#define  stricmp strcasecmp
#define  strnicmp strncasecmp

#ifdef __STRICT_ANSI__          /* defined by gcc -ansi */
extern int              strncasecmp( const char *, const char *, size_t );
extern int              strcasecmp( const char *, const char * );
#endif /* __STRICT_ANSI__ */
#ifndef S_IFDIR
#define S_IFDIR       0040000 /* Directory.  */   
#endif
#ifndef S_ISUID
#define S_ISUID 04000
#endif
#ifndef S_ISGID
#define S_ISGID 02000
#endif
#ifndef S_ISVTX
#define S_ISVTX 01000
#endif
#ifndef S_IRUSR
#define S_IRUSR 0200
#endif
#ifndef S_IWUSR
#define S_IWUSR 0200
#endif
#ifndef S_IXUSR
#define S_IXUSR 0100
#endif
#ifndef S_IRWXU
#define S_IRWXU (S_IRUSR | S_IWUSR | S_IXUSR)
#endif

typedef unsigned long long  UINT64_T;
typedef long long           INT64_T;
#define atoi64(a) strtoull(a, (char **) NULL, 10);

#define _FAST __attribute__((fastcall))

#if defined(__CYGWIN__)
#include <sys/cygwin.h>
#define LIBS POSIX
#define OSTYPE WIN32
#define _DLLIMPORT   __attribute__((dllimport))
#define _DLLEXPORT   __attribute__((dllexport))
#define NORMAL_FILE_SLASH '/'
#define OTHER_FILE_SLASH '\\'
#define FILE_NAME_COMPARE stricmp

#elif defined (__hpux__)
#define LIBS POSIX
#define OSTYPE HPUX
#define _DLLIMPORT   
#define _DLLEXPORT
#define NORMAL_FILE_SLASH '/'
#define OTHER_FILE_SLASH '\\'
#define FILE_NAME_COMPARE strcmp

#elif defined (__sun__)
#define LIBS POSIX
#define OSTYPE SUNOS
#define _DLLIMPORT   
#define _DLLEXPORT
#define NORMAL_FILE_SLASH '/'
#define OTHER_FILE_SLASH '\\'
#define FILE_NAME_COMPARE strcmp

#elif defined (__linux__)
#define LIBS POSIX
#define OSTYPE LINUX
#define _DLLIMPORT   
#define _DLLEXPORT
#define NORMAL_FILE_SLASH '/'
#define OTHER_FILE_SLASH '\\'
#define FILE_NAME_COMPARE strcmp

#endif /* __CYGWIN__ */

#define wcsfnamecmp wcscmp


#endif /* __GNUC__ */

#ifdef __BORLANDC__
#define COMPILER BCC
#define OSTYPE WIN32
#define LIBS BCC
#define _DLLIMPORT   __declspec( dllimport )
#define _DLLEXPORT   __declspec( dllexport )
#define NORMAL_FILE_SLASH '/'
#define OTHER_FILE_SLASH '\\'
#define FILE_NAME_COMPARE stricmp
#define _FAST  __fastcall
typedef unsigned __int64    UINT64_T;
typedef __int64             INT64_T;
#define atoi64(a) _atoi64(a)
#define _INLINE __inline

#endif

/*********************************************************************/
/*********************************************************************/
/* common configuration */
/*********************************************************************/
/*********************************************************************/


/* define common data types */
typedef unsigned int        UINT32_T;
typedef signed int          INT32_T;
typedef unsigned char       UINT8_T;
typedef signed char         INT8_T;
typedef unsigned short int  UINT16_T;
typedef signed short int    INT16_T;

/* define basic logical type */
typedef enum _LOGICAL
{ LOGICAL_FALSE = 0,
  LOGICAL_TRUE = 1,
  LOGICAL_UNDEF = -1
} LOGICAL;

#define RESULT_ERR       LOGICAL_FALSE
#define RESULT_OK        LOGICAL_TRUE

/* define common data types */
typedef int             Bool;
typedef unsigned char   Byte;


#ifndef SPACE
#define SPACE (char)32
#endif

#ifndef TAB
#define TAB (char)9
#endif

#ifndef LF
#define LF (char)10
#endif

#ifndef CR
#define CR (char)13
#endif

#ifndef COMMA
#define COMMA (char)','
#endif


#ifndef PATH_MAX
#define PATH_MAX 256
#endif

#ifndef SYS_NORMAL_EXIT_CODE
#define SYS_NORMAL_EXIT_CODE 0
#endif

#ifndef SYS_ERROR_EXIT_CODE
#define SYS_ERROR_EXIT_CODE 1
#endif

/* Common definitions for messages */
#ifndef DEF_MSG
#define DEF_MSG(name, value) const char * name=value
#endif

#ifndef DEF_MSG_EXT
#define DEF_MSG_EXT(name, value) extern const char * name  /* value */
#endif

#endif                         /* PORTABLE_H */
