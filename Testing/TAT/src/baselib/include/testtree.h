#ifndef TESTTREE_H
#define TESTTREE_H
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
* Functions:    MSC_MergeTreeNode
*               MSC_SimplifyTreeNode
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  Behaviour tree bulder
*
* Notes:        None
*
******************************************************************************/
#ifdef __cplusplus
extern                  "C"
{
#endif                                 /* C++ */

#include <mscbase.h>

    struct MSC_TEST_CASENODE
    {
        MSC_EVENT_KIND             *E;     /* what we should encode */
        size_t                     next_Count; /* how many alternative */

        struct MSC_TEST_CASENODE **next;    /* alternatives */
        size_t                     tag;   /* special tag */
        size_t                    brc;   /* branch counter */
    };

    struct MSC_TEST_CASE
    {

        struct MSC_TEST_CASENODE *toller;
        size_t                    branch_Count;
        size_t                   *tags;

        struct MSC_TEST_CASE  **branch;
        struct MSC_TEST_CASE   *next;
    };               

    extern LOGICAL          MSC_MergeTreeNode( struct MSC_TEST_CASENODE **x1, struct MSC_TEST_CASENODE **x2 );

    extern LOGICAL          MSC_SimplifyTreeNode( MSC_STORAGE * M, struct MSC_TEST_CASENODE *x );

#ifdef __cplusplus
}

#endif                                 /* C++ */

#endif                                 /* TESTTREE_H */
