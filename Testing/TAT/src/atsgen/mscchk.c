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
* Functions:    
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  
*
* Notes:        None
*
******************************************************************************/
#include <portable.h>
#include <mscbase.h>
#include <fileproc.h>
#include <outmess.h>
#include <qmemory.h>
#include <strproc.h>
#include <mscparse.h>
#include <mscproc.h>
#include <msctime.h>
#include <partial.h>
#include <genpath.h>
#include <mscmess.h>
#include <xmlconf.h>
#include <confproc.h>

#include "mscchk.h"

/***************************************************************************
 * Function : MSC_CheckEvents
 *
 * Description: check messages to be correct
 *
 * Returns:
 *
 * Notes:
 *
 **************************************************************************/
int MSC_CheckEvents( MSC_STORAGE * M, MSC_EVENT_TREE * TM, TAT_XML_CONFIG_DEF * d )
{
    size_t                  i, k;
    int                     j;
    MSC_EVENT_KIND              *s;
    MSC_MESSAGE_STORAGE    *x;
    char                   *signame;

    if ( !M || !d )
        return RESULT_ERR;

    if ( !TM )
        return RESULT_OK;

    for ( i = 0; i < TM->ev_Number; i++ )
    {
        s = TM->linear[i];

        if ( s->type != MSC_EV_IN_MESSAGE && s->type != MSC_EV_OUT_MESSAGE && s->type != MSC_EV_SEND_SIGNAL &&
            s->type != MSC_EV_RECV_SIGNAL && s->type != MSC_EV_EXIST_SIGNAL )
        {
            continue;
        }

        x = MSC_GetMessageParams( s->data.msg.indicator, M );
        signame = MSC_GetString( x->message_Name, M );

		j = CFG_GetSignalID( signame );
        if ( j < 0 )
        {
            fprintf( MSG_ErrorStream, MSG_CONFIG_UNDEF_SIGNAL_sus,
                MSC_GetString( M->msc_File, M ), x->line_Number, signame );
            return RESULT_ERR;
        }
        else
        {
            if ( s->type == MSC_EV_SEND_SIGNAL ||
                ( s->type == MSC_EV_OUT_MESSAGE && M->instances[s->instances[0]].ind == MSC_INSTANCE_ENV ) )
            {
                if ( ( d->signals[j].param_Count > x->param_Count ) )
                {
                    fprintf( MSG_ErrorStream,
                        "ERROR: semantic: file '%s(%u)': output signal '%s' with undefined parameters\n",
                        MSC_GetString( M->msc_File, M ), x->line_Number, signame );
                    return RESULT_ERR;
                }

                for ( k = 0; k < x->param_Count; k++ )
                    if ( x->message_Params[k] == 0 )
                    {
                        fprintf( MSG_ErrorStream,
                            "ERROR: semantic: file '%s(%u)': output signal '%s' with undefined parameters\n",
                            MSC_GetString( M->msc_File, M ), x->line_Number, signame );
                        return RESULT_ERR;
                    };

                if ( d->signals[j].type == MSC_SIG_OUT ||
                    ( ( d->signals[j].param_Count < x->param_Count ) && d->signals[j].param_Extra != 1 ) )
                {
                    fprintf( MSG_ErrorStream, "ERROR: semantic: file '%s(%u)': output invalid signal '%s'\n",
                        MSC_GetString( M->msc_File, M ), x->line_Number, signame );
                    return RESULT_ERR;
                }
            }

            if ( s->type == MSC_EV_RECV_SIGNAL ||
                ( s->type == MSC_EV_OUT_MESSAGE && M->instances[s->instances[0]].ind == MSC_INSTANCE_IUT ) )
            {
                if ( d->signals[j].type == MSC_SIG_IN )
                {
                    fprintf( MSG_ErrorStream, "ERROR: semantic: file '%s(%u)': output invalid signal '%s'\n",
                        MSC_GetString( M->msc_File, M ), x->line_Number, signame );

                    return RESULT_ERR;
                }
            }
        }
    }                           /* for ( i = 0; i < TM->ev_Number; i++ ) */

    for ( i = 0; i < TM->var_Number; i++ )
    {
        if ( !MSC_CheckEvents( M, TM->frames[i], d ) )
        {
            return RESULT_ERR;
        };
    }

    return MSC_CheckEvents( M, TM->next, d );
}

/***************************************************************************
 * Function : MSC_set_labeled_pair
 *
 * Description:
 *
 * Returns:
 *
 * Notes:
 *
 **************************************************************************/
