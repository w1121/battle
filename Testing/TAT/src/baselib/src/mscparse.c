/******************************************************************************
*
*       COPYRIGHT 2001-2004 MOTOROLA, ALL RIGHTS RESERVED
*
*       MOTOROLA CONFIDENTIAL PROPRIETARY
*
* Description:  Command-line processing
*
* $RCSfile: mscparse.c,v $
* $Revision: 1.3 $
* $Source: /home/cvsusr/cvssoft/tat_tools/baselib/src/mscparse.c,v $
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
#include <qmemory.h>
#include <strproc.h>
#include <outmess.h>
#include <mscmess.h>
#include <mscstrs.h>
#include <tatgdata.h>
#include <tatcalc.h>

static char             buf[TAT_MAX_LINE_LEN];
static int              flag_LoopEvent = 0;

static LOGICAL msc_ParseReferenceParams( char * *pos , MSC_EVENT_KIND * event, MSC_STORAGE * M );

/***************************************************************************
 * Function : MSC_CheckName
 *
 * Description: check name for compliance
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_CheckName( const char * s )
{
    return ( stricmp( s, STR_CONDITION ) &&
        stricmp( s, STR_REFERENCE ) &&
        stricmp( s, STR_ACTION ) &&
        stricmp( s, STR_INSTANCE ) &&
        stricmp( s, STR_ENDINSTANCE ) &&
        stricmp( s, STR_BEGIN ) &&
        stricmp( s, STR_CONCURRENT ) &&
        stricmp( s, STR_ENDCONCURRENT ) &&
        stricmp( s, STR_IN ) &&
        stricmp( s, STR_INF ) &&
        stricmp( s, STR_OUT ) &&
        stricmp( s, STR_FROM ) &&
        stricmp( s, STR_TO ) &&
        stricmp( s, STR_COMMENT ) &&
        stricmp( s, STR_ALT ) &&
        stricmp( s, STR_PAR ) &&
        stricmp( s, STR_OPT ) &&
        stricmp( s, STR_EXC ) &&
        stricmp( s, STR_LOOP ) &&
        stricmp( s, STR_SET ) &&
        stricmp( s, STR_RESET ) &&
        stricmp( s, STR_TIMEOUT ) &&
        stricmp( s, STR_AS ) &&
        stricmp( s, STR_ALL ) &&
        stricmp( s, STR_STOP ) &&
        stricmp( s, STR_CREATE ) &&
        stricmp( s, STR_DECOMPOSED ) &&
        stricmp( s, MSC_LOST_INSTANCE_NAME ) &&
        stricmp( s, MSC_FOUND_INSTANCE_NAME ) && stricmp( s, STR_ENDMSC ) );
}

/***************************************************************************
 * Function : MSC_ParseCOND
 *
 * Description: parse condition
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_ParseCOND( char * * line, MSC_STORAGE * M, MSC_EVENT_KIND * event )
{
    char                    *p, *end;
    size_t                  i;

    p = *line;

    if ( *p == '[' )
    {
        end = StrFindCloseBraceEx( p, STR_USE_PARENTHESIS | STR_USE_SQUARE_BRACKET );

        if ( end == NULL || ( end - p < 2 ) )
        {
            MSG_SyntaxError(  );
            return RESULT_ERR;
        }

        i = end - p;
        strncpy( buf, p + 1, i - 2 );
        buf[i - 2] = 0;

        StrTrimSpaces( buf );

        if ( i <= 2 )
        {
            event->condition = 0;
        }
        else
        {
            event->condition = MSC_AddStr( buf, M );
        }

        *line = end;

    }
    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_ParseRelTime
 *
 * Description: parse relative time specification
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_ParseRelTime( char * *d, MSC_STORAGE * M, MSC_TIME_INTERVAL * time )
{
    char * s, * save_s;
    char buffer[TAT_MAX_LINE_LEN];
    int i;
    double time_val;

    s = *d;

    i = 0;
    while (*s && !(*s=='.' && *(s+1)=='.') && *s!='}')
        buffer[i++]=*s++;
    buffer[i]=0;
    StrTrimSpaces(buffer);
    i = stricmp(buffer, STR_INF);

    /* check the syntax of expression */
    if (!buffer[0] || (i && !MSC_CalcMacro(buffer)))
    {
        MSG_SyntaxError();
        return RESULT_ERR;
    }
    if (!i)
    {
        time->min_Time = MSC_INFINITY_CONST;
    }
    /* try to calculate value */
    else if (MSC_Calc(buffer, &time_val))
    {
        if (time_val<0)
        {
            MSG_SemanticError(MSG_INVALID_TIME_EXPR);
            return RESULT_ERR;
        }
        if (time_val>MSC_INFINITY_CONST)
        {
            MSG_SemanticWarning(MSG_VALUE_TOO_LARGE);
        }
        if (!StrIsNumber(buffer))
        {
            MSC_Time2Str(time_val, buffer);
        }
        time->min_Time = time_val;
    }
    else
    {
        time->min_Time = 0;
        time->valid_Number = 0;
    }

    time->str_MinTime = MSC_AddStr(buffer, M);
    STR_SKIP_SPACES(s);
    if (*s == '}')
    {
        time->max_Time = time->min_Time;
        time->str_MaxTime = time->str_MinTime;
    }
    else if (*s=='.' && *(s+1)=='.')
    {
        s+=2;
        STR_SKIP_SPACES(s);

        i = 0;
        /* patch for supporting label+expr..label+expr syntax*/
        if (time->type == MSC_LABELED_TIME && SymIsAlpha(*s))
        {
            save_s = s;
            if (!StrExtractLabel(&s, buffer))
            {
                MSG_SyntaxError();
                return RESULT_ERR;
            }
            STR_SKIP_SPACES(s);
            /* work around with INF */
            if (stricmp(buffer, STR_INF))
            {
                if (time->time_Label != (UINT32_T)MSC_GetTimeLabelID( buffer, M ) || *s!='+')
                {
                    MSG_SyntaxError();
                    return RESULT_ERR;
                };
                s++;
                STR_SKIP_SPACES(s);
            }
            else
                s = save_s;
        }

        while (*s && !(*s=='.' && *(s+1)=='.') && *s!='}')
            buffer[i++]=*s++;
        buffer[i]=0;
        StrTrimSpaces(buffer);
        i = stricmp(buffer, STR_INF);
        if (!buffer[0] || (i && !MSC_CalcMacro(buffer)))
        {
            MSG_SyntaxError();
            return RESULT_ERR;
        }
        if (!i)
        {
            time->max_Time = MSC_INFINITY_CONST;
        }
        else if (MSC_Calc(buffer, &time_val))
        {
            if (time_val<0)
            {
                MSG_SemanticError(MSG_INVALID_TIME_EXPR);
                return RESULT_ERR;
            }
            if (time_val>MSC_INFINITY_CONST)
            {
                MSG_SemanticWarning(MSG_VALUE_TOO_LARGE);
            }
            if (!StrIsNumber(buffer))
            {
                MSC_Time2Str(time_val, buffer);
            }
            time->max_Time = time_val;
        }
        else
        {
            time->max_Time = 0;
            time->valid_Number = 0;
        }
        
        time->str_MaxTime = MSC_AddStr(buffer, M);
        STR_SKIP_SPACES(s);
    }

    /* if finish time is bigger than start time, swap it */
    if ( time->valid_Number && time->max_Time < time->min_Time )
    {
        STRING_ID              x;
        
        time_val = time->min_Time;
        time->min_Time = time->max_Time;
        time->max_Time = time_val;
        x = time->str_MinTime;
        time->str_MinTime = time->str_MaxTime;
        time->str_MaxTime = x;
    }
    
    *d = s;
    time->specified = 1;
    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_ParseTime
 *
 * Description: Parse time definition
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_ParseTime( char * *d, MSC_STORAGE * M, MSC_TIME_INTERVAL * time )
{
    char *                 s;
    char                    buffer[TAT_MAX_ID_LEN];
    int                     i;

    if ( !d || !M || !time )
    {
        return RESULT_ERR;
    }
    s = *d;

    STR_SKIP_SPACES( s );

    if ( *s != '{' )
        return RESULT_OK;          /* it is not time specification */

    s++;

    STR_SKIP_SPACES( s );

    i = 0;

    time->valid_Number = 1;
/* extract relation to Label */
    time->str_MaxTime = STRCOLL_STR_ID_0;
    time->type = MSC_RELATIVE_TIME;
    
    if (*s == '@') /* <labeled time spec> */
    {
        s++;
        time->type = MSC_LABELED_TIME;
        time->time_Label = 0; /* MSC_START by default */
        i = 1; /* flag for relative time parser */
        if (SymIsAlpha(*s)) /* <label name> */
        {
            if (!StrExtractLabel(&s, buffer) )
            {
                MSG_SyntaxError();
                return RESULT_ERR;
            }
            time->time_Label = MSC_GetTimeLabelID( buffer, M );
            
            if ( ( int ) time->time_Label < 0 )
            {
                MSG_SemanticError( MSG_UNKNOWN_LABEL );
                return RESULT_ERR;
            }
            STR_SKIP_SPACES(s);
            i = 0;
            if (*s && *s=='+')
            {
                s++;
                STR_SKIP_SPACES(s);
                /* time specification shall be here */
                i = 1;
            }
            else
            {
                time->str_MinTime = STRCOLL_STR_ID_0;
                time->str_MaxTime = STRCOLL_STR_ID_INF;
                time->max_Time = MSC_INFINITY_CONST;
                time->min_Time = 0;
            }
        }
        if (i)  /* parse time expression */
        {
            if (!tat_ParseRelTime(&s, M, time))
            {
                return RESULT_ERR;
            }
        }
        /* mark label as used */
        M->labels[time->time_Label].unused = 0;
    }
    else
    {
        /* <relative time spec> */
        if (!tat_ParseRelTime(&s, M, time))
        {
            return RESULT_ERR;
        }
    }

    STR_SKIP_SPACES(s);

    /* time specification shall finish with '}' */
    if ( *s != '}' )
    {
        MSG_SyntaxError(  );
        return RESULT_ERR;
    }
    
    *d = s + 1;
    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_ParseEventSpec
 *
 * Description: Parse additional event specifications
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_ParseEventSpec( char **line, MSC_STORAGE * M, MSC_EVENT_KIND * event )
{
    char                   *p, dest[TAT_MAX_LINE_LEN], *xline;
    int                     sl = 0;

    p = *line;
    STR_SKIP_SPACES( p );
    xline = p;

    if ( *p == '/' && *( p + 1 ) == '*' && *(p+2)!='<' )
    {
        if ( !StrParseCComment( &p, dest ) )
        {
            MSG_SyntaxError(  );
            return RESULT_ERR;
        }
        STR_SKIP_SPACES( p );

        *line = p;

        p = &dest[0];

        STR_SKIP_SPACES( p );

        if ( *p != '~' && *p != '[' && *p != '{' && *p != 0 )
        {
            if ( !flag_LoopEvent )
            {
                MSG_SyntaxError(  );
                return RESULT_ERR;
            }
            else
            {
                *line = xline;
                return RESULT_OK;
            }
        }
    }
    else
        sl = 1;

/* parse synchronization */
    event->sync = MSC_ASYNC_EVENT;

    if ( *p == '~' )
    {
        event->sync = MSC_SYNC_EVENT;
        p++;
        StrSkipComments( &p );
    }

/* parse condition */
    if ( *p == '[' )
    {
        if ( !MSC_ParseCOND( &p, M, event ) )
            return RESULT_ERR;

        StrSkipComments( &p );
    }

/* parse time */
    if ( *p == '{' )
    {
        if ( !MSC_ParseTime( &p, M, &event->time ) )
            return RESULT_ERR;

        event->time_Calc = event->time; /* copy by default */

        StrSkipComments( &p );
    }

/* check for extra specification */
    if ( *p && ( !sl || ( ( *p == '~' ) || ( *p == '{' ) || ( *p == '[' ) ) ) )
    {
        MSG_SyntaxError(  );
        return RESULT_ERR;
    }

    if ( sl )
        *line = p;

    return RESULT_OK;
}


/***************************************************************************
 * Function : tat_ParseEvMark
 *
 * Description: parse event mark specificator
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_ParseEvMark( char * * s, MSC_EVENT_KIND * event, MSC_STORAGE * M )
{
    char                    buf[TAT_MAX_ID_LEN];
    char *                 p;

    p = *s;
    if ( *p == '<' )
    {
        p++;
        STR_SKIP_SPACES( p );
        if ( !StrExtractDigit( &p, buf ) )
        {
            return RESULT_ERR;
        }
        STR_SKIP_SPACES( p );
        if ( *p != '>' )
        {
            return RESULT_ERR;
        }
        p++;
        event->event_Mark = atoi( buf );
        *s = p;
        return RESULT_OK;
    }
    return RESULT_ERR;
}

char *                 VRS_EventVerdictText[] = {
    "Absolute time:",
    "Overlapped with absolute time:",
    "Annotation proved",
    "Annotation not proved",
    "Annotation inconclusive",
    "Protocols collision",
    "Protocol",
    "No protocols"
};

#define MSC_VERDICT_COUNT sizeof(VRS_EventVerdictText)/sizeof(VRS_EventVerdictText[0])

/***************************************************************************
 * Function : tat_ParseVerdict
 *
 * Description: parse verdict specification
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_ParseVerdict( char * * s, MSC_EVENT_KIND * event, MSC_STORAGE * M )
{
    char                    protocol_name[TAT_MAX_ID_LEN];
    char                    protocol_name2[TAT_MAX_ID_LEN];
    char                    buf[TAT_MAX_ID_LEN];

    char *                 p;
    MSC_VRS_VERDICT_TYPE    type = MSC_VRS_NO_VERDICT;
    UINT32_T                    i, p_id=0, p_id2=0, trace, event_num;
    MSC_TIME_INTERVAL  time_VRS;
    size_t                  incr;

    protocol_name[0] = 0;
    protocol_name2[0] = 0;

    p = *s;
    if ( *p == '<' && *( p + 1 ) == '!' )
    {
        p += 2;
        STR_SKIP_SPACES( p );

        while (*p == '!') p++;
        
        STR_SKIP_SPACES( p ); 

    /* look for correct verdict text */
        for ( i = 0; i < MSC_VERDICT_COUNT; i++ )
        {
            incr = strlen( VRS_EventVerdictText[i] );
            if ( !strncmp( p, VRS_EventVerdictText[i], incr ) )
            {
                type = i + 1;
                p += incr;
                STR_SKIP_SPACES( p );
                break;
            }
        }
    /* if not found */
        if ( i == MSC_VERDICT_COUNT )
        {
            return RESULT_ERR;
        }

        if ( type == MSC_VRS_PROTOCOL_APPLIED || type == MSC_VRS_PROTOCOL_COLLISION )
        {
            if ( !StrExtractLabel( &p, protocol_name ) )
            {
                return RESULT_ERR;
            }
            STR_SKIP_SPACES( p );
            if ( !StrExtractDigit( &p, buf ) )
            {
                return RESULT_ERR;
            }
            p_id = atoi( buf );
            STR_SKIP_SPACES( p );
        /* parse second protocol name, if required */
            if ( type == MSC_VRS_PROTOCOL_COLLISION )
            {
                if ( *p != ',' )
                {
                    return RESULT_ERR;
                }
                p++;
                STR_SKIP_SPACES( p );
                if ( !StrExtractLabel( &p, protocol_name2 ) )
                {
                    return RESULT_ERR;
                }
                STR_SKIP_SPACES( p );
                if ( !StrExtractDigit( &p, buf ) )
                {
                    return RESULT_ERR;
                }
                p_id2 = atoi( buf );
                STR_SKIP_SPACES( p );
            }
        }
    /* parse Trace Number, Event number */
        if ( *p != '(' )
        {
            return RESULT_ERR;
        }
        p++;
        STR_SKIP_SPACES( p );
        if ( strncmp( p, "Trace", 5 ) )
        {
            return RESULT_ERR;
        }
        p += 5;
        if ( !StrExtractDigit( &p, buf ) )
        {
            return RESULT_ERR;
        }
        trace = atoi( buf );
        STR_SKIP_SPACES( p );
        if ( *p != ',' )
        {
            return RESULT_ERR;
        }
        p++;
        STR_SKIP_SPACES( p );
        if ( strncmp( p, "Event", 5 ) )
        {
            return RESULT_ERR;
        }
        p += 5;
        if ( !StrExtractDigit( &p, buf ) )
        {
            return RESULT_ERR;
        }
        event_num = atoi( buf );
        STR_SKIP_SPACES( p );
        if ( *p != ')' )
        {
            return RESULT_ERR;
        }
        p++;
        STR_SKIP_SPACES( p );
        if ( *p == '{' )
        {
            if ( !MSC_ParseTime( &p, M, &time_VRS ) )
                return RESULT_ERR;
        }
        STR_SKIP_SPACES( p );
        if ( *p != '!' || *( p + 1 ) != '>' )
        {
            return RESULT_ERR;
        }
        p += 2;
        *s = p;
        event->verdict =
            MEM_Reallocate( event->verdict, ( event->verdict_Count + 1 ) * sizeof ( event->verdict[0] ) );
        event->verdict[event->verdict_Count].event = event_num;
        event->verdict[event->verdict_Count].trace = trace;
        event->verdict[event->verdict_Count].type = type;
        event->verdict[event->verdict_Count].protocol_Name = MSC_AddStr( protocol_name, M );
        event->verdict[event->verdict_Count].protocol_Name2 = MSC_AddStr( protocol_name2, M );
        event->verdict[event->verdict_Count].protocol_Num = p_id;
        event->verdict[event->verdict_Count].protocol_Num2 = p_id2;
        event->verdict[event->verdict_Count].time_VRS = time_VRS;
        event->verdict_Count++;
        return RESULT_OK;
    }
    return RESULT_ERR;
}

