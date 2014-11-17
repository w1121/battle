/******************************************************************************
*
*       COPYRIGHT 2001-2004 MOTOROLA, ALL RIGHTS RESERVED
*
*       MOTOROLA CONFIDENTIAL PROPRIETARY
*
* Description:  Command-line processing
*
* $RCSfile: loadmsc.c,v $
* $Revision: 1.4 $
* $Source: /home/cvsusr/cvssoft/tat_tools/baselib/src/loadmsc.c,v $
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
#include <mscparse.h>
#include <mscproc.h>
#include <qmemory.h>
#include <fileproc.h>
#include <strproc.h>
#include <outmess.h>
#include <mscmess.h>
#include <mscstrs.h>
#include <tatgdata.h>

int MSC_DoNotParseText = 0;

char *  VRS_DiagramVerdictText[] = {
        "CONSISTENT DIAGRAM",
        "TIME INCONSISTENCY",
        "ANNOTATION INCORRECTNESS",
        "ANNOTATION CORRECTNESS",
        "INCOMPLETE PROCESSING"
};

#define MSC_MSC_VERDICT_COUNT sizeof(VRS_DiagramVerdictText)/ sizeof(VRS_DiagramVerdictText[0])


/* this enum encompasses states of MSC line parser */
typedef enum _MSC_LOAD_LINE_STATE
{
    MSC_START_LINE,
    MSC_GENERAL_LINE,
    MSC_QUOTE_LINE,
    MSC_SINGLE_QUOTE_LINE,
    MSC_C_COMMENT_LINE,
    MSC_C_COMMENT_START_LINE,
}
MSC_LOAD_LINE_STATE;

/***************************************************************************
 * Function : msc_LoadLine
 *
 * Description: move pointer to start of next line
 *
 * Return: NULL or pointer to current line
 *
 * Notes: none
 *
 **************************************************************************/
static char * msc_LoadLine(  MSC_LOAD_DATA * data )
{
    char                   *line;
    int                     statement_end, statement_start=0;
    size_t                  char_count = 0, parenthesis=0;
    MSC_LOAD_LINE_STATE     state;

    if ( !data->current_Position || !( *data->current_Position ) )
    {
        return NULL;
    }

    line = data->current_Position;
    state = MSC_START_LINE;
    statement_end = 0;
    while ( *data->current_Position && !statement_end )
    {
        char_count++;

        /* handle memory overwriting */
        if ( char_count >= TAT_MAX_LINE_LEN )
        {
            MSG_SyntaxError(  );
            data->error_State = RESULT_ERR;
            return NULL;
        }

        switch ( state )
        {
        case MSC_START_LINE:
        {
            switch ( *data->current_Position )
            {
                /* handle <NEWLINE> literals */
            case '\n':
            case '\r':
            {
                data->line_Counter++;

                if ( ( *( data->current_Position + 1 ) == '\n' ||
                        *( data->current_Position + 1 ) == '\r' ) &&
                    *( data->current_Position + 1 ) != *data->current_Position )
                    data->current_Position++;

                data->current_Position++;
            /* skip double-character line end */
                MSG_SetLineNumber( data->line_Counter );
                break;
            }
            /* handle <SPACE> literals in global space */
            case ' ':
            case '\t':
            {
                while ( *data->current_Position == ' ' || *data->current_Position == '\t' )
                {
                    data->current_Position++;
                }

                break;
            }
            case '/':
            {
                if ( *( data->current_Position + 1 ) == '*' )
                {
                    state = MSC_C_COMMENT_START_LINE;
                    data->current_Position += 2;
                }
                else
                {
                    state = MSC_GENERAL_LINE;
                    /* update start of MSC-PR statement */
                    line = data->current_Position;
                    char_count = 0;
                    break;
                }
                break;
            }
            /* if non-special term found, start parsing of general msc statements */
            default:
            {
                state = MSC_GENERAL_LINE;
                line = data->current_Position;
                char_count = 0;
                break;
            }
            }
            break;
        }
        case MSC_GENERAL_LINE:
        {
            statement_start = 1;
            switch ( *data->current_Position )
            {
            case ';':          /* msc event finish */
            {
                data->current_Position++;
                if (parenthesis==0)
                {
                    statement_end = 1;
                }
                break;
            }
            case '\n':
            case '\r':
            {
                data->line_Counter++;

            /* skip double-character line end */
                if ( ( *( data->current_Position + 1 ) == '\n' ||
                        *( data->current_Position + 1 ) == '\r' ) &&
                    *( data->current_Position + 1 ) != *data->current_Position )
                    data->current_Position++;

                data->current_Position++;
                break;
            }
            case ' ':
            case '\t':
            {
                /* optimize loop to skip spaces */
                while ( *data->current_Position == ' ' || *data->current_Position == '\t' )
                    data->current_Position++;
                break;
            }
            case '\'':
            {
                state = MSC_SINGLE_QUOTE_LINE;
                data->current_Position++;
                break;
            }
            case '\"':
            {
                state = MSC_QUOTE_LINE;
                data->current_Position++;
                break;
            }
            case '/':
            {
                data->current_Position++;
                if ( *data->current_Position == '*' )
                {
                    state = MSC_C_COMMENT_LINE;
                    char_count++;
                    data->current_Position++;
                }
                break;
            }
            default:
            {
                /* process usual characters */
                if (*data->current_Position=='(')
                    parenthesis++;

                if (*data->current_Position==')')
                    parenthesis--;

                data->current_Position++;

                break;
            }
            }                   /* switch on symbol */
            break;
        }                       /* case MSC_GENERAL_LINE */
        case MSC_QUOTE_LINE:
        {
            switch ( *data->current_Position )
            {
            case '\n':
            case '\r':
            {
                data->line_Counter++;
            /* skip double-character line end */
                if ( ( *( data->current_Position + 1 ) == '\n' ||
                        *( data->current_Position + 1 ) == '\r' ) &&
                    *( data->current_Position + 1 ) != *data->current_Position )
                    data->current_Position++;

                data->current_Position++;
                break;
            }
            case '\"':
            {
                data->current_Position++;
                if ( *data->current_Position == '\"' )
                {
                    data->current_Position++;
                }
                else
                {
                    state = MSC_GENERAL_LINE;
                }
                break;
            }
            default:
            {
                data->current_Position++;
                break;
            }
            }                   /* switch on symbol */
            break;
        }                       /* case MSC_QUOTE_LINE */
        case MSC_SINGLE_QUOTE_LINE:
        {
            switch ( *data->current_Position )
            {
            case '\n':
            case '\r':
            {
                data->line_Counter++;
            /* skip double-character line end */
                if ( ( *( data->current_Position + 1 ) == '\n' ||
                        *( data->current_Position + 1 ) == '\r' ) &&
                    *( data->current_Position + 1 ) != *data->current_Position )
                    data->current_Position++;

                data->current_Position++;
                break;
            }
            case '\'':
            {
                data->current_Position++;
                if ( *data->current_Position == '\'' )
                {
                    data->current_Position++;
                }
                else
                {
                    state = MSC_GENERAL_LINE;
                }
                break;
            }
            default:
            {
                data->current_Position++;
                break;
            }
            }                   /* switch on symbol */
            break;
        }                       /* case MSC_SINGLE_QUOTE_LINE */
        case MSC_C_COMMENT_LINE:
        {
            switch ( *data->current_Position )
            {
            case '\n':
            case '\r':
            {
                data->line_Counter++;
            /* skip double-character line end */
                if ( ( *( data->current_Position + 1 ) == '\n' ||
                        *( data->current_Position + 1 ) == '\r' ) &&
                    *( data->current_Position + 1 ) != *data->current_Position )
                    data->current_Position++;
                data->current_Position++;
                break;
            }
            case '*':
            {
                data->current_Position++;
                if ( *data->current_Position == '/' )
                {
                    data->current_Position++;
                    state = MSC_GENERAL_LINE;
                }
                break;
            }
            default:
            {
                data->current_Position++;
                break;
            }
            }                   /* switch on symbol */
            break;
        }                       /* case MSC_C_COMMENT_LINE */
        case MSC_C_COMMENT_START_LINE:
        {
            switch ( *data->current_Position )
            {
            case '\n':
            case '\r':
            {
                data->line_Counter++;

            /* skip double-character line end */
                if ( ( *( data->current_Position + 1 ) == '\n' ||
                        *( data->current_Position + 1 ) == '\r' ) &&
                    *( data->current_Position + 1 ) != *data->current_Position )
                    data->current_Position++;

                data->current_Position++;
                MSG_SetLineNumber( data->line_Counter );
                break;
            }
            case '*':
            {
                *data->current_Position++;
                if ( *data->current_Position == '/' )
                {
                    *data->current_Position++;
                    state = MSC_START_LINE;
                }
                break;
            }
            default:
            {
                data->current_Position++;
                break;
            }
            }                   /* switch on symbol */
            break;
        }                       /* case MSC_C_COMMENT_START_LINE */
        }                       /* switch(state) */

    /* handle empty msc lines */
        if ( statement_end && (line+1 == data->current_Position ))
        {
            statement_end = 0;
            state = MSC_START_LINE;
        }
    }                           /* until we can read */

    if ( !*data->current_Position && !statement_end && statement_start)
    {
        MSG_SyntaxError(  );
        data->error_State = RESULT_ERR;
        return NULL;
    }
    return line;
}

/***************************************************************************
 * Function : msc_GetSigNum
 *
 * Description: get unique signal number
 *
 * Return: none
 *
 * Notes:
 *
 **************************************************************************/
static size_t msc_GetSigNum( const char * s, MSC_LOAD_DATA * data )
{
    size_t                  i, open_sig;

    i = data->signal_Counter;
    open_sig = data->open_Signals;

    /* backward search to improve performance */
    while ( i && open_sig)
    {
        i--;
    /* if signal found return its number > 0 */
        if ( !strcmp( data->signal_Storage[i].message_Label, s ) )
            return i + 1;

        /* if this is inconsistent message, decrease number of looked-through inc. messages */
        if (data->signal_Storage[i].count!=0)
        {
            open_sig--;
        }
    }

    data->signal_Storage =
        MEM_AddObject( data->signal_Storage, ( data->signal_Counter + 1 ), sizeof ( data->signal_Storage[0] ) );

    data->signal_Storage[data->signal_Counter].message_Label = MEM_CopyStr( s );
    /* set up initially created signal */
    data->signal_Storage[data->signal_Counter].count = 0x80000000;

    /* save position of first definition */
    data->signal_Storage[data->signal_Counter].line_Number = data->line_Counter;
    return ++data->signal_Counter;
}

