#include "TAT_Test.h"

static TAT_INSTANCE_DEF tat_InstanceDefinition[]={
{TAT_I_IUT_ID, "IUT", 100},
{TAT_I_Test_ID, "Test", 100}
};



int TAT_Dump_Array1(void *data, char *dest) {
	int i,j;
	strcat(dest,"(: ");
	for(i=0;i<2;i++) {
		TAT_Dump_int(&(((Array1 *)data)->data[i]),dest);
		if(i<1) strcat(dest,", ");
	}
	strcat(dest," :)");
	return 1;
}

int TAT_Dump_Array2(void *data, char *dest) {
	int i,j;
	strcat(dest,"(: ");
	for(i=0;i<2;i++) {
		TAT_Dump_Array1(&(((Array2 *)data)->data[i]),dest);
		if(i<1) strcat(dest,", ");
	}
	strcat(dest," :)");
	return 1;
}

int TAT_Dump_Array3(void *data, char *dest) {
	int i,j;
	strcat(dest,"(: ");
	for(i=0;i<2;i++) {
		TAT_Dump_Array2(&(((Array3 *)data)->data[i]),dest);
		if(i<1) strcat(dest,", ");
	}
	strcat(dest," :)");
	return 1;
}

int TAT_Free_Array1(Array1 * x) {
	int i,j;
	for(i=0;i<2;i++) {
	}
	memset(x,0,sizeof(Array1));
	return 1;
}

int TAT_Free_Array2(Array2 * x) {
	int i,j;
	for(i=0;i<2;i++) {
		TAT_Free_Array1(&(x->data[i]));
	}
	memset(x,0,sizeof(Array2));
	return 1;
}

int TAT_Free_Array3(Array3 * x) {
	int i,j;
	for(i=0;i<2;i++) {
		TAT_Free_Array2(&(x->data[i]));
	}
	memset(x,0,sizeof(Array3));
	return 1;
}

Array1 TAT_Copy_Array1(Array1 src) {
	int i,j;
	Array1 x;
	for(i=0;i<2;i++) {
		x.data[i]=src.data[i];
	}
	return x;
}

Array2 TAT_Copy_Array2(Array2 src) {
	int i,j;
	Array2 x;
	for(i=0;i<2;i++) {
		x.data[i]=TAT_Copy_Array1(src.data[i]);
	}
	return x;
}

Array3 TAT_Copy_Array3(Array3 src) {
	int i,j;
	Array3 x;
	for(i=0;i<2;i++) {
		x.data[i]=TAT_Copy_Array2(src.data[i]);
	}
	return x;
}

int TAT_Compare_Array1(Array1 x1, Array1 x2) {
	int i,j;
	for(i=0;i<2;i++) {
		if(x1.data[i]!=x2.data[i]) return 1;
	}
	return 0;
}

int TAT_Compare_Array2(Array2 x1, Array2 x2) {
	int i,j;
	for(i=0;i<2;i++) {
		if(TAT_Compare_Array1(x1.data[i],x2.data[i])) return 1;
	}
	return 0;
}

int TAT_Compare_Array3(Array3 x1, Array3 x2) {
	int i,j;
	for(i=0;i<2;i++) {
		if(TAT_Compare_Array2(x1.data[i],x2.data[i])) return 1;
	}
	return 0;
}

static TAT_SIG_PARAM_DEF tat_SIG_PARAM_A[]={
	{0,"Param1",sizeof(Array3), offsetof(TAT_SIG_A, Param1), TAT_Dump_Array3}
};
TAT_TEST_RESULT TAT_Send_A ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, Array3 Param1)
{
	((TAT_SIG_A *)ev->eventData)->Param1=Param1;
	ev->eventID=TAT_S_A_ID;
	if(id->dumpEvent(id,ev,evSrc)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
	return (id->testResult=id->sendMessage(id, ev));
}
static TAT_SIG_PARAM_DEF tat_SIG_PARAM_B[]={
	{1,"Param1",sizeof(Array3), offsetof(TAT_SIG_B, Param1), TAT_Dump_Array3}
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
