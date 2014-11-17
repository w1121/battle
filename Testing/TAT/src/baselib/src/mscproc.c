/******************************************************************************
*
*       COPYRIGHT 2001-2004 MOTOROLA, ALL RIGHTS RESERVED
*
*       MOTOROLA CONFIDENTIAL PROPRIETARY
*
* Description:  Command-line processing
*
* $RCSfile: mscproc.c,v $
* $Revision: 1.3 $
* $Source: /home/cvsusr/cvssoft/tat_tools/baselib/src/mscproc.c,v $
* $Date: 2004/03/05 14:37:44 $
*
*
* CHANGE HISTORY
*                                                       
* Initial version 
*
******************************************************************************/
#include <portable.h>
#include <mscbase.h>
#include <qmemory.h>
#include <fileproc.h>
#include <mscparse.h>
#include <mscproc.h>
#include <strproc.h>
#include <outmess.h>
#include <mscmess.h>
#include <mscstrs.h>
#include <tatcalc.h>
#include <tatgdata.h>

static char             dest[TAT_MAX_ID_LEN];

static size_t             tat_TimerCount;
static struct _tat_Timers
{
    MSC_EVENT_KIND              *set;
    size_t                  t;
} *tat_Timers;

/***************************************************************************
 * Function : tat_CopyEventChain
 *
 * Description: copy event chain within MSC
 *
 * Returns: pointer to new chain
 *
 * Notes: None
 *
 **************************************************************************/
static MSC_EVENT_KIND       *tat_CopyEventChain( MSC_EVENT_KIND * s, const MSC_STORAGE * M )
{
    size_t                  i;
    MSC_EVENT_KIND              *y = NULL, *start;

    if ( s )
    {
        start = MEM_Copy( s, sizeof ( MSC_EVENT_KIND ) );
        y = start;
    }
    else
        start = NULL;

    while ( s )
    {
    /* free instance element */

        if ( s->instances && s->inst_Count )
        {
            y->instances = MEM_Copy( s->instances, s->inst_Count * sizeof ( s->instances[0] ) );
        }

        if ( s->src_Data )
        {
            y->src_Data = MEM_Copy( s->src_Data, s->src_Count * sizeof ( s->src_Data[0] ) );
        }

        switch ( s->type )
        {

        case MSC_EV_REFERENCE:
        {
        /* copy independent strings */
            y->data.ref.macro_Name = ( char ** ) MEM_Allocate( s->data.ref.param_Count * sizeof ( char * ) );

            for ( i = 0; i < s->data.ref.param_Count; i++ )
                y->data.ref.macro_Name[i] = MEM_CopyStr( s->data.ref.macro_Name[i] );

            y->data.ref.refr_Subst =
                ( STRING_ID * ) MEM_Copy( s->data.ref.refr_Subst,
                s->data.ref.param_Count * sizeof ( STRING_ID ) );

            break;
        }

        case MSC_EV_TIMER_SET:
        case MSC_EV_TIMER_RESET:
        case MSC_EV_TIMER_TIMEOUT:
        {
            if ( y->type == MSC_EV_TIMER_SET )
            {
                tat_Timers = MEM_Reallocate( tat_Timers, ( tat_TimerCount + 1 ) * sizeof ( tat_Timers[0] ) );
                tat_Timers[tat_TimerCount].set = y;
                tat_Timers[tat_TimerCount].t = y->data.timer.timer_ID;
                tat_TimerCount++;
            }
            else
            {
                UINT32_T                    j;

                for ( j = 0; j < tat_TimerCount; j++ )
                {
                    if ( tat_Timers[j].t == y->data.timer.timer_ID )
                    {
                        y->data.timer.relation = tat_Timers[j].set;
                        tat_Timers[j].set->data.timer.relation = y;
                        break;
                    }
                }
            }

            break;
        }

        case MSC_EV_OPERATOR:
        {
            break;
        }

        case MSC_EV_CREATE:
        {
            y->data.create.params =
                MEM_Copy( s->data.create.params, sizeof ( STRING_ID ) * ( s->data.create.param_Count ) );
            break;
        }

        case MSC_EV_CONDITION:
        {
            break;
        }

        case MSC_EV_START_LOOP:
        {
            break;
        }

        case MSC_EV_IN_MESSAGE:
        case MSC_EV_OUT_MESSAGE:
        case MSC_EV_SEND_SIGNAL:
        case MSC_EV_RECV_SIGNAL:
        case MSC_EV_EXIST_SIGNAL:
        {
            break;
        }
        }

    /* copy tree structure */
        if ( s->type == MSC_EV_OPERATOR )
        {
            y->data.alt.frame = MEM_Allocate( s->data.alt.frame_Count * sizeof ( MSC_EVENT_KIND * ) );

            for ( i = 0; i < s->data.alt.frame_Count; i++ )
            {
                y->data.alt.frame[i] = tat_CopyEventChain( s->data.alt.frame[i], M );
            }
        }

    /* correct labels */
        if ( s->labeled )
        {
            M->labels[s->labeled - 1].s = y;
        }

    /* allocate memory for new event */
        if ( s->next )
        {
            y->next = ( MSC_EVENT_KIND * ) MEM_Copy( s->next, ( sizeof ( MSC_EVENT_KIND ) ) );
        }

        y = y->next;
        s = s->next;
    }

    return start;
}

static size_t             lcount, ocount;

static struct _remap
{
    size_t                    old_l, new_l;
} *lbs, *inds;


/***************************************************************************
 * Function : tat_CheckTimeLab
 *
 * Description: replaces references to time labels
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_CheckTimeLab( char *dest, MSC_STORAGE * SRC, MSC_STORAGE * M )
{
    UINT32_T                    i;
    char                    repl[TAT_MAX_ID_LEN], subs[TAT_MAX_ID_LEN];
    char                    buf[TAT_MAX_LINE_LEN];

    if ( strchr( dest, '@' ) )
    {
        repl[0] = '@';
        subs[0] = '@';
        for ( i = 0; i < lcount; i++ )
        {
            strcpy( repl + 1, MSC_GetStr( SRC->labels[lbs[i].old_l].label, SRC ) );
            strcpy( subs + 1, MSC_GetStr( M->labels[lbs[i].new_l].label, M ) );
            StrReplaceLex( buf, dest, repl, subs );
            strcpy( dest, buf );
        }
    }
    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_DuplicateEventChain
 *
 * Description: copy MSC events within different block
 *
 * Returns: pointer to new event chain
 *
 * Notes: None
 *
 **************************************************************************/
