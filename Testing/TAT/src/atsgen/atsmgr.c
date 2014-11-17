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
* Description:  ATS generation manager
*
* Notes:        None
*
******************************************************************************/
#include <portable.h>
#include <mscbase.h>

#include <mscparse.h>
#include <qmemory.h>
#include <strproc.h>
#include <fileproc.h>
#include <cmdline.h>
#include <outmess.h>
#include <cfgtypes.h>
#include <confproc.h>
#include <tatgdata.h>

#include "utrhead.h"

LOGICAL MSC_RegisterATS()
{
    return RESULT_OK;
}

