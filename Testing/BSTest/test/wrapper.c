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
* Description:  Test suite "battleshipstest" wrapper functions
*
* Notes:        None
*
******************************************************************************/
/* Standart include files */
#include "wrapper.h"

/* TODO: Add necessary include files */

TAT_TEST_INSTANCE * TI_ID;
static TAT_TIME_TYPE battleshipstest_Clock=0;

static SDL_Time battleshipstest_SDLClock={0,0};
int SDL_Output_Count=0;

#ifdef XTENV
#define RETURN return SDL_Time_Lit((xint32)0,(xint32)0);
#else
#define RETURN return;
#endif

SDL_Time SDL_Clock(void)
{
	return battleshipstest_SDLClock;
}

void SDL_Tick(SDL_Time T)
{
	if(T.s>battleshipstest_SDLClock.s || (T.s==battleshipstest_SDLClock.s && T.ns>battleshipstest_SDLClock.ns)) {
		battleshipstest_SDLClock.s++;
		battleshipstest_SDLClock.ns=0;
	}
	if(T.s<battleshipstest_SDLClock.s || (T.s==battleshipstest_SDLClock.s && T.ns<battleshipstest_SDLClock.ns)) battleshipstest_SDLClock=T;
#ifdef TAT_FLOAT_TIME
	battleshipstest_Clock=(float)(battleshipstest_SDLClock.s)+(float)(battleshipstest_SDLClock.ns)/1000000000.0;
#else
	battleshipstest_Clock=battleshipstest_SDLClock.s;
	if(battleshipstest_SDLClock.ns>=500000000) battleshipstest_Clock++;
#endif
}

#ifndef XNOGLOBALNODENUMBER
extern int xGlobalNodeNumber(void) {return TAT_I_Strategy_ID;}
#endif

#ifndef XNOINITENV
extern void xInitEnv(void) {WrapInit();}
#endif

#ifndef XNOCLOSEENV
extern void xCloseEnv(void) {WrapClose();}
#endif

extern void xOutEnv( xSignalNode *SignalOut )
{
	TAT_INSTANCE_ID TAT_RECEIVER=TAT_I_Simulator_ID;
	if((*SignalOut)->Receiver.GlobalNodeNr!=xEnv.GlobalNodeNr) TAT_RECEIVER=(*SignalOut)->Receiver.GlobalNodeNr;
	if(((*SignalOut)->NameNode)==FinishSimulation) {
		battleshipstest_Recv_FinishSimulation(((yPDP_FinishSimulation)(*SignalOut))->Param1,((yPDP_FinishSimulation)(*SignalOut))->Param2,TAT_I_Strategy_ID,TAT_RECEIVER);
		xReleaseSignal(SignalOut);
		return;
	}
	if(((*SignalOut)->NameNode)==OrderFireRequest) {
		battleshipstest_Recv_OrderFireRequest(((yPDP_OrderFireRequest)(*SignalOut))->Param1,((yPDP_OrderFireRequest)(*SignalOut))->Param2,((yPDP_OrderFireRequest)(*SignalOut))->Param3,TAT_Copy_tStringShotsList(((yPDP_OrderFireRequest)(*SignalOut))->Param4),TAT_I_Strategy_ID,TAT_RECEIVER);
		xReleaseSignal(SignalOut);
		return;
	}
	if(((*SignalOut)->NameNode)==OrderSpeedRequest) {
		battleshipstest_Recv_OrderSpeedRequest(((yPDP_OrderSpeedRequest)(*SignalOut))->Param1,((yPDP_OrderSpeedRequest)(*SignalOut))->Param2,((yPDP_OrderSpeedRequest)(*SignalOut))->Param3,TAT_I_Strategy_ID,TAT_RECEIVER);
		xReleaseSignal(SignalOut);
		return;
	}
	if(((*SignalOut)->NameNode)==OrderTurnRequest) {
		battleshipstest_Recv_OrderTurnRequest(((yPDP_OrderTurnRequest)(*SignalOut))->Param1,((yPDP_OrderTurnRequest)(*SignalOut))->Param2,((yPDP_OrderTurnRequest)(*SignalOut))->Param3,TAT_I_Strategy_ID,TAT_RECEIVER);
		xReleaseSignal(SignalOut);
		return;
	}
	if(((*SignalOut)->NameNode)==messageError) {
		battleshipstest_Recv_messageError(((yPDP_messageError)(*SignalOut))->Param1,TAT_I_Strategy_ID,TAT_RECEIVER);
		xReleaseSignal(SignalOut);
		return;
	}
	if(((*SignalOut)->NameNode)==messageLog) {
		battleshipstest_Recv_messageLog(((yPDP_messageLog)(*SignalOut))->Param1,TAT_I_Strategy_ID,TAT_RECEIVER);
		xReleaseSignal(SignalOut);
		return;
	}
}