int GlobalParseCommentFlag = 1;

/***************************************************************************
 * Function : tat_ParseComment
 *
 * Description: parse comment specifications of events
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_ParseComment( char * s, MSC_EVENT_KIND * event, MSC_STORAGE * M )
{
    char                    buf[TAT_MAX_LINE_LEN];
    UINT32_T                    i;

    if (event)
    {
        if(GlobalParseCommentFlag) {
        /* support for extra event specifications */
        if (*s == '/' && *(s+1)=='*' && *(s+2)!='<' )
        {
            if (!MSC_ParseEventSpec(&s, M, event))
            {
                return RESULT_ERR;
            }
            STR_SKIP_SPACES(s);

        }
        if (*s == '/' && *(s+1)=='*' && *(s+2)!='<' )
        {
            /* support external specification of event properties */
            switch (event->type)
            {
            case MSC_EV_IN_MESSAGE:
            case MSC_EV_OUT_MESSAGE:
                {

                    break;
                }
            case MSC_EV_REFERENCE:
                {
                    if (!msc_ParseReferenceParams(&s, event, M))
                    {
                        return RESULT_ERR;
                    }
                    break;
                }
            case MSC_EV_CONDITION:
                {
                    if (StrParseCComment( &s, buf ))
                    {
                        event->data.cond.call = MSC_AddStr( buf, M );
                    }
                    else
                        return RESULT_ERR;
                    break;
                }
            }
        }

        STR_SKIP_SPACES(s);

        /* check for time label */
        if ( *s == '@' )
        {
            s++;
            if ( !StrExtractLabel( &s, buf ) )
            {
                MSG_SyntaxError(  );
                return RESULT_ERR;
            }

            if ( !MSC_CheckName( buf ) )
            {
                MSG_SemanticError( MSG_INCORRECT_LABEL_DEF );
                return RESULT_ERR;
            }

            if ( MSC_GetTimeLabelID( buf, M ) >= 0 )
            {
                MSG_SemanticError( MSG_DUP_LABEL_DEF );
                return RESULT_ERR;
            }

            M->labels = MEM_Reallocate( M->labels, ( M->label_Count + 1 ) * sizeof ( MSC_EVENT_KIND_LABEL ) );
            M->labels[M->label_Count].s = event;
            M->labels[M->label_Count].label = MSC_AddStr( buf, M );

            /* used, by default */
            M->labels[M->label_Count].unused = 0;
            /* label was not automatically generated */
            M->labels[M->label_Count].generated = 0;
            event->labeled = M->label_Count + 1;    /* save label */
            M->label_Count++;
            STR_SKIP_SPACES( s );
        }
        }

        i = 0;
        while ( *s )
        {
            /* look for special blocks in comment */
            while ( *s && *s != '<' )
            {
                buf[i++] = *s++;
            }

            if ( *s )
            {
                if (GlobalParseCommentFlag) {
                if ( *( s + 1 ) == '!' && !tat_ParseVerdict( &s, event, M ) )
                {
                    MSG_SyntaxError(  );
                    return RESULT_ERR;
                }
                else if ( tat_ParseEvMark( &s, event, M ) )
                {
                    /* first shall be mark or verdict. If verdict, mark is prohibited further */
                    break;
                }
                }                
                buf[i++] = *s++;
            }
        }

        buf[i] = 0;
        event->comment = MSC_AddStr( buf, M );
        if (GlobalParseCommentFlag) {
        while ( *s )
        {
            STR_SKIP_SPACES( s );
            if ( !tat_ParseVerdict( &s, event, M ) )
            {
                MSG_SyntaxError(  );
                return RESULT_ERR;
            }
        }
        }
    }

    return RESULT_OK;
}

