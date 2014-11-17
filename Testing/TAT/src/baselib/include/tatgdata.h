#ifndef TATGDATA_H
#define TATGDATA_H
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
* Functions:    None
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  Global configuration
*
* Notes:        None
*
******************************************************************************/
#include <mscbase.h>
#include <cfgtypes.h>
#include <xmlconf.h>

extern TAT_XML_CONFIG_DEF *CFG_DefaultConfig;

/* debug file i/o flag. If set, disable buffering */
extern int              TAT_DEBUG_FILE_IO;

extern int              TAT_ParseASN_Data;

extern UINT32_T             TAT_MajorVersion;
extern UINT32_T             TAT_MinorVersion;

extern char             TAT_ZeroStr[];

extern size_t           TAT_MaxFileNameLen;
extern size_t           TAT_MaxIdLen;


/* maximal length of file name */
#ifndef TAT_MAX_FNAME_LEN
#define TAT_MAX_FNAME_LEN 256
#endif

/* maximal line length in file */
#ifndef TAT_MAX_LINE_LEN
#define TAT_MAX_LINE_LEN 4096
#endif

/* maximal length of identifier */
#ifndef TAT_MAX_ID_LEN
#define TAT_MAX_ID_LEN 256
#endif

/* high nested level for file processing */
#ifndef TAT_FILE_ARRAY
#define TAT_FILE_ARRAY 16
#endif

#endif                         /* TATGDATA_H */
