
#ifndef XHS
#ifndef XHS_START
#define XHS_START 0
#endif

/*--------------------------- Signal numbers ----------------------------*/
#ifndef InitDataComplete
#define InitDataComplete XHS_START+1
#endif
#ifndef StartSimulation
#define StartSimulation XHS_START+2
#endif
#ifndef FinishSimulation
#define FinishSimulation XHS_START+3
#endif
#ifndef OrderSpeedRequest
#define OrderSpeedRequest XHS_START+4
#endif
#ifndef OrderTurnRequest
#define OrderTurnRequest XHS_START+5
#endif
#ifndef OrderFireRequest
#define OrderFireRequest XHS_START+6
#endif
#ifndef DestroyShip
#define DestroyShip XHS_START+7
#endif
#ifndef StrategyVictory
#define StrategyVictory XHS_START+8
#endif
#ifndef StrategyLose
#define StrategyLose XHS_START+9
#endif
#ifndef OrderSpeedResponse
#define OrderSpeedResponse XHS_START+10
#endif
#ifndef OrderSpeedReject
#define OrderSpeedReject XHS_START+11
#endif
#ifndef OrderTurnResponse
#define OrderTurnResponse XHS_START+12
#endif
#ifndef OrderTurnReject
#define OrderTurnReject XHS_START+13
#endif
#ifndef OrderFireResponse
#define OrderFireResponse XHS_START+14
#endif
#ifndef OrderFireReject
#define OrderFireReject XHS_START+15
#endif
#ifndef ShipsCurrentState
#define ShipsCurrentState XHS_START+16
#endif
#ifndef VisibleShip
#define VisibleShip XHS_START+17
#endif
#ifndef messageLog
#define messageLog XHS_START+18
#endif
#ifndef messageError
#define messageError XHS_START+19
#endif
#ifndef SendData
#define SendData XHS_START+20
#endif
#ifndef InitSocket
#define InitSocket XHS_START+21
#endif
#ifndef SendDataComplete
#define SendDataComplete XHS_START+22
#endif
#ifndef InitSocketComplete
#define InitSocketComplete XHS_START+23
#endif
#ifndef CloseClientSocket
#define CloseClientSocket XHS_START+24
#endif

/*---------------------------- Process data -----------------------------*/
#ifndef Main2_TIME
#define Main2_TIME 0
#endif
#ifndef Main2_STACKSIZE
#define Main2_STACKSIZE 1024
#endif
#ifndef Router_TIME
#define Router_TIME 0
#endif
#ifndef Router_STACKSIZE
#define Router_STACKSIZE 1024
#endif
#ifndef Main_TIME
#define Main_TIME 0
#endif
#ifndef Main_STACKSIZE
#define Main_STACKSIZE 1024
#endif
#ifndef DataSender_TIME
#define DataSender_TIME 0
#endif
#ifndef DataSender_STACKSIZE
#define DataSender_STACKSIZE 1024
#endif
#ifndef Main1_TIME
#define Main1_TIME 0
#endif
#ifndef Main1_STACKSIZE
#define Main1_STACKSIZE 1024
#endif
#endif
   /* XHS */

#ifdef XHS
#define XXHS \#include "BattleShips.hs"
XXHS
#endif
