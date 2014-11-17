#ifndef PARTIALORDER_H
#define PARTIALORDER_H
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
* Functions:    MSC_BuildLinearPart
*               MSC_CleanLinearPart
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  Build partial order from MSC
*
* Notes:        None
*
******************************************************************************/
#include <mscbase.h>
#include <testtree.h>

extern size_t             MSC_SplitCount;

#define MSC_LINEAR_HAS_COREGION 0x00000001
#define MSC_LINEAR_HAS_PAR 0x00000002

/* MSC notation as a tree structure */
typedef struct MSC_EVENT_TREE_ST
{
    MSC_OPERATOR_TYPE       type;      /* type of MSC operator */
    MSC_EVENT_KIND         *oper;      /* pointer to operator which ends linear part */
    size_t                  ev_NumberPr;    /* precalculated number of events */
    size_t                  ev_Number; /* number of events in linear part */
    size_t                  path_Number;    /* number of generated paths */
    size_t                  lflags;    /* MSC_LINEAR_HAS_COREGION, IS_PARALLEL defined if exists */
    MSC_EVENT_KIND        **linear;    /* events of linear part */

    struct MSC_TEST_CASENODE *tests;   /* starting test nodes */
    UINT8_T                *adjacency; /* calculated adjacency matrix */
    size_t                **paths;     /* test paths */
    MSC_STORAGE             *MSC;       /* original MSC */
    int                     expand;    /* 0 - expand operator, 1 - not */
    size_t                  var_Number; /* number of frame */
    size_t                  path_Limit;

    struct MSC_EVENT_TREE_ST **frames; /* pointers to operator frames */
    struct MSC_EVENT_TREE_ST *next;    /* pointer to a following linear part */
    size_t                  path_Limiter;
    size_t                  frame_Counter;
    size_t                 *frame_PathCounter;
    size_t                 *frame_FullCounter;
}
MSC_EVENT_TREE;

/***************************************************************************
 * Function : MSC_BuildLinearPart
 *
 * Description: build tree structure from MSC diagram with partial order on edges
 *
 * Returns: RESULT_ERR / RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
extern LOGICAL          MSC_BuildLinearPart( MSC_STORAGE * M,
    MSC_EVENT_TREE * TM, const UINT32_T timer_Flag, const UINT32_T inter_Flag, const UINT32_T expn_Flag );

/***************************************************************************
 * Function : MSC_CleanLinearPart
 *
 * Description: free allocated memory
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
extern LOGICAL          MSC_CleanLinearPart( MSC_EVENT_TREE * );

#endif                         /* PARTIALORDER_H */
