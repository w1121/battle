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
* Description:  Test suite "TAT_Test" wrapper header
*
* Notes:        None
*
******************************************************************************/

#include <stdio.h>
#include <testkern.h>
#include "TAT_Test.h"
#include <txtlog.h>
#include <mprlog.h>

/* User include files */
extern int TAT_Test_A_Handler(ComplexData);



extern TAT_SIG_B model_sigB;
extern TAT_EVENT_INSTANCE model_evB;

int TAT_Test_Recv_B(ComplexData, int, int);

#endif /* WRAPPER_H */
