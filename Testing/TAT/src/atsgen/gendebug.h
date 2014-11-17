#ifndef GENDEBUG_H
#define GENDEBUG_H
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
* Functions:    main
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  Abstract Test Generator
*
* Notes:        None
*
******************************************************************************/
#include <portable.h>
#include <qmemory.h>
#include <mscbase.h>
#include <xmlconf.h>

extern LOGICAL          MSC_GenDebug( TAT_XML_CONFIG_DEF * configdata, MSC_STORAGE * M );

#endif                         /* GENDEBUG_H */
