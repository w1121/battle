/******************************************************************************
*
*       COPYRIGHT 2001-2004 MOTOROLA, ALL RIGHTS RESERVED
*
*       MOTOROLA CONFIDENTIAL PROPRIETARY
*
* Description:  Command-line processing
*
* $RCSfile: mscbase.c,v $
* $Revision: 1.3 $
* $Source: /home/cvsusr/cvssoft/tat_tools/baselib/src/mscbase.c,v $
* $Date: 2004/03/05 14:37:44 $
*
*
* CHANGE HISTORY
*                                                       
* Initial version 
*
******************************************************************************/
#include <mscevent.h>
#include <qmemory.h>
#include <mscstrs.h>
#include <strcoll.h>
#include <strproc.h>
#include <mscmess.h>
#include <outmess.h>

/* event names for debugging */
const char             *MSC_EventNames[] = {
    STR_IGNORE, STR_EMPTY, STR_BREAK, STR_REFERENCE, STR_CONDITION,
    STR_ACTION, STR_SET, STR_RESET, STR_TIMEOUT, STR_CONCURRENT,
    STR_ENDCONCURRENT, STR_ALT, STR_IN, STR_OUT, STR_CREATE,
    STR_STOP, STR_SEND, STR_RECEIVE, STR_EXIST, STR_CODE, STR_LOOP, STR_END
};

/***************************************************************************
 * Function : MSC_IsDefaultEventTime
 *
 * Description: check time for default
 *
 * Returns: LOGICAL_TRUE/LOGICAL_FALSE
 *
 * Notes:
 *
 **************************************************************************/
LOGICAL MSC_IsDefaultEventTime( MSC_EVENT_KIND * x )
{
    return ( LOGICAL ) ( x->time.str_MinTime == STRCOLL_STR_ID_0 && x->time.str_MaxTime == STRCOLL_STR_ID_INF &&
        ( x->time.type == MSC_RELATIVE_TIME ||
            ( x->time.type == MSC_LABELED_TIME && x->time.time_Label == 0 ) ) );
}

/***************************************************************************
 * Function : MSC_IsPassiveEvent
 *
 * Description: Check event to be passive type
 *
 * Returns: LOGICAL_TRUE/LOGICAL_FALSE
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_IsPassiveEvent( MSC_EVENT_KIND * E )
{
    return ( E && ( E->type == MSC_EV_RECV_SIGNAL ||
     ( E->type == MSC_EV_CONDITION && E->data.cond.call ) ) )?RESULT_OK:RESULT_ERR;
}

/***************************************************************************
 * Function : MSC_IsTimerEvent
 *
 * Description: Check event to be timer
 *
 * Returns: LOGICAL_TRUE/LOGICAL_FALSE
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_IsTimerEvent( MSC_EVENT_KIND * E )
{
    return ( E && E->type == MSC_EV_TIMER_TIMEOUT )?RESULT_OK:RESULT_ERR;
}

/***************************************************************************
 * Function : MSC_AllocateEvent
 *
 * Description: allocate memory, clean memory with zero and check if memory was allocated
 *
 * Returns: pointer to storage/NULL
 *
 * Notes: None
 *
 **************************************************************************/
MSC_EVENT_KIND              *MSC_AllocateEvent( )
{
    MSC_EVENT_KIND              *x;

    x = MEM_TYPE_ALLOC( MSC_EVENT_KIND );

    if ( x )
    {
        x->time.str_MinTime = STRCOLL_STR_ID_0;
        x->time.str_MaxTime = STRCOLL_STR_ID_INF;
        x->time.type = MSC_RELATIVE_TIME;
        x->time.max_Time = MSC_INFINITY_CONST;

        x->time_Calc.str_MinTime = STRCOLL_STR_ID_0;
        x->time_Calc.str_MaxTime = STRCOLL_STR_ID_INF;
        x->time_Calc.type = MSC_RELATIVE_TIME;
        x->time_Calc.max_Time = MSC_INFINITY_CONST;
    }

    return x;
}

/***************************************************************************
 * Function : MSC_ZeroEventTime
 *
 * Description: Reset time definition
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_ZeroEventTime( MSC_EVENT_KIND * x )
{
    if ( !x )
        return RESULT_ERR;

    x->time.str_MinTime = STRCOLL_STR_ID_0;
    x->time.str_MaxTime = STRCOLL_STR_ID_0;
    x->time.type = MSC_RELATIVE_TIME;
    x->time.max_Time = 0;
    x->time.min_Time = 0;
    x->time_Calc = x->time;
    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_AllocateEventType
 *
 * Description: Allocate memory for event and its data
 *
 * Returns: pointer to storage/NULL
 *
 * Notes: None
 *
 **************************************************************************/
MSC_EVENT_KIND              *MSC_AllocateEventType( MSC_EVENT_KIND_TYPE z )
{
    MSC_EVENT_KIND              *x;

    x = MSC_AllocateEvent( );

    if ( x )
    {
        x->type = z;
    }
    return x;
}

