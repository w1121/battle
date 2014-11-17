/* TEST_SUITE HEADER FILE */
#ifndef _MPRLOG_H_
#define _MPRLOG_H_
/******************************************************************************
*
*       COPYRIGHT 2001-2002 MOTOROLA, ALL RIGHTS RESERVED
*
*       The copyright notice above does not evidence any
*       actual or intended publication of such source code.
*
* Functions:    
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  MSC-PR dumping functions for C code generation templates
*
* Notes:        None
*
******************************************************************************/

#ifndef TAT_MAX_MPR_LOG_STR_LEN
#define TAT_MAX_MPR_LOG_STR_LEN 4096
#endif /* TAT_MAX_DBG_LOG_STR_LEN */

TAT_TEST_RESULT TAT_OpenLogMPR(TAT_TEST_INSTANCE*);
TAT_TEST_RESULT TAT_LogFuncMPR(TAT_TEST_INSTANCE*, TAT_EVENT_INSTANCE*, TAT_EVENT_INSTANCE*);
TAT_TEST_RESULT TAT_CloseLogMPR(TAT_TEST_INSTANCE*);
int TAT_GetInstListMPR(TAT_TEST_INSTANCE*, TAT_EVENT_INSTANCE*, char*);

#endif