/***************************************************************************
* Function : MSC_ParseSourceRef
*
* Description: parse source references
*
* Returns: RESULT_ERR/RESULT_OK
*
* Notes: None
*
**************************************************************************/
LOGICAL MSC_ParseSourceRef( char *line, MSC_EVENT_KIND * event, MSC_STORAGE * M )
{
    char                    temp[TAT_MAX_LINE_LEN];
    size_t                  link_Index, line_Number;

    if ( !line )
    {
        return RESULT_ERR;
    }

    StrSkipMSCComments( &line );

    /* parse MSC comment statement */
    if ( !strnicmp(line, "comment", 7 ) && SymIsDelim(line[7]))
    {
        line += 7;
        StrSkipComments( &line );
        
        if ( !StrExtractMSCString( &line, temp ) )
        {
            MSG_SyntaxError(  );
            return RESULT_ERR;
        }
        
        if (!tat_ParseComment( temp, event, M ))
        {
            return RESULT_ERR;
        }

        StrSkipMSCComments( &line );
    }

    if (event)
    {
        if ( (M->source_Format==MSC_MprFormatID && *line!=';') || (M->source_Format==MSC_FGFormatID && *line!=0) )
        {
            if ( *line != '/' || *( line + 1 ) != '*' || *( line + 2 ) != '[' )
            {
                MSG_SyntaxError(  );
                return RESULT_ERR;
            }
            if ( !StrParseCComment( &line, temp ) )
            {
                MSG_SyntaxError(  );
                return RESULT_ERR;
            }
            /* check the rest part of line for extra characters */
            StrSkipComments( &line );
            if ( *line!=';' )
            {
                MSG_SyntaxError(  );
                return RESULT_ERR;
            }
            line = &temp[0];
            STR_SKIP_SPACES( line );
            while ( *line )
            {
                /* assume: [link,line][link,line] */
                if ( *line != '[' )
                {
                    MSG_SyntaxError(  );
                    return RESULT_ERR;
                }
                line++;
                STR_SKIP_SPACES( line );
                link_Index = atoi( line );
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
                line_Number = atoi( line );
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

                if ( link_Index >= M->src_Count || line_Number < M->src_Files[link_Index].first_Line ||
                    line_Number > M->src_Files[link_Index].last_Line )
                {
                    MSG_SemanticError( MSG_INVALID_SOURCEREF );
                    return RESULT_ERR;
                }

                event->src_Data =
                    MEM_Reallocate( event->src_Data, ( event->src_Count + 1 ) * sizeof ( event->src_Data[0] ) );
                event->src_Data[event->src_Count].line_Number = line_Number;
                event->src_Data[event->src_Count].link_Index = link_Index;
                event->src_Count++;
            }

        }

        /* add link to last level */
        event->src_Data =
            MEM_Reallocate( event->src_Data, ( event->src_Count + 1 ) * sizeof ( event->src_Data[0] ) );
        event->src_Data[event->src_Count].line_Number = event->line_Number;
        event->src_Data[event->src_Count].link_Index = M->src_Count - 1;
        event->src_Count++;
    }
    else
    {
        if ((((M && M->source_Format==MSC_MprFormatID) || !M) && *line!=';') || ( M && M->source_Format==MSC_FGFormatID && *line!=0))
        {
            MSG_SyntaxError(  );
            return RESULT_ERR;
        }
    }

    return RESULT_OK;
}


