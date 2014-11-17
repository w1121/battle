#include "battleshipstest.h"

static TAT_INSTANCE_DEF tat_InstanceDefinition[]={
{TAT_I_Simulator_ID, "Simulator", 100},
{TAT_I_Strategy_ID, "Strategy", 100}
};


tStringEnemyShip_yrec * TAT_tStringEnemyShip_Cur;
tStringShipCurrentState_yrec * TAT_tStringShipCurrentState_Cur;
tStringShotsList_yrec * TAT_tStringShotsList_Cur;

int TAT_Dump_tArrayAmountAmmunition(void *data, char *dest) {
	int i;
	strcat(dest,"(: ");
	for(i=0;i<4;i++) {
		TAT_Dump_int(&(((tArrayAmountAmmunition *)data)->A[i]),dest);
		if(i<3) strcat(dest,", ");
	}
	strcat(dest," :)");
	return 1;
}

int TAT_Dump_tArrayAmountLaunchUnit(void *data, char *dest) {
	int i;
	strcat(dest,"(: ");
	for(i=0;i<4;i++) {
		TAT_Dump_int(&(((tArrayAmountLaunchUnit *)data)->A[i]),dest);
		if(i<3) strcat(dest,", ");
	}
	strcat(dest," :)");
	return 1;
}

int TAT_Dump_tArrayShipsCharacter(void *data, char *dest) {
	int i;
	strcat(dest,"(: ");
	for(i=0;i<5;i++) {
		TAT_Dump_tShipCharacter(&(((tArrayShipsCharacter *)data)->A[i]),dest);
		if(i<4) strcat(dest,", ");
	}
	strcat(dest," :)");
	return 1;
}

int TAT_Dump_tArrayWeaponryCharacter(void *data, char *dest) {
	int i;
	strcat(dest,"(: ");
	for(i=0;i<4;i++) {
		TAT_Dump_tWeaponryCharacter(&(((tArrayWeaponryCharacter *)data)->A[i]),dest);
		if(i<3) strcat(dest,", ");
	}
	strcat(dest," :)");
	return 1;
}

int TAT_Dump_tCoordinates(void *data, char *dest) {
	strcat(dest,"(. ");
	TAT_Dump_int(&(((tCoordinates *)data)->x),dest);
	strcat(dest,", ");
	TAT_Dump_int(&(((tCoordinates *)data)->y),dest);
	strcat(dest," .)");
	return 1;
}

int TAT_Dump_tEnemyShip(void *data, char *dest) {
	strcat(dest,"(. ");
	TAT_Dump_int(&(((tEnemyShip *)data)->ShipId),dest);
	strcat(dest,", ");
	TAT_Dump_int(&(((tEnemyShip *)data)->StrategyId),dest);
	strcat(dest,", ");
	TAT_Dump_int(&(((tEnemyShip *)data)->TypeShip),dest);
	strcat(dest,", ");
	TAT_Dump_int(&(((tEnemyShip *)data)->Speed),dest);
	strcat(dest,", ");
	TAT_Dump_int(&(((tEnemyShip *)data)->Heading),dest);
	strcat(dest,", ");
	TAT_Dump_tCoordinates(&(((tEnemyShip *)data)->Coordinates),dest);
	strcat(dest," .)");
	return 1;
}

int TAT_Dump_tShipCharacter(void *data, char *dest) {
	strcat(dest,"(. ");
	TAT_Dump_int(&(((tShipCharacter *)data)->Range),dest);
	strcat(dest,", ");
	TAT_Dump_int(&(((tShipCharacter *)data)->MaxSpeed),dest);
	strcat(dest,", ");
	TAT_Dump_int(&(((tShipCharacter *)data)->MaxAcceleration),dest);
	strcat(dest,", ");
	TAT_Dump_int(&(((tShipCharacter *)data)->MaxResource),dest);
	strcat(dest,", ");
	TAT_Dump_tArrayAmountAmmunition(&(((tShipCharacter *)data)->MaxAmmunition),dest);
	strcat(dest,", ");
	TAT_Dump_tArrayAmountLaunchUnit(&(((tShipCharacter *)data)->MaxLaunchUnit),dest);
	strcat(dest,", ");
	TAT_Dump_int(&(((tShipCharacter *)data)->MaxDeep),dest);
	strcat(dest," .)");
	return 1;
}

int TAT_Dump_tShipCurrentState(void *data, char *dest) {
	strcat(dest,"(. ");
	TAT_Dump_int(&(((tShipCurrentState *)data)->ShipId),dest);
	strcat(dest,", ");
	TAT_Dump_int(&(((tShipCurrentState *)data)->StrategyId),dest);
	strcat(dest,", ");
	TAT_Dump_int(&(((tShipCurrentState *)data)->TypeShip),dest);
	strcat(dest,", ");
	TAT_Dump_int(&(((tShipCurrentState *)data)->Speed),dest);
	strcat(dest,", ");
	TAT_Dump_int(&(((tShipCurrentState *)data)->Heading),dest);
	strcat(dest,", ");
	TAT_Dump_int(&(((tShipCurrentState *)data)->Resource),dest);
	strcat(dest,", ");
	TAT_Dump_tCoordinates(&(((tShipCurrentState *)data)->Coordinates),dest);
	strcat(dest,", ");
	TAT_Dump_tArrayAmountAmmunition(&(((tShipCurrentState *)data)->Ammunition),dest);
	strcat(dest," .)");
	return 1;
}