static int MSC_set_labeled_pair( MSC_STORAGE * M, MSC_EVENT_KIND * s, MSC_EVENT_KIND * d )
{
    UINT32_T                    i;

    while ( s )
    {
        if ( ( s->type == MSC_EV_IN_MESSAGE && d->type == MSC_EV_OUT_MESSAGE ) ||
            ( s->type == MSC_EV_OUT_MESSAGE && d->type == MSC_EV_IN_MESSAGE ) )
            if ( s->data.msg.indicator == d->data.msg.indicator )
            {
                if ( !s->labeled )
                {
                    s->labeled = d->labeled;
                }
                else
                {
                    fprintf( MSG_ErrorStream, "WARNING: Ambigious labels '%s' and '%s'\n",
                        MSC_GetString( M->labels[s->labeled - 1].label, M ),
                        MSC_GetString( M->labels[d->labeled - 1].label, M ) );
                /* prefer to remove generated label */
                    if ( M->labels[d->labeled - 1].generated )
                    {
                    /* duplicate label in output */
                        M->labels[d->labeled - 1].label = M->labels[s->labeled - 1].label;
                    /* mark time label as unused */
                        M->labels[d->labeled - 1].unused = 1;
                    }
                    else
                    {
                    /* duplicate label in output */
                        M->labels[s->labeled - 1].label = M->labels[d->labeled - 1].label;
                    /* mark time label as unused */
                        M->labels[s->labeled - 1].unused = 1;
                    }
                }

                s->time_Calc.min_Time = MSC_MaxTime( d->time_Calc.min_Time, s->time_Calc.min_Time );

                s->time_Calc.max_Time = MSC_MaxTime( d->time_Calc.max_Time, s->time_Calc.max_Time );

                return 1;
            };

        if ( s->type == MSC_EV_OPERATOR )
        {
            for ( i = 0; i < s->data.alt.frame_Count; i++ )
                if ( MSC_set_labeled_pair( M, s->data.alt.frame[i], d ) )
                    return 1;   /*found */
        };

        s = s->next;
    };

    return 0;
}

/***************************************************************************
 * Function : MSC_set_commented_pair
 *
 * Description:
 *
 * Returns:
 *
 * Notes:
 *
 **************************************************************************/
static int MSC_set_commented_pair( MSC_STORAGE * M, MSC_EVENT_KIND * s, MSC_EVENT_KIND * d )
{
    UINT32_T                    i;

    while ( s )
    {
        if ( ( s->type == MSC_EV_IN_MESSAGE && d->type == MSC_EV_OUT_MESSAGE ) ||
            ( s->type == MSC_EV_OUT_MESSAGE && d->type == MSC_EV_IN_MESSAGE ) )
            if ( s->data.msg.indicator == d->data.msg.indicator )
            {
                if ( !s->comment )
                    s->comment = d->comment;

                return 1;
            };

        if ( s->type == MSC_EV_OPERATOR )
        {
            for ( i = 0; i < s->data.alt.frame_Count; i++ )
                if ( MSC_set_commented_pair( M, s->data.alt.frame[i], d ) )
                    return 1;   /*found */
        };

        s = s->next;
    };

    return 0;
}

/***************************************************************************
 * Function : MSC_mod
 *
 * Description:
 *
 * Returns:
 *
 * Notes:
 *
 **************************************************************************/
static int MSC_mod( MSC_STORAGE * M, MSC_EVENT_KIND ** s, MSC_EVENT_KIND * d, int flags )
{
    if ( !d )
        return RESULT_OK;

    if ( MSC_TestEvent( M, d, flags ) == MSC_REMOVE_ACTION )
    {

    /* label saving */
        d->deleted = 1;
        if ( d->labeled && ( d->type == MSC_EV_IN_MESSAGE || d->type == MSC_EV_OUT_MESSAGE ) )
        {
            MSC_set_labeled_pair( M, M->start, d );
        };

        if ( d->comment && ( d->type == MSC_EV_IN_MESSAGE || d->type == MSC_EV_OUT_MESSAGE ) )
        {
            MSC_set_commented_pair( M, M->start, d );
        };

        *s = d->next;

        MSC_FreeEventData( d );

        MEM_Free( d );

        return MSC_REPEAT_ACTION;
    };

    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_ReduceEvents
 *
 * Description: merge couple of events with label/comment saving
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes:
 *
 **************************************************************************/
LOGICAL MSC_ReduceEvents( MSC_STORAGE * M, int flags )
{
    return MSC_PR_Transform( M, MSC_mod, flags );
}
