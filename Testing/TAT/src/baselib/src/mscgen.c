/******************************************************************************
*
*       COPYRIGHT 2001-2004 MOTOROLA, ALL RIGHTS RESERVED
*
*       MOTOROLA CONFIDENTIAL PROPRIETARY
*
* Description:  Command-line processing
*
* $RCSfile: mscgen.c,v $
* $Revision: 1.3 $
* $Source: /home/cvsusr/cvssoft/tat_tools/baselib/src/mscgen.c,v $
* $Date: 2004/03/05 14:37:44 $
*
*
* CHANGE HISTORY
*                                                       
* Initial version 
*
******************************************************************************/
#include <portable.h>
#include <qmemory.h>
#include <fileproc.h>
#include <mscbase.h>

#include <mscparse.h>
#include <mscproc.h>
#include <mscgen.h>
#include <outmess.h>
#include <mscmess.h>
#include <mscstrs.h>
#include <tatgdata.h>
#include <strproc.h>

int                     MSC_VerdictTrace = 0;

static char * msc_GenTime( char * dest, MSC_TIME_INTERVAL * T, MSC_STORAGE * M, const UINT32_T flags );
static char * msc_GenEventSpec( char * dest, MSC_EVENT_KIND * event, MSC_STORAGE * M, const UINT32_T flags );

/***************************************************************************
 * Function : MSC_GenComment
 *
 * Description: generate 'comment' field of event
 *
 * Returns: pointer to next character
 *
 * Notes: None
 *
 **************************************************************************/
char * MSC_GenComment( char * dest, MSC_EVENT_KIND * event, MSC_STORAGE * M, const UINT32_T flags )
{
    int                     t;
    UINT32_T                    i;

    if ( !dest )
        return NULL;

    /* atomic references */
    if ( event->type == MSC_EV_REFERENCE &&  
         ( flags & MSC_MPR_OUTPUT_REF_PAR ) &&  event->data.ref.ref_Type==PTK_REF_PARAM)
    {
        strcpy( dest, " comment '/*<+(" );
        dest += strlen( dest );
        if (event->data.ref.param_Count)
        {
            for ( i = 0; i < event->data.ref.param_Count - 1; i++ )
            {
                sprintf( dest, "%s,",
                    event->data.ref.refr_Subst[i] ? MSC_GetStr( event->data.ref.refr_Subst[i], M ) : "" );
                dest += strlen( dest );
            }

            sprintf( dest, "%s",
                event->data.ref.refr_Subst[i] ? MSC_GetStr( event->data.ref.refr_Subst[i], M ) : "" );
            dest += strlen( dest );
        }
        strcpy( dest, ")>*/" );
        dest += 4;
        *dest++ = ';';
        *dest = 0;
        return dest;
    }

    t = ( ( ( flags & MSC_MPR_OUTPUT_TIME ) && event->labeled ) || event->comment ||
        ( ( flags & MSC_MPR_OUTPUT_EVMARK ) && event->event_Mark ) ||
        ( ( flags & MSC_MPR_OUTPUT_VERDICT ) && event->verdict_Count ) ||
        ( ( flags & MSC_MPR_OUTPUT_EVCOMMENT ) && msc_GenEventSpec(NULL, event, M, flags)) ) ? 1 : 0;

    if ( t )
    {
        strcpy( dest, " comment '" );
        dest += strlen( dest );
    }

    if (flags & MSC_MPR_OUTPUT_EVCOMMENT)
    {
        dest  = msc_GenEventSpec(dest, event, M, flags);
    }


    if ( ( flags & MSC_MPR_OUTPUT_TIME ) && event->labeled )
    {
        *dest++ = '@';
        strcpy( dest, MSC_GetStr( M->labels[event->labeled - 1].label, M ) );
        dest += strlen( dest );

        if ( event->comment )
        {
            *dest++ = ' ';
            *dest = 0;
        }
    }

    if ( event->comment )
    {
        StrEmbedMSCtext( dest, MSC_GetStr( event->comment, M ) );
        dest += strlen( dest );
    }

    if ( ( flags & MSC_MPR_OUTPUT_EVMARK ) && event->event_Mark )
    {
        sprintf( dest, "<%u>", event->event_Mark );
        dest += strlen( dest );
    }

    if ( ( flags & MSC_MPR_OUTPUT_VERDICT ) && event->verdict_Count )
    {
        for ( i = 0; i < event->verdict_Count; i++ )
        {
        /* skip verdicts for another traces */
            if ( ( flags & MSC_MPR_OUTPUT_VRSTIME ) && event->verdict[i].trace != MSC_VerdictTrace )
                continue;
            *dest++ = '\n';
            *dest++ = '<';
            *dest++ = '!';
            strcpy( dest, VRS_EventVerdictText[event->verdict[i].type - 1] );
            dest += strlen( dest );
            if ( event->verdict[i].type == MSC_VRS_PROTOCOL_APPLIED ||
                event->verdict[i].type == MSC_VRS_PROTOCOL_COLLISION )
            {
                if ( event->verdict[i].type == MSC_VRS_PROTOCOL_APPLIED )
                {
                    sprintf( dest, " %s %u", MSC_GetStr( event->verdict[i].protocol_Name, M ),
                        event->verdict[i].protocol_Num );
                }
                else
                {
                    sprintf( dest, " %s %u, %s %u", MSC_GetStr( event->verdict[i].protocol_Name, M ),
                        event->verdict[i].protocol_Num, MSC_GetStr( event->verdict[i].protocol_Name2, M ),
                        event->verdict[i].protocol_Num2 );
                }
                dest += strlen( dest );
            }
            sprintf( dest, " (Trace %u, Event %u)", event->verdict[i].trace, event->verdict[i].event );
            dest += strlen( dest );

            if ( event->verdict[i].type == MSC_VRS_ABSOLUTE_TIME ||
                event->verdict[i].type == MSC_VRS_OVERLAPED_WITH )
            {
                dest = msc_GenTime( dest, &event->verdict[i].time_VRS, M, flags );
            }
            *dest++ = '!';
            *dest++ = '>';
            *dest = 0;
        }
    }

    if ( t )
        *dest++ = '\'';

    if ( event->src_Count && ( flags & MSC_MPR_OUTPUT_SREF ) )
    {
        *dest++ = '/';
        *dest++ = '*';

        for ( i = 0; i < event->src_Count; i++ )
        {
            sprintf( dest, "[%u,%u]", event->src_Data[i].link_Index, event->src_Data[i].line_Number );
            dest += strlen( dest );
        }
        *dest++ = '*';
        *dest++ = '/';

    }
    *dest++ = ';';

    *dest = 0;

    return dest;
}

