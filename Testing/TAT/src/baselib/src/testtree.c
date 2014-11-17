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
#include <portable.h>
#include <mscbase.h>
#include <strproc.h>
#include <qmemory.h>
#include <testtree.h>
#include <mscstrs.h>

/***************************************************************************
 * Function : MSC_MergeTreeNode
 *
 * Description: Merge tree nodes w/o remove of duplicates
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_MergeTreeNode( struct MSC_TEST_CASENODE **x1, struct MSC_TEST_CASENODE **x2 )
{
    if ( !x1 || !x2 || !*x2 )
        return RESULT_ERR;

    if ( !*x1 )
    {
        *x1 = *x2;
        *x2 = NULL;
        return RESULT_OK;
    }

    if ( ( *x2 )->next  /* && */
        /* ignore empty nodes */
       /* !(!((*x2)->E) &&  (*x2)->next_Count==1 && !((*x2)->next[0]->E) && (*x2)->next[0]->next_Count==0  )*/
        )
    {

        ( *x1 )->next =
            MEM_Reallocate( ( *x1 )->next,
            ( ( *x1 )->next_Count + ( *x2 )->next_Count ) * sizeof ( struct MSC_TEST_CASENODE * ) );

        memcpy( &( ( *x1 )->next[( *x1 )->next_Count] ), ( ( *x2 )->next ),
            ( *x2 )->next_Count * sizeof ( struct MSC_TEST_CASENODE * ) );
        ( *x1 )->next_Count += ( *x2 )->next_Count;
        MEM_Free( ( *x2 )->next );
    }

    MEM_Free( *x2 );
    *x2 = NULL;
    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_SimplifyTreeNode
 *
 * Description: remove duplicates in tree node
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/

LOGICAL MSC_SimplifyTreeNode( MSC_STORAGE * M, struct MSC_TEST_CASENODE * x )
{
    size_t                    i, j;

    if ( !M || !x )
        return RESULT_ERR;

    if ( x->next_Count )
    {
        i = 0;

        while ( i < x->next_Count - 1 )
        {
            if ( x->next[i]->E && x->next[i]->E->type != MSC_EV_BREAK )
            {
                j = i + 1;

                while ( j < x->next_Count )
                {
                    while ( MSC_CompareEvents( M, x->next[i]->E, x->next[j]->E ) )
                    {
                        if ( !MSC_MergeTreeNode( &( x->next[i] ), &( x->next[j] ) ) )
                            return RESULT_ERR;

                        x->next_Count--;

                        if ( j >= x->next_Count )
                            break;

                        memcpy( &x->next[j], &x->next[j + 1],
                            ( x->next_Count - j ) * sizeof ( struct MSC_TEST_CASENODE * ) );
                    }

                    j++;
                }
            }

            i++;
        }

        for ( i = 0; i < x->next_Count; i++ )
        {
            if ( !MSC_SimplifyTreeNode( M, x->next[i] ) )
                return RESULT_ERR;
        }
    }

    return RESULT_OK;
}
