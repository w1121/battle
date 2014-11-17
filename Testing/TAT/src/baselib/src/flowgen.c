/******************************************************************************
*
*       COPYRIGHT 2001-2004 MOTOROLA, ALL RIGHTS RESERVED
*
*       MOTOROLA CONFIDENTIAL PROPRIETARY
*
* Description:  Command-line processing
*
* $RCSfile: flowgen.c,v $
* $Revision: 1.3 $
* $Source: /home/cvsusr/cvssoft/tat_tools/baselib/src/flowgen.c,v $
* $Date: 2004/03/05 14:37:44 $
*
*
* CHANGE HISTORY
*                                                       
* Initial version 
*
******************************************************************************/

#include <portable.h>
#include <fileproc.h>
#include <mscbase.h>
#include <mscparse.h>
#include <strproc.h>
#include <outmess.h>
#include <qmemory.h>
#include <flowgen.h>
#include <mscmess.h>
#include <mscstrs.h>
#include <tatgdata.h>
#include <mscproc.h>

static UINT32_T             line_Counter;
static UINT32_T             indicator;
static UINT32_T             load_Comb = 1;

/***************************************************************************
 * Function : tat_LoadFlowgenLine
 *
 * Description: load text line from FlowGen file
 *
 * Returns: pointer to line
 *
 * Notes: none
 *
 **************************************************************************/
static char            *tat_LoadFlowgenLine( FILE * f, char *dest )
{
    char                   *line;
    size_t                  i;

    if ( !f )
        return NULL;

    if ( !dest )
        return NULL;

    if ( feof( f ) )
        return NULL;

    do
    {
        line = fgets( dest, TAT_MAX_LINE_LEN - 4, f );
        line_Counter++;
        MSG_SetLineNumber( line_Counter );
        if ( !line )
            return NULL;

        StrTrimSpaces( line );

        i = strlen( line );
    }
    while ( i == 0 || ( line[0] == '#' && line[1] == '#' ) );

    i--;

    while ( i > 0 && ( line[i] == CR || line[i] == LF ) )
    {
        line[i] = 0;
        i--;
    }

    return strcpy( dest, line );
}

/***************************************************************************
 * Function : tat_ParseFlowgenSignal
 *
 * Description: parse Flowgen signal and parameters
 *
 * Returns: pointer to storage
 *
 * Notes: None
 *
 **************************************************************************/
static MSC_MESSAGE_STORAGE *tat_ParseFlowgenSignal( char **sl, MSC_STORAGE * M, int indic )
{
    MSC_MESSAGE_STORAGE     x;
    char                    name[TAT_MAX_ID_LEN];
    char                   *s;
    size_t                  i, param_Count;

    char                  **P;

/* get signal name */

    s = *sl;

    if ( !StrExtractMacroID( &s, name ) || name[0] == 0 || !MSC_CheckName( name ) )
    {
        MSG_SemanticError( MSG_RESERVED_NAME );
        return NULL;
    }

    StrSkipMSCComments( &s );
    x.delay.str_MinTime = STRCOLL_STR_ID_0;
    x.delay.str_MaxTime = STRCOLL_STR_ID_0;
    x.delay.min_Time = 0.0;
    x.delay.max_Time = 0.0;
    x.delay.type = MSC_RELATIVE_TIME;

    P = NULL;
    if ( *s == '(' )
    {
        if (!MSC_ParseParams( &s, &param_Count, &P ))
        {
            MSG_SyntaxError(  );
            return NULL;
        }
        StrSkipMSCComments( &s );
    }
    else
    {
        param_Count = 0;
    }

    *sl = s;

    x.indicator = indic;
    x.message_Name = MSC_AddStr( name, M );
    x.param_Count = param_Count;
    x.line_Number = line_Counter;
    x.params_Values = NULL;

    if ( param_Count )
    {
        x.message_Params = ( STRING_ID * ) MEM_Allocate( param_Count * sizeof ( x.message_Params[0] ) );
        x.params_Values = MEM_Allocate( param_Count * sizeof ( x.params_Values[0] ) );

        for ( i = 0; i < param_Count; i++ )
        {
            x.message_Params[i] = MSC_AddStr( P[i], M );

            /* this is always single parameter */
            x.params_Values[i].varName = 0;
            x.params_Values[i].valueCount = 1;
            x.params_Values[i].values = MEM_Allocate(sizeof(x.params_Values[i].values[0]));
            x.params_Values[i].values[0] = x.message_Params[i];

            if ( P[i] )
                MEM_Free( P[i] );
        }

        MEM_Free( P );
    }

    return MSC_AddMessage( &x, M );
}