static MSC_EVENT_KIND       *tat_DuplicateEventChain( MSC_EVENT_KIND * s,
    MSC_STORAGE * M, MSC_STORAGE * SRC, char *subs, char *repl )
{
    size_t                    i, l;
    MSC_EVENT_KIND              *y, *start;

    y = NULL;

    if ( s )
    {
        start = MEM_Copy( s, sizeof ( MSC_EVENT_KIND ) );
        y = start;
    }
    else
        start = NULL;

    while ( s )
    {
    /* copy instance element */

        if ( s->instances )
        {
            y->instances = MEM_Copy( s->instances, s->inst_Count * sizeof ( s->instances[0] ) );
        }

        if ( s->src_Data )
        {
            y->src_Data = MEM_Copy( s->src_Data, s->src_Count * sizeof ( s->src_Data[0] ) );
        }

        if ( repl || M != SRC )
        {
            if ( s->condition )
            {
                StrReplaceLex( dest, MSC_GetStr( s->condition, SRC ), subs, repl );
                y->condition = MSC_AddStr( dest, M );
            }

            if ( s->comment )
            {
                StrReplaceLex( dest, MSC_GetStr( s->comment, SRC ), subs, repl );
                y->comment = MSC_AddStr( dest, M );
            }

            if ( s->time.str_MinTime )
            {
                StrReplaceLex( dest, MSC_GetStr( s->time.str_MinTime, SRC ), subs, repl );
                y->time.str_MinTime = MSC_AddStr( dest, M );
            }

            if ( s->time.str_MaxTime )
            {
                StrReplaceLex( dest, MSC_GetStr( s->time.str_MaxTime, SRC ), subs, repl );
                y->time.str_MaxTime = MSC_AddStr( dest, M );
            }

            if ( s->time_Calc.str_MinTime )
            {
                StrReplaceLex( dest, MSC_GetStr( s->time_Calc.str_MinTime, SRC ), subs, repl );
                y->time_Calc.str_MinTime = MSC_AddStr( dest, M );
            }

            if ( s->time_Calc.str_MaxTime )
            {
                StrReplaceLex( dest, MSC_GetStr( s->time_Calc.str_MaxTime, SRC ), subs, repl );
                y->time_Calc.str_MaxTime = MSC_AddStr( dest, M );
            }

        }

    /* add new label */
        if ( s->labeled && (!repl || (repl && ( *repl != '0' || *( repl + 1 ) ) ) ) )
        {
            size_t                    id;

            lbs = MEM_Reallocate( lbs, ( lcount + 1 ) * sizeof ( lbs[0] ) );
            lbs[lcount].old_l = s->labeled - 1;
            lbs[lcount].new_l = M->label_Count;
            lcount++;
            y->labeled = M->label_Count + 1;
            M->labels = MEM_Reallocate( M->labels, sizeof ( MSC_EVENT_KIND_LABEL ) * ( M->label_Count + 1 ) );

            if ( !M->labels )
                return NULL;

            M->labels[M->label_Count].s = y;

        /* copy original name */
            strcpy( dest, MSC_GetStr( SRC->labels[s->labeled - 1].label, SRC ) );
            id = M->label_Count;

            while ( MSC_GetTimeLabelID( dest, M ) != -1 )
            {
                sprintf( dest, "LL_%u", id );
                id++;
            }

            M->labels[M->label_Count].label = MSC_AddStr( dest, M );

            ( M->label_Count )++;
        }

        if ( s->time.type == MSC_LABELED_TIME )
        {
            for ( i = 0; i < lcount; i++ )
            {
                if ( lbs[i].old_l == s->time.time_Label )
                {
                    y->time.time_Label = lbs[i].new_l;
                    break;
                }
            }
        }

        switch ( s->type )
        {

        case MSC_EV_REFERENCE:
        {
        /* copy independent strings */
            y->data.ref.macro_Name = ( char ** ) MEM_Allocate( s->data.ref.param_Count * sizeof ( char * ) );
            y->data.ref.refr_Subst =
                ( STRING_ID * ) MEM_Copy( s->data.ref.refr_Subst,
                s->data.ref.param_Count * sizeof ( STRING_ID ) );

            StrReplaceLex( dest, MSC_GetString( s->data.ref.refr_Name, SRC ), subs, repl );
            y->data.ref.refr_Name = MSC_AddStr( dest, M );

            for ( i = 0; i < s->data.ref.param_Count; i++ )
            {
                StrReplaceLex( dest, s->data.ref.macro_Name[i], subs, repl );
                y->data.ref.macro_Name[i] = MEM_CopyStr( dest );
                StrReplaceLex( dest, MSC_GetString( s->data.ref.refr_Subst[i], SRC ), subs, repl );
                y->data.ref.refr_Subst[i] = MSC_AddStr( dest, M );
            }

            break;
        }

        case MSC_EV_TIMER_SET:
        case MSC_EV_TIMER_RESET:
        case MSC_EV_TIMER_TIMEOUT:
        {
            StrReplaceLex( dest, MSC_GetString( s->data.timer.signal_Name, SRC ), subs, repl );
            y->data.timer.signal_Name = MSC_AddStr( dest, M );

            if ( s->data.timer.timing.str_MinTime )
            {
                StrReplaceLex( dest, MSC_GetString( s->data.timer.timing.str_MinTime, SRC ), subs, repl );
                y->data.timer.timing.str_MinTime = MSC_AddStr( dest, M );
            }

            if ( s->data.timer.timing.str_MaxTime )
            {
                StrReplaceLex( dest, MSC_GetString( s->data.timer.timing.str_MaxTime, SRC ), subs, repl );
                y->data.timer.timing.str_MaxTime = MSC_AddStr( dest, M );
            }

            if ( y->type == MSC_EV_TIMER_SET )
            {
                tat_Timers = MEM_Reallocate( tat_Timers, ( tat_TimerCount + 1 ) * sizeof ( tat_Timers[0] ) );
                tat_Timers[tat_TimerCount].set = y;
                tat_Timers[tat_TimerCount].t = y->data.timer.timer_ID;
                tat_TimerCount++;
            }
            else
            {
                UINT32_T                    j;

                for ( j = 0; j < tat_TimerCount; j++ )
                {
                    if ( tat_Timers[j].t == y->data.timer.timer_ID )
                    {
                        y->data.timer.relation = tat_Timers[j].set;
                        tat_Timers[j].set->data.timer.relation = y;
                        break;
                    }
                }
            }

            break;
        }

        case MSC_EV_OPERATOR:
        {
            if ( s->data.alt.l1 )
            {
                StrReplaceLex( dest, MSC_GetString( s->data.alt.l1, SRC ), subs, repl );
                y->data.alt.l1 = MSC_AddStr( dest, M );
            }

            if ( s->data.alt.l2 )
            {
                StrReplaceLex( dest, MSC_GetString( s->data.alt.l2, SRC ), subs, repl );
                y->data.alt.l2 = MSC_AddStr( dest, M );
            }

            break;
        }

        case MSC_EV_CREATE:
        {
            y->data.create.params = MEM_Allocate( sizeof ( STRING_ID ) * s->data.create.param_Count );

            for ( i = 0; i < s->data.create.param_Count; i++ )
            {
                if ( s->data.create.params[i] )
                {
                    StrReplaceLex( dest, MSC_GetString( s->data.create.params[i], SRC ), subs, repl );
                    y->data.create.params[i] = MSC_AddStr( dest, M );
                }
            }

            break;
        }

        case MSC_EV_CONDITION:
        {
            if ( s->data.cond.call )
            {
                StrReplaceLex( dest, MSC_GetString( s->data.cond.call, SRC ), subs, repl );
                tat_CheckTimeLab( dest, SRC, M );
                y->data.cond.call = MSC_AddStr( dest, M );
            }

            if ( s->data.cond.text )
            {
                StrReplaceLex( dest, MSC_GetString( s->data.cond.text, SRC ), subs, repl );
                tat_CheckTimeLab( dest, SRC, M );
                y->data.cond.text = MSC_AddStr( dest, M );
            }

            break;
        }

        case MSC_EV_ACTION:
        case MSC_EV_CODE:
        {
            if ( s->data.text )
            {
                StrReplaceLex( dest, MSC_GetString( s->data.text, SRC ), subs, repl );
                tat_CheckTimeLab( dest, SRC, M );
                y->data.text = MSC_AddStr( dest, M );
            }

            break;
        }

        case MSC_EV_START_LOOP:
        {
            if ( s->data.loop.l1 )
            {
                StrReplaceLex( dest, MSC_GetString( s->data.loop.l1, SRC ), subs, repl );
                y->data.loop.l1 = MSC_AddStr( dest, M );
            }

            if ( s->data.loop.l2 )
            {
                StrReplaceLex( dest, MSC_GetString( s->data.loop.l2, SRC ), subs, repl );
                y->data.loop.l2 = MSC_AddStr( dest, M );
            }

            if ( s->data.loop.text )
            {
                StrReplaceLex( dest, MSC_GetString( s->data.loop.text, SRC ), subs, repl );
                y->data.loop.text = MSC_AddStr( dest, M );
            }

            break;
        }

        case MSC_EV_IN_MESSAGE:
        case MSC_EV_OUT_MESSAGE:
        case MSC_EV_RECV_SIGNAL:
        case MSC_EV_SEND_SIGNAL:
        case MSC_EV_EXIST_SIGNAL:
        {

            if ( s->type == MSC_EV_OUT_MESSAGE || s->type == MSC_EV_RECV_SIGNAL ||
                s->type == MSC_EV_SEND_SIGNAL || s->type == MSC_EV_EXIST_SIGNAL ||
                ( s->type == MSC_EV_IN_MESSAGE && s->data.msg.inst < 0 ) )
            {
                inds = MEM_Reallocate( inds, ( ocount + 1 ) * sizeof ( inds[0] ) );
                inds[ocount].old_l = s->data.msg.indicator;
                inds[ocount].new_l = MSC_CopyMessage( s->data.msg.indicator, M, SRC );

                if ( subs )
                {
                    MSC_MESSAGE_STORAGE    *X;

                    X = MSC_GetMessageParams( inds[ocount].new_l, M );

                    for ( i = 0; i < X->param_Count; i++ )
                    {
                        if ( StrReplaceLex( dest, MSC_GetString( X->message_Params[i], M ), subs, repl ) )
                        {
                            X->message_Params[i] = MSC_AddStr( dest, M );
                        }

                    }
                }

                if ( !inds[ocount].new_l )
                    return NULL;

                y->data.msg.indicator = inds[ocount].new_l;

                ocount++;
            }
            else
            {
                i = ocount;
                l = y->data.msg.indicator;

                while ( i )
                {
                    if ( inds[i - 1].old_l == l )
                    {
                        y->data.msg.indicator = inds[i - 1].new_l;
                        break;
                    }

                    i--;
                }
            }

            break;
        }
        }

    /* handle tree structure */
        if ( s->type == MSC_EV_OPERATOR )
        {
            y->data.alt.frame = MEM_Allocate( s->data.alt.frame_Count * sizeof ( MSC_EVENT_KIND * ) );

            for ( i = 0; i < s->data.alt.frame_Count; i++ )
            {
                y->data.alt.frame[i] = tat_DuplicateEventChain( s->data.alt.frame[i], M, SRC, subs, repl );
            }
        }

    /* allocate memory for new event */
        if ( s->next )
        {
            y->next = ( MSC_EVENT_KIND * ) MEM_Copy( s->next, ( sizeof ( MSC_EVENT_KIND ) ) );
        }

        y = y->next;
        s = s->next;
    }

    return start;
}