/***************************************************************************
 * Function : msc_GenTime
 *
 * Description: output time structure
 *
 * Returns: pointer to dest
 *
 * Notes: None
 *
 **************************************************************************/
static char * msc_GenTime( char * dest, MSC_TIME_INTERVAL * T, MSC_STORAGE * M, const UINT32_T flags )
{
    if ( !dest )
        return NULL;

/* skip for default time */
    if ( !( flags & MSC_MPR_OUTPUT_DIAG ) &&
        ( ( T->type != MSC_LABELED_TIME ) || ( T->type == MSC_LABELED_TIME && T->time_Label == 0 ) )
        && T->str_MinTime == STRCOLL_STR_ID_0 && T->str_MaxTime == STRCOLL_STR_ID_INF )
        return dest;

    *dest++ = '{';

    *dest = 0;

    if ( T->type == MSC_LABELED_TIME )
    {
        *dest++ = '@';

        if ( T->time_Label )
        {
            strcpy( dest, MSC_GetStr( M->labels[T->time_Label].label, M ) );
            dest += strlen( dest );

            if ( T->str_MinTime != STRCOLL_STR_ID_0 || T->str_MaxTime != STRCOLL_STR_ID_INF )
            {
                *dest++ = '+';
                *dest = 0;
            }
        }
    }

    if ( ( T->str_MinTime != STRCOLL_STR_ID_0 || T->str_MaxTime != STRCOLL_STR_ID_INF ||
            ( flags & MSC_MPR_OUTPUT_DIAG ) ) )
    {
        strcpy( dest, MSC_GetStr( T->str_MinTime, M ) );
        dest += strlen( dest );

    /* do not output the same counter */
        if ( T->str_MaxTime && T->str_MaxTime != T->str_MinTime )
        {
            *dest++ = '.';
            *dest++ = '.';
            *dest = 0;
            strcpy( dest, MSC_GetStr( T->str_MaxTime, M ) );
            dest += strlen( dest );
        }
    }

    *dest++ = '}';
    *dest = 0;
    return dest;
}

/***************************************************************************
 * Function : msc_GenEventSpec
 *
 * Description: Output timer event
 *
 * Returns: 
 *
 * Notes: None
 *
 **************************************************************************/
static char * msc_GenEventSpec( char * dest, MSC_EVENT_KIND * event, MSC_STORAGE * M, const UINT32_T flags )
{
    int                     output;
    size_t                  i;
    MSC_TIME_INTERVAL *T = NULL;

    if ( !event || !M )
        return NULL;

    if ( ( flags & MSC_MPR_OUTPUT_VRSTIME ) && event->verdict_Count )
    {
        for ( i = 0; i < event->verdict_Count; i++ )
        {
            if ( event->verdict[i].trace == MSC_VerdictTrace &&
                ( event->verdict[i].type == MSC_VRS_ABSOLUTE_TIME ) )
            {
                T = &( event->verdict[i].time_VRS );
                break;
            }
        }
        if ( i == event->verdict_Count )
        {
            T = ( ( flags & MSC_MPR_OUTPUT_CALCTIME ) ? &( event->time_Calc ) : &( event->time ) );
        }
    }
    else
    {
        T = ( ( flags & MSC_MPR_OUTPUT_CALCTIME ) ? &( event->time_Calc ) : &( event->time ) );
    }

    output = ( flags & MSC_MPR_OUTPUT_DIAG ) ||
        ( ( ( ( flags & MSC_MPR_OUTPUT_SYNC ) && event->sync == MSC_SYNC_EVENT ) ||
            ( ( flags & MSC_MPR_OUTPUT_EVCOND ) && event->condition ) ||
            ( ( flags & MSC_MPR_OUTPUT_TIME ) &&
                ( ( ( T->type == MSC_LABELED_TIME && T->time_Label != 0 ) ||
                        T->str_MinTime != STRCOLL_STR_ID_0 ||
                        T->str_MaxTime != STRCOLL_STR_ID_INF ) &&
                    !( event->type == MSC_EV_OPERATOR && T->type == MSC_RELATIVE_TIME &&
                        T->str_MinTime == STRCOLL_STR_ID_0 &&
                        T->str_MaxTime == STRCOLL_STR_ID_0 ) ) ) ) &&
        !( ( event->type == MSC_EV_TIMER_SET || event->type == MSC_EV_TIMER_RESET ||
                event->type == MSC_EV_TIMER_TIMEOUT || event->type == MSC_EV_CONCURRENT_START ||
                event->type == MSC_EV_CONCURRENT_END ) ) );

    if (!dest)
        return NULL;

    if (!(flags & MSC_MPR_OUTPUT_EVCOMMENT) || event->type==MSC_EV_OPERATOR) *dest++ = ' ';
    *dest = 0;

    if ( !output )
        return dest;

    *dest++ = '/';
    *dest++ = '*';
    *dest = 0;

    if ( ( flags & MSC_MPR_OUTPUT_SYNC ) && event->sync == MSC_SYNC_EVENT )
    {
        *dest++ = '~';
        *dest = 0;
    }

    if ( event->condition && ( flags & MSC_MPR_OUTPUT_EVCOND ) )
    {
        *dest++ = '[';
        *dest = 0;
        strcpy( dest, MSC_GetStr( event->condition, M ) );
        dest += strlen( dest );
        *dest++ = ']';
        *dest = 0;
    }

    if ( ( flags & MSC_MPR_OUTPUT_TIME ) &&
        ( ( event->type != MSC_EV_TIMER_SET && event->type != MSC_EV_TIMER_RESET &&
                event->type != MSC_EV_TIMER_TIMEOUT && event->type != MSC_EV_CONCURRENT_START &&
                event->type != MSC_EV_CONCURRENT_END ) || ( flags & MSC_MPR_OUTPUT_DIAG ) ) )
    {
        dest = msc_GenTime( dest, T, M, flags );
    }

    *dest++ = '*';
    *dest++ = '/';
    *dest = 0;
    return dest;
}

