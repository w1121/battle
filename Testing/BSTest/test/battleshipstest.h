/* TAT TEST SUITE HEADER FILE */
#ifndef TAT_battleshipstest_H
#define TAT_battleshipstest_H
#ifndef TAT_INSTANCE_SUPPORT
#error TAT_INSTANCE_SUPPORT define not set for compilation
#endif
#ifndef TAT_TIME_SUPPORT
#error TAT_TIME_SUPPORT define not set for compilation
#endif
#include <testkern.h>
#include <scttypes.h>
#include "types.h"
#include "model.ifc"

#define TAT_I_Simulator_ID 1
#define TAT_I_Strategy_ID 2







extern tStringEnemyShip_yrec * TAT_tStringEnemyShip_Cur;

extern tStringShipCurrentState_yrec * TAT_tStringShipCurrentState_Cur;

extern tStringShotsList_yrec * TAT_tStringShotsList_Cur;




int TAT_Dump_tArrayAmountAmmunition(void *data, char *dest);
int TAT_Dump_tArrayAmountLaunchUnit(void *data, char *dest);
int TAT_Dump_tArrayShipsCharacter(void *data, char *dest);
int TAT_Dump_tArrayWeaponryCharacter(void *data, char *dest);
int TAT_Dump_tCoordinates(void *data, char *dest);
int TAT_Dump_tEnemyShip(void *data, char *dest);
int TAT_Dump_tShipCharacter(void *data, char *dest);
int TAT_Dump_tShipCurrentState(void *data, char *dest);
int TAT_Dump_tStringEnemyShip(void *data, char *dest);
int TAT_Dump_tStringShipCurrentState(void *data, char *dest);
int TAT_Dump_tStringShotsList(void *data, char *dest);
int TAT_Dump_tWeaponryCharacter(void *data, char *dest);

int TAT_Free_tArrayAmountAmmunition(tArrayAmountAmmunition * x);
int TAT_Free_tArrayAmountLaunchUnit(tArrayAmountLaunchUnit * x);
int TAT_Free_tArrayShipsCharacter(tArrayShipsCharacter * x);
int TAT_Free_tArrayWeaponryCharacter(tArrayWeaponryCharacter * x);
int TAT_Free_tCoordinates(tCoordinates * x);
int TAT_Free_tEnemyShip(tEnemyShip * x);
int TAT_Free_tShipCharacter(tShipCharacter * x);
int TAT_Free_tShipCurrentState(tShipCurrentState * x);
int TAT_Free_tStringEnemyShip(tStringEnemyShip * x);
int TAT_Free_tStringShipCurrentState(tStringShipCurrentState * x);
int TAT_Free_tStringShotsList(tStringShotsList * x);
int TAT_Free_tWeaponryCharacter(tWeaponryCharacter * x);

tArrayAmountAmmunition TAT_Copy_tArrayAmountAmmunition(tArrayAmountAmmunition src);
tArrayAmountLaunchUnit TAT_Copy_tArrayAmountLaunchUnit(tArrayAmountLaunchUnit src);
tArrayShipsCharacter TAT_Copy_tArrayShipsCharacter(tArrayShipsCharacter src);
tArrayWeaponryCharacter TAT_Copy_tArrayWeaponryCharacter(tArrayWeaponryCharacter src);
tCoordinates TAT_Copy_tCoordinates(tCoordinates src);
tEnemyShip TAT_Copy_tEnemyShip(tEnemyShip src);
tShipCharacter TAT_Copy_tShipCharacter(tShipCharacter src);
tShipCurrentState TAT_Copy_tShipCurrentState(tShipCurrentState src);
tStringEnemyShip TAT_Copy_tStringEnemyShip(tStringEnemyShip src);
tStringShipCurrentState TAT_Copy_tStringShipCurrentState(tStringShipCurrentState src);
tStringShotsList TAT_Copy_tStringShotsList(tStringShotsList src);
tWeaponryCharacter TAT_Copy_tWeaponryCharacter(tWeaponryCharacter src);

int TAT_Compare_tArrayAmountAmmunition(tArrayAmountAmmunition x1, tArrayAmountAmmunition x2);
int TAT_Compare_tArrayAmountLaunchUnit(tArrayAmountLaunchUnit x1, tArrayAmountLaunchUnit x2);
int TAT_Compare_tArrayShipsCharacter(tArrayShipsCharacter x1, tArrayShipsCharacter x2);
int TAT_Compare_tArrayWeaponryCharacter(tArrayWeaponryCharacter x1, tArrayWeaponryCharacter x2);
int TAT_Compare_tCoordinates(tCoordinates x1, tCoordinates x2);
int TAT_Compare_tEnemyShip(tEnemyShip x1, tEnemyShip x2);
int TAT_Compare_tShipCharacter(tShipCharacter x1, tShipCharacter x2);
int TAT_Compare_tShipCurrentState(tShipCurrentState x1, tShipCurrentState x2);
int TAT_Compare_tStringEnemyShip(tStringEnemyShip x1, tStringEnemyShip x2);
int TAT_Compare_tStringShipCurrentState(tStringShipCurrentState x1, tStringShipCurrentState x2);
int TAT_Compare_tStringShotsList(tStringShotsList x1, tStringShotsList x2);
int TAT_Compare_tWeaponryCharacter(tWeaponryCharacter x1, tWeaponryCharacter x2);

#define TAT_S_DestroyShip_ID 1
typedef struct {
	int Param1;
	int Param2;
	int Param3;
} TAT_SIG_DestroyShip;

extern TAT_TEST_RESULT TAT_Send_DestroyShip ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, int Param1, int Param2, int Param3);

#define TAT_S_FinishSimulation_ID 2
typedef struct {
	int Param1;
	int Param2;
} TAT_SIG_FinishSimulation;


