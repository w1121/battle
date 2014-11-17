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

#include <portable.h>
#include <mscbase.h>

#include <qmemory.h>
#include <partial.h>
#include <genpath.h>
#include <outmess.h>
#include <testtree.h>
#include <mscmess.h>
#include "testopt.h"

int                     MSC_OptimizeExclude = 0;

static int              tat_Tag;

/***************************************************************************
 * Function : set_Tag
 *
 * Description: mark all events with specific tag
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static void set_Tag( struct MSC_TEST_CASENODE *x, int tag )
{
    size_t                    i;

    if ( !x )
        return;

    x->tag = tag;

    i = x->next_Count;

    if ( i )
        do
        {
            i--;
            set_Tag( x->next[i], tag );
        }
        while ( i );
}

/***************************************************************************
 * Function : fill_Tree
 *
 * Description: recursively process MSC linear part and combine 'alt'
 *              expressions
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static int fill_Tree( MSC_STORAGE * M, MSC_EVENT_TREE * TM, struct MSC_TEST_CASE *MT, int p )
{
    size_t                    i;
    int                     c_tag;

    if ( !MT )
        return RESULT_ERR;

    MT->toller = TM->tests;     /* copy original tree */

/* copy sequential linear lists */
    while ( TM->type == MSC_OP_SEQ && TM->next )
    {
        MT->next = MEM_TYPE_ALLOC( struct MSC_TEST_CASE );

        MT = MT->next;
        TM = TM->next;
        MT->toller = TM->tests;
    };

    if ( TM->type != MSC_OP_SEQ && TM->type != MSC_OP_PAR && p )
    {
        MT->next = MEM_TYPE_ALLOC( struct MSC_TEST_CASE );

        MT = MT->next;
    };

    switch ( TM->type )
    {
    case MSC_OP_ALT:
    {
        if ( TM->frames )
        {
            if ( TM->frames[0] && ( TM->frames[0]->next || TM->frames[0]->frames ) )
            {

            /* set unique tag */
                c_tag = ++tat_Tag;
                set_Tag( TM->frames[0]->tests, c_tag );
            /* save as tagged branch */
                MT->tags = MEM_Reallocate( MT->tags, ( MT->branch_Count + 1 ) * sizeof ( MT->tags[0] ) );
                MT->tags[MT->branch_Count] = c_tag;
                MT->branch = MEM_Reallocate( MT->branch, ( MT->branch_Count + 1 ) * sizeof ( MT->branch[0] ) );

                MT->branch[MT->branch_Count] = MEM_TYPE_ALLOC( struct MSC_TEST_CASE );

            /* process branch recursevely */
                fill_Tree( M, TM->frames[0], MT->branch[MT->branch_Count], 0 );

            /* make toller empty */
                if ( TM->frames[0]->next && !TM->frames[0]->frames )
                {
                    MT->branch[MT->branch_Count]->toller = MEM_TYPE_ALLOC( struct MSC_TEST_CASENODE );

                    set_Tag( MT->branch[MT->branch_Count]->toller, c_tag );
                }

                if ( TM->frames[0]->tests->next_Count == 1 &&
                    TM->frames[0]->tests->next[0]->E == NULL &&
                    TM->frames[0]->tests->next[0]->next_Count == 0 &&
                    !( TM->frames[0]->var_Number == 1 && TM->frames[0]->frames[0]->tests &&
                        TM->frames[0]->frames[0]->tests->next && TM->frames[0]->frames[0]->tests->next[0]->E
                        && TM->frames[0]->frames[0]->tests->next[0]->E->type == MSC_EV_START_LOOP ) )
                {
                    TM->frames[0]->tests = MT->branch[MT->branch_Count]->toller;
                    MEM_Free( TM->frames[0]->frames );
                    TM->frames[0]->frames = NULL;
                    TM->frames[0]->var_Number = 0;
                    set_Tag( TM->frames[0]->tests, c_tag );
                /* set_Tag( TM->frames[0]->next->tests, c_tag ); */
                    memset( MT->branch[MT->branch_Count], 0, sizeof ( MT->branch[MT->branch_Count][0] ) );
                    if ( TM->frames[0]->next )
                        fill_Tree( M, TM->frames[0]->next, MT->branch[MT->branch_Count], 0 );
                }
                MT->branch_Count++;
            }

            for ( i = 1; i < TM->var_Number; i++ )
            {
            /* if there are possible branches within this frame */

                if ( TM->frames[i] && ( TM->frames[i]->next || TM->frames[i]->frames ) )
                {
                /* set unique tag */
                    c_tag = ++tat_Tag;
                    set_Tag( TM->frames[i]->tests, c_tag );
                /* save as tagged branch */
                    MT->tags = MEM_Reallocate( MT->tags, ( MT->branch_Count + 1 ) * sizeof ( MT->tags[0] ) );
                    MT->tags[MT->branch_Count] = c_tag;
                    MT->branch =
                        MEM_Reallocate( MT->branch, ( MT->branch_Count + 1 ) * sizeof ( MT->branch[0] ) );

                    MT->branch[MT->branch_Count] = MEM_TYPE_ALLOC( struct MSC_TEST_CASE );

                /* process branch recursevely */
                    fill_Tree( M, TM->frames[i], MT->branch[MT->branch_Count], 0 );
                /* make toller empty */

                    if ( TM->frames[i]->next && !TM->frames[i]->frames )
                    {
                        MT->branch[MT->branch_Count]->toller = MEM_TYPE_ALLOC( struct MSC_TEST_CASENODE );

                        set_Tag( MT->branch[MT->branch_Count]->toller, c_tag );
                    };

                    if ( TM->frames[i]->tests->next_Count == 1 &&
                        TM->frames[i]->tests->next[0]->E == NULL &&
                        TM->frames[i]->tests->next[0]->next_Count == 0 &&
                        !( TM->frames[i]->var_Number == 1 && TM->frames[i]->frames[0]->tests &&
                            TM->frames[i]->frames[0]->tests->next &&
                            TM->frames[i]->frames[0]->tests->next[0]->E &&
                            TM->frames[i]->frames[0]->tests->next[0]->E->type == MSC_EV_START_LOOP ) )
                    {
                        TM->frames[i]->tests = MT->branch[MT->branch_Count]->toller;
                        MEM_Free( TM->frames[i]->frames );
                        TM->frames[i]->frames = NULL;
                        TM->frames[i]->var_Number = 0;
                        set_Tag( TM->frames[i]->tests, c_tag );
                    /* set_Tag( TM->frames[i]->next->tests, c_tag ); */
                        memset( MT->branch[MT->branch_Count], 0, sizeof ( MT->branch[MT->branch_Count][0] ) );
                        if ( TM->frames[i]->next )
                            fill_Tree( M, TM->frames[i]->next, MT->branch[MT->branch_Count], 0 );
                    }

                    MT->branch_Count++;
                }

            /* bind nodes together */
                if ( TM->frames[i] )
                    MSC_MergeTreeNode( &( TM->frames[0]->tests ), &( TM->frames[i]->tests ) );
            };

            MSC_SimplifyTreeNode( M, TM->frames[0]->tests );
            MT->toller = TM->frames[0]->tests;
        }

        break;
    };

    case MSC_OP_OPT:
    case MSC_OP_PAR:
    case MSC_OP_EXC:
    case MSC_OP_LOOP:
    {
        MSG_PrintError( MSG_INTERNAL_PROBLEM );
        return RESULT_ERR;
    }
    };

    if ( TM->next )
    {
        MT->next = MEM_TYPE_ALLOC( struct MSC_TEST_CASE );

        if ( !fill_Tree( M, TM->next, MT->next, 1 ) )
            return RESULT_ERR;
    }

    return RESULT_OK;

}

