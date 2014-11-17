#include "TAT_Test.h"

static TAT_INSTANCE_DEF tat_InstanceDefinition[]={
{TAT_I_IUT_ID, "IUT", 100},
{TAT_I_Test_ID, "Test", 100}
};



int TAT_Dump_ComplexData(void *data, char *dest) {
	int i,j;
	strcat(dest,"(. ");
	strcat(dest,"(: ");
	for(j=0;j<((ComplexData *)data)->mStrPtr.counter;j++) {
		TAT_Dump_String(&(((ComplexData *)data)->mStrPtr.data[j]),dest);
		if(j<((ComplexData *)data)->mStrPtr.counter-1) strcat(dest,", ");
	}
	strcat(dest," :)");
	strcat(dest,", ");
	strcat(dest,"(: ");
	for(j=0;j<2;j++) {
		TAT_Dump_String(&(((ComplexData *)data)->mStrArr[j]),dest);
		if(j<1) strcat(dest,", ");
	}
	strcat(dest," :)");
	strcat(dest,", ");
	TAT_Dump_String(&(((ComplexData *)data)->mStr),dest);
	strcat(dest," .)");
	return 1;
}

int TAT_Free_ComplexData(ComplexData * x) {
	int i,j;
	for(j=0;j<x->mStrPtr.counter;j++) {
		free(x->mStrPtr.data[j]);
	}
	free(x->mStrPtr.data);
	for(j=0;j<2;j++) {
		free(x->mStrArr[j]);
	}
	free(x->mStr);
	memset(x,0,sizeof(ComplexData));
	return 1;
}

ComplexData TAT_Copy_ComplexData(ComplexData src) {
	int i,j;
	ComplexData x;
	x.mStrPtr.counter=src.mStrPtr.counter;
	if(x.mStrPtr.counter>0)
		x.mStrPtr.data=(char**)malloc(x.mStrPtr.counter*sizeof(char*));
	else
		x.mStrPtr.data=0;
	for(j=0;j<x.mStrPtr.counter;j++) {
		x.mStrPtr.data[j]=(char*)malloc(strlen(src.mStrPtr.data[j])+1);
		strcpy(x.mStrPtr.data[j],src.mStrPtr.data[j]);
	}
	for(j=0;j<2;j++) {
		x.mStrArr[j]=(char*)malloc(strlen(src.mStrArr[j])+1);
		strcpy(x.mStrArr[j],src.mStrArr[j]);
	}
	x.mStr=(char*)malloc(strlen(src.mStr)+1);
	strcpy(x.mStr,src.mStr);
	return x;
}

int TAT_Compare_ComplexData(ComplexData x1, ComplexData x2) {
	int i,j;
	if(x1.mStrPtr.counter!=x2.mStrPtr.counter) return 1;
	for(j=0;j<x1.mStrPtr.counter;j++) {
		if(strcmp(x1.mStrPtr.data[j],x2.mStrPtr.data[j])) return 1;
	}
	for(j=0;j<2;j++) {
		if(strcmp(x1.mStrArr[j],x2.mStrArr[j])) return 1;
	}
	if(strcmp(x1.mStr,x2.mStr)) return 1;
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
