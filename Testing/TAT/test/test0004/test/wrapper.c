/******************************************************************************
*
*       COPYRIGHT 2001-2003 MOTOROLA, ALL RIGHTS RESERVED
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
* Description:  Test suite "TAT_Test" wrapper functions
*
* Notes:        None
*
******************************************************************************/
/* Standart include files */
#include "wrapper.h"

/* TODO: Add necessary include files */

TAT_TEST_INSTANCE * TI_ID;
static TAT_TIME_TYPE TAT_Test_Clock=0;

/************************************************************************************
*
* Function    : TAT_Test_resetSys
*
* Description : This funtion invoked from test to initialize testing system
*
* Return      : TAT_TEST_CONTINUE - if all OK
*               TAT_TEST_CRITICAL - if something wrong
*
* Notes       : 
*
*************************************************************************************/
TAT_TEST_RESULT TAT_Test_resetSys(TAT_TEST_INSTANCE *id)
{
	int res = 0;

	TAT_Test_Clock=0;

	/* TODO: Add necessary functionality to initialaize testing system */

	if ( res != 0 )
	{
		id->testStatus = TAT_ERR_RESET_IUT;
		return TAT_TEST_CRITICAL;
	}
	else
		return TAT_TEST_CONTINUE;
}

/************************************************************************************
*
* Function    : TAT_Test_clockF
*
* Description : This funtion invoked from test to get current time value
*
* Return      : Current time value
*
* Notes       : 
*
*************************************************************************************/
TAT_TIME_TYPE TAT_Test_clockF (TAT_TEST_INSTANCE *id)
{
	/* TODO: Add necessary functionality to determine current time */

	return TAT_Test_Clock;
}

/************************************************************************************
*
* Function    : TAT_Test_startCase
*
* Description : This funtion invoked at the begining of test case
*
* Return      : TAT_TEST_CONTINUE - if all OK
*               TAT_TEST_CRITICAL - if something wrong
*
* Notes       : 
*
*************************************************************************************/
TAT_TEST_RESULT TAT_Test_startCase (TAT_TEST_INSTANCE *id)
{
	/* TODO: Add necessary functionality to execute */
	/* on the begining of each test case running    */

	return TAT_TEST_CONTINUE;
}

/************************************************************************************
*
* Function    : TAT_Test_finishCase
*
* Description : This funtion invoked at the ending of test case
*
* Return      : TAT_TEST_CONTINUE - if all OK
*               TAT_TEST_CRITICAL - if something wrong
*
* Notes       : 
*
*************************************************************************************/
TAT_TEST_RESULT TAT_Test_finishCase (TAT_TEST_INSTANCE *id)
{
	/* TODO: Add necessary functionality to execute */
	/* on the ending of each test case running    */

	return TAT_TEST_CONTINUE;
}

/************************************************************************************
*
* Function    : TAT_Test_sendMess
*
* Description : This funtion invoked from test to send signal to testing system
*
* Return      : TAT_TEST_CONTINUE - if all OK
*               TAT_TEST_CRITICAL - if something wrong
*
* Notes       : 
*
*************************************************************************************/
TAT_TEST_RESULT TAT_Test_sendMess(TAT_TEST_INSTANCE *id, TAT_EVENT_INSTANCE *event)
{
	int res = 0;
	CharString s1,s2;

	if (!id || !event)
	{
		return TAT_TEST_CRITICAL;
	}

	if (event->eventType == TAT_SEND_EV)
	{
		switch (event->eventID)
		{
		case TAT_S_A_ID:
			{
				/*res = TAT_Test_A_Handler(((TAT_SIG_A *)event->eventData)->Param1,((TAT_SIG_A *)event->eventData)->Param2);*/
				s1=(char*)malloc(strlen(((TAT_SIG_A *)event->eventData)->Param2)+1);
				strcpy(s1,((TAT_SIG_A *)event->eventData)->Param2);
				s2=(char*)malloc(strlen(((TAT_SIG_A *)event->eventData)->Param1)+1);
				strcpy(s2,((TAT_SIG_A *)event->eventData)->Param1);
				TAT_Test_Recv_B(s1,s2,TAT_I_IUT_ID,TAT_I_Test_ID);
				break;
			}
		default:
			{
				puts("ERROR: (wrapper) unknown signal to transmite to model.");
				res = 1;
			}
		}
	}

	if ( res != 0 )
	{
		id->testStatus = TAT_ERR_SEND_EVENT;
		return TAT_TEST_CRITICAL;
	}
	else
		return TAT_TEST_CONTINUE;
}

TAT_SIG_B model_sigB = { 0, 0 };
TAT_INSTANCE_ID instances_evB[] = {0, 0};
TAT_EVENT_INSTANCE model_evB = { TAT_RECV_EV, TAT_S_B_ID, &model_sigB, {{0,0}, {0,0}, ""},2,
 instances_evB, NULL, 0, NULL, TAT_VERDICT_PASS, -1, NULL, NULL};

/************************************************************************************
*
* Function    : TAT_Test_Recv_B
*
* Description : This funtion transfer signals from testing system to test
*
* Return      : 1
*
* Notes       : 
*
*************************************************************************************/
int TAT_Test_Recv_B(CharString Param1, CharString Param2, int from, int to)
{
	model_sigB.Param1 = Param1;
	model_sigB.Param2 = Param2;
	instances_evB[0] = from;
	instances_evB[1] = to;
	TAT_ReceiveMessage(TI_ID, &model_evB);
	return 1;
}

int main(int argc, char *argv[])
{
	TAT_TEST_RESULT res;

	/* Create and initialize test instance structure */
	TI_ID = TAT_CreateTestInstance(128, 3, 4096, TAT_Test_resetSys,
 												 TAT_Test_sendMess,
 												 TAT_Test_clockF,
 												 TAT_Test_startCase,
 												 TAT_Test_finishCase);
	if (!TI_ID)
	{
		printf("ERROR: can't initialize test instance\n");
		return 1;
	}

	/* Register dumping modules */
	TAT_RegisterLogFunc(TI_ID, TAT_OpenLogMPR, TAT_LogFuncMPR, TAT_CloseLogMPR);
	TAT_RegisterLogFunc(TI_ID, TAT_OpenLogTXT, TAT_LogFuncTXT, TAT_CloseLogTXT);

	/* Call generated function to additional initializing test instance structure */
	res = TAT_StartSuite(TI_ID, TAT_Init_TAT_Test);

	/* Run test suite */
	while (res == TAT_TEST_CONTINUE)
	{
		res = TAT_ContinueSuite(TI_ID);

		/* TODO: Realize functionality to transfer of control to testing system */
		/*       to get response to test`s action.                              */

	}

	if (res == TAT_TEST_CRITICAL)
	{
		printf("ERROR: critical error during execution\n");
	}

	/* Free test instance structure */
	TAT_FreeTestInstance(TI_ID);
	return (res==TAT_TEST_CRITICAL);
}