/***************************************************************************
 * Function : MSC_Copy
 *
 * Description: Duplicate MSC
 *
 * Returns: pointer to new MSC
 *
 * Notes: None
 *
 **************************************************************************/
MSC_STORAGE        *MSC_Copy( const MSC_STORAGE * s )
{
    MSC_STORAGE        *x;
    size_t                  i, j, m;

    x = ( MSC_STORAGE * ) MEM_Copy( s, sizeof ( MSC_STORAGE ) );

    if ( x == NULL )
        return x;

    tat_TimerCount = 0;

    tat_Timers = NULL;

    m = s->message_Count;

    if ( s->instances )
    {
        x->instances = MEM_Copy( s->instances, s->instance_Count * sizeof ( s->instances[0] ) );
    }

    if ( s->texts )
    {
        x->texts = MEM_Copy( s->texts, s->text_Count * sizeof ( s->texts[0] ) );
    }

    x->str = StrCollCopy( s->str );

    if ( s->src_Files )
    {
        x->src_Files = MEM_Copy( s->src_Files, s->src_Count * sizeof ( s->src_Files[0] ) );
    }

    if ( m != 0 )
    {
        x->messages = MEM_Copy( s->messages, m * sizeof ( s->messages[0] ) );
    }

    if ( s->label_Count != 0 )
    {
        x->labels = MEM_Copy( s->labels, s->label_Count * sizeof ( s->labels[0] ) );
    }

    if ( s->var_Count != 0 )
    {
        x->vars = MEM_Copy( s->vars, s->var_Count * sizeof ( s->vars[0] ) );
    }

    if ( s->param_Count != 0 )
    {
        x->params = MEM_Copy( s->params, s->param_Count * sizeof ( s->params[0] ) );
    }

    for ( i = 0; i < m; i++ )
    {
        if ( s->messages[i].param_Count )
        {
            x->messages[i].message_Params =
                MEM_Copy( s->messages[i].message_Params, s->messages[i].param_Count * sizeof ( s->messages[i].message_Params[0] ) );
            x->messages[i].params_Values =
                MEM_Copy( s->messages[i].params_Values, s->messages[i].param_Count * sizeof ( s->messages[i].params_Values[0] ) );
            for (j=0; j<s->messages[i].param_Count; j++)
            {
                x->messages[i].params_Values[j].values = MEM_Copy(s->messages[i].params_Values[j].values,
                    s->messages[i].params_Values[j].valueCount*sizeof(s->messages[0].params_Values[0].values[0]));
            }
        }
    }

    if ( s->start )
        x->start = tat_CopyEventChain( s->start, x );

    if ( tat_Timers )
        MEM_Free( tat_Timers );

    return x;
}

/***************************************************************************
 * Function : msc_Process
 *
 * Description: Call user's function for each event recursively
 *
 * Returns: ERROR/OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL msc_Process( MSC_STORAGE * M, MSC_EVENT_KIND * s, MSC_EVENT_KIND_PROCESS_FUNC user_func )
{
    size_t                   i;

    while ( s )
    {
    /* call user's function for each event */
        if ( !user_func( M, s ) )
            return RESULT_ERR;

        if ( s->type == MSC_EV_OPERATOR )
        {
        /* process inline expressions */
            for ( i = 0; i < s->data.alt.frame_Count; i++ )
            {
                if ( !msc_Process( M, s->data.alt.frame[i], user_func ) )
                    return RESULT_ERR;
            }

        }
        s = s->next;
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_Process
 *
 * Description: Call user's function for each event
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_Process( MSC_STORAGE * M, MSC_EVENT_KIND_PROCESS_FUNC user_func )
{
    if ( !M || !user_func )
        return RESULT_ERR;

    return msc_Process( M, M->start, user_func );
}

/***************************************************************************
 * Function : MSC_FindEndEventChain
 *
 * Description: find last event in chain
 *
 * Returns: pointer to event
 *
 * Notes: None
 *
 **************************************************************************/
MSC_EVENT_KIND              *MSC_FindEndEventChain( MSC_EVENT_KIND * s )
{
    while ( s && s->next )
        s = s->next;

    return s;
}

