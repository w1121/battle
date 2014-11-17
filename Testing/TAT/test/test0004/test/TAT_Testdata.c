#include "TAT_Test.h"

static TAT_INSTANCE_DEF tat_InstanceDefinition[]={
{TAT_I_IUT_ID, "IUT", 100},
{TAT_I_Test_ID, "Test", 100}
};



int TAT_Dump_CharString(void *data, char *dest) {
	int i,j;
	TAT_Dump_String(data,dest);
	return 1;
}

int TAT_Free_CharString(CharString * x) {
	int i,j;
	free(*x);
	memset(x,0,sizeof(CharString));
	return 1;
}

CharString TAT_Copy_CharString(CharString src) {
	int i,j;
	CharString x;
	x=(char*)malloc(strlen(src)+1);
	strcpy(x,src);
	return x;
}

int TAT_Compare_CharString(CharString x1, CharString x2) {
	int i,j;
	if(strcmp(x1,x2)) return 1;
	return 0;
}

static TAT_SIG_PARAM_DEF tat_SIG_PARAM_A[]={
	{0,"Param1",sizeof(CharString), offsetof(TAT_SIG_A, Param1), TAT_Dump_CharString},
	{1,"Param2",sizeof(CharString), offsetof(TAT_SIG_A, Param2), TAT_Dump_CharString}
};
TAT_TEST_RESULT TAT_Send_A ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, CharString Param1, CharString Param2)
{
	((TAT_SIG_A *)ev->eventData)->Param1=Param1;
	((TAT_SIG_A *)ev->eventData)->Param2=Param2;
	ev->eventID=TAT_S_A_ID;
	if(id->dumpEvent(id,ev,evSrc)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
	return (id->testResult=id->sendMessage(id, ev));
}
static TAT_SIG_PARAM_DEF tat_SIG_PARAM_B[]={
	{2,"Param1",sizeof(CharString), offsetof(TAT_SIG_B, Param1), TAT_Dump_CharString},
	{3,"Param2",sizeof(CharString), offsetof(TAT_SIG_B, Param2), TAT_Dump_CharString}
};
static TAT_SIGNAL_DEF tat_SignalDefinition[]={
	{              TAT_S_A_ID,	"A", sizeof(TAT_SIG_A), 2, (TAT_SIG_PARAM_DEF *)&tat_SIG_PARAM_A, 100},
	{              TAT_S_B_ID,	"B", sizeof(TAT_SIG_B), 2, (TAT_SIG_PARAM_DEF *)&tat_SIG_PARAM_B, 100}
};


static int _ssort(const void *x1, const void *x2)
{
	return (( TAT_SIGNAL_DEF * ) x1 )->signalID - (( TAT_SIGNAL_DEF * ) x2 )->signalID;
}
static int _isort(const void *x1, const void *x2)
{
	return (( TAT_INSTANCE_DEF * ) x1 )->instanceID - (( TAT_INSTANCE_DEF * ) x2 )->instanceID;
}
TAT_TEST_RESULT TAT_Init_TAT_Test (TAT_TEST_INSTANCE * id)
{
	if (!id) return TAT_TEST_CRITICAL;
	id->signalCount = 2;
	id->signalDef = (TAT_SIGNAL_DEF *)&tat_SignalDefinition;
	id->instanceCount = 2;
	id->instanceDef = (TAT_INSTANCE_DEF *)&tat_InstanceDefinition;
	qsort(id->signalDef, id->signalCount, sizeof(TAT_SIGNAL_DEF), _ssort);
	qsort(id->instanceDef, id->instanceCount, sizeof(TAT_INSTANCE_DEF), _isort);
	id->currSuiteName = "TAT_Test";
	id->testCount = 1;
	id->testTable = (TAT_TEST_TABLE *)&TAT_TAT_Test_TestTable;
	return TAT_TEST_CONTINUE;
}
TAT_TEST_TABLE TAT_TAT_Test_TestTable[]={
{"test", TAT_T_TAT_Test_test, TAT_TI_TAT_Test_test}};