int TAT_Dump_tStringEnemyShip(void *data, char *dest) {
	tStringEnemyShip_yrec * cur=((tStringEnemyShip *)data)->First;
	strcat(dest,"(: ");
	while(cur) {
		TAT_Dump_tEnemyShip(&(cur->Data),dest);
		cur=cur->Suc;
		if(cur) strcat(dest,", ");
	}
	strcat(dest," :)");
	return 1;
}

int TAT_Dump_tStringShipCurrentState(void *data, char *dest) {
	tStringShipCurrentState_yrec * cur=((tStringShipCurrentState *)data)->First;
	strcat(dest,"(: ");
	while(cur) {
		TAT_Dump_tShipCurrentState(&(cur->Data),dest);
		cur=cur->Suc;
		if(cur) strcat(dest,", ");
	}
	strcat(dest," :)");
	return 1;
}

int TAT_Dump_tStringShotsList(void *data, char *dest) {
	tStringShotsList_yrec * cur=((tStringShotsList *)data)->First;
	strcat(dest,"(: ");
	while(cur) {
		TAT_Dump_tCoordinates(&(cur->Data),dest);
		cur=cur->Suc;
		if(cur) strcat(dest,", ");
	}
	strcat(dest," :)");
	return 1;
}

int TAT_Dump_tWeaponryCharacter(void *data, char *dest) {
	strcat(dest,"(. ");
	TAT_Dump_int(&(((tWeaponryCharacter *)data)->Power),dest);
	strcat(dest,", ");
	TAT_Dump_int(&(((tWeaponryCharacter *)data)->Range),dest);
	strcat(dest,", ");
	TAT_Dump_int(&(((tWeaponryCharacter *)data)->Speed),dest);
	strcat(dest," .)");
	return 1;
}

int TAT_Free_tArrayAmountAmmunition(tArrayAmountAmmunition * x) {
	int i;
	for(i=0;i<4;i++) {
	}
	memset(x,0,sizeof(tArrayAmountAmmunition));
	return 1;
}

int TAT_Free_tArrayAmountLaunchUnit(tArrayAmountLaunchUnit * x) {
	int i;
	for(i=0;i<4;i++) {
	}
	memset(x,0,sizeof(tArrayAmountLaunchUnit));
	return 1;
}

int TAT_Free_tArrayShipsCharacter(tArrayShipsCharacter * x) {
	int i;
	for(i=0;i<5;i++) {
		TAT_Free_tShipCharacter(&(x->A[i]));
	}
	memset(x,0,sizeof(tArrayShipsCharacter));
	return 1;
}

int TAT_Free_tArrayWeaponryCharacter(tArrayWeaponryCharacter * x) {
	int i;
	for(i=0;i<4;i++) {
		TAT_Free_tWeaponryCharacter(&(x->A[i]));
	}
	memset(x,0,sizeof(tArrayWeaponryCharacter));
	return 1;
}

int TAT_Free_tCoordinates(tCoordinates * x) {
	memset(x,0,sizeof(tCoordinates));
	return 1;
}

int TAT_Free_tEnemyShip(tEnemyShip * x) {
	TAT_Free_tCoordinates(&(x->Coordinates));
	memset(x,0,sizeof(tEnemyShip));
	return 1;
}

int TAT_Free_tShipCharacter(tShipCharacter * x) {
	TAT_Free_tArrayAmountAmmunition(&(x->MaxAmmunition));
	TAT_Free_tArrayAmountLaunchUnit(&(x->MaxLaunchUnit));
	memset(x,0,sizeof(tShipCharacter));
	return 1;
}

int TAT_Free_tShipCurrentState(tShipCurrentState * x) {
	TAT_Free_tCoordinates(&(x->Coordinates));
	TAT_Free_tArrayAmountAmmunition(&(x->Ammunition));
	memset(x,0,sizeof(tShipCurrentState));
	return 1;
}

int TAT_Free_tStringEnemyShip(tStringEnemyShip * x) {
	tStringEnemyShip_yrec * cur=x->First;
	tStringEnemyShip_yrec * prev;
	while(cur) {
		TAT_Free_tEnemyShip(&(cur->Data));
		prev=cur;
		cur=cur->Suc;
		free(prev);
	}
	memset(x,0,sizeof(tStringEnemyShip));
	return 1;
}

int TAT_Free_tStringShipCurrentState(tStringShipCurrentState * x) {
	tStringShipCurrentState_yrec * cur=x->First;
	tStringShipCurrentState_yrec * prev;
	while(cur) {
		TAT_Free_tShipCurrentState(&(cur->Data));
		prev=cur;
		cur=cur->Suc;
		free(prev);
	}
	memset(x,0,sizeof(tStringShipCurrentState));
	return 1;
}

int TAT_Free_tStringShotsList(tStringShotsList * x) {
	tStringShotsList_yrec * cur=x->First;
	tStringShotsList_yrec * prev;
	while(cur) {
		TAT_Free_tCoordinates(&(cur->Data));
		prev=cur;
		cur=cur->Suc;
		free(prev);
	}
	memset(x,0,sizeof(tStringShotsList));
	return 1;
}

int TAT_Free_tWeaponryCharacter(tWeaponryCharacter * x) {
	memset(x,0,sizeof(tWeaponryCharacter));
	return 1;
}

