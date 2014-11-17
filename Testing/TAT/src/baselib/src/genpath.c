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

#include <portable.h>
#include <qmemory.h>
#include <mscbase.h>
#include <partial.h>
#include <genpath.h>
#include <outmess.h>
#include <testtree.h>
#include <msctime.h>
#include <mscgen.h>
#include <mscmess.h>
#include <mscstrs.h>
#include <tatgdata.h>

static size_t             linear_Vertexes;    /* number of events in a linear part */
static size_t            *current_Trace; /* concrete test path */
static size_t           **linear_PathStorage; /* storage for paths */
static UINT8_T   *linear_Semaphores;  /* working array */
static UINT8_T   *linear_AdjacencyMatrix; /* adjacency matrix */
static size_t             linear_PathCounter; /* number of generated test paths */
static size_t             linear_PathLimit;   /* maximal number of paths to generate */
static size_t            *linear_EventMap;    /* ordered vertexes by increasing topological level */
static size_t            *linear_TopLevel;    /* topological levels of vertexes */
static MSC_EVENT_TREE  *current_Linear; /* current event tree */

static MSC_EVENT_CLASS *event_Types;

static MSC_GENPATH_PARAM process_Params;

/* triangle matrix for storing possible events on each nested level */
static size_t            *stack_Buffer;

static MSC_PATH_SCHEDULE_FUNC schedule_Function = NULL;

/***************************************************************************
 * Function : tat_AddPath
 *
 * Description: add current PATH(trace) to tree
 *
 * Returns: none
 *
 * Notes: none
 *
 **************************************************************************/
static void tat_AddPath( struct MSC_TEST_CASENODE *x )
{
    register size_t  i;         /* depth counter */
    register size_t  k;         /* loop counter */
    MSC_EVENT_KIND  *z;

    i = 0;

    while ( i < linear_Vertexes )
    {
    /* get event */
        z = current_Linear->linear[current_Trace[i]];
    /* look for existing branches (backward) */
    /* the probability to use here last element is higher than others */
    /* it may be possible to check only last element (TBD) */
        k = x->next_Count;

        while ( k )
        {
            k--;

            if ( x->next[k]->E == z )
                goto end_;
        }

    /* add new branch */
        k = x->next_Count++;
        /* use optimized version (??) */
        x->next = MEM_AddObject( x->next, x->next_Count, sizeof ( struct MSC_TEST_CASENODE * ) );
        x->next[k] = MEM_TYPE_ALLOC( struct MSC_TEST_CASENODE );

        ( x->next[k] )->E = z;

      end_:
        x = x->next[k];

        i++;
    }
}

/***************************************************************************
 * Function : tat_ScheduleEvent
 *
 * Description: order events using specified rules
 *
 * Returns: number of actual events
 *
 * Notes: None
 *
 **************************************************************************/
