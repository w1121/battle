/******************************************************************************
*
*       COPYRIGHT 2001-2002 MOTOROLA, ALL RIGHTS RESERVED
*
*       The code is the property of Motorola St.Petersburg Software Development
*       and is Motorola Confidential Proprietary Information.
*
*       The copyright notice above does not evidence any
*       actual or intended publication of such source code.
*
* Functions:    None
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  Shared message text
*
* Notes:        None
*
******************************************************************************/
#include <portable.h>
#include <mscmess.h>


DEF_MSG( MSG_FEATURE_NOT_SUPP,
    "ERROR: partial-order: Feature not supported: operators within parallel frames\n" );
DEF_MSG( MSG_MESS_WIN_ALT_MIXED, "ERROR: partial-order: messages in 'alt' frames are mixed\n" );

DEF_MSG( MSG_VARIANCE_IN_PO, "ERROR: partial-order: variance in partial order specification\n" );
DEF_MSG( MSG_PATHS_CAN_GENERATE, "REMARK: At least %u paths can be generated\n" );

DEF_MSG( MSG_RELATIVE_TIME_TF_su, "WARNING: semantic: file '%s(%u)': 'relative' time is not ordered\n" );