/***************************************************************************
 * Function : msc_AddVarLink
 *
 * Description: Add link to variable
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL msc_AddVarLink( char * name, MSC_LOAD_DATA * data )
{
    size_t    i;

    for ( i = 0; i < data->variable_Counter; i++ )
    {
        if ( !strcmp( name, data->var_Storage[i].var_Name ) )
            return RESULT_OK;
    };
/* allocate memory and save link */
    data->var_Storage = MEM_AddObject( data->var_Storage, i + 1, sizeof ( data->var_Storage[0] ) );
    data->var_Storage[i].var_Name = MEM_CopyStr( name );
    data->var_Storage[i].line_Number = data->line_Counter;
    data->variable_Counter++;
    return RESULT_OK;
}

/***************************************************************************
 * Function : msc_ParseSignal
 *
 * Description: extract signal information: condition, time, name, duration, parameters
 *
 * Return: none
 *
 * Notes:
 *
 **************************************************************************/
static MSC_MESSAGE_STORAGE *msc_ParseSignal( char * * t,
    MSC_STORAGE * M, MSC_MESSAGE_STORAGE ** prev, MSC_LOAD_DATA * data )
{
    MSC_MESSAGE_STORAGE     x, *p;
    char                   *orig, *s, *tm;
    char                    name[TAT_MAX_ID_LEN];
    char                    params[TAT_MAX_LINE_LEN];
    size_t                  param_Count, i, j;
    size_t                  ind;
    char                    **P, **messPar;

    s = *t;
    orig = s;
/* get signal name */
    i = 0;

    if ( !StrExtractMacroID( &s, name ) || name[0] == 0 )
    {
        MSG_SyntaxError(  );
        return NULL;
    }

    x.delay.type = MSC_RELATIVE_TIME;
    x.delay.str_MinTime = STRCOLL_STR_ID_0;
    x.delay.str_MaxTime = STRCOLL_STR_ID_0;
    x.delay.min_Time = 0.0;
    x.delay.max_Time = 0.0;

/* signal name mustn't be reserved name */
    if ( !MSC_CheckName( name ) )
    {
        MSG_SemanticError( MSG_RESERVED_NAME );
        return NULL;
    }

    STR_SKIP_SPACES( s );

    if ( *s == '/' )
    {
        if ( !StrParseCComment( &s, params ) )
        {
            MSG_SyntaxError(  );
            return NULL;
        }

        tm = params;
        STR_SKIP_SPACES( tm );

        if ( *tm == '{' )
        {
            if ( !MSC_ParseTime( &tm, M, &x.delay ) )
                return NULL;
        };

        STR_SKIP_SPACES( s );
    }
    else
    /* get signal duration */
    if ( *s == '{' )
    {
        if ( !MSC_ParseTime( &s, M, &x.delay ) )
            return NULL;

        StrSkipComments( &s );
    }

    if ( x.delay.type != MSC_RELATIVE_TIME )
    {
        MSG_SemanticError( MSG_INCORRECT_DUR_SPEC );
        return NULL;
    }

    if ( *s != ',' )
    {
        /* if no label is specified for message, set the label as message name */
        strcpy(params, name);
        x.source_ID = 0;
    }
    else
    {
        s++;
        StrSkipComments( &s );

        if ( !StrExtractLabel( &s, params ) || params[0] == 0 )
        {
            MSG_SemanticError( MSG_MESS_EXCHNUM_EXP );
            return NULL;
        }

        /* save original message label */
        x.source_ID = MSC_AddStr( params, M );
    }

    ind = msc_GetSigNum( params, data );

    if ( data->signal_Storage[ind - 1].count == 0 )
    {
    /* duplicated signal use */
        MSG_SemanticError( MSG_MESS_LABEL_DUPLICATED );
        return NULL;
    }

    if ( data->signal_Storage[ind - 1].count == 0x80000000 )
    {
        data->signal_Storage[ind - 1].count = 0;
    }

    StrSkipComments( &s );

    p = MSC_GetMessageParams( ind, M );

    *prev = p;

    if ( p && ( stricmp( MSC_GetString( p->message_Name, M ), name ) ||
            p->delay.str_MinTime != x.delay.str_MinTime || p->delay.str_MaxTime != x.delay.str_MaxTime ) )
    {
        MSG_SemanticError( MSG_INCONSISTENT_MESSAGE );
        return NULL;
    }

    P = NULL;

    if ( *s != '(' )
        param_Count = 0;
    else
    {
        if (!MSC_ParseParams( &s, &param_Count, &P ))
        {
            MSG_SyntaxError(  );
            return NULL;
        }
    }
    *t=s;

    if ( p && param_Count != p->param_Count )
    {
        MSG_SemanticError( MSG_INCONSISTENT_MESSAGE );
        MEM_FreeStrArray( param_Count, P );
        return NULL;
    }


    if ( p )
    {
        for ( i = 0; i < param_Count; i++ )
        {
            if ( ( P[i] && p->message_Params[i] && strcmp( P[i], MSC_GetString( p->message_Params[i], M ) ) )
                || ( P[i] && !p->message_Params[i] ) || ( !P[i] && p->message_Params[i] ) )
            {
                MSG_SemanticError( MSG_INCONSISTENT_MESSAGE );
                MEM_FreeStrArray( param_Count, P );
                return NULL;
            }

        }
        MEM_FreeStrArray( param_Count, P );
        return p;
    }
    
    x.indicator = ind;
    x.message_Name = MSC_AddStr( name, M );
    x.param_Count = param_Count;
    x.line_Number = data->line_Counter;
    x.params_Values = NULL;
    x.message_Params = NULL;

    if ( param_Count )
    {
        x.message_Params = MEM_Allocate( param_Count * sizeof ( x.message_Params[0] ) );
        x.params_Values = MEM_Allocate( param_Count * sizeof ( x.params_Values[0] ) );

        for ( i = 0; i < param_Count; i++ )
        {
            x.message_Params[i] = MSC_AddStr( P[i], M );

            /* skip processing, if we don't parse text */
            if (!MSC_DoNotParseText)
            {
                s = P[i];

                if (s)
                    StrSkipComments(&s);

                /* check variable/multiparameter syntax */
                if ( s && s[0] == '&' && s[1]!='&')
                {
                    s++;
                    STR_SKIP_SPACES(s);
                    if (*s!='{')
                    {
                        /* if it's a label, parse it */
                        if ( !StrExtractLabel( &s, params ) || params[0] == 0 )
                        {
                            MSG_SyntaxError(  );
                            goto ParamExit;
                        }
                        /* save parsed MSC label */
                        x.params_Values[i].varName = MSC_AddStr(params, M);
                        /* save label name for further check */
                        msc_AddVarLink( params, data );
                        StrSkipComments( &s );
                        if (*s && *s!=':')
                        {
                            MSG_SyntaxError(  );
                            goto ParamExit;
                        }
                        if (*s) 
                        {
                            s++;
                            STR_SKIP_SPACES(s);
                        }
                    }

                    /*  parse multivariant expression */
                    if ( *s=='{' )
                    {
                        if (!MSC_ParseParams(&s, &x.params_Values[i].valueCount, &messPar))
                        {
                            MSG_SyntaxError(  );
                            goto ParamExit;
                        }
                        x.params_Values[i].values = MEM_Allocate(sizeof(x.params_Values[i].values)*x.params_Values[i].valueCount);
                        for (j=0; j<x.params_Values[i].valueCount; j++)
                        {
                            if (!messPar[j])
                            {
                                MSG_SyntaxError(  );
                                goto ParamExit;
                            }
                            x.params_Values[i].values[j]=MSC_AddStr(messPar[j], M);
                            MEM_Free(messPar[j]);
                        }
                        MEM_Free(messPar);
                    }
                    StrSkipComments(&s);
                    if (*s)
                    {
                        MSG_SyntaxError(  );
                        goto ParamExit;
                    }
                } /* *tm =='&' */
                else
                {
                    /* this is single parameter */
                    x.params_Values[i].varName = 0;
                    x.params_Values[i].valueCount = 1;
                    x.params_Values[i].values = MEM_Allocate(sizeof(x.params_Values[i].values[0]));
                    x.params_Values[i].values[0] = x.message_Params[i];
                }
            } /* !MSC_DoNotParseText */

            if ( P[i] )
                MEM_Free( P[i] );
        }

        MEM_Free( P );
    }

    return MSC_AddMessage( &x, M );

  ParamExit:
    while ( i < param_Count )
    {
        if ( P[i] )
            MEM_Free( P[i] );
        i++;
    }
    MEM_Free( P );
    return NULL;

}

/***************************************************************************
 * Function : msc_ParseMessage
 *
 * Description: parse full message including signal information, comments, instances
 *
 * Return: none
 *
 * Notes:
 *
 **************************************************************************/