/***************************************************************************
 * Function : tat_SortInstance
 *
 * Description: routine for sorting
 *
 * Returns: sort instances
 *
 * Notes: None
 *
 **************************************************************************/
static int tat_SortInstance( const void *x1, const void *x2 )
{
    return ( int ) *( ( MSC_INSTANCE_ID_TYPE * ) x1 ) - ( int ) *( ( MSC_INSTANCE_ID_TYPE * ) x2 );
}

/***************************************************************************
 * Function : MSC_ParseInstances
 *
 * Description: extract instance names delimited with commas and put their
 *              numbers int the array (x).
 *              if 'all' specified, only active instances added x
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_ParseInstances( char * s, size_t *icount, MSC_INSTANCE_ID_TYPE ** x,
                               MSC_STORAGE * M, MSC_LOAD_DATA * data )
{
    size_t                  i, j, count;
    char                   *str;

/* empty strings */
    if ( !s )
        return RESULT_ERR;

    str = s;
    StrSkipComments( &s );

    /* clean for reliability */
    *x = NULL;                  
    if ( s[0] == 0 )
    {   
        *icount = 0;
        return RESULT_OK;
    }

    count = 0;

    while ( *s )
    {
        if ( !StrExtractMacroID( &s, buf ) || buf[0] == 0 )
        {
            MSG_SyntaxError(  );
            return RESULT_ERR;
        }

        if ( !stricmp( buf, STR_ALL ) )
        {
            *x = MEM_Reallocate( *x, (M->instance_Count+count) * sizeof ( MSC_INSTANCE_ID_TYPE ) );
            for ( i = 0; i < M->instance_Count; i++ )
            {
                for ( j = 0; j < count; j++ )
                    if ( ( *x )[j] == ( UINT32_T ) i )
                        goto next_i;

                if (!data->instance_State[i].instance_Stopped &&
                    !data->instance_State[i].instance_Ended)
                {
                    ( *x )[count++] = i;
                }
next_i: ;
            }
        }
        else
        {
            int jj;
            jj = MSC_GetInstanceID( buf, M );
            if ( jj < 0 )
            {
                MSG_SemanticError( MSG_UNDECLARED_INSTANCE );
                return RESULT_ERR;
            }
            j = (size_t)jj;

            /* check against duplicate definition */

            for ( i = 0; i < count; i++ )
                if ( ( *x )[i] == ( MSC_INSTANCE_ID_TYPE ) j )
                    break;

            if ( i == count )
            {
                *x = MEM_Reallocate( *x, ( count + 1 ) * sizeof ( MSC_INSTANCE_ID_TYPE ) );
                ( *x )[count++] = ( MSC_INSTANCE_ID_TYPE ) j;
            }
        }

        StrSkipComments( &s );

        if ( *s == ',' )
        {
            s++;
        }
        else if ( *s == ':' || *s==';' )
        {
            break;
        }
        else
        {
            MSG_SyntaxError(  );
            return RESULT_ERR;
        }
    }

/* sort instance definitions */
    if ( ( count > 1 ) && *x )
    {
        qsort( *x, count, sizeof ( *x[0] ), tat_SortInstance );
    }
    *icount = count;
    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_ParseParams
 *
 * Description: extract parameters from string
 *
 * Returns: number of parameters/-1 if error
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_ParseParams( char * *p, size_t *pcount, char * **P )
{
    char *                 s;
    size_t                  count, acount, i;
    int                     state;
    char                    stack[TAT_MAX_ID_LEN];
    size_t                  stack_point = 0;
    char *                 *bf;
    size_t                  last_nsp;

    if ( !p || !pcount || !P )
        return -1;
    s = *p;
    if (!s)
        return -1;

    STR_SKIP_SPACES(s);

    /* default state */
    state = 0;

    /* handle case of parameters embrassed with braces */
    if (*s== '(' && *(s+1)!='.')
    {
        state = 1;
        s++;
    }
    else
        if (*s=='{')
        {
            state = 2;
            s++;
        }

    *p = s;

    /* pre-allocate buffer for message parameters */
    acount = 16;
    bf = ( char ** ) MEM_Allocate( acount * sizeof ( char * ) );
    /* allocate buffer for first parameter */
    bf[0] = MEM_Allocate(TAT_MAX_LINE_LEN);
    i = 0;
    count = 0; /* number of parameters */
    last_nsp = 0;
    /* analyse string */
    while ( *s )
    {
        if ( *s == '(' )
        {
            /* save value */
            bf[count][i++] = *s;
            /* push in stack */
            stack[stack_point]= *s++;
            
            if ( *s == '.' )
            {
                /* save value */
                bf[count][i++] = *s;
                stack[stack_point]= *s++;
            }
            stack_point++;
            last_nsp = i;
        }
        else if ( *s == ')' )
        {
            /* check, if it is final brace */
            if (!stack_point && (state==1))
            {
                /* last character is '\0', so we shan't explicitly end line */
                s++;
                if (bf[count][0]==0)
                {
                    MEM_Free(bf[count]);
                    bf[count]=NULL;
                }
                break;
            }
                
            /* check for correct stack filling */
            if (!stack_point || stack[--stack_point]!='(')
                goto error_ex;

            /* save value */
            bf[count][i++] = *s++;
            last_nsp = i;
        }
        else if ( *s == '{' )
        {
            /* save value */
            bf[count][i++] = *s;
            /* push in stack */
            stack[stack_point++]= *s++;
            last_nsp = i;
        }
        else if ( *s == '.' && *( s + 1 ) == ')' )
        {
            if (!stack_point || stack[--stack_point]!='.')
                goto error_ex;
            /* save value */
            bf[count][i++] = *s++;
            bf[count][i++] = *s++;
            last_nsp = i;
        }
        else if ( *s == '}')
        {
            /* check, if it is final brace */
            if (!stack_point && (state==2))
            {
                /* last character is '\0', so we shan't explicitly end line */
                s++;
                if (bf[count][0]==0)
                {
                    MEM_Free(bf[count]);
                    bf[count]=NULL;
                }
                break;
            }

            if (!stack_point || stack[--stack_point]!='{')
                goto error_ex;
            /* save value */
            bf[count][i++] = *s++;
            last_nsp = i;
        }
        else if ( *s == '\'' )
        {
            if ( !StrExtractString( &s, '\'', &bf[count][i] ) )
                goto error_ex;
            i+=strlen(&bf[count][i]);
            last_nsp = i;
        }
        else if ( *s == '"' )
        {
            if ( !StrExtractString( &s, '"', &bf[count][i] ) )
                goto error_ex;
            i+=strlen(&bf[count][i]);
            last_nsp = i;
        }
        else if ( *s == '/' && *( s + 1 ) == '*' )
        {
            bf[count][i++] = '/';
            bf[count][i++] = '*';
            if ( !StrParseCComment( &s, &bf[count][i] ) )
                goto error_ex;
            i+=strlen(&bf[count][i]);
            bf[count][i++] = '*';
            bf[count][i++] = '/';
            last_nsp = i;
        }
        else if ( *s == '*' && *( s + 1 ) == '/' )
        {
            goto error_ex;
        }
        else
        {
            if ( *s == ',' && stack_point==0 )
            {
                bf[count][last_nsp]=0;
                count++;
                if (count>=acount)
                {
                    acount+=8;
                    bf=MEM_Reallocate(bf, (acount*sizeof(bf[0])));
                }
                /* allocate memory for next item */
                if (bf[count-1][0]==0)
                {
                    bf[count]=bf[count-1];
                    bf[count-1]=NULL;
                }
                else
                    bf[count]=MEM_Allocate(TAT_MAX_LINE_LEN);

                s++; i=0;
                STR_SKIP_SPACES(s);
                last_nsp = 0;
            }
            else
            {
                /* update non-space pointer */
                if (!SymIsSpace(*s)) last_nsp=i+1;
                /* in any case, move to next character */
                bf[count][i++] = *s++;
            }

        }
    }
    if (bf[count])
        bf[count][last_nsp]=0;

    /* check for consistency */
    if ( stack_point )
    {
        goto error_ex;
    }

    *p = s;
    /* if no parameters */
    if ( count == 0 && (!bf[0] || bf[0][0]==0 ))
    {
        *pcount = 0;
        *P = NULL;
    }
    else
    {
        *pcount = count+1;
        *P = bf;
    }

    return RESULT_OK;

error_ex:
    for (i=0; i<=count; i++)
    {
        MEM_Free(bf[i]);
    }
    MEM_Free(bf);
    return RESULT_ERR;
}