static size_t tat_ScheduleEvent( MSC_EVENT_TREE * current_linear,
    size_t path_len, size_t * path, size_t ev_select, size_t * ev_nums )
{
    size_t                  i, active = 0, passive = 0;
    int                     action = 0, condition = 0;
    MSC_EVENT_KIND              *ev;

/* passive - timer or receive */

    for ( i = 0; i < ev_select; i++ )
    {
        event_Types[i] = MSC_ACTIVE;
        ev = current_linear->linear[ev_nums[i]];

        if ( ( MSC_IsPassiveEvent( ev ) ) || ( MSC_IsTimerEvent( ev ) ) )
        {
            event_Types[i] = MSC_PASSIVE;
            passive++;
        }

        if ( ev->type == MSC_EV_ACTION )
            action++;

        if ( ev->type == MSC_EV_CONDITION )
            condition++;
    }

    active = ev_select - passive;

    if ( process_Params.flag_ActionNonInterleave && action && ( active > 1 || passive > 0 ) )
    {
        action = 1;

        for ( i = 0; i < ev_select; i++ )
        {
            ev = current_linear->linear[ev_nums[i]];

            if ( ev->type != MSC_EV_ACTION || action <= 0 )
            {
                ev_nums[i] = MSC_INFINITY_CONST;
                event_Types[i] = MSC_DELETED;
            }

            if ( ev->type == MSC_EV_ACTION )
                action--;
        };

        passive = 0;
        active = 1;
        action = 1;
        condition = 0;
    }

    if ( process_Params.flag_ConditionNonInterleave && condition )
    {
        condition = 1;

        for ( i = 0; i < ev_select; i++ )
        {
            ev = current_linear->linear[ev_nums[i]];

            if ( ev->type != MSC_EV_CONDITION || ( condition <= 0 ) )
            {
                ev_nums[i] = MSC_INFINITY_CONST;
                event_Types[i] = MSC_DELETED;
            }

            if ( ev->type == MSC_EV_CONDITION )
                condition--;
        };

        passive = 0;
        active = 1;
        action = 0;
        condition = 1;
    }

    if ( process_Params.flag_ActiveFirst && active && passive )
    {
        for ( i = 0; i < ev_select; i++ )
        {
            if ( event_Types[i] == MSC_PASSIVE )
            {
                ev_nums[i] = MSC_INFINITY_CONST;
                event_Types[i] = MSC_DELETED;
            }
        }

        passive = 0;
    }

    if ( process_Params.flag_PassiveFirst && active && passive )
    {
        for ( i = 0; i < ev_select; i++ )
        {
            if ( event_Types[i] == MSC_ACTIVE )
            {
                ev_nums[i] = MSC_INFINITY_CONST;
                event_Types[i] = MSC_DELETED;
            }
        }

        active = 0;
    }

    if ( process_Params.flag_LimitActive && active > ( size_t ) process_Params.flag_LimitActive )
    {
        active = 0;
        for ( i = 0; i < ev_select; i++ )
        {
            if ( event_Types[i] == MSC_ACTIVE )
            {
                if ( active >= (size_t) process_Params.flag_LimitActive )
                {
                    ev_nums[i] = MSC_INFINITY_CONST;
                    event_Types[i] = MSC_DELETED;
                }
                active++;
            }
        }

        active = process_Params.flag_LimitActive;
    }

    if ( process_Params.flag_LimitPassive && passive > ( size_t ) process_Params.flag_LimitPassive )
    {
        passive = 0;
        for ( i = 0; i < ev_select; i++ )
        {
            if ( event_Types[i] == MSC_PASSIVE )
            {
                if ( passive >= (size_t) process_Params.flag_LimitPassive )
                {
                    ev_nums[i] = MSC_INFINITY_CONST;
                    event_Types[i] = MSC_DELETED;
                }
                passive++;
            }
        }

        passive = process_Params.flag_LimitPassive;
    }

    ev_select = active + passive;
    return ev_select;
}

/***************************************************************************
 * Function : tat_GenPathSchedule
 *
 * Description: generate paths
 *
 * Return: none
 *
 * Notes:
 *
 **************************************************************************/
static void _FAST tat_GenPathSchedule( size_t len, size_t mlen )
{
    size_t                    i, k, v, w, j, pb, t;
    size_t                    *pbuffer;

    pbuffer = &stack_Buffer[( ( len * ( ( linear_Vertexes << 1 ) - len + 1 ) ) >> 1 )];

/* check for paths overflow */

    if ( !process_Params.flag_GenerateTree && linear_PathCounter >= linear_PathLimit )
        return;

    pb = 0;
    t = mlen++;
/* select possible continuations of trace */
    for ( i = 0; i < linear_Vertexes && ( linear_TopLevel[w = linear_EventMap[i]] <= mlen ); i++ )
    {
        if ( !linear_Semaphores[w] )    /* the vertex hasn't been already used */
        {
        /* look for possibility to use this event */
            k = 0;
            v = w + ( linear_Vertexes * ( linear_Vertexes - 1 ) );
            j = linear_Vertexes;

        /* check for availability of whole trace */
            while ( j )
            {
                j--;

                if ( !linear_Semaphores[j] && linear_AdjacencyMatrix[v] )
                {
                    k = 1;
                    break;
                }

                v -= linear_Vertexes;
            }

        /* add possible element to list */
            if ( !k )
                pbuffer[pb++] = w;
            if ( t < linear_TopLevel[w] )
                t = linear_TopLevel[w];
        }
    }

/* filtering continuations using rules */
    k = schedule_Function( current_Linear, len, current_Trace, pb, pbuffer );

    if ( len != ( linear_Vertexes - 1 ) )
    {
        for ( i = 0; i < pb; i++ )
        {
            if ( pbuffer[i] == MSC_INFINITY_CONST )
                continue;

            w = pbuffer[i];
            current_Trace[len] = w;

        /* mark next vertex as used */
            linear_Semaphores[w] = 1;

        /* and generate next vertex */
            tat_GenPathSchedule( len + 1, t );
            linear_Semaphores[w] = 0;
        }
    }
    else
    {
        for ( i = 0; i < pb; i++ )
        {
            if ( pbuffer[i] == MSC_INFINITY_CONST )
                continue;

            w = pbuffer[i];

            current_Trace[len] = w;
            linear_PathCounter++;

        /* save path */
            if ( process_Params.flag_GenerateTree )
            {
                tat_AddPath( current_Linear->tests );
            }
            else
            {
                if ( !process_Params.flag_CountTracesOnly )
                {
                /* reallocate memory dynamically */
                    linear_PathStorage =
                        MEM_AddObject( linear_PathStorage, linear_PathCounter,
                        sizeof ( linear_PathStorage[0] ) );
                    linear_PathStorage[linear_PathCounter - 1] =
                        MEM_Copy( current_Trace, sizeof ( current_Trace[0] ) * linear_Vertexes );
                }

                if ( linear_PathCounter >= linear_PathLimit )
                    return;
            }
        }
    }
    return;
}