#ifndef XTENV
extern void xInEnv ( SDL_Time Time_for_next_event )
#else
extern SDL_Duration xInEnv ( SDL_Time Time_for_next_event )
#endif
{
	SDL_Output_Count=0;
	if(WrapRun()) SDL_Halt();
	if(SDL_Output_Count==0) SDL_Tick(Time_for_next_event);
	RETURN
}

/* Signal to reset SDL system */
TAT_INSTANCE_ID battleshipstest_resetSignalInstances[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
TAT_EVENT_INSTANCE battleshipstest_resetSignal;
/************************************************************************************
*
* Function    : battleshipstest_resetSys
*
* Description : This function invoked from test to initialize testing system
*
* Return      : TAT_TEST_CONTINUE - if all OK
*               TAT_TEST_CRITICAL - if something wrong
*
* Notes       : 
*
*************************************************************************************/
TAT_TEST_RESULT battleshipstest_resetSys(TAT_TEST_INSTANCE *id)
{
	int res = 0;

	battleshipstest_Clock=0;

	/* Reset SDL time */
	battleshipstest_SDLClock.s=0;
	battleshipstest_SDLClock.ns=0;

	/* Send SDL system reset signal */
	battleshipstest_resetSignal.eventType=TAT_SEND_EV;
	battleshipstest_resetSignal.eventID=TAT_S_resetStrategy_ID;
	battleshipstest_resetSignal.eventInstanceSize=2;
	battleshipstest_resetSignal.eventInstance=(TAT_INSTANCE_ID*)&battleshipstest_resetSignalInstances;
	res=id->sendMessage(id,&battleshipstest_resetSignal);

	/* TODO: Add necessary functionality to initialize testing system */

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
* Function    : battleshipstest_clockF
*
* Description : This function invoked from test to get current time value
*
* Return      : Current time value
*
* Notes       : 
*
*************************************************************************************/
TAT_TIME_TYPE battleshipstest_clockF (TAT_TEST_INSTANCE *id)
{
	/* TODO: Add necessary functionality to determine current time */

	return battleshipstest_Clock;
}

/************************************************************************************
*
* Function    : battleshipstest_startCase
*
* Description : This function invoked at the begining of test case
*
* Return      : TAT_TEST_CONTINUE - if all OK
*               TAT_TEST_CRITICAL - if something wrong
*
* Notes       : 
*
*************************************************************************************/
TAT_TEST_RESULT battleshipstest_startCase (TAT_TEST_INSTANCE *id)
{
	/* TODO: Add necessary functionality to execute */
	/* on the begining of each test case running    */

	return TAT_TEST_CONTINUE;
}

/************************************************************************************
*
* Function    : battleshipstest_finishCase
*
* Description : This function invoked at the ending of test case
*
* Return      : TAT_TEST_CONTINUE - if all OK
*               TAT_TEST_CRITICAL - if something wrong
*
* Notes       : 
*
*************************************************************************************/
TAT_TEST_RESULT battleshipstest_finishCase (TAT_TEST_INSTANCE *id)
{
	/* TODO: Add necessary functionality to execute */
	/* on the ending of each test case running    */

	return TAT_TEST_CONTINUE;
}

/************************************************************************************
*
* Function    : battleshipstest_sendMess
*
* Description : This function invoked from test to send signal to testing system
*
* Return      : TAT_TEST_CONTINUE - if all OK
*               TAT_TEST_CRITICAL - if something wrong
*
* Notes       : 
*
*************************************************************************************/
TAT_TEST_RESULT battleshipstest_sendMess(TAT_TEST_INSTANCE *id, TAT_EVENT_INSTANCE *event)
{
	int res = 0;

	xSignalNode SignalIn;
	SDL_PId pid;

	if (!id || !event)
	{
		return TAT_TEST_CRITICAL;
	}

	if (event->eventType == TAT_SEND_EV)
	{
		pid.GlobalNodeNr=event->eventInstance[TAT_INST_FROM];
		pid.LocalPId=NULL;
		switch (event->eventID)
		{
		case TAT_S_DestroyShip_ID:
			{
				SignalIn=xGetSignal(DestroyShip,xNotDefPId,pid);
				((yPDef_DestroyShip *)(SignalIn))->Param1=((TAT_SIG_DestroyShip*)event->eventData)->Param1;
				((yPDef_DestroyShip *)(SignalIn))->Param2=((TAT_SIG_DestroyShip*)event->eventData)->Param2;
				((yPDef_DestroyShip *)(SignalIn))->Param3=((TAT_SIG_DestroyShip*)event->eventData)->Param3;
				SDL_Output(SignalIn xSigPrioPar(xDefaultSignalPrio),(xIdNode *)0);
				SDL_Output_Count++;
				break;
			}
		case TAT_S_InitDataComplete_ID:
			{
				SignalIn=xGetSignal(InitDataComplete,xNotDefPId,pid);
				SDL_Output(SignalIn xSigPrioPar(xDefaultSignalPrio),(xIdNode *)0);
				SDL_Output_Count++;
				break;
			}
		case TAT_S_OrderFireReject_ID:
			{
				SignalIn=xGetSignal(OrderFireReject,xNotDefPId,pid);
				((yPDef_OrderFireReject *)(SignalIn))->Param1=((TAT_SIG_OrderFireReject*)event->eventData)->Param1;
				((yPDef_OrderFireReject *)(SignalIn))->Param2=((TAT_SIG_OrderFireReject*)event->eventData)->Param2;
				SDL_Output(SignalIn xSigPrioPar(xDefaultSignalPrio),(xIdNode *)0);
				SDL_Output_Count++;
				break;
			}
		case TAT_S_OrderFireResponse_ID:
			{
				SignalIn=xGetSignal(OrderFireResponse,xNotDefPId,pid);
				((yPDef_OrderFireResponse *)(SignalIn))->Param1=((TAT_SIG_OrderFireResponse*)event->eventData)->Param1;
				((yPDef_OrderFireResponse *)(SignalIn))->Param2=((TAT_SIG_OrderFireResponse*)event->eventData)->Param2;
				SDL_Output(SignalIn xSigPrioPar(xDefaultSignalPrio),(xIdNode *)0);
				SDL_Output_Count++;
				break;
			}
		case TAT_S_OrderSpeedReject_ID:
			{
				SignalIn=xGetSignal(OrderSpeedReject,xNotDefPId,pid);
				((yPDef_OrderSpeedReject *)(SignalIn))->Param1=((TAT_SIG_OrderSpeedReject*)event->eventData)->Param1;
				((yPDef_OrderSpeedReject *)(SignalIn))->Param2=((TAT_SIG_OrderSpeedReject*)event->eventData)->Param2;
				SDL_Output(SignalIn xSigPrioPar(xDefaultSignalPrio),(xIdNode *)0);
				SDL_Output_Count++;
				break;
			}
		case TAT_S_OrderSpeedResponse_ID:
			{
				SignalIn=xGetSignal(OrderSpeedResponse,xNotDefPId,pid);
				((yPDef_OrderSpeedResponse *)(SignalIn))->Param1=((TAT_SIG_OrderSpeedResponse*)event->eventData)->Param1;
				((yPDef_OrderSpeedResponse *)(SignalIn))->Param2=((TAT_SIG_OrderSpeedResponse*)event->eventData)->Param2;
				SDL_Output(SignalIn xSigPrioPar(xDefaultSignalPrio),(xIdNode *)0);
				SDL_Output_Count++;
				break;
			}
		case TAT_S_OrderTurnReject_ID:
			{
				SignalIn=xGetSignal(OrderTurnReject,xNotDefPId,pid);
				((yPDef_OrderTurnReject *)(SignalIn))->Param1=((TAT_SIG_OrderTurnReject*)event->eventData)->Param1;
				((yPDef_OrderTurnReject *)(SignalIn))->Param2=((TAT_SIG_OrderTurnReject*)event->eventData)->Param2;
				SDL_Output(SignalIn xSigPrioPar(xDefaultSignalPrio),(xIdNode *)0);
				SDL_Output_Count++;
				break;
			}
		case TAT_S_OrderTurnResponse_ID:
			{
				SignalIn=xGetSignal(OrderTurnResponse,xNotDefPId,pid);
				((yPDef_OrderTurnResponse *)(SignalIn))->Param1=((TAT_SIG_OrderTurnResponse*)event->eventData)->Param1;
				((yPDef_OrderTurnResponse *)(SignalIn))->Param2=((TAT_SIG_OrderTurnResponse*)event->eventData)->Param2;
				SDL_Output(SignalIn xSigPrioPar(xDefaultSignalPrio),(xIdNode *)0);
				SDL_Output_Count++;
				break;
			}
		case TAT_S_ShipsCurrentState_ID:
			{
				SignalIn=xGetSignal(ShipsCurrentState,xNotDefPId,pid);
				((yPDef_ShipsCurrentState *)(SignalIn))->Param1=((TAT_SIG_ShipsCurrentState*)event->eventData)->Param1;
				((yPDef_ShipsCurrentState *)(SignalIn))->Param2=((TAT_SIG_ShipsCurrentState*)event->eventData)->Param2;
				SDL_Output(SignalIn xSigPrioPar(xDefaultSignalPrio),(xIdNode *)0);
				SDL_Output_Count++;
				break;
			}
		case TAT_S_StartSimulation_ID:
			{
				SignalIn=xGetSignal(StartSimulation,xNotDefPId,pid);
				((yPDef_StartSimulation *)(SignalIn))->Param1=((TAT_SIG_StartSimulation*)event->eventData)->Param1;
				SDL_Output(SignalIn xSigPrioPar(xDefaultSignalPrio),(xIdNode *)0);
				SDL_Output_Count++;
				break;
			}
		case TAT_S_StrategyLose_ID:
			{
				SignalIn=xGetSignal(StrategyLose,xNotDefPId,pid);
				((yPDef_StrategyLose *)(SignalIn))->Param1=((TAT_SIG_StrategyLose*)event->eventData)->Param1;
				SDL_Output(SignalIn xSigPrioPar(xDefaultSignalPrio),(xIdNode *)0);
				SDL_Output_Count++;
				break;
			}
		case TAT_S_StrategyVictory_ID:
			{
				SignalIn=xGetSignal(StrategyVictory,xNotDefPId,pid);
				((yPDef_StrategyVictory *)(SignalIn))->Param1=((TAT_SIG_StrategyVictory*)event->eventData)->Param1;
				SDL_Output(SignalIn xSigPrioPar(xDefaultSignalPrio),(xIdNode *)0);
				SDL_Output_Count++;
				break;
			}
		case TAT_S_VisibleShip_ID:
			{
				SignalIn=xGetSignal(VisibleShip,xNotDefPId,pid);
				((yPDef_VisibleShip *)(SignalIn))->Param1=((TAT_SIG_VisibleShip*)event->eventData)->Param1;
				((yPDef_VisibleShip *)(SignalIn))->Param2=((TAT_SIG_VisibleShip*)event->eventData)->Param2;
				SDL_Output(SignalIn xSigPrioPar(xDefaultSignalPrio),(xIdNode *)0);
				SDL_Output_Count++;
				break;
			}
		case TAT_S_resetStrategy_ID:
			{
				SignalIn=xGetSignal(resetStrategy,xNotDefPId,pid);
				SDL_Output(SignalIn xSigPrioPar(xDefaultSignalPrio),(xIdNode *)0);
				SDL_Output_Count++;
				break;
			}
		default:
			{
				puts("ERROR: (wrapper) unknown signal to transmit to model.");
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

TAT_SIG_FinishSimulation model_sigFinishSimulation = { 0, 0 };
TAT_INSTANCE_ID instances_evFinishSimulation[] = {0, 0};
TAT_EVENT_INSTANCE model_evFinishSimulation = { TAT_RECV_EV, TAT_S_FinishSimulation_ID, &model_sigFinishSimulation, {{0,0}, {0,0}, ""},2,
 instances_evFinishSimulation, NULL, 0, NULL, TAT_VERDICT_PASS, -1, NULL, NULL};

TAT_SIG_OrderFireRequest model_sigOrderFireRequest = { 0, 0, 0, 0 };
TAT_INSTANCE_ID instances_evOrderFireRequest[] = {0, 0};
TAT_EVENT_INSTANCE model_evOrderFireRequest = { TAT_RECV_EV, TAT_S_OrderFireRequest_ID, &model_sigOrderFireRequest, {{0,0}, {0,0}, ""},2,
 instances_evOrderFireRequest, NULL, 0, NULL, TAT_VERDICT_PASS, -1, NULL, NULL};

TAT_SIG_OrderSpeedRequest model_sigOrderSpeedRequest = { 0, 0, 0 };
TAT_INSTANCE_ID instances_evOrderSpeedRequest[] = {0, 0};
TAT_EVENT_INSTANCE model_evOrderSpeedRequest = { TAT_RECV_EV, TAT_S_OrderSpeedRequest_ID, &model_sigOrderSpeedRequest, {{0,0}, {0,0}, ""},2,
 instances_evOrderSpeedRequest, NULL, 0, NULL, TAT_VERDICT_PASS, -1, NULL, NULL};

TAT_SIG_OrderTurnRequest model_sigOrderTurnRequest = { 0, 0, 0 };
TAT_INSTANCE_ID instances_evOrderTurnRequest[] = {0, 0};
TAT_EVENT_INSTANCE model_evOrderTurnRequest = { TAT_RECV_EV, TAT_S_OrderTurnRequest_ID, &model_sigOrderTurnRequest, {{0,0}, {0,0}, ""},2,
 instances_evOrderTurnRequest, NULL, 0, NULL, TAT_VERDICT_PASS, -1, NULL, NULL};

TAT_SIG_messageError model_sigmessageError = { 0 };
TAT_INSTANCE_ID instances_evmessageError[] = {0, 0};
TAT_EVENT_INSTANCE model_evmessageError = { TAT_RECV_EV, TAT_S_messageError_ID, &model_sigmessageError, {{0,0}, {0,0}, ""},2,
 instances_evmessageError, NULL, 0, NULL, TAT_VERDICT_PASS, -1, NULL, NULL};

TAT_SIG_messageLog model_sigmessageLog = { 0 };
TAT_INSTANCE_ID instances_evmessageLog[] = {0, 0};
TAT_EVENT_INSTANCE model_evmessageLog = { TAT_RECV_EV, TAT_S_messageLog_ID, &model_sigmessageLog, {{0,0}, {0,0}, ""},2,
 instances_evmessageLog, NULL, 0, NULL, TAT_VERDICT_PASS, -1, NULL, NULL};

/************************************************************************************
*
* Function    : battleshipstest_Recv_FinishSimulation
*
* Description : This function transfer signals from testing system to test
*
* Return      : 1
*
* Notes       : 
*
*************************************************************************************/
int battleshipstest_Recv_FinishSimulation(int Param1, int Param2, int from, int to)
{
	model_sigFinishSimulation.Param1 = Param1;
	model_sigFinishSimulation.Param2 = Param2;
	instances_evFinishSimulation[0] = from;
	instances_evFinishSimulation[1] = to;
	TAT_ReceiveMessage(TI_ID, &model_evFinishSimulation);
	return 1;
}

/************************************************************************************
*
* Function    : battleshipstest_Recv_OrderFireRequest
*
* Description : This function transfer signals from testing system to test
*
* Return      : 1
*
* Notes       : 
*
*************************************************************************************/
int battleshipstest_Recv_OrderFireRequest(int Param1, int Param2, int Param3, tStringShotsList Param4, int from, int to)
{
	model_sigOrderFireRequest.Param1 = Param1;
	model_sigOrderFireRequest.Param2 = Param2;
	model_sigOrderFireRequest.Param3 = Param3;
	model_sigOrderFireRequest.Param4 = Param4;
	instances_evOrderFireRequest[0] = from;
	instances_evOrderFireRequest[1] = to;
	TAT_ReceiveMessage(TI_ID, &model_evOrderFireRequest);
	return 1;
}

/************************************************************************************
*
* Function    : battleshipstest_Recv_OrderSpeedRequest
*
* Description : This function transfer signals from testing system to test
*
* Return      : 1
*
* Notes       : 
*
*************************************************************************************/
int battleshipstest_Recv_OrderSpeedRequest(int Param1, int Param2, float Param3, int from, int to)
{
	model_sigOrderSpeedRequest.Param1 = Param1;
	model_sigOrderSpeedRequest.Param2 = Param2;
	model_sigOrderSpeedRequest.Param3 = Param3;
	instances_evOrderSpeedRequest[0] = from;
	instances_evOrderSpeedRequest[1] = to;
	TAT_ReceiveMessage(TI_ID, &model_evOrderSpeedRequest);
	return 1;
}

/************************************************************************************
*
* Function    : battleshipstest_Recv_OrderTurnRequest
*
* Description : This function transfer signals from testing system to test
*
* Return      : 1
*
* Notes       : 
*
*************************************************************************************/
int battleshipstest_Recv_OrderTurnRequest(int Param1, int Param2, int Param3, int from, int to)
{
	model_sigOrderTurnRequest.Param1 = Param1;
	model_sigOrderTurnRequest.Param2 = Param2;
	model_sigOrderTurnRequest.Param3 = Param3;
	instances_evOrderTurnRequest[0] = from;
	instances_evOrderTurnRequest[1] = to;
	TAT_ReceiveMessage(TI_ID, &model_evOrderTurnRequest);
	return 1;
}

/************************************************************************************
*
* Function    : battleshipstest_Recv_messageError
*
* Description : This function transfer signals from testing system to test
*
* Return      : 1
*
* Notes       : 
*
*************************************************************************************/
int battleshipstest_Recv_messageError(SDL_Charstring Param1, int from, int to)
{
	model_sigmessageError.Param1 = Param1;
	instances_evmessageError[0] = from;
	instances_evmessageError[1] = to;
	TAT_ReceiveMessage(TI_ID, &model_evmessageError);
	return 1;
}

/************************************************************************************
*
* Function    : battleshipstest_Recv_messageLog
*
* Description : This function transfer signals from testing system to test
*
* Return      : 1
*
* Notes       : 
*
*************************************************************************************/
int battleshipstest_Recv_messageLog(SDL_Charstring Param1, int from, int to)
{
	model_sigmessageLog.Param1 = Param1;
	instances_evmessageLog[0] = from;
	instances_evmessageLog[1] = to;
	TAT_ReceiveMessage(TI_ID, &model_evmessageLog);
	return 1;
}

TAT_TEST_RESULT res;

int WrapInit()
{
	TI_ID = TAT_CreateTestInstance(128, 3, 4096, battleshipstest_resetSys,
 												 battleshipstest_sendMess,
 												 battleshipstest_clockF,
 												 battleshipstest_startCase,
 												 battleshipstest_finishCase);
	if (!TI_ID)
	{
		printf("ERROR: can't initialize test instance\n");
		return 1;
	}

	/* Register dumping modules */
	TAT_RegisterLogFunc(TI_ID, TAT_OpenLogMPR, TAT_LogFuncMPR, TAT_CloseLogMPR);
	TAT_RegisterLogFunc(TI_ID, TAT_OpenLogTXT, TAT_LogFuncTXT, TAT_CloseLogTXT);

	/* Call generated function to additional initializing test instance structure */
	res = TAT_StartSuite(TI_ID, TAT_Init_battleshipstest);

	return (res==TAT_TEST_CRITICAL);
}

int WrapRun()
{
	/* Run test suite */
	if(res == TAT_TEST_CONTINUE) {
		res = TAT_ContinueSuite(TI_ID);
		return 0;
	}
	return 1;
}

int WrapClose()
{
	TAT_FreeTestInstance(TI_ID);
	return (res==TAT_TEST_CRITICAL);
}