static char            *msc_GenEventName( char * dest, MSC_EVENT_KIND * event )
{
    if ( !dest )
        return NULL;

    switch ( event->type )
    {

    case MSC_EV_UNDEFINED:
        break;

    case MSC_EV_REFERENCE:
    {
        APPEND_STR( dest, STR_REFERENCE );
        break;
    }

    case MSC_EV_CONDITION:
    {
        APPEND_STR( dest, STR_CONDITION );
        break;
    }

    case MSC_EV_CODE:
    {
        APPEND_STR( dest, STR_ACTION );
        break;
    }

    case MSC_EV_ACTION:
    {
        APPEND_STR( dest, STR_ACTION );
        break;
    }

    case MSC_EV_TIMER_SET:
    {
        APPEND_STR( dest, STR_SET );
        break;
    }

    case MSC_EV_TIMER_RESET:
    {
        APPEND_STR( dest, STR_RESET );
        break;
    }

    case MSC_EV_TIMER_TIMEOUT:
    {
        APPEND_STR( dest, STR_TIMEOUT );
        break;
    }

    case MSC_EV_CONCURRENT_START:
    {
        APPEND_STR( dest, STR_CONCURRENT );
        break;
    }

    case MSC_EV_CONCURRENT_END:
    {
        APPEND_STR( dest, STR_ENDCONCURRENT );
        break;
    }

    case MSC_EV_OPERATOR:
        break;

    case MSC_EV_IN_MESSAGE:
    {
        APPEND_STR( dest, STR_IN );
        break;
    }

    case MSC_EV_OUT_MESSAGE:
    {
        APPEND_STR( dest, STR_OUT );
        break;
    }

    case MSC_EV_SEND_SIGNAL:
    {
        APPEND_STR( dest, STR_SEND );
        break;
    }

    case MSC_EV_RECV_SIGNAL:
    {
        APPEND_STR( dest, STR_RECEIVE );
        break;
    }

    case MSC_EV_EXIST_SIGNAL:
    {
        APPEND_STR( dest, STR_EXIST );
        break;
    }

    case MSC_EV_CREATE:
    {
        APPEND_STR( dest, STR_CREATE );
        break;
    }

    case MSC_EV_STOP:
    {
        APPEND_STR( dest, STR_STOP );
        break;
    }
    }

    return dest;
}

/***************************************************************************
 * Function : MSC_GenerateLine
 *
 * Description: Generate text representation of event
 *
 * Returns: dest/NULL
 *
 * Notes: None
 *
 **************************************************************************/
