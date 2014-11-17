#ifndef MSCCHK_H
#define MSCCHK_H
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
* Functions:    MSC_CheckEvents
*               MSC_ReduceEvents
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  MSC checking
*
* Notes:        None
*
******************************************************************************/
#include <mscbase.h>
#include <testtree.h>
#include <xmlconf.h>

#ifdef __cplusplus
extern                  "C"
{
#endif                                 /* C++ */

    /***************************************************************************
     * Function : MSC_CheckEvents
     *
     * Description: check messages to be correct
     *
     * Returns:
     *
     * Notes:
     *
     **************************************************************************/
    extern int              MSC_CheckEvents( MSC_STORAGE * M,
        MSC_EVENT_TREE * TM, TAT_XML_CONFIG_DEF * d );

    /***************************************************************************
     * Function : MSC_ReduceEvents
     *
     * Description: merge couple of events with label/comment saving
     *
     * Returns: RESULT_ERR/RESULT_OK
     *
     * Notes:
     *
     **************************************************************************/
    extern LOGICAL          MSC_ReduceEvents( MSC_STORAGE * M, int flags );

#ifdef __cplusplus
}

#endif                                 /* C++ */

#endif                                 /* MSCCHK_H */