static LOGICAL msc_ParseMessage( char * line,
    MSC_EVENT_KIND * s, MSC_EVENT_KIND_TYPE t, MSC_STORAGE * M, MSC_LOAD_DATA * data )
{
    char                   *p;
    MSC_MESSAGE_STORAGE    *m, *prev;
    int                     i;
    char                    temp[TAT_MAX_ID_LEN];

    if ( !line || !s || !M )
        return RESULT_ERR;

    if ( s->inst_Count == 0 )
    {
        MSG_SemanticError( MSG_EVENT_WO_INST );
        return RESULT_ERR;
    }

    p = line;
    m = msc_ParseSignal( &p, M, &prev, data );

    if ( !m )
        return RESULT_ERR;

/* copy identifier */
    s->data.msg.indicator = m->indicator;

/* skip spaces */
    StrSkipComments( &p );

    if ( !StrExtractMacroID( &p, temp ) || temp[0] == 0 )
    {
        MSG_SyntaxError(  );
        return RESULT_ERR;
    }

    StrSkipComments( &p );

    if ( t == MSC_EV_IN_MESSAGE || t == MSC_EV_RECV_SIGNAL )
    {
        /* check for inconsistent message event */
        if ( data->signal_Storage[m->indicator - 1].count < 0 )
        {
            MSG_SetLineNumber( data->signal_Storage[m->indicator - 1].line_Number );
            MSG_SemanticError( MSG_MESS_IN_WO_OUT );
            return RESULT_ERR;
        }

        data->signal_Storage[m->indicator - 1].count--;

        if ( data->signal_Storage[m->indicator - 1].count != 0 )
        {
            data->signal_Storage[m->indicator - 1].time = s->time;
            data->signal_Storage[m->indicator - 1].cond = s->condition;
            data->signal_Storage[m->indicator - 1].tat_Sync = s->sync;

            /* increase number of open message events */
            data->open_Signals++;

        }
        else
        {
            if ( data->signal_Storage[m->indicator - 1].time.str_MinTime != s->time.str_MinTime ||
                data->signal_Storage[m->indicator - 1].time.str_MaxTime != s->time.str_MaxTime ||
                data->signal_Storage[m->indicator - 1].time.type != s->time.type ||
                data->signal_Storage[m->indicator - 1].time.time_Label != s->time.time_Label ||
                data->signal_Storage[m->indicator - 1].tat_Sync != s->sync ||
                data->signal_Storage[m->indicator - 1].cond != s->condition )
            {
                MSG_SemanticError( MSG_INCONSISTENT_MESSAGE );
                return RESULT_ERR;
            }
            /* decrease number of open message events */
            data->open_Signals--;
        }

        if ( stricmp( temp, STR_FROM ) )
        {
            MSG_SyntaxError(  );
            return RESULT_ERR;
        }

    }
    else
    {
        if ( data->signal_Storage[m->indicator - 1].count > 0 )
        {
            MSG_SetLineNumber( data->signal_Storage[m->indicator - 1].line_Number );
            MSG_SemanticError( MSG_MESS_OUT_WO_IN );
            return RESULT_ERR;
        }

        data->signal_Storage[m->indicator - 1].count++;

        if ( data->signal_Storage[m->indicator - 1].count != 0 )
        {
            data->signal_Storage[m->indicator - 1].time = s->time;
            data->signal_Storage[m->indicator - 1].cond = s->condition;
            data->signal_Storage[m->indicator - 1].tat_Sync = s->sync;

            /* increase number of open message events */
            data->open_Signals++;
        }
        else
        {
            if ( data->signal_Storage[m->indicator - 1].time.str_MinTime != s->time.str_MinTime ||
                data->signal_Storage[m->indicator - 1].time.str_MaxTime != s->time.str_MaxTime ||
                data->signal_Storage[m->indicator - 1].time.type != s->time.type ||
                data->signal_Storage[m->indicator - 1].time.time_Label != s->time.time_Label ||
                data->signal_Storage[m->indicator - 1].tat_Sync != s->sync ||
                data->signal_Storage[m->indicator - 1].cond != s->condition )
            {
                MSG_SemanticError( MSG_INCONSISTENT_MESSAGE );
                return RESULT_ERR;
            }
            /* decrease number of open message events */
            data->open_Signals--;
        }

        if ( stricmp( temp, STR_TO ) )
        {
            MSG_SyntaxError(  );
            return RESULT_ERR;
        }

    }

    if ( !StrExtractMacroID( &p, temp ) || temp[0] == 0 )
    {
        MSG_SyntaxError(  );
        return RESULT_ERR;
    }

    if ( !MSC_ParseSourceRef( p, s, M ) )
    {
        return RESULT_ERR;
    }

    i = MSC_GetInstanceID( temp, M );

    if ( ( i == MSC_LOST_INSTANCE_ID && t == MSC_EV_IN_MESSAGE ) ||
        ( i == MSC_FOUND_INSTANCE_ID && t == MSC_EV_OUT_MESSAGE ) )
    {
        MSG_SemanticError( MSG_INCORRECT_FORM );
        return RESULT_ERR;
    }

    if ( i == MSC_UNKNOWN_INSTANCE_ID )
    {
        MSG_SemanticError( MSG_UNDECLARED_INSTANCE );
        return RESULT_ERR;
    }

/* save instance destination */
    s->data.msg.inst = i;

    /* lost/found message events shall be consistent */
    if ( i < 0 )
    {
        data->signal_Storage[m->indicator - 1].count = 0;
        /* decrease number of open message events */
        data->open_Signals--;
    }

    if ( data->signal_Storage[m->indicator - 1].count == 0 && i >= 0 )
    {
    /* check instance consistency */
        if ( data->signal_Storage[m->indicator - 1].inst1 != s->data.msg.inst ||
            ( MSC_INSTANCE_ID_TYPE ) data->signal_Storage[m->indicator - 1].inst2 != s->instances[0] )
        {
            MSG_SemanticError( MSG_INCONSISTENT_MESSAGE );
            return RESULT_ERR;
        }
    }
    else
    {
    /* save instances */
        data->signal_Storage[m->indicator - 1].inst1 = s->instances[0];
        data->signal_Storage[m->indicator - 1].inst2 = s->data.msg.inst;
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : msc_CheckInstanceState
 *
 * Description: 
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: usage: if (!msc_CheckInstanceState(s,data)) return RESULT_ERR;
 *
 **************************************************************************/
static LOGICAL msc_CheckInstanceState( MSC_EVENT_KIND * e, MSC_LOAD_DATA * data )
{
    UINT32_T                    i;

    for ( i = 0; i < e->inst_Count; i++ )
    {
        if ( !data->instance_State[e->instances[i]].instance_Declared )
        {
            MSG_SemanticError( MSG_INST_NOT_DECL );
            return RESULT_ERR;
        }

        if ( data->instance_State[e->instances[i]].instance_Stopped ||
            data->instance_State[e->instances[i]].instance_Ended )
        {
            MSG_SemanticError( MSG_EVENT_USE_STOPPED_INST );
            return RESULT_ERR;
        }
    /* mark instance as used */
        data->instance_State[e->instances[i]].instance_Used = 1;
    }
    return RESULT_OK;
}

/***************************************************************************
 * Function : msc_CheckInOutParity
 *
 * Description: checks in/out events parity
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes:
 *
 **************************************************************************/
static LOGICAL msc_CheckInOutParity( MSC_LOAD_DATA * data )
{
    UINT32_T                    i;

    /* check for signals */
    if (data->open_Signals)
    {
        for ( i = 0; i < data->signal_Counter; i++ )
        {
            if ( data->signal_Storage[i].count > 0 )
            {
                MSG_SetLineNumber( data->signal_Storage[i].line_Number );
                MSG_SemanticError( MSG_MESS_OUT_WO_IN );
                return RESULT_ERR;
            }
            else if ( data->signal_Storage[i].count < 0 )
            {
                MSG_SetLineNumber( data->signal_Storage[i].line_Number );
                MSG_SemanticError( MSG_MESS_IN_WO_OUT );
                return RESULT_ERR;
            }
        }
    }
    return RESULT_OK;
}

/***************************************************************************
 * Function : msc_ParseSourceRefs
 *
 * Description: parse 'msc' statement source references
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL msc_ParseSourceRefs( char * line, MSC_STORAGE * M )
{
    char                    temp[TAT_MAX_LINE_LEN];
    char                    sfile[TAT_MAX_FNAME_LEN];
    int                     sline, fline;
    size_t                  level;

    if (*line == ';')
        return RESULT_OK;

    if ( !( *line == '/' && *( line + 1 ) == '*' ) )
    {
        MSG_SyntaxError(  );
        return RESULT_ERR;
    }

    if ( !StrParseCComment( &line, temp ) )
    {
        MSG_SyntaxError(  );
        return RESULT_ERR;
    }

    StrSkipComments( &line );
    if ( *line!=';' )
    {
        MSG_SyntaxError(  );
        return RESULT_ERR;
    }

    line = temp;
    STR_SKIP_SPACES( line );
    while ( *line )
    {
    /* assume: ["fname",level,start,end]["fname",level,start,end] */
        if ( *line != '[' )
        {
            MSG_SyntaxError(  );
            return RESULT_ERR;
        }
        line++;
        STR_SKIP_SPACES( line );
        if ( *line != '\"' )
        {
            MSG_SyntaxError(  );
            return RESULT_ERR;
        }
        if ( !StrExtractString( &line, '\"', sfile ) )
        {
            MSG_SyntaxError(  );
            return RESULT_ERR;
        }
        STR_SKIP_SPACES( line );
        if ( *line != ',' )
        {
            MSG_SyntaxError(  );
            return RESULT_ERR;
        }
        line++;
        STR_SKIP_SPACES( line );
        level = atoi( line );
        while ( SymIsDigit( *line ) )
            line++;
        STR_SKIP_SPACES( line );
        if ( *line != ',' )
        {
            MSG_SyntaxError(  );
            return RESULT_ERR;
        }
        line++;
        STR_SKIP_SPACES( line );
        sline = atoi( line );
        while ( SymIsDigit( *line ) )
            line++;
        STR_SKIP_SPACES( line );
        if ( *line != ',' )
        {
            MSG_SyntaxError(  );
            return RESULT_ERR;
        }
        line++;
        STR_SKIP_SPACES( line );
        fline = atoi( line );
        while ( SymIsDigit( *line ) )
            line++;
        STR_SKIP_SPACES( line );
        if ( *line != ']' )
        {
            MSG_SyntaxError(  );
            return RESULT_ERR;
        }
        line++;
        STR_SKIP_SPACES( line );
        M->src_Files = MEM_Reallocate( M->src_Files, ( M->src_Count + 1 ) * sizeof ( M->src_Files[0] ) );
        M->src_Files[M->src_Count].link = MSC_AddStr( sfile, M );
        M->src_Files[M->src_Count].level = level;
        M->src_Files[M->src_Count].first_Line = sline;
        M->src_Files[M->src_Count].last_Line = fline;
        M->src_Count++;
        if ( M->src_MscLevel <= level )
            M->src_MscLevel = level + 1;
    }
    return RESULT_OK;
}