/***************************************************************************
 * Function : break_TM
 *
 * Description: Free allocated memory
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static MSC_EVENT_KIND        MSC_EV_BREAK_EV;
static MSC_EVENT_TREE  *break_TM(  )
{
    MSC_EVENT_TREE         *x;

    x = MEM_TYPE_ALLOC( MSC_EVENT_TREE );

    x->tests = MEM_TYPE_ALLOC( struct MSC_TEST_CASENODE );

    x->type = MSC_OP_SEQ;
/*    x->tests->E=MEM_TYPE_ALLOC(MSC_EVENT_KIND);
   x->tests->E->type=MSC_EV_BREAK; */
    x->tests->next_Count = 1;

    x->tests->next = MEM_TYPE_ALLOC( struct MSC_TEST_CASENODE * );
    x->tests->next[0] = MEM_TYPE_ALLOC( struct MSC_TEST_CASENODE );

    x->tests->next[0]->E = &MSC_EV_BREAK_EV;
    return x;
}

/***************************************************************************
 * Function : add_end
 *
 * Description: Free allocated memory
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static int add_end( MSC_EVENT_TREE * TM, MSC_EVENT_TREE * T )
{
    if ( !TM )
        return RESULT_ERR;

    while ( TM->next )
        TM = TM->next;

    TM->next = T;

    return RESULT_OK;
}

/***************************************************************************
 * Function : end_node
 *
 * Description: Free allocated memory
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static int end_node( struct MSC_TEST_CASENODE *x )
{
    size_t                    i;

    for ( i = 0; i < x->next_Count; i++ )
        end_node( x->next[i] );

    if ( !x->next_Count )
    {
        x->next_Count = 1;
        x->next = MEM_TYPE_ALLOC( struct MSC_TEST_CASENODE * );
        x->next[0] = MEM_TYPE_ALLOC( struct MSC_TEST_CASENODE );

        x->next[0]->E = &MSC_EV_BREAK_EV;
    };

    return 1;
}

/***************************************************************************
 * Function : set_EndEvents
 *
 * Description: mark finish events
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static int set_EndEvents( MSC_EVENT_TREE * TM )
{
    if ( !TM )
        return RESULT_ERR;

    while ( TM->next )
        TM = TM->next;

    end_node( TM->tests );

    return RESULT_OK;
}

/***************************************************************************
 * Function : equiv_Tree
 *
 * Description: convert inline expression to equivalent 'alt' expressions
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static void equiv_Tree( MSC_EVENT_TREE * TM )
{
    size_t                    i;

    if ( !TM )
        return;

    if ( TM->tests == NULL )
    {
        TM->tests = MEM_TYPE_ALLOC( struct MSC_TEST_CASENODE );

        TM->tests->next_Count = 1;
        TM->tests->next = MEM_TYPE_ALLOC( struct MSC_TEST_CASENODE * );
        TM->tests->next[0] = MEM_TYPE_ALLOC( struct MSC_TEST_CASENODE );
    }

    if ( TM->type == MSC_OP_OPT )
    {
        TM->type = MSC_OP_ALT;
        TM->var_Number++;
        TM->frames = MEM_Reallocate( TM->frames, TM->var_Number * sizeof ( TM->frames[0] ) );

        TM->frames[1] = MEM_TYPE_ALLOC( MSC_EVENT_TREE );
        TM->frames[1]->type = MSC_OP_SEQ;
    }
    else if ( TM->type == MSC_OP_EXC )
    {
        TM->type = MSC_OP_ALT;
        TM->var_Number++;

        TM->frames = MEM_Reallocate( TM->frames, TM->var_Number * sizeof ( TM->frames[0] ) );

        if ( MSC_OptimizeExclude )
            set_EndEvents( TM->frames[0] );
        else
            add_end( TM->frames[0], break_TM(  ) );

        TM->frames[1] = MEM_TYPE_ALLOC( MSC_EVENT_TREE );

        TM->frames[1]->type = MSC_OP_SEQ;
    }
    else if ( TM->type == MSC_OP_LOOP )
    {
        TM->type = MSC_OP_ALT;
    };

    for ( i = 0; i < TM->var_Number; i++ )
        equiv_Tree( TM->frames[i] );

/* bind alternatives */
    if ( TM->type == MSC_OP_ALT )
    {
        for ( i = 0; i < TM->var_Number; i++ )
        {
            if ( TM->frames[i] && TM->frames[i]->tests == NULL && TM->frames[i]->var_Number &&
                TM->frames[i]->next == NULL )
            {
                TM->frames =
                    MEM_Reallocate( TM->frames,
                    ( TM->var_Number + TM->frames[i]->var_Number ) * sizeof ( TM->frames[0] ) );
                memcpy( &( TM->frames[TM->var_Number] ), TM->frames[i]->frames,
                    TM->frames[i]->var_Number * sizeof ( TM->frames[0] ) );
                TM->var_Number += TM->frames[i]->var_Number;
                TM->frames[i]->var_Number = 0;
                MEM_Free( TM->frames[i]->frames );
                TM->frames[i]->frames = NULL;
            };

        }
    }

    equiv_Tree( TM->next );
}