/***************************************************************************
 * Function : tat_Partition
 *
 * Description: select pivot and exchange with 1st element
 *
 * Returns: pivot
 *
 * Notes: None
 *
 **************************************************************************/
static size_t tat_Partition( size_t * a, size_t lb, size_t ub, size_t * index )
{
    size_t                    t, pivot;
    size_t                    i, j, p;

/* select pivot and exchange with 1st element */
    p = lb + ( ( ub - lb ) >> 1 );
    pivot = a[p];
    a[p] = a[lb];

/* sort lb+1..ub based on pivot */
    i = lb + 1;
    j = ub;

    for ( ;; )
    {
        while ( i < j && ( index[pivot] > index[a[i]] ) )
            i++;

        while ( j >= i && ( index[a[j]] > index[pivot] ) )
            j--;

        if ( i >= j )
            break;

        t = a[i];
        a[i] = a[j];
        a[j] = t;
        j--;
        i++;
    }

/* pivot belongs in a[j] */
    a[lb] = a[j];
    a[j] = pivot;
    return j;
}

/***************************************************************************
 * Function : tat_InsertSort
 *
 * Description: Insert-sort of a[lb..ub], using index as value source
 *
 * Returns: sorted array
 *
 * Notes: None
 *
 **************************************************************************/
static void tat_InsertSort( size_t * a, size_t lb, size_t ub, size_t * index )
{
    size_t                    t;
    size_t                    i, j;

    for ( i = lb + 1; i <= ub; i++ )
    {
        t = a[i];

    /* Shift elements down until */
    /* insertion point found.    */

        for ( j = i - 1; (int)j >= (int)lb && ( index[a[j]] > index[t] ); j-- )
            a[j + 1] = a[j];

    /* Insert */
        a[j + 1] = t;
    }
}

/***************************************************************************
 * Function : tat_QuickSort
 *
 * Description: Quick-sort of a[lb..ub], using index as value source
 *
 * Returns: sorted array
 *
 * Notes: None
 *
 **************************************************************************/
static void tat_QuickSort( size_t * a, size_t lb, size_t ub, size_t * index )
{
    size_t                     m;

    while ( (int)lb < (int)ub )
    {
    /* quickly sort short lists */

        if ( ub - lb <= 12 )
        {
            tat_InsertSort( a, lb, ub, index );
            return;
        }

    /* partition into two segments */
        m = tat_Partition( a, lb, ub, index );

    /* sort the smallest partition    */
    /* to minimize stack requirements */
        if ( m - lb <= ub - m )
        {
            tat_QuickSort( a, lb, m - 1, index );
            lb = m + 1;
        }
        else
        {
            tat_QuickSort( a, m + 1, ub, index );
            ub = m - 1;
        }
    }
}

/***************************************************************************
 * Function : tat_TopologicalSort
 *
 * Description: Topological sort of partial-ordering graph, defined
 *              by adjacency matrix Adj and number of vertexes
 *
 * Returns: Array with levels for each vertex
 *
 * Notes: None
 *
 **************************************************************************/
static size_t       *tat_TopologicalSort( UINT8_T * adj, size_t vertex )
{
    UINT8_T                   *ii;
    int                    *incident, level;
    size_t                   *ev_level;
    size_t                    i, j, k, new_set, l;

    incident = MEM_Allocate( ( vertex * sizeof ( incident[0] ) ) << 1 );
    ev_level = MEM_Allocate( vertex * sizeof ( ev_level[0] ) );

/* calculate initial incidents array */
    for ( i = 0; i < vertex; i++ )
    {
        ii = adj + i;
        level = 0;
        j = vertex;
        while ( j )
        {
            level += *ii;
            ii += vertex;
            j--;
        }
        incident[i] = level;
    }
/* initialize topological sort by levels */
    level = 1;
    k = vertex;
    while ( k )
    {
        new_set = 0;
    /* find input vertexes of next level */
        for ( i = 0; i < vertex; i++ )
        {
            if ( incident[i] == 0 )
            {
                incident[i] = -1;
                ev_level[i] = ( size_t ) level;
                incident[new_set + vertex] = (int)i;
                new_set++;
                k--;
            }
        }

        if ( !k )
            break;

    /* exclude set of input vertexes */
        for ( i = vertex; i < vertex + new_set; i++ )
        {
            l = incident[i] * vertex;
            for ( j = 0; j < vertex; j++ )
            {
                incident[j] -= ( int ) adj[l + j];
            }
        }
        level++;
    }

    MEM_Free( incident );
    return ev_level;
}

