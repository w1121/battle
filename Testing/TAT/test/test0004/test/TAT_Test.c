#include "TAT_Test.h"

static char * tat_TAT_Test_test_MessageTable[] = {
	"A(\"Hello\",\"World\")",
	"B(\"World\",\"Hello\")"
};
static TAT_SRC_FILE_DEF tat_TAT_Test_test_SrcFiles[] = {
	{"D:\\Projects\\TAT\\test\\test0004\\mpr\\test.mpr", 1, 5, 8 }};
TAT_TEST_RESULT TAT_TI_TAT_Test_test( TAT_TEST_INSTANCE* id )
{
	id->varDefs = NULL;
	id->timeLabels = NULL;
	id->srcFiles = (TAT_SRC_FILE_DEF*)&tat_TAT_Test_test_SrcFiles;
	id->timeLabelCount = 0;
	id->srcFilesCount = 1;
	id->varCount = 0;
	id->branchCount = 0;
	id->repeatCount = 1;
	id->timerCount  = 0;
	id->loopCount   = 0;
	return TAT_TEST_CONTINUE;
}
static TAT_INSTANCE_ID tat_I_TAT_Test_test_N0[]={TAT_I_Test_ID,TAT_I_IUT_ID};
static TAT_INSTANCE_ID tat_I_TAT_Test_test_N1[]={TAT_I_IUT_ID,TAT_I_Test_ID};
static TAT_SRC_REF_STRUCT tat_TAT_Test_test_0_SrcRef[] = {{0, 5}};
static TAT_SIG_A tat_TAT_Test_test_0_sigdata;
static TAT_EVENT_INSTANCE tat_TAT_Test_test_0_event = {TAT_SEND_EV, TAT_S_A_ID, &tat_TAT_Test_test_0_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_TAT_Test_test_N0, "", 1, (TAT_SRC_REF_STRUCT *) tat_TAT_Test_test_0_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_TAT_Test_test_MessageTable[0],
NULL};

static TAT_SRC_REF_STRUCT tat_TAT_Test_test_1_SrcRef[] = {{0, 7}};
static TAT_SIG_B tat_TAT_Test_test_1_sigdata;
static TAT_EVENT_INSTANCE tat_TAT_Test_test_1_event = {TAT_RECV_EV, TAT_S_B_ID, &tat_TAT_Test_test_1_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_TAT_Test_test_N1, "", 1, (TAT_SRC_REF_STRUCT *) tat_TAT_Test_test_1_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_TAT_Test_test_MessageTable[1],
NULL};

static TAT_TEST_RESULT tat_test_state0(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_TAT_Test_test_0_event, sizeof(__ev));
			if (TAT_Send_A(id, &__ev, &tat_TAT_Test_test_0_event, TAT_Copy_CharString("Hello"), TAT_Copy_CharString("World"))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=1;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_test_state1(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_B_ID)&&
				((!TAT_Compare_CharString(((TAT_SIG_B *)_ev->eventData)->Param1,"World")))&&
				((!TAT_Compare_CharString(((TAT_SIG_B *)_ev->eventData)->Param2,"Hello")))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_TAT_Test_test_1_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				TAT_Free_CharString(&(((TAT_SIG_B *)_ev->eventData)->Param1));
				TAT_Free_CharString(&(((TAT_SIG_B *)_ev->eventData)->Param2));
				id->popQueue(id);
				id->nextState=-1;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
TAT_TEST_RESULT TAT_T_TAT_Test_test( TAT_TEST_INSTANCE* id )
{
	static const TAT_STATE_FUNC states[2]={
	tat_test_state0,tat_test_state1};
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
