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
#include <portable.h>
#include <tatgdata.h>

TAT_XML_CONFIG_DEF     *CFG_DefaultConfig = NULL;

int                     TAT_DEBUG_FILE_IO = 0;  /* if set, all output is not buffered */

/* parse ASN.1 braces or skip (0) */
int                     TAT_ParseASN_Data = 0;

char                    TAT_ZeroStr[] = { 7 };

/* constant  = 256 */
size_t                  TAT_MaxFileNameLen = 8;
size_t                  TAT_MaxIdLen = 4;