/***************************************************************************
 * Function : MSC_ParseCreate
 *
 * Description: parse 'create' event
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_ParseCreate( char *line, MSC_EVENT_KIND * s, MSC_STORAGE * M )
{
    size_t                  i = 0, param_Count;
    char                    buffer[TAT_MAX_ID_LEN];
    char                  **P;
    int                     inst_c;

    if ( s->inst_Count == 0 )
    {
        MSG_SemanticError( MSG_EVENT_WO_INST );
        return RESULT_ERR;
    }

    if ( !StrExtractMacroID( &line, buffer ) || buffer[0] == 0 )
    {
        MSG_SyntaxError(  );
        return RESULT_ERR;
    }

    inst_c = MSC_GetInstanceID( buffer, M );

    if ( inst_c < 0 )
    {
        MSG_SemanticError( MSG_UNDECLARED_INSTANCE );
        return RESULT_ERR;
    }
    i = (size_t)inst_c;

    if ( i == s->instances[0] )
    {
        MSG_SemanticError( MSG_CANT_CREATE_SELF );
        return RESULT_ERR;
    }
/* set the destination instance */
    s->data.create.instance = ( MSC_INSTANCE_ID_TYPE ) i;

    StrSkipMSCComments( &line );

/* check for parameters */
    if ( *line == '(' )
    {
        if (!MSC_ParseParams( &line, &param_Count, &P ))
        {
            MSG_SyntaxError(  );
            return RESULT_ERR;
        }
        s->data.create.param_Count = param_Count;
        s->data.create.params = ( STRING_ID * ) MEM_Allocate( param_Count * sizeof ( STRING_ID ) );

        for ( i = 0; i < param_Count; i++ )
        {
            s->data.create.params[i] = MSC_AddStr( P[i], M );

            if ( P[i] )
                MEM_Free( P[i] );
        }
    }

