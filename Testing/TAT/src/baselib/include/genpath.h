#ifndef GENPATH_H
#define GENPATH_H
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
* Functions:    MSC_ResearchPaths
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  Generate path according partial-ordering graph
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

    typedef enum
    {
        MSC_ACTIVE,
        MSC_PASSIVE,
        MSC_DELETED
    }
    MSC_EVENT_CLASS;

    typedef                 size_t ( *MSC_PATH_SCHEDULE_FUNC ) ( MSC_EVENT_TREE *,
        size_t path_len, size_t * path, size_t ev_select, size_t *ev_nums );

    typedef struct
    {
        int                     flag_ActiveFirst;   /* active events has higher priority */
        int                     flag_PassiveFirst;  /* passive events has higher priority */
        int                     flag_LimitActive;   /* restruct number of active events per branch */
        int                     flag_LimitPassive;  /* restruct number of passive events per branch */
        int                     flag_ActionNonInterleave;   /* action events has higher priority */
        int                     flag_ConditionNonInterleave;    /* condition events has higher priority */
        int                     flag_GenerateTree;  /* generate tree instead of traces */
        int                     flag_CountTracesOnly;   /* do not generate, just calculate */
    }
    MSC_GENPATH_PARAM;

#define GP_NONE                 0x00000000
#define GP_FILTER_SR_EVENTS     0x00000001
#define GP_FILTER_CONDITIONS    0x00000002
#define GP_FILTER_ESR_EVENTS    0x00000004
#define GP_FILTER_TIMERS        0x00000008
#define GP_FILTER_GEN_COND      0x00000010
#define GP_FILTER_OUT_MESS      0x00000020
#define GP_FILTER_IN_MESS       0x00000040
#define GP_SAVE_ENVSELF         0x00000080

/* Generate specified number of test paths for all linear parts:
   TM - pointer to MSC as a tree structure,
   numb - number of paths for every linear part
   tr - if set - build TEST tree, otherwise generate planar paths 
 */
    extern LOGICAL          MSC_ResearchPaths( MSC_EVENT_TREE * TM, MSC_GENPATH_PARAM params, size_t path_num );

/* Filter paths of linear parts to avoid similiar MSC generation:
   M - pointer to loaded MSC,
   T - pointer to MSC as a tree structure,
   reduce - 1 - remove extra events.
   insys - 0 - in message for system, 1 - out message for system, 
   inenv - 0 - in message for environment, 1 - out message for environment
 */

    extern LOGICAL          MSC_ReduceEventTree( const MSC_STORAGE * M,
        MSC_EVENT_TREE * T, int reduce, int insys, int inenv );

    extern LOGICAL          MSC_ReduceEventTreeSigIgn( const MSC_STORAGE * M,
        MSC_EVENT_TREE * T, int reduce, int insys, int inenv, size_t scount, char *sname[] );

#ifdef __cplusplus
}

#endif                                 /* C++ */

#endif                                 /* GENPATH_H */