/***************************************************************************
 * Function : check_msc
 *
 * Description: open file with MSC, check it for correctness and preload instances
 *
 * Return: none
 *
 * Notes:
 *
 **************************************************************************/
static int msc_PreLoad( const char * filename, MSC_STORAGE * M,
                       MSC_LOAD_DATA * data )
{
    FILE                   *f;

    char *                 buffer;
    char                    temp[TAT_MAX_LINE_LEN];
    
    size_t                  i, end_msc = 0;
    char                   *line, *r, *b;

    if ( !filename || !M )
        return RESULT_ERR;

    f = FP_OpenFile( filename, "r" );
    if ( !f )
    {
        return RESULT_ERR;
    }

    i = FP_FileSize( f );

    if ( !i )
    {
        fclose( f );
        return RESULT_ERR;
    };

    data->source_Buffer = MEM_Allocate( i + 1 );
    fread( data->source_Buffer, i, 1, f );
    data->source_Buffer[i] = 0; /* finish buffer */
    fclose( f );
    data->current_Position = data->source_Buffer;

    MSC_Allocate( filename, M );

    while ( *data->current_Position )
    {
        line = msc_LoadLine( data );
        buffer = line;

        if ( !data->error_State )
        {
            return RESULT_ERR;
        }

        if ( !line )
        {
            break;
        }

        if ( *line == 0 )
            continue;           /* skip blank lines */

        if ( !strnicmp( line, STR_MSCSP, STR_MSCSP_LEN ) )
        {
            line += STR_MSCSP_LEN;
            StrSkipComments( &line );

            if ( !data->mscdocument_Started )
            {
                MSG_SemanticWarning( MSG_MSC_WO_MSCDOCUMENT );
                data->mscdocument_Started = 1;
            }

            if ( data->msc_Started )
            {
                MSG_SemanticError( MSG_MSC_DUPL_DECLARATION );
                return RESULT_ERR;
            }

            if ( !StrExtractFLabel( &line, temp ) || temp[0] == 0 )
            {
                MSG_SyntaxError(  );
                return RESULT_ERR;
            }

        /* if Untitled MSC is loaded, title it :) */
            if ( !stricmp( temp, STR_UNTITLED ) )
            {
                FP_SplitPath( MSC_GetString( M->msc_File, M ), NULL, temp, NULL );
            }

            if ( SymIsDigit( temp[0] ) )
            {
                MSG_SemanticError( MSG_INCOMPATIBLE_MSCNAME );
                return RESULT_ERR;
            }

            M->msc_Name = MSC_AddStr( temp, M );

            data->msc_Started = 1;
            StrSkipMSCComments( &line );
            if ( *line!=';' )
            {
                if ( !msc_ParseSourceRefs( line, M ) )
                {
                    return RESULT_ERR;
                };
            }
            else
                M->src_MscLevel = 1;

        /* add reference to current MSC */
            M->src_Files = MEM_Reallocate( M->src_Files, ( M->src_Count + 1 ) * sizeof ( M->src_Files[0] ) );
            line = &temp[0];
            *line++ = '\"';
            FP_FullName( line, MSC_GetStr( M->msc_File, M ) );
            line += strlen( line );
            *line++ = '\"';
            *line = 0;
            M->src_Files[M->src_Count].link = MSC_AddStr( temp, M );
            M->src_Files[M->src_Count].level = M->src_MscLevel;
            M->src_Files[M->src_Count].first_Line = 1;
            M->src_Files[M->src_Count].last_Line = 1;
            M->src_Count++;
            continue;
        }

        if ( !strnicmp( line, STR_MSCDOCUMENTSP, STR_MSCDOCUMENTSP_LEN ) )
        {
            line += STR_MSCDOCUMENTSP_LEN;
            StrSkipComments( &line );

            if ( data->mscdocument_Started )
            {
                MSG_SemanticError( MSG_MSC_DUPL_DECLARATION );
                return RESULT_ERR;
            }

            if ( !StrExtractFLabel( &line, temp ) || temp[0] == 0 )
            {
                MSG_SyntaxError(  );
                return RESULT_ERR;
            }

        /* if Untitled MSC is loaded, title it :) */
            if ( !stricmp( temp, STR_UNTITLED ) )
            {
                FP_SplitPath( MSC_GetString( M->msc_File, M ), NULL, temp, NULL );
            }

            if ( SymIsDigit( temp[0] ) )
            {
                MSG_SemanticError( MSG_INCOMPATIBLE_MSCNAME );
                return RESULT_ERR;
            }


            M->msc_Document = MSC_AddStr( temp, M );
            data->mscdocument_Started = 1;
            continue;
        }

        if ( !strnicmp( line, STR_ENDMSC, STR_ENDMSC_LEN ) )
        {
            if ( !data->msc_Started )
            {
                MSG_SemanticError( MSG_ENDMSC_WO_MSC );
                return RESULT_ERR;
            }
            end_msc = 1;
            continue;
        }

        if ( !data->msc_Started )
        {
            MSG_SemanticError( MSG_MSCBODY_WO_MSC );
            return RESULT_ERR;
        }

        if (end_msc)
            break;
        line = strchr( ( char * ) & ( buffer[0] ), ':' );

        if (line > strchr( ( char * ) & ( buffer[0] ), ';' ))
            continue;

        r = line;
        b = &buffer[0];
        StrSkipComments( &b );

        if ( line == NULL )
            continue;

        line++;

        StrSkipComments( &line );

    /* check if we have real transitions (msc is not empty) */
        if ( !strnicmp( line, STR_INSTANCE, STR_INSTANCE_LEN ) ||
            !strnicmp( line, STR_INSTANCEHEAD, STR_INSTANCEHEAD_LEN ) )
        {
            int comment_exist=0,  ratio_exist=0;
            char kind_buffer[TAT_MAX_LINE_LEN];
            size_t kind_buffer_size=0;

            strncpy( temp, b, r - b );
            temp[r - b] = 0;
            StrTrimSpaces( temp );
        /* check instance name for correctness */

            if ( !StrCheckIDName( temp ) || ( temp[0] == 0 ) )
            {
                MSG_SyntaxError(  );
                return RESULT_ERR;
            }

            if ( MSC_GetInstanceID( temp, M ) >= 0 )
            {
                MSG_SemanticError( MSG_INSTANCE_ALREADY_DEFINED );
                return RESULT_ERR;
            }

            M->instances =
                MEM_AddObject( M->instances, ( M->instance_Count + 1 ), sizeof ( MSC_INSTANCE_STORAGE ) );

            M->instances[M->instance_Count].name = MSC_AddStr( temp, M );
            M->instances[M->instance_Count].ind = MSC_INSTANCE_ENV;
            M->instances[M->instance_Count].decompose = 0;
            M->instances[M->instance_Count].comment = 0;
            M->instances[M->instance_Count].ratio = STRCOLL_STR_ID_1;
            M->instances[M->instance_Count].ratio_max = STRCOLL_STR_ID_1;

            M->instances[M->instance_Count].line_number = data->line_Counter;

            line += 8;

            if ( *line && *line != 'h' && *line != ';' && *line != 'H' && !SymIsSpace( *line ) && *line != '{' )
            {
                MSG_SyntaxError(  );
                return RESULT_ERR;
            }

            if ( !strnicmp( line, STR_HEAD, STR_HEAD_LEN ) )
            {
                line += STR_HEAD_LEN;
            }

            kind_buffer[0]=0;
            do
            {
                StrSkipComments( &line );

                if ((*line=='c' || *line=='C') && !strnicmp(line, STR_COMMENTSP,STR_COMMENTSP_LEN))
                {
                    if (comment_exist)
                    {
                        MSG_SyntaxError(  );
                        return RESULT_ERR;
                    }
                    line+=STR_COMMENTSP_LEN;
                    StrSkipComments(&line);
                    if (*line!='\'')
                    {
                        MSG_SyntaxError(  );
                        return RESULT_ERR;
                    }
                    if ( !StrExtractMSCString( &line, temp ) )
                    {
                        MSG_SyntaxError(  );
                        return RESULT_ERR;
                    }
                    M->instances[M->instance_Count].comment = MSC_AddStr( temp, M );
                    comment_exist = 1;
                }
                else
                if (*line=='{')
                {
                    if (ratio_exist)
                    {
                        MSG_SyntaxError(  );
                        return RESULT_ERR;
                    }
                    line++;
                    StrSkipComments( &line );
                    i = 0;

                    while ( *line && *line != '}' && *line!=';') temp[i++] = *line++;

                    temp[i] = 0;

                    if ( *line != '}' )
                    {
                        MSG_SyntaxError(  );
                        return RESULT_ERR;
                    }
                    line++;
                    M->instances[M->instance_Count].ratio = MSC_AddStr( temp, M );

                    if ( temp[0] == 0 || ( !StrCheckIDName( temp ) && !StrIsAnyNumber( temp ) ) ||
                        ( !SymIsDigit( temp[0] ) && temp[0] != MSC_MACRO_START ) )
                    {
                        MSG_SyntaxError(  );
                        return RESULT_ERR;
                    }
                    M->instances[M->instance_Count].ratio_max = M->instances[M->instance_Count].ratio;
                    ratio_exist=1;
                }  else
                {
                    if (kind_buffer_size)
                    {
                        kind_buffer[kind_buffer_size++]=' ';
                    }
                    while ( *line && *line != '{' && *line != ';' && *line != '}'
                             && !SymIsDelim(*line))
                    {
                        kind_buffer[kind_buffer_size++] = *line++;
                    }

                    kind_buffer[kind_buffer_size] = 0;
                    if (*line && SymIsDelim(*line) && *line!=' ' && *line>31
                        && *line!='{' && *line!=';')
                    {
                        MSG_SyntaxError(  );
                        return RESULT_ERR;
                    }
                }

            }  while (*line && *line!=';');

            M->instances[M->instance_Count].type = MSC_AddStr( kind_buffer, M );

            M->instance_Count++;
            StrSkipComments( &line );
            if ( *line!=';' )
            {
                MSG_SyntaxError(  );
                return RESULT_ERR;
            }

            continue;
        }

        if ( !strnicmp( line, STR_ENDINSTANCE, STR_ENDINSTANCE_LEN ) )
        {
            strncpy( temp, b, r - b );
            temp[r - b] = 0;
            StrTrimSpaces( temp );
            line += 11;
            StrSkipComments( &line );

            if ( *line!=';' )
            {
                MSG_SyntaxError(  );
                return RESULT_ERR;
            }

            if ( MSC_GetInstanceID( temp, M ) < 0 )
            {
                MSG_SemanticError( MSG_UNDECLARED_INSTANCE );
                return RESULT_ERR;
            }
            continue;
        }

        if ( !strnicmp( line, STR_STOP, STR_STOP_LEN ) )
        {
            strncpy( temp, b, r - b );
            temp[r - b] = 0;

            if ( MSC_GetInstanceID( temp, M ) < 0 )
            {
                MSG_SemanticError( MSG_UNDECLARED_INSTANCE );
                return RESULT_ERR;
            }

            continue;
        }

    }

    if ( end_msc == 0 )
    {
        MSG_SemanticError( MSG_NO_ENDMSC_FIELD );
        return RESULT_ERR;
    }

/* move to the start of file */
    data->current_Position = data->source_Buffer;

/* try to swap instances */
    for ( i = 0; i < M->instance_Count; i++ )
    {
        if ( !strnicmp( MSC_GetString( M->instances[i].name, M ), STR_ENV, STR_ENV_LEN ) ||
            !stricmp( MSC_GetString( M->instances[i].name, M ), "InP" ) )

        {                       /* make Env the first */
            MSC_INSTANCE_STORAGE    k;

            k = M->instances[0];
            M->instances[0] = M->instances[i];
            M->instances[i] = k;
        }
    }

    return RESULT_OK;
}


