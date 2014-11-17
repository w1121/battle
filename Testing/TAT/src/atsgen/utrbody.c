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
* Functions:    MSC_AtsOutTime
*               MSC_AtsOutTimer
*               MSC_AtsGenKeyword
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
#include <qmemory.h>
#include <fileproc.h>
#include <strproc.h>
#include <mscbase.h>

#include <partial.h>
#include <genpath.h>
#include <mscparse.h>
#include <mscproc.h>
#include <msctime.h>
#include <outmess.h>
#include <cmdline.h>
#include <testtree.h>
#include <mscmess.h>
#include <mscstrs.h>
#include <confproc.h>
#include <tatgdata.h>

#include "utgflags.h"
#include "utrhead.h"
#include "sigopt.h"

#include "asn1proc.h"

extern Bool tat_ASN1;

/***************************************************************************
 * Function : out_params
 *
 * Description: Output message parameters into Out
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL out_params( FILE * Out, MSC_STORAGE * M, MSC_MESSAGE_STORAGE * p )
{
    size_t                  i, j;
    char                    dest[TAT_MAX_LINE_LEN];

/* check input parameters for correctness */

    if ( !Out || !M || !p )
        return RESULT_ERR;
    fprintf( Out, "[ list " );
  
    for ( i = 0; i < p->param_Count; i++ )
    {
        /* output variable name */
        fprintf(Out, "[ list \"%s\" ", MSC_GetStr( p->params_Values[i].varName, M ) );
        for (j=0; j<p->params_Values[i].valueCount; j++)
        {
            /*StrEmbedLineQuotes( dest, MSC_GetStr( p->params_Values[i].values[j], M ) );*/
			if(ParseASN1(MSC_GetStr( p->params_Values[i].values[j], M ),dest) || !tat_ASN1)	/* if ASN.1 parser fails */
					StrEmbedLineQuotes( dest, MSC_GetStr( p->params_Values[i].values[j], M ) );	/* output as is */
			fprintf( Out, "%s ", dest );
		}
		fputs (" ] ", Out );
    }

    fputs (" ] ", Out );
    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_AtsOutTime
 *
 * Description: output event time 
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_AtsOutTime( FILE * Out, MSC_STORAGE * M, MSC_TIME_INTERVAL time_Calc )
{
    char *lab_name;
    fprintf( Out, "[ list " );

    switch ( time_Calc.type )
    {

    case MSC_RELATIVE_TIME:
        fprintf( Out, "REL " );
        break;

    case MSC_LABELED_TIME:
        fprintf( Out, "ABS " );
        break;
    };

    if ( ( time_Calc.type == MSC_LABELED_TIME ) && ( time_Calc.time_Label ) )
    {
        lab_name = MSC_GetStr( M->labels[time_Calc.time_Label].label, M );
        fprintf( Out, "\"@%s+%s\" \"@%s+%s\" ", lab_name, MSC_GetStr( time_Calc.str_MinTime, M ),
            lab_name, MSC_GetStr( time_Calc.str_MaxTime, M ) );

    }
    else
    {
        fprintf( Out, "\"%s\" \"%s\" ", MSC_GetStr( time_Calc.str_MinTime, M ),
            MSC_GetStr( time_Calc.str_MaxTime, M ) );
    }

    fprintf( Out, "] " );

    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_AtsOutTimer
 *
 * Description: output Timer specification
 *
 * Returns: 
 *
 * Notes: None
 *
 **************************************************************************/
int MSC_AtsOutTimer( FILE * Out, MSC_STORAGE * M, MSC_TIMER_TIME time_Calc )
{
    fprintf( Out, "[ list " );

    switch ( time_Calc.type )
    {

    case MSC_RELATIVE_TIMER:
        fprintf( Out, "REL " );
        break;

    case MSC_ABSOLUTE_TIMER:
        fprintf( Out, "ABS " );
        break;
    };

    fprintf( Out, "\"%s\" \"%s\" ] ", MSC_GetStr( time_Calc.str_MinTime, M ),
        MSC_GetStr( time_Calc.str_MaxTime, M ) );

    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_AtsGenKeyword
 *
 * Description: generate event type
 *
 * Returns: pointer to string
 *
 * Notes: None
 *
 **************************************************************************/
char                   *MSC_AtsGenKeyword( MSC_EVENT_KIND * x )
{
    if ( !x )
        return "empty";

/* empty |condition |send |receive |check |input |output |action |break |reference |create |
   stop |save_time |timer_set |reset_time |timer_reset |check_time |timer_timeout |concurrent_start |
   concurrent_end |code |start_loop | end_loop ;
 */

    switch ( x->type )
    {

    case MSC_EV_OPERATOR:
    {
        if ( x->data.alt.flag_End )
        {
            switch ( x->data.alt.type )
            {
            case MSC_OP_ALT:
                return "end_alt";
            case MSC_OP_OPT:
                return "end_opt";
            case MSC_OP_EXC:
                return "end_exc";
            case MSC_OP_PAR:
                return "end_par";
            case MSC_OP_LOOP:
                return "end_loop";
            case MSC_OP_SEQ:
                return "end_seq";
            }
        }
        else
        {
            switch ( x->data.alt.type )
            {
            case MSC_OP_ALT:
                return "alt";
            case MSC_OP_OPT:
                return "opt";
            case MSC_OP_EXC:
                return "exc";
            case MSC_OP_PAR:
                return "par";
            case MSC_OP_LOOP:
                return "loop";
            case MSC_OP_SEQ:
                return "seq";
            }
        }
        break;
    }
    case MSC_EV_SEND_SIGNAL:
        return "send";

    case MSC_EV_EXIST_SIGNAL:
        return "chck";

    case MSC_EV_RECV_SIGNAL:
        return "recv";

    case MSC_EV_IN_MESSAGE:
        return "inpt";

    case MSC_EV_OUT_MESSAGE:
        return "outp";

    case MSC_EV_ACTION:
        return "actn";

    case MSC_EV_CONCURRENT_START:
        return "concurrent_start";

    case MSC_EV_CONCURRENT_END:
        return "concurrent_end";

    case MSC_EV_CODE:
        return "code";

    case MSC_EV_START_LOOP:
        return "start_loop";

    case MSC_EV_END_LOOP:
        return "end_loop";

    case MSC_EV_CONDITION:
        return "cond";

    case MSC_EV_REFERENCE:
        return "refr";

    case MSC_EV_CREATE:
        return "create";

    case MSC_EV_EMPTY:
        return "empty";

    case MSC_EV_STOP:
        return "stop";

    case MSC_EV_TIMER_SET:
    {
        if ( x->data.timer.timing.type != MSC_SPECIAL_TIMER )
            return "timer_set";
        else
            return "save_time";
    };

    case MSC_EV_TIMER_RESET:
    {
        if ( x->data.timer.timing.type != MSC_SPECIAL_TIMER )
            return "timer_reset";
        else
            return "reset_time";
    };

    case MSC_EV_TIMER_TIMEOUT:
    {
        if ( x->data.timer.timing.type != MSC_SPECIAL_TIMER )
            return "timer_timeout";
        else
            return "check_time";
    };

    case MSC_EV_BREAK:
        return "break";
    };

    return "undefined";
}

/***************************************************************************
 * Function : MSC_AtsGenTime
 *
 * Description: Generate event's time
 *
 * Returns: None
 *
 * Notes: None
 *
 **************************************************************************/
void MSC_AtsGenTime( FILE * out, MSC_STORAGE * M, MSC_EVENT_KIND * x )
{
    if ( MSC_OutCalcTime )
        MSC_AtsOutTime( out, M, x->time_Calc );
    else
        MSC_AtsOutTime( out, M, x->time );
}

/***************************************************************************
 * Function : MSC_AtsGenInstances
 *
 * Description: Generate list of instances
 *
 * Returns: None
 *
 * Notes: None
 *
 **************************************************************************/
void MSC_AtsGenInstances( FILE * out, MSC_STORAGE * M, MSC_EVENT_KIND * x )
{
    size_t                    i;

    fputs( " [list ", out );

    for ( i = 0; i < x->inst_Count; i++ )
        if ( M->instances[x->instances[i]].type != MSC_INSTANCE_IGNORE )
        {
            fprintf( out, " %d ", CFG_GetInstanceID( MSC_GetStr( M->instances[x->instances[i]].name, M ) ) );
        };

    if ( MSC_IsMessageEvent( x ) && ( x->inst_Count == 1 ) )
    {
        fprintf( out, " %d ", CFG_GetInstanceID( MSC_GetStr( M->instances[x->data.msg.inst].name, M ) ) );
    }

    fputs( " ]", out );
}

/***************************************************************************
 * Function : MSC_AtsGenData
 *
 * Description: Generate event's data
 *
 * Returns: None
 *
 * Notes: None
 *
 **************************************************************************/
void MSC_AtsGenData( FILE * out, MSC_STORAGE * M, MSC_EVENT_KIND * x )
{
    char                    dest[TAT_MAX_LINE_LEN];
    MSC_MESSAGE_STORAGE    *p;
    size_t                    i;

    fputs( " [list ", out );

    switch ( x->type )
    {

    case MSC_EV_OPERATOR:
    {
        if ( ( x->data.alt.type == MSC_OP_PAR ) || ( x->data.alt.type == MSC_OP_ALT ) )
        {
            fprintf( out, " %u ", x->data.alt.frame_Count );
        }
        break;
    }
    case MSC_EV_SEND_SIGNAL:
    case MSC_EV_EXIST_SIGNAL:
    case MSC_EV_RECV_SIGNAL:
    case MSC_EV_IN_MESSAGE:
    case MSC_EV_OUT_MESSAGE:
    {
        p = MSC_GetMessageParams( x->data.msg.indicator, M );
    /* output signal identificator */
        fprintf( out, " %d ", CFG_GetSignalID( MSC_GetStr( p->message_Name, M ) ) );

    /* output parameters list */
        out_params( out, M, p );

    /* output message identificator */
        fprintf( out, " %u ", MSC_MapMessageID( x->data.msg.indicator ) );

        MSC_AtsOutTime( out, M, p->delay );

        break;
    }

    case MSC_EV_CODE:

    case MSC_EV_ACTION:
    {
        StrEmbedLineQuotes( dest, MSC_GetStr( x->data.text, M ) );
        fputs( dest, out );
        break;
    }

    case MSC_EV_STOP:
    case MSC_EV_CONCURRENT_END:
    case MSC_EV_CONCURRENT_START:
    case MSC_EV_END_LOOP:

    case MSC_EV_BREAK:
        break;

    case MSC_EV_START_LOOP:
    {
        StrEmbedLineQuotes( dest, MSC_GetStr( x->data.loop.text, M ) );
        fprintf( out, "\"%s\" \"%s\" %s", MSC_GetStr( x->data.loop.l1, M ),
            MSC_GetStr( x->data.loop.l2, M ), dest );
        break;
    }

    case MSC_EV_CONDITION:
    {
        StrEmbedLineQuotes( dest, MSC_GetStr( x->data.cond.call, M ) );
        fprintf( out, "\"%s\" %s", MSC_GetStr( x->data.cond.text, M ), dest );
        break;
    }

    case MSC_EV_REFERENCE:
    {

        StrEmbedLineQuotes( dest, MSC_GetStr( x->data.ref.refr_Name, M ) );
        fprintf( out, "%s [list", dest );

        for ( i = 0; i < x->data.ref.param_Count; i++ )
        {
            StrEmbedLineQuotes( dest, MSC_GetStr( x->data.ref.refr_Subst[i], M ) );
            fprintf( out, " [list \"%s\" %s]", x->data.ref.macro_Name[i], dest );
        }

        fputs( " ]", out );
        break;
    };

    case MSC_EV_CREATE:
    {
        fprintf( out, "%d [list",
            CFG_GetInstanceID( MSC_GetStr( M->instances[x->data.create.instance].name, M ) ) );

        for ( i = 0; i < x->data.create.param_Count; i++ )
        {
            StrEmbedLineQuotes( dest, MSC_GetStr( x->data.create.params[i], M ) );
            fprintf( out, " \"%s\"", dest );
        }

        fputs( " ]", out );
        break;
    };

    case MSC_EV_TIMER_SET:
    {
        if ( x->data.timer.timing.type != MSC_SPECIAL_TIMER )
        {
            fprintf( out, "\"%s\" %u %s \"%s\"",
                MSC_GetStr( x->data.timer.signal_Name, M ),
                x->data.timer.timer_ID,
                ( x->data.timer.timing.type == MSC_ABSOLUTE_TIMER ) ? "ABS" : "REL",
                MSC_GetStr( x->data.timer.timing.str_MaxTime, M ) );
        }
        else
        {
            fprintf( out, "%u ", x->data.timer.timer_ID );
            MSC_AtsOutTimer( out, M, x->data.timer.timing );
        };

        break;
    };

    case MSC_EV_TIMER_RESET:
    {
        if ( x->data.timer.timing.type != MSC_SPECIAL_TIMER )
        {
            fprintf( out, "\"%s\" %u", MSC_GetStr( x->data.timer.signal_Name, M ), x->data.timer.timer_ID );
        }
        else
        {
            fprintf( out, "%u ", x->data.timer.timer_ID );
            MSC_AtsOutTimer( out, M, x->data.timer.timing );
        };

        break;
    };

    case MSC_EV_TIMER_TIMEOUT:
    {
        if ( x->data.timer.timing.type != MSC_SPECIAL_TIMER )
        {
            fprintf( out, "\"%s\" %u %s \"%s\"",
                MSC_GetStr( x->data.timer.signal_Name, M ),
                x->data.timer.timer_ID,
                ( x->data.timer.relation->data.timer.timing.type ==
                    MSC_ABSOLUTE_TIMER ) ? "ABS" : "REL",
                MSC_GetStr( x->data.timer.relation->data.timer.timing.str_MaxTime, M ) );
        }
        else
        {
            fprintf( out, "%u ", x->data.timer.timer_ID );
            MSC_AtsOutTimer( out, M, x->data.timer.timing );
        };

        break;
    };
    };

    fputs( "] ", out );

}

/***************************************************************************
 * Function : MSC_AtsGenVerdict
 *
 * Description: generate verdict status for event
 *
 * Returns: RESULT_OK/RESULT_ERR
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_AtsGenVerdict(FILE * out, MSC_STORAGE *M, MSC_EVENT_KIND * x)
{
    if (x->event_Verdict == MSC_VERDICT_PASS )
        fputs(" PASS ", out);
    else
        if (x->event_Verdict == MSC_VERDICT_FAIL )
            fputs(" FAIL ", out);
        else
            if (x->event_Verdict == MSC_VERDICT_INCONCLUSIVE )
                fputs(" INC ", out);
    return RESULT_OK;
}

/***************************************************************************
* Function : MSC_AtsGenerateCommand
*
* Description: Generate full command.
*              <command>::=list <LBRASE><keyword><RBRASE>
*                         <time><cond><instances_applied><line><comment><data> ;
* Returns: None
*
* Notes: None
*
**************************************************************************/
static void tat_AtsGenerateCommand( MSC_STORAGE * M, MSC_EVENT_KIND * x, FILE * out, size_t instance_id )
{
    char                    dest[TAT_MAX_LINE_LEN];
    size_t                  i;

    fprintf( out, "list \"%s\" ", MSC_AtsGenKeyword( x ) );

    if ( x && x->type != MSC_EV_EMPTY )
    {
        /* output required time constraints */
        fputs("[list ", out);
        MSC_AtsOutTime( out, M, x->time );
        MSC_AtsOutTime( out, M, x->time_Calc );
        fputs("] ", out);

        /* output offered time constraints */
        fputs("[list ] ", out);

        /* output prohibited time constraints */
        fputs("[list ] ", out);


        /*        MSC_AtsGenTime( out, M, x ); */

        /* output condition */
        StrEmbedLineQuotes( dest, MSC_GetStr( x->condition, M ) );
        fputs( dest, out );

        /* output instances */
        fprintf(out, " %d ", instance_id);

/*        MSC_AtsGenInstances( out, M, x ); */

        /* output verdict specification */
        MSC_AtsGenVerdict(out, M, x);

        /* output source-reference */
        fprintf( out, " [list " );
        for ( i = 0; i < x->src_Count; i++ )
        {
            fprintf( out, "[list %u %u ] ", x->src_Data[i].link_Index, x->src_Data[i].line_Number );
        }
        fputs( "] ", out );

        /* output comment */
        StrEmbedLineQuotes( dest, MSC_GetStr( x->comment, M ) );
        fputs( dest, out );

        /* output time-label setting */
        fprintf( out, " \"%s\"", ( x->labeled ) ? MSC_GetStr( M->labels[x->labeled - 1].label, M ) : "" );

        /* output event data */
        MSC_AtsGenData( out, M, x );
    }
}

/***************************************************************************
 * Function : MSC_AtsGenerateCommand
 *
 * Description: Generate full command.
 *              <command>::=list <LBRASE><keyword><RBRASE>
 *                         <time><cond><instances_applied><line><comment><data> ;
 * Returns: None
 *
 * Notes: None
 *
 **************************************************************************/
void MSC_AtsGenerateCommand( MSC_STORAGE * M, MSC_EVENT_KIND * x, FILE * out )
{
    fprintf(out, "list %d ", MSC_GetEventID(x));
}

static int tat_OutputBranch( FILE * Out, MSC_STORAGE * M, struct MSC_TEST_CASENODE *x, size_t settag )
{
    size_t  i, passive = 0, timers = 0, logicals = 0, active;

    if ( !x || !x->next )
        return RESULT_OK;

/* count types of events */
    for ( i = 0; i < x->next_Count; i++ )
    {
        if ( x->next[i] && MSC_IsPassiveEvent( x->next[i]->E ) )
        {
            ( x->next[i]->E->type == MSC_EV_CONDITION ) ? logicals++ : passive++;
        }

        if ( x->next[i] && x->next[i]->E && x->next[i]->E->type == MSC_EV_TIMER_TIMEOUT )
            timers++;
    };

    active = x->next_Count - logicals - passive - timers;

    /* branch case */
    fprintf( Out, " %u %u %u %u %u\\\n", logicals, timers, passive, active, x->brc );

    if ( logicals )
    {
        for ( i = 0; i < x->next_Count; i++ )
        {
            if ( x->next[i] && MSC_IsPassiveEvent( x->next[i]->E ) &&
                ( x->next[i]->E->type == MSC_EV_CONDITION ) )
            {
                fputs( "[", Out );
                MSC_AtsGenerateCommand( M, x->next[i]->E, Out );
                /* output tag label of next event */
                fprintf( Out, " %d [ list ", x->next[i]->tag);

                /* output next branch */
                tat_OutputBranch( Out, M, x->next[i], x->tag );

                fprintf( Out, "] ]\\\n", i );  /* output case number */
            }
        }
    }

    if ( timers )
    {
        for ( i = 0; i < x->next_Count; i++ )
        {
            if ( x->next[i] && x->next[i]->E->type == MSC_EV_TIMER_TIMEOUT )
            {
                fputs( "[", Out );
                MSC_AtsGenerateCommand( M, x->next[i]->E, Out );
                /* output tag label of next event */
                fprintf( Out, " %d [ list ", x->next[i]->tag);

                /* output next branch */
                tat_OutputBranch( Out, M, x->next[i], x->tag );

                fprintf( Out, " ] ]\\\n", i );  /* output case number */
            }
        }
    }

    if ( passive )
    {
        for ( i = 0; i < x->next_Count; i++ )
        {
            if ( x->next[i] && MSC_IsPassiveEvent( x->next[i]->E ) &&
                ( x->next[i]->E->type != MSC_EV_CONDITION ) )
            {
                fputs( "[", Out );
                MSC_AtsGenerateCommand( M, x->next[i]->E, Out );
                /* output tag label of next event */
                fprintf( Out, " %d [ list ", x->next[i]->tag);

                /* output next branch */
                tat_OutputBranch( Out, M, x->next[i], x->tag );

                fprintf( Out, " ] ]\\\n", i );  /* output case number */
            }
        }
    }

    if ( active )
    {
        for ( i = 0; i < x->next_Count; i++ )
        {
            if ( ( x->next[i] && !MSC_IsPassiveEvent( x->next[i]->E ) &&
                !MSC_IsTimerEvent( x->next[i]->E ) ) || ( !x->next[i] ) )
            {
                fputs( "[", Out );
                MSC_AtsGenerateCommand( M, x->next[i]->E, Out );
                /* output tag label of next event */
                fprintf( Out, " %d [ list ", x->next[i]->tag);

                /* output next branch */
                tat_OutputBranch( Out, M, x->next[i], x->tag );

                fprintf( Out, " ] ]\\\n", i );  /* output case number */
            }
        }
    }

    return RESULT_OK;
}

static size_t             tat_AtsBranchCount;

static LOGICAL tat_AtsCountBranchTree( struct MSC_TEST_CASENODE *x )
{
    size_t                    i, k;

    while ( x && x->next_Count )
    {

        if ( x->next_Count == 1 )
            x = x->next[0];
        else
        {
            k = 0;

            for ( i = 0; i < x->next_Count; i++ )
            {
                if ( ( x->next[i] && ( ( x->next[i]->E && !MSC_IsPassiveEvent( x->next[i]->E )
                                && !MSC_IsTimerEvent( x->next[i]->E ) )
                            || ( !x->next[i]->E ) ) ) || !x->next[i] )
                    k++;
            };

            if ( k > 1 )
                x->brc = tat_AtsBranchCount++;

            for ( i = 0; i < x->next_Count; i++ )
            {
                tat_AtsCountBranchTree( x->next[i] );
            };

            break;
        }
    };

    return RESULT_OK;
}

static size_t tat_AtsCountBranchSuperTree( struct MSC_TEST_CASE *X )
{
    size_t                    i;

    while ( X )
    {
        tat_AtsCountBranchTree( X->toller );

        for ( i = 0; i < X->branch_Count; i++ )
        {
            tat_AtsCountBranchSuperTree( X->branch[i] );
        };

        X = X->next;
    };

    return tat_AtsBranchCount;
}

static LOGICAL MSC_AtsGentest_( FILE * Out, MSC_STORAGE * M, struct MSC_TEST_CASE *X )
{
    size_t                    i;

    if ( !M )
        return RESULT_ERR;

    if ( !Out )
        return RESULT_ERR;

    while ( X )
    {

        fprintf( Out, "EventTree [ list ");
        tat_OutputBranch( Out, M, X->toller, 0 );
        fprintf( Out, "]\n");

        for ( i = 0; i < X->branch_Count; i++ )
        {
            fprintf( Out, "IfTag %u\n", X->tags[i] );
            MSC_AtsGentest_( Out, M, X->branch[i] );
            fprintf( Out, "EndTag\n" );
        };

        X = X->next;
    }

    return RESULT_OK;
}

static size_t tat_AtsTimerCount;
static size_t tat_AtsLoopCount;

static LOGICAL tat_AtsCountTimerEvent( MSC_STORAGE * M, MSC_EVENT_KIND * x )
{
    if ( x->type == MSC_EV_TIMER_SET )
        tat_AtsTimerCount++;

    if ( x->type == MSC_EV_START_LOOP )
    {
        x->data.loop.id = tat_AtsLoopCount;
        tat_AtsLoopCount++;
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_AtsCountRepeatitions
 *
 * Description: count number of efficient paths through linear part
 *
 * Returns: number of paths
 *
 * Notes: None
 *
 **************************************************************************/
static size_t tat_AtsCountRepeatitions( struct MSC_TEST_CASENODE *x )
{
    size_t c, i;

    if ( !x )
        return 0;

    if ( !x->next )
        return 0;

    c = 0;

    for ( i = 0; i < x->next_Count; i++ )
        if ( ( x->next[i] && ( ( x->next[i]->E &&
                        !MSC_IsPassiveEvent( x->next[i]->E ) && !MSC_IsTimerEvent( x->next[i]->E ) ) ||
                    !x->next[i]->E ) ) || !x->next[i] )
            c++;

    if ( c == 1 )
        c = 0;

    for ( i = 0; i < x->next_Count; i++ )
        c += tat_AtsCountRepeatitions( x->next[i] );

    return c;
}

/***************************************************************************
 * Function : tat_AtsCountMaxRepeatitions
 *
 * Description: count maximal number of paths through linear part
 *
 * Returns: number of paths
 *
 * Notes: None
 *
 **************************************************************************/
static size_t tat_AtsCountMaxRepeatitions( struct MSC_TEST_CASENODE *x )
{
    size_t                    c, i;

    if ( !x )
        return 0;

    if ( !x->next )
        return 0;

    c = x->next_Count;

    if ( c == 1 )
        c = 0;

    for ( i = 0; i < x->next_Count; i++ )
        c += tat_AtsCountMaxRepeatitions( x->next[i] );

    return c;
}

/***************************************************************************
 * Function : tat_AtsCountTestPaths
 *
 * Description: count number of paths (max or efficient)
 *
 * Returns: number of paths through test case
 *
 * Notes: None
 *
 **************************************************************************/
static size_t tat_AtsCountTestPaths( struct MSC_TEST_CASE *X )
{
    size_t                    c, i;

    if ( !X )
        return 0;

    c = 0;

    for ( i = 0; i < X->branch_Count; i++ )
        c += tat_AtsCountTestPaths( X->branch[i] );

    c += ( MSC_CountMaxPath ) ? tat_AtsCountMaxRepeatitions( X->toller ) : tat_AtsCountRepeatitions( X->
        toller );

    c += tat_AtsCountTestPaths( X->next );

    return c;
}

/***************************************************************************
 * Function : MSC_AtsGenerateSigDef
 *
 * Description: Output string table of messages in MSC(s)
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_AtsGenerateSigDef( FILE * Out )
{
    size_t                    i;

    fprintf( Out, "SignalTable [ list \\\n" );

    for ( i = 0; i < MSC_UniqMessCountATS; i++ )
        fprintf( Out, "\"%s(%s)\"\\\n", MSC_UniqMessStorage[i].signame,
            ( MSC_UniqMessStorage[i].params ) ? MSC_UniqMessStorage[i].params : "" );

    fprintf( Out, "]\n" );

    return RESULT_OK;
}


MSC_ATS_EVENT_LIST * MSC_ATSEvList = NULL;
size_t MSC_ATSEvListCount  = 0;

/***************************************************************************
 * Function : MSC_GetEventID
 *
 * Description: Get unique event ID in test case
 *
 * Returns: number / INFINITE
 *
 * Notes: None
 *
 **************************************************************************/
size_t MSC_GetEventID(MSC_EVENT_KIND *ev)
{
    size_t i;
    for (i=0; i<MSC_ATSEvListCount; i++)
    {
        if (MSC_ATSEvList[i].event == ev)
        {
            return i;
        }
    }

    return MSC_INFINITY_CONST;
}

static LOGICAL tat_GenEventList(MSC_STORAGE * msc, MSC_EVENT_KIND *ev)
{
    if (!ev->deleted)
    {
        MSC_ATSEvList=MEM_AddObject(MSC_ATSEvList, MSC_ATSEvListCount+1, sizeof(MSC_ATSEvList[0]));
        MSC_ATSEvList[MSC_ATSEvListCount].event = ev;
        MSC_ATSEvList[MSC_ATSEvListCount].id = MSC_ATSEvListCount;
        MSC_ATSEvList[MSC_ATSEvListCount].instance_id = MSC_ATSEvListCount;
        MSC_ATSEvListCount++;
    }
    
    return RESULT_OK;
}



/***************************************************************************
 * Function : MSC_AtsGenTestCaseHeader
 *
 * Description: Output test case header
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_AtsGenTestCaseHeader( FILE * Out,
    MSC_STORAGE * M, struct MSC_TEST_CASE * X, size_t br_count, char *prefix, int mode )
{
    size_t                  i, count;
    char                    dest[TAT_MAX_LINE_LEN];
    char                    test_name[TAT_MAX_FNAME_LEN];

    if ( X )
    {
        count = tat_AtsCountTestPaths( X );

        if ( count == 0 )
            count = 1;
    }
    else
        count = 1;

    MSC_SortMess( M );
    FP_SplitPath( MSC_GetStr( M->msc_File, M ), NULL, test_name, NULL );

    fprintf( Out, "\n\nStartTestCase \"%s%s\" %u [ list ", ( prefix ) ? prefix : "", test_name, count );

/* output test case parameters */
    for ( i = 0; i < M->param_Count; i++ )
    {
        fprintf( Out, " [list \"%s\" \"%s\"]", MSC_GetStr( M->params[i].pv_Type, M ),
            MSC_GetStr( M->params[i].pv_Name, M ) );
    };

    fputs( "]\\\n", Out );

/* output time labels */
    fputs( "[ list ", Out );
    for ( i = 1; i < M->label_Count; i++ )  /* skip first label */
    {
        if ( !M->labels[i].unused && M->labels[i].label )
        {
            fprintf( Out, "[list \"%s\" %d ] ", MSC_GetStr( M->labels[i].label, M ), M->labels[i].generated );
        }
    }
    fputs( "]\\\n", Out );

/* output text statements */
    fputs( "[ list ", Out );
    for ( i = 0; i < M->text_Count; i++ )
    {
        StrEmbedLineQuotes( dest, MSC_GetStr( M->texts[i], M ) );
        fprintf( Out, "%s\\\n", dest );
    };
    fputs( "] ", Out );

/* output number of timers and branches */
    tat_AtsTimerCount = 0;
    tat_AtsLoopCount = 0;

    MSC_Process( M, tat_AtsCountTimerEvent );

    fprintf( Out, " %u %u \\\n", tat_AtsTimerCount, br_count );

/* output messages */
    fputs( "[ list \\\n", Out );

    if ( !MSC_SingleMessTable )
    {
        for ( i = 0; i < MSC_UniqMessCountATS; i++ )
        {
            fprintf( Out, "\"%s(%s)\"\\\n", MSC_UniqMessStorage[i].signame,
                ( MSC_UniqMessStorage[i].params ) ? MSC_UniqMessStorage[i].params : "" );
        }
    }

    fputs( "] [ list  ", Out );

/* output variables */
    for ( i = 0; i < M->var_Count; i++ )
    {
        fprintf( Out, " [list \"%s\" \"%s\" %d]", MSC_GetStr( M->vars[i].pv_TypeSrc, M ),
            MSC_GetStr( M->vars[i].pv_Name, M ),
            CFG_GetDataTypeID(MSC_GetStr( M->vars[i].pv_TypeSrc, M )) );
    };

    fputs( "]\\\n[ list  ", Out );
    for ( i = 0; i < M->src_Count; i++ )
    {
        /* copy w/o first quote mark */
        strcpy(test_name, MSC_GetStr( M->src_Files[i].link, M )+1);
        test_name[strlen(test_name)-1]=0;
        StrEmbedLineQuotes( dest, test_name );
        fprintf( Out, "[list %s %u %u %u ]\\\n", dest,
            M->src_Files[i].level, M->src_Files[i].first_Line, M->src_Files[i].last_Line );
    };

    fputs( "]\\\n", Out );

    MSC_ATSEvList = NULL;
    MSC_ATSEvListCount  = 0;

    /* get the unique names */
    MSC_Process(M, tat_GenEventList);

    fputs( "[ list \\\n", Out );
    for (i=0; i<MSC_ATSEvListCount; i++)
    {
        MSC_AtsGenInstances( Out, M, MSC_ATSEvList[i].event );
        fputs("\\\n",Out);
    }
    fputs( "]\\\n", Out );

    fputs( "[ list \\\n", Out );
    for (i=0; i<MSC_ATSEvListCount; i++)
    {
        fputs("[ ",Out);
        tat_AtsGenerateCommand( M, MSC_ATSEvList[i].event,  Out, MSC_ATSEvList[i].instance_id);
        fputs("]\\\n",Out);
    }

    /* output generation mode */
    fprintf( Out, "] %d %u\n\n", mode, tat_AtsLoopCount);

    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_AtsGenTestCaseFooter
 *
 * Description: Generate test case footer
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_AtsGenTestCaseFooter( FILE * Out, MSC_STORAGE * M, char *prefix )
{
    char                    test_name[TAT_MAX_FNAME_LEN];

    FP_SplitPath( MSC_GetStr( M->msc_File, M ), NULL, test_name, NULL );
    fprintf( Out, "FinishTestCase \"%s%s\"\n", ( prefix ) ? prefix : "", test_name );
    /* free reserved memory */
    MEM_Free(MSC_ATSEvList);
    MSC_ATSEvList = NULL;
    MSC_ATSEvListCount = 0;
    return MSC_FreeSigMap(  );
}

/***************************************************************************
 * Function : MSC_AtsGenTest
 *
 * Description: Generate active test case
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_AtsGenTest( FILE * Out, MSC_STORAGE * M, struct MSC_TEST_CASE * X, char *prefix )
{
    LOGICAL                 res;

    tat_AtsBranchCount = 0;
    MSC_AtsGenTestCaseHeader( Out, M, X, tat_AtsCountBranchSuperTree( X ), prefix, 0 );
    res = MSC_AtsGentest_( Out, M, X );
    MSC_AtsGenTestCaseFooter( Out, M, prefix );
    return res;
}

/***************************************************************************
 * Function : tat_AtsGenTestDirect
 *
 * Description: generate MSC-Direct form of ATS
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_AtsGenTestDirect( FILE * Out, MSC_STORAGE * M, MSC_EVENT_KIND * E, int branch )
{
    size_t                    i, passive, tmr, nxt;

    if ( !E )
        return RESULT_OK;

    while ( E )
    {
        passive = 0;
        tmr = 0;
        nxt = 1;
    /* count number of alternatives */

        if ( E->type != MSC_EV_OPERATOR )
        {
            if ( E->next && E->next->type == MSC_EV_RECV_SIGNAL )
                passive++;

            if ( E->next && E->next->type == MSC_EV_TIMER_TIMEOUT )
                tmr++;
        }
        else if ( E->data.alt.type != MSC_OP_PAR )
        {
            for ( i = 0; i < E->data.alt.frame_Count; i++ )
            {
                if ( E->data.alt.frame[i] )
                {
                    if ( E->data.alt.frame[i]->type == MSC_EV_RECV_SIGNAL )
                        passive++;

                    if ( E->data.alt.frame[i]->type == MSC_EV_TIMER_TIMEOUT )
                        tmr++;
                }
            };

            nxt = E->data.alt.frame_Count;

            if ( E->data.alt.type == MSC_OP_EXC || E->data.alt.type == MSC_OP_OPT )
                nxt++;
        };

        if ( E->type != MSC_EV_OPERATOR )
        {
        /* printevent(Out, M, E, branch); */
        /* single command case */
            fputs( "Operation [", Out );
            MSC_AtsGenerateCommand( M, E, Out );
            fputs( "0 ]\n", Out );  /* 0 is default case for single command */
        }

        if ( E->type == MSC_EV_OPERATOR )
        {
        /* output prolog */

            if ( E->data.alt.type == MSC_OP_PAR ||
                E->data.alt.type == MSC_OP_ALT || E->data.alt.type == MSC_OP_OPT ||
                E->data.alt.type == MSC_OP_EXC )
            {
                fputs( "Operation [", Out );
                MSC_AtsGenerateCommand( M, E, Out );
                fputs( "0 ]\n", Out );  /* 0 is default case for single command */
            }

            for ( i = 0; i < E->data.alt.frame_Count; i++ )
            {

                if ( E->data.alt.type == MSC_OP_PAR )
                {
                    fprintf( Out, "StartThread %u\n", i );
                }

                if ( E->data.alt.type == MSC_OP_ALT )
                {
                    fprintf( Out, "StartCase %u\n", i );
                }


                tat_AtsGenTestDirect( Out, M, E->data.alt.frame[i], 0 );


                if ( E->data.alt.type == MSC_OP_PAR )
                {
                    fprintf( Out, "FinishThread %u\n", i );
                }

                if ( E->data.alt.type == MSC_OP_ALT )
                {
                    fprintf( Out, "FinishCase %u\n", i );
                }

            }

        /* output epilog */

            if ( E->data.alt.type == MSC_OP_PAR ||
                E->data.alt.type == MSC_OP_ALT || E->data.alt.type == MSC_OP_OPT ||
                E->data.alt.type == MSC_OP_EXC )
            {
                E->data.alt.flag_End = 1;
                fputs( "Operation [", Out );
                MSC_AtsGenerateCommand( M, E, Out );
                fputs( "0 ]\n", Out );  /* 0 is default case for single command */
                E->data.alt.flag_End = 0;
            }

        };

        E = E->next;

        branch = 0;
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_AtsGenTestReduced
 *
 * Description: Generate flat test case
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_AtsGenTestReduced( FILE * Out, MSC_STORAGE * M, char *prefix )
{
    LOGICAL                 res;

    MSC_AtsGenTestCaseHeader( Out, M, NULL, 0, prefix, 2 );

    res = tat_AtsGenTestDirect( Out, M, M->start, 0 );

    MSC_AtsGenTestCaseFooter( Out, M, prefix );

    return res;
}
