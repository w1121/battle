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

#include <portable.h>
#include <partial.h>
#include <mscbase.h>
#include <msctime.h>
#include <qmemory.h>
#include <testtree.h>
#include <outmess.h>
#include <mscgen.h>
#include <mscmess.h>
#include <mscstrs.h>
#include <tatgdata.h>

/* state of an instance */

typedef struct
{
    size_t                  event;     /* current event */
    size_t                  coregion;  /* coregion flag (bit) */
    size_t                  co_Number; /* number of events in buffer */
    size_t                 *co_Events; /* events of buffer */
}
MSC_INSTANCE_STATE;

/* force-spliting parameter */
/* UINT32_T MSC_SplitCount = 200; */

static MSC_STORAGE *tat_Msc;       /* pointer to loaded MSC */
static size_t           tat_InstNum;   /* number of instances in a loaded MSC diagram */
static int             tat_TimeFlag;  /* 0 -  without time, 1 - considering time */
static int             tat_IntervFlag; /* 0 - compare minimal time, 1 - compare time intervals */
static int             tat_ExpandOpFlag;   /* 1 - expand all operators */

/* if set, start new linear part after 'par' inline expression */
#ifndef MSC_OP_PAR_FLAG
#define MSC_OP_PAR_FLAG 1
#endif

/* nesting level */
static size_t           tat_NestLevel;

/* create new element in linear part */
static LOGICAL          tat_AddEvent( MSC_EVENT_TREE *, MSC_EVENT_KIND * );

/* create new ordered pair */
static LOGICAL          tat_AddPair( MSC_EVENT_TREE *, const size_t, const size_t );

/* copy state of instances */
static LOGICAL          tat_CopyInstanceState( MSC_INSTANCE_STATE *, MSC_INSTANCE_STATE * );

/* free instance state */
static LOGICAL          tat_FreeInstanceState( MSC_INSTANCE_STATE * );

/* process event */
static LOGICAL          tat_ProcessEvent( MSC_EVENT_TREE *, MSC_EVENT_KIND *, MSC_INSTANCE_STATE * );

/* select linear part of MSC diagram */
static LOGICAL          tat_ParseLinear( MSC_EVENT_TREE *, MSC_INSTANCE_STATE *, MSC_EVENT_KIND * );

/***************************************************************************
 * Function : tat_CountLinearEvents
 *
 * Description: count number of events in next linear part
 *
 * Returns: number of events
 *
 * Notes: None
 *
 **************************************************************************/
static size_t tat_CountLinearEvents( MSC_EVENT_KIND * event, size_t level )
{
    size_t                  res;
    size_t                  i, count;

    count = 0;

    while ( event )
    {
        if ( ( tat_NestLevel == 0 ) && ( level == 0 ) && ( event->inst_Count == tat_Msc->instance_Count ) &&
            count )
        {
            if ( ( event->type == MSC_EV_CONDITION || event->type == MSC_EV_REFERENCE ||
                    event->type == MSC_EV_CODE || ( event->type == MSC_EV_OPERATOR &&
                        event->data.alt.type == MSC_OP_PAR ) ) )
                break;
        }

        if ( event->type == MSC_EV_OPERATOR )
        {
            if ( event->data.alt.type == MSC_OP_PAR )
            {
                for ( i = 0; i < event->data.alt.frame_Count; i++ )
                {
                    res = tat_CountLinearEvents( event->data.alt.frame[i], level + 1 );

                    if ( res == (size_t)-1 )
                        return (size_t)-1;

                    count += res;
                }
#ifdef MSC_OP_PAR_FLAG
                if ( ( level == 0 ) && ( tat_NestLevel == 0 ) )
                    return count;
#endif
            }
            else
            {
                if ( level != 0 )
                {
                    MSG_PrintError( MSG_FEATURE_NOT_SUPP );
                    return (size_t)-1;
                }
                else
                    return count;
            }

        }
        else
            count++;

        event = event->next;
    }

    return count;
}

