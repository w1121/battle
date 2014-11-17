#ifndef TEST_OPT_H
#define TEST_OPT_H
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
* Functions:    MSC_BuildExecutionSuperTree
*               MSC_FreeExecutionSuperTree
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  Behavior tree optimization
*
* Notes:        None
*
******************************************************************************/
#ifdef __cplusplus
extern                  "C"
{
#endif                                 /* C++ */

#include <mscbase.h>
#include <partial.h>
#include <testtree.h>

    extern int              MSC_OptimizeExclude;

    extern struct MSC_TEST_CASE *MSC_BuildExecutionSuperTree( MSC_STORAGE * M, MSC_EVENT_TREE * TM );

    extern LOGICAL          MSC_FreeExecutionSuperTree( struct MSC_TEST_CASE *x );

#ifdef __cplusplus
}

#endif                                 /* C++ */

#endif                                 /* TEST_OPT_H */