char * MSC_GenerateLine( char * dest, MSC_EVENT_KIND * event, MSC_STORAGE * M, const UINT32_T flags )
{
    MSC_MESSAGE_STORAGE    *message_Params;
    char                   *t;
    UINT32_T                    i;

/* check for correct input parameters */
    if ( !dest || !event || !M )
    {
        return NULL;
    }

/* check for allowed event types */
    if ( event->type == MSC_EV_UNDEFINED || event->type == MSC_EV_OPERATOR || event->type == MSC_EV_BREAK )
    {
        return NULL;
    }

    message_Params = NULL;

/* event should not exist */

    t = dest;

    dest[0] = 0;

    if ( event->type == MSC_EV_EMPTY || event->type == MSC_EV_END_LOOP ||
        ( event->type == MSC_EV_START_LOOP && event->data.loop.text == 0 ) )
    {
        return dest;
    }

    if ( ( event->type == MSC_EV_TIMER_SET || event->type == MSC_EV_TIMER_RESET ||
            event->type == MSC_EV_TIMER_TIMEOUT ) && ( event->data.timer.timing.type == MSC_SPECIAL_TIMER )
        && ( ( !( flags & MSC_MPR_OUTPUT_TIME ) ) || ( flags & MSC_MPR_EXCLUDE_STIMER ) ) )
    {
        return dest;
    }

    if ( event->type == MSC_EV_IN_MESSAGE || event->type == MSC_EV_OUT_MESSAGE ||
        event->type == MSC_EV_RECV_SIGNAL || event->type == MSC_EV_SEND_SIGNAL ||
        event->type == MSC_EV_EXIST_SIGNAL )
    {
        message_Params = MSC_GetMessageParams( event->data.msg.indicator, M );

        if ( message_Params == NULL )
        {
            return NULL;
        }
    }

    if ( event->type == MSC_EV_START_LOOP && event->data.loop.text )
    {
        sprintf( dest, "all: condition WHILE /* %s */;\n", MSC_GetStr( event->data.loop.text, M ) );
        return dest;
    }

    if ( !( flags & MSC_MPR_OUTPUT_ALL_EVENTS ) )
    {
        if ( event->type == MSC_EV_SEND_SIGNAL || event->type == MSC_EV_EXIST_SIGNAL ||
            event->type == MSC_EV_RECV_SIGNAL )
        {
            MSC_EVENT_KIND_TYPE          saved_type;
            STRING_ID              saved_comment;
            size_t                  saved_label;

            saved_type = event->type;
            saved_comment = event->comment;
            saved_label = event->labeled;
            if ( event->type == MSC_EV_SEND_SIGNAL || event->type == MSC_EV_EXIST_SIGNAL )
            {

                if ( event->data.msg.inst == MSC_FOUND_INSTANCE_ID )
                {
                    event->type = MSC_EV_IN_MESSAGE;

                    if ( !MSC_GenerateLine( dest, event, M, flags ) )
                    {
                        event->type = saved_type;
                        return NULL;
                    }
                }
                else
                {
                    event->type = MSC_EV_OUT_MESSAGE;

                    if ( !MSC_GenerateLine( dest, event, M, flags ) )
                    {
                        event->type = saved_type;
                        return NULL;
                    }
                    event->comment = 0;
                    event->labeled = 0;
                    event->type = MSC_EV_IN_MESSAGE;
                    i = event->data.msg.inst;
                    event->data.msg.inst = (int)event->instances[0];
                    event->instances[0] = i;
                    dest += strlen( dest );
                    *dest++ = '\n';
                    *dest = 0;
                    dest = MSC_GenerateLine( dest, event, M, flags );

                    event->comment = saved_comment;
                    event->labeled = saved_label;

                    i = event->data.msg.inst;
                    event->data.msg.inst = (int)event->instances[0];
                    event->instances[0] = i;
                }

                event->type = saved_type;
                return t;
            }
            else if ( event->type == MSC_EV_RECV_SIGNAL )
            {
                if ( event->data.msg.inst == MSC_LOST_INSTANCE_ID )
                {
                    event->type = MSC_EV_OUT_MESSAGE;

                    if ( !MSC_GenerateLine( dest, event, M, flags ) )
                    {
                        event->type = MSC_EV_RECV_SIGNAL;
                        return NULL;
                    }
                }
                else
                {
                    event->type = MSC_EV_OUT_MESSAGE;

                    if ( !MSC_GenerateLine( dest, event, M, flags ) )
                    {
                        event->type = MSC_EV_RECV_SIGNAL;
                        return NULL;
                    }
                    event->comment = 0;
                    event->labeled = 0;
                    event->type = MSC_EV_IN_MESSAGE;
                    i = event->data.msg.inst;
                    event->data.msg.inst = (int)event->instances[0];
                    event->instances[0] = i;
                    dest += strlen( dest );
                    *dest++ = '\n';
                    *dest = 0;
                    dest = MSC_GenerateLine( dest, event, M, flags );
                    event->comment = saved_comment;
                    event->labeled = saved_label;

                    i = event->data.msg.inst;
                    event->data.msg.inst = (int)event->instances[0];
                    event->instances[0] = i;
                }

                event->type = MSC_EV_RECV_SIGNAL;
                return t;
            }

        }

    }

/* force single instance for special elements */
    if ( event->type == MSC_EV_CODE || event->type == MSC_EV_ACTION || event->type == MSC_EV_TIMER_SET
        || event->type == MSC_EV_TIMER_RESET || event->type == MSC_EV_TIMER_TIMEOUT ||
        event->type == MSC_EV_IN_MESSAGE || event->type == MSC_EV_OUT_MESSAGE ||
        event->type == MSC_EV_SEND_SIGNAL || event->type == MSC_EV_EXIST_SIGNAL )
    {
        sprintf( dest, "%s: ", MSC_GetInstanceName( event->instances[0], M ) );
        dest += strlen( dest );
    }
    else if ( event->type == MSC_EV_RECV_SIGNAL )
    {
    /* swap instances for receive signal event */
        sprintf( dest, "%s: ", MSC_GetInstanceName( event->data.msg.inst, M ) );
        dest += strlen( dest );
    }
    else
    {
    /* special hack to avoid problems */
        if ( ( UINT32_T ) event->inst_Count >= ( UINT32_T ) M->instance_Count )
        {
            sprintf( dest, "all: " );
            dest += 5;
        }
        else
        {
        /* print instances */
            for ( i = 0; i < event->inst_Count - 1; i++ )
            {
                sprintf( dest, "%s,", MSC_GetInstanceName( event->instances[i], M ) );
                dest += strlen( dest );
            }

            sprintf( dest, "%s: ", MSC_GetInstanceName( event->instances[i], M ) );
            dest += strlen( dest );
        }
    }

    dest = msc_GenEventName( dest, event );

    if ( event->type != MSC_EV_CONCURRENT_END && event->type != MSC_EV_CONCURRENT_START &&
        event->type != MSC_EV_ACTION && event->type != MSC_EV_CODE && !( flags & MSC_MPR_OUTPUT_EVCOMMENT ))
        dest = msc_GenEventSpec( dest, event, M, flags );

    switch ( event->type )
    {

    case MSC_EV_UNDEFINED:
        return t;

    case MSC_EV_REFERENCE:
    {
        if ( *( dest - 1 ) != ' '  && *( dest - 1 ) != '/')
            *dest++ = ' ';

        strcpy( dest, MSC_GetStr( event->data.ref.refr_Name, M ) );

        dest += strlen( dest );

        if ( event->data.ref.param_Count && ( flags & MSC_MPR_OUTPUT_REF_PAR ) &&  event->data.ref.ref_Type==TAT_REF_PARAM)
        {
            strcpy( dest, " /* " );
            dest += 4;

            for ( i = 0; i < event->data.ref.param_Count - 1; i++ )
            {
                sprintf( dest, "%s=%s,",
                    event->data.ref.macro_Name[i],
                    event->data.ref.refr_Subst[i] ? MSC_GetStr( event->data.ref.refr_Subst[i], M ) : "" );
                dest += strlen( dest );
            }

            sprintf( dest, "%s=%s",
                event->data.ref.macro_Name[i],
                event->data.ref.refr_Subst[i] ? MSC_GetStr( event->data.ref.refr_Subst[i], M ) : "" );
            dest += strlen( dest );
            strcpy( dest, " */" );
            dest += 3;
        }

        break;
    }

    case MSC_EV_CONDITION:
    {
        if ( *( dest - 1 ) != ' '  && *( dest - 1 ) != '/')
            *dest++ = ' ';

        strcpy( dest, MSC_GetStr( event->data.cond.text, M ) );

        dest += strlen( dest );

        if ( event->data.cond.call )
        {
            sprintf( dest, " /* %s */", MSC_GetStr( event->data.cond.call, M ) );
            dest += strlen( dest );
        }

        break;
    }

    case MSC_EV_CODE:

    case MSC_EV_ACTION:
    {
        if ( *( dest - 1 ) != ' '  && *( dest - 1 ) != '/')
        {
            *dest++ = ' ';
        }
        *dest++ = '\'';
        StrEmbedMSCtext( dest, MSC_GetStr( event->data.text, M ) );
        dest += strlen( dest );
        *dest++ = '\'';
        break;
    }

    case MSC_EV_TIMER_SET:
    {
        if ( *( dest - 1 ) != ' '  && *( dest - 1 ) != '/')
        {
            *dest++ = ' ';
        }

        if ( event->data.timer.timing.type == MSC_SPECIAL_TIMER )
        {
            if ( event->data.timer.timing.str_MaxTime &&
                event->data.timer.timing.str_MaxTime != event->data.timer.timing.str_MinTime )
            {
                sprintf( dest, "{%s..%s},Tmr%u",
                    MSC_GetStr( event->data.timer.timing.str_MinTime, M ),
                    MSC_GetStr( event->data.timer.timing.str_MaxTime, M ), event->data.timer.timer_ID );
            }
            else
            {
                sprintf( dest, "{%s},Tmr%u",
                    MSC_GetStr( event->data.timer.timing.str_MinTime, M ), event->data.timer.timer_ID );

            }
            dest += strlen( dest );
        }
        else if ( event->data.timer.timing.type == MSC_ABSOLUTE_TIMER ||
            event->data.timer.timing.type == MSC_RELATIVE_TIMER )
        {
            sprintf( dest, "%s,Tmr%u", MSC_GetStr( event->data.timer.signal_Name, M ),
                event->data.timer.timer_ID );
            dest += strlen( dest );

            if ( event->data.timer.timing.str_MaxTime )
            {
                if ( event->data.timer.timing.type == MSC_ABSOLUTE_TIMER )
                    sprintf( dest, "/* #SDTMSCPAR(%s) */", MSC_GetStr( event->data.timer.timing.str_MaxTime,
                            M ) );
                else
                    sprintf( dest, "/* #SDTMSCPAR(NOW+%s) */",
                        MSC_GetStr( event->data.timer.timing.str_MaxTime, M ) );

                dest += strlen( dest );
            }
        }

        break;
    }

    case MSC_EV_TIMER_RESET:

    case MSC_EV_TIMER_TIMEOUT:
    {
        if ( *( dest - 1 ) != ' '  && *( dest - 1 ) != '/')
        {
            *dest++ = ' ';
        }

        if ( event->data.timer.timing.type == MSC_SPECIAL_TIMER )
        {
            if ( event->data.timer.timing.str_MaxTime &&
                event->data.timer.timing.str_MaxTime != event->data.timer.timing.str_MinTime )
            {
                sprintf( dest, "{%s..%s},Tmr%u",
                    MSC_GetStr( event->data.timer.timing.str_MinTime, M ),
                    MSC_GetStr( event->data.timer.timing.str_MaxTime, M ), event->data.timer.timer_ID );
            }
            else
            {
                sprintf( dest, "{%s},Tmr%u",
                    MSC_GetStr( event->data.timer.timing.str_MinTime, M ), event->data.timer.timer_ID );

            }
            dest += strlen( dest );
        }
        else if ( event->data.timer.timing.type == MSC_ABSOLUTE_TIMER ||
            event->data.timer.timing.type == MSC_RELATIVE_TIMER )
        {
            sprintf( dest, "%s,Tmr%u", MSC_GetStr( event->data.timer.signal_Name, M ),
                event->data.timer.timer_ID );
            dest += strlen( dest );
        }

        break;
    }

    case MSC_EV_CONCURRENT_START:
        break;

    case MSC_EV_CONCURRENT_END:
        break;

    case MSC_EV_IN_MESSAGE:
    case MSC_EV_OUT_MESSAGE:
    case MSC_EV_SEND_SIGNAL:
    case MSC_EV_RECV_SIGNAL:
    case MSC_EV_EXIST_SIGNAL:
    {
        if ( *( dest - 1 ) != ' ' && *( dest - 1 ) != '/')
            *dest++ = ' ';

        strcpy( dest, MSC_GetStr( message_Params->message_Name, M ) );

        dest += strlen( dest );

        if ( ( flags & MSC_MPR_OUTPUT_DURATION ) &&
            ( message_Params->delay.str_MinTime != STRCOLL_STR_ID_0 ||
                message_Params->delay.str_MaxTime != STRCOLL_STR_ID_0 ) )
        {
            *dest++ = '/';
            *dest++ = '*';
            *dest = 0;
            dest = msc_GenTime( dest, &( message_Params->delay ), M, flags );
            *dest++ = '*';
            *dest++ = '/';
            *dest = 0;
        }

        *dest++ = ',';
        sprintf( dest, "%u", message_Params->indicator );
        dest += strlen( dest );

        if ( message_Params->param_Count )
        {
            strcpy( dest, "(" );
            dest++;

            for ( i = 0; i < message_Params->param_Count - 1; i++ )
            {
                if ( message_Params->message_Params[i] )
                    strcpy( dest, MSC_GetStr( message_Params->message_Params[i], M ) );

                dest += strlen( dest );

                *dest++ = ',';
                *dest++ = ' ';

                *dest = 0;
            }

            if ( message_Params->message_Params[i] )
            {
                strcpy( dest, MSC_GetStr( message_Params->message_Params[i], M ) );
            }

            dest += strlen( dest );
            *dest++ = ')';
            *dest = 0;
        }

        if ( ( event->type == MSC_EV_IN_MESSAGE ) || ( event->type == MSC_EV_RECV_SIGNAL ) )
        {
            strcpy( dest, " from " );
            dest += 6;
        }
        else
        {
            strcpy( dest, " to " );
            dest += 4;
        }

    /* swap instances for receive signal event */
        if ( event->type == MSC_EV_RECV_SIGNAL )
        {
            strcpy( dest, MSC_GetInstanceName( event->instances[0], M ) );
        }
        else
        {
            strcpy( dest, MSC_GetInstanceName( event->data.msg.inst, M ) );
        }

        dest += strlen( dest );
        break;
    }

    case MSC_EV_CREATE:
    {
        if ( *( dest - 1 ) != ' '  && *( dest - 1 ) != '/')
        {
            *dest++ = ' ';
        }

        strcpy( dest, MSC_GetStr( M->instances[( int ) event->data.create.instance].name, M ) );

        dest += strlen( dest );

        if ( event->data.create.param_Count )
        {
            strcpy( dest, "( " );
            dest += 2;

            for ( i = 0; i < event->data.create.param_Count - 1; i++ )
            {
                if ( event->data.create.params[i] )
                    strcpy( dest, MSC_GetStr( event->data.create.params[i], M ) );

                dest += strlen( dest );
                *dest++ = ',';
                *dest++ = ' ';
                *dest = 0;
            }

            if ( event->data.create.params[i] )
                strcpy( dest, MSC_GetStr( event->data.create.params[i], M ) );

            dest += strlen( dest );

            strcpy( dest, " )" );

            dest += 2;
        }

        break;
    }

    case MSC_EV_STOP:
        break;
    }

    dest = MSC_GenComment( dest, event, M, flags );
    return t;
}