/***************************************************************************
 * Function : msc_ParseTextDef
 *
 * Description: parse variable,parameter defintions
 *
 * Returns: 0-not parsed/ 1-parsed RESULT_OK, 2-parsed with errors
 *
 * Notes: None
 *
 **************************************************************************/
static int msc_ParseTextDef( char * s, MSC_STORAGE * M, MSC_LOAD_DATA * data )
{
    char                    buffer[TAT_MAX_LINE_LEN];
    STRING_ID              curtype;
    size_t                  i, j, num, mode;

    if ( !s )
        return 0;
    
    STR_SKIP_SPACES(s);
    /* handle PTK specific directives */
    if (*s=='/' && *(s+1)=='*' && *(s+2)=='<')
    {
        s+=3;
        STR_SKIP_SPACES(s);
        if (!strnicmp(s, "PARAMS", 6) && SymIsDelim(s[6]))
        {
            s+=6;
            STR_SKIP_SPACES(s);
            if (*s!='(')
            {
                MSG_SyntaxError();
                return 2;
            }
            s++;
            if (strchr(s, ':'))
            {
                /* TTCN-2 format */
                M->params_Type=TTCN2_PARAMS;
                while (*s && *s!=')')
                {
                    size_t   start_i;

                    start_i = M->param_Count;
                    s--;
                    do {
                        s++;
                        STR_SKIP_SPACES(s);
                        /* extract first item */
                        if ( !StrExtractLabel( &s, buffer ) || ( buffer[0] == 0 ) )
                        {
                            MSG_SyntaxError(  );
                            return 2;
                        };
                        StrSkipComments( &s );
                        if ( !StrCheckIDName( buffer ) )
                        {
                            MSG_SemanticError( MSG_INVALID_VARNAME );
                            return 2;
                        }

                        M->params = MEM_AddObject( M->params, ( M->param_Count + 1 ), sizeof ( M->params[0] ) );
                        M->params[M->param_Count].pv_Name = MSC_AddStr( buffer, M );
                        M->params[M->param_Count].line_number = data->line_Counter;

                        M->param_Count++;
                    } while (*s==',');
                    if (*s!=':')
                    {
                        MSG_SyntaxError();
                        return 2;
                    }
                    s++;
                    if ( !StrExtractLabel( &s, buffer ) || ( buffer[0] == 0 ) )
                    {
                            MSG_SyntaxError(  );
                            return 2;
                    };
                    curtype=MSC_AddStr(buffer,M);
                    while (start_i<M->param_Count) M->params[start_i++].pv_Type=curtype;
                    STR_SKIP_SPACES(s);
                    
                    if (*s==';')
                    {
                        s++;
                        StrSkipComments(&s);
                    }
                } /* while */
            }
            else
            {
                /* TTCN-3 format */
                M->params_Type=TTCN3_PARAMS;
                while (*s && *s!=')')
                {
                    STR_SKIP_SPACES(s);
                    /* extract first item */
                    if ( !StrExtractLabel( &s, buffer ) || ( buffer[0] == 0 ) )
                    {
                        MSG_SyntaxError(  );
                        return 2;
                    };
                    StrSkipComments( &s );
                    if ( !StrCheckIDName( buffer ) )
                    {
                        MSG_SemanticError( MSG_INVALID_VARNAME );
                        return 2;
                    }

                    M->params = MEM_AddObject( M->params, ( M->param_Count + 1 ), sizeof ( M->params[0] ) );
                    M->params[M->param_Count].pv_Type = MSC_AddStr( buffer, M );
                    M->params[M->param_Count].line_number = data->line_Counter;

                    /* extract second item */
                    if ( !StrExtractLabel( &s, buffer ) || ( buffer[0] == 0 ) )
                    {
                        MSG_SyntaxError(  );
                        return 2;
                    };
                    StrSkipComments( &s );
                    if ( !StrCheckIDName( buffer ) )
                    {
                        MSG_SemanticError( MSG_INVALID_VARNAME );
                        return 2;
                    }
                    M->params[M->param_Count].pv_Name = MSC_AddStr( buffer, M );
                    M->param_Count++;
                    if (*s==',')
                        s++;
                }  /* while (*s && *s!=')') */
            } /* !strchr(s, ':') - TTCN3*/
            STR_SKIP_SPACES(s);
            if (*s!=')')
            {
                MSG_SyntaxError();
                return 2;
            }
            s++;
            STR_SKIP_SPACES(s);
            if (*s!='>' || *(s+1)!='*' || *(s+2)!='/')
            {
                MSG_SyntaxError();
                return 2;
            }


        }
        return 1;
    }
    StrSkipComments(&s);
    mode = 0;
    while (*s) 
    {
        curtype = 0;
        if ( !strnicmp( s, STR_DEFSP, STR_DEFSP_LEN ) )
        {
            mode = 1;
            s += STR_DEFSP_LEN;
            StrSkipComments( &s );
            /* avoid empty 'def' statements */
            if (*s==0 || *s==';')
            {
                MSG_SyntaxError(  );
                return 2;
            }

            while (*s && *s!=';')
            {
                /* extract first item */
                if ( !StrExtractLabel( &s, buffer ) || ( buffer[0] == 0 ) )
                {
                    MSG_SyntaxError(  );
                    return 2;
                };

                StrSkipComments( &s );
                if (*s && *s!=',' && *s!=';')
                {
                    /* set new type */
                    curtype = MSC_AddStr( buffer, M );
                    /* parse variable name */
                    if ( !StrExtractLabel( &s, buffer ) || ( buffer[0] == 0 ) )
                    {
                        MSG_SyntaxError(  );
                        return 2;
                    };
                }

                if ( !StrCheckIDName( buffer ) )
                {
                    MSG_SemanticError( MSG_INVALID_VARNAME );
                    return 2;
                }

                /* buffer contains name of variable */
                M->vars = MEM_AddObject( M->vars, ( M->var_Count + 1 ), sizeof ( M->vars[0] ) );
                
                /* TO DO: check for variable redefinition */
                M->vars[M->var_Count].pv_Name = MSC_AddStr( buffer, M );
                M->vars[M->var_Count].pv_Type = curtype;
                M->vars[M->var_Count].line_number = data->line_Counter;

                M->var_Count++;
                if (*s==',')
                {
                    s++;
                    StrSkipComments(&s);
                }
            } /* while */
        } /* 'def' */
        else if ( !strnicmp( s, STR_PARAMSSP, STR_PARAMSSP_LEN ) )
        {
            mode = 2;
            s += STR_PARAMSSP_LEN;
            StrSkipComments( &s );
            M->params_Type = TAT_PARAMS;

            /* avoid empty 'params' statements */
            if (*s==0 || *s==';')
            {
                MSG_SyntaxError(  );
                return 2;
            }

            while (*s && *s!=';')
            {
                /* extract first item */
                if ( !StrExtractLabel( &s, buffer ) || ( buffer[0] == 0 ) )
                {
                    MSG_SyntaxError(  );
                    return 2;
                };

                StrSkipComments( &s );
                if (*s && *s!=',' && *s!=';')
                {
                    /* set new type */
                    curtype = MSC_AddStr( buffer, M );
                    /* parse variable name */
                    if ( !StrExtractLabel( &s, buffer ) || ( buffer[0] == 0 ) )
                    {
                        MSG_SyntaxError(  );
                        return 2;
                    };
                }

                if ( !StrCheckIDName( buffer ) )
                {
                    MSG_SemanticError( MSG_INVALID_VARNAME );
                    return 2;
                }

                M->params = MEM_AddObject( M->params, ( M->param_Count + 1 ), sizeof ( M->params[0] ) );
                M->params[M->param_Count].pv_Name = MSC_AddStr( buffer, M );
                M->params[M->param_Count].pv_Type = curtype;
                M->params[M->param_Count].line_number = data->line_Counter;

                M->param_Count++;
                if (*s==',')
                {
                    s++;
                    StrSkipComments(&s);
                }
            } /* while */
        } /* 'params' */
        else
        {
            if (mode)
            {
                MSG_SyntaxError();
                return 2;
            }

            /* check for VRS verdict specification */
            num = 0;
            for ( i = 0; i < MSC_MSC_VERDICT_COUNT; i++ )
            {
                j = strlen( VRS_DiagramVerdictText[i] );
                if ( !strncmp( s, VRS_DiagramVerdictText[i], j ) )
                {
                    s += j;
                    break;
                }
            }
            /* if it's not a verdict */
            if ( i == MSC_MSC_VERDICT_COUNT )
            {
                return 0;
            }
            STR_SKIP_SPACES( s );
            if ( i == MSC_VRS_CONSISTENT )
            {
                if ( *s )
                {
                    MSG_SyntaxError(  );
                    return 2;
                }
            }
            else
            {
                if ( *s != '(' )
                {
                    MSG_SyntaxError(  );
                    return 2;
                }
                s++;
                STR_SKIP_SPACES( s );
                if ( strncmp( s, "Trace", 5 ) )
                {
                    MSG_SyntaxError(  );
                    return 2;
                }
                s += 5;
                if ( !StrExtractDigit( &s, buffer ) )
                {
                    MSG_SyntaxError(  );
                    return 2;
                }

                num = atoi( buffer );

                if ( *s != ')' )
                {
                    MSG_SyntaxError(  );
                    return 2;
                }
                s++;
                STR_SKIP_SPACES( s );
                if ( *s )
                {
                    MSG_SyntaxError(  );
                    return 2;
                }
            }
            M->traces = MEM_Reallocate( M->traces, ( M->trace_Count + 1 ) * sizeof ( M->traces[0] ) );
            M->traces[M->trace_Count].status = i;
            M->traces[M->trace_Count].trace = (size_t)num;
            M->trace_Count++;
        }
        if (*s==';')
        {
            s++;
            STR_SKIP_SPACES(s);
        }
    } /* while */

    return 1;
}