/***************************************************************************
 * Function : tat_InitLinear
 *
 * Description: Initialize MSC_EVENT_TREE structure
 *
 * Returns: RESULT_ERR / RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_InitLinear( MSC_EVENT_TREE * TM, size_t count )
{
    if ( count == (size_t)-1 )
        return RESULT_ERR;

    TM->linear = MEM_Allocate( count * sizeof ( MSC_EVENT_KIND * ) );
    TM->adjacency = MEM_Allocate( count * count * sizeof ( Byte ) );
    TM->ev_NumberPr = count;
    TM->MSC = tat_Msc;
    TM->path_Limit = MSC_INFINITY_CONST;
    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_ParseLinear
 *
 * Description: select linear part of MSC diagram
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_ParseLinear( MSC_EVENT_TREE * TM, MSC_INSTANCE_STATE * inst_State, MSC_EVENT_KIND * event )
{
    size_t                  i, j;      /* loop counters */
    size_t                  first_inst;
    int                     indic = 0; /* kind of the first OUT in alt frames */

/* while events */

    while ( event )
    {
    /* force splitting */

        if ( ( tat_NestLevel == 0 ) && ( event->inst_Count == tat_Msc->instance_Count ) && TM->ev_Number )
        {
            if ( ( event->type == MSC_EV_CONDITION || event->type == MSC_EV_REFERENCE ||
                    event->type == MSC_EV_CODE || ( event->type == MSC_EV_OPERATOR &&
                        event->data.alt.type == MSC_OP_PAR ) ) )
            {
                TM->next = ( MSC_EVENT_TREE * ) MEM_Allocate( sizeof ( MSC_EVENT_TREE ) );
            /* freeing state */

                if ( !tat_FreeInstanceState( inst_State ) )
                {
                    return RESULT_ERR;
                }

                if ( !tat_InitLinear( TM->next, tat_CountLinearEvents( event, 0 ) ) )
                    return RESULT_ERR;

                if ( tat_ParseLinear( TM->next, inst_State, event ) )
                    return RESULT_OK;

                return RESULT_ERR;

            }
        }

    /* add event into list of events */
        if ( TM->ev_Number < TM->ev_NumberPr )
            TM->linear[TM->ev_Number] = event;

        first_inst = event->instances[0];

        switch ( event->type )
        {
        /* linear elements */

        case MSC_EV_IN_MESSAGE:
        case MSC_EV_OUT_MESSAGE:
        /* a message must first be sent before it is consumed */
        /* looking for output from the end of list */
            i = TM->ev_Number;

            if ( event->type == MSC_EV_IN_MESSAGE )
            {
                while ( i )
                {
                    i--;

                    if ( ( ( TM->linear[i] )->type == MSC_EV_OUT_MESSAGE ) &&
                        ( ( TM->linear[i] )->data.msg.indicator == event->data.msg.indicator ) )
                    {
                        if ( !tat_AddPair( TM, i, TM->ev_Number ) )
                        {
                            return RESULT_ERR;
                        }

                        break;  /* just one pair of events */
                    }
                }
            }
            else
            {                   /* MSC_EV_OUT_MESSAGE - allow backward links */

                while ( i )
                {
                    i--;

                    if ( ( ( TM->linear[i] )->type == MSC_EV_IN_MESSAGE ) &&
                        ( ( TM->linear[i] )->data.msg.indicator == event->data.msg.indicator ) )
                    {
                        if ( !tat_AddPair( TM, TM->ev_Number, i ) )
                        {
                            return RESULT_ERR;
                        }

                        break;  /* just one pair of events */
                    }
                }
            }

        /* to be continued in MSC_EV_OUT_MESSAGE case */

        case MSC_EV_CREATE:
        case MSC_EV_SEND_SIGNAL:
        case MSC_EV_RECV_SIGNAL:
        case MSC_EV_EXIST_SIGNAL:
        /* validate  environment for MSC_LOST_INSTANCE_ID and MSC_FOUND_INSTANCE_ID */
        /* in concurrent region */
            if ( inst_State[first_inst].coregion > 0 )
            {
            /* order after beginning of coregion */

                if ( !tat_AddPair( TM, inst_State[first_inst].event, TM->ev_Number ) )
                {
                    return RESULT_ERR;
                }

            /* add event to coregion buffer */
                inst_State[first_inst].co_Events =
                    ( UINT32_T * ) MEM_Reallocate( inst_State[first_inst].co_Events,
                    ( inst_State[first_inst].co_Number + 1 ) * sizeof ( UINT32_T ) );

                if ( inst_State[first_inst].co_Events != NULL )
                {
                    inst_State[first_inst].co_Events[inst_State[first_inst].co_Number] = TM->ev_Number;
                    inst_State[first_inst].co_Number++;
                }
                else
                {
                    return RESULT_ERR;
                }

            /* add event to linear part */
                if ( tat_AddEvent( TM, event ) )
                {
                    TM->ev_Number++;
                }
                else
                    return RESULT_ERR;
            }
            else if ( !tat_ProcessEvent( TM, event, inst_State ) )
                return RESULT_ERR;

            break;

        case MSC_EV_CONCURRENT_END:
            if ( inst_State[first_inst].coregion )
                ( inst_State[first_inst].coregion )--;

        /* protection from nested coregion */
            if ( inst_State[first_inst].coregion == 0 )
            {
                if ( !tat_ProcessEvent( TM, event, inst_State ) )
                    return RESULT_ERR;
            }

            break;

        case MSC_EV_CONCURRENT_START:
        /* set flag */
            TM->lflags |= MSC_LINEAR_HAS_COREGION;
            ( inst_State[first_inst].coregion )++;
        /* protection from nested coregion */

            if ( inst_State[first_inst].coregion > 1 )
            {
                MSG_PrintError( MSG_NESTED_COREGIONS );
                break;
            }

        /* to be continued in MSC_EV_ACTION case */

        case MSC_EV_REFERENCE: /* reference */
        case MSC_EV_CONDITION: /* condition */
        case MSC_EV_START_LOOP:
        case MSC_EV_END_LOOP:
        case MSC_EV_EMPTY:
        case MSC_EV_ACTION:    /* action */
        case MSC_EV_CODE:      /* code from FLOWGEN */
        case MSC_EV_STOP:

        /* timer */
        case MSC_EV_TIMER_SET:
        case MSC_EV_TIMER_RESET:
        case MSC_EV_TIMER_TIMEOUT:
            if ( !tat_ProcessEvent( TM, event, inst_State ) )
            {
                return RESULT_ERR;
            }

            break;

        /* operator */

        case MSC_EV_OPERATOR:
        /* operator can not be in coregion */

            for ( i = 0; i < event->inst_Count; i++ )
            {
                if ( inst_State[event->instances[i]].coregion > 0 )
                {
                    MSG_PrintError( MSG_OPER_IN_COREGION );
                    return RESULT_ERR;
                }
            }
            TM->path_Limit = event->data.alt.path_Limit;

            switch ( event->data.alt.type )
            {

            case MSC_OP_SEQ:
                break;

            case MSC_OP_PAR:
            {
                MSC_INSTANCE_STATE     *par_State;  /* copy of each instance state */
                size_t                  **par_Result; /* results of par execution */
                size_t                   *numb_Result;    /* number of result event */

            /* set flag */
                TM->lflags |= MSC_LINEAR_HAS_PAR;

            /* allocate memory for parallel states */
                par_State =
                    ( MSC_INSTANCE_STATE * ) MEM_Allocate( sizeof ( par_State[0] ) * tat_InstNum );
                par_Result = ( size_t ** ) MEM_Allocate( sizeof ( par_Result[0] ) * tat_InstNum );
                numb_Result = ( size_t * ) MEM_Allocate( sizeof ( numb_Result[0] ) * tat_InstNum );

            /* process all par sections */

                for ( i = 0; i < event->data.alt.frame_Count; i++ )
                {
                /* save current state */

                    if ( !tat_CopyInstanceState( par_State, inst_State ) )
                    {
                        return RESULT_ERR;
                    }

                    tat_NestLevel++;

                    if ( !tat_ParseLinear( TM, par_State, event->data.alt.frame[i] ) )
                    {
                    /* MSG_PrintError( MSG_CANT_BUILD_PO_PAR, event->line_Number); */
                        return RESULT_ERR;
                    }

                    tat_NestLevel--;
                /* save instance states after each parallel */

                    for ( j = 0; j < tat_InstNum; j++ )
                    {
                        if ( ( par_State[j].event != (size_t)-1 ) && ( par_State[j].event != inst_State[j].event ) )
                        {
                            par_Result[j] =
                                ( size_t * ) MEM_Reallocate( par_Result[j],
                                ( numb_Result[j] + 1 ) * sizeof ( par_Result[j][0] ) );
                            par_Result[j][numb_Result[j]] = par_State[j].event;
                            numb_Result[j]++;
                        }
                    }
                }

            /* copy all result to original state */
                for ( i = 0; i < tat_InstNum; i++ )
                {
                    if ( par_Result[i] != NULL )
                    {
                        if ( inst_State[i].co_Events != NULL )
                        {
                            MEM_Free( inst_State[i].co_Events );
                        }

                        inst_State[i].co_Events =
                            MEM_Copy( par_Result[i], ( sizeof ( UINT32_T ) * numb_Result[i] ) );
                        inst_State[i].co_Number = numb_Result[i];
                        MEM_Free( par_Result[i] );
                        par_Result[i] = NULL;
                    }
                }

            /* free memory */
                MEM_Free( par_Result );

                MEM_Free( numb_Result );

                if ( !tat_FreeInstanceState( par_State ) )
                {
                    return RESULT_ERR;
                }

                MEM_Free( par_State );
#ifdef          MSC_OP_PAR_FLAG
                if ( tat_NestLevel == 0 )
                {
                    TM->next = ( MSC_EVENT_TREE * ) MEM_Allocate( sizeof ( MSC_EVENT_TREE ) );
                /* freeing state */

                    if ( !tat_FreeInstanceState( inst_State ) )
                    {
                        return RESULT_ERR;
                    }

                    if ( !tat_InitLinear( TM->next, tat_CountLinearEvents( event->next, 0 ) ) )
                        return RESULT_ERR;

                    if ( tat_ParseLinear( TM->next, inst_State, event->next ) )
                        return RESULT_OK;

                    return RESULT_ERR;
                }
#endif
                break;
            }

            case MSC_OP_LOOP:
            {
                TM->expand = 1;
                if ( tat_ExpandOpFlag )
                {
                /* make decision about loop expansion */

                    if ( !( ( event->data.alt.l1 ) &&
                            !strcmp( MSC_GetString( event->data.alt.l1, tat_Msc ), "1" ) &&
                            ( event->data.alt.l2 == 0 ) ) )
                    {
                        TM->expand = 1;
                    }
                }
            }

            case MSC_OP_ALT:
            case MSC_OP_OPT:
            case MSC_OP_EXC:
            {
            /* process operator */
                TM->type = event->data.alt.type;
                TM->oper = event;
                TM->var_Number = event->data.alt.frame_Count;
            /* allocate memory for pointers to operator frames */
                TM->frames =
                    ( MSC_EVENT_TREE ** ) MEM_Allocate( ( TM->var_Number ) * sizeof ( MSC_EVENT_TREE * ) );
            /* process operator's frames */

                for ( i = 0; i < TM->var_Number; i++ )
                {
                /* allocate memory */
                    TM->frames[i] = ( MSC_EVENT_TREE * ) MEM_Allocate( sizeof ( MSC_EVENT_TREE ) );
                /* free states */

                    if ( !tat_FreeInstanceState( inst_State ) )
                    {
                        return RESULT_ERR;
                    }

                    if ( TM->type != MSC_OP_LOOP )
                        tat_NestLevel++;
                /* dive into recursion */
                    if ( !tat_InitLinear
                        ( TM->frames[i], tat_CountLinearEvents( ( TM->oper )->data.alt.frame[i], 0 ) ) )
                        return RESULT_ERR;

                    if ( !tat_ParseLinear( TM->frames[i], inst_State, TM->oper->data.alt.frame[i] ) )
                    {
                        return RESULT_ERR;
                    }
                    if ( TM->type != MSC_OP_LOOP )
                        tat_NestLevel--;

                /* validate operator's frames */
                    if ( tat_ExpandOpFlag && event->data.alt.type != MSC_OP_LOOP )
                    {
                        for ( j = 0; j < ( TM->frames[i] )->ev_Number; j++ )
                        {
                        /* if out message not to MSC_LOST_INSTANCE_ID */

                            if ( ( ( ( TM->frames[i] )->linear[j] )->type == MSC_EV_OUT_MESSAGE )
                                && ( ( ( TM->frames[i] )->linear[j] )->data.msg.inst != MSC_LOST_INSTANCE_ID ) )
                            {
                                if ( tat_Msc->instances[( ( TM->frames[i] )->linear[j] )->instances[0]].
                                    ind != MSC_INSTANCE_UNDEF )
                                {
                                    if ( ( i > 0 ) &&
                                        ( ( tat_Msc->
                                                instances[( ( TM->frames[i] )->linear[j] )->instances[0]].
                                                ind != MSC_INSTANCE_ENV ) != indic ) )
                                    {
                                        MSG_PrintError( MSG_MESS_WIN_ALT_MIXED );
                                        return RESULT_ERR;
                                    }

                                    indic =
                                        ( tat_Msc->
                                        instances[( ( TM->frames[i] )->linear[j] )->instances[0]].ind !=
                                        MSC_INSTANCE_ENV );
                                }

                                break;
                            }
                        }
                    }
                }

                if ( tat_ExpandOpFlag )
                {
                /* assign expansion flag */
                    TM->expand |= indic;
                }

            /* if there are any events after operator */
                if ( event->next )
                {
                    TM->next = ( MSC_EVENT_TREE * ) MEM_Allocate( sizeof ( MSC_EVENT_TREE ) );
                /* freeing state */

                    if ( !tat_FreeInstanceState( inst_State ) )
                    {
                        return RESULT_ERR;
                    }

                /* dive into recursion */
                    if ( !tat_InitLinear( TM->next, tat_CountLinearEvents( event->next, 0 ) ) )
                        return RESULT_ERR;

                    if ( tat_ParseLinear( TM->next, inst_State, event->next ) )
                        return RESULT_OK;

                /*  MSG_PrintError(MSG_CANT_PARSE_FRAME); */

                    return RESULT_ERR;
                }

                break;
            }

            /* error */

            default:
            {
                MSG_Abort( MSG_INTERNAL_PROBLEM );
                return RESULT_ERR;
            }
            }

            break;
        /* skip element */

        case MSC_EV_UNDEFINED:
            break;
        /* error */

        default:
        {
            MSG_Abort( MSG_INTERNAL_PROBLEM );
            return RESULT_ERR;
        }
        }

        event = event->next;
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_CopyInstanceState
 *
 * Description: copy state of instances
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_CopyInstanceState( MSC_INSTANCE_STATE * dest_State, MSC_INSTANCE_STATE * src_State )
{
    UINT32_T                    i;         /* loop counter */

/* check input parameters */

    if ( ( dest_State == NULL ) || ( src_State == NULL ) )
    {
        return RESULT_ERR;
    }

/* copy memory */
    memcpy( dest_State, src_State, ( tat_InstNum * sizeof ( MSC_INSTANCE_STATE ) ) );

    for ( i = 0; i < tat_InstNum; i++ )
    {
        if ( dest_State[i].co_Events != NULL )
        {
        /* MEM_Free(dest_State[i].co_Events); */
            dest_State[i].co_Events =
                MEM_Copy( src_State[i].co_Events, ( sizeof ( UINT32_T ) * src_State[i].co_Number ) );
        }
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_FreeInstanceState
 *
 * Description: free used memory
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_FreeInstanceState( MSC_INSTANCE_STATE * inst_State )
{
    size_t             i;         /* loop counter */

/* check input parameters */

    if ( inst_State == NULL )
    {
        return RESULT_OK;
    }

/* initialize instances */
    for ( i = 0; i < tat_InstNum; i++ )
    {
        if ( inst_State[i].co_Events != NULL )
        {
            MEM_Free( inst_State[i].co_Events );
            inst_State[i].co_Events = NULL;
        }

        inst_State[i].co_Number = 0;
        inst_State[i].coregion = 0;
        inst_State[i].event = (size_t)-1;   /* no event */
    }

    return RESULT_OK;
}

/* create new ordered pair */
static LOGICAL tat_AddPair( MSC_EVENT_TREE * TM, const size_t pr /* predecessor */ ,
    const size_t nt /* succecessor */  )
{
/* check for incorrect ordering */

    if ( TM->adjacency[nt * TM->ev_NumberPr + pr] == 1 )
    {
        char                    dest[TAT_MAX_LINE_LEN];

        MEM_Free( TM->adjacency );
        TM->adjacency = NULL;
        MSG_PrintError( MSG_VARIANCE_IN_PO );
        MSC_GenerateLine( dest, TM->linear[nt], TM->MSC,
            MSC_MPR_VERBOSE_OUT | MSC_MPR_OUTPUT_DIAG | MSC_MPR_OUTPUT_CALCTIME );
        MSG_PrintError( "EVENT %d: %s\n", nt, dest );
        MSC_GenerateLine( dest, TM->linear[pr], TM->MSC,
            MSC_MPR_VERBOSE_OUT | MSC_MPR_OUTPUT_DIAG | MSC_MPR_OUTPUT_CALCTIME );
        MSG_PrintError( "EVENT %d: %s\n", pr, dest );
        return RESULT_ERR;
    }

    TM->adjacency[pr * TM->ev_NumberPr + nt] = 1;
    return RESULT_OK;
}

static MSC_TIME_INTERVAL get_AbsLab( MSC_TIME_INTERVAL x, MSC_STORAGE * M )
{
    MSC_EVENT_KIND              *s;

    if ( x.type != MSC_LABELED_TIME )
    {
        return x;
    }

    s = M->labels[x.time_Label].s;

    while ( s && ( s->time_Calc.type == MSC_LABELED_TIME ) && ( s->time_Calc.max_Time != MSC_INFINITY_CONST ) )
    {
        x.min_Time = MSC_AddTime_( x.min_Time, s->time_Calc.min_Time );
        x.max_Time = MSC_AddTime_( x.max_Time, s->time_Calc.max_Time );
        x.time_Label = s->time_Calc.time_Label; /* set old label */
        x.type = s->time_Calc.type;

        if ( s->time_Calc.type == MSC_LABELED_TIME )
            s = M->labels[x.time_Label].s;
        else
            break;
    }

    return x;
}

/***************************************************************************
 * Function : comp_Time
 *
 * Description: Compare times of events
 *
 * Returns: 0 - incomparable of equal, -1/1 (s1>s2)/(s2<s1)
 *
 * Notes: None
 *
 **************************************************************************/
static int comp_Time( MSC_EVENT_KIND * s1, MSC_EVENT_KIND * s2 )
{
    MSC_TIME_INTERVAL  x1, x2;

    if ( s1->time_Calc.type == MSC_RELATIVE_TIME || s2->time_Calc.type == MSC_RELATIVE_TIME )
        return 0;

    x1 = get_AbsLab( s1->time_Calc, tat_Msc );
    x2 = get_AbsLab( s2->time_Calc, tat_Msc );

    if ( x1.type != x2.type )
        return 0;

    if ( x1.type == MSC_LABELED_TIME && ( x1.time_Label != x2.time_Label ) )
        return 0;

    if ( tat_IntervFlag == 0 )
    {
        if ( x1.min_Time < x2.min_Time )
            return -1;

        if ( x1.min_Time > x2.min_Time )
            return 1;
    }
    else
    {
        if ( x1.max_Time < x2.min_Time )
            return -1;

        if ( x1.min_Time > x2.max_Time )
            return 1;
    }

    return 0;
}

/* create new element in linear part */
/* assume input parameters are correct */
static LOGICAL tat_AddEvent( MSC_EVENT_TREE * TM, MSC_EVENT_KIND * event )
{
    size_t                    i;         /* loop counter */
    int                     comp_t;

/* considering time */

    if ( tat_TimeFlag && event->time_Calc.type == MSC_RELATIVE_TIME &&
        !( event->type == MSC_EV_CONDITION && event->data.cond.generated ) )
    {
    /* output semantic warning */
        MSG_PrintError( MSG_RELATIVE_TIME_TF_su, MSC_GetStr( tat_Msc->msc_File, tat_Msc ),
            event->line_Number );
    }

    if ( ( tat_TimeFlag ) && ( !MSC_IsDefaultEventTime( event ) ) &&
        ( event->type != MSC_EV_TIMER_SET ) && ( event->time_Calc.type != MSC_RELATIVE_TIME ) )
    {
        i = TM->ev_Number;
    /* compare minimal time */

        while ( i > 0 )
        {
            i--;

        /*&& ((TM->linear[i])->time_Calc.max_Time != MSC_INFINITY_CONST) */
            if ( !MSC_IsDefaultEventTime( ( TM->linear[i] )) )
            {
            /* considering absolute time */

                if ( ( comp_t = comp_Time( event, TM->linear[i] ) ) != 0 )
                {
                /* a new event before event */

                    if ( comp_t < 0 )
                    {
                        if ( !tat_AddPair( TM, TM->ev_Number, i ) )
                        {
                            return RESULT_ERR;
                        }
                    }
                    else
                    {
                        if ( !tat_AddPair( TM, i, TM->ev_Number ) )
                        {
                            return RESULT_ERR;
                        }
                    }
                }
            }
        }
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_ProcessEvent
 *
 * Description: add event to linear part with pre-processing
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_ProcessEvent( MSC_EVENT_TREE * TM, MSC_EVENT_KIND * event, MSC_INSTANCE_STATE * inst_State )
{
    size_t                    i, j;      /* loop counters */

/* check input parameters */

    if ( ( TM == NULL ) || ( event == NULL ) || ( inst_State == NULL ) )
    {
        return RESULT_ERR;
    }

/* process partial order first */
    for ( i = 0; i < event->inst_Count; i++ )
    {
    /* order with buffer */

        if ( inst_State[event->instances[i]].co_Events != NULL )
        {
        /* extract events from buffer */

            for ( j = 0; j < inst_State[event->instances[i]].co_Number; j++ )
            {
                if ( !tat_AddPair( TM, inst_State[event->instances[i]].co_Events[j], TM->ev_Number ) )
                {
                    return RESULT_ERR;
                }
            }

            inst_State[event->instances[i]].co_Number = 0;
            MEM_Free( inst_State[event->instances[i]].co_Events );
            inst_State[event->instances[i]].co_Events = NULL;
        }
        else                    /* order along instance axis */
        {
            if ( inst_State[event->instances[i]].event != (size_t)-1 )
            {
                if ( !tat_AddPair( TM, inst_State[event->instances[i]].event, TM->ev_Number ) )
                {
                    return RESULT_ERR;
                }
            }
        }

        inst_State[event->instances[i]].event = TM->ev_Number;
    }

    if ( event->inst_Count )
    {
        if ( event->type == MSC_EV_SEND_SIGNAL || event->type == MSC_EV_RECV_SIGNAL ||
            event->type == MSC_EV_EXIST_SIGNAL )
        {
            i = event->data.msg.inst;

            if ( i > 0 )
            {
            /* order with buffer */

                if ( inst_State[i].co_Events != NULL )
                {
                /* extract events from buffer */

                    for ( j = 0; j < inst_State[i].co_Number; j++ )
                    {
                        if ( !tat_AddPair( TM, inst_State[i].co_Events[j], TM->ev_Number ) )
                        {
                            return RESULT_ERR;
                        }
                    }

                    inst_State[i].co_Number = 0;
                    MEM_Free( inst_State[i].co_Events );
                    inst_State[i].co_Events = NULL;
                }
                else            /* order along instance axis */
                {
                    if ( inst_State[i].event != (size_t)-1 )
                    {
                        if ( !tat_AddPair( TM, inst_State[i].event, TM->ev_Number ) )
                        {
                            return RESULT_ERR;
                        }
                    }
                }

                inst_State[i].event = TM->ev_Number;
            }
        }

    /* special case - instance creation */
        if ( event->type == MSC_EV_CREATE )
        {
            if ( inst_State[event->data.create.instance].event != (size_t)-1 )
            {
                if ( !tat_AddPair( TM, inst_State[event->data.create.instance].event, TM->ev_Number ) )
                {
                    return RESULT_ERR;
                }
            }

            inst_State[event->data.create.instance].event = TM->ev_Number;
        }

        if ( event->type == MSC_EV_END_LOOP )
        {
            i = TM->ev_Number;
        /* compare minimal time */

            while ( i > 0 )
            {
                i--;

                if ( TM->linear[i]->type == MSC_EV_START_LOOP )
                {
                    if ( !tat_AddPair( TM, i, TM->ev_Number - 1 ) )
                    {
                        return RESULT_ERR;
                    };

                    break;
                }
            }
        }
    }

/* process time & semaphores */
    if ( !tat_AddEvent( TM, event ) )
    {
        return RESULT_ERR;
    }

    TM->ev_Number++;
    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_CleanTests
 *
 * Description: Free allocated memory
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_CleanTests( struct MSC_TEST_CASENODE *x )
{
    size_t                    i;

    if ( !x )
        return RESULT_ERR;

    for ( i = 0; i < x->next_Count; i++ )
    {
        tat_CleanTests( x->next[i] );
    }

    if ( x->next )
        MEM_Free( x->next );

    MEM_Free( x );
    return RESULT_OK;
}

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
LOGICAL MSC_CleanLinearPart( MSC_EVENT_TREE * TM )
{
    size_t                 i;         /* loop counter */

/* check input parameters */

    if ( TM == NULL )
    {
        return RESULT_OK;
    }

/* free all leaves */
    for ( i = 0; i < TM->var_Number; i++ )
    {
        if ( !MSC_CleanLinearPart( TM->frames[i] ) )
        {
            return RESULT_ERR;
        }

        MEM_Free( TM->frames[i] );
    }

    if ( TM->frames )
        MEM_Free( TM->frames );

    if ( TM->next )
    {
        if ( !MSC_CleanLinearPart( TM->next ) )
        {
            return RESULT_ERR;
        }

        MEM_Free( TM->next );
    }

/* free current node */
    if ( TM->linear )
    {
        MEM_Free( TM->linear );
        TM->linear = NULL;
    }

    if ( TM->paths )
    {
        for ( i = 0; i < TM->path_Number; i++ )
            MEM_Free( TM->paths[i] );
        MEM_Free( TM->paths );
    }

    if ( TM->adjacency )
    {
        MEM_Free( TM->adjacency );
        TM->adjacency = NULL;
    }

    if ( TM->frame_FullCounter )
        MEM_Free( TM->frame_FullCounter );

    if ( TM->frame_PathCounter )
        MEM_Free( TM->frame_PathCounter );

    if ( TM->tests )
    {
        tat_CleanTests( TM->tests );
        TM->tests = NULL;
    }

    return RESULT_OK;
}

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
LOGICAL MSC_BuildLinearPart( MSC_STORAGE * M,
    MSC_EVENT_TREE * TM, const UINT32_T timer_Flag, const UINT32_T interv_Flag, const UINT32_T expn_Flag )
{
    MSC_INSTANCE_STATE     *inst_State; /* state of each instance */
    UINT32_T                    i;         /* loop counter */

/* check input parameters */

    if ( ( M == NULL ) || ( TM == NULL ) || ( timer_Flag > 1 ) || ( interv_Flag > 1 ) )
    {
        return RESULT_ERR;
    }

/* initialization */
    tat_Msc = M;
    tat_InstNum = M->instance_Count;
    tat_TimeFlag = timer_Flag;
    tat_IntervFlag = interv_Flag;
    tat_ExpandOpFlag = expn_Flag & 1;

/*    MSC_OP_PAR_FLAG = expn_Flag & 2; */
    tat_NestLevel = 0;

/* allocate memory for state */
    inst_State = ( MSC_INSTANCE_STATE * ) MEM_Allocate( sizeof ( inst_State[0] ) * tat_InstNum );

/* set no events */
    for ( i = 0; i < tat_InstNum; i++ )
    {
        inst_State[i].event = (size_t)-1;   /* no event */
    }

/* node initialization */
    memset( TM, 0, sizeof ( MSC_EVENT_TREE ) );

/* select linear parts and create partial order */
    if ( !tat_InitLinear( TM, tat_CountLinearEvents( M->start, 0 ) ) )
        return RESULT_ERR;

    if ( !tat_ParseLinear( TM, inst_State, M->start ) )
    {
        return RESULT_ERR;
    }

/* free states */
    if ( !tat_FreeInstanceState( inst_State ) )
    {
        return RESULT_ERR;
    }

    if ( inst_State )
    {
        MEM_Free( inst_State );
    }
    return RESULT_OK;
}