static char            *ended;         /* Storage for MSC instance state */

/***************************************************************************
 * Function : MSC_GenStartOp
 *
 * Description: generate start statement of MSC's inline expression
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_GenStartOp( char * dest, MSC_EVENT_KIND * event, MSC_STORAGE * M, const UINT32_T flags )
{
    char                    buffer[TAT_MAX_LINE_LEN];
    UINT32_T                    i;

    if ( !event )
        return RESULT_ERR;

    if ( event->type != MSC_EV_OPERATOR )
        return RESULT_ERR;

    *dest = 0;

    if ( ( size_t ) event->inst_Count == ( size_t ) M->instance_Count )
    {
        sprintf( dest, "all: " );
        dest += 5;
    }
    else
    {
    /* print instances */

        for ( i = 0; i < event->inst_Count - 1; i++ )
        {
            sprintf( dest, "%s,", MSC_GetStr( M->instances[( int ) event->instances[i]].name, M ) );
            dest += strlen( dest );
        }

        sprintf( dest, "%s: ", MSC_GetStr( M->instances[( int ) event->instances[i]].name, M ) );
        dest += strlen( dest );
    }

    msc_GenEventSpec( buffer, event, M, flags );

    switch ( event->data.alt.type )
    {

    case MSC_OP_SEQ:
        break;

    case MSC_OP_ALT:
    {
        sprintf( dest, "alt%sbegin", buffer );
        break;
    }

    case MSC_OP_PAR:
    {
        sprintf( dest, "par%sbegin", buffer );
        break;
    }

    case MSC_OP_EXC:
    {
        sprintf( dest, "exc%sbegin", buffer );
        break;
    }

    case MSC_OP_OPT:
    {
        sprintf( dest, "opt%sbegin", buffer );
        break;
    }

    case MSC_OP_LOOP:
    {
        char                   *l1;

        l1 = MSC_GetStr( event->data.alt.l1, M );

        if ( event->data.alt.l2 && event->data.alt.l2 != event->data.alt.l1 )
        {
            sprintf( dest, "loop%s< %s, %s > begin", buffer, l1, MSC_GetStr( event->data.alt.l2, M ) );
        }

    /* check for single zero loop counter */
        else
        {
            if ( event->data.alt.l1 )
                sprintf( dest, "loop%s<%s> begin", buffer, l1 );
            else
                sprintf( dest, "loop%sbegin", buffer );
        }

        break;
    }
    }

    MSC_GenComment( buffer, event, M, flags );
    dest += strlen( dest );
    strcpy( dest, buffer );

    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_GenEndOp
 *
 * Description: generate finish statement of inline expression
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_GenEndOp( char * dest, MSC_EVENT_KIND * event, MSC_STORAGE * M, const UINT32_T flags )
{
    if ( !event )
        return RESULT_ERR;

    if ( event->type != MSC_EV_OPERATOR )
        return RESULT_ERR;

    *dest = 0;

    switch ( event->data.alt.type )
    {

    case MSC_OP_SEQ:
        break;

    case MSC_OP_ALT:
    {
        strcpy( dest, "alt end;" );
        break;
    }

    case MSC_OP_PAR:
    {
        strcpy( dest, "par end;" );
        break;
    }

    case MSC_OP_EXC:
    {
        strcpy( dest, "exc end;" );
        break;
    }

    case MSC_OP_OPT:
    {
        strcpy( dest, "opt end;" );
        break;
    }

    case MSC_OP_LOOP:
    {
        strcpy( dest, "loop end;" );
        break;
    }
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_GenSepOp
 *
 * Description: generate separator for inline expressions
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_GenSepOp( char * dest, MSC_EVENT_KIND * event, MSC_STORAGE * M, const UINT32_T flags )
{
    if ( !event )
        return RESULT_ERR;

    if ( event->type != MSC_EV_OPERATOR )
        return RESULT_ERR;

    *dest = 0;

    switch ( event->data.alt.type )
    {

    case MSC_OP_SEQ:
        break;

    case MSC_OP_ALT:
    {
        strcpy( dest, "alt;" );
        break;
    }

    case MSC_OP_PAR:
    {
        strcpy( dest, "par;" );
        break;
    }

    case MSC_OP_EXC:
    case MSC_OP_OPT:
    case MSC_OP_LOOP:
        return RESULT_ERR;
    }

    return RESULT_OK;
}

static char sv_buffer[TAT_MAX_LINE_LEN * 2];
static LOGICAL msc_SaveEvents( FILE * f, MSC_EVENT_KIND * s, MSC_STORAGE * M, UINT32_T flags )
{
    
    UINT32_T                    i;

    while ( s )
    {
        if ( s->type == MSC_EV_OPERATOR )
        {
            MSC_GenStartOp( sv_buffer, s, M, flags );
            fprintf( f, "%s\n", sv_buffer );

            for ( i = 0; i < s->data.alt.frame_Count - 1; i++ )
            {
                if ( s->data.alt.frame )
                    msc_SaveEvents( f, s->data.alt.frame[i], M, flags );

                MSC_GenSepOp( sv_buffer, s, M, flags );
                fprintf( f, "%s\n", sv_buffer );
            }

            if ( s->data.alt.frame )
                msc_SaveEvents( f, s->data.alt.frame[i], M, flags );

            MSC_GenEndOp( sv_buffer, s, M, flags );
            fprintf( f, "%s\n", sv_buffer );
        }
        else
        {
            if ( !MSC_GenerateLine( sv_buffer, s, M, flags ) )
            {
                MSG_PrintError( MSG_INTERNAL_PROBLEM );
                return RESULT_ERR;
            }

            if ( sv_buffer[0] )
            {
                fputs( sv_buffer, f );
                fputc( '\n', f );
            }

            if ( s->type == MSC_EV_STOP )
                ended[( int ) s->instances[0]] = 1;
        }

        s = s->next;
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_Save
 *
 * Description: Save MSC in MSC-PR format
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_Save( const char * filename, MSC_STORAGE * M, const UINT32_T flags )
{
    char                    file[TAT_MAX_FNAME_LEN];
    char                    path[TAT_MAX_FNAME_LEN];
    char                    ext[TAT_MAX_FNAME_LEN];
    char                    dest[TAT_MAX_LINE_LEN], *pdest;
    FILE                   *f;
    UINT32_T                    i;
    LOGICAL                 res;

    if ( !filename )
    {
        MSG_PrintError( MSG_INTERNAL_PROBLEM );
        return RESULT_ERR;
    }

    if ( !M )
    {
        MSG_PrintError( MSG_INTERNAL_PROBLEM );
        return RESULT_ERR;
    }

    FP_SplitPath( filename, path, file, ext );

    ended = MEM_Allocate( sizeof ( char ) * M->instance_Count );

    f = FP_OpenFile( filename, "w" );

    if ( !f )
    {
        MEM_Free( ended );
        return RESULT_ERR;
    }

/* output base source references */
    dest[0] = 0;
    pdest = &dest[0];

    if ( M->src_Count && ( flags & MSC_MPR_OUTPUT_SREF ) )
    {
        *pdest++ = '/';
        *pdest++ = '*';

        for ( i = 0; i < M->src_Count; i++ )
        {
            sprintf( pdest, "[%s,%u,%u,%u]", MSC_GetStr( M->src_Files[i].link, M ),
                M->src_Files[i].level, M->src_Files[i].first_Line, M->src_Files[i].last_Line );
            pdest += strlen( pdest );
        }
        *pdest++ = '*';
        *pdest++ = '/';
        *pdest = 0;
    }

    if ( flags & MSC_MPR_CORRECT_NAME )
    {
        fprintf( f, "mscdocument %s;\n", file );
        fprintf( f, "msc %s%s;\n", file, dest );
    }
    else
    {
        if ( M->msc_Document )
            fprintf( f, "mscdocument %s;\n", MSC_GetStr( M->msc_Document, M ) );

        if ( M->msc_Name )
            fprintf( f, "msc %s%s;\n", MSC_GetStr( M->msc_Name, M ), dest );
    }

    for ( i = 0; i < M->instance_Count; i++ )
    {
        fprintf( f, "%s: instance", MSC_GetStr( M->instances[i].name, M ) );

        if ( M->instances[i].type )
            fprintf( f, " %s", MSC_GetStr( M->instances[i].type, M ) );

        if ( !M->instances[i].type && ( flags & MSC_MPR_OUTPUT_TIME ) && M->instances[i].ratio &&
            M->instances[i].ratio != STRCOLL_STR_ID_1 )
            fprintf( f, " " );

        if ( ( flags & MSC_MPR_OUTPUT_TIME ) && M->instances[i].ratio &&
            M->instances[i].ratio != STRCOLL_STR_ID_1 )
            fprintf( f, "{%s}", MSC_GetStr( M->instances[i].ratio, M ) );

        if ( M->instances[i].comment )
            fprintf( f, " comment '%s'", MSC_GetStr( M->instances[i].comment, M ) );

        fprintf( f, ";\n" );
    }

    if ( ( flags & MSC_MPR_OUTPUT_VERDICT ) )
    {
        for ( i = 0; i < M->trace_Count; i++ )
        {
            if ( !MSC_VerdictTrace )
            {
                if ( M->traces[i].status == MSC_VRS_CONSISTENT )
                {
                    fprintf( f, "text '%s';\n", VRS_DiagramVerdictText[M->traces[i].status] );
                }
                else
                {
                    fprintf( f, "text '%s (Trace %u)';\n", VRS_DiagramVerdictText[M->traces[i].status],
                        M->traces[i].trace );
                }
            }
            else
            {
                if ( M->traces[i].trace == MSC_VerdictTrace )
                {
                    if ( M->traces[i].status == MSC_VRS_CONSISTENT )
                    {
                        fprintf( f, "text '%s';\n", VRS_DiagramVerdictText[M->traces[i].status] );
                    }
                    else
                    {
                        fprintf( f, "text '%s (Trace %u)';\n", VRS_DiagramVerdictText[M->traces[i].status],
                            M->traces[i].trace );
                    }
                }
            }

        }
    }

/* output text */
    for ( i = 0; i < M->text_Count; i++ )
    {
        fputs( "text '", f );
        StrEmbedMSCtext( dest, MSC_GetStr( M->texts[i], M ) );
        fputs( dest, f );
        fputs( "';\n", f );
    }

    if ( M->param_Count && M->params_Type==TAT_PARAMS )
    {
        fputs( "text 'params ", f );
        for ( i = 0; i < M->param_Count; i++ )
        {
            fprintf( f, "%s %s", MSC_GetStr( M->params[i].pv_Type, M ), MSC_GetStr( M->params[i].pv_Name, M ) );
            if ( ( i + 1 ) < M->param_Count )
            {
                fputs( ",\n    ", f );
            }
        }
        fputs( ";';\n", f );
    }

    if ( M->var_Count )
    {
        fputs( "text 'def ", f );
        for ( i = 0; i < M->var_Count; i++ )
        {
            fprintf( f, "%s %s", MSC_GetStr( M->vars[i].pv_Type, M ), MSC_GetStr( M->vars[i].pv_Name, M ) );
            if ( ( i + 1 ) < M->var_Count )
            {
                fputs( ",\n    ", f );
            }
        }

        fputs( ";';\n", f );
    }

    res = msc_SaveEvents( f, M->start, M, flags );

    for ( i = 0; i < M->instance_Count; i++ )
    {
        if ( !ended[i] )
            fprintf( f, "%s: endinstance;\n", MSC_GetStr( M->instances[i].name, M ) );
    }

    fprintf( f, "endmsc;\n" );
    fclose( f );
    MEM_Free( ended );
    return res;
}