/***************************************************************************
 * Function : MSC_BuildExecutionSuperTree
 *
 * Description: Build consolidated execution tree
 *
 * Returns: pointer to root node
 *
 * Notes: None
 *
 **************************************************************************/
struct MSC_TEST_CASE   *MSC_BuildExecutionSuperTree( MSC_STORAGE * M, MSC_EVENT_TREE * TM )
{

    struct MSC_TEST_CASE   *MT;

    if ( !M )
        return NULL;

    if ( !TM )
        return NULL;

    equiv_Tree( TM );           /* remove MSC_OP_OPT & MSC_OP_EXC */

    MT = MEM_TYPE_ALLOC( struct MSC_TEST_CASE );
    tat_Tag = 0;

/*     MT->toller = TM->tests; */
    MSC_EV_BREAK_EV.type = MSC_EV_BREAK;

    if ( !fill_Tree( M, TM, MT, 1 ) )
    {
        MEM_Free( MT );
        return NULL;
    };

    return MT;
}

/***************************************************************************
 * Function : MSC_FreeExecutionSuperTree
 *
 * Description: Free allocated memory
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_FreeExecutionSuperTree( struct MSC_TEST_CASE * x )
{
    size_t                    i;

    if ( !x )
        return RESULT_OK;

    for ( i = 0; i < x->branch_Count; i++ )
        MSC_FreeExecutionSuperTree( x->branch[i] );

    if ( x->branch )
        MEM_Free( x->branch );

    if ( x->tags )
        MEM_Free( x->tags );

    MSC_FreeExecutionSuperTree( x->next );
    MEM_Free( x );
    return RESULT_OK;
}