/***************************************************************************
 * Function : tat_PartialReoder
 *
 * Description: generate sequence of vertexes based on topological levels
 *
 * Returns: pointer to sequence
 *
 * Notes: None
 *
 **************************************************************************/
static size_t            *tat_PartialReoder( size_t * tsort, size_t vertex )
{
    size_t                   *col, i;

    col = MEM_Allocate( vertex * sizeof ( col[0] ) );

/* fill indexes */
    for ( i = 0; i < vertex; i++ )
        col[i] = i;

    tat_QuickSort( col, 0, vertex - 1, tsort );
    return col;
}

static                  DEF_MSG( EVENT_N, "EVENT N %d: %s\n" );

static char             dest[TAT_MAX_LINE_LEN];

/***************************************************************************
 * Function : tat_GenLinearPaths
 *
 * Description: generate paths
 *
 * Return: none
 *
 * Notes:
 *
 **************************************************************************/
static size_t tat_GenLinearPaths( MSC_EVENT_TREE * TM, size_t max_paths )
{
    size_t                   *col, *tsort;
    size_t                    i, j, ev_start;

/* check input parameters */

    if ( TM->ev_Number == 0 )
        return 1;

    if ( TM->adjacency == NULL )
    {
        TM->tests = NULL;
        TM->paths = 0;
        TM->path_Number = 0;
        return RESULT_ERR;
    }

/* storage for single path */
    current_Trace = MEM_Allocate( TM->ev_Number * sizeof ( current_Trace[0] ) );
    linear_PathStorage = NULL;

    if ( !process_Params.flag_CountTracesOnly )
    {
        if ( process_Params.flag_GenerateTree )
        {
            TM->tests = MEM_TYPE_ALLOC( struct MSC_TEST_CASENODE );
        }
    }

/* memory allocation */

    linear_Semaphores = ( unsigned char * ) MEM_Allocate( TM->ev_Number * sizeof ( linear_Semaphores[0] ) );
    stack_Buffer = MEM_Allocate( TM->ev_Number * TM->ev_Number * sizeof ( stack_Buffer[0] ) );
    event_Types = ( MSC_EVENT_CLASS * ) MEM_Allocate( TM->ev_Number * sizeof ( event_Types[0] ) );

    schedule_Function = tat_ScheduleEvent;

/* initialization */
    current_Linear = TM;

    linear_Vertexes = TM->ev_Number;
    linear_PathCounter = 0;
    linear_PathLimit = max_paths;

    linear_AdjacencyMatrix = TM->adjacency;
/* check transitive closing for inconsistency detection */
/* we use Adjmatr instead of AA because we want to get original
   events numbers */
    for ( i = 0; i < linear_Vertexes; i++ )
    {
        for ( j = i + 1; j < linear_Vertexes; j++ )
            if ( linear_AdjacencyMatrix[i * linear_Vertexes + j] &&
                linear_AdjacencyMatrix[j * linear_Vertexes + i] )
            {
                MSG_PrintError( MSG_VARIANCE_IN_PO );
                MSC_GenerateLine( dest, TM->linear[i], TM->MSC,
                    MSC_MPR_VERBOSE_OUT | MSC_MPR_OUTPUT_DIAG | MSC_MPR_OUTPUT_CALCTIME );
                MSG_PrintError( EVENT_N, i, dest );
                MSC_GenerateLine( dest, TM->linear[j], TM->MSC,
                    MSC_MPR_VERBOSE_OUT | MSC_MPR_OUTPUT_DIAG | MSC_MPR_OUTPUT_CALCTIME );
                MSG_PrintError( EVENT_N, j, dest );
                return RESULT_ERR;
            }
    }
    tsort = tat_TopologicalSort( linear_AdjacencyMatrix, linear_Vertexes );
    col = tat_PartialReoder( tsort, linear_Vertexes );
    linear_EventMap = col;
    linear_TopLevel = tsort;

    if ( linear_Vertexes == 1 )
    {
        current_Trace[0] = 0;

        linear_PathStorage =
            MEM_AddObject( linear_PathStorage, linear_PathCounter + 1, sizeof ( linear_PathStorage[0] ) );
        linear_PathStorage[linear_PathCounter++] = MEM_Copy( current_Trace, sizeof ( current_Trace[0] ) );

        if ( process_Params.flag_GenerateTree )
        {
            tat_AddPath( current_Linear->tests );
        }
    }
    else
    {
        ev_start = 0;
        i = 0;
    /* Input elements have minimal topological level == 1 */
        while ( ( i < linear_Vertexes ) && ( tsort[col[i]] == 1 ) )
        {
        /* add event to list of possible initiators */
            stack_Buffer[ev_start++] = col[i];
            i++;
        }

        j = schedule_Function( current_Linear, 0, current_Trace, ev_start, stack_Buffer );

        if ( j )
        {
            for ( i = 0; i < ev_start; i++ )
            {
                if ( stack_Buffer[i] == MSC_INFINITY_CONST )
                    continue;

            /* initialization */
                current_Trace[0] = stack_Buffer[i];

                memset( linear_Semaphores, 0, linear_Vertexes * sizeof ( linear_Semaphores[0] ) );

                linear_Semaphores[stack_Buffer[i]] = 1;

                tat_GenPathSchedule( 1, 1 );

            /* all paths have been generated */
                if ( !process_Params.flag_GenerateTree && linear_PathCounter >= linear_PathLimit )
                {
                    break;
                }
            }
        }
    }

    if ( !process_Params.flag_GenerateTree && !process_Params.flag_CountTracesOnly )
        TM->paths = linear_PathStorage;

/* free memory */
    MEM_Free( col );
    MEM_Free( tsort );
    MEM_Free( current_Trace );
    MEM_Free( linear_Semaphores );

    MEM_Free( event_Types );
    MEM_Free( stack_Buffer );

    TM->path_Number = linear_PathCounter;

    return linear_PathCounter;  /* return number of generated test paths */
}

