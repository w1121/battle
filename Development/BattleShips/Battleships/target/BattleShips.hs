
#ifndef XHS
#ifndef XHS_START
#define XHS_START 0
#endif

/*--------------------------- Signal numbers ----------------------------*/
#ifndef sig_BasicTypes_InitDataComplete
#define sig_BasicTypes_InitDataComplete XHS_START+1
#endif
#ifndef sig_BasicTypes_StartSimulation
#define sig_BasicTypes_StartSimulation XHS_START+2
#endif
#ifndef sig_BasicTypes_FinishSimulation
#define sig_BasicTypes_FinishSimulation XHS_START+3
#endif
#ifndef sig_BasicTypes_OrderSpeedRequest
#define sig_BasicTypes_OrderSpeedRequest XHS_START+4
#endif
#ifndef sig_BasicTypes_OrderTurnRequest
#define sig_BasicTypes_OrderTurnRequest XHS_START+5
#endif
#ifndef sig_BasicTypes_OrderFireRequest
#define sig_BasicTypes_OrderFireRequest XHS_START+6
#endif
#ifndef sig_BasicTypes_DestroyShip
#define sig_BasicTypes_DestroyShip XHS_START+7
#endif
#ifndef sig_BasicTypes_StrategyVictory
#define sig_BasicTypes_StrategyVictory XHS_START+8
#endif
#ifndef sig_BasicTypes_StrategyLose
#define sig_BasicTypes_StrategyLose XHS_START+9
#endif
#ifndef sig_BasicTypes_OrderSpeedResponse
#define sig_BasicTypes_OrderSpeedResponse XHS_START+10
#endif
#ifndef sig_BasicTypes_OrderSpeedReject
#define sig_BasicTypes_OrderSpeedReject XHS_START+11
#endif
#ifndef sig_BasicTypes_OrderTurnResponse
#define sig_BasicTypes_OrderTurnResponse XHS_START+12
#endif
#ifndef sig_BasicTypes_OrderTurnReject
#define sig_BasicTypes_OrderTurnReject XHS_START+13
#endif
#ifndef sig_BasicTypes_OrderFireResponse
#define sig_BasicTypes_OrderFireResponse XHS_START+14
#endif
#ifndef sig_BasicTypes_OrderFireReject
#define sig_BasicTypes_OrderFireReject XHS_START+15
#endif
#ifndef sig_BasicTypes_ShipsCurrentState
#define sig_BasicTypes_ShipsCurrentState XHS_START+16
#endif
#ifndef sig_BasicTypes_VisibleShip
#define sig_BasicTypes_VisibleShip XHS_START+17
#endif
#ifndef sig_BasicTypes_messageLog
#define sig_BasicTypes_messageLog XHS_START+18
#endif
#ifndef sig_BasicTypes_messageError
#define sig_BasicTypes_messageError XHS_START+19
#endif
#ifndef sig_Simulator_SendData
#define sig_Simulator_SendData XHS_START+20
#endif
#ifndef sig_Simulator_InitSocket
#define sig_Simulator_InitSocket XHS_START+21
#endif
#ifndef sig_Simulator_SendDataComplete
#define sig_Simulator_SendDataComplete XHS_START+22
#endif
#ifndef sig_Simulator_InitSocketComplete
#define sig_Simulator_InitSocketComplete XHS_START+23
#endif
#ifndef sig_Simulator_CloseClientSocket
#define sig_Simulator_CloseClientSocket XHS_START+24
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