#define TAT_S_InitDataComplete_ID 3
extern TAT_TEST_RESULT TAT_Send_InitDataComplete ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc);

#define TAT_S_OrderFireReject_ID 4
typedef struct {
	int Param1;
	int Param2;
} TAT_SIG_OrderFireReject;

extern TAT_TEST_RESULT TAT_Send_OrderFireReject ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, int Param1, int Param2);

#define TAT_S_OrderFireRequest_ID 5
typedef struct {
	int Param1;
	int Param2;
	int Param3;
	tStringShotsList Param4;
} TAT_SIG_OrderFireRequest;


#define TAT_S_OrderFireResponse_ID 6
typedef struct {
	int Param1;
	int Param2;
} TAT_SIG_OrderFireResponse;

extern TAT_TEST_RESULT TAT_Send_OrderFireResponse ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, int Param1, int Param2);

#define TAT_S_OrderSpeedReject_ID 7
typedef struct {
	int Param1;
	int Param2;
} TAT_SIG_OrderSpeedReject;

extern TAT_TEST_RESULT TAT_Send_OrderSpeedReject ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, int Param1, int Param2);

#define TAT_S_OrderSpeedRequest_ID 8
typedef struct {
	int Param1;
	int Param2;
	float Param3;
} TAT_SIG_OrderSpeedRequest;


#define TAT_S_OrderSpeedResponse_ID 9
typedef struct {
	int Param1;
	int Param2;
} TAT_SIG_OrderSpeedResponse;

extern TAT_TEST_RESULT TAT_Send_OrderSpeedResponse ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, int Param1, int Param2);

#define TAT_S_OrderTurnReject_ID 10
typedef struct {
	int Param1;
	int Param2;
} TAT_SIG_OrderTurnReject;

extern TAT_TEST_RESULT TAT_Send_OrderTurnReject ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, int Param1, int Param2);

#define TAT_S_OrderTurnRequest_ID 11
typedef struct {
	int Param1;
	int Param2;
	int Param3;
} TAT_SIG_OrderTurnRequest;


#define TAT_S_OrderTurnResponse_ID 12
typedef struct {
	int Param1;
	int Param2;
} TAT_SIG_OrderTurnResponse;

extern TAT_TEST_RESULT TAT_Send_OrderTurnResponse ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, int Param1, int Param2);

#define TAT_S_ShipsCurrentState_ID 13
typedef struct {
	int Param1;
	tStringShipCurrentState Param2;
} TAT_SIG_ShipsCurrentState;

extern TAT_TEST_RESULT TAT_Send_ShipsCurrentState ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, int Param1, tStringShipCurrentState Param2);

#define TAT_S_StartSimulation_ID 14
typedef struct {
	int Param1;
} TAT_SIG_StartSimulation;

extern TAT_TEST_RESULT TAT_Send_StartSimulation ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, int Param1);

#define TAT_S_StrategyLose_ID 15
typedef struct {
	int Param1;
} TAT_SIG_StrategyLose;

extern TAT_TEST_RESULT TAT_Send_StrategyLose ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, int Param1);

#define TAT_S_StrategyVictory_ID 16
typedef struct {
	int Param1;
} TAT_SIG_StrategyVictory;

extern TAT_TEST_RESULT TAT_Send_StrategyVictory ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, int Param1);

#define TAT_S_VisibleShip_ID 17
typedef struct {
	int Param1;
	tStringEnemyShip Param2;
} TAT_SIG_VisibleShip;

extern TAT_TEST_RESULT TAT_Send_VisibleShip ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc, int Param1, tStringEnemyShip Param2);

#define TAT_S_messageError_ID 18
typedef struct {
	SDL_Charstring Param1;
} TAT_SIG_messageError;


#define TAT_S_messageLog_ID 19
typedef struct {
	SDL_Charstring Param1;
} TAT_SIG_messageLog;


#define TAT_S_resetStrategy_ID 20
extern TAT_TEST_RESULT TAT_Send_resetStrategy ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc);

extern TAT_TEST_RESULT TAT_Init_battleshipstest (TAT_TEST_INSTANCE * id);
extern TAT_TEST_TABLE TAT_battleshipstest_TestTable[];
extern TAT_TEST_RESULT TAT_TI_battleshipstest_Test0001N0( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_T_battleshipstest_Test0001N0( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_TI_battleshipstest_Test0008N0( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_T_battleshipstest_Test0008N0( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_TI_battleshipstest_Test0014N0( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_T_battleshipstest_Test0014N0( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_TI_battleshipstest_Test0022N0( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_T_battleshipstest_Test0022N0( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_TI_battleshipstest_Test0032N0( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_T_battleshipstest_Test0032N0( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_TI_battleshipstest_Test0001N1( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_T_battleshipstest_Test0001N1( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_TI_battleshipstest_Test0008N1( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_T_battleshipstest_Test0008N1( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_TI_battleshipstest_Test0014N1( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_T_battleshipstest_Test0014N1( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_TI_battleshipstest_Test0022N1( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_T_battleshipstest_Test0022N1( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_TI_battleshipstest_Test0032N1( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_T_battleshipstest_Test0032N1( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_TI_battleshipstest_Test0001N2( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_T_battleshipstest_Test0001N2( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_TI_battleshipstest_Test0008N2( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_T_battleshipstest_Test0008N2( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_TI_battleshipstest_Test0014N2( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_T_battleshipstest_Test0014N2( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_TI_battleshipstest_Test0022N2( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_T_battleshipstest_Test0022N2( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_TI_battleshipstest_Test0032N2( TAT_TEST_INSTANCE* id );
extern TAT_TEST_RESULT TAT_T_battleshipstest_Test0032N2( TAT_TEST_INSTANCE* id );
#endif
