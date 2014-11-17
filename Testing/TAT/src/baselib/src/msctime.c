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
* Functions:    MSC_CalcTime
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  MSC Time calculation
*
* Notes:        None
*
******************************************************************************/
#include <portable.h>
#include <qmemory.h>
#include <fileproc.h>
#include <mscbase.h>

#include <mscparse.h>
#include <mscproc.h>
#include <mscgen.h>
#include <msctime.h>
#include <math.h>
#include <outmess.h>
#include <mscmess.h>
#include <mscstrs.h>
#include <tatgdata.h>

static int              tat_TranspLabel = 0;


typedef struct
{
    size_t                  label;          /* zero if none */
    MSC_TIME_VALUE          min_Time;       /* minimal time */
    MSC_TIME_VALUE          max_Time;       /* maximal time */
    MSC_EVENT_KIND         *last_Event;     /* last event on instance */
}
MSC_INSTANCE_TIME;

static UINT32_T             tat_OutMessageCount;

static struct
{
    size_t                  ind;
    MSC_EVENT_KIND         *s;
    MSC_MESSAGE_STORAGE    *p;
}
                       *tat_OutMessages;

MSC_TIME_VALUE         *tat_InstanceTimeScale;

/***************************************************************************
 * Function : MSC_AddTime
 *
 * Description: calculate sum of two times with overflow control
 *
 * Returns: time
 *
 * Notes: None
 *
 **************************************************************************/
MSC_TIME_VALUE MSC_AddTime( MSC_TIME_VALUE x, MSC_TIME_VALUE y )
{
    if ( x == MSC_INFINITY_CONST || y == MSC_INFINITY_CONST )
        return MSC_INFINITY_CONST;

    return x + y;
}

/***************************************************************************
 * Function : MSC_MulTime
 *
 * Description: calculates multiplication of two times with 
 *              overflow control
 *
 * Returns: time
 *
 * Notes: None
 *
 **************************************************************************/
MSC_TIME_VALUE MSC_MulTime( MSC_TIME_VALUE x, MSC_TIME_VALUE y )
{
    if ( x == MSC_INFINITY_CONST || y == MSC_INFINITY_CONST )
        return MSC_INFINITY_CONST;

    if ( x == 0 || y == 0 )
        return 0;

    return x * y;
}

/***************************************************************************
 * Function : MSC_AddMulTime
 *
 * Description: calculates x+(y*z) with overflow control
 *
 * Returns: time
 *
 * Notes: None
 *
 **************************************************************************/
MSC_TIME_VALUE MSC_AddMulTime( MSC_TIME_VALUE x, MSC_TIME_VALUE y, MSC_TIME_VALUE z )
{
    return MSC_AddTime( x, MSC_MulTime( y, z ) );
}

/***************************************************************************
 * Function : MSC_CompareTime
 *
 * Description: compare two times
 *
 * Returns: time
 *
 * Notes: None
 *
 **************************************************************************/
int MSC_CompareTime( MSC_TIME_VALUE x, MSC_TIME_VALUE y )
{
    if ( x < y )
        return -1;

    if ( x == y )
        return 0;

    return 1;
}

/***************************************************************************
 * Function : MSC_MinTime
 *
 * Description: get minimal time
 *
 * Returns: time
 *
 * Notes: None
 *
 **************************************************************************/
MSC_TIME_VALUE MSC_MinTime( MSC_TIME_VALUE x, MSC_TIME_VALUE y )
{
    return ( x <= y ) ? x : y;
}

/***************************************************************************
 * Function : MSC_MaxTime
 *
 * Description: get maximal time 
 *
 * Returns: time
 *
 * Notes: None
 *
 **************************************************************************/
MSC_TIME_VALUE MSC_MaxTime( MSC_TIME_VALUE x, MSC_TIME_VALUE y )
{
    return ( x > y ) ? x : y;
}

static int tat_TimeError( MSC_STORAGE * M, MSC_EVENT_KIND * s )
{
    MSG_PrintError( MSG_INVALID_TIME_SPEC_CALC, MSC_GetString( M->msc_File, M ), s->line_Number );
    return RESULT_OK;
}

