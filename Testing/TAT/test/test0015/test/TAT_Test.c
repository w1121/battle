#include "TAT_Test.h"

static char * tat_TAT_Test_test_MessageTable[] = {
	"A((:1,2,3,4,5,6,7:))",
	"B(&x:{(:1,2,3,4,5,6,7:)})",
	"A(x)",
	"B(x)"
};
static TAT_VARIABLE_DEF  tat_TAT_Test_test_Vardef[] = {
	{1, "x", sizeof(ComplexData), TAT_Dump_ComplexData}};
#define var_test_x (*(ComplexData*)id->vars[0])
static TAT_SRC_FILE_DEF tat_TAT_Test_test_SrcFiles[] = {
	{"D:\\Projects\\TAT\\test\\test0015\\mpr\\test.mpr", 1, 6, 13 }};
TAT_TEST_RESULT TAT_TI_TAT_Test_test( TAT_TEST_INSTANCE* id )
{
	id->varDefs = (TAT_VARIABLE_DEF*)&tat_TAT_Test_test_Vardef;
	id->timeLabels = NULL;
	id->srcFiles = (TAT_SRC_FILE_DEF*)&tat_TAT_Test_test_SrcFiles;
	id->timeLabelCount = 0;
	id->srcFilesCount = 1;
	id->varCount = 1;
	id->branchCount = 0;
	id->repeatCount = 1;
	id->timerCount  = 0;
	id->loopCount   = 0;
	return TAT_TEST_CONTINUE;
}
static TAT_INSTANCE_ID tat_I_TAT_Test_test_N0[]={TAT_I_Test_ID,TAT_I_IUT_ID};
static TAT_INSTANCE_ID tat_I_TAT_Test_test_N1[]={TAT_I_IUT_ID,TAT_I_Test_ID};
static TAT_INSTANCE_ID tat_I_TAT_Test_test_N2[]={TAT_I_Test_ID,TAT_I_IUT_ID};
static TAT_INSTANCE_ID tat_I_TAT_Test_test_N3[]={TAT_I_IUT_ID,TAT_I_Test_ID};
static TAT_SRC_REF_STRUCT tat_TAT_Test_test_0_SrcRef[] = {{0, 6}};
static TAT_SIG_A tat_TAT_Test_test_0_sigdata;
static TAT_EVENT_INSTANCE tat_TAT_Test_test_0_event = {TAT_SEND_EV, TAT_S_A_ID, &tat_TAT_Test_test_0_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_TAT_Test_test_N0, "", 1, (TAT_SRC_REF_STRUCT *) tat_TAT_Test_test_0_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_TAT_Test_test_MessageTable[0],
NULL};

static TAT_SRC_REF_STRUCT tat_TAT_Test_test_1_SrcRef[] = {{0, 8}};
static TAT_SIG_B tat_TAT_Test_test_1_sigdata;
static TAT_EVENT_INSTANCE tat_TAT_Test_test_1_event = {TAT_RECV_EV, TAT_S_B_ID, &tat_TAT_Test_test_1_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_TAT_Test_test_N1, "", 1, (TAT_SRC_REF_STRUCT *) tat_TAT_Test_test_1_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_TAT_Test_test_MessageTable[1],
NULL};

static TAT_SRC_REF_STRUCT tat_TAT_Test_test_2_SrcRef[] = {{0, 10}};
static TAT_SIG_A tat_TAT_Test_test_2_sigdata;
static TAT_EVENT_INSTANCE tat_TAT_Test_test_2_event = {TAT_SEND_EV, TAT_S_A_ID, &tat_TAT_Test_test_2_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_TAT_Test_test_N2, "", 1, (TAT_SRC_REF_STRUCT *) tat_TAT_Test_test_2_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_TAT_Test_test_MessageTable[2],
NULL};

static TAT_SRC_REF_STRUCT tat_TAT_Test_test_3_SrcRef[] = {{0, 12}};
static TAT_SIG_B tat_TAT_Test_test_3_sigdata;
static TAT_EVENT_INSTANCE tat_TAT_Test_test_3_event = {TAT_RECV_EV, TAT_S_B_ID, &tat_TAT_Test_test_3_sigdata, {{0,0}, {0,0}, ""},2, 
(TAT_INSTANCE_ID *) &tat_I_TAT_Test_test_N3, "", 1, (TAT_SRC_REF_STRUCT *) tat_TAT_Test_test_3_SrcRef, 
TAT_VERDICT_PASS, -1, &tat_TAT_Test_test_MessageTable[3],
NULL};