/***************************************************************************
 * Function : MSC_FreeEventData
 *
 * Description: free memory allocated for event's data
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_FreeEventData( MSC_EVENT_KIND * event )
{
    size_t                   i;

    if ( !event )
        return RESULT_ERR;

    if ( event->instances )
    {
        MEM_Free( event->instances );
        event->instances = NULL;
        event->inst_Count = 0;
    }

/* free source-references information */
    if ( event->src_Data )
    {
        MEM_Free( event->src_Data );
        event->src_Data = NULL;
        event->src_Count = 0;
    }

    if ( event->verdict )
    {
        MEM_Free( event->verdict );
        event->verdict = NULL;
        event->verdict_Count = 0;
    }

    switch ( event->type )
    {

    case MSC_EV_REFERENCE:
    {
        for ( i = 0; i < event->data.ref.param_Count; i++ )
            if ( event->data.ref.macro_Name[i] )
            {
                MEM_Free( event->data.ref.macro_Name[i] );
                event->data.ref.macro_Name[i] = NULL;
            }

        if ( event->data.ref.macro_Name )
        {
            MEM_Free( event->data.ref.macro_Name );
            event->data.ref.macro_Name = NULL;
        }

        if ( event->data.ref.refr_Subst )
        {
            MEM_Free( event->data.ref.refr_Subst );
            event->data.ref.refr_Subst = NULL;
        }
        break;
    }

    case MSC_EV_OPERATOR:
    {
        if ( event->data.alt.frame )
        {
            MEM_Free( event->data.alt.frame );
            event->data.alt.frame = NULL;
        }
        break;
    }

    /* nothing to process */
    case MSC_EV_CREATE:
    case MSC_EV_CONDITION:
    case MSC_EV_TIMER_SET:
    case MSC_EV_TIMER_RESET:
    case MSC_EV_TIMER_TIMEOUT:
    case MSC_EV_EXIST_SIGNAL:
    case MSC_EV_RECV_SIGNAL:
    case MSC_EV_SEND_SIGNAL:
    case MSC_EV_IN_MESSAGE:
    case MSC_EV_OUT_MESSAGE:
    case MSC_EV_START_LOOP:
    case MSC_EV_END_LOOP:
    case MSC_EV_UNDEFINED:
    case MSC_EV_ACTION:
    case MSC_EV_CODE:
    case MSC_EV_BREAK:
    case MSC_EV_EMPTY:
    case MSC_EV_CONCURRENT_START:
    case MSC_EV_CONCURRENT_END:
    case MSC_EV_STOP:
        break;
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_FreeEventChain
 *
 * Description: 
 *
 * Returns: 
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_FreeEventChain( MSC_EVENT_KIND * event )
{
    size_t                  i;
    MSC_EVENT_KIND              *x;

    while ( event )
    {
        x = event->next;

        if ( event->type == MSC_EV_OPERATOR )
        {
            for ( i = 0; i < event->data.alt.frame_Count; i++ )
            {
                MSC_FreeEventChain( event->data.alt.frame[i] );
                event->data.alt.frame[i] = NULL;
            }

            if ( event->data.alt.frame )
            {
                MEM_Free( event->data.alt.frame );
                event->data.alt.frame = NULL;
            }
        }

    /* free instance element */
        MSC_FreeEventData( event );

        MEM_Free( event );

        event = x;
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_RemoveTailEvents
 *
 * Description: remove unnecessary events from tree
 *
 * Returns: RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_RemoveTailEvents( MSC_EVENT_KIND * s )
{
    size_t  i;

    while ( s )
    {
        if ( ( s->next ) && ( s->next->next == NULL ) && s->next->type == MSC_EV_UNDEFINED )
        {
            if ( s->next->instances )
                MEM_Free( s->next->instances );

            MEM_Free( s->next );

            s->next = NULL;
        }

        if ( s->type == MSC_EV_OPERATOR )
        {
            if ( s->data.alt.type == MSC_OP_SEQ )
            {
                MSG_Abort( MSG_INTERNAL_PROBLEM );
            }

            for ( i = 0; i < s->data.alt.frame_Count; i++ )
                MSC_RemoveTailEvents( s->data.alt.frame[i] );
        }

        s = s->next;
    }

    return RESULT_OK;
}


/***************************************************************************
 * Function : MSC_Time2Str
 *
 * Description: Convert double to time string
 *
 * Returns: d
 *
 * Notes: None
 *
 **************************************************************************/
char * MSC_Time2Str( MSC_TIME_VALUE t, char * d )
{
    if ( t == MSC_INFINITY_CONST )
        strcpy( d, STR_INF );
    else
#ifdef MSC_INTEGER_TIME
        sprintf( d, "%u", t );
#else
        sprintf( d, "%.32g", t );
#endif

    return d;
}

/***************************************************************************
 * Function : MSC_Str2Dbl
 *
 * Description: Convert string to number
 *
 * Returns: 
 *
 * Notes: None
 *
 **************************************************************************/
MSC_TIME_VALUE MSC_Str2Dbl( const char * x )
{
    if ( !stricmp( x, STR_INF ) )
        return MSC_INFINITY_CONST;

    return StrToDouble( x );
}

/***************************************************************************
 * Function : MSC_Str2Int
 *
 * Description: Convert string to integer number
 *
 * Returns: 
 *
 * Notes: None
 *
 **************************************************************************/
UINT32_T MSC_Str2Int( const char * x )
{
    if ( !stricmp( x, STR_INF ) )
        return MSC_INFINITY_CONST;

    if ( !StrIsIntegerNumber( x ) )
        return 0;

    return ( UINT32_T ) StrToInt( x );
}


/***************************************************************************
 * Function : MSC_IsMessageEvent
 *
 * Description: check wherever event is message
 *
 * Returns: RESULT_OK/RESULT_ERR
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_IsMessageEvent( MSC_EVENT_KIND * x )
{
    return ( x->type == MSC_EV_SEND_SIGNAL ) || ( x->type == MSC_EV_RECV_SIGNAL ) ||
        ( x->type == MSC_EV_IN_MESSAGE ) || ( x->type == MSC_EV_OUT_MESSAGE ) ||
        ( x->type == MSC_EV_EXIST_SIGNAL );
}