tArrayAmountAmmunition TAT_Copy_tArrayAmountAmmunition(tArrayAmountAmmunition src) {
	tArrayAmountAmmunition x;
	int i;
	for(i=0;i<4;i++) {
		x.A[i]=src.A[i];
	}
	return x;
}

tArrayAmountLaunchUnit TAT_Copy_tArrayAmountLaunchUnit(tArrayAmountLaunchUnit src) {
	tArrayAmountLaunchUnit x;
	int i;
	for(i=0;i<4;i++) {
		x.A[i]=src.A[i];
	}
	return x;
}

tArrayShipsCharacter TAT_Copy_tArrayShipsCharacter(tArrayShipsCharacter src) {
	tArrayShipsCharacter x;
	int i;
	for(i=0;i<5;i++) {
		x.A[i]=TAT_Copy_tShipCharacter(src.A[i]);
	}
	return x;
}

tArrayWeaponryCharacter TAT_Copy_tArrayWeaponryCharacter(tArrayWeaponryCharacter src) {
	tArrayWeaponryCharacter x;
	int i;
	for(i=0;i<4;i++) {
		x.A[i]=TAT_Copy_tWeaponryCharacter(src.A[i]);
	}
	return x;
}

tCoordinates TAT_Copy_tCoordinates(tCoordinates src) {
	tCoordinates x;
	x.x=src.x;
	x.y=src.y;
	return x;
}

tEnemyShip TAT_Copy_tEnemyShip(tEnemyShip src) {
	tEnemyShip x;
	x.ShipId=src.ShipId;
	x.StrategyId=src.StrategyId;
	x.TypeShip=src.TypeShip;
	x.Speed=src.Speed;
	x.Heading=src.Heading;
	x.Coordinates=TAT_Copy_tCoordinates(src.Coordinates);
	return x;
}

tShipCharacter TAT_Copy_tShipCharacter(tShipCharacter src) {
	tShipCharacter x;
	x.Range=src.Range;
	x.MaxSpeed=src.MaxSpeed;
	x.MaxAcceleration=src.MaxAcceleration;
	x.MaxResource=src.MaxResource;
	x.MaxAmmunition=TAT_Copy_tArrayAmountAmmunition(src.MaxAmmunition);
	x.MaxLaunchUnit=TAT_Copy_tArrayAmountLaunchUnit(src.MaxLaunchUnit);
	x.MaxDeep=src.MaxDeep;
	return x;
}

tShipCurrentState TAT_Copy_tShipCurrentState(tShipCurrentState src) {
	tShipCurrentState x;
	x.ShipId=src.ShipId;
	x.StrategyId=src.StrategyId;
	x.TypeShip=src.TypeShip;
	x.Speed=src.Speed;
	x.Heading=src.Heading;
	x.Resource=src.Resource;
	x.Coordinates=TAT_Copy_tCoordinates(src.Coordinates);
	x.Ammunition=TAT_Copy_tArrayAmountAmmunition(src.Ammunition);
	return x;
}

tStringEnemyShip TAT_Copy_tStringEnemyShip(tStringEnemyShip src) {
	tStringEnemyShip x;
	tStringEnemyShip_yrec *cur, *scur;
	if(src.Length==0) {
		x.First=NULL;
		x.Last=NULL;
		x.Length=0;
		x.IsAssigned=1;
		return x;
	}
	x.First=(tStringEnemyShip_yrec *)malloc(sizeof(tStringEnemyShip_yrec));
	x.Last=x.First;
	x.Length=src.Length;
	x.IsAssigned=1;
	cur=x.First;
	scur=src.First;
	while(scur) {
		cur->Data=TAT_Copy_tEnemyShip(scur->Data);
		scur=scur->Suc;
		if(scur) {
			cur->Suc=(tStringEnemyShip_yrec *)malloc(sizeof(tStringEnemyShip_yrec));
			cur=cur->Suc;
		} else {
			cur->Suc=NULL;
			x.Last=cur;
		}
	}
	return x;
}

tStringShipCurrentState TAT_Copy_tStringShipCurrentState(tStringShipCurrentState src) {
	tStringShipCurrentState x;
	tStringShipCurrentState_yrec *cur, *scur;
	if(src.Length==0) {
		x.First=NULL;
		x.Last=NULL;
		x.Length=0;
		x.IsAssigned=1;
		return x;
	}
	x.First=(tStringShipCurrentState_yrec *)malloc(sizeof(tStringShipCurrentState_yrec));
	x.Last=x.First;
	x.Length=src.Length;
	x.IsAssigned=1;
	cur=x.First;
	scur=src.First;
	while(scur) {
		cur->Data=TAT_Copy_tShipCurrentState(scur->Data);
		scur=scur->Suc;
		if(scur) {
			cur->Suc=(tStringShipCurrentState_yrec *)malloc(sizeof(tStringShipCurrentState_yrec));
			cur=cur->Suc;
		} else {
			cur->Suc=NULL;
			x.Last=cur;
		}
	}
	return x;
}

tStringShotsList TAT_Copy_tStringShotsList(tStringShotsList src) {
	tStringShotsList x;
	tStringShotsList_yrec *cur, *scur;
	if(src.Length==0) {
		x.First=NULL;
		x.Last=NULL;
		x.Length=0;
		x.IsAssigned=1;
		return x;
	}
	x.First=(tStringShotsList_yrec *)malloc(sizeof(tStringShotsList_yrec));
	x.Last=x.First;
	x.Length=src.Length;
	x.IsAssigned=1;
	cur=x.First;
	scur=src.First;
	while(scur) {
		cur->Data=TAT_Copy_tCoordinates(scur->Data);
		scur=scur->Suc;
		if(scur) {
			cur->Suc=(tStringShotsList_yrec *)malloc(sizeof(tStringShotsList_yrec));
			cur=cur->Suc;
		} else {
			cur->Suc=NULL;
			x.Last=cur;
		}
	}
	return x;
}

