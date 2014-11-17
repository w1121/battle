#ifndef WRAPPER_H
#define WRAPPER_H
/******************************************************************************
*
*       COPYRIGHT 2001-2004 MOTOROLA, ALL RIGHTS RESERVED
*
*       The code is the property of Motorola St.Petersburg Software Development
*       and is Motorola Confidential Proprietary Information.
*
*       The copyright notice above does not evidence any
*       actual or intended publication of such source code.
*
* Functions:    
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  Test suite "battleshipstest" wrapper header
*
* Notes:        None
*
******************************************************************************/

#include "battleshipstest.h"
#include <txtlog.h>
#include <mprlog.h>

/* User include files */



extern TAT_SIG_FinishSimulation model_sigFinishSimulation;
extern TAT_EVENT_INSTANCE model_evFinishSimulation;


extern TAT_SIG_OrderFireRequest model_sigOrderFireRequest;
extern TAT_EVENT_INSTANCE model_evOrderFireRequest;


extern TAT_SIG_OrderSpeedRequest model_sigOrderSpeedRequest;
extern TAT_EVENT_INSTANCE model_evOrderSpeedRequest;


extern TAT_SIG_OrderTurnRequest model_sigOrderTurnRequest;
extern TAT_EVENT_INSTANCE model_evOrderTurnRequest;


extern TAT_SIG_messageError model_sigmessageError;
extern TAT_EVENT_INSTANCE model_evmessageError;


extern TAT_SIG_messageLog model_sigmessageLog;
extern TAT_EVENT_INSTANCE model_evmessageLog;

int battleshipstest_Recv_FinishSimulation(int, int, int, int);
int battleshipstest_Recv_OrderFireRequest(int, int, int, tStringShotsList, int, int);
int battleshipstest_Recv_OrderSpeedRequest(int, int, float, int, int);
int battleshipstest_Recv_OrderTurnRequest(int, int, int, int, int);
int battleshipstest_Recv_messageError(SDL_Charstring, int, int);
int battleshipstest_Recv_messageLog(SDL_Charstring, int, int);

#endif /* WRAPPER_H */