static int tat_TimeWarning( MSC_STORAGE * M, MSC_EVENT_KIND * s, size_t l1, size_t l2 )
{
    MSG_PrintError( MSG_AMBIG_TIME_SPEC,
        MSC_GetString( M->msc_File, M ), s->line_Number,
        MSC_GetString( M->labels[l1].label, M ), MSC_GetString( M->labels[l2].label, M ) );

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_UpdateEventTime
 *
 * Description: update time for event
 *
 * Returns: new time
 *
 * Notes: None
 *
 **************************************************************************/
static MSC_TIME_INTERVAL tat_UpdateEventTime( MSC_TIME_INTERVAL x, MSC_STORAGE * M )
{
    MSC_EVENT_KIND              *s;

    if ( x.type != MSC_LABELED_TIME )
    {
        return x;
    }

/* set only fixed times */
    s = M->labels[x.time_Label].s;
    if (x.valid_Number)
    {
        if ( x.max_Time != MSC_INFINITY_CONST)
        {
            while ( s && s->time_Calc.valid_Number && ( s->time_Calc.type == MSC_LABELED_TIME ) && s->time_Calc.max_Time != MSC_INFINITY_CONST &&
                ( x.time_Label != s->time_Calc.time_Label ) &&
                ( ( s->time_Calc.max_Time == s->time_Calc.min_Time ) ||
                ( tat_TranspLabel && s->time_Calc.min_Time != MSC_INFINITY_CONST ) ) )
            {
                x.min_Time = MSC_AddTime_( x.min_Time, s->time_Calc.min_Time );
                x.max_Time = MSC_AddTime_( x.max_Time, s->time_Calc.max_Time );
                x.time_Label = s->time_Calc.time_Label; /* set old label */
                
                if ( s->time_Calc.type == MSC_LABELED_TIME )
                    s = M->labels[x.time_Label].s;
                else
                    break;
            }
        }
        else
        {
            while ( s && s->time_Calc.valid_Number && ( s->time_Calc.type == MSC_LABELED_TIME ) &&
                ( x.time_Label != s->time_Calc.time_Label ) &&
                ( ( s->time_Calc.max_Time == s->time_Calc.min_Time ) ||
                ( tat_TranspLabel && s->time_Calc.min_Time != MSC_INFINITY_CONST ) ||
                ( s->time_Calc.max_Time == MSC_INFINITY_CONST ) ) )
            {
                x.min_Time = MSC_AddTime_( x.min_Time, s->time_Calc.min_Time );
                x.max_Time = MSC_AddTime_( x.max_Time, s->time_Calc.max_Time );
                x.time_Label = s->time_Calc.time_Label; /* set old label */
                
                if ( s->time_Calc.type == MSC_LABELED_TIME )
                    s = M->labels[x.time_Label].s;
                else
                    break;
            }
        }
    }
    return x;
}

/***************************************************************************
 * Function : tat_GetEqTime
 *
 * Description: get equivalent time for specified base, if possible
 *
 * Returns: new time
 *
 * Notes: None
 *
 **************************************************************************/
static MSC_INSTANCE_TIME tat_GetEqTime( MSC_INSTANCE_TIME x, MSC_STORAGE * M, size_t lab )
{
    MSC_EVENT_KIND              *s;

    if ( !x.label )             /* absolute is absolute */
    {
        return x;
    }

    s = M->labels[x.label].s;

    while ( s && s->time_Calc.valid_Number && ( s->time_Calc.type == MSC_LABELED_TIME ) &&
        ( ( s->time_Calc.min_Time == s->time_Calc.max_Time ) ||
            ( tat_TranspLabel && s->time_Calc.max_Time != MSC_INFINITY_CONST ) ) )
    {
        x.min_Time = MSC_AddTime_( x.min_Time, s->time_Calc.min_Time );
        x.max_Time = MSC_AddTime_( x.max_Time, s->time_Calc.max_Time );

        if ( x.label != s->time_Calc.time_Label )
        {
            x.label = s->time_Calc.time_Label;  /* set old label */
        }
        else
            break;

        s = M->labels[x.label].s;

        if ( x.label == lab )
            break;
    }

    return x;
}

/***************************************************************************
 * Function : tat_GetAbsTime
 *
 * Description: Get absolute time, even with INFINITE bounds
 *
 * Returns: absolute time
 *
 * Notes: None
 *
 **************************************************************************/
static MSC_INSTANCE_TIME tat_GetAbsTime( MSC_INSTANCE_TIME x, MSC_STORAGE * M )
{
    MSC_EVENT_KIND              *s;

    if ( !x.label )             /* absolute is absolute */
    {
        return x;
    }

    s = M->labels[x.label].s;

    while ( s && s->time_Calc.valid_Number && ( s->time_Calc.type == MSC_LABELED_TIME ) )
    {
        x.min_Time = MSC_AddTime_( x.min_Time, s->time_Calc.min_Time );
        x.max_Time = MSC_AddTime_( x.max_Time, s->time_Calc.max_Time );

        if ( x.label != s->time_Calc.time_Label )
        {
            x.label = s->time_Calc.time_Label;  /* set old label */
        }
        else
            break;

        s = M->labels[x.label].s;

        if ( !x.label )
            break;
    }

    return x;
}

/***************************************************************************
 * Function : tat_GetMultiTime
 *
 * Description: calculate time if event shares some instances.
 *
 * Returns: filled 'res' parameter
 *
 * Notes: None
 *
 **************************************************************************/
static int tat_GetMultiTime( MSC_STORAGE * M,   /* MSC */
    MSC_EVENT_KIND * s,                     /* event */
    MSC_INSTANCE_TIME * c_time,        /* current time on instances */
    MSC_INSTANCE_TIME * res )
{
    MSC_TIME_VALUE                  x1, x2;
    size_t                    i, j, l, k;
    MSC_INSTANCE_TIME       p_time;

    l = c_time[s->instances[0]].label;
    x1 = c_time[s->instances[0]].min_Time;
    x2 = c_time[s->instances[0]].max_Time;

    for ( i = 1; i < s->inst_Count; i++ )
    {
    /* if time bases are wrong */
        if ( l != c_time[s->instances[i]].label )
        {
        /* try to find equivalent basis */
            p_time = tat_GetEqTime( c_time[s->instances[i]], M, l );
            if ( p_time.label == l )
            {
                x1 = MSC_MaxTime_( x1, p_time.min_Time );
                x2 = MSC_MaxTime_( x2, p_time.max_Time );
            }
            else
            {
                MSC_TIME_VALUE                  p_x1, p_x2;
                size_t                    p_label;

            /* try to find alternative time base */
                p_label = c_time[s->instances[i]].label;
                p_x1 = c_time[s->instances[i]].min_Time;
                p_x2 = c_time[s->instances[i]].max_Time;
                for ( j = 0; j < i; j++ )
                {
                    p_time = tat_GetEqTime( c_time[s->instances[j]], M, p_label );

                    if ( p_time.label == p_label )
                    {
                        p_x1 = MSC_MaxTime_( p_x1, p_time.min_Time );
                        p_x2 = MSC_MaxTime_( p_x2, p_time.max_Time );
                    }
                    else
                    {
                    /* output Warning message */
                        tat_TimeWarning( M, s, l, c_time[s->instances[i]].label );
                        for ( k = 0; k < s->inst_Count; k++ )
                        {
                            if ( l == c_time[s->instances[k]].label )
                            {
                                x1 = MSC_MaxTime_( x1, c_time[s->instances[k]].min_Time );
                                x2 = MSC_MaxTime_( x2, c_time[s->instances[k]].max_Time );
                            }
                        }
                        goto l_exit;
                    }
                }
            /* re-assign unified time base */
                l = p_label;
                x1 = p_x1;
                x2 = p_x2;
            }
        }
        else
        {
            x1 = MSC_MaxTime_( x1, c_time[s->instances[i]].min_Time );
            x2 = MSC_MaxTime_( x2, c_time[s->instances[i]].max_Time );
        }
    }

    if ( MSC_CompareTime_( x1, x2 ) > 0 )
        x2 = x1;                /* adjust to max time if problems */

  l_exit:
    res->label = l;
    res->min_Time = x1;
    res->max_Time = x2;
    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_MscCalcInst
 *
 * Description: calculate time
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_MscCalcInst( MSC_STORAGE * M, MSC_EVENT_KIND * s, MSC_INSTANCE_TIME * c_time, int flags )
{
    MSC_INSTANCE_TIME      *t_, *t_s, t;
    size_t                  i = 0, j, lab, sizeinst;
    unsigned char          *mode;
    MSC_EVENT_KIND              *x;

/* size for instances */
    sizeinst = M->instance_Count * sizeof ( MSC_INSTANCE_TIME );

/* coregions nesting counter */
    mode = MEM_Allocate( M->instance_Count * sizeof ( char ) );

/* time containers */
    t_ = MEM_Allocate( sizeinst );
    t_s = MEM_Allocate( sizeinst );

    while ( s )
    {
        if ( s->type == MSC_EV_UNDEFINED || s->type == MSC_EV_EMPTY || s->type ==MSC_EV_START_LOOP ||
            s->type==MSC_EV_END_LOOP)
        {
            s = s->next;
            continue;
        }

    /* add MSC_EV_OUT_MESSAGE event into list */
        if ( s->type == MSC_EV_OUT_MESSAGE && s->data.msg.inst >= 0 )
        {
            tat_OutMessages = MEM_Reallocate( tat_OutMessages,
                ( tat_OutMessageCount + 1 ) * sizeof ( tat_OutMessages[0] ) );
            tat_OutMessages[tat_OutMessageCount].ind = s->data.msg.indicator;
            tat_OutMessages[tat_OutMessageCount].s = s;
            tat_OutMessages[tat_OutMessageCount].p = MSC_GetMessageParams( s->data.msg.indicator, M );
            tat_OutMessageCount++;
        }

    /*         tat_GetMultiTime( M, s, c_time, &t );   */

    /* MSC_EV_IN_MESSAGE time is equal to MSC_EV_OUT_MESSAGE + DURATION
       MSC_EV_OUT_MESSAGE is already calculated so we will specify
       MSC_EV_IN_MESSAGE time as absolute or labeled */

        if ( s->type == MSC_EV_IN_MESSAGE && s->data.msg.inst >= 0 )
        {
            i = tat_OutMessageCount;

            while ( i )
            {
                i--;

                if ( tat_OutMessages[i].ind == s->data.msg.indicator )
                {
                    if ( flags & MSCTIME_USE_DURATION )
                    {
                        if ( ( ( tat_OutMessages[i].p )->delay.type ) != MSC_RELATIVE_TIME ||
                            ( tat_OutMessages[i].p )->delay.min_Time == MSC_INFINITY_CONST ||
                            ( tat_OutMessages[i].p )->delay.max_Time == MSC_INFINITY_CONST )
                        {
                            tat_TimeError( M, s );
                        }

                        s->time_Calc = ( tat_OutMessages[i].s )->time_Calc;

                        s->time_Calc.min_Time =
                            MSC_AddMulTime( ( tat_OutMessages[i].s )->time_Calc.min_Time,
                            ( tat_OutMessages[i].p )->delay.min_Time, tat_InstanceTimeScale[s->instances[0]] );
                        s->time_Calc.max_Time =
                            MSC_AddMulTime( ( tat_OutMessages[i].s )->time_Calc.max_Time,
                            ( tat_OutMessages[i].p )->delay.max_Time, tat_InstanceTimeScale[s->instances[0]] );
                    }
                    else
                    {
                    /* zero time duration, by default */
                        s->time_Calc = ( tat_OutMessages[i].s )->time_Calc;
                    }

                    t = c_time[s->instances[0]];
                /* fix multiinstance configurations */
                    if ( t.label == s->time_Calc.time_Label )
                    {
                        s->time_Calc.min_Time = MSC_MaxTime( s->time_Calc.min_Time, t.min_Time );
                    /* if we copy it */

                        if ( s->time_Calc.min_Time == t.min_Time )
                        {
                            s->time_Calc.type = MSC_LABELED_TIME;
                            s->time_Calc.time_Label = t.label;
                            s->time_Calc.max_Time = MSC_MaxTime( s->time_Calc.max_Time, t.max_Time );
                        }
                    }

                    if ( MSC_CompareTime( s->time_Calc.max_Time, s->time_Calc.min_Time ) < 0 )
                    {
                        s->time_Calc.max_Time = s->time_Calc.min_Time;
                    }

                    break;
                }
            } /* while (i) */
            /* mark if not found */
            if ( tat_OutMessages[i].ind != s->data.msg.indicator ) i=MSC_INFINITY_CONST;
        }
    /* handle 'relative' time */
        else if ( s->time.type == MSC_RELATIVE_TIME )
        {
        /* set time relative to label */
            s->time_Calc.type = MSC_LABELED_TIME;

        /* single instance event */
            if ( s->inst_Count == 1 )
            {
                if ( c_time[s->instances[0]].last_Event )
                {
                    if ( !c_time[s->instances[0]].last_Event->labeled )
                    {
                    /* add label to previous event */
                        MSC_TimeLabelEvent( M, c_time[s->instances[0]].last_Event, "TL" );

                    }
                /* Just copy label. Other specification stay unchanged */
                    s->time_Calc.time_Label = c_time[s->instances[0]].last_Event->labeled - 1;
                }
                else
                {
                /* set as MSC_START 'absolute' time */
                    s->time_Calc.time_Label = 0;
                }
                s->time_Calc.min_Time =
                    MSC_MulTime( s->time_Calc.min_Time, tat_InstanceTimeScale[s->instances[0]] );
                s->time_Calc.max_Time =
                    MSC_MulTime( s->time_Calc.max_Time, tat_InstanceTimeScale[s->instances[0]] );
            /* correct -ndefst switch */
                if ( !c_time[s->instances[0]].last_Event )
                {
                    s->time_Calc.min_Time =
                        MSC_AddTime( s->time_Calc.min_Time, c_time[s->instances[0]].min_Time );
                    s->time_Calc.max_Time =
                        MSC_AddTime( s->time_Calc.max_Time, c_time[s->instances[0]].max_Time );
                }
            }
            else
            /* multiple-instance event */
            {
                tat_GetMultiTime( M, s, c_time, &t );
                s->time_Calc.min_Time = MSC_AddTime( t.min_Time, s->time.min_Time );
                s->time_Calc.max_Time = MSC_AddTime( t.max_Time, s->time.max_Time );
                s->time_Calc.time_Label = t.label;
            }
        }                       /* RELATIVE time */
        else if ( s->time.type == MSC_LABELED_TIME )
        {

            MSC_INSTANCE_TIME       x, ev;

        /* fill temporal variable with event's time */
            x.label = s->time.time_Label;
            x.min_Time = s->time.min_Time;
            x.max_Time = s->time.max_Time;

        /* calculate absolute alternative */
            x = tat_GetAbsTime( x, M );

            for ( i = 0; i < s->inst_Count; i++ )
            {
                ev = tat_GetAbsTime( c_time[s->instances[i]], M );
                if ( ev.label == x.label )
                {
                    if ( ( ev.min_Time != MSC_INFINITY_CONST && ev.min_Time > x.min_Time ) ||
                        ( ev.max_Time != MSC_INFINITY_CONST && ev.max_Time > x.max_Time ) )
                    {
                        tat_TimeError( M, s );
                    }
                }
            }
        }

        if ( s->type == MSC_EV_TIMER_TIMEOUT && s->data.timer.timing.type == MSC_SPECIAL_TIMER )
        {
            register MSC_TIME_VALUE         p;

            p = tat_InstanceTimeScale[s->instances[0]];

            s->time_Calc.min_Time =
                MSC_AddMulTime( s->data.timer.relation->time_Calc.min_Time, s->data.timer.timing.min_Time, p );
            s->time_Calc.max_Time =
                MSC_AddMulTime( s->data.timer.relation->time_Calc.max_Time, s->data.timer.timing.max_Time, p );

            s->time_Calc.time_Label = s->data.timer.relation->time_Calc.time_Label;
            s->time_Calc.type = s->data.timer.relation->time_Calc.type;

            lab = s->time_Calc.time_Label;

            if ( MSC_CompareTime( s->time.min_Time, c_time[s->instances[0]].max_Time ) > 0
                && lab == c_time[s->instances[0]].label )
            {
                tat_TimeError( M, s );
            }
        }

        if ( s->type == MSC_EV_CONCURRENT_END )
        {
            mode[s->instances[0]] = 0;
            s->time_Calc.min_Time = c_time[s->instances[0]].min_Time = t_s[s->instances[0]].min_Time;
            s->time_Calc.max_Time = c_time[s->instances[0]].max_Time = t_s[s->instances[0]].max_Time;
            s->time_Calc.type = MSC_LABELED_TIME;
            s->time_Calc.time_Label = t_s[s->instances[0]].label;
        }

    /* propagate 'IN' event time to 'OUT' event */
        if ( s->type == MSC_EV_IN_MESSAGE && (i < tat_OutMessageCount) && ( ( flags & MSCTIME_USE_DURATION ) == 0 ) )
        {
            ( tat_OutMessages[i].s )->time_Calc = s->time_Calc;
        }

        s->time_Calc = tat_UpdateEventTime( s->time_Calc, M );

        if ( s->time_Calc.type != MSC_RELATIVE_TIME )
        {
            for ( i = 0; i < s->inst_Count; i++ )
            {
                if ( mode[s->instances[i]] == 0 )
                {
                    if ( s->labeled )
                    {
                        c_time[s->instances[i]].label = s->labeled - 1;
                        c_time[s->instances[i]].min_Time = 0;
                        c_time[s->instances[i]].max_Time = 0;
                        c_time[s->instances[i]].last_Event = s;
                    }
                    else
                    {
                        c_time[s->instances[i]].label = s->time_Calc.time_Label;
                        c_time[s->instances[i]].min_Time = s->time_Calc.min_Time;
                        c_time[s->instances[i]].max_Time = s->time_Calc.max_Time;
                        c_time[s->instances[i]].last_Event = s;
                    }
                }
                else
                {
                    if ( t_s[s->instances[i]].label == s->time_Calc.time_Label )
                    {
                    /* event within coregion */
                        t_s[s->instances[i]].min_Time =
                            MSC_MaxTime( s->time_Calc.min_Time, t_s[s->instances[i]].min_Time );
                        t_s[s->instances[i]].max_Time =
                            MSC_MaxTime( s->time_Calc.max_Time, t_s[s->instances[i]].max_Time );
                    }
                }
            }
        }

        if ( s->type == MSC_EV_CONCURRENT_START )
        {
            mode[s->instances[0]] = 1;
        }

        lab = 0;

        if ( s->type == MSC_EV_OPERATOR )
        {
            MSC_INSTANCE_TIME       p;
            size_t                  set_time = 1;

        /* try to use current time */
            lab = c_time[0].label;
            p.label = lab;
            p.min_Time = c_time[0].min_Time;
            p.max_Time = c_time[0].max_Time;

            for ( i = 1; i < M->instance_Count; i++ )
            {
                if ( ( c_time[i].label != lab ) || ( c_time[0].min_Time != c_time[i].min_Time ) ||
                    ( c_time[0].max_Time != c_time[i].max_Time ) )
                {
                    lab = MSC_TimeLabelEvent( M, s, "OP_L" ) - 1;
                    p.label = lab;
                    p.min_Time = 0;
                    p.max_Time = 0;
                    break;
                }

            }

            p = tat_GetEqTime( p, M, 0 );

        /* calculate nested frames */

            for ( i = 0; i < s->data.alt.frame_Count; i++ )
            {
            /* propagate label */
                memset( t_, 0, sizeinst );

                for ( j = 0; j < M->instance_Count; j++ )
                {
                    t_[j].label = lab;
                /* last event is current event */
                    t_[j].last_Event = s;
                }

                tat_MscCalcInst( M, s->data.alt.frame[i], t_, flags );

                if ( s->data.alt.type == MSC_OP_PAR )
                {
                    for ( j = 0; j < M->instance_Count; j++ )
                    {
                        t_[j] = tat_GetEqTime( t_[j], M, p.label );

                        if ( p.label == t_[j].label )
                        {
                            c_time[j].min_Time = MSC_MaxTime( c_time[j].min_Time, t_[j].min_Time );
                            c_time[j].max_Time = MSC_MaxTime( c_time[j].max_Time, t_[j].max_Time );
                            c_time[j].label = p.label;
                        }
                        else
                            set_time=0;
                    }
                }

            }

        /* insert_sync label after condition */
            if ( s->next )
            {
                UINT32_T                    insert_sync = 0;

            /* if following event is global condition, do not add ours */
                if ( s->next->type == MSC_EV_CONDITION && s->next->inst_Count == M->instance_Count &&
                    s->next->time.type == MSC_RELATIVE_TIME && s->next->time.min_Time == 0 &&
                    s->next->time.max_Time == 0 )
                {
                    insert_sync = 0;
                    x = s->next;
                }
                else
                {
                    x = MSC_GlobalCond( M );
                    MSC_ZeroEventTime( x );
                }

            /* handle special case of 'PAR' expression */
                if ( s->data.alt.type == MSC_OP_PAR && set_time)
                {
                    t.label = c_time[0].label;
                    t.min_Time = c_time[0].min_Time;
                    t.max_Time = c_time[0].max_Time;

                    for ( j = 1; j < M->instance_Count; j++ )
                    {
                        if ( c_time[j].label == t.label )
                        {
                            t.min_Time = MSC_MaxTime( c_time[j].min_Time, t.min_Time );
                            t.max_Time = MSC_MaxTime( c_time[j].max_Time, t.max_Time );
                        }
                        else break;

                    }
                /* we know interval */
                    if (j == M->instance_Count)
                    {
                        x->time_Calc.min_Time = t.min_Time;
                        x->time_Calc.max_Time = t.max_Time;
                        x->time_Calc.type = MSC_LABELED_TIME;
                        x->time_Calc.time_Label = t.label;
                    }
                }

                x->next = s->next;
                s->next = x;

            /* move label, if next inline expression is labeled */
                if ( x->next && x->next->type == MSC_EV_OPERATOR && x->next->labeled )
                {
                    x->labeled = x->next->labeled;
                    x->next->labeled = 0;
                    M->labels[x->labeled - 1].s = x;
                    lab = x->labeled - 1;
                }
                else
                {
                    lab = MSC_TimeLabelEvent( M, x, "OP_L" ) - 1;
                }

            /* propogate new time to all instances */
                for ( j = 0; j < M->instance_Count; j++ )
                {
                    c_time[j].label = lab;
                    c_time[j].min_Time = 0;
                    c_time[j].max_Time = 0;
                    c_time[j].last_Event = x;
                }

                s = x;
            }
        }

        if ( s )
            s = s->next;
    }

    MEM_Free( mode );
    MEM_Free( t_ );
    MEM_Free( t_s );
    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_UpdateTime
 *
 * Description: update string time specification
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_UpdateTime( MSC_STORAGE * M, MSC_EVENT_KIND * s, int flags, int label )
{
    char                    dest[TAT_MAX_ID_LEN];
    size_t                  i, lab;

    while ( s )
    {
        if ( flags & MSCTIME_TRUNCATE )
        {
            if ( s->time_Calc.min_Time != MSC_INFINITY_CONST )
                s->time_Calc.str_MinTime =
                    MSC_AddStr( MSC_Time2Str( floor( s->time_Calc.min_Time ), dest ), M );
            else
                s->time_Calc.str_MinTime = STRCOLL_STR_ID_INF;

            if ( s->time_Calc.max_Time != MSC_INFINITY_CONST )
                s->time_Calc.str_MaxTime =
                    MSC_AddStr( MSC_Time2Str( floor( s->time_Calc.max_Time ), dest ), M );
            else
                s->time_Calc.str_MaxTime = STRCOLL_STR_ID_INF;
        }
        else
        {
            s->time_Calc.str_MinTime = MSC_AddStr( MSC_Time2Str( s->time_Calc.min_Time, dest ), M );
            s->time_Calc.str_MaxTime = MSC_AddStr( MSC_Time2Str( s->time_Calc.max_Time, dest ), M );
        }

        if ( label && !MSC_IsDefaultEventTime( s ) && ( flags & MSCTIME_ZERO_PAR_TIME ) &&
            ( s->type != MSC_EV_IN_MESSAGE || ( s->type == MSC_EV_IN_MESSAGE && s->data.msg.inst < 0 ) ) )
        {
        /* we should correct output */
            s->time = s->time_Calc;
        }

        if ( s->type == MSC_EV_OPERATOR )
        {
            lab = 0;

            if ( ( flags & MSCTIME_ZERO_PAR_TIME ) && ( s->data.alt.type == MSC_OP_PAR ) )
                lab = s->labeled - 1;

            for ( i = 0; i < s->data.alt.frame_Count; i++ )
            {
                tat_UpdateTime( M, s->data.alt.frame[i], flags, lab );
            }
        }

        s = s->next;
    }

    return RESULT_OK;
}

static LOGICAL tat_CheckLabelUse( MSC_STORAGE * M, MSC_EVENT_KIND * s )
{
    if ( s->time_Calc.type == MSC_LABELED_TIME )
    {
        M->labels[s->time_Calc.time_Label].unused = 0;
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_MscReduceLabel
 *
 * Description: remove unused labels
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_MscReduceLabel( MSC_STORAGE * M )
{
    size_t                    i;

/* mark all as unused */

    for ( i = 0; i < M->label_Count; i++ )
    {
        if ( M->labels[i].generated )
        {
            M->labels[i].unused = 1;
        }
    }

    MSC_Process( M, tat_CheckLabelUse );

/* free all unused */

    for ( i = 0; i < M->label_Count; i++ )
    {
        if ( M->labels[i].unused == 1 && M->labels[i].generated )
        {
            if ( M->labels[i].s )
            {
                M->labels[i].s->labeled = 0;
            }

            M->labels[i].label = 0;
            M->labels[i].s = NULL;
        }
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_CalcTime
 *
 * Description: Calculate and verify time specifications in MSC
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_CalcTime( MSC_STORAGE * M, int flags )
{
    MSC_INSTANCE_TIME      *t;
    size_t                  i;
    MSC_TIME_VALUE                  startt, finisht;

    tat_OutMessages = NULL;
    tat_OutMessageCount = 0;

/* min is allocated and filled with zero */
    t = MEM_Allocate( M->instance_Count * sizeof ( MSC_INSTANCE_TIME ) );
    tat_InstanceTimeScale = MEM_Allocate( M->instance_Count * sizeof ( tat_InstanceTimeScale[0] ) );
    startt = 0;

    tat_TranspLabel = flags & MSCTIME_TRANS_LABEL;

/* pre-calculate instance's time scales */
    for ( i = 0; i < M->instance_Count; i++ )
    {
        tat_InstanceTimeScale[i] = MSC_Str2Dbl( MSC_GetString( M->instances[i].ratio, M ) );
    }

    if ( M->time_Offset )
    {
        startt = MSC_Str2Dbl( MSC_GetString( M->time_Offset, M ) );
        for ( i = 0; i < M->instance_Count; i++ )
        {
            t[i].min_Time = startt;
        }
    }

    finisht = ( flags & MSCTIME_DEFINED_START ) ? startt : MSC_INFINITY_CONST;

    if ( finisht )
    {
        for ( i = 0; i < M->instance_Count; i++ )
        {
            t[i].max_Time = finisht;
        }
    }

/* add starting Label for absolute time */
    if ( M->labels[0].label == 0 )
    {
        M->labels[0].label = MSC_AddStr( STR_MSCSTART, M );
    }

    if ( !tat_MscCalcInst( M, M->start, t, flags ) )
    {
        MEM_Free( t );
        return RESULT_ERR;
    }

/* set '1' time scale */
    for ( i = 0; i < M->instance_Count; i++ )
        M->instances[i].ratio = STRCOLL_STR_ID_1;

    if ( tat_OutMessages )
        MEM_Free( tat_OutMessages );

    tat_UpdateTime( M, M->start, flags, 0 );
    tat_MscReduceLabel( M );

/* MSC_Transform(M,MSC_TRANS_OPER_SYNC); */
    MEM_Free( t );
    MEM_Free( tat_InstanceTimeScale );

    return RESULT_OK;
}