ComplexData TAT_Ass_test_0_0 (TAT_TEST_INSTANCE *id) {
	ComplexData x;
	x=(ComplexData)malloc(sizeof(struct _ComplexData));
	TAT_ComplexData_Cur=x;
	TAT_ComplexData_Cur->next=0;
	TAT_ComplexData_Cur->data=1;
	TAT_ComplexData_Cur->next=(ComplexData)malloc(sizeof(struct _ComplexData));
	TAT_ComplexData_Cur=TAT_ComplexData_Cur->next;
	TAT_ComplexData_Cur->next=0;
	TAT_ComplexData_Cur->data=2;
	TAT_ComplexData_Cur->next=(ComplexData)malloc(sizeof(struct _ComplexData));
	TAT_ComplexData_Cur=TAT_ComplexData_Cur->next;
	TAT_ComplexData_Cur->next=0;
	TAT_ComplexData_Cur->data=3;
	TAT_ComplexData_Cur->next=(ComplexData)malloc(sizeof(struct _ComplexData));
	TAT_ComplexData_Cur=TAT_ComplexData_Cur->next;
	TAT_ComplexData_Cur->next=0;
	TAT_ComplexData_Cur->data=4;
	TAT_ComplexData_Cur->next=(ComplexData)malloc(sizeof(struct _ComplexData));
	TAT_ComplexData_Cur=TAT_ComplexData_Cur->next;
	TAT_ComplexData_Cur->next=0;
	TAT_ComplexData_Cur->data=5;
	TAT_ComplexData_Cur->next=(ComplexData)malloc(sizeof(struct _ComplexData));
	TAT_ComplexData_Cur=TAT_ComplexData_Cur->next;
	TAT_ComplexData_Cur->next=0;
	TAT_ComplexData_Cur->data=6;
	TAT_ComplexData_Cur->next=(ComplexData)malloc(sizeof(struct _ComplexData));
	TAT_ComplexData_Cur=TAT_ComplexData_Cur->next;
	TAT_ComplexData_Cur->next=0;
	TAT_ComplexData_Cur->data=7;
	return x;
}
int TAT_Cmp_test_1_0_1 (ComplexData x, TAT_TEST_INSTANCE *id) {
	TAT_ComplexData_Cur=x;
	if(TAT_ComplexData_Cur==0) return 1;
	if(TAT_ComplexData_Cur->data!=1) return 1;
	TAT_ComplexData_Cur=TAT_ComplexData_Cur->next;
	if(TAT_ComplexData_Cur==0) return 1;
	if(TAT_ComplexData_Cur->data!=2) return 1;
	TAT_ComplexData_Cur=TAT_ComplexData_Cur->next;
	if(TAT_ComplexData_Cur==0) return 1;
	if(TAT_ComplexData_Cur->data!=3) return 1;
	TAT_ComplexData_Cur=TAT_ComplexData_Cur->next;
	if(TAT_ComplexData_Cur==0) return 1;
	if(TAT_ComplexData_Cur->data!=4) return 1;
	TAT_ComplexData_Cur=TAT_ComplexData_Cur->next;
	if(TAT_ComplexData_Cur==0) return 1;
	if(TAT_ComplexData_Cur->data!=5) return 1;
	TAT_ComplexData_Cur=TAT_ComplexData_Cur->next;
	if(TAT_ComplexData_Cur==0) return 1;
	if(TAT_ComplexData_Cur->data!=6) return 1;
	TAT_ComplexData_Cur=TAT_ComplexData_Cur->next;
	if(TAT_ComplexData_Cur==0) return 1;
	if(TAT_ComplexData_Cur->data!=7) return 1;
	TAT_ComplexData_Cur=TAT_ComplexData_Cur->next;
	if(TAT_ComplexData_Cur!=0) return 1;
	return 0;
}
static TAT_TEST_RESULT tat_test_state0(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_TAT_Test_test_0_event, sizeof(__ev));
			if (TAT_Send_A(id, &__ev, &tat_TAT_Test_test_0_event, TAT_Ass_test_0_0(id))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
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
				((TAT_Cmp_test_1_0_1(((TAT_SIG_B *)_ev->eventData)->Param1,id) == 0))){
				id->lastEvent = id->clock;
				TAT_Free_ComplexData(&(var_test_x));
				var_test_x=TAT_Copy_ComplexData(((TAT_SIG_B *)_ev->eventData)->Param1);
				if (id->dumpEvent(id, _ev, &tat_TAT_Test_test_1_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				TAT_Free_ComplexData(&(((TAT_SIG_B *)_ev->eventData)->Param1));
				id->popQueue(id);
				id->nextState=2;
				return TAT_TEST_CONTINUE;
			}
		}
	};
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_test_state2(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE __ev;
		if ((id->clock-id->lastEvent>=0)) {
			memcpy(&__ev, &tat_TAT_Test_test_2_event, sizeof(__ev));
			if (TAT_Send_A(id, &__ev, &tat_TAT_Test_test_2_event, TAT_Copy_ComplexData(var_test_x))!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				id->lastEvent = id->clock;
			id->nextState=3;
		}
	return TAT_TEST_FINISHED;
}
static TAT_TEST_RESULT tat_test_state3(TAT_TEST_INSTANCE *id)
{
	TAT_EVENT_INSTANCE *_ev;
	if (id->queue.length>0) {
		_ev = id->getQueue(id);
		if ((_ev->eventTime.timeCalc.minTime-id->lastEvent>=0)) {
			if ((_ev->eventID == TAT_S_B_ID)&&
				((!TAT_Compare_ComplexData(((TAT_SIG_B *)_ev->eventData)->Param1,var_test_x)))){
				id->lastEvent = id->clock;
				if (id->dumpEvent(id, _ev, &tat_TAT_Test_test_3_event)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
				TAT_Free_ComplexData(&(((TAT_SIG_B *)_ev->eventData)->Param1));
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
	static const TAT_STATE_FUNC states[4]={
	tat_test_state0,tat_test_state1,tat_test_state2,tat_test_state3};
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