/***************************************************************************
 * Function : msc_ParseTextStatement
 *
 * Description: parse 'text' statement in MSC
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL msc_ParseTextStatement(char * line, MSC_STORAGE * M, MSC_LOAD_DATA * data)
{
    int i;
    char                    buffer[TAT_MAX_LINE_LEN];

    if ( !StrExtractMSCString( &line,  buffer ) )
    {
        MSG_SyntaxError(  );
        return RESULT_ERR;
    }
    StrSkipComments( &line );

    if (!MSC_DoNotParseText)
    {
        i = msc_ParseTextDef( buffer, M, data );

        if ( i == 2 )
            return RESULT_ERR;

    }
    else
        i = 0;


    /* check for extra characters after 'text' statement */
    if (*line!=';')
    {
        MSG_SyntaxError(  );
        return RESULT_ERR;
    }

    if ( i == 0 )
    {
        /* this is informal text or comments */
        M->texts = MEM_AddObject( M->texts, ( ++M->text_Count ), sizeof ( STRING_ID ) );
        M->texts[M->text_Count - 1] = MSC_AddStr( buffer, M );
    }
    return RESULT_OK;
}

/***************************************************************************
 * Function: msc_LoadEvents
 *
 * Description: load & parse MSC-PR file
 *
 * Return: none
 *
 * Notes:
 *
 **************************************************************************/
