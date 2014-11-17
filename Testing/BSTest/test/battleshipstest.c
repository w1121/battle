#include "battleshipstest.h"

static char * tat_battleshipstest_Test0001N0_MessageTable[] = {
	"InitDataComplete()",
	"ShipsCurrentState(1,(: (. 1, 1, CRUISER, 5, SOUTH_EAST, 70, (. 13, 15 .), (: 15, 15, 3, 0 :) .) :))",
	"StartSimulation(0)",
	"OrderTurnRequest(1,1,LEFT)",
	"FinishSimulation(0,1)"
};
static TAT_SRC_FILE_DEF tat_battleshipstest_Test0001N0_SrcFiles[] = {
	{"C:\\Work\\Testing\\BSTest\\mpr\\Test0001.mpr", 1, 5, 14 },
	{"C:\\Work\\Testing\\BSTest\\work\\Test0001N0.mpr", 2, 5, 14 }};
TAT_TEST_RESULT TAT_TI_battleshipstest_Test0001N0( TAT_TEST_INSTANCE* id )
{
	id->varDefs = NULL;
	id->timeLabels = NULL;
	id->srcFiles = (TAT_SRC_FILE_DEF*)&tat_battleshipstest_Test0001N0_SrcFiles;
	id->timeLabelCount = 0;
	id->srcFilesCount = 2;
	id->varCount = 0;
	id->branchCount = 0;
	id->repeatCount = 1;
	id->timerCount  = 0;
	id->loopCount   = 0;
	return TAT_TEST_CONTINUE;
}
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0001N0_N0[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0001N0_N1[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0001N0_N2[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0001N0_N3[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0001N0_N4[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0001N0_0_SrcRef[] = {{0, 5}, {1, 5}};
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0001N0_0_event = {TAT_SEND_EV, TAT_S_InitDataComplete_ID, NULL, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0001N0_N0, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0001N0_0_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0001N0_MessageTable[0],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0001N0_1_SrcRef[] = {{0, 7}, {1, 7}};
static TAT_SIG_ShipsCurrentState tat_battleshipstest_Test0001N0_1_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0001N0_1_event = {TAT_SEND_EV, TAT_S_ShipsCurrentState_ID, &tat_battleshipstest_Test0001N0_1_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0001N0_N1, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0001N0_1_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0001N0_MessageTable[1],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0001N0_2_SrcRef[] = {{0, 9}, {1, 9}};
static TAT_SIG_StartSimulation tat_battleshipstest_Test0001N0_2_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0001N0_2_event = {TAT_SEND_EV, TAT_S_StartSimulation_ID, &tat_battleshipstest_Test0001N0_2_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0001N0_N2, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0001N0_2_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0001N0_MessageTable[2],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0001N0_3_SrcRef[] = {{0, 11}, {1, 11}};
static TAT_SIG_OrderTurnRequest tat_battleshipstest_Test0001N0_3_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0001N0_3_event = {TAT_RECV_EV, TAT_S_OrderTurnRequest_ID, &tat_battleshipstest_Test0001N0_3_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0001N0_N3, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0001N0_3_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0001N0_MessageTable[3],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0001N0_4_SrcRef[] = {{0, 13}, {1, 13}};
static TAT_SIG_FinishSimulation tat_battleshipstest_Test0001N0_4_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0001N0_4_event = {TAT_RECV_EV, TAT_S_FinishSimulation_ID, &tat_battleshipstest_Test0001N0_4_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0001N0_N4, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0001N0_4_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0001N0_MessageTable[4],
NULL};

tStringShipCurrentState TAT_Ass_Test0001N0_1_1 (TAT_TEST_INSTANCE *id) {
	tStringShipCurrentState x;
	x.First=(tStringShipCurrentState_yrec*)malloc(sizeof(tStringShipCurrentState_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringShipCurrentState_Cur=x.First;
	TAT_tStringShipCurrentState_Cur->Suc=NULL;
	TAT_tStringShipCurrentState_Cur->Data.ShipId=1;
	TAT_tStringShipCurrentState_Cur->Data.StrategyId=1;
	TAT_tStringShipCurrentState_Cur->Data.TypeShip=CRUISER;
	TAT_tStringShipCurrentState_Cur->Data.Speed=5;
	TAT_tStringShipCurrentState_Cur->Data.Heading=SOUTH_EAST;
	TAT_tStringShipCurrentState_Cur->Data.Resource=70;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.x=13;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.y=15;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[0]=15;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[1]=15;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[2]=3;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[3]=0;
	return x;
}
static TAT_TEST_RESULT tat_Test0001N0_state0(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0001N0_0_event, sizeof(__ev));
			if (TAT_Send_InitDataComplete(id, &__ev, &tat_battleshipstest_Test0001N0_0_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=1;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0001N0_state1(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0001N0_1_event, sizeof(__ev));
			if (TAT_Send_ShipsCurrentState(id, &__ev, &tat_battleshipstest_Test0001N0_1_event, 1, TAT_Ass_Test0001N0_1_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=2;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0001N0_state2(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0001N0_2_event, sizeof(__ev));
			if (TAT_Send_StartSimulation(id, &__ev, &tat_battleshipstest_Test0001N0_2_event, 0)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=3;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0001N0_state3(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_OrderTurnRequest_ID)&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param1==(1)))&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param2==(1)))&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param3==(LEFT)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0001N0_3_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=4;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0001N0_state4(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_FinishSimulation_ID)&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param1==(0)))&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param2==(1)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0001N0_4_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=-1;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
TAT_TEST_RESULT TAT_T_battleshipstest_Test0001N0( TAT_TEST_INSTANCE* id )
{
	static const TAT_STATE_FUNC states[5]={
	tat_Test0001N0_state0,tat_Test0001N0_state1,tat_Test0001N0_state2,tat_Test0001N0_state3,tat_Test0001N0_state4};
	TAT_TEST_RESULT res;
	int prevState;
	if( id == NULL )
	{
		id->testStatus = TAT_ERR_INTERNAL;
		id->testResult = TAT_TEST_CRITICAL;
		id->dumpEvent(id, NULL, NULL);
		return TAT_TEST_CRITICAL;
	}
	while( id->nextState != TAT_FINAL_STATE )
	{
		prevState=id->nextState;
		res = states[id->nextState](id);
		if (res == TAT_TEST_CRITICAL) return TAT_TEST_CRITICAL;
		if (res == TAT_TEST_FINISHED && id->testStatus != TAT_OK)
			return TAT_TEST_FINISHED;
		if (res == TAT_TEST_FINISHED && id->nextState != TAT_FINAL_STATE && id->nextState==prevState)
			return TAT_TEST_CONTINUE;
	}
	return TAT_TEST_FINISHED;
}


/****************************************************************************/
static char * tat_battleshipstest_Test0008N0_MessageTable[] = {
	"InitDataComplete()",
	"ShipsCurrentState(1,(: (. 1, 1, CRUISER, 8, EAST, 70, (. 1, 1 .), (: 15, 15, 3, 0 :) .) :))",
	"VisibleShip(1,(: (. 2, 2, REPAIR_BOAT, 0, NORTH_WEST, (. 25, 25 .) .) :))",
	"StartSimulation(0)",
	"OrderSpeedRequest(1,1,-1.0000)",
	"FinishSimulation(0,1)",
	"StrategyVictory(1)"
};
static TAT_SRC_FILE_DEF tat_battleshipstest_Test0008N0_SrcFiles[] = {
	{"C:\\Work\\Testing\\BSTest\\mpr\\Test0008.mpr", 1, 5, 18 },
	{"C:\\Work\\Testing\\BSTest\\work\\Test0008N0.mpr", 2, 5, 18 }};
TAT_TEST_RESULT TAT_TI_battleshipstest_Test0008N0( TAT_TEST_INSTANCE* id )
{
	id->varDefs = NULL;
	id->timeLabels = NULL;
	id->srcFiles = (TAT_SRC_FILE_DEF*)&tat_battleshipstest_Test0008N0_SrcFiles;
	id->timeLabelCount = 0;
	id->srcFilesCount = 2;
	id->varCount = 0;
	id->branchCount = 0;
	id->repeatCount = 1;
	id->timerCount  = 0;
	id->loopCount   = 0;
	return TAT_TEST_CONTINUE;
}
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0008N0_N0[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0008N0_N1[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0008N0_N2[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0008N0_N3[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0008N0_N4[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0008N0_N5[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0008N0_N6[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0008N0_0_SrcRef[] = {{0, 5}, {1, 5}};
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0008N0_0_event = {TAT_SEND_EV, TAT_S_InitDataComplete_ID, NULL, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0008N0_N0, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0008N0_0_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0008N0_MessageTable[0],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0008N0_1_SrcRef[] = {{0, 7}, {1, 7}};
static TAT_SIG_ShipsCurrentState tat_battleshipstest_Test0008N0_1_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0008N0_1_event = {TAT_SEND_EV, TAT_S_ShipsCurrentState_ID, &tat_battleshipstest_Test0008N0_1_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0008N0_N1, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0008N0_1_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0008N0_MessageTable[1],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0008N0_2_SrcRef[] = {{0, 9}, {1, 9}};
static TAT_SIG_VisibleShip tat_battleshipstest_Test0008N0_2_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0008N0_2_event = {TAT_SEND_EV, TAT_S_VisibleShip_ID, &tat_battleshipstest_Test0008N0_2_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0008N0_N2, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0008N0_2_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0008N0_MessageTable[2],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0008N0_3_SrcRef[] = {{0, 11}, {1, 11}};
static TAT_SIG_StartSimulation tat_battleshipstest_Test0008N0_3_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0008N0_3_event = {TAT_SEND_EV, TAT_S_StartSimulation_ID, &tat_battleshipstest_Test0008N0_3_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0008N0_N3, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0008N0_3_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0008N0_MessageTable[3],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0008N0_4_SrcRef[] = {{0, 13}, {1, 13}};
static TAT_SIG_OrderSpeedRequest tat_battleshipstest_Test0008N0_4_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0008N0_4_event = {TAT_RECV_EV, TAT_S_OrderSpeedRequest_ID, &tat_battleshipstest_Test0008N0_4_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0008N0_N4, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0008N0_4_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0008N0_MessageTable[4],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0008N0_5_SrcRef[] = {{0, 15}, {1, 15}};
static TAT_SIG_FinishSimulation tat_battleshipstest_Test0008N0_5_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0008N0_5_event = {TAT_RECV_EV, TAT_S_FinishSimulation_ID, &tat_battleshipstest_Test0008N0_5_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0008N0_N5, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0008N0_5_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0008N0_MessageTable[5],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0008N0_6_SrcRef[] = {{0, 17}, {1, 17}};
static TAT_SIG_StrategyVictory tat_battleshipstest_Test0008N0_6_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0008N0_6_event = {TAT_SEND_EV, TAT_S_StrategyVictory_ID, &tat_battleshipstest_Test0008N0_6_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0008N0_N6, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0008N0_6_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0008N0_MessageTable[6],
NULL};

tStringShipCurrentState TAT_Ass_Test0008N0_1_1 (TAT_TEST_INSTANCE *id) {
	tStringShipCurrentState x;
	x.First=(tStringShipCurrentState_yrec*)malloc(sizeof(tStringShipCurrentState_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringShipCurrentState_Cur=x.First;
	TAT_tStringShipCurrentState_Cur->Suc=NULL;
	TAT_tStringShipCurrentState_Cur->Data.ShipId=1;
	TAT_tStringShipCurrentState_Cur->Data.StrategyId=1;
	TAT_tStringShipCurrentState_Cur->Data.TypeShip=CRUISER;
	TAT_tStringShipCurrentState_Cur->Data.Speed=8;
	TAT_tStringShipCurrentState_Cur->Data.Heading=EAST;
	TAT_tStringShipCurrentState_Cur->Data.Resource=70;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.x=1;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.y=1;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[0]=15;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[1]=15;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[2]=3;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[3]=0;
	return x;
}
tStringEnemyShip TAT_Ass_Test0008N0_2_1 (TAT_TEST_INSTANCE *id) {
	tStringEnemyShip x;
	x.First=(tStringEnemyShip_yrec*)malloc(sizeof(tStringEnemyShip_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringEnemyShip_Cur=x.First;
	TAT_tStringEnemyShip_Cur->Suc=NULL;
	TAT_tStringEnemyShip_Cur->Data.ShipId=2;
	TAT_tStringEnemyShip_Cur->Data.StrategyId=2;
	TAT_tStringEnemyShip_Cur->Data.TypeShip=REPAIR_BOAT;
	TAT_tStringEnemyShip_Cur->Data.Speed=0;
	TAT_tStringEnemyShip_Cur->Data.Heading=NORTH_WEST;
	TAT_tStringEnemyShip_Cur->Data.Coordinates.x=25;
	TAT_tStringEnemyShip_Cur->Data.Coordinates.y=25;
	return x;
}
static TAT_TEST_RESULT tat_Test0008N0_state0(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0008N0_0_event, sizeof(__ev));
			if (TAT_Send_InitDataComplete(id, &__ev, &tat_battleshipstest_Test0008N0_0_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=1;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0008N0_state1(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0008N0_1_event, sizeof(__ev));
			if (TAT_Send_ShipsCurrentState(id, &__ev, &tat_battleshipstest_Test0008N0_1_event, 1, TAT_Ass_Test0008N0_1_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=2;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0008N0_state2(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0008N0_2_event, sizeof(__ev));
			if (TAT_Send_VisibleShip(id, &__ev, &tat_battleshipstest_Test0008N0_2_event, 1, TAT_Ass_Test0008N0_2_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=3;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0008N0_state3(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0008N0_3_event, sizeof(__ev));
			if (TAT_Send_StartSimulation(id, &__ev, &tat_battleshipstest_Test0008N0_3_event, 0)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=4;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0008N0_state4(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_OrderSpeedRequest_ID)&&
				((((TAT_SIG_OrderSpeedRequest *)_ev->eventData)->Param1==(1)))&&
				((((TAT_SIG_OrderSpeedRequest *)_ev->eventData)->Param2==(1)))&&
				((((TAT_SIG_OrderSpeedRequest *)_ev->eventData)->Param3==(-1.0000)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0008N0_4_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=5;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0008N0_state5(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_FinishSimulation_ID)&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param1==(0)))&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param2==(1)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0008N0_5_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=6;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0008N0_state6(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0008N0_6_event, sizeof(__ev));
			if (TAT_Send_StrategyVictory(id, &__ev, &tat_battleshipstest_Test0008N0_6_event, 1)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=-1;
		}
	return TAT_TEST_FINISHED;
}
TAT_TEST_RESULT TAT_T_battleshipstest_Test0008N0( TAT_TEST_INSTANCE* id )
{
	static const TAT_STATE_FUNC states[7]={
	tat_Test0008N0_state0,tat_Test0008N0_state1,tat_Test0008N0_state2,tat_Test0008N0_state3,tat_Test0008N0_state4,
	tat_Test0008N0_state5,tat_Test0008N0_state6};
	TAT_TEST_RESULT res;
	int prevState;
	if( id == NULL )
	{
		id->testStatus = TAT_ERR_INTERNAL;
		id->testResult = TAT_TEST_CRITICAL;
		id->dumpEvent(id, NULL, NULL);
		return TAT_TEST_CRITICAL;
	}
	while( id->nextState != TAT_FINAL_STATE )
	{
		prevState=id->nextState;
		res = states[id->nextState](id);
		if (res == TAT_TEST_CRITICAL) return TAT_TEST_CRITICAL;
		if (res == TAT_TEST_FINISHED && id->testStatus != TAT_OK)
			return TAT_TEST_FINISHED;
		if (res == TAT_TEST_FINISHED && id->nextState != TAT_FINAL_STATE && id->nextState==prevState)
			return TAT_TEST_CONTINUE;
	}
	return TAT_TEST_FINISHED;
}


/****************************************************************************/
static char * tat_battleshipstest_Test0014N0_MessageTable[] = {
	"InitDataComplete()",
	"ShipsCurrentState(1,(: (. 1, 1, CRUISER, 0, SOUTH, 70, (. 10, 10 .), (: 0, 1, 0, 0 :) .) :))",
	"VisibleShip(1,(: (. 2, 2, REPAIR_BOAT, 0, NORTH_WEST, (. 25, 25 .) .) :))",
	"StartSimulation(0)",
	"OrderTurnRequest(1,1,LEFT)",
	"FinishSimulation(0,1)",
	"StrategyVictory(1)"
};
static TAT_SRC_FILE_DEF tat_battleshipstest_Test0014N0_SrcFiles[] = {
	{"C:\\Work\\Testing\\BSTest\\mpr\\Test0014.mpr", 1, 5, 18 },
	{"C:\\Work\\Testing\\BSTest\\work\\Test0014N0.mpr", 2, 5, 18 }};
TAT_TEST_RESULT TAT_TI_battleshipstest_Test0014N0( TAT_TEST_INSTANCE* id )
{
	id->varDefs = NULL;
	id->timeLabels = NULL;
	id->srcFiles = (TAT_SRC_FILE_DEF*)&tat_battleshipstest_Test0014N0_SrcFiles;
	id->timeLabelCount = 0;
	id->srcFilesCount = 2;
	id->varCount = 0;
	id->branchCount = 0;
	id->repeatCount = 1;
	id->timerCount  = 0;
	id->loopCount   = 0;
	return TAT_TEST_CONTINUE;
}
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0014N0_N0[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0014N0_N1[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0014N0_N2[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0014N0_N3[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0014N0_N4[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0014N0_N5[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0014N0_N6[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0014N0_0_SrcRef[] = {{0, 5}, {1, 5}};
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0014N0_0_event = {TAT_SEND_EV, TAT_S_InitDataComplete_ID, NULL, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0014N0_N0, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0014N0_0_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0014N0_MessageTable[0],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0014N0_1_SrcRef[] = {{0, 7}, {1, 7}};
static TAT_SIG_ShipsCurrentState tat_battleshipstest_Test0014N0_1_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0014N0_1_event = {TAT_SEND_EV, TAT_S_ShipsCurrentState_ID, &tat_battleshipstest_Test0014N0_1_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0014N0_N1, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0014N0_1_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0014N0_MessageTable[1],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0014N0_2_SrcRef[] = {{0, 9}, {1, 9}};
static TAT_SIG_VisibleShip tat_battleshipstest_Test0014N0_2_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0014N0_2_event = {TAT_SEND_EV, TAT_S_VisibleShip_ID, &tat_battleshipstest_Test0014N0_2_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0014N0_N2, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0014N0_2_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0014N0_MessageTable[2],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0014N0_3_SrcRef[] = {{0, 11}, {1, 11}};
static TAT_SIG_StartSimulation tat_battleshipstest_Test0014N0_3_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0014N0_3_event = {TAT_SEND_EV, TAT_S_StartSimulation_ID, &tat_battleshipstest_Test0014N0_3_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0014N0_N3, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0014N0_3_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0014N0_MessageTable[3],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0014N0_4_SrcRef[] = {{0, 13}, {1, 13}};
static TAT_SIG_OrderTurnRequest tat_battleshipstest_Test0014N0_4_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0014N0_4_event = {TAT_RECV_EV, TAT_S_OrderTurnRequest_ID, &tat_battleshipstest_Test0014N0_4_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0014N0_N4, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0014N0_4_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0014N0_MessageTable[4],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0014N0_5_SrcRef[] = {{0, 15}, {1, 15}};
static TAT_SIG_FinishSimulation tat_battleshipstest_Test0014N0_5_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0014N0_5_event = {TAT_RECV_EV, TAT_S_FinishSimulation_ID, &tat_battleshipstest_Test0014N0_5_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0014N0_N5, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0014N0_5_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0014N0_MessageTable[5],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0014N0_6_SrcRef[] = {{0, 17}, {1, 17}};
static TAT_SIG_StrategyVictory tat_battleshipstest_Test0014N0_6_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0014N0_6_event = {TAT_SEND_EV, TAT_S_StrategyVictory_ID, &tat_battleshipstest_Test0014N0_6_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0014N0_N6, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0014N0_6_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0014N0_MessageTable[6],
NULL};

tStringShipCurrentState TAT_Ass_Test0014N0_1_1 (TAT_TEST_INSTANCE *id) {
	tStringShipCurrentState x;
	x.First=(tStringShipCurrentState_yrec*)malloc(sizeof(tStringShipCurrentState_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringShipCurrentState_Cur=x.First;
	TAT_tStringShipCurrentState_Cur->Suc=NULL;
	TAT_tStringShipCurrentState_Cur->Data.ShipId=1;
	TAT_tStringShipCurrentState_Cur->Data.StrategyId=1;
	TAT_tStringShipCurrentState_Cur->Data.TypeShip=CRUISER;
	TAT_tStringShipCurrentState_Cur->Data.Speed=0;
	TAT_tStringShipCurrentState_Cur->Data.Heading=SOUTH;
	TAT_tStringShipCurrentState_Cur->Data.Resource=70;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.x=10;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.y=10;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[0]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[1]=1;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[2]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[3]=0;
	return x;
}
tStringEnemyShip TAT_Ass_Test0014N0_2_1 (TAT_TEST_INSTANCE *id) {
	tStringEnemyShip x;
	x.First=(tStringEnemyShip_yrec*)malloc(sizeof(tStringEnemyShip_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringEnemyShip_Cur=x.First;
	TAT_tStringEnemyShip_Cur->Suc=NULL;
	TAT_tStringEnemyShip_Cur->Data.ShipId=2;
	TAT_tStringEnemyShip_Cur->Data.StrategyId=2;
	TAT_tStringEnemyShip_Cur->Data.TypeShip=REPAIR_BOAT;
	TAT_tStringEnemyShip_Cur->Data.Speed=0;
	TAT_tStringEnemyShip_Cur->Data.Heading=NORTH_WEST;
	TAT_tStringEnemyShip_Cur->Data.Coordinates.x=25;
	TAT_tStringEnemyShip_Cur->Data.Coordinates.y=25;
	return x;
}
static TAT_TEST_RESULT tat_Test0014N0_state0(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0014N0_0_event, sizeof(__ev));
			if (TAT_Send_InitDataComplete(id, &__ev, &tat_battleshipstest_Test0014N0_0_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=1;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0014N0_state1(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0014N0_1_event, sizeof(__ev));
			if (TAT_Send_ShipsCurrentState(id, &__ev, &tat_battleshipstest_Test0014N0_1_event, 1, TAT_Ass_Test0014N0_1_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=2;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0014N0_state2(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0014N0_2_event, sizeof(__ev));
			if (TAT_Send_VisibleShip(id, &__ev, &tat_battleshipstest_Test0014N0_2_event, 1, TAT_Ass_Test0014N0_2_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=3;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0014N0_state3(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0014N0_3_event, sizeof(__ev));
			if (TAT_Send_StartSimulation(id, &__ev, &tat_battleshipstest_Test0014N0_3_event, 0)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=4;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0014N0_state4(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_OrderTurnRequest_ID)&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param1==(1)))&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param2==(1)))&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param3==(LEFT)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0014N0_4_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=5;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0014N0_state5(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_FinishSimulation_ID)&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param1==(0)))&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param2==(1)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0014N0_5_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=6;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0014N0_state6(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0014N0_6_event, sizeof(__ev));
			if (TAT_Send_StrategyVictory(id, &__ev, &tat_battleshipstest_Test0014N0_6_event, 1)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=-1;
		}
	return TAT_TEST_FINISHED;
}
TAT_TEST_RESULT TAT_T_battleshipstest_Test0014N0( TAT_TEST_INSTANCE* id )
{
	static const TAT_STATE_FUNC states[7]={
	tat_Test0014N0_state0,tat_Test0014N0_state1,tat_Test0014N0_state2,tat_Test0014N0_state3,tat_Test0014N0_state4,
	tat_Test0014N0_state5,tat_Test0014N0_state6};
	TAT_TEST_RESULT res;
	int prevState;
	if( id == NULL )
	{
		id->testStatus = TAT_ERR_INTERNAL;
		id->testResult = TAT_TEST_CRITICAL;
		id->dumpEvent(id, NULL, NULL);
		return TAT_TEST_CRITICAL;
	}
	while( id->nextState != TAT_FINAL_STATE )
	{
		prevState=id->nextState;
		res = states[id->nextState](id);
		if (res == TAT_TEST_CRITICAL) return TAT_TEST_CRITICAL;
		if (res == TAT_TEST_FINISHED && id->testStatus != TAT_OK)
			return TAT_TEST_FINISHED;
		if (res == TAT_TEST_FINISHED && id->nextState != TAT_FINAL_STATE && id->nextState==prevState)
			return TAT_TEST_CONTINUE;
	}
	return TAT_TEST_FINISHED;
}


/****************************************************************************/
static char * tat_battleshipstest_Test0022N0_MessageTable[] = {
	"InitDataComplete()",
	"ShipsCurrentState(1,(: (. 1, 1, CRUISER, 0, SOUTH, 20, (. 10, 10 .), (: 0, 0, 0, 0 :) .), (. 2, 1, REPAIR_BOAT, 0, SOUTH, 25, (. 12, 10 .), (: 0, 0, 0, 10 :) .), (. 3, 1, CRUISER, 0, SOUTH, 5, (. 15, 10 .), (: 0, 0, 0, 0 :) .) :))",
	"VisibleShip(1,(: (. 1, 2, REPAIR_BOAT, 0, NORTH_WEST, (. 25, 25 .) .) :))",
	"StartSimulation(0)",
	"OrderTurnRequest(1,1,LEFT)",
	"OrderFireRequest(2,1,REPAIR,(: (. 15, 10 .), (. 10, 10 .) :))",
	"OrderTurnRequest(3,1,LEFT)",
	"FinishSimulation(0,1)",
	"StrategyVictory(1)"
};
static TAT_SRC_FILE_DEF tat_battleshipstest_Test0022N0_SrcFiles[] = {
	{"C:\\Work\\Testing\\BSTest\\mpr\\Test0022.mpr", 1, 5, 22 },
	{"C:\\Work\\Testing\\BSTest\\work\\Test0022N0.mpr", 2, 5, 22 }};
TAT_TEST_RESULT TAT_TI_battleshipstest_Test0022N0( TAT_TEST_INSTANCE* id )
{
	id->varDefs = NULL;
	id->timeLabels = NULL;
	id->srcFiles = (TAT_SRC_FILE_DEF*)&tat_battleshipstest_Test0022N0_SrcFiles;
	id->timeLabelCount = 0;
	id->srcFilesCount = 2;
	id->varCount = 0;
	id->branchCount = 0;
	id->repeatCount = 1;
	id->timerCount  = 0;
	id->loopCount   = 0;
	return TAT_TEST_CONTINUE;
}
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N0_N0[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N0_N1[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N0_N2[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N0_N3[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N0_N4[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N0_N5[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N0_N6[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N0_N7[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N0_N8[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N0_0_SrcRef[] = {{0, 5}, {1, 5}};
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N0_0_event = {TAT_SEND_EV, TAT_S_InitDataComplete_ID, NULL, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N0_N0, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N0_0_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N0_MessageTable[0],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N0_1_SrcRef[] = {{0, 7}, {1, 7}};
static TAT_SIG_ShipsCurrentState tat_battleshipstest_Test0022N0_1_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N0_1_event = {TAT_SEND_EV, TAT_S_ShipsCurrentState_ID, &tat_battleshipstest_Test0022N0_1_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N0_N1, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N0_1_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N0_MessageTable[1],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N0_2_SrcRef[] = {{0, 9}, {1, 9}};
static TAT_SIG_VisibleShip tat_battleshipstest_Test0022N0_2_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N0_2_event = {TAT_SEND_EV, TAT_S_VisibleShip_ID, &tat_battleshipstest_Test0022N0_2_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N0_N2, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N0_2_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N0_MessageTable[2],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N0_3_SrcRef[] = {{0, 11}, {1, 11}};
static TAT_SIG_StartSimulation tat_battleshipstest_Test0022N0_3_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N0_3_event = {TAT_SEND_EV, TAT_S_StartSimulation_ID, &tat_battleshipstest_Test0022N0_3_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N0_N3, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N0_3_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N0_MessageTable[3],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N0_4_SrcRef[] = {{0, 13}, {1, 13}};
static TAT_SIG_OrderTurnRequest tat_battleshipstest_Test0022N0_4_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N0_4_event = {TAT_RECV_EV, TAT_S_OrderTurnRequest_ID, &tat_battleshipstest_Test0022N0_4_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N0_N4, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N0_4_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N0_MessageTable[4],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N0_5_SrcRef[] = {{0, 15}, {1, 15}};
static TAT_SIG_OrderFireRequest tat_battleshipstest_Test0022N0_5_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N0_5_event = {TAT_RECV_EV, TAT_S_OrderFireRequest_ID, &tat_battleshipstest_Test0022N0_5_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N0_N5, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N0_5_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N0_MessageTable[5],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N0_6_SrcRef[] = {{0, 17}, {1, 17}};
static TAT_SIG_OrderTurnRequest tat_battleshipstest_Test0022N0_6_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N0_6_event = {TAT_RECV_EV, TAT_S_OrderTurnRequest_ID, &tat_battleshipstest_Test0022N0_6_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N0_N6, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N0_6_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N0_MessageTable[6],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N0_7_SrcRef[] = {{0, 19}, {1, 19}};
static TAT_SIG_FinishSimulation tat_battleshipstest_Test0022N0_7_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N0_7_event = {TAT_RECV_EV, TAT_S_FinishSimulation_ID, &tat_battleshipstest_Test0022N0_7_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N0_N7, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N0_7_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N0_MessageTable[7],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N0_8_SrcRef[] = {{0, 21}, {1, 21}};
static TAT_SIG_StrategyVictory tat_battleshipstest_Test0022N0_8_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N0_8_event = {TAT_SEND_EV, TAT_S_StrategyVictory_ID, &tat_battleshipstest_Test0022N0_8_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N0_N8, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N0_8_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N0_MessageTable[8],
NULL};

tStringShipCurrentState TAT_Ass_Test0022N0_1_1 (TAT_TEST_INSTANCE *id) {
	tStringShipCurrentState x;
	x.First=(tStringShipCurrentState_yrec*)malloc(sizeof(tStringShipCurrentState_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringShipCurrentState_Cur=x.First;
	TAT_tStringShipCurrentState_Cur->Suc=NULL;
	TAT_tStringShipCurrentState_Cur->Data.ShipId=1;
	TAT_tStringShipCurrentState_Cur->Data.StrategyId=1;
	TAT_tStringShipCurrentState_Cur->Data.TypeShip=CRUISER;
	TAT_tStringShipCurrentState_Cur->Data.Speed=0;
	TAT_tStringShipCurrentState_Cur->Data.Heading=SOUTH;
	TAT_tStringShipCurrentState_Cur->Data.Resource=20;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.x=10;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.y=10;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[0]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[1]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[2]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[3]=0;
	TAT_tStringShipCurrentState_Cur->Suc=(tStringShipCurrentState_yrec*)malloc(sizeof(tStringShipCurrentState_yrec));
	TAT_tStringShipCurrentState_Cur=TAT_tStringShipCurrentState_Cur->Suc;
	TAT_tStringShipCurrentState_Cur->Suc=0;
	x.Last=TAT_tStringShipCurrentState_Cur;
	x.Length++;
	TAT_tStringShipCurrentState_Cur->Data.ShipId=2;
	TAT_tStringShipCurrentState_Cur->Data.StrategyId=1;
	TAT_tStringShipCurrentState_Cur->Data.TypeShip=REPAIR_BOAT;
	TAT_tStringShipCurrentState_Cur->Data.Speed=0;
	TAT_tStringShipCurrentState_Cur->Data.Heading=SOUTH;
	TAT_tStringShipCurrentState_Cur->Data.Resource=25;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.x=12;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.y=10;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[0]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[1]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[2]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[3]=10;
	TAT_tStringShipCurrentState_Cur->Suc=(tStringShipCurrentState_yrec*)malloc(sizeof(tStringShipCurrentState_yrec));
	TAT_tStringShipCurrentState_Cur=TAT_tStringShipCurrentState_Cur->Suc;
	TAT_tStringShipCurrentState_Cur->Suc=0;
	x.Last=TAT_tStringShipCurrentState_Cur;
	x.Length++;
	TAT_tStringShipCurrentState_Cur->Data.ShipId=3;
	TAT_tStringShipCurrentState_Cur->Data.StrategyId=1;
	TAT_tStringShipCurrentState_Cur->Data.TypeShip=CRUISER;
	TAT_tStringShipCurrentState_Cur->Data.Speed=0;
	TAT_tStringShipCurrentState_Cur->Data.Heading=SOUTH;
	TAT_tStringShipCurrentState_Cur->Data.Resource=5;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.x=15;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.y=10;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[0]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[1]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[2]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[3]=0;
	return x;
}
tStringEnemyShip TAT_Ass_Test0022N0_2_1 (TAT_TEST_INSTANCE *id) {
	tStringEnemyShip x;
	x.First=(tStringEnemyShip_yrec*)malloc(sizeof(tStringEnemyShip_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringEnemyShip_Cur=x.First;
	TAT_tStringEnemyShip_Cur->Suc=NULL;
	TAT_tStringEnemyShip_Cur->Data.ShipId=1;
	TAT_tStringEnemyShip_Cur->Data.StrategyId=2;
	TAT_tStringEnemyShip_Cur->Data.TypeShip=REPAIR_BOAT;
	TAT_tStringEnemyShip_Cur->Data.Speed=0;
	TAT_tStringEnemyShip_Cur->Data.Heading=NORTH_WEST;
	TAT_tStringEnemyShip_Cur->Data.Coordinates.x=25;
	TAT_tStringEnemyShip_Cur->Data.Coordinates.y=25;
	return x;
}
int TAT_Cmp_Test0022N0_5_3_1 (tStringShotsList x, TAT_TEST_INSTANCE *id) {
	TAT_tStringShotsList_Cur=x.First;
	if(TAT_tStringShotsList_Cur==NULL) return 1;
	if(TAT_tStringShotsList_Cur->Data.x!=15) return 1;
	if(TAT_tStringShotsList_Cur->Data.y!=10) return 1;
	TAT_tStringShotsList_Cur=TAT_tStringShotsList_Cur->Suc;
	if(TAT_tStringShotsList_Cur==NULL) return 1;
	if(TAT_tStringShotsList_Cur->Data.x!=10) return 1;
	if(TAT_tStringShotsList_Cur->Data.y!=10) return 1;
	TAT_tStringShotsList_Cur=TAT_tStringShotsList_Cur->Suc;
	if(TAT_tStringShotsList_Cur!=NULL) return 1;
	return 0;
}
static TAT_TEST_RESULT tat_Test0022N0_state0(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0022N0_0_event, sizeof(__ev));
			if (TAT_Send_InitDataComplete(id, &__ev, &tat_battleshipstest_Test0022N0_0_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=1;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0022N0_state1(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0022N0_1_event, sizeof(__ev));
			if (TAT_Send_ShipsCurrentState(id, &__ev, &tat_battleshipstest_Test0022N0_1_event, 1, TAT_Ass_Test0022N0_1_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=2;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0022N0_state2(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0022N0_2_event, sizeof(__ev));
			if (TAT_Send_VisibleShip(id, &__ev, &tat_battleshipstest_Test0022N0_2_event, 1, TAT_Ass_Test0022N0_2_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=3;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0022N0_state3(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0022N0_3_event, sizeof(__ev));
			if (TAT_Send_StartSimulation(id, &__ev, &tat_battleshipstest_Test0022N0_3_event, 0)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=4;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0022N0_state4(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_OrderTurnRequest_ID)&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param1==(1)))&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param2==(1)))&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param3==(LEFT)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0022N0_4_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=5;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0022N0_state5(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_OrderFireRequest_ID)&&
				((((TAT_SIG_OrderFireRequest *)_ev->eventData)->Param1==(2)))&&
				((((TAT_SIG_OrderFireRequest *)_ev->eventData)->Param2==(1)))&&
				((((TAT_SIG_OrderFireRequest *)_ev->eventData)->Param3==(REPAIR)))&&
				((TAT_Cmp_Test0022N0_5_3_1(((TAT_SIG_OrderFireRequest *)_ev->eventData)->Param4,id) == 0))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0022N0_5_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				TAT_Free_tStringShotsList(&(((TAT_SIG_OrderFireRequest *)_ev->eventData)->Param4));
				id->popQueue(id);
				id->nextState=6;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0022N0_state6(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_OrderTurnRequest_ID)&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param1==(3)))&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param2==(1)))&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param3==(LEFT)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0022N0_6_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=7;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0022N0_state7(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_FinishSimulation_ID)&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param1==(0)))&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param2==(1)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0022N0_7_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=8;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0022N0_state8(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0022N0_8_event, sizeof(__ev));
			if (TAT_Send_StrategyVictory(id, &__ev, &tat_battleshipstest_Test0022N0_8_event, 1)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=-1;
		}
	return TAT_TEST_FINISHED;
}
TAT_TEST_RESULT TAT_T_battleshipstest_Test0022N0( TAT_TEST_INSTANCE* id )
{
	static const TAT_STATE_FUNC states[9]={
	tat_Test0022N0_state0,tat_Test0022N0_state1,tat_Test0022N0_state2,tat_Test0022N0_state3,tat_Test0022N0_state4,
	tat_Test0022N0_state5,tat_Test0022N0_state6,tat_Test0022N0_state7,tat_Test0022N0_state8};
	TAT_TEST_RESULT res;
	int prevState;
	if( id == NULL )
	{
		id->testStatus = TAT_ERR_INTERNAL;
		id->testResult = TAT_TEST_CRITICAL;
		id->dumpEvent(id, NULL, NULL);
		return TAT_TEST_CRITICAL;
	}
	while( id->nextState != TAT_FINAL_STATE )
	{
		prevState=id->nextState;
		res = states[id->nextState](id);
		if (res == TAT_TEST_CRITICAL) return TAT_TEST_CRITICAL;
		if (res == TAT_TEST_FINISHED && id->testStatus != TAT_OK)
			return TAT_TEST_FINISHED;
		if (res == TAT_TEST_FINISHED && id->nextState != TAT_FINAL_STATE && id->nextState==prevState)
			return TAT_TEST_CONTINUE;
	}
	return TAT_TEST_FINISHED;
}


/****************************************************************************/
static char * tat_battleshipstest_Test0032N0_MessageTable[] = {
	"InitDataComplete()",
	"ShipsCurrentState(1,(: (. 1, 1, CRUISER, 8, EAST, 70, (. 40, 25 .), (: 15, 15, 3, 0 :) .) :))",
	"VisibleShip(1,(: (. 1, 2, REPAIR_BOAT, 0, NORTH_WEST, (. 15, 15 .) .) :))",
	"StartSimulation(0)",
	"OrderSpeedRequest(1,1,-1.0000)",
	"FinishSimulation(0,1)",
	"StrategyVictory(1)"
};
static TAT_SRC_FILE_DEF tat_battleshipstest_Test0032N0_SrcFiles[] = {
	{"C:\\Work\\Testing\\BSTest\\mpr\\Test0032.mpr", 1, 5, 18 },
	{"C:\\Work\\Testing\\BSTest\\work\\Test0032N0.mpr", 2, 5, 18 }};
TAT_TEST_RESULT TAT_TI_battleshipstest_Test0032N0( TAT_TEST_INSTANCE* id )
{
	id->varDefs = NULL;
	id->timeLabels = NULL;
	id->srcFiles = (TAT_SRC_FILE_DEF*)&tat_battleshipstest_Test0032N0_SrcFiles;
	id->timeLabelCount = 0;
	id->srcFilesCount = 2;
	id->varCount = 0;
	id->branchCount = 0;
	id->repeatCount = 1;
	id->timerCount  = 0;
	id->loopCount   = 0;
	return TAT_TEST_CONTINUE;
}
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0032N0_N0[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0032N0_N1[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0032N0_N2[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0032N0_N3[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0032N0_N4[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0032N0_N5[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0032N0_N6[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0032N0_0_SrcRef[] = {{0, 5}, {1, 5}};
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0032N0_0_event = {TAT_SEND_EV, TAT_S_InitDataComplete_ID, NULL, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0032N0_N0, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0032N0_0_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0032N0_MessageTable[0],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0032N0_1_SrcRef[] = {{0, 7}, {1, 7}};
static TAT_SIG_ShipsCurrentState tat_battleshipstest_Test0032N0_1_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0032N0_1_event = {TAT_SEND_EV, TAT_S_ShipsCurrentState_ID, &tat_battleshipstest_Test0032N0_1_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0032N0_N1, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0032N0_1_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0032N0_MessageTable[1],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0032N0_2_SrcRef[] = {{0, 9}, {1, 9}};
static TAT_SIG_VisibleShip tat_battleshipstest_Test0032N0_2_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0032N0_2_event = {TAT_SEND_EV, TAT_S_VisibleShip_ID, &tat_battleshipstest_Test0032N0_2_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0032N0_N2, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0032N0_2_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0032N0_MessageTable[2],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0032N0_3_SrcRef[] = {{0, 11}, {1, 11}};
static TAT_SIG_StartSimulation tat_battleshipstest_Test0032N0_3_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0032N0_3_event = {TAT_SEND_EV, TAT_S_StartSimulation_ID, &tat_battleshipstest_Test0032N0_3_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0032N0_N3, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0032N0_3_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0032N0_MessageTable[3],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0032N0_4_SrcRef[] = {{0, 13}, {1, 13}};
static TAT_SIG_OrderSpeedRequest tat_battleshipstest_Test0032N0_4_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0032N0_4_event = {TAT_RECV_EV, TAT_S_OrderSpeedRequest_ID, &tat_battleshipstest_Test0032N0_4_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0032N0_N4, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0032N0_4_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0032N0_MessageTable[4],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0032N0_5_SrcRef[] = {{0, 15}, {1, 15}};
static TAT_SIG_FinishSimulation tat_battleshipstest_Test0032N0_5_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0032N0_5_event = {TAT_RECV_EV, TAT_S_FinishSimulation_ID, &tat_battleshipstest_Test0032N0_5_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0032N0_N5, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0032N0_5_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0032N0_MessageTable[5],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0032N0_6_SrcRef[] = {{0, 17}, {1, 17}};
static TAT_SIG_StrategyVictory tat_battleshipstest_Test0032N0_6_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0032N0_6_event = {TAT_SEND_EV, TAT_S_StrategyVictory_ID, &tat_battleshipstest_Test0032N0_6_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0032N0_N6, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0032N0_6_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0032N0_MessageTable[6],
NULL};

tStringShipCurrentState TAT_Ass_Test0032N0_1_1 (TAT_TEST_INSTANCE *id) {
	tStringShipCurrentState x;
	x.First=(tStringShipCurrentState_yrec*)malloc(sizeof(tStringShipCurrentState_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringShipCurrentState_Cur=x.First;
	TAT_tStringShipCurrentState_Cur->Suc=NULL;
	TAT_tStringShipCurrentState_Cur->Data.ShipId=1;
	TAT_tStringShipCurrentState_Cur->Data.StrategyId=1;
	TAT_tStringShipCurrentState_Cur->Data.TypeShip=CRUISER;
	TAT_tStringShipCurrentState_Cur->Data.Speed=8;
	TAT_tStringShipCurrentState_Cur->Data.Heading=EAST;
	TAT_tStringShipCurrentState_Cur->Data.Resource=70;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.x=40;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.y=25;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[0]=15;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[1]=15;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[2]=3;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[3]=0;
	return x;
}
tStringEnemyShip TAT_Ass_Test0032N0_2_1 (TAT_TEST_INSTANCE *id) {
	tStringEnemyShip x;
	x.First=(tStringEnemyShip_yrec*)malloc(sizeof(tStringEnemyShip_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringEnemyShip_Cur=x.First;
	TAT_tStringEnemyShip_Cur->Suc=NULL;
	TAT_tStringEnemyShip_Cur->Data.ShipId=1;
	TAT_tStringEnemyShip_Cur->Data.StrategyId=2;
	TAT_tStringEnemyShip_Cur->Data.TypeShip=REPAIR_BOAT;
	TAT_tStringEnemyShip_Cur->Data.Speed=0;
	TAT_tStringEnemyShip_Cur->Data.Heading=NORTH_WEST;
	TAT_tStringEnemyShip_Cur->Data.Coordinates.x=15;
	TAT_tStringEnemyShip_Cur->Data.Coordinates.y=15;
	return x;
}
static TAT_TEST_RESULT tat_Test0032N0_state0(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0032N0_0_event, sizeof(__ev));
			if (TAT_Send_InitDataComplete(id, &__ev, &tat_battleshipstest_Test0032N0_0_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=1;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0032N0_state1(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0032N0_1_event, sizeof(__ev));
			if (TAT_Send_ShipsCurrentState(id, &__ev, &tat_battleshipstest_Test0032N0_1_event, 1, TAT_Ass_Test0032N0_1_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=2;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0032N0_state2(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0032N0_2_event, sizeof(__ev));
			if (TAT_Send_VisibleShip(id, &__ev, &tat_battleshipstest_Test0032N0_2_event, 1, TAT_Ass_Test0032N0_2_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=3;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0032N0_state3(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0032N0_3_event, sizeof(__ev));
			if (TAT_Send_StartSimulation(id, &__ev, &tat_battleshipstest_Test0032N0_3_event, 0)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=4;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0032N0_state4(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_OrderSpeedRequest_ID)&&
				((((TAT_SIG_OrderSpeedRequest *)_ev->eventData)->Param1==(1)))&&
				((((TAT_SIG_OrderSpeedRequest *)_ev->eventData)->Param2==(1)))&&
				((((TAT_SIG_OrderSpeedRequest *)_ev->eventData)->Param3==(-1.0000)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0032N0_4_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=5;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0032N0_state5(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_FinishSimulation_ID)&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param1==(0)))&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param2==(1)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0032N0_5_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=6;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0032N0_state6(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0032N0_6_event, sizeof(__ev));
			if (TAT_Send_StrategyVictory(id, &__ev, &tat_battleshipstest_Test0032N0_6_event, 1)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=-1;
		}
	return TAT_TEST_FINISHED;
}
TAT_TEST_RESULT TAT_T_battleshipstest_Test0032N0( TAT_TEST_INSTANCE* id )
{
	static const TAT_STATE_FUNC states[7]={
	tat_Test0032N0_state0,tat_Test0032N0_state1,tat_Test0032N0_state2,tat_Test0032N0_state3,tat_Test0032N0_state4,
	tat_Test0032N0_state5,tat_Test0032N0_state6};
	TAT_TEST_RESULT res;
	int prevState;
	if( id == NULL )
	{
		id->testStatus = TAT_ERR_INTERNAL;
		id->testResult = TAT_TEST_CRITICAL;
		id->dumpEvent(id, NULL, NULL);
		return TAT_TEST_CRITICAL;
	}
	while( id->nextState != TAT_FINAL_STATE )
	{
		prevState=id->nextState;
		res = states[id->nextState](id);
		if (res == TAT_TEST_CRITICAL) return TAT_TEST_CRITICAL;
		if (res == TAT_TEST_FINISHED && id->testStatus != TAT_OK)
			return TAT_TEST_FINISHED;
		if (res == TAT_TEST_FINISHED && id->nextState != TAT_FINAL_STATE && id->nextState==prevState)
			return TAT_TEST_CONTINUE;
	}
	return TAT_TEST_FINISHED;
}


/****************************************************************************/
static char * tat_battleshipstest_Test0001N1_MessageTable[] = {
	"InitDataComplete()",
	"ShipsCurrentState(1,(: (. 1, 1, DESTROYER, 10, SOUTH_EAST, 70, (. 13, 15 .), (: 15, 15, 3, 0 :) .) :))",
	"StartSimulation(0)",
	"OrderTurnRequest(1,1,LEFT)",
	"FinishSimulation(0,1)"
};
static TAT_SRC_FILE_DEF tat_battleshipstest_Test0001N1_SrcFiles[] = {
	{"C:\\Work\\Testing\\BSTest\\mpr\\Test0001.mpr", 1, 5, 14 },
	{"C:\\Work\\Testing\\BSTest\\work\\Test0001N1.mpr", 2, 5, 14 }};
TAT_TEST_RESULT TAT_TI_battleshipstest_Test0001N1( TAT_TEST_INSTANCE* id )
{
	id->varDefs = NULL;
	id->timeLabels = NULL;
	id->srcFiles = (TAT_SRC_FILE_DEF*)&tat_battleshipstest_Test0001N1_SrcFiles;
	id->timeLabelCount = 0;
	id->srcFilesCount = 2;
	id->varCount = 0;
	id->branchCount = 0;
	id->repeatCount = 1;
	id->timerCount  = 0;
	id->loopCount   = 0;
	return TAT_TEST_CONTINUE;
}
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0001N1_N0[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0001N1_N1[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0001N1_N2[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0001N1_N3[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0001N1_N4[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0001N1_0_SrcRef[] = {{0, 5}, {1, 5}};
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0001N1_0_event = {TAT_SEND_EV, TAT_S_InitDataComplete_ID, NULL, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0001N1_N0, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0001N1_0_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0001N1_MessageTable[0],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0001N1_1_SrcRef[] = {{0, 7}, {1, 7}};
static TAT_SIG_ShipsCurrentState tat_battleshipstest_Test0001N1_1_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0001N1_1_event = {TAT_SEND_EV, TAT_S_ShipsCurrentState_ID, &tat_battleshipstest_Test0001N1_1_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0001N1_N1, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0001N1_1_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0001N1_MessageTable[1],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0001N1_2_SrcRef[] = {{0, 9}, {1, 9}};
static TAT_SIG_StartSimulation tat_battleshipstest_Test0001N1_2_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0001N1_2_event = {TAT_SEND_EV, TAT_S_StartSimulation_ID, &tat_battleshipstest_Test0001N1_2_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0001N1_N2, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0001N1_2_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0001N1_MessageTable[2],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0001N1_3_SrcRef[] = {{0, 11}, {1, 11}};
static TAT_SIG_OrderTurnRequest tat_battleshipstest_Test0001N1_3_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0001N1_3_event = {TAT_RECV_EV, TAT_S_OrderTurnRequest_ID, &tat_battleshipstest_Test0001N1_3_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0001N1_N3, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0001N1_3_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0001N1_MessageTable[3],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0001N1_4_SrcRef[] = {{0, 13}, {1, 13}};
static TAT_SIG_FinishSimulation tat_battleshipstest_Test0001N1_4_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0001N1_4_event = {TAT_RECV_EV, TAT_S_FinishSimulation_ID, &tat_battleshipstest_Test0001N1_4_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0001N1_N4, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0001N1_4_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0001N1_MessageTable[4],
NULL};

tStringShipCurrentState TAT_Ass_Test0001N1_1_1 (TAT_TEST_INSTANCE *id) {
	tStringShipCurrentState x;
	x.First=(tStringShipCurrentState_yrec*)malloc(sizeof(tStringShipCurrentState_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringShipCurrentState_Cur=x.First;
	TAT_tStringShipCurrentState_Cur->Suc=NULL;
	TAT_tStringShipCurrentState_Cur->Data.ShipId=1;
	TAT_tStringShipCurrentState_Cur->Data.StrategyId=1;
	TAT_tStringShipCurrentState_Cur->Data.TypeShip=DESTROYER;
	TAT_tStringShipCurrentState_Cur->Data.Speed=10;
	TAT_tStringShipCurrentState_Cur->Data.Heading=SOUTH_EAST;
	TAT_tStringShipCurrentState_Cur->Data.Resource=70;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.x=13;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.y=15;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[0]=15;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[1]=15;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[2]=3;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[3]=0;
	return x;
}
static TAT_TEST_RESULT tat_Test0001N1_state0(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0001N1_0_event, sizeof(__ev));
			if (TAT_Send_InitDataComplete(id, &__ev, &tat_battleshipstest_Test0001N1_0_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=1;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0001N1_state1(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0001N1_1_event, sizeof(__ev));
			if (TAT_Send_ShipsCurrentState(id, &__ev, &tat_battleshipstest_Test0001N1_1_event, 1, TAT_Ass_Test0001N1_1_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=2;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0001N1_state2(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0001N1_2_event, sizeof(__ev));
			if (TAT_Send_StartSimulation(id, &__ev, &tat_battleshipstest_Test0001N1_2_event, 0)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=3;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0001N1_state3(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_OrderTurnRequest_ID)&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param1==(1)))&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param2==(1)))&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param3==(LEFT)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0001N1_3_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=4;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0001N1_state4(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_FinishSimulation_ID)&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param1==(0)))&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param2==(1)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0001N1_4_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=-1;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
TAT_TEST_RESULT TAT_T_battleshipstest_Test0001N1( TAT_TEST_INSTANCE* id )
{
	static const TAT_STATE_FUNC states[5]={
	tat_Test0001N1_state0,tat_Test0001N1_state1,tat_Test0001N1_state2,tat_Test0001N1_state3,tat_Test0001N1_state4};
	TAT_TEST_RESULT res;
	int prevState;
	if( id == NULL )
	{
		id->testStatus = TAT_ERR_INTERNAL;
		id->testResult = TAT_TEST_CRITICAL;
		id->dumpEvent(id, NULL, NULL);
		return TAT_TEST_CRITICAL;
	}
	while( id->nextState != TAT_FINAL_STATE )
	{
		prevState=id->nextState;
		res = states[id->nextState](id);
		if (res == TAT_TEST_CRITICAL) return TAT_TEST_CRITICAL;
		if (res == TAT_TEST_FINISHED && id->testStatus != TAT_OK)
			return TAT_TEST_FINISHED;
		if (res == TAT_TEST_FINISHED && id->nextState != TAT_FINAL_STATE && id->nextState==prevState)
			return TAT_TEST_CONTINUE;
	}
	return TAT_TEST_FINISHED;
}


/****************************************************************************/
static char * tat_battleshipstest_Test0008N1_MessageTable[] = {
	"InitDataComplete()",
	"ShipsCurrentState(1,(: (. 1, 1, CRUISER, 8, EAST, 70, (. 1, 1 .), (: 15, 15, 3, 0 :) .) :))",
	"VisibleShip(1,(: (. 2, 2, REPAIR_BOAT, 0, NORTH_WEST, (. 25, 25 .) .) :))",
	"StartSimulation(0)",
	"OrderSpeedRequest(1,1,-1.0000)",
	"FinishSimulation(0,1)",
	"StrategyVictory(1)"
};
static TAT_SRC_FILE_DEF tat_battleshipstest_Test0008N1_SrcFiles[] = {
	{"C:\\Work\\Testing\\BSTest\\mpr\\Test0008.mpr", 1, 5, 18 },
	{"C:\\Work\\Testing\\BSTest\\work\\Test0008N1.mpr", 2, 5, 18 }};
TAT_TEST_RESULT TAT_TI_battleshipstest_Test0008N1( TAT_TEST_INSTANCE* id )
{
	id->varDefs = NULL;
	id->timeLabels = NULL;
	id->srcFiles = (TAT_SRC_FILE_DEF*)&tat_battleshipstest_Test0008N1_SrcFiles;
	id->timeLabelCount = 0;
	id->srcFilesCount = 2;
	id->varCount = 0;
	id->branchCount = 0;
	id->repeatCount = 1;
	id->timerCount  = 0;
	id->loopCount   = 0;
	return TAT_TEST_CONTINUE;
}
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0008N1_N0[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0008N1_N1[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0008N1_N2[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0008N1_N3[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0008N1_N4[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0008N1_N5[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0008N1_N6[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0008N1_0_SrcRef[] = {{0, 5}, {1, 5}};
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0008N1_0_event = {TAT_SEND_EV, TAT_S_InitDataComplete_ID, NULL, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0008N1_N0, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0008N1_0_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0008N1_MessageTable[0],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0008N1_1_SrcRef[] = {{0, 7}, {1, 7}};
static TAT_SIG_ShipsCurrentState tat_battleshipstest_Test0008N1_1_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0008N1_1_event = {TAT_SEND_EV, TAT_S_ShipsCurrentState_ID, &tat_battleshipstest_Test0008N1_1_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0008N1_N1, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0008N1_1_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0008N1_MessageTable[1],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0008N1_2_SrcRef[] = {{0, 9}, {1, 9}};
static TAT_SIG_VisibleShip tat_battleshipstest_Test0008N1_2_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0008N1_2_event = {TAT_SEND_EV, TAT_S_VisibleShip_ID, &tat_battleshipstest_Test0008N1_2_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0008N1_N2, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0008N1_2_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0008N1_MessageTable[2],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0008N1_3_SrcRef[] = {{0, 11}, {1, 11}};
static TAT_SIG_StartSimulation tat_battleshipstest_Test0008N1_3_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0008N1_3_event = {TAT_SEND_EV, TAT_S_StartSimulation_ID, &tat_battleshipstest_Test0008N1_3_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0008N1_N3, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0008N1_3_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0008N1_MessageTable[3],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0008N1_4_SrcRef[] = {{0, 13}, {1, 13}};
static TAT_SIG_OrderSpeedRequest tat_battleshipstest_Test0008N1_4_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0008N1_4_event = {TAT_RECV_EV, TAT_S_OrderSpeedRequest_ID, &tat_battleshipstest_Test0008N1_4_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0008N1_N4, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0008N1_4_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0008N1_MessageTable[4],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0008N1_5_SrcRef[] = {{0, 15}, {1, 15}};
static TAT_SIG_FinishSimulation tat_battleshipstest_Test0008N1_5_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0008N1_5_event = {TAT_RECV_EV, TAT_S_FinishSimulation_ID, &tat_battleshipstest_Test0008N1_5_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0008N1_N5, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0008N1_5_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0008N1_MessageTable[5],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0008N1_6_SrcRef[] = {{0, 17}, {1, 17}};
static TAT_SIG_StrategyVictory tat_battleshipstest_Test0008N1_6_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0008N1_6_event = {TAT_SEND_EV, TAT_S_StrategyVictory_ID, &tat_battleshipstest_Test0008N1_6_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0008N1_N6, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0008N1_6_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0008N1_MessageTable[6],
NULL};

tStringShipCurrentState TAT_Ass_Test0008N1_1_1 (TAT_TEST_INSTANCE *id) {
	tStringShipCurrentState x;
	x.First=(tStringShipCurrentState_yrec*)malloc(sizeof(tStringShipCurrentState_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringShipCurrentState_Cur=x.First;
	TAT_tStringShipCurrentState_Cur->Suc=NULL;
	TAT_tStringShipCurrentState_Cur->Data.ShipId=1;
	TAT_tStringShipCurrentState_Cur->Data.StrategyId=1;
	TAT_tStringShipCurrentState_Cur->Data.TypeShip=CRUISER;
	TAT_tStringShipCurrentState_Cur->Data.Speed=8;
	TAT_tStringShipCurrentState_Cur->Data.Heading=EAST;
	TAT_tStringShipCurrentState_Cur->Data.Resource=70;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.x=1;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.y=1;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[0]=15;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[1]=15;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[2]=3;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[3]=0;
	return x;
}
tStringEnemyShip TAT_Ass_Test0008N1_2_1 (TAT_TEST_INSTANCE *id) {
	tStringEnemyShip x;
	x.First=(tStringEnemyShip_yrec*)malloc(sizeof(tStringEnemyShip_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringEnemyShip_Cur=x.First;
	TAT_tStringEnemyShip_Cur->Suc=NULL;
	TAT_tStringEnemyShip_Cur->Data.ShipId=2;
	TAT_tStringEnemyShip_Cur->Data.StrategyId=2;
	TAT_tStringEnemyShip_Cur->Data.TypeShip=REPAIR_BOAT;
	TAT_tStringEnemyShip_Cur->Data.Speed=0;
	TAT_tStringEnemyShip_Cur->Data.Heading=NORTH_WEST;
	TAT_tStringEnemyShip_Cur->Data.Coordinates.x=25;
	TAT_tStringEnemyShip_Cur->Data.Coordinates.y=25;
	return x;
}
static TAT_TEST_RESULT tat_Test0008N1_state0(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0008N1_0_event, sizeof(__ev));
			if (TAT_Send_InitDataComplete(id, &__ev, &tat_battleshipstest_Test0008N1_0_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=1;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0008N1_state1(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0008N1_1_event, sizeof(__ev));
			if (TAT_Send_ShipsCurrentState(id, &__ev, &tat_battleshipstest_Test0008N1_1_event, 1, TAT_Ass_Test0008N1_1_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=2;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0008N1_state2(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0008N1_2_event, sizeof(__ev));
			if (TAT_Send_VisibleShip(id, &__ev, &tat_battleshipstest_Test0008N1_2_event, 1, TAT_Ass_Test0008N1_2_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=3;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0008N1_state3(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0008N1_3_event, sizeof(__ev));
			if (TAT_Send_StartSimulation(id, &__ev, &tat_battleshipstest_Test0008N1_3_event, 0)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=4;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0008N1_state4(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_OrderSpeedRequest_ID)&&
				((((TAT_SIG_OrderSpeedRequest *)_ev->eventData)->Param1==(1)))&&
				((((TAT_SIG_OrderSpeedRequest *)_ev->eventData)->Param2==(1)))&&
				((((TAT_SIG_OrderSpeedRequest *)_ev->eventData)->Param3==(-1.0000)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0008N1_4_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=5;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0008N1_state5(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_FinishSimulation_ID)&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param1==(0)))&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param2==(1)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0008N1_5_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=6;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0008N1_state6(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0008N1_6_event, sizeof(__ev));
			if (TAT_Send_StrategyVictory(id, &__ev, &tat_battleshipstest_Test0008N1_6_event, 1)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=-1;
		}
	return TAT_TEST_FINISHED;
}
TAT_TEST_RESULT TAT_T_battleshipstest_Test0008N1( TAT_TEST_INSTANCE* id )
{
	static const TAT_STATE_FUNC states[7]={
	tat_Test0008N1_state0,tat_Test0008N1_state1,tat_Test0008N1_state2,tat_Test0008N1_state3,tat_Test0008N1_state4,
	tat_Test0008N1_state5,tat_Test0008N1_state6};
	TAT_TEST_RESULT res;
	int prevState;
	if( id == NULL )
	{
		id->testStatus = TAT_ERR_INTERNAL;
		id->testResult = TAT_TEST_CRITICAL;
		id->dumpEvent(id, NULL, NULL);
		return TAT_TEST_CRITICAL;
	}
	while( id->nextState != TAT_FINAL_STATE )
	{
		prevState=id->nextState;
		res = states[id->nextState](id);
		if (res == TAT_TEST_CRITICAL) return TAT_TEST_CRITICAL;
		if (res == TAT_TEST_FINISHED && id->testStatus != TAT_OK)
			return TAT_TEST_FINISHED;
		if (res == TAT_TEST_FINISHED && id->nextState != TAT_FINAL_STATE && id->nextState==prevState)
			return TAT_TEST_CONTINUE;
	}
	return TAT_TEST_FINISHED;
}


/****************************************************************************/
static char * tat_battleshipstest_Test0014N1_MessageTable[] = {
	"InitDataComplete()",
	"ShipsCurrentState(1,(: (. 1, 1, CRUISER, 0, SOUTH, 70, (. 10, 10 .), (: 0, 1, 0, 0 :) .) :))",
	"VisibleShip(1,(: (. 2, 2, REPAIR_BOAT, 0, NORTH_WEST, (. 25, 25 .) .) :))",
	"StartSimulation(0)",
	"OrderTurnRequest(1,1,LEFT)",
	"FinishSimulation(0,1)",
	"StrategyVictory(1)"
};
static TAT_SRC_FILE_DEF tat_battleshipstest_Test0014N1_SrcFiles[] = {
	{"C:\\Work\\Testing\\BSTest\\mpr\\Test0014.mpr", 1, 5, 18 },
	{"C:\\Work\\Testing\\BSTest\\work\\Test0014N1.mpr", 2, 5, 18 }};
TAT_TEST_RESULT TAT_TI_battleshipstest_Test0014N1( TAT_TEST_INSTANCE* id )
{
	id->varDefs = NULL;
	id->timeLabels = NULL;
	id->srcFiles = (TAT_SRC_FILE_DEF*)&tat_battleshipstest_Test0014N1_SrcFiles;
	id->timeLabelCount = 0;
	id->srcFilesCount = 2;
	id->varCount = 0;
	id->branchCount = 0;
	id->repeatCount = 1;
	id->timerCount  = 0;
	id->loopCount   = 0;
	return TAT_TEST_CONTINUE;
}
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0014N1_N0[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0014N1_N1[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0014N1_N2[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0014N1_N3[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0014N1_N4[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0014N1_N5[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0014N1_N6[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0014N1_0_SrcRef[] = {{0, 5}, {1, 5}};
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0014N1_0_event = {TAT_SEND_EV, TAT_S_InitDataComplete_ID, NULL, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0014N1_N0, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0014N1_0_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0014N1_MessageTable[0],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0014N1_1_SrcRef[] = {{0, 7}, {1, 7}};
static TAT_SIG_ShipsCurrentState tat_battleshipstest_Test0014N1_1_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0014N1_1_event = {TAT_SEND_EV, TAT_S_ShipsCurrentState_ID, &tat_battleshipstest_Test0014N1_1_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0014N1_N1, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0014N1_1_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0014N1_MessageTable[1],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0014N1_2_SrcRef[] = {{0, 9}, {1, 9}};
static TAT_SIG_VisibleShip tat_battleshipstest_Test0014N1_2_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0014N1_2_event = {TAT_SEND_EV, TAT_S_VisibleShip_ID, &tat_battleshipstest_Test0014N1_2_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0014N1_N2, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0014N1_2_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0014N1_MessageTable[2],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0014N1_3_SrcRef[] = {{0, 11}, {1, 11}};
static TAT_SIG_StartSimulation tat_battleshipstest_Test0014N1_3_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0014N1_3_event = {TAT_SEND_EV, TAT_S_StartSimulation_ID, &tat_battleshipstest_Test0014N1_3_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0014N1_N3, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0014N1_3_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0014N1_MessageTable[3],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0014N1_4_SrcRef[] = {{0, 13}, {1, 13}};
static TAT_SIG_OrderTurnRequest tat_battleshipstest_Test0014N1_4_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0014N1_4_event = {TAT_RECV_EV, TAT_S_OrderTurnRequest_ID, &tat_battleshipstest_Test0014N1_4_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0014N1_N4, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0014N1_4_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0014N1_MessageTable[4],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0014N1_5_SrcRef[] = {{0, 15}, {1, 15}};
static TAT_SIG_FinishSimulation tat_battleshipstest_Test0014N1_5_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0014N1_5_event = {TAT_RECV_EV, TAT_S_FinishSimulation_ID, &tat_battleshipstest_Test0014N1_5_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0014N1_N5, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0014N1_5_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0014N1_MessageTable[5],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0014N1_6_SrcRef[] = {{0, 17}, {1, 17}};
static TAT_SIG_StrategyVictory tat_battleshipstest_Test0014N1_6_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0014N1_6_event = {TAT_SEND_EV, TAT_S_StrategyVictory_ID, &tat_battleshipstest_Test0014N1_6_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0014N1_N6, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0014N1_6_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0014N1_MessageTable[6],
NULL};

tStringShipCurrentState TAT_Ass_Test0014N1_1_1 (TAT_TEST_INSTANCE *id) {
	tStringShipCurrentState x;
	x.First=(tStringShipCurrentState_yrec*)malloc(sizeof(tStringShipCurrentState_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringShipCurrentState_Cur=x.First;
	TAT_tStringShipCurrentState_Cur->Suc=NULL;
	TAT_tStringShipCurrentState_Cur->Data.ShipId=1;
	TAT_tStringShipCurrentState_Cur->Data.StrategyId=1;
	TAT_tStringShipCurrentState_Cur->Data.TypeShip=CRUISER;
	TAT_tStringShipCurrentState_Cur->Data.Speed=0;
	TAT_tStringShipCurrentState_Cur->Data.Heading=SOUTH;
	TAT_tStringShipCurrentState_Cur->Data.Resource=70;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.x=10;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.y=10;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[0]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[1]=1;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[2]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[3]=0;
	return x;
}
tStringEnemyShip TAT_Ass_Test0014N1_2_1 (TAT_TEST_INSTANCE *id) {
	tStringEnemyShip x;
	x.First=(tStringEnemyShip_yrec*)malloc(sizeof(tStringEnemyShip_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringEnemyShip_Cur=x.First;
	TAT_tStringEnemyShip_Cur->Suc=NULL;
	TAT_tStringEnemyShip_Cur->Data.ShipId=2;
	TAT_tStringEnemyShip_Cur->Data.StrategyId=2;
	TAT_tStringEnemyShip_Cur->Data.TypeShip=REPAIR_BOAT;
	TAT_tStringEnemyShip_Cur->Data.Speed=0;
	TAT_tStringEnemyShip_Cur->Data.Heading=NORTH_WEST;
	TAT_tStringEnemyShip_Cur->Data.Coordinates.x=25;
	TAT_tStringEnemyShip_Cur->Data.Coordinates.y=25;
	return x;
}
static TAT_TEST_RESULT tat_Test0014N1_state0(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0014N1_0_event, sizeof(__ev));
			if (TAT_Send_InitDataComplete(id, &__ev, &tat_battleshipstest_Test0014N1_0_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=1;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0014N1_state1(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0014N1_1_event, sizeof(__ev));
			if (TAT_Send_ShipsCurrentState(id, &__ev, &tat_battleshipstest_Test0014N1_1_event, 1, TAT_Ass_Test0014N1_1_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=2;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0014N1_state2(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0014N1_2_event, sizeof(__ev));
			if (TAT_Send_VisibleShip(id, &__ev, &tat_battleshipstest_Test0014N1_2_event, 1, TAT_Ass_Test0014N1_2_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=3;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0014N1_state3(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0014N1_3_event, sizeof(__ev));
			if (TAT_Send_StartSimulation(id, &__ev, &tat_battleshipstest_Test0014N1_3_event, 0)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=4;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0014N1_state4(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_OrderTurnRequest_ID)&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param1==(1)))&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param2==(1)))&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param3==(LEFT)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0014N1_4_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=5;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0014N1_state5(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_FinishSimulation_ID)&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param1==(0)))&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param2==(1)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0014N1_5_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=6;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0014N1_state6(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0014N1_6_event, sizeof(__ev));
			if (TAT_Send_StrategyVictory(id, &__ev, &tat_battleshipstest_Test0014N1_6_event, 1)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=-1;
		}
	return TAT_TEST_FINISHED;
}
TAT_TEST_RESULT TAT_T_battleshipstest_Test0014N1( TAT_TEST_INSTANCE* id )
{
	static const TAT_STATE_FUNC states[7]={
	tat_Test0014N1_state0,tat_Test0014N1_state1,tat_Test0014N1_state2,tat_Test0014N1_state3,tat_Test0014N1_state4,
	tat_Test0014N1_state5,tat_Test0014N1_state6};
	TAT_TEST_RESULT res;
	int prevState;
	if( id == NULL )
	{
		id->testStatus = TAT_ERR_INTERNAL;
		id->testResult = TAT_TEST_CRITICAL;
		id->dumpEvent(id, NULL, NULL);
		return TAT_TEST_CRITICAL;
	}
	while( id->nextState != TAT_FINAL_STATE )
	{
		prevState=id->nextState;
		res = states[id->nextState](id);
		if (res == TAT_TEST_CRITICAL) return TAT_TEST_CRITICAL;
		if (res == TAT_TEST_FINISHED && id->testStatus != TAT_OK)
			return TAT_TEST_FINISHED;
		if (res == TAT_TEST_FINISHED && id->nextState != TAT_FINAL_STATE && id->nextState==prevState)
			return TAT_TEST_CONTINUE;
	}
	return TAT_TEST_FINISHED;
}


/****************************************************************************/
static char * tat_battleshipstest_Test0022N1_MessageTable[] = {
	"InitDataComplete()",
	"ShipsCurrentState(1,(: (. 1, 1, CRUISER, 0, SOUTH, 20, (. 10, 10 .), (: 0, 0, 0, 0 :) .), (. 2, 1, REPAIR_BOAT, 0, SOUTH, 25, (. 12, 10 .), (: 0, 0, 0, 10 :) .), (. 3, 1, CRUISER, 0, SOUTH, 5, (. 15, 10 .), (: 0, 0, 0, 0 :) .) :))",
	"VisibleShip(1,(: (. 1, 2, REPAIR_BOAT, 0, NORTH_WEST, (. 25, 25 .) .) :))",
	"StartSimulation(0)",
	"OrderTurnRequest(1,1,LEFT)",
	"OrderFireRequest(2,1,REPAIR,(: (. 15, 10 .), (. 10, 10 .) :))",
	"OrderTurnRequest(3,1,LEFT)",
	"FinishSimulation(0,1)",
	"StrategyVictory(1)"
};
static TAT_SRC_FILE_DEF tat_battleshipstest_Test0022N1_SrcFiles[] = {
	{"C:\\Work\\Testing\\BSTest\\mpr\\Test0022.mpr", 1, 5, 22 },
	{"C:\\Work\\Testing\\BSTest\\work\\Test0022N1.mpr", 2, 5, 22 }};
TAT_TEST_RESULT TAT_TI_battleshipstest_Test0022N1( TAT_TEST_INSTANCE* id )
{
	id->varDefs = NULL;
	id->timeLabels = NULL;
	id->srcFiles = (TAT_SRC_FILE_DEF*)&tat_battleshipstest_Test0022N1_SrcFiles;
	id->timeLabelCount = 0;
	id->srcFilesCount = 2;
	id->varCount = 0;
	id->branchCount = 0;
	id->repeatCount = 1;
	id->timerCount  = 0;
	id->loopCount   = 0;
	return TAT_TEST_CONTINUE;
}
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N1_N0[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N1_N1[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N1_N2[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N1_N3[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N1_N4[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N1_N5[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N1_N6[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N1_N7[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N1_N8[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N1_0_SrcRef[] = {{0, 5}, {1, 5}};
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N1_0_event = {TAT_SEND_EV, TAT_S_InitDataComplete_ID, NULL, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N1_N0, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N1_0_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N1_MessageTable[0],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N1_1_SrcRef[] = {{0, 7}, {1, 7}};
static TAT_SIG_ShipsCurrentState tat_battleshipstest_Test0022N1_1_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N1_1_event = {TAT_SEND_EV, TAT_S_ShipsCurrentState_ID, &tat_battleshipstest_Test0022N1_1_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N1_N1, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N1_1_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N1_MessageTable[1],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N1_2_SrcRef[] = {{0, 9}, {1, 9}};
static TAT_SIG_VisibleShip tat_battleshipstest_Test0022N1_2_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N1_2_event = {TAT_SEND_EV, TAT_S_VisibleShip_ID, &tat_battleshipstest_Test0022N1_2_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N1_N2, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N1_2_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N1_MessageTable[2],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N1_3_SrcRef[] = {{0, 11}, {1, 11}};
static TAT_SIG_StartSimulation tat_battleshipstest_Test0022N1_3_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N1_3_event = {TAT_SEND_EV, TAT_S_StartSimulation_ID, &tat_battleshipstest_Test0022N1_3_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N1_N3, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N1_3_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N1_MessageTable[3],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N1_4_SrcRef[] = {{0, 13}, {1, 13}};
static TAT_SIG_OrderTurnRequest tat_battleshipstest_Test0022N1_4_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N1_4_event = {TAT_RECV_EV, TAT_S_OrderTurnRequest_ID, &tat_battleshipstest_Test0022N1_4_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N1_N4, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N1_4_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N1_MessageTable[4],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N1_5_SrcRef[] = {{0, 15}, {1, 15}};
static TAT_SIG_OrderFireRequest tat_battleshipstest_Test0022N1_5_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N1_5_event = {TAT_RECV_EV, TAT_S_OrderFireRequest_ID, &tat_battleshipstest_Test0022N1_5_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N1_N5, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N1_5_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N1_MessageTable[5],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N1_6_SrcRef[] = {{0, 17}, {1, 17}};
static TAT_SIG_OrderTurnRequest tat_battleshipstest_Test0022N1_6_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N1_6_event = {TAT_RECV_EV, TAT_S_OrderTurnRequest_ID, &tat_battleshipstest_Test0022N1_6_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N1_N6, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N1_6_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N1_MessageTable[6],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N1_7_SrcRef[] = {{0, 19}, {1, 19}};
static TAT_SIG_FinishSimulation tat_battleshipstest_Test0022N1_7_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N1_7_event = {TAT_RECV_EV, TAT_S_FinishSimulation_ID, &tat_battleshipstest_Test0022N1_7_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N1_N7, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N1_7_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N1_MessageTable[7],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N1_8_SrcRef[] = {{0, 21}, {1, 21}};
static TAT_SIG_StrategyVictory tat_battleshipstest_Test0022N1_8_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N1_8_event = {TAT_SEND_EV, TAT_S_StrategyVictory_ID, &tat_battleshipstest_Test0022N1_8_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N1_N8, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N1_8_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N1_MessageTable[8],
NULL};

tStringShipCurrentState TAT_Ass_Test0022N1_1_1 (TAT_TEST_INSTANCE *id) {
	tStringShipCurrentState x;
	x.First=(tStringShipCurrentState_yrec*)malloc(sizeof(tStringShipCurrentState_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringShipCurrentState_Cur=x.First;
	TAT_tStringShipCurrentState_Cur->Suc=NULL;
	TAT_tStringShipCurrentState_Cur->Data.ShipId=1;
	TAT_tStringShipCurrentState_Cur->Data.StrategyId=1;
	TAT_tStringShipCurrentState_Cur->Data.TypeShip=CRUISER;
	TAT_tStringShipCurrentState_Cur->Data.Speed=0;
	TAT_tStringShipCurrentState_Cur->Data.Heading=SOUTH;
	TAT_tStringShipCurrentState_Cur->Data.Resource=20;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.x=10;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.y=10;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[0]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[1]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[2]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[3]=0;
	TAT_tStringShipCurrentState_Cur->Suc=(tStringShipCurrentState_yrec*)malloc(sizeof(tStringShipCurrentState_yrec));
	TAT_tStringShipCurrentState_Cur=TAT_tStringShipCurrentState_Cur->Suc;
	TAT_tStringShipCurrentState_Cur->Suc=0;
	x.Last=TAT_tStringShipCurrentState_Cur;
	x.Length++;
	TAT_tStringShipCurrentState_Cur->Data.ShipId=2;
	TAT_tStringShipCurrentState_Cur->Data.StrategyId=1;
	TAT_tStringShipCurrentState_Cur->Data.TypeShip=REPAIR_BOAT;
	TAT_tStringShipCurrentState_Cur->Data.Speed=0;
	TAT_tStringShipCurrentState_Cur->Data.Heading=SOUTH;
	TAT_tStringShipCurrentState_Cur->Data.Resource=25;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.x=12;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.y=10;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[0]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[1]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[2]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[3]=10;
	TAT_tStringShipCurrentState_Cur->Suc=(tStringShipCurrentState_yrec*)malloc(sizeof(tStringShipCurrentState_yrec));
	TAT_tStringShipCurrentState_Cur=TAT_tStringShipCurrentState_Cur->Suc;
	TAT_tStringShipCurrentState_Cur->Suc=0;
	x.Last=TAT_tStringShipCurrentState_Cur;
	x.Length++;
	TAT_tStringShipCurrentState_Cur->Data.ShipId=3;
	TAT_tStringShipCurrentState_Cur->Data.StrategyId=1;
	TAT_tStringShipCurrentState_Cur->Data.TypeShip=CRUISER;
	TAT_tStringShipCurrentState_Cur->Data.Speed=0;
	TAT_tStringShipCurrentState_Cur->Data.Heading=SOUTH;
	TAT_tStringShipCurrentState_Cur->Data.Resource=5;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.x=15;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.y=10;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[0]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[1]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[2]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[3]=0;
	return x;
}
tStringEnemyShip TAT_Ass_Test0022N1_2_1 (TAT_TEST_INSTANCE *id) {
	tStringEnemyShip x;
	x.First=(tStringEnemyShip_yrec*)malloc(sizeof(tStringEnemyShip_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringEnemyShip_Cur=x.First;
	TAT_tStringEnemyShip_Cur->Suc=NULL;
	TAT_tStringEnemyShip_Cur->Data.ShipId=1;
	TAT_tStringEnemyShip_Cur->Data.StrategyId=2;
	TAT_tStringEnemyShip_Cur->Data.TypeShip=REPAIR_BOAT;
	TAT_tStringEnemyShip_Cur->Data.Speed=0;
	TAT_tStringEnemyShip_Cur->Data.Heading=NORTH_WEST;
	TAT_tStringEnemyShip_Cur->Data.Coordinates.x=25;
	TAT_tStringEnemyShip_Cur->Data.Coordinates.y=25;
	return x;
}
int TAT_Cmp_Test0022N1_5_3_1 (tStringShotsList x, TAT_TEST_INSTANCE *id) {
	TAT_tStringShotsList_Cur=x.First;
	if(TAT_tStringShotsList_Cur==NULL) return 1;
	if(TAT_tStringShotsList_Cur->Data.x!=15) return 1;
	if(TAT_tStringShotsList_Cur->Data.y!=10) return 1;
	TAT_tStringShotsList_Cur=TAT_tStringShotsList_Cur->Suc;
	if(TAT_tStringShotsList_Cur==NULL) return 1;
	if(TAT_tStringShotsList_Cur->Data.x!=10) return 1;
	if(TAT_tStringShotsList_Cur->Data.y!=10) return 1;
	TAT_tStringShotsList_Cur=TAT_tStringShotsList_Cur->Suc;
	if(TAT_tStringShotsList_Cur!=NULL) return 1;
	return 0;
}
static TAT_TEST_RESULT tat_Test0022N1_state0(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0022N1_0_event, sizeof(__ev));
			if (TAT_Send_InitDataComplete(id, &__ev, &tat_battleshipstest_Test0022N1_0_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=1;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0022N1_state1(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0022N1_1_event, sizeof(__ev));
			if (TAT_Send_ShipsCurrentState(id, &__ev, &tat_battleshipstest_Test0022N1_1_event, 1, TAT_Ass_Test0022N1_1_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=2;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0022N1_state2(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0022N1_2_event, sizeof(__ev));
			if (TAT_Send_VisibleShip(id, &__ev, &tat_battleshipstest_Test0022N1_2_event, 1, TAT_Ass_Test0022N1_2_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=3;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0022N1_state3(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0022N1_3_event, sizeof(__ev));
			if (TAT_Send_StartSimulation(id, &__ev, &tat_battleshipstest_Test0022N1_3_event, 0)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=4;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0022N1_state4(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_OrderTurnRequest_ID)&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param1==(1)))&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param2==(1)))&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param3==(LEFT)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0022N1_4_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=5;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0022N1_state5(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_OrderFireRequest_ID)&&
				((((TAT_SIG_OrderFireRequest *)_ev->eventData)->Param1==(2)))&&
				((((TAT_SIG_OrderFireRequest *)_ev->eventData)->Param2==(1)))&&
				((((TAT_SIG_OrderFireRequest *)_ev->eventData)->Param3==(REPAIR)))&&
				((TAT_Cmp_Test0022N1_5_3_1(((TAT_SIG_OrderFireRequest *)_ev->eventData)->Param4,id) == 0))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0022N1_5_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				TAT_Free_tStringShotsList(&(((TAT_SIG_OrderFireRequest *)_ev->eventData)->Param4));
				id->popQueue(id);
				id->nextState=6;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0022N1_state6(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_OrderTurnRequest_ID)&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param1==(3)))&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param2==(1)))&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param3==(LEFT)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0022N1_6_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=7;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0022N1_state7(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_FinishSimulation_ID)&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param1==(0)))&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param2==(1)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0022N1_7_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=8;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0022N1_state8(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0022N1_8_event, sizeof(__ev));
			if (TAT_Send_StrategyVictory(id, &__ev, &tat_battleshipstest_Test0022N1_8_event, 1)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=-1;
		}
	return TAT_TEST_FINISHED;
}
TAT_TEST_RESULT TAT_T_battleshipstest_Test0022N1( TAT_TEST_INSTANCE* id )
{
	static const TAT_STATE_FUNC states[9]={
	tat_Test0022N1_state0,tat_Test0022N1_state1,tat_Test0022N1_state2,tat_Test0022N1_state3,tat_Test0022N1_state4,
	tat_Test0022N1_state5,tat_Test0022N1_state6,tat_Test0022N1_state7,tat_Test0022N1_state8};
	TAT_TEST_RESULT res;
	int prevState;
	if( id == NULL )
	{
		id->testStatus = TAT_ERR_INTERNAL;
		id->testResult = TAT_TEST_CRITICAL;
		id->dumpEvent(id, NULL, NULL);
		return TAT_TEST_CRITICAL;
	}
	while( id->nextState != TAT_FINAL_STATE )
	{
		prevState=id->nextState;
		res = states[id->nextState](id);
		if (res == TAT_TEST_CRITICAL) return TAT_TEST_CRITICAL;
		if (res == TAT_TEST_FINISHED && id->testStatus != TAT_OK)
			return TAT_TEST_FINISHED;
		if (res == TAT_TEST_FINISHED && id->nextState != TAT_FINAL_STATE && id->nextState==prevState)
			return TAT_TEST_CONTINUE;
	}
	return TAT_TEST_FINISHED;
}


/****************************************************************************/
static char * tat_battleshipstest_Test0032N1_MessageTable[] = {
	"InitDataComplete()",
	"ShipsCurrentState(1,(: (. 1, 1, CRUISER, 8, EAST, 70, (. 40, 25 .), (: 15, 15, 3, 0 :) .) :))",
	"VisibleShip(1,(: (. 1, 2, REPAIR_BOAT, 0, NORTH_WEST, (. 15, 15 .) .) :))",
	"StartSimulation(0)",
	"OrderSpeedRequest(1,1,-1.0000)",
	"FinishSimulation(0,1)",
	"StrategyVictory(1)"
};
static TAT_SRC_FILE_DEF tat_battleshipstest_Test0032N1_SrcFiles[] = {
	{"C:\\Work\\Testing\\BSTest\\mpr\\Test0032.mpr", 1, 5, 18 },
	{"C:\\Work\\Testing\\BSTest\\work\\Test0032N1.mpr", 2, 5, 18 }};
TAT_TEST_RESULT TAT_TI_battleshipstest_Test0032N1( TAT_TEST_INSTANCE* id )
{
	id->varDefs = NULL;
	id->timeLabels = NULL;
	id->srcFiles = (TAT_SRC_FILE_DEF*)&tat_battleshipstest_Test0032N1_SrcFiles;
	id->timeLabelCount = 0;
	id->srcFilesCount = 2;
	id->varCount = 0;
	id->branchCount = 0;
	id->repeatCount = 1;
	id->timerCount  = 0;
	id->loopCount   = 0;
	return TAT_TEST_CONTINUE;
}
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0032N1_N0[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0032N1_N1[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0032N1_N2[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0032N1_N3[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0032N1_N4[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0032N1_N5[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0032N1_N6[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0032N1_0_SrcRef[] = {{0, 5}, {1, 5}};
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0032N1_0_event = {TAT_SEND_EV, TAT_S_InitDataComplete_ID, NULL, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0032N1_N0, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0032N1_0_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0032N1_MessageTable[0],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0032N1_1_SrcRef[] = {{0, 7}, {1, 7}};
static TAT_SIG_ShipsCurrentState tat_battleshipstest_Test0032N1_1_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0032N1_1_event = {TAT_SEND_EV, TAT_S_ShipsCurrentState_ID, &tat_battleshipstest_Test0032N1_1_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0032N1_N1, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0032N1_1_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0032N1_MessageTable[1],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0032N1_2_SrcRef[] = {{0, 9}, {1, 9}};
static TAT_SIG_VisibleShip tat_battleshipstest_Test0032N1_2_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0032N1_2_event = {TAT_SEND_EV, TAT_S_VisibleShip_ID, &tat_battleshipstest_Test0032N1_2_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0032N1_N2, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0032N1_2_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0032N1_MessageTable[2],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0032N1_3_SrcRef[] = {{0, 11}, {1, 11}};
static TAT_SIG_StartSimulation tat_battleshipstest_Test0032N1_3_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0032N1_3_event = {TAT_SEND_EV, TAT_S_StartSimulation_ID, &tat_battleshipstest_Test0032N1_3_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0032N1_N3, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0032N1_3_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0032N1_MessageTable[3],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0032N1_4_SrcRef[] = {{0, 13}, {1, 13}};
static TAT_SIG_OrderSpeedRequest tat_battleshipstest_Test0032N1_4_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0032N1_4_event = {TAT_RECV_EV, TAT_S_OrderSpeedRequest_ID, &tat_battleshipstest_Test0032N1_4_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0032N1_N4, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0032N1_4_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0032N1_MessageTable[4],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0032N1_5_SrcRef[] = {{0, 15}, {1, 15}};
static TAT_SIG_FinishSimulation tat_battleshipstest_Test0032N1_5_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0032N1_5_event = {TAT_RECV_EV, TAT_S_FinishSimulation_ID, &tat_battleshipstest_Test0032N1_5_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0032N1_N5, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0032N1_5_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0032N1_MessageTable[5],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0032N1_6_SrcRef[] = {{0, 17}, {1, 17}};
static TAT_SIG_StrategyVictory tat_battleshipstest_Test0032N1_6_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0032N1_6_event = {TAT_SEND_EV, TAT_S_StrategyVictory_ID, &tat_battleshipstest_Test0032N1_6_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0032N1_N6, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0032N1_6_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0032N1_MessageTable[6],
NULL};

tStringShipCurrentState TAT_Ass_Test0032N1_1_1 (TAT_TEST_INSTANCE *id) {
	tStringShipCurrentState x;
	x.First=(tStringShipCurrentState_yrec*)malloc(sizeof(tStringShipCurrentState_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringShipCurrentState_Cur=x.First;
	TAT_tStringShipCurrentState_Cur->Suc=NULL;
	TAT_tStringShipCurrentState_Cur->Data.ShipId=1;
	TAT_tStringShipCurrentState_Cur->Data.StrategyId=1;
	TAT_tStringShipCurrentState_Cur->Data.TypeShip=CRUISER;
	TAT_tStringShipCurrentState_Cur->Data.Speed=8;
	TAT_tStringShipCurrentState_Cur->Data.Heading=EAST;
	TAT_tStringShipCurrentState_Cur->Data.Resource=70;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.x=40;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.y=25;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[0]=15;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[1]=15;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[2]=3;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[3]=0;
	return x;
}
tStringEnemyShip TAT_Ass_Test0032N1_2_1 (TAT_TEST_INSTANCE *id) {
	tStringEnemyShip x;
	x.First=(tStringEnemyShip_yrec*)malloc(sizeof(tStringEnemyShip_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringEnemyShip_Cur=x.First;
	TAT_tStringEnemyShip_Cur->Suc=NULL;
	TAT_tStringEnemyShip_Cur->Data.ShipId=1;
	TAT_tStringEnemyShip_Cur->Data.StrategyId=2;
	TAT_tStringEnemyShip_Cur->Data.TypeShip=REPAIR_BOAT;
	TAT_tStringEnemyShip_Cur->Data.Speed=0;
	TAT_tStringEnemyShip_Cur->Data.Heading=NORTH_WEST;
	TAT_tStringEnemyShip_Cur->Data.Coordinates.x=15;
	TAT_tStringEnemyShip_Cur->Data.Coordinates.y=15;
	return x;
}
static TAT_TEST_RESULT tat_Test0032N1_state0(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0032N1_0_event, sizeof(__ev));
			if (TAT_Send_InitDataComplete(id, &__ev, &tat_battleshipstest_Test0032N1_0_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=1;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0032N1_state1(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0032N1_1_event, sizeof(__ev));
			if (TAT_Send_ShipsCurrentState(id, &__ev, &tat_battleshipstest_Test0032N1_1_event, 1, TAT_Ass_Test0032N1_1_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=2;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0032N1_state2(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0032N1_2_event, sizeof(__ev));
			if (TAT_Send_VisibleShip(id, &__ev, &tat_battleshipstest_Test0032N1_2_event, 1, TAT_Ass_Test0032N1_2_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=3;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0032N1_state3(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0032N1_3_event, sizeof(__ev));
			if (TAT_Send_StartSimulation(id, &__ev, &tat_battleshipstest_Test0032N1_3_event, 0)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=4;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0032N1_state4(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_OrderSpeedRequest_ID)&&
				((((TAT_SIG_OrderSpeedRequest *)_ev->eventData)->Param1==(1)))&&
				((((TAT_SIG_OrderSpeedRequest *)_ev->eventData)->Param2==(1)))&&
				((((TAT_SIG_OrderSpeedRequest *)_ev->eventData)->Param3==(-1.0000)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0032N1_4_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=5;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0032N1_state5(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_FinishSimulation_ID)&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param1==(0)))&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param2==(1)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0032N1_5_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=6;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0032N1_state6(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0032N1_6_event, sizeof(__ev));
			if (TAT_Send_StrategyVictory(id, &__ev, &tat_battleshipstest_Test0032N1_6_event, 1)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=-1;
		}
	return TAT_TEST_FINISHED;
}
TAT_TEST_RESULT TAT_T_battleshipstest_Test0032N1( TAT_TEST_INSTANCE* id )
{
	static const TAT_STATE_FUNC states[7]={
	tat_Test0032N1_state0,tat_Test0032N1_state1,tat_Test0032N1_state2,tat_Test0032N1_state3,tat_Test0032N1_state4,
	tat_Test0032N1_state5,tat_Test0032N1_state6};
	TAT_TEST_RESULT res;
	int prevState;
	if( id == NULL )
	{
		id->testStatus = TAT_ERR_INTERNAL;
		id->testResult = TAT_TEST_CRITICAL;
		id->dumpEvent(id, NULL, NULL);
		return TAT_TEST_CRITICAL;
	}
	while( id->nextState != TAT_FINAL_STATE )
	{
		prevState=id->nextState;
		res = states[id->nextState](id);
		if (res == TAT_TEST_CRITICAL) return TAT_TEST_CRITICAL;
		if (res == TAT_TEST_FINISHED && id->testStatus != TAT_OK)
			return TAT_TEST_FINISHED;
		if (res == TAT_TEST_FINISHED && id->nextState != TAT_FINAL_STATE && id->nextState==prevState)
			return TAT_TEST_CONTINUE;
	}
	return TAT_TEST_FINISHED;
}


/****************************************************************************/
static char * tat_battleshipstest_Test0001N2_MessageTable[] = {
	"InitDataComplete()",
	"ShipsCurrentState(1,(: (. 1, 1, REPAIR_BOAT, 15, SOUTH_EAST, 70, (. 13, 15 .), (: 15, 15, 3, 0 :) .) :))",
	"StartSimulation(0)",
	"OrderTurnRequest(1,1,LEFT)",
	"FinishSimulation(0,1)"
};
static TAT_SRC_FILE_DEF tat_battleshipstest_Test0001N2_SrcFiles[] = {
	{"C:\\Work\\Testing\\BSTest\\mpr\\Test0001.mpr", 1, 5, 14 },
	{"C:\\Work\\Testing\\BSTest\\work\\Test0001N2.mpr", 2, 5, 14 }};
TAT_TEST_RESULT TAT_TI_battleshipstest_Test0001N2( TAT_TEST_INSTANCE* id )
{
	id->varDefs = NULL;
	id->timeLabels = NULL;
	id->srcFiles = (TAT_SRC_FILE_DEF*)&tat_battleshipstest_Test0001N2_SrcFiles;
	id->timeLabelCount = 0;
	id->srcFilesCount = 2;
	id->varCount = 0;
	id->branchCount = 0;
	id->repeatCount = 1;
	id->timerCount  = 0;
	id->loopCount   = 0;
	return TAT_TEST_CONTINUE;
}
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0001N2_N0[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0001N2_N1[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0001N2_N2[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0001N2_N3[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0001N2_N4[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0001N2_0_SrcRef[] = {{0, 5}, {1, 5}};
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0001N2_0_event = {TAT_SEND_EV, TAT_S_InitDataComplete_ID, NULL, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0001N2_N0, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0001N2_0_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0001N2_MessageTable[0],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0001N2_1_SrcRef[] = {{0, 7}, {1, 7}};
static TAT_SIG_ShipsCurrentState tat_battleshipstest_Test0001N2_1_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0001N2_1_event = {TAT_SEND_EV, TAT_S_ShipsCurrentState_ID, &tat_battleshipstest_Test0001N2_1_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0001N2_N1, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0001N2_1_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0001N2_MessageTable[1],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0001N2_2_SrcRef[] = {{0, 9}, {1, 9}};
static TAT_SIG_StartSimulation tat_battleshipstest_Test0001N2_2_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0001N2_2_event = {TAT_SEND_EV, TAT_S_StartSimulation_ID, &tat_battleshipstest_Test0001N2_2_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0001N2_N2, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0001N2_2_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0001N2_MessageTable[2],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0001N2_3_SrcRef[] = {{0, 11}, {1, 11}};
static TAT_SIG_OrderTurnRequest tat_battleshipstest_Test0001N2_3_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0001N2_3_event = {TAT_RECV_EV, TAT_S_OrderTurnRequest_ID, &tat_battleshipstest_Test0001N2_3_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0001N2_N3, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0001N2_3_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0001N2_MessageTable[3],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0001N2_4_SrcRef[] = {{0, 13}, {1, 13}};
static TAT_SIG_FinishSimulation tat_battleshipstest_Test0001N2_4_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0001N2_4_event = {TAT_RECV_EV, TAT_S_FinishSimulation_ID, &tat_battleshipstest_Test0001N2_4_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0001N2_N4, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0001N2_4_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0001N2_MessageTable[4],
NULL};

tStringShipCurrentState TAT_Ass_Test0001N2_1_1 (TAT_TEST_INSTANCE *id) {
	tStringShipCurrentState x;
	x.First=(tStringShipCurrentState_yrec*)malloc(sizeof(tStringShipCurrentState_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringShipCurrentState_Cur=x.First;
	TAT_tStringShipCurrentState_Cur->Suc=NULL;
	TAT_tStringShipCurrentState_Cur->Data.ShipId=1;
	TAT_tStringShipCurrentState_Cur->Data.StrategyId=1;
	TAT_tStringShipCurrentState_Cur->Data.TypeShip=REPAIR_BOAT;
	TAT_tStringShipCurrentState_Cur->Data.Speed=15;
	TAT_tStringShipCurrentState_Cur->Data.Heading=SOUTH_EAST;
	TAT_tStringShipCurrentState_Cur->Data.Resource=70;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.x=13;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.y=15;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[0]=15;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[1]=15;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[2]=3;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[3]=0;
	return x;
}
static TAT_TEST_RESULT tat_Test0001N2_state0(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0001N2_0_event, sizeof(__ev));
			if (TAT_Send_InitDataComplete(id, &__ev, &tat_battleshipstest_Test0001N2_0_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=1;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0001N2_state1(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0001N2_1_event, sizeof(__ev));
			if (TAT_Send_ShipsCurrentState(id, &__ev, &tat_battleshipstest_Test0001N2_1_event, 1, TAT_Ass_Test0001N2_1_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=2;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0001N2_state2(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0001N2_2_event, sizeof(__ev));
			if (TAT_Send_StartSimulation(id, &__ev, &tat_battleshipstest_Test0001N2_2_event, 0)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=3;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0001N2_state3(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_OrderTurnRequest_ID)&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param1==(1)))&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param2==(1)))&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param3==(LEFT)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0001N2_3_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=4;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0001N2_state4(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_FinishSimulation_ID)&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param1==(0)))&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param2==(1)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0001N2_4_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=-1;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
TAT_TEST_RESULT TAT_T_battleshipstest_Test0001N2( TAT_TEST_INSTANCE* id )
{
	static const TAT_STATE_FUNC states[5]={
	tat_Test0001N2_state0,tat_Test0001N2_state1,tat_Test0001N2_state2,tat_Test0001N2_state3,tat_Test0001N2_state4};
	TAT_TEST_RESULT res;
	int prevState;
	if( id == NULL )
	{
		id->testStatus = TAT_ERR_INTERNAL;
		id->testResult = TAT_TEST_CRITICAL;
		id->dumpEvent(id, NULL, NULL);
		return TAT_TEST_CRITICAL;
	}
	while( id->nextState != TAT_FINAL_STATE )
	{
		prevState=id->nextState;
		res = states[id->nextState](id);
		if (res == TAT_TEST_CRITICAL) return TAT_TEST_CRITICAL;
		if (res == TAT_TEST_FINISHED && id->testStatus != TAT_OK)
			return TAT_TEST_FINISHED;
		if (res == TAT_TEST_FINISHED && id->nextState != TAT_FINAL_STATE && id->nextState==prevState)
			return TAT_TEST_CONTINUE;
	}
	return TAT_TEST_FINISHED;
}


/****************************************************************************/
static char * tat_battleshipstest_Test0008N2_MessageTable[] = {
	"InitDataComplete()",
	"ShipsCurrentState(1,(: (. 1, 1, CRUISER, 8, EAST, 70, (. 1, 1 .), (: 15, 15, 3, 0 :) .) :))",
	"VisibleShip(1,(: (. 2, 2, REPAIR_BOAT, 0, NORTH_WEST, (. 25, 25 .) .) :))",
	"StartSimulation(0)",
	"OrderSpeedRequest(1,1,-1.0000)",
	"FinishSimulation(0,1)",
	"StrategyVictory(1)"
};
static TAT_SRC_FILE_DEF tat_battleshipstest_Test0008N2_SrcFiles[] = {
	{"C:\\Work\\Testing\\BSTest\\mpr\\Test0008.mpr", 1, 5, 18 },
	{"C:\\Work\\Testing\\BSTest\\work\\Test0008N2.mpr", 2, 5, 18 }};
TAT_TEST_RESULT TAT_TI_battleshipstest_Test0008N2( TAT_TEST_INSTANCE* id )
{
	id->varDefs = NULL;
	id->timeLabels = NULL;
	id->srcFiles = (TAT_SRC_FILE_DEF*)&tat_battleshipstest_Test0008N2_SrcFiles;
	id->timeLabelCount = 0;
	id->srcFilesCount = 2;
	id->varCount = 0;
	id->branchCount = 0;
	id->repeatCount = 1;
	id->timerCount  = 0;
	id->loopCount   = 0;
	return TAT_TEST_CONTINUE;
}
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0008N2_N0[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0008N2_N1[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0008N2_N2[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0008N2_N3[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0008N2_N4[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0008N2_N5[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0008N2_N6[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0008N2_0_SrcRef[] = {{0, 5}, {1, 5}};
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0008N2_0_event = {TAT_SEND_EV, TAT_S_InitDataComplete_ID, NULL, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0008N2_N0, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0008N2_0_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0008N2_MessageTable[0],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0008N2_1_SrcRef[] = {{0, 7}, {1, 7}};
static TAT_SIG_ShipsCurrentState tat_battleshipstest_Test0008N2_1_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0008N2_1_event = {TAT_SEND_EV, TAT_S_ShipsCurrentState_ID, &tat_battleshipstest_Test0008N2_1_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0008N2_N1, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0008N2_1_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0008N2_MessageTable[1],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0008N2_2_SrcRef[] = {{0, 9}, {1, 9}};
static TAT_SIG_VisibleShip tat_battleshipstest_Test0008N2_2_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0008N2_2_event = {TAT_SEND_EV, TAT_S_VisibleShip_ID, &tat_battleshipstest_Test0008N2_2_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0008N2_N2, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0008N2_2_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0008N2_MessageTable[2],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0008N2_3_SrcRef[] = {{0, 11}, {1, 11}};
static TAT_SIG_StartSimulation tat_battleshipstest_Test0008N2_3_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0008N2_3_event = {TAT_SEND_EV, TAT_S_StartSimulation_ID, &tat_battleshipstest_Test0008N2_3_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0008N2_N3, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0008N2_3_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0008N2_MessageTable[3],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0008N2_4_SrcRef[] = {{0, 13}, {1, 13}};
static TAT_SIG_OrderSpeedRequest tat_battleshipstest_Test0008N2_4_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0008N2_4_event = {TAT_RECV_EV, TAT_S_OrderSpeedRequest_ID, &tat_battleshipstest_Test0008N2_4_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0008N2_N4, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0008N2_4_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0008N2_MessageTable[4],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0008N2_5_SrcRef[] = {{0, 15}, {1, 15}};
static TAT_SIG_FinishSimulation tat_battleshipstest_Test0008N2_5_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0008N2_5_event = {TAT_RECV_EV, TAT_S_FinishSimulation_ID, &tat_battleshipstest_Test0008N2_5_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0008N2_N5, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0008N2_5_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0008N2_MessageTable[5],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0008N2_6_SrcRef[] = {{0, 17}, {1, 17}};
static TAT_SIG_StrategyVictory tat_battleshipstest_Test0008N2_6_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0008N2_6_event = {TAT_SEND_EV, TAT_S_StrategyVictory_ID, &tat_battleshipstest_Test0008N2_6_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0008N2_N6, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0008N2_6_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0008N2_MessageTable[6],
NULL};

tStringShipCurrentState TAT_Ass_Test0008N2_1_1 (TAT_TEST_INSTANCE *id) {
	tStringShipCurrentState x;
	x.First=(tStringShipCurrentState_yrec*)malloc(sizeof(tStringShipCurrentState_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringShipCurrentState_Cur=x.First;
	TAT_tStringShipCurrentState_Cur->Suc=NULL;
	TAT_tStringShipCurrentState_Cur->Data.ShipId=1;
	TAT_tStringShipCurrentState_Cur->Data.StrategyId=1;
	TAT_tStringShipCurrentState_Cur->Data.TypeShip=CRUISER;
	TAT_tStringShipCurrentState_Cur->Data.Speed=8;
	TAT_tStringShipCurrentState_Cur->Data.Heading=EAST;
	TAT_tStringShipCurrentState_Cur->Data.Resource=70;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.x=1;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.y=1;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[0]=15;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[1]=15;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[2]=3;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[3]=0;
	return x;
}
tStringEnemyShip TAT_Ass_Test0008N2_2_1 (TAT_TEST_INSTANCE *id) {
	tStringEnemyShip x;
	x.First=(tStringEnemyShip_yrec*)malloc(sizeof(tStringEnemyShip_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringEnemyShip_Cur=x.First;
	TAT_tStringEnemyShip_Cur->Suc=NULL;
	TAT_tStringEnemyShip_Cur->Data.ShipId=2;
	TAT_tStringEnemyShip_Cur->Data.StrategyId=2;
	TAT_tStringEnemyShip_Cur->Data.TypeShip=REPAIR_BOAT;
	TAT_tStringEnemyShip_Cur->Data.Speed=0;
	TAT_tStringEnemyShip_Cur->Data.Heading=NORTH_WEST;
	TAT_tStringEnemyShip_Cur->Data.Coordinates.x=25;
	TAT_tStringEnemyShip_Cur->Data.Coordinates.y=25;
	return x;
}
static TAT_TEST_RESULT tat_Test0008N2_state0(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0008N2_0_event, sizeof(__ev));
			if (TAT_Send_InitDataComplete(id, &__ev, &tat_battleshipstest_Test0008N2_0_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=1;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0008N2_state1(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0008N2_1_event, sizeof(__ev));
			if (TAT_Send_ShipsCurrentState(id, &__ev, &tat_battleshipstest_Test0008N2_1_event, 1, TAT_Ass_Test0008N2_1_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=2;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0008N2_state2(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0008N2_2_event, sizeof(__ev));
			if (TAT_Send_VisibleShip(id, &__ev, &tat_battleshipstest_Test0008N2_2_event, 1, TAT_Ass_Test0008N2_2_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=3;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0008N2_state3(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0008N2_3_event, sizeof(__ev));
			if (TAT_Send_StartSimulation(id, &__ev, &tat_battleshipstest_Test0008N2_3_event, 0)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=4;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0008N2_state4(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_OrderSpeedRequest_ID)&&
				((((TAT_SIG_OrderSpeedRequest *)_ev->eventData)->Param1==(1)))&&
				((((TAT_SIG_OrderSpeedRequest *)_ev->eventData)->Param2==(1)))&&
				((((TAT_SIG_OrderSpeedRequest *)_ev->eventData)->Param3==(-1.0000)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0008N2_4_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=5;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0008N2_state5(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_FinishSimulation_ID)&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param1==(0)))&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param2==(1)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0008N2_5_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=6;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0008N2_state6(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0008N2_6_event, sizeof(__ev));
			if (TAT_Send_StrategyVictory(id, &__ev, &tat_battleshipstest_Test0008N2_6_event, 1)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=-1;
		}
	return TAT_TEST_FINISHED;
}
TAT_TEST_RESULT TAT_T_battleshipstest_Test0008N2( TAT_TEST_INSTANCE* id )
{
	static const TAT_STATE_FUNC states[7]={
	tat_Test0008N2_state0,tat_Test0008N2_state1,tat_Test0008N2_state2,tat_Test0008N2_state3,tat_Test0008N2_state4,
	tat_Test0008N2_state5,tat_Test0008N2_state6};
	TAT_TEST_RESULT res;
	int prevState;
	if( id == NULL )
	{
		id->testStatus = TAT_ERR_INTERNAL;
		id->testResult = TAT_TEST_CRITICAL;
		id->dumpEvent(id, NULL, NULL);
		return TAT_TEST_CRITICAL;
	}
	while( id->nextState != TAT_FINAL_STATE )
	{
		prevState=id->nextState;
		res = states[id->nextState](id);
		if (res == TAT_TEST_CRITICAL) return TAT_TEST_CRITICAL;
		if (res == TAT_TEST_FINISHED && id->testStatus != TAT_OK)
			return TAT_TEST_FINISHED;
		if (res == TAT_TEST_FINISHED && id->nextState != TAT_FINAL_STATE && id->nextState==prevState)
			return TAT_TEST_CONTINUE;
	}
	return TAT_TEST_FINISHED;
}


/****************************************************************************/
static char * tat_battleshipstest_Test0014N2_MessageTable[] = {
	"InitDataComplete()",
	"ShipsCurrentState(1,(: (. 1, 1, CRUISER, 0, SOUTH, 70, (. 10, 10 .), (: 0, 1, 0, 0 :) .) :))",
	"VisibleShip(1,(: (. 2, 2, REPAIR_BOAT, 0, NORTH_WEST, (. 25, 25 .) .) :))",
	"StartSimulation(0)",
	"OrderTurnRequest(1,1,LEFT)",
	"FinishSimulation(0,1)",
	"StrategyVictory(1)"
};
static TAT_SRC_FILE_DEF tat_battleshipstest_Test0014N2_SrcFiles[] = {
	{"C:\\Work\\Testing\\BSTest\\mpr\\Test0014.mpr", 1, 5, 18 },
	{"C:\\Work\\Testing\\BSTest\\work\\Test0014N2.mpr", 2, 5, 18 }};
TAT_TEST_RESULT TAT_TI_battleshipstest_Test0014N2( TAT_TEST_INSTANCE* id )
{
	id->varDefs = NULL;
	id->timeLabels = NULL;
	id->srcFiles = (TAT_SRC_FILE_DEF*)&tat_battleshipstest_Test0014N2_SrcFiles;
	id->timeLabelCount = 0;
	id->srcFilesCount = 2;
	id->varCount = 0;
	id->branchCount = 0;
	id->repeatCount = 1;
	id->timerCount  = 0;
	id->loopCount   = 0;
	return TAT_TEST_CONTINUE;
}
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0014N2_N0[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0014N2_N1[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0014N2_N2[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0014N2_N3[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0014N2_N4[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0014N2_N5[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0014N2_N6[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0014N2_0_SrcRef[] = {{0, 5}, {1, 5}};
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0014N2_0_event = {TAT_SEND_EV, TAT_S_InitDataComplete_ID, NULL, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0014N2_N0, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0014N2_0_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0014N2_MessageTable[0],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0014N2_1_SrcRef[] = {{0, 7}, {1, 7}};
static TAT_SIG_ShipsCurrentState tat_battleshipstest_Test0014N2_1_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0014N2_1_event = {TAT_SEND_EV, TAT_S_ShipsCurrentState_ID, &tat_battleshipstest_Test0014N2_1_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0014N2_N1, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0014N2_1_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0014N2_MessageTable[1],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0014N2_2_SrcRef[] = {{0, 9}, {1, 9}};
static TAT_SIG_VisibleShip tat_battleshipstest_Test0014N2_2_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0014N2_2_event = {TAT_SEND_EV, TAT_S_VisibleShip_ID, &tat_battleshipstest_Test0014N2_2_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0014N2_N2, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0014N2_2_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0014N2_MessageTable[2],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0014N2_3_SrcRef[] = {{0, 11}, {1, 11}};
static TAT_SIG_StartSimulation tat_battleshipstest_Test0014N2_3_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0014N2_3_event = {TAT_SEND_EV, TAT_S_StartSimulation_ID, &tat_battleshipstest_Test0014N2_3_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0014N2_N3, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0014N2_3_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0014N2_MessageTable[3],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0014N2_4_SrcRef[] = {{0, 13}, {1, 13}};
static TAT_SIG_OrderTurnRequest tat_battleshipstest_Test0014N2_4_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0014N2_4_event = {TAT_RECV_EV, TAT_S_OrderTurnRequest_ID, &tat_battleshipstest_Test0014N2_4_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0014N2_N4, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0014N2_4_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0014N2_MessageTable[4],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0014N2_5_SrcRef[] = {{0, 15}, {1, 15}};
static TAT_SIG_FinishSimulation tat_battleshipstest_Test0014N2_5_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0014N2_5_event = {TAT_RECV_EV, TAT_S_FinishSimulation_ID, &tat_battleshipstest_Test0014N2_5_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0014N2_N5, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0014N2_5_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0014N2_MessageTable[5],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0014N2_6_SrcRef[] = {{0, 17}, {1, 17}};
static TAT_SIG_StrategyVictory tat_battleshipstest_Test0014N2_6_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0014N2_6_event = {TAT_SEND_EV, TAT_S_StrategyVictory_ID, &tat_battleshipstest_Test0014N2_6_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0014N2_N6, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0014N2_6_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0014N2_MessageTable[6],
NULL};

tStringShipCurrentState TAT_Ass_Test0014N2_1_1 (TAT_TEST_INSTANCE *id) {
	tStringShipCurrentState x;
	x.First=(tStringShipCurrentState_yrec*)malloc(sizeof(tStringShipCurrentState_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringShipCurrentState_Cur=x.First;
	TAT_tStringShipCurrentState_Cur->Suc=NULL;
	TAT_tStringShipCurrentState_Cur->Data.ShipId=1;
	TAT_tStringShipCurrentState_Cur->Data.StrategyId=1;
	TAT_tStringShipCurrentState_Cur->Data.TypeShip=CRUISER;
	TAT_tStringShipCurrentState_Cur->Data.Speed=0;
	TAT_tStringShipCurrentState_Cur->Data.Heading=SOUTH;
	TAT_tStringShipCurrentState_Cur->Data.Resource=70;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.x=10;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.y=10;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[0]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[1]=1;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[2]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[3]=0;
	return x;
}
tStringEnemyShip TAT_Ass_Test0014N2_2_1 (TAT_TEST_INSTANCE *id) {
	tStringEnemyShip x;
	x.First=(tStringEnemyShip_yrec*)malloc(sizeof(tStringEnemyShip_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringEnemyShip_Cur=x.First;
	TAT_tStringEnemyShip_Cur->Suc=NULL;
	TAT_tStringEnemyShip_Cur->Data.ShipId=2;
	TAT_tStringEnemyShip_Cur->Data.StrategyId=2;
	TAT_tStringEnemyShip_Cur->Data.TypeShip=REPAIR_BOAT;
	TAT_tStringEnemyShip_Cur->Data.Speed=0;
	TAT_tStringEnemyShip_Cur->Data.Heading=NORTH_WEST;
	TAT_tStringEnemyShip_Cur->Data.Coordinates.x=25;
	TAT_tStringEnemyShip_Cur->Data.Coordinates.y=25;
	return x;
}
static TAT_TEST_RESULT tat_Test0014N2_state0(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0014N2_0_event, sizeof(__ev));
			if (TAT_Send_InitDataComplete(id, &__ev, &tat_battleshipstest_Test0014N2_0_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=1;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0014N2_state1(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0014N2_1_event, sizeof(__ev));
			if (TAT_Send_ShipsCurrentState(id, &__ev, &tat_battleshipstest_Test0014N2_1_event, 1, TAT_Ass_Test0014N2_1_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=2;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0014N2_state2(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0014N2_2_event, sizeof(__ev));
			if (TAT_Send_VisibleShip(id, &__ev, &tat_battleshipstest_Test0014N2_2_event, 1, TAT_Ass_Test0014N2_2_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=3;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0014N2_state3(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0014N2_3_event, sizeof(__ev));
			if (TAT_Send_StartSimulation(id, &__ev, &tat_battleshipstest_Test0014N2_3_event, 0)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=4;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0014N2_state4(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_OrderTurnRequest_ID)&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param1==(1)))&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param2==(1)))&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param3==(LEFT)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0014N2_4_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=5;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0014N2_state5(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_FinishSimulation_ID)&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param1==(0)))&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param2==(1)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0014N2_5_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=6;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0014N2_state6(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0014N2_6_event, sizeof(__ev));
			if (TAT_Send_StrategyVictory(id, &__ev, &tat_battleshipstest_Test0014N2_6_event, 1)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=-1;
		}
	return TAT_TEST_FINISHED;
}
TAT_TEST_RESULT TAT_T_battleshipstest_Test0014N2( TAT_TEST_INSTANCE* id )
{
	static const TAT_STATE_FUNC states[7]={
	tat_Test0014N2_state0,tat_Test0014N2_state1,tat_Test0014N2_state2,tat_Test0014N2_state3,tat_Test0014N2_state4,
	tat_Test0014N2_state5,tat_Test0014N2_state6};
	TAT_TEST_RESULT res;
	int prevState;
	if( id == NULL )
	{
		id->testStatus = TAT_ERR_INTERNAL;
		id->testResult = TAT_TEST_CRITICAL;
		id->dumpEvent(id, NULL, NULL);
		return TAT_TEST_CRITICAL;
	}
	while( id->nextState != TAT_FINAL_STATE )
	{
		prevState=id->nextState;
		res = states[id->nextState](id);
		if (res == TAT_TEST_CRITICAL) return TAT_TEST_CRITICAL;
		if (res == TAT_TEST_FINISHED && id->testStatus != TAT_OK)
			return TAT_TEST_FINISHED;
		if (res == TAT_TEST_FINISHED && id->nextState != TAT_FINAL_STATE && id->nextState==prevState)
			return TAT_TEST_CONTINUE;
	}
	return TAT_TEST_FINISHED;
}


/****************************************************************************/
static char * tat_battleshipstest_Test0022N2_MessageTable[] = {
	"InitDataComplete()",
	"ShipsCurrentState(1,(: (. 1, 1, CRUISER, 0, SOUTH, 20, (. 10, 10 .), (: 0, 0, 0, 0 :) .), (. 2, 1, REPAIR_BOAT, 0, SOUTH, 25, (. 12, 10 .), (: 0, 0, 0, 10 :) .), (. 3, 1, CRUISER, 0, SOUTH, 5, (. 15, 10 .), (: 0, 0, 0, 0 :) .) :))",
	"VisibleShip(1,(: (. 1, 2, REPAIR_BOAT, 0, NORTH_WEST, (. 25, 25 .) .) :))",
	"StartSimulation(0)",
	"OrderTurnRequest(1,1,LEFT)",
	"OrderFireRequest(2,1,REPAIR,(: (. 15, 10 .), (. 10, 10 .) :))",
	"OrderTurnRequest(3,1,LEFT)",
	"FinishSimulation(0,1)",
	"StrategyVictory(1)"
};
static TAT_SRC_FILE_DEF tat_battleshipstest_Test0022N2_SrcFiles[] = {
	{"C:\\Work\\Testing\\BSTest\\mpr\\Test0022.mpr", 1, 5, 22 },
	{"C:\\Work\\Testing\\BSTest\\work\\Test0022N2.mpr", 2, 5, 22 }};
TAT_TEST_RESULT TAT_TI_battleshipstest_Test0022N2( TAT_TEST_INSTANCE* id )
{
	id->varDefs = NULL;
	id->timeLabels = NULL;
	id->srcFiles = (TAT_SRC_FILE_DEF*)&tat_battleshipstest_Test0022N2_SrcFiles;
	id->timeLabelCount = 0;
	id->srcFilesCount = 2;
	id->varCount = 0;
	id->branchCount = 0;
	id->repeatCount = 1;
	id->timerCount  = 0;
	id->loopCount   = 0;
	return TAT_TEST_CONTINUE;
}
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N2_N0[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N2_N1[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N2_N2[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N2_N3[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N2_N4[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N2_N5[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N2_N6[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N2_N7[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0022N2_N8[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N2_0_SrcRef[] = {{0, 5}, {1, 5}};
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N2_0_event = {TAT_SEND_EV, TAT_S_InitDataComplete_ID, NULL, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N2_N0, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N2_0_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N2_MessageTable[0],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N2_1_SrcRef[] = {{0, 7}, {1, 7}};
static TAT_SIG_ShipsCurrentState tat_battleshipstest_Test0022N2_1_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N2_1_event = {TAT_SEND_EV, TAT_S_ShipsCurrentState_ID, &tat_battleshipstest_Test0022N2_1_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N2_N1, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N2_1_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N2_MessageTable[1],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N2_2_SrcRef[] = {{0, 9}, {1, 9}};
static TAT_SIG_VisibleShip tat_battleshipstest_Test0022N2_2_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N2_2_event = {TAT_SEND_EV, TAT_S_VisibleShip_ID, &tat_battleshipstest_Test0022N2_2_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N2_N2, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N2_2_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N2_MessageTable[2],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N2_3_SrcRef[] = {{0, 11}, {1, 11}};
static TAT_SIG_StartSimulation tat_battleshipstest_Test0022N2_3_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N2_3_event = {TAT_SEND_EV, TAT_S_StartSimulation_ID, &tat_battleshipstest_Test0022N2_3_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N2_N3, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N2_3_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N2_MessageTable[3],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N2_4_SrcRef[] = {{0, 13}, {1, 13}};
static TAT_SIG_OrderTurnRequest tat_battleshipstest_Test0022N2_4_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N2_4_event = {TAT_RECV_EV, TAT_S_OrderTurnRequest_ID, &tat_battleshipstest_Test0022N2_4_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N2_N4, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N2_4_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N2_MessageTable[4],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N2_5_SrcRef[] = {{0, 15}, {1, 15}};
static TAT_SIG_OrderFireRequest tat_battleshipstest_Test0022N2_5_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N2_5_event = {TAT_RECV_EV, TAT_S_OrderFireRequest_ID, &tat_battleshipstest_Test0022N2_5_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N2_N5, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N2_5_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N2_MessageTable[5],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N2_6_SrcRef[] = {{0, 17}, {1, 17}};
static TAT_SIG_OrderTurnRequest tat_battleshipstest_Test0022N2_6_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N2_6_event = {TAT_RECV_EV, TAT_S_OrderTurnRequest_ID, &tat_battleshipstest_Test0022N2_6_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N2_N6, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N2_6_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N2_MessageTable[6],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N2_7_SrcRef[] = {{0, 19}, {1, 19}};
static TAT_SIG_FinishSimulation tat_battleshipstest_Test0022N2_7_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N2_7_event = {TAT_RECV_EV, TAT_S_FinishSimulation_ID, &tat_battleshipstest_Test0022N2_7_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N2_N7, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N2_7_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N2_MessageTable[7],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0022N2_8_SrcRef[] = {{0, 21}, {1, 21}};
static TAT_SIG_StrategyVictory tat_battleshipstest_Test0022N2_8_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0022N2_8_event = {TAT_SEND_EV, TAT_S_StrategyVictory_ID, &tat_battleshipstest_Test0022N2_8_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0022N2_N8, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0022N2_8_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0022N2_MessageTable[8],
NULL};

tStringShipCurrentState TAT_Ass_Test0022N2_1_1 (TAT_TEST_INSTANCE *id) {
	tStringShipCurrentState x;
	x.First=(tStringShipCurrentState_yrec*)malloc(sizeof(tStringShipCurrentState_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringShipCurrentState_Cur=x.First;
	TAT_tStringShipCurrentState_Cur->Suc=NULL;
	TAT_tStringShipCurrentState_Cur->Data.ShipId=1;
	TAT_tStringShipCurrentState_Cur->Data.StrategyId=1;
	TAT_tStringShipCurrentState_Cur->Data.TypeShip=CRUISER;
	TAT_tStringShipCurrentState_Cur->Data.Speed=0;
	TAT_tStringShipCurrentState_Cur->Data.Heading=SOUTH;
	TAT_tStringShipCurrentState_Cur->Data.Resource=20;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.x=10;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.y=10;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[0]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[1]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[2]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[3]=0;
	TAT_tStringShipCurrentState_Cur->Suc=(tStringShipCurrentState_yrec*)malloc(sizeof(tStringShipCurrentState_yrec));
	TAT_tStringShipCurrentState_Cur=TAT_tStringShipCurrentState_Cur->Suc;
	TAT_tStringShipCurrentState_Cur->Suc=0;
	x.Last=TAT_tStringShipCurrentState_Cur;
	x.Length++;
	TAT_tStringShipCurrentState_Cur->Data.ShipId=2;
	TAT_tStringShipCurrentState_Cur->Data.StrategyId=1;
	TAT_tStringShipCurrentState_Cur->Data.TypeShip=REPAIR_BOAT;
	TAT_tStringShipCurrentState_Cur->Data.Speed=0;
	TAT_tStringShipCurrentState_Cur->Data.Heading=SOUTH;
	TAT_tStringShipCurrentState_Cur->Data.Resource=25;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.x=12;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.y=10;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[0]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[1]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[2]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[3]=10;
	TAT_tStringShipCurrentState_Cur->Suc=(tStringShipCurrentState_yrec*)malloc(sizeof(tStringShipCurrentState_yrec));
	TAT_tStringShipCurrentState_Cur=TAT_tStringShipCurrentState_Cur->Suc;
	TAT_tStringShipCurrentState_Cur->Suc=0;
	x.Last=TAT_tStringShipCurrentState_Cur;
	x.Length++;
	TAT_tStringShipCurrentState_Cur->Data.ShipId=3;
	TAT_tStringShipCurrentState_Cur->Data.StrategyId=1;
	TAT_tStringShipCurrentState_Cur->Data.TypeShip=CRUISER;
	TAT_tStringShipCurrentState_Cur->Data.Speed=0;
	TAT_tStringShipCurrentState_Cur->Data.Heading=SOUTH;
	TAT_tStringShipCurrentState_Cur->Data.Resource=5;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.x=15;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.y=10;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[0]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[1]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[2]=0;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[3]=0;
	return x;
}
tStringEnemyShip TAT_Ass_Test0022N2_2_1 (TAT_TEST_INSTANCE *id) {
	tStringEnemyShip x;
	x.First=(tStringEnemyShip_yrec*)malloc(sizeof(tStringEnemyShip_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringEnemyShip_Cur=x.First;
	TAT_tStringEnemyShip_Cur->Suc=NULL;
	TAT_tStringEnemyShip_Cur->Data.ShipId=1;
	TAT_tStringEnemyShip_Cur->Data.StrategyId=2;
	TAT_tStringEnemyShip_Cur->Data.TypeShip=REPAIR_BOAT;
	TAT_tStringEnemyShip_Cur->Data.Speed=0;
	TAT_tStringEnemyShip_Cur->Data.Heading=NORTH_WEST;
	TAT_tStringEnemyShip_Cur->Data.Coordinates.x=25;
	TAT_tStringEnemyShip_Cur->Data.Coordinates.y=25;
	return x;
}
int TAT_Cmp_Test0022N2_5_3_1 (tStringShotsList x, TAT_TEST_INSTANCE *id) {
	TAT_tStringShotsList_Cur=x.First;
	if(TAT_tStringShotsList_Cur==NULL) return 1;
	if(TAT_tStringShotsList_Cur->Data.x!=15) return 1;
	if(TAT_tStringShotsList_Cur->Data.y!=10) return 1;
	TAT_tStringShotsList_Cur=TAT_tStringShotsList_Cur->Suc;
	if(TAT_tStringShotsList_Cur==NULL) return 1;
	if(TAT_tStringShotsList_Cur->Data.x!=10) return 1;
	if(TAT_tStringShotsList_Cur->Data.y!=10) return 1;
	TAT_tStringShotsList_Cur=TAT_tStringShotsList_Cur->Suc;
	if(TAT_tStringShotsList_Cur!=NULL) return 1;
	return 0;
}
static TAT_TEST_RESULT tat_Test0022N2_state0(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0022N2_0_event, sizeof(__ev));
			if (TAT_Send_InitDataComplete(id, &__ev, &tat_battleshipstest_Test0022N2_0_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=1;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0022N2_state1(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0022N2_1_event, sizeof(__ev));
			if (TAT_Send_ShipsCurrentState(id, &__ev, &tat_battleshipstest_Test0022N2_1_event, 1, TAT_Ass_Test0022N2_1_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=2;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0022N2_state2(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0022N2_2_event, sizeof(__ev));
			if (TAT_Send_VisibleShip(id, &__ev, &tat_battleshipstest_Test0022N2_2_event, 1, TAT_Ass_Test0022N2_2_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=3;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0022N2_state3(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0022N2_3_event, sizeof(__ev));
			if (TAT_Send_StartSimulation(id, &__ev, &tat_battleshipstest_Test0022N2_3_event, 0)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=4;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0022N2_state4(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_OrderTurnRequest_ID)&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param1==(1)))&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param2==(1)))&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param3==(LEFT)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0022N2_4_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=5;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0022N2_state5(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_OrderFireRequest_ID)&&
				((((TAT_SIG_OrderFireRequest *)_ev->eventData)->Param1==(2)))&&
				((((TAT_SIG_OrderFireRequest *)_ev->eventData)->Param2==(1)))&&
				((((TAT_SIG_OrderFireRequest *)_ev->eventData)->Param3==(REPAIR)))&&
				((TAT_Cmp_Test0022N2_5_3_1(((TAT_SIG_OrderFireRequest *)_ev->eventData)->Param4,id) == 0))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0022N2_5_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				TAT_Free_tStringShotsList(&(((TAT_SIG_OrderFireRequest *)_ev->eventData)->Param4));
				id->popQueue(id);
				id->nextState=6;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0022N2_state6(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_OrderTurnRequest_ID)&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param1==(3)))&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param2==(1)))&&
				((((TAT_SIG_OrderTurnRequest *)_ev->eventData)->Param3==(LEFT)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0022N2_6_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=7;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0022N2_state7(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_FinishSimulation_ID)&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param1==(0)))&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param2==(1)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0022N2_7_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=8;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0022N2_state8(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0022N2_8_event, sizeof(__ev));
			if (TAT_Send_StrategyVictory(id, &__ev, &tat_battleshipstest_Test0022N2_8_event, 1)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=-1;
		}
	return TAT_TEST_FINISHED;
}
TAT_TEST_RESULT TAT_T_battleshipstest_Test0022N2( TAT_TEST_INSTANCE* id )
{
	static const TAT_STATE_FUNC states[9]={
	tat_Test0022N2_state0,tat_Test0022N2_state1,tat_Test0022N2_state2,tat_Test0022N2_state3,tat_Test0022N2_state4,
	tat_Test0022N2_state5,tat_Test0022N2_state6,tat_Test0022N2_state7,tat_Test0022N2_state8};
	TAT_TEST_RESULT res;
	int prevState;
	if( id == NULL )
	{
		id->testStatus = TAT_ERR_INTERNAL;
		id->testResult = TAT_TEST_CRITICAL;
		id->dumpEvent(id, NULL, NULL);
		return TAT_TEST_CRITICAL;
	}
	while( id->nextState != TAT_FINAL_STATE )
	{
		prevState=id->nextState;
		res = states[id->nextState](id);
		if (res == TAT_TEST_CRITICAL) return TAT_TEST_CRITICAL;
		if (res == TAT_TEST_FINISHED && id->testStatus != TAT_OK)
			return TAT_TEST_FINISHED;
		if (res == TAT_TEST_FINISHED && id->nextState != TAT_FINAL_STATE && id->nextState==prevState)
			return TAT_TEST_CONTINUE;
	}
	return TAT_TEST_FINISHED;
}


/****************************************************************************/
static char * tat_battleshipstest_Test0032N2_MessageTable[] = {
	"InitDataComplete()",
	"ShipsCurrentState(1,(: (. 1, 1, CRUISER, 8, EAST, 70, (. 40, 25 .), (: 15, 15, 3, 0 :) .) :))",
	"VisibleShip(1,(: (. 1, 2, REPAIR_BOAT, 0, NORTH_WEST, (. 15, 15 .) .) :))",
	"StartSimulation(0)",
	"OrderSpeedRequest(1,1,-1.0000)",
	"FinishSimulation(0,1)",
	"StrategyVictory(1)"
};
static TAT_SRC_FILE_DEF tat_battleshipstest_Test0032N2_SrcFiles[] = {
	{"C:\\Work\\Testing\\BSTest\\mpr\\Test0032.mpr", 1, 5, 18 },
	{"C:\\Work\\Testing\\BSTest\\work\\Test0032N2.mpr", 2, 5, 18 }};
TAT_TEST_RESULT TAT_TI_battleshipstest_Test0032N2( TAT_TEST_INSTANCE* id )
{
	id->varDefs = NULL;
	id->timeLabels = NULL;
	id->srcFiles = (TAT_SRC_FILE_DEF*)&tat_battleshipstest_Test0032N2_SrcFiles;
	id->timeLabelCount = 0;
	id->srcFilesCount = 2;
	id->varCount = 0;
	id->branchCount = 0;
	id->repeatCount = 1;
	id->timerCount  = 0;
	id->loopCount   = 0;
	return TAT_TEST_CONTINUE;
}
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0032N2_N0[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0032N2_N1[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0032N2_N2[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0032N2_N3[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0032N2_N4[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0032N2_N5[]={TAT_I_Strategy_ID,TAT_I_Simulator_ID};
static TAT_INSTANCE_ID tat_I_battleshipstest_Test0032N2_N6[]={TAT_I_Simulator_ID,TAT_I_Strategy_ID};
static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0032N2_0_SrcRef[] = {{0, 5}, {1, 5}};
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0032N2_0_event = {TAT_SEND_EV, TAT_S_InitDataComplete_ID, NULL, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0032N2_N0, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0032N2_0_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0032N2_MessageTable[0],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0032N2_1_SrcRef[] = {{0, 7}, {1, 7}};
static TAT_SIG_ShipsCurrentState tat_battleshipstest_Test0032N2_1_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0032N2_1_event = {TAT_SEND_EV, TAT_S_ShipsCurrentState_ID, &tat_battleshipstest_Test0032N2_1_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0032N2_N1, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0032N2_1_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0032N2_MessageTable[1],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0032N2_2_SrcRef[] = {{0, 9}, {1, 9}};
static TAT_SIG_VisibleShip tat_battleshipstest_Test0032N2_2_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0032N2_2_event = {TAT_SEND_EV, TAT_S_VisibleShip_ID, &tat_battleshipstest_Test0032N2_2_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0032N2_N2, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0032N2_2_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0032N2_MessageTable[2],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0032N2_3_SrcRef[] = {{0, 11}, {1, 11}};
static TAT_SIG_StartSimulation tat_battleshipstest_Test0032N2_3_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0032N2_3_event = {TAT_SEND_EV, TAT_S_StartSimulation_ID, &tat_battleshipstest_Test0032N2_3_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0032N2_N3, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0032N2_3_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0032N2_MessageTable[3],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0032N2_4_SrcRef[] = {{0, 13}, {1, 13}};
static TAT_SIG_OrderSpeedRequest tat_battleshipstest_Test0032N2_4_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0032N2_4_event = {TAT_RECV_EV, TAT_S_OrderSpeedRequest_ID, &tat_battleshipstest_Test0032N2_4_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0032N2_N4, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0032N2_4_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0032N2_MessageTable[4],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0032N2_5_SrcRef[] = {{0, 15}, {1, 15}};
static TAT_SIG_FinishSimulation tat_battleshipstest_Test0032N2_5_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0032N2_5_event = {TAT_RECV_EV, TAT_S_FinishSimulation_ID, &tat_battleshipstest_Test0032N2_5_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0032N2_N5, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0032N2_5_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0032N2_MessageTable[5],
NULL};

static TAT_SRC_REF_STRUCT tat_battleshipstest_Test0032N2_6_SrcRef[] = {{0, 17}, {1, 17}};
static TAT_SIG_StrategyVictory tat_battleshipstest_Test0032N2_6_sigdata;
static TAT_EVENT_INSTANCE tat_battleshipstest_Test0032N2_6_event = {TAT_SEND_EV, TAT_S_StrategyVictory_ID, &tat_battleshipstest_Test0032N2_6_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_battleshipstest_Test0032N2_N6, "", 2, (TAT_SRC_REF_STRUCT *) tat_battleshipstest_Test0032N2_6_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_battleshipstest_Test0032N2_MessageTable[6],
NULL};

tStringShipCurrentState TAT_Ass_Test0032N2_1_1 (TAT_TEST_INSTANCE *id) {
	tStringShipCurrentState x;
	x.First=(tStringShipCurrentState_yrec*)malloc(sizeof(tStringShipCurrentState_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringShipCurrentState_Cur=x.First;
	TAT_tStringShipCurrentState_Cur->Suc=NULL;
	TAT_tStringShipCurrentState_Cur->Data.ShipId=1;
	TAT_tStringShipCurrentState_Cur->Data.StrategyId=1;
	TAT_tStringShipCurrentState_Cur->Data.TypeShip=CRUISER;
	TAT_tStringShipCurrentState_Cur->Data.Speed=8;
	TAT_tStringShipCurrentState_Cur->Data.Heading=EAST;
	TAT_tStringShipCurrentState_Cur->Data.Resource=70;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.x=40;
	TAT_tStringShipCurrentState_Cur->Data.Coordinates.y=25;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[0]=15;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[1]=15;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[2]=3;
	TAT_tStringShipCurrentState_Cur->Data.Ammunition.A[3]=0;
	return x;
}
tStringEnemyShip TAT_Ass_Test0032N2_2_1 (TAT_TEST_INSTANCE *id) {
	tStringEnemyShip x;
	x.First=(tStringEnemyShip_yrec*)malloc(sizeof(tStringEnemyShip_yrec));
	x.Last=x.First;
	x.Length=1;
	x.IsAssigned=1;
	TAT_tStringEnemyShip_Cur=x.First;
	TAT_tStringEnemyShip_Cur->Suc=NULL;
	TAT_tStringEnemyShip_Cur->Data.ShipId=1;
	TAT_tStringEnemyShip_Cur->Data.StrategyId=2;
	TAT_tStringEnemyShip_Cur->Data.TypeShip=REPAIR_BOAT;
	TAT_tStringEnemyShip_Cur->Data.Speed=0;
	TAT_tStringEnemyShip_Cur->Data.Heading=NORTH_WEST;
	TAT_tStringEnemyShip_Cur->Data.Coordinates.x=15;
	TAT_tStringEnemyShip_Cur->Data.Coordinates.y=15;
	return x;
}
static TAT_TEST_RESULT tat_Test0032N2_state0(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0032N2_0_event, sizeof(__ev));
			if (TAT_Send_InitDataComplete(id, &__ev, &tat_battleshipstest_Test0032N2_0_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=1;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0032N2_state1(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0032N2_1_event, sizeof(__ev));
			if (TAT_Send_ShipsCurrentState(id, &__ev, &tat_battleshipstest_Test0032N2_1_event, 1, TAT_Ass_Test0032N2_1_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=2;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0032N2_state2(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0032N2_2_event, sizeof(__ev));
			if (TAT_Send_VisibleShip(id, &__ev, &tat_battleshipstest_Test0032N2_2_event, 1, TAT_Ass_Test0032N2_2_1(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=3;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0032N2_state3(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0032N2_3_event, sizeof(__ev));
			if (TAT_Send_StartSimulation(id, &__ev, &tat_battleshipstest_Test0032N2_3_event, 0)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=4;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0032N2_state4(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_OrderSpeedRequest_ID)&&
				((((TAT_SIG_OrderSpeedRequest *)_ev->eventData)->Param1==(1)))&&
				((((TAT_SIG_OrderSpeedRequest *)_ev->eventData)->Param2==(1)))&&
				((((TAT_SIG_OrderSpeedRequest *)_ev->eventData)->Param3==(-1.0000)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0032N2_4_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=5;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0032N2_state5(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_FinishSimulation_ID)&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param1==(0)))&&
				((((TAT_SIG_FinishSimulation *)_ev->eventData)->Param2==(1)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_battleshipstest_Test0032N2_5_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->popQueue(id);
				id->nextState=6;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_Test0032N2_state6(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_battleshipstest_Test0032N2_6_event, sizeof(__ev));
			if (TAT_Send_StrategyVictory(id, &__ev, &tat_battleshipstest_Test0032N2_6_event, 1)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=-1;
		}
	return TAT_TEST_FINISHED;
}
TAT_TEST_RESULT TAT_T_battleshipstest_Test0032N2( TAT_TEST_INSTANCE* id )
{
	static const TAT_STATE_FUNC states[7]={
	tat_Test0032N2_state0,tat_Test0032N2_state1,tat_Test0032N2_state2,tat_Test0032N2_state3,tat_Test0032N2_state4,
	tat_Test0032N2_state5,tat_Test0032N2_state6};
	TAT_TEST_RESULT res;
	int prevState;
	if( id == NULL )
	{
		id->testStatus = TAT_ERR_INTERNAL;
		id->testResult = TAT_TEST_CRITICAL;
		id->dumpEvent(id, NULL, NULL);
		return TAT_TEST_CRITICAL;
	}
	while( id->nextState != TAT_FINAL_STATE )
	{
		prevState=id->nextState;
		res = states[id->nextState](id);
		if (res == TAT_TEST_CRITICAL) return TAT_TEST_CRITICAL;
		if (res == TAT_TEST_FINISHED && id->testStatus != TAT_OK)
			return TAT_TEST_FINISHED;
		if (res == TAT_TEST_FINISHED && id->nextState != TAT_FINAL_STATE && id->nextState==prevState)
			return TAT_TEST_CONTINUE;
	}
	return TAT_TEST_FINISHED;
}


/****************************************************************************/
