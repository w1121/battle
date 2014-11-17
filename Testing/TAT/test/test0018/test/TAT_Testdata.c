#include "TAT_Test.h"

static TAT_INSTANCE_DEF tat_InstanceDefinition[]={
{TAT_I_IUT_ID, "IUT", 100},
{TAT_I_Test_ID, "Test", 100}
};


ComplexData TAT_ComplexData_Cur;

int TAT_Dump_ComplexData(void *data, char *dest) {
	int i,j;
	ComplexData cur=*(ComplexData *)data;
	strcat(dest,"(: ");
	while(cur) {
		strcat(dest,"(. ");
		strcat(dest,"(: ");
		for(j=0;j<cur->data.mStrPtr.counter;j++) {
			TAT_Dump_String(&(cur->data.mStrPtr.data[j]),dest);
			if(j<cur->data.mStrPtr.counter-1) strcat(dest,", ");
		}
		strcat(dest," :)");
		strcat(dest,", ");
		strcat(dest,"(: ");
		for(j=0;j<2;j++) {
			TAT_Dump_String(&(cur->data.mStrArr[j]),dest);
			if(j<1) strcat(dest,", ");
		}
		strcat(dest," :)");
		strcat(dest,", ");
		TAT_Dump_String(&(cur->data.mStr),dest);
		strcat(dest," .)");
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
		for(j=0;j<cur->data.mStrPtr.counter;j++) {
			free(cur->data.mStrPtr.data[j]);
		}
		free(cur->data.mStrPtr.data);
		for(j=0;j<2;j++) {
			free(cur->data.mStrArr[j]);
		}
		free(cur->data.mStr);
		prev=cur;
		cur=cur->next;
		free(prev);
	}
	memset(x,0,sizeof(ComplexData));
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
		cur->data.mStrPtr.counter=scur->data.mStrPtr.counter;
		if(cur->data.mStrPtr.counter>0)
			cur->data.mStrPtr.data=(char**)malloc(cur->data.mStrPtr.counter*sizeof(char*));
		else
			cur->data.mStrPtr.data=0;
		for(j=0;j<cur->data.mStrPtr.counter;j++) {
			cur->data.mStrPtr.data[j]=(char*)malloc(strlen(scur->data.mStrPtr.data[j])+1);
			strcpy(cur->data.mStrPtr.data[j],scur->data.mStrPtr.data[j]);
		}
		for(j=0;j<2;j++) {
			cur->data.mStrArr[j]=(char*)malloc(strlen(scur->data.mStrArr[j])+1);
			strcpy(cur->data.mStrArr[j],scur->data.mStrArr[j]);
		}
		cur->data.mStr=(char*)malloc(strlen(scur->data.mStr)+1);
		strcpy(cur->data.mStr,scur->data.mStr);
		scur=scur->next;
		if(scur)
			cur->next=(ComplexData)malloc(sizeof(struct _ComplexData));
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
		if(cur1->data.mStrPtr.counter!=cur2->data.mStrPtr.counter) return 1;
		for(j=0;j<cur1->data.mStrPtr.counter;j++) {
			if(strcmp(cur1->data.mStrPtr.data[j],cur2->data.mStrPtr.data[j])) return 1;
		}
		for(j=0;j<2;j++) {
			if(strcmp(cur1->data.mStrArr[j],cur2->data.mStrArr[j])) return 1;
		}
		if(strcmp(cur1->data.mStr,cur2->data.mStr)) return 1;
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
