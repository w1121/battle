#include "TAT_Test.h"

static TAT_INSTANCE_DEF tat_InstanceDefinition[]={
{TAT_I_IUT_ID, "IUT", 100},
{TAT_I_Test_ID, "Test", 100}
};



int TAT_Dump_ComplexData(void *data, char *dest) {
	int i,j;
	strcat(dest,"(. ");
	strcat(dest,"(: ");
	for(j=0;j<((ComplexData *)data)->mIntPtr.counter;j++) {
		TAT_Dump_int(&(((ComplexData *)data)->mIntPtr.data[j]),dest);
		if(j<((ComplexData *)data)->mIntPtr.counter-1) strcat(dest,", ");
	}
	strcat(dest," :)");
	strcat(dest,", ");
	strcat(dest,"(: ");
	for(j=0;j<2;j++) {
		TAT_Dump_int(&(((ComplexData *)data)->mIntArr[j]),dest);
		if(j<1) strcat(dest,", ");
	}
	strcat(dest," :)");
	strcat(dest,", ");
	TAT_Dump_int(&(((ComplexData *)data)->mInt),dest);
	strcat(dest," .)");
	return 1;
}

int TAT_Free_ComplexData(ComplexData * x) {
	int i,j;
	free(x->mIntPtr.data);
	memset(x,0,sizeof(ComplexData));
	return 1;
}

ComplexData TAT_Copy_ComplexData(ComplexData src) {
	int i,j;
	ComplexData x;
	x.mIntPtr.counter=src.mIntPtr.counter;
	if(x.mIntPtr.counter>0)
		x.mIntPtr.data=(int*)malloc(x.mIntPtr.counter*sizeof(int));
	else
		x.mIntPtr.data=0;
	for(j=0;j<x.mIntPtr.counter;j++) {
		x.mIntPtr.data[j]=src.mIntPtr.data[j];
	}
	for(j=0;j<2;j++) {
		x.mIntArr[j]=src.mIntArr[j];
	}
	x.mInt=src.mInt;
	return x;
}

int TAT_Compare_ComplexData(ComplexData x1, ComplexData x2) {
	int i,j;
	if(x1.mIntPtr.counter!=x2.mIntPtr.counter) return 1;
	for(j=0;j<x1.mIntPtr.counter;j++) {
		if(x1.mIntPtr.data[j]!=x2.mIntPtr.data[j]) return 1;
	}
	for(j=0;j<2;j++) {
		if(x1.mIntArr[j]!=x2.mIntArr[j]) return 1;
	}
	if(x1.mInt!=x2.mInt) return 1;
	return 0;
}

static TAT_SIG_PARAM_DEF tat_SIG_PARAM_A[]={
	{0,"Param1",sizeof(ComplexData), offsetof(TAT_SIG_A, Param1), TAT_Dump_ComplexData}
};
TAT_TEST_RESULT TAT_Send_A ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, ComplexData Param1)
{
	((TAT_SIG_A *)ev->eventData)->Param1=Param1;
	ev->eventID=TAT_S_A_ID;
	if(id->dumpEvent(id,ev,evSrc)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
	return (id->testResult=id->sendMessage(id, ev));
}
static TAT_SIG_PARAM_DEF tat_SIG_PARAM_B[]={
	{1,"Param1",sizeof(ComplexData), offsetof(TAT_SIG_B, Param1), TAT_Dump_ComplexData}
};
static TAT_SIGNAL_DEF tat_SignalDefinition[]={
	{              TAT_S_A_ID,	"A", sizeof(TAT_SIG_A), 1, (TAT_SIG_PARAM_DEF *)&tat_SIG_PARAM_A, 100},
	{              TAT_S_B_ID,	"B", sizeof(TAT_SIG_B), 1, (TAT_SIG_PARAM_DEF *)&tat_SIG_PARAM_B, 100}
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