/***************************************************************************
 * Function : msc_GenProc
 *
 * Description: conduct general processing in MSC
 *
 * Returns: RESULT_OK/MSC_REPEAT_ACTION/MSC_NOT_END
 *
 * Notes: None
 *
 **************************************************************************/
static int msc_GenProc( MSC_STORAGE * M, MSC_EVENT_KIND ** s, MSC_EVENT_KIND * d, int flags )
{
    MSC_EVENT_KIND              *x, *y, *z, *tt;
    MSC_INSTANCE_ID_TYPE   *inst;
    char                    num[32];
    char                   *l1, *l2;
    int                     k, l;
    UINT32_T                    i;
    double                  fcond;

    if ( !d )
        return RESULT_OK;

/* process condition of existness */
    if ( ( flags & MSC_TRANS_USE_COND ) && ( d->condition ) )
    {
        l1 = MSC_GetString( d->condition, M );

    /* check if it is expression, and compute the result */

        if ( MSC_Calc( l1, &fcond ) )
        {
        /* if result is equal to zero, remove event */

            if ( fcond == 0 )
            {
                if ( d->labeled )
                {
                    k = d->labeled;
                    x = d->next;
                    l = d->inst_Count;
                    inst = d->instances;
                    d->instances = NULL;
                /* handle OPERATOR */

                    if ( d->type == MSC_EV_OPERATOR )
                    {
                        for ( i = 0; i < d->data.alt.frame_Count; i++ )
                        {
                            MSC_FreeEventChain( d->data.alt.frame[i] );
                            d->data.alt.frame[i] = NULL;
                        }

                        if ( d->data.alt.frame )
                        {
                            MEM_Free( d->data.alt.frame );
                            d->data.alt.frame = NULL;
                        }
                    }

                    MSC_FreeEventData( d );
                    d->type = MSC_EV_CONDITION;
                    d->labeled = k;
                    d->next = x;
                    d->inst_Count = l;
                    d->instances = inst;
                    d->data.cond.text = MSC_AddStr( (char *)MSC_CONDITION_EVENT_NAME, M );
                    MSC_ZeroEventTime( d );
                    return MSC_NOT_END;
                }
                else
                {
                    *s = d->next;
                    d->next = NULL;
                    MSC_FreeEventChain( d );
                    return MSC_REPEAT_ACTION;
                }
            }

            d->condition = 0;
        }                       /* if (PTK_Calc()) */
    }

    if ( d->type == MSC_EV_EMPTY )
    {
        *s = d->next;
        MSC_FreeEventData( d );
        MEM_Free( d );
        return MSC_REPEAT_ACTION;
    }

    if ( !d->labeled )
    {
        if ( d->type == MSC_EV_CONDITION )
        {
        /* remove generated conditions */
            if ( ( flags & MSC_TRANS_REM_GEN_CND ) && d->data.cond.generated )
            {
                *s = d->next;
                MSC_FreeEventData( d );
                MEM_Free( d );
                return MSC_REPEAT_ACTION;
            }

        /* -rmvc switch */
            if ( ( flags & MSC_TRANS_REM_FR_COND ) && d->type == MSC_EV_CONDITION )
            {
                *s = d->next;
                MSC_FreeEventData( d );
                MEM_Free( d );
                return MSC_REPEAT_ACTION;
            }
        }

    /* remove timers (-rmvt switch) */
        if ( ( flags & MSC_TRANS_REM_FR_TMR ) && ( d->type == MSC_EV_TIMER_SET ||
                d->type == MSC_EV_TIMER_RESET || d->type == MSC_EV_TIMER_TIMEOUT ) &&
            !d->data.timer.relation->labeled )
        {
            *s = d->next;
            MSC_FreeEventData( d );
            MEM_Free( d );
            return MSC_REPEAT_ACTION;
        }

    /* -sro switch */
        if ( ( flags & MSC_TRANS_SRO ) &&
            d->type != MSC_EV_IN_MESSAGE &&
            d->type != MSC_EV_OUT_MESSAGE &&
            d->type != MSC_EV_SEND_SIGNAL &&
            d->type != MSC_EV_RECV_SIGNAL && d->type != MSC_EV_OPERATOR && d->type != MSC_EV_EXIST_SIGNAL )
        {
            *s = d->next;
            MSC_FreeEventData( d );
            MEM_Free( d );
            return MSC_REPEAT_ACTION;
        }

    }

    if ( d->labeled && ( d->type == MSC_EV_CONCURRENT_START || d->type == MSC_EV_CONCURRENT_END ||
            ( ( flags & MSC_TRANS_MOVE_LAB_TM ) &&
                ( d->type == MSC_EV_TIMER_SET || d->type == MSC_EV_TIMER_RESET ||
                    d->type == MSC_EV_TIMER_TIMEOUT ) ) || ( ( flags & MSC_TRANS_MOVE_LAB_CS ) &&
                ( d->type == MSC_EV_CREATE || d->type == MSC_EV_STOP ) ) ) )
    {
        x = MSC_AllocateEventType( MSC_EV_CONDITION );

        if ( !x )
            return RESULT_ERR;

        x->data.cond.text = MSC_AddStr( (char *)MSC_CONDITION_EVENT_NAME, M );
        x->data.cond.generated = 1;

    /* move label to condition */
        x->labeled = d->labeled;
        M->labels[x->labeled - 1].s = x;

    /* remove label from event */
        d->labeled = 0;
    /* copy instance binding */
        x->inst_Count = d->inst_Count;
        x->instances = MEM_Copy( d->instances, sizeof ( MSC_INSTANCE_ID_TYPE ) * d->inst_Count );

    /* copy time */
        x->time = d->time;
        x->time_Calc = d->time_Calc;

        MSC_ZeroEventTime( d );
        d->sync = MSC_ASYNC_EVENT;
        x->next = d;

        *s = x;
        return MSC_NOT_END;
    }

    if ( ( d->type == MSC_EV_OPERATOR ) )
    {
    /* remove embedded operators */

        while ( d->data.alt.frame[0] && d->data.alt.frame[0]->type == MSC_EV_OPERATOR &&
            ( ( d->data.alt.type == MSC_OP_OPT ) || ( d->data.alt.type == MSC_OP_EXC ) ) &&
            d->data.alt.frame[0]->data.alt.type == d->data.alt.type && d->data.alt.frame[0]->next == NULL )
        {
            x = d->data.alt.frame[0];
            d->data.alt.frame[0] = d->data.alt.frame[0]->data.alt.frame[0];
            MSC_FreeEventData( x );
            MEM_Free( x );
        }

    /* remove alt with single frame */
        if ( ( d->data.alt.type == MSC_OP_ALT ) )
        {
            if ( d->data.alt.frame_Count == 1 )
            {
                y = ( *s )->next;
                if (d->data.alt.frame[0])
                {
                    *s = d->data.alt.frame[0];
                    MEM_Free( d->data.alt.frame );
                    d->data.alt.frame = NULL;
                    x = MSC_FindEndEventChain( *s );
                    x->next = y;
                    MSC_FreeEventData( d );
                    MEM_Free( d );
                }
                else
                {
                    /* handle case of empty 'alt' */
                    *s = y;
                    MEM_Free( d->data.alt.frame );
                    d->data.alt.frame = NULL;
                    MSC_FreeEventData( d );
                    MEM_Free( d );
                }
                return MSC_REPEAT_ACTION;
            }

        /* concatenate embedded alt operators */
            k = 0;

            while ( k < ( int ) d->data.alt.frame_Count )
            {
                if ( d->data.alt.frame[k] && d->data.alt.frame[k]->type == MSC_EV_OPERATOR
                    && d->data.alt.frame[k]->next == NULL && d->data.alt.frame[k]->data.alt.type == MSC_OP_ALT )
                {
                    l = d->data.alt.frame_Count;
                    d->data.alt.frame_Count += d->data.alt.frame[k]->data.alt.frame_Count - 1;
                    d->data.alt.frame =
                        MEM_Reallocate( d->data.alt.frame, d->data.alt.frame_Count * sizeof ( MSC_EVENT_KIND * ) );

                /* save event */
                    x = d->data.alt.frame[k];

                /* copy events from nested level */
                    d->data.alt.frame[k] = d->data.alt.frame[k]->data.alt.frame[0];

                    for ( i = 1; i < x->data.alt.frame_Count; i++ )
                    {
                        d->data.alt.frame[l++] = x->data.alt.frame[i];
                    }

                /* remove extra alt event */
                    MSC_FreeEventData( x );

                    MEM_Free( x );
                }
                else
                    k++;
            }
        }

    /* process LOOP inline operator */
        if ( ( d->data.alt.type == MSC_OP_LOOP ) && ( flags & ( MSC_TRANS_INLINE_LOOP |
                    MSC_TRANS_REMOVE_ZERO_LOOP | MSC_TRANS_UNROLL_LOOP ) ) )
        {
            double                     pl1, pl2, cnt;

            l1 = MSC_GetString( d->data.alt.l1, M );
            l2 = MSC_GetString( d->data.alt.l2, M );
            pl1 = MSC_INFINITY_CONST;
            pl2 = MSC_INFINITY_CONST;
            
            if ( l1 && MSC_Calc( l1, &pl1 ) )
            {
                sprintf( num, "%g", pl1 );
                d->data.alt.l1 = MSC_AddStr( num, M );
                pl2 = pl1;
            }
            else
            {
                pl1 = MSC_INFINITY_CONST;
            }

            if ( l2 && MSC_Calc( l2, &pl2 ) )
            {
                sprintf( num, "%g", pl2 );
                d->data.alt.l2 = MSC_AddStr( num, M );
            }
            else
            {
                pl2 = MSC_INFINITY_CONST;
            }

            if ( !l2 )
            {
                pl2 = pl1;
            }
            
        /* remove loops with zero iterator */
            if ( ( ( pl1 == 0 && pl2 == 0 ) || ( pl1 == MSC_INFINITY_CONST && *l1 == '$' && pl2 == 0 ) )
                && ( flags & MSC_TRANS_REMOVE_ZERO_LOOP ) )
            {
                *s = d->next;
                MSC_FreeEventChain( d->data.alt.frame[0] );
                MSC_FreeEventData( d );
                MEM_Free( d );
                return MSC_REPEAT_ACTION;
            }

        /* if counters duplicated, remove it */
            if ( l1 && l2 && ( pl1 == pl2 ) && pl1 != MSC_INFINITY_CONST )
            {
                l2 = NULL;
                d->data.alt.l2 = 0;
            }

        /* if only one counter is specified */
            if ( d->data.alt.l1 && !d->data.alt.l2 )
            {
            /* if counter is equal to 1, and inlining is swithed ON */
                if ( pl1 == 1.0 && ( flags & MSC_TRANS_INLINE_LOOP ) )
                {
                    z = d->data.alt.frame[0];
                    x = MSC_FindEndEventChain( z );

                    if ( z->type == MSC_EV_START_LOOP && x->type == MSC_EV_END_LOOP )
                    {
                        MSC_FreeEventData( z );
                        z->type = MSC_EV_EMPTY;
                        MSC_FreeEventData( x );
                        x->type = MSC_EV_EMPTY;
                    };

                    *s = z;

/* x = MSC_FindEndEventChain(d->data.alt.frame[0]); *//* goto the end of loop */
                    x->next = d->next;

                    MSC_FreeEventData( d );
                    MEM_Free( d );
                    return MSC_REPEAT_ACTION;
                }

            /* if integer */
                if ( pl1 > 1 && pl1==floor(pl1) )
                {
                    cnt = pl1;

                    if ( cnt!=0.0 && ( flags & MSC_TRANS_UNROLL_LOOP ) && (cnt != MSC_INFINITY_CONST) )
                    {
                        z = d->data.alt.frame[0];
                        x = MSC_FindEndEventChain( z );

                        if ( z->type == MSC_EV_START_LOOP && x->type == MSC_EV_END_LOOP )
                        {
                            MSC_FreeEventData( z );
                            z->type = MSC_EV_EMPTY;
                            MSC_FreeEventData( x );
                            x->type = MSC_EV_EMPTY;
                        };

                        while ( cnt - 1 > 0 )
                        {
                            x = MSC_FindEndEventChain( z );
                            lbs = NULL;
                            lcount = 0;
                            ocount = 0;
                            inds = NULL;
                            tat_Timers = NULL;
                            tat_TimerCount = 0;
                            y = tat_DuplicateEventChain( z, M, M, NULL, NULL );

                            if ( lbs )
                                MEM_Free( lbs );

                            if ( inds )
                                MEM_Free( inds );

                            if ( tat_Timers )
                                MEM_Free( tat_Timers );

                            x->next = y;

                            z = y;

                            cnt--;
                        }

                    /* set to single loop */
                        d->data.alt.l1 = STRCOLL_STR_ID_1;
                    }
                }
            }

            if ( d->data.alt.l1 && d->data.alt.l2 )
            {
                if ( *l1 == '$' )   /* loop with counter */
                {
                    if ( pl2 >= 0 && floor(pl2)==pl2 )
                    {
                        cnt = pl2;

                        if ( (cnt>0) && ( flags & MSC_TRANS_UNROLL_LOOP ) && cnt != MSC_INFINITY_CONST )
                        {
                            z = d->data.alt.frame[0];
                            x = MSC_FindEndEventChain( z );

                            if ( z->type == MSC_EV_START_LOOP && x->type == MSC_EV_END_LOOP )
                            {
                                MSC_FreeEventData( z );
                                z->type = MSC_EV_EMPTY;
                                MSC_FreeEventData( x );
                                x->type = MSC_EV_EMPTY;
                            };

                            lbs = NULL;
                            lcount = 0;
                            ocount = 0;
                            inds = NULL;
                            tat_Timers = NULL;
                            tat_TimerCount = 0;
                            tt = d->data.alt.frame[0];
                            y = tat_DuplicateEventChain( tt, M, M, l1, "0" );
                        /* free temporary used data */
                            if ( lbs )
                                MEM_Free( lbs );

                            if ( inds )
                                MEM_Free( inds );

                            if ( tat_Timers )
                                MEM_Free( tat_Timers );

                            d->data.alt.frame[0] = y;
                            z = y;
                            for ( i = 1; ( int ) i < cnt; i++ )
                            {
                                sprintf( num, "%u", i );
                                x = MSC_FindEndEventChain( z );
                                lbs = NULL;
                                lcount = 0;
                                ocount = 0;
                                inds = NULL;
                                tat_Timers = NULL;
                                tat_TimerCount = 0;
                                y = tat_DuplicateEventChain( tt, M, M, l1, num );

                                if ( lbs )
                                    MEM_Free( lbs );

                                if ( inds )
                                    MEM_Free( inds );

                                if ( tat_Timers )
                                    MEM_Free( tat_Timers );

                                x->next = y;

                                z = y;
                            }

                            MSC_FreeEventChain( tt );
                        /* set to single loop */
                            d->data.alt.l1 = STRCOLL_STR_ID_1;
                            d->data.alt.l2 = 0;
                        }
                        else if ( *l2 == '0' && *( l2 + 1 ) == 0 && ( flags & MSC_TRANS_REMOVE_ZERO_LOOP ) )
                        {
                            *s = d->next;
                            MSC_FreeEventChain( d->data.alt.frame[0] );
                            MSC_FreeEventData( d );
                            MEM_Free( d );
                            return MSC_REPEAT_ACTION;
                        }
                    }
                }               /* loop with counter */
                else
                {
#if 0
                    if ( StrIsIntegerNumber( l1 ) && StrIsIntegerNumber( l2 ) ) /* loop with two counters */
                    {
                        cnt = MSC_Str2Int( l1 );
                        l = MSC_Str2Int( l2 );

                        if ( cnt && l )
                        {
                        /* swap maximal and minimal */

                            if ( cnt < l )
                            {
                                UINT32_T                    temp;

                                temp = cnt;
                                cnt = l;
                                l = temp;
                            }

                            if ( cnt && ( flags & MSC_TRANS_UNROLL_LOOP ) )
                            {
                                z = d->data.alt.frame[0];

                                while ( cnt - 1 )
                                {
                                    x = MSC_FindEndEventChain( z );
                                    lbs = NULL;
                                    lcount = 0;
                                    ocount = 0;
                                    inds = NULL;
                                    tat_Timers = NULL;
                                    tat_TimerCount = 0;
                                    y = tat_DuplicateEventChain( z, M, M, NULL, NULL );

                                    if ( lbs )
                                        MEM_Free( lbs );

                                    if ( inds )
                                        MEM_Free( inds );

                                    if ( tat_Timers )
                                        MEM_Free( tat_Timers );

                                    x->next = y;

                                    z = y;

                                    cnt--;
                                }

                            /* set to single loop */
                                d->data.alt.l1 = STRCOLL_STR_ID_1;
                            }
                        }

                    }
#endif
                }
            }

/*            return RESULT_OK; */
        }

        if ( ( flags & MSC_TRANS_OPER_SYNC ) && d->labeled )
        {
            x = MSC_AllocateEventType( MSC_EV_CONDITION );

            if ( !x )
                return RESULT_ERR;

            x->data.cond.text = MSC_AddStr( (char *)MSC_CONDITION_EVENT_NAME, M );
            x->data.cond.generated = 1;
            x->labeled = d->labeled;

        /* move label to condition */
            if ( x->labeled )
                M->labels[x->labeled - 1].s = x;

            d->labeled = 0;
            x->inst_Count = d->inst_Count;
            x->instances = MEM_Copy( d->instances, sizeof ( MSC_INSTANCE_ID_TYPE ) * d->inst_Count );
            x->time = d->time;
            x->time_Calc = d->time_Calc;

            if ( !x->data.cond.text )
                return RESULT_ERR;

            d->sync = MSC_ASYNC_EVENT;
            x->next = d;
            *s = x;

            return MSC_NOT_END;
        }

    }

/* StrInsert global condition before actions */
    if ( ( d->type == MSC_EV_ACTION ) && ( flags & MSC_TRANS_ACTION_SYNC ) )
    {
        MSC_ExpandInstance( d, M );
        d->sync = MSC_ASYNC_EVENT;
        return RESULT_OK;
    }

/* expand conditions */
    if ( d->type == MSC_EV_CONDITION && ( flags & MSC_TRANS_EXPN_CONDITION ) &&
        d->inst_Count != M->instance_Count )
    {
        MSC_ExpandInstance( d, M );
        return RESULT_OK;
    }

    if ( ( d->type == MSC_EV_TIMER_SET || d->type == MSC_EV_TIMER_TIMEOUT || d->type == MSC_EV_TIMER_RESET ) &&
        ( flags & MSC_TRANS_TIMER_SYNC ) )
    {
        MSC_ExpandInstance( d, M );
        return RESULT_OK;
    }

    if ( ( flags & MSC_TRANS_GLOBAL_SYNC ) )
    {
        if ( d->type != MSC_EV_CONDITION && d->type != MSC_EV_REFERENCE && d->type != MSC_EV_IN_MESSAGE &&
            d->type != MSC_EV_OPERATOR && d->type != MSC_EV_START_LOOP && d->type != MSC_EV_END_LOOP )
        {
            x = MSC_AllocateEventType( MSC_EV_CONDITION );

            if ( !x )
                return RESULT_ERR;

            x->data.cond.text = MSC_AddStr( (char *)MSC_GLOBAL_CONDITION_NAME, M );
            x->data.cond.generated = 1;
            x->inst_Count = M->instance_Count;
            x->instances = MEM_Allocate( M->instance_Count * sizeof ( MSC_INSTANCE_ID_TYPE ) );

            for ( i = 0; i < ( int ) M->instance_Count; i++ )
            {
                x->instances[i] = i;
            }

            MSC_ZeroEventTime( x );
            x->next = d;
            *s = x;
            s = &( x->next );
            return MSC_NOT_END;
        }
    }
    else
    /* process syncronous signals */
    if ( ( d->sync == MSC_SYNC_EVENT ) && ( flags & MSC_TRANS_USE_SYNC ) &&
        !( flags & MSC_TRANS_GLOBAL_SYNC ) && d->type != MSC_EV_CONDITION )
    {

        if ( d->type == MSC_EV_OUT_MESSAGE ||
            d->type == MSC_EV_RECV_SIGNAL || d->type == MSC_EV_SEND_SIGNAL || d->type == MSC_EV_EXIST_SIGNAL )
        {
            if ( ( d->inst_Count == 1 ) &&
                ( d->data.msg.inst >= 0 ) && ( ( ( int ) d->instances[0] ) != d->data.msg.inst ) )
            {
                d->inst_Count = 2;  /* hack to expand MSC_EV_OUT_MESSAGE */
                d->instances = MEM_Reallocate( d->instances, 2 * sizeof ( MSC_INSTANCE_ID_TYPE ) );
                d->instances[1] = d->data.msg.inst;
            }
        }
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : msc_Transf
 *
 * Description: process MSC by calling 'f' function
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL msc_Transf( MSC_STORAGE * M, MSC_EVENT_KIND * s, MSC_PREVNEXT_PROCESS_FUNC f, int flags )
{
    UINT32_T                    i, j;
    MSC_EVENT_KIND              *next_pos;

    while ( s && ( s->next || s->type == MSC_EV_OPERATOR ) )
    {
        next_pos = s;
        i = MSC_REPEAT_ACTION;

        while ( i == MSC_REPEAT_ACTION )
        {
            i = f( M, &( s->next ), s->next, flags );

            if ( i == RESULT_ERR )
                return RESULT_ERR;

            if ( i == MSC_NOT_END )
            {

                next_pos = s->next;
            }

        }

        if ( s->type == MSC_EV_OPERATOR )
        {
            i = 0;

            while ( i < s->data.alt.frame_Count )
            {
                j = MSC_REPEAT_ACTION;

                while ( j == MSC_REPEAT_ACTION )
                {
                    j = f( M, &( s->data.alt.frame[i] ), s->data.alt.frame[i], flags );

                    if ( ( s->data.alt.frame[i] == NULL ) && ( s->data.alt.type != MSC_OP_ALT ) && ( s->data.alt.type != MSC_OP_EXC ) )
                    {
                        /* remove empty frame */
                        memcpy( &( s->data.alt.frame[i] ), &( s->data.alt.frame[i + 1] ),
                            ( s->data.alt.frame_Count - i - 1 ) * sizeof ( MSC_EVENT_KIND * ) );
                        s->data.alt.frame_Count--;

                        if ( i >= s->data.alt.frame_Count )
                            goto break_oper;
                    }

                    if ( j == RESULT_ERR )
                        return RESULT_ERR;

                    if ( j == MSC_NOT_END )
                        break;
                }

                if ( j != MSC_NOT_END )
                {
                    if ( !msc_Transf( M, s->data.alt.frame[i], f, flags ) )
                        return RESULT_ERR;
                }
                else
                {
                    if ( !msc_Transf( M, s->data.alt.frame[i]->next, f, flags ) )
                        return RESULT_ERR;
                }

                i++;
            }

          break_oper:
        /* check for empty operator */

            if ( s->data.alt.frame_Count == 0 )
            {
                if ( s->labeled && s->next && s->next->labeled )
                {
                /* propagate label */
                    M->labels[s->next->labeled - 1].s = s;
                }

                MSC_FreeEventData( s );

                if ( s->next )
                {
                    MSC_EVENT_KIND              *x = s->next;

                    memcpy( s, s->next, sizeof ( MSC_EVENT_KIND ) );
                    MEM_Free( x );
                }
                else
                {
                    s->type = MSC_EV_EMPTY;
                }

                continue;
            }
        }

        s = next_pos->next;
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_PR_Transform
 *
 * Description: tree traversal on MSC, calls user's function for each
 *              pair of events
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_PR_Transform( MSC_STORAGE * M, MSC_PREVNEXT_PROCESS_FUNC f, int flags )
{
    UINT32_T                    i;

    if ( !M )
        return RESULT_ERR;

    do
    {
        i = f( M, &( M->start ), M->start, flags );
    }
    while ( i == MSC_REPEAT_ACTION );

    if ( !i )
        return RESULT_ERR;

    if ( i == MSC_NOT_END )
    {
    /* if first is alt */

        if ( M->start->type == MSC_EV_OPERATOR )
        {
            for ( i = 0; i < M->start->data.alt.frame_Count; i++ )
            {
                if ( !f( M, &( M->start->data.alt.frame[i] ), M->start->data.alt.frame[i], flags ) )
                    return RESULT_ERR;

                if ( !msc_Transf( M, M->start->data.alt.frame[i], f, flags ) )
                    return RESULT_ERR;
            }
        }

        return msc_Transf( M, M->start->next, f, flags );
    }
    else
        return msc_Transf( M, M->start, f, flags );
}

/***************************************************************************
 * Function : MSC_Transform
 *
 * Description: Conduct MSC processing, specified by flags
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_Transform( MSC_STORAGE * M, int flags )
{
    return MSC_PR_Transform( M, msc_GenProc, flags );
}

static MSC_INSTANCE_ID_TYPE *tat_InstConversion;
static size_t            *tat_SrcRefIndex;
static MSC_STORAGE *tat_DestMsc;

/***************************************************************************
 * Function : msc_ObjectRemap
 *
 * Description: map source MSC objects to destination MSC
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL msc_ObjectRemap( MSC_STORAGE * M, MSC_EVENT_KIND * s )
{
    size_t                    i;

/* modify instances */

/* propagate global events to all instances */

    if ( s->inst_Count == M->instance_Count &&
        ( s->type == MSC_EV_OPERATOR || s->type == MSC_EV_CONDITION || s->type == MSC_EV_REFERENCE ) )
    {
        s->instances =
            MEM_Reallocate( s->instances, tat_DestMsc->instance_Count * sizeof ( MSC_INSTANCE_ID_TYPE ) );
        s->inst_Count = tat_DestMsc->instance_Count;

        for ( i = 0; i < s->inst_Count; i++ )
            s->instances[i] = i;
    }
    else
    {
        for ( i = 0; i < s->inst_Count; i++ )
        {
            s->instances[i] = tat_InstConversion[s->instances[i]];
        }
    }

    if ( s->type == MSC_EV_IN_MESSAGE || s->type == MSC_EV_OUT_MESSAGE ||
        s->type == MSC_EV_SEND_SIGNAL || s->type == MSC_EV_RECV_SIGNAL || s->type == MSC_EV_EXIST_SIGNAL )
    {
        if ( s->data.msg.inst >= 0 )
            s->data.msg.inst = tat_InstConversion[s->data.msg.inst];
    }

    if ( s->type == MSC_EV_CREATE )
    {
        s->data.create.instance = tat_InstConversion[s->data.create.instance];
    }

/* update source-file references */
    for ( i = 0; i < s->src_Count; i++ )
    {
        s->src_Data[i].link_Index = tat_SrcRefIndex[s->src_Data[i].link_Index];
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_InstanceProp
 *
 * Description: propagate global events for sub-MSC to all instances 
 *              of destination MSC
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static size_t             tat_OldInstCount;
static LOGICAL tat_InstanceProp( MSC_STORAGE * M, MSC_EVENT_KIND * s )
{
    size_t                    i;

/* propagate global events to all instances */
    if ( s->inst_Count == tat_OldInstCount &&
        ( s->type == MSC_EV_OPERATOR || s->type == MSC_EV_CONDITION || s->type == MSC_EV_REFERENCE ) )
    {
        s->instances = MEM_Reallocate( s->instances, M->instance_Count * sizeof ( MSC_INSTANCE_ID_TYPE ) );
        s->inst_Count = M->instance_Count;

        for ( i = 0; i < s->inst_Count; i++ )
            s->instances[i] = i;
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_Include
 *
 * Description: Replace event with content of another MSC
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_Include( MSC_STORAGE * D, MSC_STORAGE * S, MSC_EVENT_KIND * s, int sync )
{
    size_t                  i, k;
    int                     u;
    char                   *l;
    MSC_EVENT_KIND              *n, *m, *res;
    if ( !D || !S || !s )
        return RESULT_ERR;

    S=MSC_Copy(S);

/* adjust sync for empty MSC's */
    sync = sync && ( S->start != NULL );

    tat_DestMsc = D;

    if ( !S )
        return RESULT_ERR;

/* copy unique text fields */
    for ( i = 0; i < S->text_Count; i++ )
    {
        l = MSC_GetString( S->texts[i], S );

        if ( l )
        {
            for ( k = 0; k < D->text_Count; k++ )
            {
                if ( !strcmp( l, MSC_GetString( D->texts[k], D ) ) )
                    break;
            }
        }
        else
        {
            k = D->text_Count;
        }

        if ( k == D->text_Count )
        {
            D->texts = MEM_Reallocate( D->texts, ( D->text_Count + 1 ) * sizeof ( STRING_ID ) );
            D->texts[D->text_Count] = MSC_AddStr( l, D );
            D->text_Count++;
        }
    }

    for ( i = 0; i < S->var_Count; i++ )
    {
        l = MSC_GetStr( S->vars[i].pv_Name, S );

        for ( k = 0; k < D->var_Count; k++ )
        {
            if ( !strcmp( l, MSC_GetStr( D->vars[k].pv_Name, D ) ) )
            {
                if ( !stricmp( MSC_GetStr( S->vars[i].pv_Type, S ), MSC_GetStr( D->vars[k].pv_Type, D ) ) )
                {
                    MSG_PrintError( MSG_INCOMPATIBLE_TYPE_susss, MSC_GetStr(S->msc_File, S),S->vars[i].line_number,
                        MSC_GetString( S->vars[i].pv_Type, S ),
                        MSC_GetString( D->vars[k].pv_Type, D ), l );
                }

                break;
            }
        }

        if ( k == D->var_Count )
        {
            D->vars = MEM_Reallocate( D->vars, ( D->var_Count + 1 ) * sizeof ( D->vars[0] ) );
            D->vars[D->var_Count].pv_Name = MSC_AddStr( l, D );
            D->vars[D->var_Count].pv_Type = MSC_AddStr( MSC_GetString( S->vars[i].pv_Type, S ), D );
            D->var_Count++;
        }
    }

    tat_InstConversion = NULL;

    if ( !S->labels[0].unused && D->label_Count )
    {
    /* add label definition */
        n = MSC_GlobalCond( S );
        MSC_ZeroEventTime( n );
        n->labeled = 1;
        n->next = S->start;
        S->start = n;
        S->labels[0].s = n;
    }

    if ( !D->label_Count )
    {
        D->label_Count = 1;
        D->labels = MEM_Allocate( sizeof ( MSC_EVENT_KIND_LABEL ) );
        D->labels[0].label = MSC_AddStr( (char *)STR_MSCSTART, D );
        D->labels[0].unused = 1;
    }

/* instance consolidation */
    if ( S->instance_Count || S->src_Count )
    {
        if ( S->instance_Count )
        {
            tat_InstConversion = MEM_Allocate( S->instance_Count * sizeof ( tat_InstConversion[0] ) );

            tat_OldInstCount = D->instance_Count;

            for ( i = 0; i <  S->instance_Count; i++ )
            {
                l = MSC_GetString( S->instances[i].name, S );

                if ( ( u = MSC_GetInstanceID( l, D ) ) < 0 )
                {
                    k = D->instance_Count;
                    D->instances = MEM_Reallocate( D->instances, ( k + 1 ) * sizeof ( MSC_INSTANCE_STORAGE ) );
                    D->instances[k].name = MSC_AddStr( l, D );
                    D->instances[k].ratio = MSC_AddStr( MSC_GetString( S->instances[i].ratio, S ), D );
                    D->instances[k].type = MSC_AddStr( MSC_GetString( S->instances[i].type, S ), D );
                    D->instances[k].comment =
                        MSC_AddStr( MSC_GetString( S->instances[i].comment, S ), D );
                    D->instances[k].decompose =
                        MSC_AddStr( MSC_GetString( S->instances[i].decompose, S ), D );
/*                    D->instances[k].ind = S->instances[i].ind; */
                    tat_InstConversion[i] = k;
                    ( D->instance_Count )++;
                }
                else
                {
                    tat_InstConversion[i] = u;
                }
            }

            if ( tat_OldInstCount != D->instance_Count )
            {
                MSC_Process( D, tat_InstanceProp );
            }

        }

        if ( S->src_Count )
        {
            tat_SrcRefIndex = MEM_Allocate( S->src_Count * sizeof ( tat_SrcRefIndex[0] ) );
            for ( i = 0; i < S->src_Count; i++ )
            {
                l = MSC_GetString( S->src_Files[i].link, S );

                if ( ( u = MSC_GetLinkID( l, D ) ) < 0 )
                {
                    k = D->src_Count;
                    D->src_Files = MEM_Reallocate( D->src_Files, ( k + 1 ) * sizeof ( D->src_Files[0] ) );
                    D->src_Files[k].link = MSC_AddStr( l, D );
                    D->src_Files[k].first_Line = S->src_Files[i].first_Line;
                    D->src_Files[k].last_Line = S->src_Files[i].last_Line;
                    D->src_Files[k].level = S->src_Files[i].level;
                    tat_SrcRefIndex[i] = k;
                    ( D->src_Count )++;
                }
                else
                {
                    tat_SrcRefIndex[i] = u;
                }
            }
        }

        MSC_Process( S, msc_ObjectRemap );   /* modify instances & indicators */
    }

/* TD DO: Check carefully */
    if ( sync && ( S->labels[0].unused || D->label_Count == 0 ) )
    {                           /* StrInsert global condition */
        n = ( MSC_EVENT_KIND * ) MEM_Copy( s, sizeof ( MSC_EVENT_KIND ) );

        memset( s, 0, sizeof ( s[0] ) );
        s->type = MSC_EV_CONDITION;
        MSC_ZeroEventTime( s );
        s->inst_Count = D->instance_Count;
        s->instances = MEM_Allocate( s->inst_Count * sizeof ( MSC_INSTANCE_ID_TYPE ) );
    /* really global condition!!! */

        for ( i = 0; i < ( UINT32_T ) D->instance_Count; i++ )
            s->instances[i] = i;

        s->data.cond.text = MSC_AddStr( (char *)MSC_REF_CONDITION_NAME, D );
        s->data.cond.generated = 1;
        s->next = n;
        s = n;
    }

/* save continuation of destination MSC */
    n = s->next;

/* clean related structure of original event */
    MSC_FreeEventData( s );

/* prepeare empty event for empty MSC */
    if ( !S->start )
    {
        s->type = MSC_EV_EMPTY;
        s->next = NULL;
    }

    lbs = NULL;
    lcount = 0;
    ocount = 0;
    inds = NULL;
    tat_Timers = NULL;
    tat_TimerCount = 0;

/* make copy with substitutions */
    res = tat_DuplicateEventChain( S->start, D, S, NULL, NULL );

    if ( lbs )
        MEM_Free( lbs );

    if ( inds )
        MEM_Free( inds );

    if ( tat_Timers )
        MEM_Free( tat_Timers );

/* remove source */
    MSC_FreeMemory( S );

    if ( res )
        memcpy( s, res, sizeof ( MSC_EVENT_KIND ) );

    s = MSC_FindEndEventChain( s );

    if ( sync )
    {
        m = ( MSC_EVENT_KIND * ) MEM_Allocate( sizeof ( MSC_EVENT_KIND ) );
        m->type = MSC_EV_CONDITION;
        MSC_ZeroEventTime( m );
        m->inst_Count = D->instance_Count;
        m->instances = MEM_Allocate( m->inst_Count * sizeof ( MSC_INSTANCE_ID_TYPE ) );
    /* really global condition!!! */

        for ( i = 0; i < D->instance_Count; i++ )
            m->instances[i] = i;

        m->next = n;
        m->data.cond.text = MSC_AddStr( (char *)MSC_REF_CONDITION_NAME, D );
        m->data.cond.generated = 1;
        n = m;
    }

    if ( s )
        s->next = n;

/* free event */
    if ( res )
        MEM_Free( res );

/* free copy of MSC */
    MEM_Free( S );

/* free instance transformation array */
    if ( tat_InstConversion )
        MEM_Free( tat_InstConversion );

    if ( tat_SrcRefIndex )
        MEM_Free( tat_SrcRefIndex );

    return RESULT_OK;
}

static size_t  p_line = 0;
static LOGICAL msc_MaxLine( MSC_STORAGE * M, MSC_EVENT_KIND * x )
{
    if ( x->line_Number > p_line )
        p_line = x->line_Number;
    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_GetLastSourceLine
 *
 * Description: Get last line with event from MSC
 *
 * Returns: number
 *
 * Notes: None
 *
 **************************************************************************/
size_t MSC_GetLastSourceLine( MSC_STORAGE * M )
{
    p_line = 0;
    MSC_Process( M, msc_MaxLine );
    return p_line;
}