/* check for source references */
    if ( !MSC_ParseSourceRef( line, s, M ) )
    {
        return RESULT_ERR;
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_ParseTimer
 *
 * Description: parse set, reset, timeout events.
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_ParseTimer( char *line, MSC_EVENT_KIND * s, MSC_STORAGE * M )
{
    size_t                  i = 0;
    char                   *t, *x;
    MSC_TIME_INTERVAL  temptime;

    if ( s->inst_Count == 0 )
    {
        MSG_SemanticError( MSG_EVENT_WO_INST );
        return RESULT_ERR;
    }

    while ( *line && *line != ',' )
        buf[i++] = *line++;

    buf[i] = 0;

    if ( i == 0 )
    {
        MSG_SyntaxError(  );
        return RESULT_ERR;
    }

    s->data.timer.signal_Name = MSC_AddStr( buf, M );

    if ( *line != ',' )
    {
        MSG_SyntaxError(  );
        return RESULT_ERR;
    }

    line++;

    StrSkipComments( &line );

    if ( !strnicmp( line, "Tmr", 3 ) )
    {
        line += 3;
    }

    s->data.timer.timer_ID = atoi( line );

    while ( SymIsDigit( *line ) )
        line++;

    t = &buf[0];

/* check for special timers */
    if ( buf[0] == '{' )
    {
        temptime.type = MSC_RELATIVE_TIME;

        if ( !MSC_ParseTime( &t, M, &temptime ) )
            return RESULT_ERR;

        if ( temptime.type == MSC_LABELED_TIME )
        {
            MSG_SemanticError( MSG_INVALID_TIME_EXPR );
            return RESULT_ERR;
        }
        StrSkipComments( &t );
        if ( *t )
        {
            MSG_SyntaxError(  );
            return RESULT_ERR;
        }
        s->data.timer.timing.str_MaxTime = temptime.str_MaxTime;
        s->data.timer.timing.str_MinTime = temptime.str_MinTime;
        s->data.timer.timing.min_Time = temptime.min_Time;
        s->data.timer.timing.max_Time = temptime.max_Time;
        s->data.timer.timing.type = MSC_SPECIAL_TIMER;
        if ( !MSC_ParseSourceRef( line, s, M ) )
        {
            return RESULT_ERR;
        }
        return RESULT_OK;
    }
    else if ( !StrCheckCName( buf ) || !MSC_CheckName( buf ) )
    {
        MSG_SemanticError( MSG_RESERVED_NAME );
        return RESULT_ERR;
    }

    if ( *line == '(' )
    {
    /* skip this parameter */
        line = StrFindCloseBrace( line );

        if ( !line )
        {
            MSG_SyntaxError(  );
            return RESULT_ERR;
        }
    }

    STR_SKIP_SPACES( line );

    if ( *line == '/' && *( line + 1 ) == '*' && *( line + 2 ) != '[' )
    {
    /* should check SDTMSCPAR */
        if ( !StrParseCComment( &line, t ) )
        {
        /* get comment into t */
            MSG_SyntaxError(  );
            return RESULT_ERR;
        };

        STR_SKIP_SPACES( t );

        if ( !strnicmp( t, "#SDTMSCPAR", 10 ) && SymIsDelim( t[10] ) )
        {
            t += 10;
            i = 0;
            STR_SKIP_SPACES( t );

        /* copy from temp to temp!!! */
            x = StrFindCloseBrace( t );

            if ( !x )
            {
                MSG_SyntaxError(  );
                return RESULT_ERR;
            }

            STR_SKIP_SPACES( x );
            if ( *x )
            {
                MSG_SyntaxError(  );
                return RESULT_ERR;
            }

            t++;

            i = strlen( t );

            if ( i == 0 )
            {
                MSG_SyntaxError(  );
                return RESULT_ERR;
            }

            if ( t[i - 1] != ')' )
            {
                MSG_SyntaxError(  );
                return RESULT_ERR;
            }

            t[i - 1] = 0;

            if ( !strnicmp( t, "now ", 4 ) || !strnicmp( t, "now+", 4 ) )
            {
                t += 3;
                STR_SKIP_SPACES( t );

                if ( *t != '+' )
                {
                    MSG_SyntaxError(  );
                    return RESULT_ERR;
                }

                t++;
                s->data.timer.timing.type = MSC_RELATIVE_TIMER;
            }
        }
        else
        {
            t[0] = 0;
        }

        s->data.timer.timing.str_MaxTime = MSC_AddStr( t, M );
        s->data.timer.timing.str_MinTime = s->data.timer.timing.str_MaxTime;
        s->data.timer.timing.min_Time = MSC_Str2Dbl( t );
        s->data.timer.timing.max_Time = s->data.timer.timing.min_Time;
    }

    if ( !MSC_ParseSourceRef( line, s, M ) )
    {
        return RESULT_ERR;
    }

/* set by default */
    if ( s->data.timer.timing.type == MSC_UNKNOWN_TIMER )
        s->data.timer.timing.type = MSC_ABSOLUTE_TIMER;

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_ParseRefSubst
 *
 * Description: Parse reference parameters
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_ParseRefSubst( char *s, char *d1, char *d2 )
{
    STR_SKIP_SPACES( s );

    if ( *s != '#' )
    {
        MSG_SyntaxError(  );
        return RESULT_ERR;
    }

    while ( *s && *s != '=' && !SymIsSpace( *s ) )
        *d1++ = *s++;

    *d1 = 0;

    STR_SKIP_SPACES( s );

    if ( *s != '=' )
    {
        MSG_SyntaxError(  );
        return RESULT_ERR;
    }

    s++;
    STR_SKIP_SPACES( s );
    strcpy( d2, s );
    return RESULT_OK;
}

/***************************************************************************
 * Function : msc_ParseReferenceParams
 *
 * Description: parse additional parameters of reference event
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL msc_ParseReferenceParams( char * *pos , MSC_EVENT_KIND * event, MSC_STORAGE * M )
{
    size_t                  i, param_Count;
    char                    **P, *line;
    char                    buffer[TAT_MAX_LINE_LEN], buffer1[TAT_MAX_LINE_LEN];

    line = *pos;
    
    /* check for reference parameters */
    if ( *line == '/' && *( line + 1 ) == '*' && *( line + 2 ) != '[' && *( line + 2 ) != '<' )
    {
        P = NULL;
        
        if ( !StrParseCComment( &line, buffer ) )
        {
            return RESULT_ERR;
        }
        *pos = line;
        line = &buffer[0];

        {
            if (!MSC_ParseParams( &line, &param_Count, &P ))
            {
                MSG_SyntaxError(  );
                return RESULT_ERR;
            }
            
            event->data.ref.param_Count = param_Count;
            
            if ( param_Count )
            {
                event->data.ref.macro_Name =
                    ( char ** ) MEM_Allocate( param_Count * sizeof ( event->data.ref.macro_Name[0] ) );
                event->data.ref.refr_Subst =
                    ( STRING_ID * ) MEM_Allocate( param_Count * sizeof ( event->data.ref.refr_Subst[0] ) );
            }
            
            for ( i = 0; i < param_Count; i++ )
            {
                if ( !P[i] || !tat_ParseRefSubst( P[i], buffer, buffer1 ) )
                {
                    if ( !P[i] )
                    {
                        MSG_SyntaxError(  );
                    }
                    MEM_FreeStrArray( param_Count, P );
                    return RESULT_ERR;
                }
                
                event->data.ref.macro_Name[i] = MEM_CopyStr( buffer );
                event->data.ref.refr_Subst[i] = MSC_AddStr( buffer1, M );
            }

            /* free allocated memory */
            if ( param_Count )
            {
                MEM_FreeStrArray( param_Count, P );
            }
            event->data.ref.ref_Type = TAT_REF_PARAM;
        }
    }
    return RESULT_OK;
}

LOGICAL tat_ParseAtomicRef(MSC_EVENT_KIND *event, MSC_STORAGE *M)
{
    size_t                  i, param_Count;
    char                    **P, *line;

        line = MSC_GetStr(event->comment, M);
            /* ptk macro */
        if (*line == '/' && *( line + 1 ) == '*'  && *( line + 2 ) == '<')
        {
            line+=3;
            STR_SKIP_SPACES(line);
            /* non - ptk macro */
            if (*line!='+')
                return RESULT_OK; 
            line++;
            STR_SKIP_SPACES(line);
            if (*line!='(')
            {
                MSG_SyntaxError();
                return RESULT_ERR;
            }
            if (!MSC_ParseParams( &line, &param_Count, &P ))
            {
                MSG_SyntaxError(  );
                return RESULT_ERR;
            }
            STR_SKIP_SPACES(line);
            if (*line!='>' || *(line+1)!='*' || *(line+2)!='/')
            {
                MSG_SyntaxError();
                return RESULT_ERR;
            }
            line+=3;
            STR_SKIP_SPACES(line);
            if (*line)
            {
                MSG_SyntaxError();
                return RESULT_ERR;
            }
            event->data.ref.ref_Type = PTK_REF_PARAM;

            if ( param_Count )
            {
                event->data.ref.macro_Name =
                    ( char ** ) MEM_Allocate( param_Count * sizeof ( event->data.ref.macro_Name[0] ) );
                event->data.ref.refr_Subst =
                    ( STRING_ID * ) MEM_Allocate( param_Count * sizeof ( event->data.ref.refr_Subst[0] ) );
            }

            for (i=0; i<param_Count; i++)
            {
                event->data.ref.refr_Subst[i] = MSC_AddStr(P[i], M);
            }

            event->data.ref.param_Count = param_Count;
            event->comment=0;
            /* free allocated memory */
            if ( param_Count )
            {
                MEM_FreeStrArray( param_Count, P );
            }

        }
    return RESULT_OK;
}
/***************************************************************************
* Function : MSC_ParseReference
*
* Description: **d is for future extensions t1 alt t2
*
* Returns: RESULT_ERR/RESULT_OK
*
* Notes: None
*
**************************************************************************/
LOGICAL MSC_ParseReference( char *line, MSC_EVENT_KIND * event, MSC_STORAGE * M )
{
    char                    buffer[TAT_MAX_LINE_LEN];
    
    
    if ( *line == '$' || *line=='+')
    {
        /* handle 'functional' (ptk:atomic) references */
        buffer[0] = '$';
        line++;
        if ( !StrExtractMacroID( &line, buffer + 1 ) || buffer[1] == 0 )
        {
            MSG_SyntaxError(  );
            return RESULT_ERR;
        }
    }
    else
    {
        
        if ( !StrExtractMacroID( &line, buffer ) || buffer[0] == 0 )
        {
            MSG_SyntaxError(  );
            return RESULT_ERR;
        }
    }
    
    /* check for reserved name */
    if ( !MSC_CheckName( buffer ) )
    {
        MSG_SemanticError( MSG_RESERVED_NAME );
        return RESULT_ERR;
    }
    
    event->data.ref.refr_Name = MSC_AddStr( buffer, M );

    STR_SKIP_SPACES( line );

    if (!msc_ParseReferenceParams(&line, event, M))
    {
        return RESULT_ERR;
    }

    if ( !MSC_ParseSourceRef( line, event, M ) )
    {
        return RESULT_ERR;
    }

    if (event->comment)
        if (!tat_ParseAtomicRef(event, M))
        {
            return RESULT_ERR;
        }

    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_ParseAction
 *
 * Description: parse 'action' event
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_ParseAction( char *line, MSC_EVENT_KIND * s, MSC_STORAGE * M )
{
    if ( s->inst_Count == 0 )
    {
        MSG_SemanticError( MSG_EVENT_WO_INST );
        return RESULT_ERR;
    }

    if ( !StrExtractMSCString( &line, buf ) )
    {
        MSG_SyntaxError(  );
        return RESULT_ERR;
    }

    if ( !MSC_ParseSourceRef( line, s, M ) )
    {
        return RESULT_ERR;
    }

    s->data.text = MSC_AddStr( buf, M );
    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_ParseCondition
 *
 * Description: parse 'condition' event
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_ParseCondition( char *line, MSC_EVENT_KIND * s, MSC_STORAGE * M, MSC_LOAD_DATA *data )
{
    MSC_INSTANCE_ID_TYPE   *x;
    size_t                   i, j, k;

    if ( !StrExtractMacroID( &line, buf ) || buf[0] == 0 )
    {
        MSG_SyntaxError(  );
        return RESULT_ERR;
    }
    /* parse ptk's 'when' statement */
    if (!stricmp(buf, "when"))
    {
        char *ls;
        STR_SKIP_SPACES(line);
        
        if (*line=='(')
        {
            ls = StrFindCloseBrace(line);
            if (!ls)
            {
                MSG_SyntaxError(  );
                return RESULT_ERR;
            }
            k = *ls;
            *ls = 0;
            strcat(buf, line);
            *ls = k;
            line = ls;
        }
    }
    else
    {
        if ( buf[0] == '^' || buf[0] == '_' )
        {
            if ( !MSC_CheckName( buf + 1 ) )
            {
                MSG_SemanticError( MSG_RESERVED_NAME );
                return RESULT_ERR;
            }
        }
        else if ( !MSC_CheckName( buf ) )
        {
            MSG_SemanticError( MSG_RESERVED_NAME );
            return RESULT_ERR;
        }
    }
    s->data.cond.text = MSC_AddStr( buf, M );
    STR_SKIP_SPACES( line );


/* parse only first occurence of parameters, ignores others */
    if ( *line == '/' && *( line + 1 ) == '*' && *( line + 2 ) != '[' && *(line+2)!='<' )
    {
        StrParseCComment( &line, buf );
        s->data.cond.call = MSC_AddStr( buf, M );
    }

    StrSkipMSCComments( &line );

    /* parse MSC'96 standard condition definition */
    if ( *line )
    {
        if ( !strnicmp( line, "shared ", 7 ) )
        {
            line += 7;
            STR_SKIP_SPACES( line );
            if (!MSC_ParseInstances( line, &k, &x, M, data ))
            {
                return RESULT_ERR;
            }

        /* merge instance lists */
            for ( j = 0; j < k; j++ )
            {
                for ( i = 0; i < s->inst_Count; i++ )
                {
                    if ( s->instances[i] == x[j] )
                        break;
                }

                if ( i == s->inst_Count )
                {
                    s->instances =
                        MEM_Reallocate( s->instances, ( s->inst_Count + 1 ) * sizeof ( MSC_INSTANCE_ID_TYPE ) );
                    s->instances[s->inst_Count++] = x[j];
                }
            }
        /* skip till comment, if any */
            while ( *line && *line != '/' && *line!=';' && *( line + 1 ) != '*' )
                line++;
        }
    }

/* process source references */
    if ( !MSC_ParseSourceRef( line, s, M ) )
    {
        return RESULT_ERR;
    }

    return ( s->data.cond.text ) ? RESULT_OK : RESULT_ERR;
}

/***************************************************************************
 * Function : MSC_CheckEventSpecification
 *
 * Description: check that all necessary fields are filled with correct
 *              information.
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_CheckEventSpecification( MSC_EVENT_KIND *event, MSC_STORAGE *M, MSC_LOAD_DATA * data)
{
    char * s;
    UINT32_T i;

    /* add label specification */
    if ( event->labeled )
    {
        /* handle MSC_START label */
        if ( event->labeled - 2 != data->msc_LabelCount )
        {
            MSG_PrintError( MSG_INTERNAL_PROBLEM );
        }
        
        data->msc_LabelDeclPath =
            MEM_Reallocate( data->msc_LabelDeclPath,
            ( data->msc_LabelCount + 1 ) * sizeof ( data->msc_LabelDeclPath[0] ) );
        data->msc_LabelDeclPath[data->msc_LabelCount].path_Length = data->nest_Level;
        data->msc_LabelDeclPath[data->msc_LabelCount].path =
            MEM_Copy( data->nest_Path, data->nest_Level * sizeof ( data->nest_Path[0] ) );
        data->msc_LabelCount++;
    }
    
    /* check if time label is used in line where defined */
    if ( event->labeled && event->time.type == MSC_LABELED_TIME
        && event->time.time_Label + 1 == event->labeled )
    {
        MSG_SemanticError( MSG_INVALID_TIME_LABEL_LOOP );
        return RESULT_ERR;
    }
    
    if ( ( event->time.type == MSC_LABELED_TIME ) && event->time.time_Label &&
        ( event->branch_ID != M->labels[event->time.time_Label].s->branch_ID ) )
    {
        if ( data->msc_LabelDeclPath[event->time.time_Label - 1].path_Length > data->nest_Level )
        {
            MSG_SemanticError( MSG_REFERENCE_TO_LABEL_LOOP );
            return RESULT_ERR;
        }
        /* compare path to label and path to current event */
        for ( i = 0; i < data->msc_LabelDeclPath[event->time.time_Label - 1].path_Length; i++ )
        {
            if ( data->msc_LabelDeclPath[event->time.time_Label - 1].path[i] != data->nest_Path[i] )
            {
                MSG_SemanticError( MSG_REFERENCE_TO_LABEL_LOOP );
                return RESULT_ERR;
            }
        }
        
        MSG_SemanticWarning( MSG_REFERENCE_TO_LABEL_LOOP );
    }
    
    switch(event->type)
    {
    case MSC_EV_CONDITION:
        {
            s= MSC_GetString( event->data.cond.text, M );

            if ( ( !stricmp( s, STR_WAIT ) ||
                !stricmp( s, STR_ASSERT ) ||
                !stricmp( s, STR_IF ) ||
                !stricmp( s, STR_WHILE ) ||
                !stricmp( s, STR_WHEN ) ||
                !stricmp( s, STR_ANNO ) ||
                !stricmp( s, STR_PRE ) ||
                !stricmp( s, STR_POST ) ) && !event->data.cond.call )
            {
                MSG_SemanticWarning( MSG_REQUIRES_EXPRESSION );
            }
            break;
        }
    default:
        {

        }
    }

    return RESULT_OK;
}
/***************************************************************************
 * Function : MSC_CheckOperator
 *
 * Description: see if line is inline expression control
 *
 * Returns: RESULT_OK/RESULT_ERR/MSC_NOT_END/MSC_REPEAT_ACTION
 *
 * Notes: None
 *
 **************************************************************************/
int MSC_CheckOperator( char *line, MSC_OPERATOR_TYPE Curr )
{
    register int            alt, opt, exc, par, loop, end;
    char                    buf[TAT_MAX_ID_LEN];

    alt = 0;
    opt = 0;
    exc = 0;
    par = 0;
    loop = 0;

    if ( !StrExtractMacroID( &line, buf ) || buf[0] == 0 )
    {
        return MSC_NOT_END;
    }

    if ( !stricmp( buf, STR_ALT ) )
    {
        alt = 1;
    }
    else if ( !stricmp( buf, STR_OPT ) )
    {
        opt = 1;
    }
    else if ( !stricmp( buf, STR_EXC ) )
    {
        exc = 1;
    }
    else if ( !stricmp( buf, STR_PAR ) )
    {
        par = 1;
    }
    else if ( !stricmp( buf, STR_LOOP ) )
    {
        loop = 1;
    }

    StrSkipComments( &line );

    end = !strnicmp( line, STR_END, STR_END_LEN ) &&
        ( line[STR_END_LEN] == 0 || 
          line[STR_END_LEN] == ';' || 
          SymIsSpace( line[STR_END_LEN] ) ||
          ( line[STR_END_LEN] == '/' ) );

    if ( end )
    {
        line += 3;
        StrSkipComments( &line );
        if ( *line!=';' )
        {
            MSG_SyntaxError(  );
            return RESULT_ERR;
        }
    }

    if ( alt && end )
    {
        if ( Curr != MSC_OP_ALT )
        {
            MSG_SemanticError( MSG_EXPR_WITHIN_ANOTHER );
            return RESULT_ERR;
        }

        return RESULT_OK;
    }

    if ( opt && end )
    {
        if ( Curr != MSC_OP_OPT )
        {
            MSG_SemanticError( MSG_EXPR_WITHIN_ANOTHER );
            return RESULT_ERR;
        }

        return RESULT_OK;
    }

    if ( exc && end )
    {
        if ( Curr != MSC_OP_EXC )
        {
            MSG_SemanticError( MSG_EXPR_WITHIN_ANOTHER );
            return RESULT_ERR;
        }

        return RESULT_OK;
    }

    if ( par && end )
    {
        if ( Curr != MSC_OP_PAR )
        {
            MSG_SemanticError( MSG_EXPR_WITHIN_ANOTHER );
            return RESULT_ERR;
        }

        return RESULT_OK;
    }

    if ( loop && end )
    {
        if ( Curr != MSC_OP_LOOP )
        {
            MSG_SemanticError( MSG_EXPR_WITHIN_ANOTHER );
            return RESULT_ERR;
        }

        return RESULT_OK;
    }

    if ( par )
    {
        if ( Curr != MSC_OP_PAR )
        {
            MSG_SemanticError( MSG_EXPR_WITHIN_ANOTHER );
            return RESULT_ERR;
        }
        
        if ( *line!=';' )
        {
            if ( !MSC_ParseSourceRef( line, NULL, NULL ) )
                return RESULT_ERR;
        }
        return MSC_REPEAT_ACTION;
    }

    if ( alt )
    {
        if ( Curr != MSC_OP_ALT )
        {
            MSG_SemanticError( MSG_EXPR_WITHIN_ANOTHER );
            return RESULT_ERR;
        }
        if ( *line!=';' )
        {
            if ( !MSC_ParseSourceRef( line, NULL, NULL ) )
                return RESULT_ERR;
        }
        return MSC_REPEAT_ACTION;
    }

    if ( opt || loop || exc )
    {
        MSG_SyntaxError(  );
        return RESULT_ERR;
    }

/* we should simply continue analysis of MSC */
    return MSC_NOT_END;
}

/***************************************************************************
 * Function : MSC_GetEventType
 *
 * Description: parse 'p' and identifies event type. Parse additional specs.
 *
 * Returns: MSC element type
 *
 * Notes: None
 *
 **************************************************************************/
MSC_EVENT_KIND_TYPE MSC_GetEventType( char **p, MSC_STORAGE * M, MSC_EVENT_KIND * event )
{
    static const char      *msc_event_names[] = {
        NULL, NULL, NULL, STR_REFERENCE, STR_CONDITION,
        STR_ACTION, STR_SET, STR_RESET, STR_TIMEOUT, STR_CONCURRENT,
        STR_ENDCONCURRENT, NULL, STR_IN, STR_OUT, STR_CREATE,
        STR_STOP, STR_SEND, STR_RECEIVE, STR_EXIST, NULL, NULL, NULL
    };

#define PTK_MSC_EV_MSCNAMES_COUNT sizeof(msc_event_names)/sizeof(msc_event_names[0])

    static const char      *msc_oper_names[] = {
        NULL, STR_ALT, STR_PAR, STR_LOOP, STR_OPT, STR_EXC
    };

#define PTK_MSC_EV_MSCOPNAMES_COUNT sizeof(msc_oper_names)/sizeof(msc_oper_names[0])

    size_t                    i;

    event->type = MSC_EV_UNDEFINED;

    if ( !StrExtractMacroID( p, buf ) || buf[0] == 0 )
    {
        MSG_SyntaxError(  );
        return MSC_EV_UNDEFINED;
    }

    STR_SKIP_SPACES( ( *p ) );

    for ( i = 0; i < PTK_MSC_EV_MSCNAMES_COUNT; i++ )
    {
        if ( msc_event_names[i] && !stricmp( msc_event_names[i], buf ) )
        {
            event->type = ( MSC_EVENT_KIND_TYPE ) i;
            break;
        }
    }

    switch ( event->type )
    {
    case MSC_EV_OUT_MESSAGE:
    case MSC_EV_IN_MESSAGE:
    case MSC_EV_REFERENCE:
    case MSC_EV_CONDITION:
    case MSC_EV_SEND_SIGNAL:
    case MSC_EV_RECV_SIGNAL:
    case MSC_EV_EXIST_SIGNAL:
    case MSC_EV_CREATE:
    case MSC_EV_STOP:
    case MSC_EV_ACTION:
    {
        if ( !MSC_ParseEventSpec( p, M, event ) )
        {
            event->type = MSC_EV_UNDEFINED;
            return MSC_EV_UNDEFINED;
        }
        break;
    }
    case MSC_EV_CONCURRENT_START:
    case MSC_EV_CONCURRENT_END:
    case MSC_EV_TIMER_SET:
    case MSC_EV_TIMER_RESET:
    case MSC_EV_TIMER_TIMEOUT:
        break;
    case MSC_EV_UNDEFINED:
    {
    /* it may be OPERATOR or undefined keyword */
        for ( i = 0; i < PTK_MSC_EV_MSCOPNAMES_COUNT; i++ )
        {
            if ( msc_oper_names[i] && !stricmp( msc_oper_names[i], buf ) )
            {
                event->type = MSC_EV_OPERATOR;
                event->data.alt.type = ( MSC_OPERATOR_TYPE ) i;
                MSC_ZeroEventTime( event );

                if ( event->data.alt.type == MSC_OP_LOOP )
                {
                    flag_LoopEvent = 1;
                }
                if ( !MSC_ParseEventSpec( p, M, event ) )
                {
                    event->type = MSC_EV_UNDEFINED;
                    flag_LoopEvent = 0;
                    return MSC_EV_UNDEFINED;
                }
                flag_LoopEvent = 0;
                break;
            }
        }

        if ( event->type == MSC_EV_UNDEFINED )
        {                       /* undefined keyword */
            MSG_SyntaxError(  );
            return MSC_EV_UNDEFINED;
        }

        switch ( event->data.alt.type )
        {
        case MSC_OP_PAR:
        case MSC_OP_ALT:
        case MSC_OP_EXC:
        case MSC_OP_OPT:
            break;

        case MSC_OP_LOOP:
        {
        /* special processing of loop */
            char                  **el, *end, *line;
            size_t                  i;

        /* set by default */
            event->data.alt.l1 = STRCOLL_STR_ID_1;
            event->data.alt.l2 = STRCOLL_STR_ID_INF;
            line = *p;
            i = MSC_INFINITY_CONST;

            if ( *line == '<' )
            {
                /* extract 'loop' parameters in angle brackets */
                end = StrFindCloseBraceEx( line, STR_USE_PARENTHESIS | STR_USE_ANGLE_BRACKET );

                if ( !end || *end == 0 || end - line < 2 )
                {
                    MSG_SyntaxError(  );
                    return MSC_EV_UNDEFINED;
                }

                strncpy( buf, line + 1, end - line - 2 );

                buf[end - line - 2] = 0;
                *p = end;
                i = 0;
            }
            /* parse special loop parameters in C comment box */
            else if ( *line == '/' && *( line + 1 ) == '*' )
            {
                if ( !StrParseCComment( &line, buf ) )
                {
                    MSG_SyntaxError(  );
                    return MSC_EV_UNDEFINED;
                }
                *p = line;
                i = 0;
            }

            if ( i == 0 )
            {
                line = &buf[0];

                /* if we have loop parameters */
                if (!MSC_ParseParams( &line, &i, &el ) || ( i > MAX_LOOP_PARAMETERS ) || i == 0 )
                {
                    MSG_SyntaxError(  );
                    return MSC_EV_UNDEFINED;
                }

                if ( !el[0] || el[0][0] == 0 )
                {
                    MSG_SyntaxError(  );
                    return MSC_EV_UNDEFINED;
                }

                if ( i == 2 && ( !el[1] || el[1][0] == 0 ) )
                {
                    MSG_SyntaxError(  );
                    return MSC_EV_UNDEFINED;
                }

            /* check for correct numbers */
                if ( StrIsNumber( el[0] ) )
                {
                    if ( !StrIsNaturalNumber( el[0] ) )
                    {
                        MSG_SemanticError( MSG_INVALID_COUNTER );
                        return MSC_EV_UNDEFINED;
                    }
                }
                else
                {
                /* first parameter shall not be 'inf' or anything except counter or macro */

                    if ( !stricmp( el[0], STR_INF ) || ( el[0][0] != '$' && el[0][0] != MSC_MACRO_START ) )
                    {
                        MSG_SemanticError( MSG_INVALID_COUNTER );
                        return MSC_EV_UNDEFINED;
                    }
                }

                event->data.alt.l1 = MSC_AddStr( el[0], M );

                if ( i == MAX_LOOP_PARAMETERS )
                {
                /* if 2 arguments are different */
                    event->data.alt.l2 = event->data.alt.l1;
                    if ( stricmp( el[0], el[1] ) )
                    {
                        if ( el[1][0] == 0 )
                        {
                            MSG_SyntaxError(  );
                            return MSC_EV_UNDEFINED;
                        }

                    /* check for correct numbers */
                        if ( StrIsNumber( el[1] ) )
                        {
                            if ( !StrIsNaturalNumber( el[1] ) )
                            {
                                MSG_SemanticError( MSG_INVALID_COUNTER );
                                return MSC_EV_UNDEFINED;
                            }
                        }
                        else
                        {
                        /* first parameter shall not be 'inf' or anything except counter or macro */
                            if ( ( !stricmp( el[1], STR_INF ) && el[0][0] == '$' )
                                || ( el[1][0] != MSC_MACRO_START && stricmp( el[1], STR_INF ) ) )
                            {
                                MSG_SemanticError( MSG_INVALID_COUNTER );
                                return MSC_EV_UNDEFINED;
                            }
                        }

                        event->data.alt.l2 = MSC_AddStr( el[1], M );
                    }
                    else
                    {           /* equal parameters */

                        if ( el[1][0] == '$' )
                        {
                            MSG_SemanticError( MSG_INVALID_COUNTER );
                            return MSC_EV_UNDEFINED;
                        }
                    }
                }
                else
                    event->data.alt.l2 = event->data.alt.l1;    /* second is equal to first */

                MEM_FreeStrArray( i, el );
                StrSkipComments( p );
            }                   /* end if */
        }                       /* case MSC_OP_LOOP: */
        }                       /* switch (event->data.alt.type) */
    }
    }

/* check for correct finish of operator statement */
    if ( event->type == MSC_EV_OPERATOR )
    {
        if ( !StrExtractMacroID( p, buf ) || stricmp( buf, STR_BEGIN ) )
        {
            MSG_SemanticError( MSG_MISSING_BEGIN );
            event->type = MSC_EV_UNDEFINED;
        }
        else
        {
            if ( !MSC_ParseSourceRef( *p, event, M ) )
            {
                return RESULT_ERR;
            }
        }
    }

    return ( event->type );
}