/***************************************************************************
 * Function : tat_ResearchPaths
 *
 * Description: generate specified number of test paths for all linear parts
 *              numb - number of paths for every linear part
 * Return: RESULT_ERR/RESULT_OK
 *
 * Notes:
 *
 **************************************************************************/
static LOGICAL tat_ResearchPaths( MSC_EVENT_TREE * TM, size_t numb )
{
    size_t                    i,         /* loop counters */
                            gen_Number = 0; /* actually generated number of test paths */
    size_t                    oper_Numb; /* number of paths for operator frames */

/* if there are any events in the linear part */
    if ( TM->ev_Number > 0 )
    {

        gen_Number = tat_GenLinearPaths( TM, ( numb < TM->path_Limit ) ? numb : TM->path_Limit );

        if ( process_Params.flag_GenerateTree && gen_Number > 4096 )
        {
            printf( MSG_PATHS_CAN_GENERATE, gen_Number );
        }

        if ( gen_Number == 0 )
        {
            return RESULT_ERR;
        }
    }

/* look for another branches */
    if ( TM->oper )
    {
    /* make decision about number of paths */
        oper_Numb = numb;

        if ( ( TM->oper )->data.alt.path_Limit != MSC_INFINITY_CONST )
        {
            oper_Numb = ( TM->oper )->data.alt.path_Limit;
        }

    /* process frames */
        for ( i = 0; i < TM->var_Number; i++ )
        {
            if ( !tat_ResearchPaths( TM->frames[i], oper_Numb ) )
            {
                return RESULT_ERR;
            }
        }
    }

/* look for another branches */
    if ( TM->next )
    {
        if ( !tat_ResearchPaths( TM->next, numb ) )
        {
            return RESULT_ERR;
        }
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_ResearchPaths
 *
 * Description: generate specified number of test paths for all linear parts
 *              numb - number of paths for every linear part
 * Return: RESULT_ERR/RESULT_OK
 *
 * Notes:
 *
 **************************************************************************/
LOGICAL MSC_ResearchPaths( MSC_EVENT_TREE * TM, MSC_GENPATH_PARAM params, size_t path_num )
{
    if ( TM == NULL )
        return RESULT_ERR;

    if ( params.flag_LimitActive < 0 || params.flag_LimitPassive < 0 ||
        ( params.flag_ActiveFirst && params.flag_PassiveFirst ) )
    {
        return RESULT_ERR;
    }
    if ( !path_num )
        path_num = MSC_INFINITY_CONST;

    schedule_Function = tat_ScheduleEvent;
    process_Params = params;

    return tat_ResearchPaths( TM, path_num );
}
