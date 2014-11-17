#include "TAT_Test.h"

static TAT_INSTANCE_DEF tat_InstanceDefinition[]={
{TAT_I_IUT_ID, "IUT", 100},
{TAT_I_Test_ID, "Test", 100}
};


ComplexData TAT_ComplexData_Cur;
List1 TAT_List1_Cur;
List2 TAT_List2_Cur;

int TAT_Dump_ComplexData(void *data, char *dest) {
	int i,j;
	ComplexData cur=*(ComplexData *)data;
	strcat(dest,"(: ");
	while(cur) {
		TAT_Dump_List2(&(cur->data),dest);
		cur=cur->next;
		if(cur) strcat(dest,", ");
	}
	strcat(dest," :)");
	return 1;
}

int TAT_Dump_List1(void *data, char *dest) {
	int i,j;
	List1 cur=*(List1 *)data;
	strcat(dest,"(: ");
	while(cur) {
		TAT_Dump_int(&(cur->data),dest);
		cur=cur->next;
		if(cur) strcat(dest,", ");
	}
	strcat(dest," :)");
	return 1;
}

int TAT_Dump_List2(void *data, char *dest) {
	int i,j;
	List2 cur=*(List2 *)data;
	strcat(dest,"(: ");
	while(cur) {
		TAT_Dump_List1(&(cur->data),dest);
		cur=cur->next;
		if(cur) strcat(dest,", ");
	}
	strcat(dest," :)");
	return 1;
}

int TAT_Free_ComplexData(ComplexData * x) {
	int i,j;
	ComplexData cur=*x;
	ComplexData prev;
	while(cur) {
		TAT_Free_List2(&(cur->data));
		prev=cur;
		cur=cur->next;
		free(prev);
	}
	memset(x,0,sizeof(ComplexData));
	return 1;
}

int TAT_Free_List1(List1 * x) {
	int i,j;
	List1 cur=*x;
	List1 prev;
	while(cur) {
		prev=cur;
		cur=cur->next;
		free(prev);
	}
	memset(x,0,sizeof(List1));
	return 1;
}

int TAT_Free_List2(List2 * x) {
	int i,j;
	List2 cur=*x;
	List2 prev;
	while(cur) {
		TAT_Free_List1(&(cur->data));
		prev=cur;
		cur=cur->next;
		free(prev);
	}
	memset(x,0,sizeof(List2));
	return 1;
}

ComplexData TAT_Copy_ComplexData(ComplexData src) {
	int i,j;
	ComplexData x;
	ComplexData cur,scur;
	if(src==0) return 0;
	x=(ComplexData)malloc(sizeof(struct _ComplexData));
	cur=x;
	scur=src;
	while(scur) {
		cur->data=TAT_Copy_List2(scur->data);
		scur=scur->next;
		if(scur)
			cur->next=(ComplexData)malloc(sizeof(struct _ComplexData));
		else
			cur->next=0;
		cur=cur->next;
	}
	return x;
}

List1 TAT_Copy_List1(List1 src) {
	int i,j;
	List1 x;
	List1 cur,scur;
	if(src==0) return 0;
	x=(List1)malloc(sizeof(struct _List1));
	cur=x;
	scur=src;
	while(scur) {
		cur->data=scur->data;
		scur=scur->next;
		if(scur)
			cur->next=(List1)malloc(sizeof(struct _List1));
		else
			cur->next=0;
		cur=cur->next;
	}
	return x;
}

List2 TAT_Copy_List2(List2 src) {
	int i,j;
	List2 x;
	List2 cur,scur;
	if(src==0) return 0;
	x=(List2)malloc(sizeof(struct _List2));
	cur=x;
	scur=src;
	while(scur) {
		cur->data=TAT_Copy_List1(scur->data);
		scur=scur->next;
		if(scur)
			cur->next=(List2)malloc(sizeof(struct _List2));
		else
			cur->next=0;
		cur=cur->next;
	}
	return x;
}

int TAT_Compare_ComplexData(ComplexData x1, ComplexData x2) {
	int i,j;
	ComplexData cur1,cur2;
	if(x1==0) return (x2!=0);
	cur1=x1; cur2=x2;
	while(cur1 && cur2) {
		if(TAT_Compare_List2(cur1->data,cur2->data)) return 1;
		cur1=cur1->next;
		cur2=cur2->next;
		if(cur1==0) return (cur2!=0);
	}
	return 0;
}

int TAT_Compare_List1(List1 x1, List1 x2) {
	int i,j;
	List1 cur1,cur2;
	if(x1==0) return (x2!=0);
	cur1=x1; cur2=x2;
	while(cur1 && cur2) {
		if(cur1->data!=cur2->data) return 1;
		cur1=cur1->next;
		cur2=cur2->next;
		if(cur1==0) return (cur2!=0);
	}
	return 0;
}

int TAT_Compare_List2(List2 x1, List2 x2) {
	int i,j;
	List2 cur1,cur2;
	if(x1==0) return (x2!=0);
	cur1=x1; cur2=x2;
	while(cur1 && cur2) {
		if(TAT_Compare_List1(cur1->data,cur2->data)) return 1;
		cur1=cur1->next;
		cur2=cur2->next;
		if(cur1==0) return (cur2!=0);
	}
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
