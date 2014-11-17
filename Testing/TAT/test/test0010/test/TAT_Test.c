#include "TAT_Test.h"

static char * tat_TAT_Test_test_MessageTable[] = {
	"A((:(.(:\"qwe\",\"rty\":),(:\"ui\",\"op\":),\"123\".),(.(:\"asd\",\"fgh\":),(:\"jk\",\"l;\":),\"456\".):))",
	"B(&x:{(:(.(:\"qwe\",\"rty\":),(:\"ui\",\"op\":),\"123\".),(.(:\"asd\",\"fgh\":),(:\"jk\",\"l;\":),\"456\".):)})",
	"A(x)",
	"B(x)"
};
static TAT_VARIABLE_DEF  tat_TAT_Test_test_Vardef[] = {
	{1, "x", sizeof(ComplexData), TAT_Dump_ComplexData}};
#define var_test_x (*(ComplexData*)id->vars[0])
static TAT_SRC_FILE_DEF tat_TAT_Test_test_SrcFiles[] = {
	{"D:\\Projects\\TAT\\test\\test0010\\mpr\\test.mpr", 1, 6, 13 }};
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
	x.data[0].mStrPtr.data=(char**)malloc(2*sizeof(char*));
	x.data[0].mStrPtr.counter=2;
	x.data[0].mStrPtr.data[0]=(char*)malloc(strlen("qwe")+1);
	strcpy(x.data[0].mStrPtr.data[0],"qwe");
	x.data[0].mStrPtr.data[1]=(char*)malloc(strlen("rty")+1);
	strcpy(x.data[0].mStrPtr.data[1],"rty");
	x.data[0].mStrArr[0]=(char*)malloc(strlen("ui")+1);
	strcpy(x.data[0].mStrArr[0],"ui");
	x.data[0].mStrArr[1]=(char*)malloc(strlen("op")+1);
	strcpy(x.data[0].mStrArr[1],"op");
	x.data[0].mStr=(char*)malloc(strlen("123")+1);
	strcpy(x.data[0].mStr,"123");
	x.data[1].mStrPtr.data=(char**)malloc(2*sizeof(char*));
	x.data[1].mStrPtr.counter=2;
	x.data[1].mStrPtr.data[0]=(char*)malloc(strlen("asd")+1);
	strcpy(x.data[1].mStrPtr.data[0],"asd");
	x.data[1].mStrPtr.data[1]=(char*)malloc(strlen("fgh")+1);
	strcpy(x.data[1].mStrPtr.data[1],"fgh");
	x.data[1].mStrArr[0]=(char*)malloc(strlen("jk")+1);
	strcpy(x.data[1].mStrArr[0],"jk");
	x.data[1].mStrArr[1]=(char*)malloc(strlen("l;")+1);
	strcpy(x.data[1].mStrArr[1],"l;");
	x.data[1].mStr=(char*)malloc(strlen("456")+1);
	strcpy(x.data[1].mStr,"456");
	return x;
}
int TAT_Cmp_test_1_0_1 (ComplexData x, TAT_TEST_INSTANCE *id) {
	if(x.data[0].mStrPtr.counter!=2) return 1;
	if(strcmp(x.data[0].mStrPtr.data[0],"qwe")) return 1;
	if(strcmp(x.data[0].mStrPtr.data[1],"rty")) return 1;
	if(strcmp(x.data[0].mStrArr[0],"ui")) return 1;
	if(strcmp(x.data[0].mStrArr[1],"op")) return 1;
	if(strcmp(x.data[0].mStr,"123")) return 1;
	if(x.data[1].mStrPtr.counter!=2) return 1;
	if(strcmp(x.data[1].mStrPtr.data[0],"asd")) return 1;
	if(strcmp(x.data[1].mStrPtr.data[1],"fgh")) return 1;
	if(strcmp(x.data[1].mStrArr[0],"jk")) return 1;
	if(strcmp(x.data[1].mStrArr[1],"l;")) return 1;
	if(strcmp(x.data[1].mStr,"456")) return 1;
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