tWeaponryCharacter TAT_Copy_tWeaponryCharacter(tWeaponryCharacter src) {
	tWeaponryCharacter x;
	x.Power=src.Power;
	x.Range=src.Range;
	x.Speed=src.Speed;
	return x;
}

int TAT_Compare_tArrayAmountAmmunition(tArrayAmountAmmunition x1, tArrayAmountAmmunition x2) {
	int i;
	for(i=0;i<4;i++) {
		if(x1.A[i]!=x2.A[i]) return 1;
	}
	return 0;
}

int TAT_Compare_tArrayAmountLaunchUnit(tArrayAmountLaunchUnit x1, tArrayAmountLaunchUnit x2) {
	int i;
	for(i=0;i<4;i++) {
		if(x1.A[i]!=x2.A[i]) return 1;
	}
	return 0;
}

int TAT_Compare_tArrayShipsCharacter(tArrayShipsCharacter x1, tArrayShipsCharacter x2) {
	int i;
	for(i=0;i<5;i++) {
		if(TAT_Compare_tShipCharacter(x1.A[i],x2.A[i])) return 1;
	}
	return 0;
}

int TAT_Compare_tArrayWeaponryCharacter(tArrayWeaponryCharacter x1, tArrayWeaponryCharacter x2) {
	int i;
	for(i=0;i<4;i++) {
		if(TAT_Compare_tWeaponryCharacter(x1.A[i],x2.A[i])) return 1;
	}
	return 0;
}

int TAT_Compare_tCoordinates(tCoordinates x1, tCoordinates x2) {
	if(x1.x!=x2.x) return 1;
	if(x1.y!=x2.y) return 1;
	return 0;
}

int TAT_Compare_tEnemyShip(tEnemyShip x1, tEnemyShip x2) {
	if(x1.ShipId!=x2.ShipId) return 1;
	if(x1.StrategyId!=x2.StrategyId) return 1;
	if(x1.TypeShip!=x2.TypeShip) return 1;
	if(x1.Speed!=x2.Speed) return 1;
	if(x1.Heading!=x2.Heading) return 1;
	if(TAT_Compare_tCoordinates(x1.Coordinates,x2.Coordinates)) return 1;
	return 0;
}

int TAT_Compare_tShipCharacter(tShipCharacter x1, tShipCharacter x2) {
	if(x1.Range!=x2.Range) return 1;
	if(x1.MaxSpeed!=x2.MaxSpeed) return 1;
	if(x1.MaxAcceleration!=x2.MaxAcceleration) return 1;
	if(x1.MaxResource!=x2.MaxResource) return 1;
	if(TAT_Compare_tArrayAmountAmmunition(x1.MaxAmmunition,x2.MaxAmmunition)) return 1;
	if(TAT_Compare_tArrayAmountLaunchUnit(x1.MaxLaunchUnit,x2.MaxLaunchUnit)) return 1;
	if(x1.MaxDeep!=x2.MaxDeep) return 1;
	return 0;
}

int TAT_Compare_tShipCurrentState(tShipCurrentState x1, tShipCurrentState x2) {
	if(x1.ShipId!=x2.ShipId) return 1;
	if(x1.StrategyId!=x2.StrategyId) return 1;
	if(x1.TypeShip!=x2.TypeShip) return 1;
	if(x1.Speed!=x2.Speed) return 1;
	if(x1.Heading!=x2.Heading) return 1;
	if(x1.Resource!=x2.Resource) return 1;
	if(TAT_Compare_tCoordinates(x1.Coordinates,x2.Coordinates)) return 1;
	if(TAT_Compare_tArrayAmountAmmunition(x1.Ammunition,x2.Ammunition)) return 1;
	return 0;
}

int TAT_Compare_tStringEnemyShip(tStringEnemyShip x1, tStringEnemyShip x2) {
	tStringEnemyShip_yrec *cur1, *cur2;
	if(x1.Length==0) return (x2.Length!=0);
	cur1=x1.First; cur2=x2.First;
	while(cur1 && cur2) {
		if(TAT_Compare_tEnemyShip(cur1->Data,cur2->Data)) return 1;
		cur1=cur1->Suc;
		cur2=cur2->Suc;
		if(cur1==NULL) return (cur2!=NULL);
	}
	return 0;
}

int TAT_Compare_tStringShipCurrentState(tStringShipCurrentState x1, tStringShipCurrentState x2) {
	tStringShipCurrentState_yrec *cur1, *cur2;
	if(x1.Length==0) return (x2.Length!=0);
	cur1=x1.First; cur2=x2.First;
	while(cur1 && cur2) {
		if(TAT_Compare_tShipCurrentState(cur1->Data,cur2->Data)) return 1;
		cur1=cur1->Suc;
		cur2=cur2->Suc;
		if(cur1==NULL) return (cur2!=NULL);
	}
	return 0;
}