/***************************************************************************
 * Function : add_instance
 *
 * Description: Add new instance to test case
 *
 * Returns: instance number
 *
 * Notes: None
 *
 **************************************************************************/
static int tat_AddInstance( char *s, MSC_STORAGE * M )
{
    size_t                    i;

    i = MSC_GetInstanceID( s, M );

    if ( i == MSC_UNKNOWN_INSTANCE_ID )
    {

        M->instances =
            MEM_Reallocate( M->instances, ( M->instance_Count + 1 ) * sizeof ( MSC_INSTANCE_STORAGE ) );
        M->instances[M->instance_Count].name = MSC_AddStr( s, M );
        M->instances[M->instance_Count].type = 0;
        M->instances[M->instance_Count].ratio = STRCOLL_STR_ID_1;
        M->instances[M->instance_Count].decompose = 0;
        M->instances[M->instance_Count].comment = 0;
/*        M->instances[M->instance_Count].ind = MSC_INSTANCE_UNDEF; */
        i = M->instance_Count;
        M->instance_Count++;
    }

    return i;
}

/***************************************************************************
 * Function : tat_LoadFlowgenEvents
 *
 * Description: load Flowgen events
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_LoadFlowgenEvents( FILE * f, MSC_EVENT_KIND * s, MSC_STORAGE * M )
{
    char                    buffer[TAT_MAX_LINE_LEN];
    char                    temp[TAT_MAX_LINE_LEN];
    char                   *line;
    int                     i, inst1, inst2;

    if ( !f || !M || !s )
        return RESULT_ERR;

    while ( !feof( f ) )
    {
        line = tat_LoadFlowgenLine( f, buffer );

        if ( !line )
            break;

        line = &buffer[0];

    /* simple text */
        if ( line[0] == '#' && SymIsSpace( line[1] ) )
        {
            line++;
            STR_SKIP_SPACES( line );
            M->texts = MEM_Reallocate( M->texts, ( ++M->text_Count ) * sizeof ( STRING_ID ) );
            M->texts[M->text_Count - 1] = MSC_AddStr( line, M );
            continue;
        }

    /* code */
        if ( !strnicmp( line, "#CODE#", 6 ) )
        {
            s->type = MSC_EV_CODE;
            s->data.text = MSC_AddStr( line + 7, M );
            s->line_Number = line_Counter;
            s->next = MSC_AllocateEvent( M );

            if ( s->next == NULL )
                return RESULT_ERR;

            s = s->next;

            continue;
        }

    /* message exchange */
        i = 0;

    /* get first instance */
        while ( line && !SymIsSpace( *line ) )
            temp[i++] = *line++;

        temp[i] = 0;

        STR_SKIP_SPACES( line );

        if ( !StrCheckIDName( temp ) || ( temp[0] == 0 ) )
        {
            MSG_SyntaxError(  );
            return RESULT_ERR;
        }

        s->type = ( load_Comb ) ? MSC_EV_SEND_SIGNAL : MSC_EV_OUT_MESSAGE;
        inst1 = tat_AddInstance( temp, M );
        s->inst_Count = 1;
        s->instances = MEM_Allocate( sizeof ( MSC_INSTANCE_ID_TYPE ) );
        s->instances[0] = inst1;
        s->data.msg.indicator = indicator++;
        i = 0;
    /* get second instance */

        while ( line && !SymIsSpace( *line ) )
            temp[i++] = *line++;

        temp[i] = 0;

        STR_SKIP_SPACES( line );

        if ( !StrCheckIDName( temp ) || ( temp[0] == 0 ) )
        {
            MSG_SyntaxError(  );
            return RESULT_ERR;
        }

        inst2 = tat_AddInstance( temp, M );
    /* set destination */
        s->data.msg.inst = inst2;

        if ( line[0] == 0 )
        {
            MSG_SyntaxError(  );
            return RESULT_ERR;
        }

        if ( !tat_ParseFlowgenSignal( &line, M, indicator - 1 ) )
        {
            return RESULT_ERR;
        }

        s->line_Number = line_Counter;

        if ( !MSC_ParseSourceRef( line, s, M ) )
        {
            return RESULT_ERR;
        }

        s->next = MSC_AllocateEvent( M );

        if ( s->next == NULL )
            return RESULT_ERR;

        s = s->next;

        if ( !load_Comb )
        {
            s->type = MSC_EV_IN_MESSAGE;
            s->line_Number = line_Counter;
            s->inst_Count = 1;
            s->instances = MEM_Allocate( sizeof ( MSC_INSTANCE_ID_TYPE ) );
            s->instances[0] = inst2;
            s->data.msg.indicator = indicator - 1;
            s->data.msg.inst = inst1;
            s->next = MSC_AllocateEvent( M );

            if ( s->next == NULL )
                return RESULT_ERR;

            s = s->next;
        }

    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : PTK_RemoveMscTailCode
 *
 * Description: adjust number of instances for MSC_EV_CODE event
 *
 * Returns:
 *
 * Notes:
 *
 **************************************************************************/
