/* TEST_SUITE HEADER FILE */
#ifndef _TXTLOG_H_
#define _TXTLOG_H_
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
* Description:  Text dumping functions for C code generation templates
*
* Notes:        None
*
******************************************************************************/

#ifndef TAT_MAX_TXT_LOG_STR_LEN
#define TAT_MAX_TXT_LOG_STR_LEN 4096
#endif /* TAT_MAX_TXT_LOG_STR_LEN */

TAT_TEST_RESULT TAT_OpenLogTXT(TAT_TEST_INSTANCE*);
TAT_TEST_RESULT TAT_LogFuncTXT(TAT_TEST_INSTANCE*, TAT_EVENT_INSTANCE*, TAT_EVENT_INSTANCE*);
TAT_TEST_RESULT TAT_CloseLogTXT(TAT_TEST_INSTANCE*);

int TAT_GetInstListTXT(TAT_TEST_INSTANCE*, TAT_EVENT_INSTANCE*, char*);
int TAT_GetQueueSignals (TAT_TEST_INSTANCE*);

#endif