int TAT_Compare_tStringShotsList(tStringShotsList x1, tStringShotsList x2) {
	tStringShotsList_yrec *cur1, *cur2;
	if(x1.Length==0) return (x2.Length!=0);
	cur1=x1.First; cur2=x2.First;
	while(cur1 && cur2) {
		if(TAT_Compare_tCoordinates(cur1->Data,cur2->Data)) return 1;
		cur1=cur1->Suc;
		cur2=cur2->Suc;
		if(cur1==NULL) return (cur2!=NULL);
	}
	return 0;
}

int TAT_Compare_tWeaponryCharacter(tWeaponryCharacter x1, tWeaponryCharacter x2) {
	if(x1.Power!=x2.Power) return 1;
	if(x1.Range!=x2.Range) return 1;
	if(x1.Speed!=x2.Speed) return 1;
	return 0;
}

static TAT_SIG_PARAM_DEF tat_SIG_PARAM_DestroyShip[]={
	{0,"Param1",sizeof(int), offsetof(TAT_SIG_DestroyShip, Param1), TAT_Dump_int},
	{1,"Param2",sizeof(int), offsetof(TAT_SIG_DestroyShip, Param2), TAT_Dump_int},
	{2,"Param3",sizeof(int), offsetof(TAT_SIG_DestroyShip, Param3), TAT_Dump_int}
};
TAT_TEST_RESULT TAT_Send_DestroyShip ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, int Param1, int Param2, int Param3)
{
	((TAT_SIG_DestroyShip *)ev->eventData)->Param1=Param1;
	((TAT_SIG_DestroyShip *)ev->eventData)->Param2=Param2;
	((TAT_SIG_DestroyShip *)ev->eventData)->Param3=Param3;
	ev->eventID=TAT_S_DestroyShip_ID;
	if(id->dumpEvent(id,ev,evSrc)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
	return (id->testResult=id->sendMessage(id, ev));
}
static TAT_SIG_PARAM_DEF tat_SIG_PARAM_FinishSimulation[]={
	{3,"Param1",sizeof(int), offsetof(TAT_SIG_FinishSimulation, Param1), TAT_Dump_int},
	{4,"Param2",sizeof(int), offsetof(TAT_SIG_FinishSimulation, Param2), TAT_Dump_int}
};
TAT_TEST_RESULT TAT_Send_InitDataComplete ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc)
{
	ev->eventID=TAT_S_InitDataComplete_ID;
	if(id->dumpEvent(id,ev,evSrc)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
	return (id->testResult=id->sendMessage(id, ev));
}
static TAT_SIG_PARAM_DEF tat_SIG_PARAM_OrderFireReject[]={
	{5,"Param1",sizeof(int), offsetof(TAT_SIG_OrderFireReject, Param1), TAT_Dump_int},
	{6,"Param2",sizeof(int), offsetof(TAT_SIG_OrderFireReject, Param2), TAT_Dump_int}
};
TAT_TEST_RESULT TAT_Send_OrderFireReject ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, int Param1, int Param2)
{
	((TAT_SIG_OrderFireReject *)ev->eventData)->Param1=Param1;
	((TAT_SIG_OrderFireReject *)ev->eventData)->Param2=Param2;
	ev->eventID=TAT_S_OrderFireReject_ID;
	if(id->dumpEvent(id,ev,evSrc)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
	return (id->testResult=id->sendMessage(id, ev));
}
static TAT_SIG_PARAM_DEF tat_SIG_PARAM_OrderFireRequest[]={
	{7,"Param1",sizeof(int), offsetof(TAT_SIG_OrderFireRequest, Param1), TAT_Dump_int},
	{8,"Param2",sizeof(int), offsetof(TAT_SIG_OrderFireRequest, Param2), TAT_Dump_int},
	{9,"Param3",sizeof(int), offsetof(TAT_SIG_OrderFireRequest, Param3), TAT_Dump_int},
	{10,"Param4",sizeof(tStringShotsList), offsetof(TAT_SIG_OrderFireRequest, Param4), TAT_Dump_tStringShotsList}
};
static TAT_SIG_PARAM_DEF tat_SIG_PARAM_OrderFireResponse[]={
	{11,"Param1",sizeof(int), offsetof(TAT_SIG_OrderFireResponse, Param1), TAT_Dump_int},
	{12,"Param2",sizeof(int), offsetof(TAT_SIG_OrderFireResponse, Param2), TAT_Dump_int}
};
TAT_TEST_RESULT TAT_Send_OrderFireResponse ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, int Param1, int Param2)
{
	((TAT_SIG_OrderFireResponse *)ev->eventData)->Param1=Param1;
	((TAT_SIG_OrderFireResponse *)ev->eventData)->Param2=Param2;
	ev->eventID=TAT_S_OrderFireResponse_ID;
	if(id->dumpEvent(id,ev,evSrc)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
	return (id->testResult=id->sendMessage(id, ev));
}
static TAT_SIG_PARAM_DEF tat_SIG_PARAM_OrderSpeedReject[]={
	{13,"Param1",sizeof(int), offsetof(TAT_SIG_OrderSpeedReject, Param1), TAT_Dump_int},
	{14,"Param2",sizeof(int), offsetof(TAT_SIG_OrderSpeedReject, Param2), TAT_Dump_int}
};
TAT_TEST_RESULT TAT_Send_OrderSpeedReject ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, int Param1, int Param2)
{
	((TAT_SIG_OrderSpeedReject *)ev->eventData)->Param1=Param1;
	((TAT_SIG_OrderSpeedReject *)ev->eventData)->Param2=Param2;
	ev->eventID=TAT_S_OrderSpeedReject_ID;
	if(id->dumpEvent(id,ev,evSrc)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
	return (id->testResult=id->sendMessage(id, ev));
}
static TAT_SIG_PARAM_DEF tat_SIG_PARAM_OrderSpeedRequest[]={
	{15,"Param1",sizeof(int), offsetof(TAT_SIG_OrderSpeedRequest, Param1), TAT_Dump_int},
	{16,"Param2",sizeof(int), offsetof(TAT_SIG_OrderSpeedRequest, Param2), TAT_Dump_int},
	{17,"Param3",sizeof(float), offsetof(TAT_SIG_OrderSpeedRequest, Param3), TAT_Dump_float}
};
static TAT_SIG_PARAM_DEF tat_SIG_PARAM_OrderSpeedResponse[]={
	{18,"Param1",sizeof(int), offsetof(TAT_SIG_OrderSpeedResponse, Param1), TAT_Dump_int},
	{19,"Param2",sizeof(int), offsetof(TAT_SIG_OrderSpeedResponse, Param2), TAT_Dump_int}
};
TAT_TEST_RESULT TAT_Send_OrderSpeedResponse ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, int Param1, int Param2)
{
	((TAT_SIG_OrderSpeedResponse *)ev->eventData)->Param1=Param1;
	((TAT_SIG_OrderSpeedResponse *)ev->eventData)->Param2=Param2;
	ev->eventID=TAT_S_OrderSpeedResponse_ID;
	if(id->dumpEvent(id,ev,evSrc)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
	return (id->testResult=id->sendMessage(id, ev));
}
static TAT_SIG_PARAM_DEF tat_SIG_PARAM_OrderTurnReject[]={
	{20,"Param1",sizeof(int), offsetof(TAT_SIG_OrderTurnReject, Param1), TAT_Dump_int},
	{21,"Param2",sizeof(int), offsetof(TAT_SIG_OrderTurnReject, Param2), TAT_Dump_int}
};
TAT_TEST_RESULT TAT_Send_OrderTurnReject ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, int Param1, int Param2)
{
	((TAT_SIG_OrderTurnReject *)ev->eventData)->Param1=Param1;
	((TAT_SIG_OrderTurnReject *)ev->eventData)->Param2=Param2;
	ev->eventID=TAT_S_OrderTurnReject_ID;
	if(id->dumpEvent(id,ev,evSrc)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
	return (id->testResult=id->sendMessage(id, ev));
}
static TAT_SIG_PARAM_DEF tat_SIG_PARAM_OrderTurnRequest[]={
	{22,"Param1",sizeof(int), offsetof(TAT_SIG_OrderTurnRequest, Param1), TAT_Dump_int},
	{23,"Param2",sizeof(int), offsetof(TAT_SIG_OrderTurnRequest, Param2), TAT_Dump_int},
	{24,"Param3",sizeof(int), offsetof(TAT_SIG_OrderTurnRequest, Param3), TAT_Dump_int}
};
static TAT_SIG_PARAM_DEF tat_SIG_PARAM_OrderTurnResponse[]={
	{25,"Param1",sizeof(int), offsetof(TAT_SIG_OrderTurnResponse, Param1), TAT_Dump_int},
	{26,"Param2",sizeof(int), offsetof(TAT_SIG_OrderTurnResponse, Param2), TAT_Dump_int}
};
TAT_TEST_RESULT TAT_Send_OrderTurnResponse ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, int Param1, int Param2)
{
	((TAT_SIG_OrderTurnResponse *)ev->eventData)->Param1=Param1;
	((TAT_SIG_OrderTurnResponse *)ev->eventData)->Param2=Param2;
	ev->eventID=TAT_S_OrderTurnResponse_ID;
	if(id->dumpEvent(id,ev,evSrc)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
	return (id->testResult=id->sendMessage(id, ev));
}
static TAT_SIG_PARAM_DEF tat_SIG_PARAM_ShipsCurrentState[]={
	{27,"Param1",sizeof(int), offsetof(TAT_SIG_ShipsCurrentState, Param1), TAT_Dump_int},
	{28,"Param2",sizeof(tStringShipCurrentState), offsetof(TAT_SIG_ShipsCurrentState, Param2), TAT_Dump_tStringShipCurrentState}
};
TAT_TEST_RESULT TAT_Send_ShipsCurrentState ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, int Param1, tStringShipCurrentState Param2)
{
	((TAT_SIG_ShipsCurrentState *)ev->eventData)->Param1=Param1;
	((TAT_SIG_ShipsCurrentState *)ev->eventData)->Param2=Param2;
	ev->eventID=TAT_S_ShipsCurrentState_ID;
	if(id->dumpEvent(id,ev,evSrc)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
	return (id->testResult=id->sendMessage(id, ev));
}
static TAT_SIG_PARAM_DEF tat_SIG_PARAM_StartSimulation[]={
	{29,"Param1",sizeof(int), offsetof(TAT_SIG_StartSimulation, Param1), TAT_Dump_int}
};
TAT_TEST_RESULT TAT_Send_StartSimulation ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, int Param1)
{
	((TAT_SIG_StartSimulation *)ev->eventData)->Param1=Param1;
	ev->eventID=TAT_S_StartSimulation_ID;
	if(id->dumpEvent(id,ev,evSrc)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
	return (id->testResult=id->sendMessage(id, ev));
}
static TAT_SIG_PARAM_DEF tat_SIG_PARAM_StrategyLose[]={
	{30,"Param1",sizeof(int), offsetof(TAT_SIG_StrategyLose, Param1), TAT_Dump_int}
};
TAT_TEST_RESULT TAT_Send_StrategyLose ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, int Param1)
{
	((TAT_SIG_StrategyLose *)ev->eventData)->Param1=Param1;
	ev->eventID=TAT_S_StrategyLose_ID;
	if(id->dumpEvent(id,ev,evSrc)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
	return (id->testResult=id->sendMessage(id, ev));
}
static TAT_SIG_PARAM_DEF tat_SIG_PARAM_StrategyVictory[]={
	{31,"Param1",sizeof(int), offsetof(TAT_SIG_StrategyVictory, Param1), TAT_Dump_int}
};
TAT_TEST_RESULT TAT_Send_StrategyVictory ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, int Param1)
{
	((TAT_SIG_StrategyVictory *)ev->eventData)->Param1=Param1;
	ev->eventID=TAT_S_StrategyVictory_ID;
	if(id->dumpEvent(id,ev,evSrc)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
	return (id->testResult=id->sendMessage(id, ev));
}
static TAT_SIG_PARAM_DEF tat_SIG_PARAM_VisibleShip[]={
	{32,"Param1",sizeof(int), offsetof(TAT_SIG_VisibleShip, Param1), TAT_Dump_int},
	{33,"Param2",sizeof(tStringEnemyShip), offsetof(TAT_SIG_VisibleShip, Param2), TAT_Dump_tStringEnemyShip}
};
TAT_TEST_RESULT TAT_Send_VisibleShip ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, int Param1, tStringEnemyShip Param2)
{
	((TAT_SIG_VisibleShip *)ev->eventData)->Param1=Param1;
	((TAT_SIG_VisibleShip *)ev->eventData)->Param2=Param2;
	ev->eventID=TAT_S_VisibleShip_ID;
	if(id->dumpEvent(id,ev,evSrc)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
	return (id->testResult=id->sendMessage(id, ev));
}
static TAT_SIG_PARAM_DEF tat_SIG_PARAM_messageError[]={
	{34,"Param1",sizeof(SDL_Charstring), offsetof(TAT_SIG_messageError, Param1), TAT_Dump_String}
};
static TAT_SIG_PARAM_DEF tat_SIG_PARAM_messageLog[]={
	{35,"Param1",sizeof(SDL_Charstring), offsetof(TAT_SIG_messageLog, Param1), TAT_Dump_String}
};
TAT_TEST_RESULT TAT_Send_resetStrategy ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc)
{
	ev->eventID=TAT_S_resetStrategy_ID;
	if(id->dumpEvent(id,ev,evSrc)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;
	return (id->testResult=id->sendMessage(id, ev));
}
static TAT_SIGNAL_DEF tat_SignalDefinition[]={
	{    TAT_S_DestroyShip_ID,	"DestroyShip", sizeof(TAT_SIG_DestroyShip), 3, (TAT_SIG_PARAM_DEF *)&tat_SIG_PARAM_DestroyShip, 100},
	{TAT_S_FinishSimulation_ID,	"FinishSimulation", sizeof(TAT_SIG_FinishSimulation), 2, (TAT_SIG_PARAM_DEF *)&tat_SIG_PARAM_FinishSimulation, 100},
	{TAT_S_InitDataComplete_ID,	"InitDataComplete", 0, 0, NULL, 100},
	{TAT_S_OrderFireReject_ID,	"OrderFireReject", sizeof(TAT_SIG_OrderFireReject), 2, (TAT_SIG_PARAM_DEF *)&tat_SIG_PARAM_OrderFireReject, 100},
	{TAT_S_OrderFireRequest_ID,	"OrderFireRequest", sizeof(TAT_SIG_OrderFireRequest), 4, (TAT_SIG_PARAM_DEF *)&tat_SIG_PARAM_OrderFireRequest, 100},
	{TAT_S_OrderFireResponse_ID,	"OrderFireResponse", sizeof(TAT_SIG_OrderFireResponse), 2, (TAT_SIG_PARAM_DEF *)&tat_SIG_PARAM_OrderFireResponse, 100},
	{TAT_S_OrderSpeedReject_ID,	"OrderSpeedReject", sizeof(TAT_SIG_OrderSpeedReject), 2, (TAT_SIG_PARAM_DEF *)&tat_SIG_PARAM_OrderSpeedReject, 100},
	{TAT_S_OrderSpeedRequest_ID,	"OrderSpeedRequest", sizeof(TAT_SIG_OrderSpeedRequest), 3, (TAT_SIG_PARAM_DEF *)&tat_SIG_PARAM_OrderSpeedRequest, 100},
	{TAT_S_OrderSpeedResponse_ID,	"OrderSpeedResponse", sizeof(TAT_SIG_OrderSpeedResponse), 2, (TAT_SIG_PARAM_DEF *)&tat_SIG_PARAM_OrderSpeedResponse, 100},
	{TAT_S_OrderTurnReject_ID,	"OrderTurnReject", sizeof(TAT_SIG_OrderTurnReject), 2, (TAT_SIG_PARAM_DEF *)&tat_SIG_PARAM_OrderTurnReject, 100},
	{TAT_S_OrderTurnRequest_ID,	"OrderTurnRequest", sizeof(TAT_SIG_OrderTurnRequest), 3, (TAT_SIG_PARAM_DEF *)&tat_SIG_PARAM_OrderTurnRequest, 100},
	{TAT_S_OrderTurnResponse_ID,	"OrderTurnResponse", sizeof(TAT_SIG_OrderTurnResponse), 2, (TAT_SIG_PARAM_DEF *)&tat_SIG_PARAM_OrderTurnResponse, 100},
	{TAT_S_ShipsCurrentState_ID,	"ShipsCurrentState", sizeof(TAT_SIG_ShipsCurrentState), 2, (TAT_SIG_PARAM_DEF *)&tat_SIG_PARAM_ShipsCurrentState, 100},
	{TAT_S_StartSimulation_ID,	"StartSimulation", sizeof(TAT_SIG_StartSimulation), 1, (TAT_SIG_PARAM_DEF *)&tat_SIG_PARAM_StartSimulation, 100},
	{   TAT_S_StrategyLose_ID,	"StrategyLose", sizeof(TAT_SIG_StrategyLose), 1, (TAT_SIG_PARAM_DEF *)&tat_SIG_PARAM_StrategyLose, 100},
	{TAT_S_StrategyVictory_ID,	"StrategyVictory", sizeof(TAT_SIG_StrategyVictory), 1, (TAT_SIG_PARAM_DEF *)&tat_SIG_PARAM_StrategyVictory, 100},
	{    TAT_S_VisibleShip_ID,	"VisibleShip", sizeof(TAT_SIG_VisibleShip), 2, (TAT_SIG_PARAM_DEF *)&tat_SIG_PARAM_VisibleShip, 100},
	{   TAT_S_messageError_ID,	"messageError", sizeof(TAT_SIG_messageError), 1, (TAT_SIG_PARAM_DEF *)&tat_SIG_PARAM_messageError, 100},
	{     TAT_S_messageLog_ID,	"messageLog", sizeof(TAT_SIG_messageLog), 1, (TAT_SIG_PARAM_DEF *)&tat_SIG_PARAM_messageLog, 100},
	{  TAT_S_resetStrategy_ID,	"resetStrategy", 0, 0, NULL, 100}
};


static int _ssort(const void *x1, const void *x2)
{
	return (( TAT_SIGNAL_DEF * ) x1 )->signalID - (( TAT_SIGNAL_DEF * ) x2 )->signalID;
}
static int _isort(const void *x1, const void *x2)
{
	return (( TAT_INSTANCE_DEF * ) x1 )->instanceID - (( TAT_INSTANCE_DEF * ) x2 )->instanceID;
}
TAT_TEST_RESULT TAT_Init_battleshipstest (TAT_TEST_INSTANCE * id)
{
	if (!id) return TAT_TEST_CRITICAL;
	id->signalCount = 20;
	id->signalDef = (TAT_SIGNAL_DEF *)&tat_SignalDefinition;
	id->instanceCount = 2;
	id->instanceDef = (TAT_INSTANCE_DEF *)&tat_InstanceDefinition;
	qsort(id->signalDef, id->signalCount, sizeof(TAT_SIGNAL_DEF), _ssort);
	qsort(id->instanceDef, id->instanceCount, sizeof(TAT_INSTANCE_DEF), _isort);
	id->currSuiteName = "battleshipstest";
	id->testCount = 15;
	id->testTable = (TAT_TEST_TABLE *)&TAT_battleshipstest_TestTable;
	return TAT_TEST_CONTINUE;
}
TAT_TEST_TABLE TAT_battleshipstest_TestTable[]={
{"Test0001N0", TAT_T_battleshipstest_Test0001N0, TAT_TI_battleshipstest_Test0001N0},
{"Test0008N0", TAT_T_battleshipstest_Test0008N0, TAT_TI_battleshipstest_Test0008N0},
{"Test0014N0", TAT_T_battleshipstest_Test0014N0, TAT_TI_battleshipstest_Test0014N0},
{"Test0022N0", TAT_T_battleshipstest_Test0022N0, TAT_TI_battleshipstest_Test0022N0},
{"Test0032N0", TAT_T_battleshipstest_Test0032N0, TAT_TI_battleshipstest_Test0032N0},
{"Test0001N1", TAT_T_battleshipstest_Test0001N1, TAT_TI_battleshipstest_Test0001N1},
{"Test0008N1", TAT_T_battleshipstest_Test0008N1, TAT_TI_battleshipstest_Test0008N1},
{"Test0014N1", TAT_T_battleshipstest_Test0014N1, TAT_TI_battleshipstest_Test0014N1},
{"Test0022N1", TAT_T_battleshipstest_Test0022N1, TAT_TI_battleshipstest_Test0022N1},
{"Test0032N1", TAT_T_battleshipstest_Test0032N1, TAT_TI_battleshipstest_Test0032N1},
{"Test0001N2", TAT_T_battleshipstest_Test0001N2, TAT_TI_battleshipstest_Test0001N2},
{"Test0008N2", TAT_T_battleshipstest_Test0008N2, TAT_TI_battleshipstest_Test0008N2},
{"Test0014N2", TAT_T_battleshipstest_Test0014N2, TAT_TI_battleshipstest_Test0014N2},
{"Test0022N2", TAT_T_battleshipstest_Test0022N2, TAT_TI_battleshipstest_Test0022N2},
{"Test0032N2", TAT_T_battleshipstest_Test0032N2, TAT_TI_battleshipstest_Test0032N2}};
