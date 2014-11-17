#include "TAT_Test.h"

static TAT_INSTANCE_DEF tat_InstanceDefinition[]={
{TAT_I_IUT_ID, "IUT", 100},
{TAT_I_Test_ID, "Test", 100}
};



int TAT_Dump_Struct1(void *data, char *dest) {
	int i,j;
	strcat(dest,"(. ");
	TAT_Dump_int(&(((Struct1 *)data)->a),dest);
	strcat(dest,", ");
	TAT_Dump_int(&(((Struct1 *)data)->b),dest);
	strcat(dest," .)");
	return 1;
}

int TAT_Dump_Struct2(void *data, char *dest) {
	int i,j;
	strcat(dest,"(. ");
	TAT_Dump_Struct1(&(((Struct2 *)data)->a),dest);
	strcat(dest,", ");
	TAT_Dump_Struct1(&(((Struct2 *)data)->b),dest);
	strcat(dest," .)");
	return 1;
}

int TAT_Dump_Struct3(void *data, char *dest) {
	int i,j;
	strcat(dest,"(. ");
	TAT_Dump_Struct2(&(((Struct3 *)data)->a),dest);
	strcat(dest,", ");
	TAT_Dump_Struct2(&(((Struct3 *)data)->b),dest);
	strcat(dest," .)");
	return 1;
}

int TAT_Free_Struct1(Struct1 * x) {
	int i,j;
	memset(x,0,sizeof(Struct1));
	return 1;
}

int TAT_Free_Struct2(Struct2 * x) {
	int i,j;
	TAT_Free_Struct1(&(x->a));
	TAT_Free_Struct1(&(x->b));
	memset(x,0,sizeof(Struct2));
	return 1;
}

int TAT_Free_Struct3(Struct3 * x) {
	int i,j;
	TAT_Free_Struct2(&(x->a));
	TAT_Free_Struct2(&(x->b));
	memset(x,0,sizeof(Struct3));
	return 1;
}

Struct1 TAT_Copy_Struct1(Struct1 src) {
	int i,j;
	Struct1 x;
	x.a=src.a;
	x.b=src.b;
	return x;
}

Struct2 TAT_Copy_Struct2(Struct2 src) {
	int i,j;
	Struct2 x;
	x.a=TAT_Copy_Struct1(src.a);
	x.b=TAT_Copy_Struct1(src.b);
	return x;
}

Struct3 TAT_Copy_Struct3(Struct3 src) {
	int i,j;
	Struct3 x;
	x.a=TAT_Copy_Struct2(src.a);
	x.b=TAT_Copy_Struct2(src.b);
	return x;
}

int TAT_Compare_Struct1(Struct1 x1, Struct1 x2) {
	int i,j;
	if(x1.a!=x2.a) return 1;
	if(x1.b!=x2.b) return 1;
	return 0;
}

int TAT_Compare_Struct2(Struct2 x1, Struct2 x2) {
	int i,j;
	if(TAT_Compare_Struct1(x1.a,x2.a)) return 1;
	if(TAT_Compare_Struct1(x1.b,x2.b)) return 1;
	return 0;
}

int TAT_Compare_Struct3(Struct3 x1, Struct3 x2) {
	int i,j;
	if(TAT_Compare_Struct2(x1.a,x2.a)) return 1;
	if(TAT_Compare_Struct2(x1.b,x2.b)) return 1;
	return 0;
}

static TAT_SIG_PARAM_DEF tat_SIG_PARAM_A[]={
	{0,"Param1",sizeof(Struct3), offsetof(TAT_SIG_A, Param1), TAT_Dump_Struct3}
};
TAT_TEST_RESULT TAT_Send_A ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, Struct3 Param1)
{
	((TAT_SIG_A *)ev->eventData)->Param1=Param1;
	ev->eventID=TAT_S_A_ID;
	if(id->dumpEvent(id,ev,evSrc)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
	return (id->testResult=id->sendMessage(id, ev));
}
static TAT_SIG_PARAM_DEF tat_SIG_PARAM_B[]={
	{1,"Param1",sizeof(Struct3), offsetof(TAT_SIG_B, Param1), TAT_Dump_Struct3}
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