int PTK_RemoveMscTailCode( MSC_EVENT_KIND * s, MSC_STORAGE * M )
{
    UINT32_T                    i;

    while ( s )
    {
        if ( ( s->next ) && ( s->next->next == NULL ) )
        {
            if ( s->next->instances )
                MEM_Free( s->next->instances );

            MEM_Free( s->next );

            s->next = NULL;
        }

        if ( s->type == MSC_EV_CODE )
        {
            s->inst_Count = M->instance_Count;
            s->instances = MEM_Reallocate( s->instances, M->instance_Count * sizeof ( MSC_INSTANCE_ID_TYPE ) );

            for ( i = 0; i < M->instance_Count; i++ )
                s->instances[i] = i;
        }

        if ( s->type == MSC_EV_OPERATOR )
        {
            if ( s->data.alt.type == MSC_OP_SEQ )
            {
                MSG_Abort( MSG_INTERNAL_PROBLEM );
            }

            for ( i = 0; i < s->data.alt.frame_Count; i++ )
                PTK_RemoveMscTailCode( s->data.alt.frame[i], M );
        }

        s = s->next;
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : FG_Load
 *
 * Description: Load file in FlowGen format
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL FG_Load( const char * filename, MSC_STORAGE * msc, const UINT32_T comb )
{
    FILE                   *f;
    MSC_EVENT_KIND              *root;
    int                     result;
    char                    tempf[TAT_MAX_FNAME_LEN + 4], *line;

    if ( !filename || !msc || filename[0] == 0 )
        return RESULT_ERR;

    f = FP_OpenFile( filename, "r" );

    load_Comb = comb;

    if ( !f )
        return RESULT_ERR;

    MSG_PushProcessedFile( filename );

    line_Counter = 0;

    indicator = 1;

    MSC_Allocate( filename, msc );

    msc->source_Format = 1;
    msc->src_MscLevel = 1;

/* initialize source references */
    line = &tempf[0];
    *line++ = '\"';
    FP_FullName( line, filename );
    line += strlen( line );
    *line++ = '\"';
    *line = 0;
    msc->src_Files = MEM_Allocate( sizeof ( msc->src_Files[0] ) );
    msc->src_Files[msc->src_Count].link = MSC_AddStr( tempf, msc );
    msc->src_Files[msc->src_Count].level = msc->src_MscLevel;
    msc->src_Files[msc->src_Count].first_Line = 0;
    msc->src_Files[msc->src_Count].last_Line = 0;
    msc->src_Count++;

    root = MSC_AllocateEvent( msc );

    if ( root == NULL )
    {
        return RESULT_ERR;
    }

    result = tat_LoadFlowgenEvents( f, root, msc );
    MSG_PopProcessedFile(  );

    if ( !root->type )
    {
        MEM_Free( root );
        msc->start = NULL;
    }
    else
    {
        msc->start = root;
        PTK_RemoveMscTailCode( root, msc );
    /* correct current level source information */
        msc->src_Files[msc->src_Count - 1].first_Line = ( msc->start ) ? msc->start->line_Number : 0;
        msc->src_Files[msc->src_Count - 1].last_Line = MSC_GetLastSourceLine( msc );
    }

    fclose( f );
    return result;
}

/***************************************************************************
 * Function : FG_GenerateLine
 *
 * Description: Generate single line in FlowGen format
 *
 * Returns: dest/NULL
 *
 * Notes:
 *
 **************************************************************************/
char * FG_GenerateLine( char *dest, MSC_EVENT_KIND * E, MSC_STORAGE * M, const UINT32_T flags )
{
    char                   *t;
    MSC_MESSAGE_STORAGE    *x;
    UINT32_T                    i;

    t = dest;
    *dest = 0;

    if ( !dest )
        return NULL;

    if ( !E )
        return NULL;

    if ( !M )
        return NULL;

    switch ( E->type )
    {

    case MSC_EV_CODE:

    case MSC_EV_ACTION:
    {
        sprintf( dest, "#CODE#\t%s", MSC_GetStr( E->data.text, M ) );
        return dest;
    }

    case MSC_EV_OUT_MESSAGE:

    case MSC_EV_IN_MESSAGE:

    case MSC_EV_SEND_SIGNAL:

    case MSC_EV_RECV_SIGNAL:

    case MSC_EV_EXIST_SIGNAL:

    {
        if ( E->type == MSC_EV_IN_MESSAGE )
            return t;

        if ( E->comment )
        {
            *dest++ = '#';
            *dest++ = '#';
            *dest++ = ' ';
            strcpy( dest, MSC_GetStr( E->comment, M ) );
            dest += strlen( dest );
            *dest++ = '\n';
            *dest = 0;
        }

        x = MSC_GetMessageParams( E->data.msg.indicator, M );
        strcpy( dest, MSC_GetStr( M->instances[E->instances[0]].name, M ) );
        dest += strlen( dest );
        *dest++ = '\t';
        strcpy( dest, MSC_GetStr( M->instances[E->data.msg.inst].name, M ) );
        dest += strlen( dest );

        *dest++ = '\t';

        if ( x == NULL )
            return NULL;

        strcpy( dest, MSC_GetStr( x->message_Name, M ) );

        dest += strlen( dest );

        if ( x->param_Count )
        {
            *dest++ = '(';
            for ( i = 0; i < x->param_Count - 1; i++ )
            {
                if ( x->message_Params[i] )
                {
                    strcpy( dest, MSC_GetStr( x->message_Params[i], M ) );
                    dest += strlen( dest );
                }
                *dest++ = ',';
                *dest++ = ' ';
                *dest = 0;
            }

            if ( x->message_Params[i] )
            {
                strcpy( dest, MSC_GetStr( x->message_Params[i], M ) );
            }

            dest += strlen( dest );
            *dest++ = ')';
            *dest = 0;
        }

        return t;
    }
    }

    return t;
}

/***************************************************************************
 * Function : tat_SaveFlowgenEvents
 *
 * Description: save flowgen line
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes:
 *
 **************************************************************************/
static LOGICAL tat_SaveFlowgenEvents( FILE * f, MSC_EVENT_KIND * s, MSC_STORAGE * M, int flags )
{
    char                    buffer[TAT_MAX_LINE_LEN];
    UINT32_T                    i;

    while ( s )
    {
        if ( s->type == MSC_EV_OPERATOR )
        {
            for ( i = 0; i < s->data.alt.frame_Count - 1; i++ )
            {
                tat_SaveFlowgenEvents( f, s->data.alt.frame[i], M, flags );
            }
            tat_SaveFlowgenEvents( f, s->data.alt.frame[i], M, flags );
        }
        else
        {
            FG_GenerateLine( buffer, s, M, flags );

            if ( buffer[0] )
                fprintf( f, "%s\n", buffer );
        }
        s = s->next;
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : FG_Save
 *
 * Description: Save file in FlowGen format
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes:
 *
 **************************************************************************/
LOGICAL FG_Save( const char * filename, MSC_STORAGE * M, const UINT32_T flags )
{
    char                    file[TAT_MAX_FNAME_LEN];
    char                    path[TAT_MAX_FNAME_LEN];
    char                    ext[TAT_MAX_FNAME_LEN];
    FILE                   *f;
    UINT32_T                    i;
    int                     res;

    if ( !filename )
        return RESULT_ERR;

    if ( !M )
        return RESULT_ERR;

    FP_SplitPath( filename, path, file, ext );

    f = FP_OpenFile( filename, "w" );

    if ( !f )
    {
        MSG_PrintFileError( filename );
        return RESULT_ERR;
    }

/* output text */
    for ( i = 0; i < M->text_Count; i++ )
        fprintf( f, "#\t%s\n", MSC_GetStr( M->texts[i], M ) );

    res = tat_SaveFlowgenEvents( f, M->start, M, flags );

    fclose( f );

    return res;

}