static int msc_LoadEvents( MSC_EVENT_KIND * s,
    MSC_OPERATOR_TYPE Curr, MSC_STORAGE * M, MSC_LOAD_DATA * data )
{
    size_t                  i, end;
    char                   *line, *b;
    UINT32_T                    j;

    if ( !M || !s )
        return RESULT_ERR;

    while ( *data->current_Position )
    {
        line = msc_LoadLine( data );

        if ( !line )
            break;

        b = line;

    /* skip default lines */
        if ( !strnicmp( line, STR_MSCSP, STR_MSCSP_LEN ) )
        {
            continue;
        }

        if ( !strnicmp( line, STR_MSCDOCUMENTSP, STR_MSCDOCUMENTSP_LEN ) )
        {
            continue;
        }

        if ( !strnicmp( line, STR_ENDMSC, STR_ENDMSC_LEN ) )
        {
            data->msc_Started = 0;
            data->msc_Ended = 1;
        /* check for inline operator terminator */

        /* check all data->coregion_State are closed */

            for ( i = 0; i < ( int ) M->instance_Count; i++ )
                if ( data->coregion_State[i] )
                {
                    MSG_SemanticError( MSG_OPENED_COREGION );
                    return RESULT_ERR;
                }

        /* check for signals */
            if ( !msc_CheckInOutParity( data ) )
                return RESULT_ERR;

            if ( data->timers )
            {
                for ( i = 0; i < ( int ) data->timer_Counter; i++ )
                    if ( data->timers[i].event_Pointer )
                    {
                        MSG_SemanticError( MSG_INCONSISTENT_TIMER );
                        return RESULT_ERR;
                    }
            }

            break;
        }

    /* check if we should exit */
        end = MSC_CheckOperator( line, Curr );

        if ( end != MSC_NOT_END )
        {
        /* check for signals */
            if ( !msc_CheckInOutParity( data ) )
                return RESULT_ERR;

            if ( Curr == MSC_OP_LOOP )
            {
                s->type = MSC_EV_END_LOOP;
                MSC_ZeroEventTime( s );
                MSC_ExpandInstance( s, M );
            }

            return end;
        }


    /* text statement is not attached to anything */
        if ( !strnicmp( line, STR_TEXTSP, STR_TEXTSP_LEN ) )
        {
            line += STR_TEXTSP_LEN;
            StrSkipComments( &line );

            if (!msc_ParseTextStatement(line, M, data))
            {
                return RESULT_ERR;
            }
            continue;
        }

    /* ----------------------------------------------------------- */
    /* parse statements which belongs to instance axes */

        line = strchr( line, ':' );

        if ( line == NULL )
        {
            MSG_SyntaxError(  );
            return RESULT_ERR;
        }

    /* parse instances into event parameters */
        if (!MSC_ParseInstances( b, &end, &( s->instances ), M, data ))
        {
            return RESULT_ERR;
        }
        s->inst_Count = ( MSC_INSTANCE_ID_TYPE ) end;

    /* we will fill this structure */
        line++;
        StrSkipComments( &line );
    /* we should check that after instances the line is not empty */

        if ( *line == 0 )
        {
            MSG_SyntaxError(  );
            return RESULT_ERR;
        }

        if ( !strnicmp( line, STR_INSTANCE, STR_INSTANCE_LEN ) ||
            !strnicmp( line, STR_INSTANCEHEAD, STR_INSTANCEHEAD_LEN ) )
        {
            if ( s->inst_Count != 1 )
            {
                MSG_SemanticError( MSG_MULTIPLE_INSTANCES );
                return RESULT_ERR;
            }
            if ( data->instance_State[s->instances[0]].instance_Declared )
            {
                MSG_SemanticError( MSG_INSTANCE_ALREADY_DEFINED );
                return RESULT_ERR;
            }
            data->instance_State[s->instances[0]].instance_Declared = 1;
            data->instance_State[s->instances[0]].branch_ID = data->branch_ID;
            MEM_Free( s->instances );
            s->instances = NULL;
            s->inst_Count = 0;
            continue;
        }

        if ( !strnicmp( line, STR_ENDINSTANCE, STR_ENDINSTANCE_LEN ) )
        {
            if ( s->inst_Count != 1 )
            {
                MSG_SemanticError( MSG_MULTIPLE_INSTANCES );
                return RESULT_ERR;
            }

            if ( !data->instance_State[s->instances[0]].instance_Declared )
            {
                MSG_SemanticError( MSG_INST_STOP_NOTCREATE );
                return RESULT_ERR;
            }

            if ( data->instance_State[s->instances[0]].instance_Ended ||
                data->instance_State[s->instances[0]].instance_Stopped )
            {
                MSG_SemanticError( MSG_INST_STOP_TWICE );
                return RESULT_ERR;
            }
            data->instance_State[s->instances[0]].instance_Ended = 1;
            MEM_Free( s->instances );
            s->instances = NULL;
            continue;
        }

        s->line_Number = data->line_Counter;


        switch ( MSC_GetEventType( &line, M, s ) )
        {

        case MSC_EV_UNDEFINED:
        {
        /* Handle error result from MSC_GetEventType */
            return RESULT_ERR;
        }

        case MSC_EV_STOP:
        {
            if ( s->inst_Count > 1 )
            {
                MSG_SemanticError( MSG_MULTIPLE_INSTANCES );
                return RESULT_ERR;
            }

            if ( !data->instance_State[s->instances[0]].instance_Created )
            {
                MSG_SemanticError( MSG_INST_STOP_NOTCREATE );
                return RESULT_ERR;
            }

            if ( data->instance_State[s->instances[0]].instance_Stopped )
            {
                MSG_SemanticError( MSG_INST_STOP_TWICE );
                return RESULT_ERR;
            }

            if ( data->loop_NestLevel )
            {
                MSG_SemanticError( MSG_STOP_INSIDE_LOOP );
                return RESULT_ERR;
            }

            data->instance_State[s->instances[0]].instance_Stopped = 1;

            if ( data->instance_State[s->instances[0]].branch_ID != data->branch_ID )
            {
                MSG_SemanticError( MSG_STOP_OUTSIDE_BLOCK );
                return RESULT_ERR;
            }

            StrSkipMSCComments( &line );
            if ( !MSC_ParseSourceRef( line, s, M ) )
            {
                return RESULT_ERR;
            }

            break;
        }

        case MSC_EV_CREATE:
        {
            if ( s->inst_Count > 1 )
            {
                MSG_SemanticError( MSG_MULTIPLE_INSTANCES );
                return RESULT_ERR;
            }

            if ( !msc_CheckInstanceState( s, data ) )
                return RESULT_ERR;

            if ( !MSC_ParseCreate( line, s, M ) )
            {
                return RESULT_ERR;
            }

        /* created instance shall not be used before */
            if ( data->instance_State[s->data.create.instance].instance_Used )
            {
                MSG_SemanticError( MSG_CREATE_USED_INST );
                return RESULT_ERR;
            }

            if ( data->instance_State[s->data.create.instance].instance_Created == 1 &&
                data->instance_State[s->data.create.instance].instance_Stopped == 0 )
            {
                MSG_SemanticError( MSG_DOUBLE_INST_CREATION );
                return RESULT_ERR;
            }

            if ( data->loop_NestLevel )
            {
                MSG_SemanticError( MSG_CREATE_INSIDE_LOOP );
                return RESULT_ERR;
            }

            data->instance_State[s->data.create.instance].instance_Created = 1;
            data->instance_State[s->data.create.instance].instance_Stopped = 0;
            data->instance_State[s->data.create.instance].branch_ID = data->branch_ID;
            break;
        }

        case MSC_EV_IN_MESSAGE:
        case MSC_EV_OUT_MESSAGE:
        case MSC_EV_SEND_SIGNAL:
        case MSC_EV_RECV_SIGNAL:
        case MSC_EV_EXIST_SIGNAL:
        {
            if ( s->inst_Count > 1 )
            {
                MSG_SemanticError( MSG_MULTIPLE_INSTANCES );
                return RESULT_ERR;
            }

            if ( !msc_CheckInstanceState( s, data ) )
                return RESULT_ERR;

            if ( !msc_ParseMessage( line, s, s->type, M, data ) )
            {
                return RESULT_ERR;
            }

            break;
        }

        case MSC_EV_CONCURRENT_START:
        {
            if ( s->inst_Count > 1 )
            {
                MSG_SemanticError( MSG_MULTIPLE_INSTANCES );
                return RESULT_ERR;
            }

            if ( !msc_CheckInstanceState( s, data ) )
                return RESULT_ERR;

        /* check for extra symbols */
            StrSkipMSCComments( &line );
            if ( !MSC_ParseSourceRef( line, s, M ) )
            {
                return RESULT_ERR;
            }

            data->coregion_State[s->instances[0]]++;

            if ( data->coregion_State[s->instances[0]] > 1 )
            {
                MSG_SemanticWarning( MSG_NESTED_COREGIONS );
                /* do not load this event */
                MSC_FreeEventData(s);
                memset(s, 0, sizeof(s[0]));
                continue;
            }


        /* set zero time for data->coregion_State */
            MSC_ZeroEventTime( s );

            break;
        }

        case MSC_EV_CONCURRENT_END:
        {
            if ( s->inst_Count > 1 )
            {
                MSG_SemanticError( MSG_MULTIPLE_INSTANCES );
                return RESULT_ERR;
            }

            if ( !msc_CheckInstanceState( s, data ) )
                return RESULT_ERR;

            if ( data->coregion_State[s->instances[0]] == 0 )
            {
                MSG_SemanticError( MSG_UNOPENED_COREGION );
                return RESULT_ERR;
            }

        /* check for extra symbols */
            StrSkipMSCComments( &line );
            if ( !MSC_ParseSourceRef( line, s, M ) )
            {
                return RESULT_ERR;
            }

            data->coregion_State[s->instances[0]]--;
            if (data->coregion_State[s->instances[0]]>0)
            {
                /* do not load this event */
                MSC_FreeEventData(s);
                memset(s, 0, sizeof(s[0]));
                continue;
            }

        /* set zero time for data->coregion_State */
            MSC_ZeroEventTime( s );
            break;
        }

        case MSC_EV_CONDITION:
        {
            if ( !msc_CheckInstanceState( s, data ) )
                return RESULT_ERR;

            if ( !MSC_ParseCondition( line, s, M, data ) )
            {
                return RESULT_ERR;
            }

            break;
        }

        case MSC_EV_ACTION:
        {
            if ( s->inst_Count > 1 )
            {
                MSG_SemanticError( MSG_MULTIPLE_INSTANCES );
                return RESULT_ERR;
            }

            if ( !msc_CheckInstanceState( s, data ) )
                return RESULT_ERR;

            if ( !MSC_ParseAction( line, s, M ) )
            {
                return RESULT_ERR;
            };

            MSC_ZeroEventTime( s );

            break;
        }

        case MSC_EV_REFERENCE:
        {
            if ( !msc_CheckInstanceState( s, data ) )
                return RESULT_ERR;

            if ( !MSC_ParseReference( line, s, M ) )
            {
                return RESULT_ERR;
            }

            break;
        }

        case MSC_EV_TIMER_SET:
        case MSC_EV_TIMER_RESET:
        case MSC_EV_TIMER_TIMEOUT:
        {
            if ( s->inst_Count > 1 )
            {
                MSG_SemanticError( MSG_MULTIPLE_INSTANCES );
                return RESULT_ERR;
            }

            if ( !msc_CheckInstanceState( s, data ) )
                return RESULT_ERR;

            if ( !MSC_ParseTimer( line, s, M ) )
            {
                return RESULT_ERR;
            }

            MSC_ZeroEventTime( s );

            if ( s->type == MSC_EV_TIMER_SET )
            {

                for ( j = 0; j < data->timer_Counter; j++ )
                {
                    if ( data->timers[j].timer_Index == s->data.timer.timer_ID )
                    {
                        if ( data->timers[j].event_Pointer != NULL )
                        {
                            MSG_SemanticError( MSG_DUPLICATED_TIMER );
                            return RESULT_ERR;
                        }
                        else
                            break;
                    }
                }

                if ( j == data->timer_Counter )
                {
                    data->timers = MEM_AddObject( data->timers, ( j + 1 ), sizeof ( data->timers[0] ) );
                    data->timer_Counter++;
                }

                data->timers[j].event_Pointer = s;
                data->timers[j].timer_Type = s->data.timer.timing.type;
                data->timers[j].timer_Index = s->data.timer.timer_ID;

                if ( M->last_TimerNum < s->data.timer.timer_ID )
                    M->last_TimerNum = s->data.timer.timer_ID;

            /* renumber all timers */
                s->data.timer.timer_ID = ++data->uniq_TimerID;
            }
            else
            {

                for ( j = 0; j < data->timer_Counter; j++ )
                {
                    if ( data->timers[j].timer_Index == s->data.timer.timer_ID )
                    {
                    /* copy timer type from 'set' statement */
                        s->data.timer.timing.type = data->timers[j].timer_Type;
                    /* link to 'set' statement */
                        s->data.timer.relation = data->timers[j].event_Pointer;

                        if ( data->timers[j].event_Pointer == NULL )
                        {
                            MSG_SemanticError( MSG_DUPLICATED_TIMER );
                            return RESULT_ERR;
                        }

                    /* renumber all timers */
                        s->data.timer.timer_ID = data->timers[j].event_Pointer->data.timer.timer_ID;

                        break;
                    }
                }

                if ( j == data->timer_Counter )
                {
                    MSG_SemanticError( MSG_UNDEFINED_TIMER );
                    return RESULT_ERR;
                }

                if ( !data->timers ||
                    ( data->timers != NULL &&
                        ( ( ( s->data.timer.timing.type == MSC_SPECIAL_TIMER &&
                                    ( s->data.timer.timing.str_MinTime !=
                                        data->timers[j].event_Pointer->data.timer.timing.str_MinTime ||
                                        s->data.timer.timing.str_MaxTime !=
                                        data->timers[j].event_Pointer->data.timer.timing.str_MaxTime ) ) ||
                                s->instances[0] != data->timers[j].event_Pointer->instances[0] ) ) ) )
                {
                    MSG_SemanticError( MSG_INCONSISTENT_TIMER );
                    return RESULT_ERR;
                }

            /* backward relation set */
                data->timers[j].event_Pointer->data.timer.relation = s;

                data->timers[j].event_Pointer = NULL;
            }

            break;
        }

        case MSC_EV_OPERATOR:
        {
/*                if ( !msc_CheckInstanceState( s, data ) )
                    return RESULT_ERR;
*/
        /* check operators are not in data->coregion_State */
            for ( i = 0; i < ( int ) M->instance_Count; i++ )
                if ( data->coregion_State[i] )
                {
                    MSG_SemanticError( MSG_OPER_IN_COREGION );
                    return RESULT_ERR;
                }

            /* check for signals */
            if ( !msc_CheckInOutParity( data ) )
                return RESULT_ERR;

            s->data.alt.path_Limit = MSC_INFINITY_CONST;

            if ( s->comment )
            {
                char                   *comm = MSC_GetString( s->comment, M );

                if ( comm[0] == ( char ) '$' )
                {
                    if ( !StrIsNaturalNumber( comm + 1 ) )
                    {
                        MSG_SemanticWarning( MSG_BAD_PATH_CONTROL );
                    }
                    else
                        s->data.alt.path_Limit = ( UINT32_T ) StrToInt( comm + 1 );
                    if ( s->data.alt.path_Limit == 0 )
                    {
                        MSG_SemanticWarning( MSG_BAD_PATH_CONTROL );
                        s->data.alt.path_Limit = MSC_INFINITY_CONST;
                    }
                }
            }
            s->branch_ID = data->branch_ID;

            /* check event correctness */
            if (!MSC_CheckEventSpecification( s, M, data))
            {
                return RESULT_ERR;
            }

            switch ( s->data.alt.type )
            {

            case MSC_OP_ALT:
            case MSC_OP_PAR:
            {
                int                     result;
                size_t                 t, save;

                s->data.alt.frame_Count = 1;
                t = 0;
                result = MSC_REPEAT_ACTION;

                while ( result == MSC_REPEAT_ACTION )
                {
                    s->data.alt.frame = MEM_AddObject( s->data.alt.frame, ( t + 1 ), sizeof ( MSC_EVENT_KIND * ) );
                    s->data.alt.frame[t] = MSC_AllocateEvent( M );

                    if ( !s->data.alt.frame[t] )
                        return RESULT_ERR;
                    save = data->branch_ID;

                    data->branch_ID = ++data->last_ID;

                    if ( s->data.alt.type == MSC_OP_ALT )
                    {
                        data->nest_Level++;
                        data->nest_Path =
                            MEM_Reallocate( data->nest_Path, data->nest_Level * sizeof ( data->nest_Path[0] ) );
                        data->nest_Path[data->nest_Level - 1] = data->branch_ID;
                    }

                    result = msc_LoadEvents( s->data.alt.frame[t], s->data.alt.type, M, data );

                    data->branch_ID = save;
                    if ( s->data.alt.type == MSC_OP_ALT )
                    {
                        data->nest_Level--;
                    }

                    if ( result == RESULT_ERR )
                        return RESULT_ERR;

                    if ( s->data.alt.frame[t]->type == MSC_EV_UNDEFINED )
                    {
                        MEM_Free( s->data.alt.frame[t] );
                        s->data.alt.frame[t] = NULL;
                        if ( s->data.alt.type == MSC_OP_ALT )
                        {
                            t++;
                        }
                    }
                    else
                    {
                        t++;
                    }
                }

                if ( t == 0 )
                {
                    MSC_FreeEventData(s);
                    memset(s, 0, sizeof(s[0]));
                    continue;
                }
                else
                    s->data.alt.frame_Count = t;

                break;
            }

            case MSC_OP_EXC:
            case MSC_OP_OPT:
            case MSC_OP_LOOP:
            {
                int                     result;
                size_t                  save;

                s->data.alt.frame_Count = 1;
                s->data.alt.frame = MEM_Allocate( sizeof ( MSC_EVENT_KIND * ) );
                s->data.alt.frame[0] = MSC_AllocateEvent( M );

                if ( !s->data.alt.frame[0] )
                    return RESULT_ERR;

                if ( s->data.alt.type == MSC_OP_LOOP )
                {
                    MSC_EVENT_KIND              *pp = s->data.alt.frame[0];

                    pp->type = MSC_EV_START_LOOP;
                    MSC_ZeroEventTime( pp );
                    MSC_ExpandInstance( pp, M );
                    pp->next = MSC_AllocateEvent( M );
                    pp->data.loop.l1 = s->data.alt.l1;
                    pp->data.loop.l2 = s->data.alt.l2;

                    data->loop_NestLevel++;
                    save = data->branch_ID;
                    data->branch_ID = ++data->last_ID;

                    result = msc_LoadEvents( pp->next, MSC_OP_LOOP, M, data );
                    data->branch_ID = save;

                    data->loop_NestLevel--;

                    if ( pp->next->type == MSC_EV_CONDITION && pp->next->data.cond.call &&
                        stricmp(MSC_GetStr(pp->next->data.cond.text, M), STR_WHILE)==0 )
                    {
                    /* free unused data */
                        pp->data.loop.text = pp->next->data.cond.call;
                        pp->data.loop.name = pp->next->data.cond.text;
                        pp->next->type = MSC_EV_EMPTY;
                    }
                }
                else
                {
                    save = data->branch_ID;
                    data->branch_ID = ++data->last_ID;
                    data->nest_Level++;
                    data->nest_Path =
                        MEM_Reallocate( data->nest_Path, data->nest_Level * sizeof ( data->nest_Path[0] ) );
                    data->nest_Path[data->nest_Level - 1] = data->branch_ID;

                    result = msc_LoadEvents( s->data.alt.frame[0], s->data.alt.type, M, data );
                    data->branch_ID = save;
                    data->nest_Level--;
                }

                if ( result == RESULT_ERR )
                    return RESULT_ERR;

                /* if it is empty frame */
                if ( s->data.alt.frame[0]->type == MSC_EV_UNDEFINED )
                {
                    /* specially handle EXC operator */
                    if (s->data.alt.type == MSC_OP_EXC)
                    {
                        MSC_FreeEventData(s->data.alt.frame[0]);
                        s->data.alt.frame[0] = NULL;
                    }
                    else
                    {
                        MSC_FreeEventData(s->data.alt.frame[0]);
                        MSC_FreeEventData(s);
                        memset(s, 0, sizeof(s[0]));
                        continue;
                    }

                }
                break;
            }

            case MSC_OP_SEQ:
            {
                MSG_Abort( MSG_INTERNAL_PROBLEM );
            }
            }

            break;
        }                       /* case MSC_EV_OPERATOR: */

        }                       /* switch (MSC_GetEventType(&line, M, s)) */

        if (s->type != MSC_EV_OPERATOR)
        {
            s->branch_ID = data->branch_ID;

            /* check event correctness */
            if (!MSC_CheckEventSpecification( s, M, data))
            {
                return RESULT_ERR;
            }
        }

        s->next = MSC_AllocateEvent( M );
        s = s->next;
    }

/* check for inline operator terminator */
    if ( Curr != MSC_OP_SEQ )
    {
        MSG_SemanticError( MSG_TERM_MISSING );
        return RESULT_ERR;
    }

/* check all data->coregion_State are closed */
    for ( i = 0; i < ( int ) M->instance_Count; i++ )
        if ( data->coregion_State[i] )
        {
            MSG_SemanticError( MSG_OPENED_COREGION );
            return RESULT_ERR;
        }

/* check for signals */
    if ( !msc_CheckInOutParity( data ) )
        return RESULT_ERR;

    if ( data->timers )
    {
        for ( i = 0; i < ( int ) data->timer_Counter; i++ )
            if ( data->timers[i].event_Pointer )
            {
                MSG_SemanticError( MSG_INCONSISTENT_TIMER );
                return RESULT_ERR;
            }
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_Load
 *
 * Description: High-Level function for MSC-PR loading
 *
 * Return: RESULT_ERR/RESULT_OK
 *
 * Notes:
 *
 **************************************************************************/

LOGICAL MSC_Load( const char * filename, MSC_STORAGE * MSC, const UINT32_T flags)
{
    int                     result;
    size_t                  i, j, k;
    MSC_EVENT_KIND              *root;
    static int              MSC_Load_Invoked = 0;
    MSC_LOAD_DATA       data;

    if ( !filename || !MSC || filename[0] == 0 )
    {
        return RESULT_ERR;
    }

/* prevent reentrance */
    if ( MSC_Load_Invoked )
    {
        return RESULT_ERR;
    }

    MSC_Load_Invoked++;
    memset( &MSC[0], 0, sizeof ( MSC[0] ) );
    memset( &data, 0, sizeof ( data ) );
    data.error_State = RESULT_OK;
    MSG_PushProcessedFile( filename );
    data.line_Counter = 1;
    MSG_SetLineNumber( 1 );

    result = msc_PreLoad( filename, MSC, &data );

    if ( result == RESULT_OK )
    {
        root = MSC_AllocateEvent( MSC );

        if ( root == NULL )
        {
            MSC_Load_Invoked--;
            MSG_PopProcessedFile(  );
            return RESULT_ERR;
        }

        data.line_Counter = 1;
        data.coregion_State = MEM_Allocate( MSC->instance_Count * sizeof ( int ) );
        data.instance_State = MEM_Allocate( MSC->instance_Count * sizeof ( MSC_INSTANCE_STATE ) );
        data.nest_Level = 1;
        data.nest_Path = MEM_Allocate( data.nest_Level * sizeof ( data.nest_Path[0] ) );

        MSG_SetLineNumber( 1 );
        result = msc_LoadEvents( root, MSC_OP_SEQ, MSC, &data );

        if ( result != RESULT_ERR )
        {
            for ( i = 0; i < data.variable_Counter; i++ )
            {
                for ( j = 0; j < MSC->var_Count; j++ )
                {
                    if ( !strcmp( data.var_Storage[i].var_Name, MSC_GetString( MSC->vars[j].pv_Name, MSC ) ) )
                        break;
                }
                if ( j == MSC->var_Count )
                {
                    MSG_PrintError( MSG_SEM_UNDECL_VAR_sus,
                        MSC_GetString( MSC->msc_File, MSC ), data.var_Storage[i].line_Number,
                        data.var_Storage[i].var_Name );
                    result = RESULT_ERR;
                    break;
                }
            }
        /* check end conditions */
            k = 0;
            if ( result == RESULT_OK )
            {
                for ( i = 0; i < MSC->instance_Count; i++ )
                {
                    if ( data.instance_State[i].instance_Created && !data.instance_State[i].instance_Stopped )
                    {
                        MSG_SemanticError( MSG_CREATE_WO_STOP );
                        result = RESULT_ERR;
                        break;
                    }

                    if ( data.instance_State[i].instance_Declared && !data.instance_State[i].instance_Ended &&
                        !( data.instance_State[i].instance_Created &&
                            data.instance_State[i].instance_Stopped ) )
                    {
                        if ( !k )
                            MSG_SemanticWarning( MSG_MISSED_ENDINSTANCE );
                        k++;
                    }
                }
            }
        }

    /* free conditionally allocated memory */
        if ( data.var_Storage )
        {
            for ( i = 0; ( UINT32_T ) i < data.variable_Counter; i++ )
            {
                MEM_Free( data.var_Storage[i].var_Name );
            }
            MEM_Free( data.var_Storage );
        }

        if ( data.signal_Storage )
        {
            for ( i = 0; i < data.signal_Counter; i++ )
            {
                MEM_Free( data.signal_Storage[i].message_Label );
            }

            MEM_Free( data.signal_Storage );
        }

        if ( data.nest_Path )
            MEM_Free( data.nest_Path );

        if ( data.msc_LabelCount )
        {
            for ( i = 0; i < data.msc_LabelCount; i++ )
            {
                MEM_Free( data.msc_LabelDeclPath[i].path );
            }
            MEM_Free( data.msc_LabelDeclPath );
        }

        if ( data.coregion_State )
            MEM_Free( data.coregion_State );

        if ( data.timers )
            MEM_Free( data.timers );

        if ( data.instance_State )
            MEM_Free( data.instance_State );

        if ( !root->type )
        {
            MEM_Free( root );
            MSC->start = NULL;
        }
        else
        {
            MSC->start = root;
            MSC_RemoveTailEvents( root );
        }

        if ( data.source_Buffer )
            MEM_Free( data.source_Buffer );

        /*MSC->source_Format = 0;*/

        if ( result != RESULT_ERR )
        {
            MSC_Transform( MSC, 0 );

            for ( i = 0; ( UINT32_T ) i < MSC->label_Count; i++ )
            {
                if ( MSC->labels[i].unused )
                {
                /* remove label definition */

                    if ( MSC->labels[i].s )
                        MSC->labels[i].s->labeled = 0;

                    MSC->labels[i].s = NULL;
                    MSC->labels[i].label = 0;
                }
            }
        }

        MSC_Load_Invoked--;

        if ( result == RESULT_ERR )
        {
            MSC_FreeMemory( MSC );
        }
        else
        {
        /* correct current level source information */
            MSC->src_Files[MSC->src_Count - 1].first_Line = ( MSC->start ) ? MSC->start->line_Number : 0;
            MSC->src_Files[MSC->src_Count - 1].last_Line = MSC_GetLastSourceLine( MSC );
        }

        MSG_PopProcessedFile(  );
        return result;
    }
    else
    {
        MSC_FreeMemory( MSC );
    }

    if ( data.source_Buffer )
    {
        MEM_Free( data.source_Buffer );
    }

    MSC_Load_Invoked--;
    MSG_PopProcessedFile(  );
    return result;
}
