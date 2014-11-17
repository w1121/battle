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
#include <portable.h>
#include <mscbase.h>
#include <qmemory.h>
#include <mscgen.h>
#include <strproc.h>
#include <fileproc.h>
#include <outmess.h>
#include <mscmess.h>
#include <flowgen.h>
#include <mscstrs.h>
#include <mscevent.h>
#include <tatgdata.h>


/***************************************************************************
 * Function : MSC_GetString
 *
 * Description: returns pointer to string
 *
 * Returns: pointer to string / NULL
 *
 * Notes:
 *
**************************************************************************/
char                   *MSC_GetString( const STRING_ID x, const MSC_STORAGE * M )
{
    if ( !M )
        return NULL;

    return StrCollGet( x, M->str );
}

/***************************************************************************
 * Function : MSC_GetStr
 *
 * Description: returns pointer to string
 *
 * Returns: pointer to string / Zero_String
 *
 * Notes:
 *
**************************************************************************/
char                   *MSC_GetStr( const STRING_ID s, const MSC_STORAGE * M )
{
    char                   *x;

    x = MSC_GetString( s, M );

    if ( x == NULL )
    {
        x = ( char * ) TAT_ZeroStr;
    }

    return x;
}

/***************************************************************************
 * Function : MSC_AddStr
 *
 * Description: allocate memory in string collection of MSC
 *
 * Returns: index of string
 *
 * Notes:
 *
**************************************************************************/
STRING_ID MSC_AddStr( const char * s, MSC_STORAGE * M )
{
    if ( !M )
        return 0;

    return StrCollAdd( s, M->str );
}

/***************************************************************************
 * Function : MSC_GetTimeLabelID
 *
 * Description: look for time label definition
 *
 * Returns: return positive number if found, -1 if not
 *
 * Notes: None
 *
 **************************************************************************/
int MSC_GetTimeLabelID( const char * label, const MSC_STORAGE * M )
{
    UINT32_T                    i;

    for ( i = 0; i < M->label_Count; i++ )
        if ( M->labels[i].label && !stricmp( label, MSC_GetString( M->labels[i].label, M ) ) )
            return i;

    return -1;
}

/* find the number of instance with same name
returns:
MSC_UNKNOWN_INSTANCE_ID - if not found
MSC_LOST_INSTANCE_ID -  if 'lost'
MSC_FOUND_INSTANCE_ID - if 'found'
0+ - if real instance
*/
int MSC_GetInstanceID( const char * instance, const MSC_STORAGE * M )
{
    size_t                    i;

    if ( !instance )
        return MSC_UNKNOWN_INSTANCE_ID;

    if ( !M )
        return MSC_UNKNOWN_INSTANCE_ID;

    if ( instance[0] == 0 )
        return MSC_UNKNOWN_INSTANCE_ID;

    if ( !stricmp( instance, MSC_LOST_INSTANCE_NAME ) )
        return MSC_LOST_INSTANCE_ID;

    if ( !stricmp( instance, MSC_FOUND_INSTANCE_NAME ) )
        return MSC_FOUND_INSTANCE_ID;

    for ( i = 0; i <  M->instance_Count; i++ )
    {
        if ( !stricmp( instance, MSC_GetString( M->instances[i].name, M ) ) )
            return (int)i;
    }

    return MSC_UNKNOWN_INSTANCE_ID;
}

/***************************************************************************
 * Function : MSC_GetLinkID
 *
 * Description: looks for link in MSC 
 *
 * Returns: -1 if not found, >=0, if found
 *
 * Notes: None
 *
 **************************************************************************/
int MSC_GetLinkID( const char * link, const MSC_STORAGE * M )
{
    int                     i;

    if ( !link )
        return -1;

    if ( !M )
        return -1;

    if ( link[0] == 0 )
        return -1;

    for ( i = 0; i < ( int ) M->src_Count; i++ )
    {
        if ( !stricmp( link, MSC_GetString( M->src_Files[i].link, M ) ) )
            return i;
    }

    return -1;
}

/***************************************************************************
 * Function : MSC_GetInstanceName
 *
 * Description: get instance name by number
 *
 * Returns: pointer to instance name
 *
 * Notes: None
 *
 **************************************************************************/
char  *MSC_GetInstanceName( int inst, MSC_STORAGE * M )
{
    if ( inst == MSC_LOST_INSTANCE_ID )
        return MSC_LOST_INSTANCE_NAME;

    if ( inst == MSC_FOUND_INSTANCE_ID )
        return MSC_FOUND_INSTANCE_NAME;

    return ( MSC_GetStr( M->instances[inst].name, M ) );
}

/***************************************************************************
 * Function : MSC_GetMessageParams
 *
 * Description: get message params by number
 *
 * Returns: pointer to data structure
 *
 * Notes: None
 *
 **************************************************************************/
MSC_MESSAGE_STORAGE    *MSC_GetMessageParams( const size_t ind, const MSC_STORAGE * M )
{
    size_t                  i;
    MSC_MESSAGE_STORAGE    *X;

    if ( !M || !M->message_Count)
        return NULL;

    if ( ind == MSC_INFINITY_CONST )
        return NULL;

    X = M->messages;

    if ( !X )
        return NULL;

    /* empirical checking */
    if ( ind <= M->message_Count && ( X[ind - 1].indicator == ind ) )
        return &( X[ind - 1] );

    /* sequential search */
    i = M->message_Count;
    while (i)
    {
        i--;
        if (X[i].indicator == ind) return &X[i];
    }

    return NULL; 
}

/***************************************************************************
 * Function : MSC_AddMessage
 *
 * Description: Add message ID to MSC storage
 *
 * Returns: 
 *
 * Notes: None
 *
 **************************************************************************/
MSC_MESSAGE_STORAGE    *MSC_AddMessage( MSC_MESSAGE_STORAGE * S, MSC_STORAGE * M )
{
    if ( !M )
        return NULL;

    M->messages = MEM_Reallocate( M->messages, ( M->message_Count + 1 ) * sizeof ( MSC_MESSAGE_STORAGE ) );

/* choose maximal number */
    if ( M->message_Number < S->indicator )
        M->message_Number = S->indicator;

    memcpy( &( M->messages[M->message_Count] ), S, sizeof ( MSC_MESSAGE_STORAGE ) );
    ( M->message_Count )++;
    return &( M->messages[M->message_Count - 1] );
}

/***************************************************************************
 * Function : MSC_CopyMessage
 *
 * Description: Copy message from SRC to M
 *
 * Returns: new message ID
 *
 * Notes: None
 *
 **************************************************************************/
size_t MSC_CopyMessage( size_t ind, MSC_STORAGE * M, MSC_STORAGE * SRC )
{
    MSC_MESSAGE_STORAGE    *X;
    MSC_MESSAGE_STORAGE     Y;
    size_t                  i, j;

    X = MSC_GetMessageParams( ind, SRC );


    if ( !X )
        return RESULT_ERR;

    memcpy( &Y, X, sizeof ( Y ) );

    /* convert source ID */
    Y.source_ID = MSC_AddStr( MSC_GetString( Y.source_ID, SRC ), M );

    if ( Y.param_Count )
    {
        Y.message_Params = MEM_Copy( Y.message_Params, Y.param_Count * sizeof ( Y.message_Params[0] ) );
        Y.params_Values = MEM_Copy( Y.params_Values, Y.param_Count * sizeof ( Y.params_Values[0] ) );
    }

    if ( SRC != M )
    {
        for ( i = 0; i < Y.param_Count; i++ )
        {
            Y.params_Values[i].values=MEM_Copy(Y.params_Values[i].values, Y.params_Values[i].valueCount*sizeof(Y.params_Values[i].values[0]));
            Y.message_Params[i] = MSC_AddStr( MSC_GetString( Y.message_Params[i], SRC ), M );
            Y.params_Values[i].varName = MSC_AddStr( MSC_GetString( Y.params_Values[i].varName, SRC ), M );
            for (j=0; j<Y.params_Values[i].valueCount; j++)
            {
                Y.params_Values[i].values[j] = MSC_AddStr( MSC_GetString( Y.params_Values[i].values[j], SRC ), M );
            }
        }
        Y.message_Name = MSC_AddStr( MSC_GetString( Y.message_Name, SRC ), M );
        Y.delay.str_MaxTime = MSC_AddStr( MSC_GetString( Y.delay.str_MaxTime, SRC ), M );
        Y.delay.str_MinTime = MSC_AddStr( MSC_GetString( Y.delay.str_MinTime, SRC ), M );
    }
    else
    {
        for ( i = 0; i < Y.param_Count; i++ )
        {
            Y.params_Values[i].values=MEM_Copy(Y.params_Values[i].values, Y.params_Values[i].valueCount*sizeof(Y.params_Values[i].values[0]));
        }
    }

    j = M->message_Number;
    X = MSC_AddMessage( &Y, M );

    if ( !X )
        return RESULT_ERR;

    if ( X->indicator <= j )
        X->indicator = ++M->message_Number;

    return X->indicator;
}

/***************************************************************************
 * Function : MSC_FreeMemory
 *
 * Description: clean allocated memory
 *
 * Return: none
 *
 * Notes:
 *
 **************************************************************************/
LOGICAL MSC_FreeMemory( MSC_STORAGE * M )
{
    size_t    i, j;

    if ( !M )
        return RESULT_ERR;

    StrCollClean( M->str );

    for ( i = 0; i < M->message_Count; i++ )
        if ( M->messages[i].param_Count )
        {
            MEM_Free( M->messages[i].message_Params );
            M->messages[i].message_Params=NULL;
            /* free parsed parameters */
            for (j=0; j<M->messages[i].param_Count; j++)
            {
                MEM_Free(M->messages[i].params_Values[j].values);
                M->messages[i].params_Values[j].values=NULL;
            }
            MEM_Free( M->messages[i].params_Values );
            M->messages[i].params_Values=NULL;
        }

    if ( M->messages )
    {
        MEM_Free( M->messages );
        M->messages=NULL;
    }

    if ( M->var_Count )
    {
        MEM_Free( M->vars );
        M->var_Count=0;
        M->vars=NULL;
    }

    if ( M->param_Count )
    {
        MEM_Free( M->params );
        M->param_Count=0;
        M->params=NULL;
    }

    if ( M->src_Files )
    {
        MEM_Free( M->src_Files );
        M->src_Files=NULL;
    }

    if ( M->labels )
        MEM_Free( M->labels );

    if ( M->instances )
        MEM_Free( M->instances );

    if ( M->texts )
        MEM_Free( M->texts );

    if ( M->traces )
        MEM_Free( M->traces );

    MSC_FreeEventChain( M->start );

/* fill memory with zero to avoid further bugs */
    memset( M, 0, sizeof ( MSC_STORAGE ) );

    return RESULT_OK;
}


/***************************************************************************
 * Function : MSC_Allocate
 *
 * Description: Initialize MSC storage structure
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_Allocate( char *filename, MSC_STORAGE * M )
{
    char                    fn[TAT_MAX_FNAME_LEN];

    memset( M, 0, sizeof ( MSC_STORAGE ) );

    M->str = StrCollInit( NULL );

    if ( filename )
    {
        M->msc_File = MSC_AddStr( filename, M );
        FP_SplitPath( filename, NULL, fn, NULL );
        M->msc_Document = MSC_AddStr( fn, M );
        M->msc_Name = M->msc_Document;
    }

/* allocate special label for handling ABSOLUTE time */
    M->label_Count = 1;
    M->labels = MEM_Allocate( sizeof ( MSC_EVENT_KIND_LABEL ) );
    M->labels[0].label = MSC_AddStr( STR_MSCSTART, M );
    M->labels[0].unused = 1;
    return RESULT_OK;
}

/******************************************************************************/
static MSC_EVENT_KIND_TYPE   msc_EventTypeTable[6][6] = {
/*from/to           UNDEF,              IUT,            ENV,            MONITOR,            VMODEL,           IGNORE */
     /*UNDEF*/      {MSC_EV_UNDEFINED, MSC_EV_UNDEFINED, MSC_EV_UNDEFINED, MSC_EV_UNDEFINED, MSC_EV_UNDEFINED, MSC_EV_UNDEFINED,},
     /*IUT*/        {MSC_EV_UNDEFINED, MSC_EV_BREAK, MSC_EV_RECV_SIGNAL, MSC_EV_EXIST_SIGNAL, MSC_EV_EXIST_SIGNAL, MSC_EV_EMPTY},
     /*ENV*/        {MSC_EV_UNDEFINED, MSC_EV_SEND_SIGNAL, MSC_EV_BREAK, MSC_EV_EXIST_SIGNAL, MSC_EV_SEND_SIGNAL, MSC_EV_EMPTY},
     /*MONITOR*/    {MSC_EV_UNDEFINED, MSC_EV_SEND_SIGNAL, MSC_EV_RECV_SIGNAL, MSC_EV_EXIST_SIGNAL, MSC_EV_SEND_SIGNAL, MSC_EV_EMPTY},
     /*VMODEL*/     {MSC_EV_UNDEFINED, MSC_EV_EXIST_SIGNAL, MSC_EV_RECV_SIGNAL, MSC_EV_EXIST_SIGNAL, MSC_EV_EXIST_SIGNAL, MSC_EV_EMPTY},
     /*IGNORE */    {MSC_EV_UNDEFINED, MSC_EV_EMPTY, MSC_EV_EMPTY, MSC_EV_EMPTY, MSC_EV_EMPTY, MSC_EV_EMPTY}
};

static MSC_INSTANCE_TYPE msc_InverseTypeTable[6] = {
    MSC_INSTANCE_UNDEF, MSC_INSTANCE_ENV, MSC_INSTANCE_IUT, MSC_INSTANCE_UNDEF, MSC_INSTANCE_UNDEF,
    MSC_INSTANCE_IGNORE
};

static LOGICAL msc_IsEnvInstance( MSC_INSTANCE_TYPE x )
{
    return ( x == MSC_INSTANCE_ENV ) || ( x == MSC_INSTANCE_MONITOR );
}

static LOGICAL msc_IsIUTInstance( MSC_INSTANCE_TYPE x )
{
    return ( x == MSC_INSTANCE_IUT ) || ( x == MSC_INSTANCE_VMODEL );
}

/***************************************************************************
 * Function : MSC_TestEvent
 *
 * Description: check event for existence criteria against flags
 *
 * Returns: RESULT_ERR/RESULT_OK/MSC_REMOVE_ACTION
 *
 * Notes: None
 *
 **************************************************************************/
int MSC_TestEvent( const MSC_STORAGE * M, MSC_EVENT_KIND * E, const UINT32_T flags )
{
    size_t                  i, insys, inenv;
    MSC_EVENT_KIND_TYPE          act;

    if ( !M || !E )
        return RESULT_ERR;

    insys = ( flags & MSC_CHECK_INSYS ) != 0;

    inenv = ( flags & MSC_CHECK_INENV ) != 0;

    if ( E->labeled == 0 )
    {
        if ( ( flags & MSC_CHECK_DEL_TIMERS ) &&
            ( E->type == MSC_EV_TIMER_SET || E->type == MSC_EV_TIMER_TIMEOUT ||
                E->type == MSC_EV_TIMER_RESET ) )
        {
            if ( E->data.timer.relation && E->data.timer.relation->labeled )
                return RESULT_OK;

            return MSC_REMOVE_ACTION;
        };

        if ( E->type == MSC_EV_CONDITION )
        {
        /* do not remove conditions */

            if ( E->data.cond.call )
                return RESULT_OK;

        /* remove condition if ordered */
            if ( flags & MSC_CHECK_DEL_ALL_COND )
                return MSC_REMOVE_ACTION;

        /* remove generated conditions */
            if ( E->data.cond.generated )
            {
                return MSC_REMOVE_ACTION;
            }
        }

    /* remove events which belongs to ignored instances */
        for ( i = 0; ( MSC_INSTANCE_ID_TYPE ) i < E->inst_Count; i++ )
        {
            if ( M->instances[E->instances[i]].ind != MSC_INSTANCE_IGNORE )
                break;
        }

        if ( ( MSC_INSTANCE_ID_TYPE ) i == E->inst_Count )
            return MSC_REMOVE_ACTION;
    }

/* special handle of FlowGen messages */
    if ( E->type == MSC_EV_SEND_SIGNAL && M->source_Format == 1 )
    {
        act = msc_EventTypeTable[M->instances[E->instances[0]].ind][M->instances[E->data.msg.inst].ind];

        if ( act == MSC_EV_EMPTY )
            return MSC_REMOVE_ACTION;

        if ( act == MSC_EV_UNDEFINED || act == MSC_EV_BREAK )
        {
            if ( ( M->instances[E->data.msg.inst].ind == M->instances[E->instances[0]].ind ) &&
                ( M->instances[E->instances[0]].ind == MSC_INSTANCE_ENV ) && ( flags & MSC_CHECK_ENVSELF ) )
            {
                act = MSC_EV_SEND_SIGNAL;
            }
            else
            {
                MSG_PrintError( MSG_INVALID_MESS_DIR_su, MSC_GetStr( M->msc_File, M ), E->line_Number );
                return MSC_REMOVE_ACTION;
            }
        }

        E->type = act;
        return RESULT_OK;
    }

/* exit if no information about instances is provided or no rule is defined */
    if ( ( M->instances[E->instances[0]].ind == MSC_INSTANCE_UNDEF ) || ( flags & MSC_CHECK_SAVESR ) )
    {
        return RESULT_OK;
    }

    if ( ( E->type == MSC_EV_IN_MESSAGE || E->type == MSC_EV_OUT_MESSAGE
            || ( E->type == MSC_EV_SEND_SIGNAL && M->source_Format == 1 ) ) && !( flags & MSC_CHECK_SAVESR ) )
    {
        if ( M->instances[E->instances[0]].ind == MSC_INSTANCE_IGNORE ||
            ( E->data.msg.inst >= 0 && ( M->instances[E->data.msg.inst].ind == MSC_INSTANCE_IGNORE ) ) )
        {
            return MSC_REMOVE_ACTION;
        }

        if ( E->data.msg.inst < 0 )
        {
            if ( E->type == MSC_EV_IN_MESSAGE )
            {
                act =
                    msc_EventTypeTable[msc_InverseTypeTable[M->instances[E->instances[0]].ind]][M->
                    instances[E->instances[0]].ind];

                if ( act == MSC_EV_UNDEFINED || act == MSC_EV_BREAK )
                {
                    MSG_PrintError( MSG_MESS_MSC_FOUND_INSTANCE_ID_ENV_su,
                        MSC_GetStr( M->msc_File, M ), E->line_Number );
                    return MSC_REMOVE_ACTION;
                };

                if ( act == MSC_EV_EMPTY )
                    return MSC_REMOVE_ACTION;

                E->type = act;
            }
            else
            {
                act =
                    msc_EventTypeTable[M->instances[E->instances[0]].
                    ind][msc_InverseTypeTable[M->instances[E->instances[0]].ind]];

                if ( act == MSC_EV_UNDEFINED || act == MSC_EV_BREAK )
                {
                    MSG_PrintError( MSG_MESS_ENV_LOST_su, MSC_GetStr( M->msc_File, M ), E->line_Number );
                    return MSC_REMOVE_ACTION;
                };

                if ( act == MSC_EV_EMPTY )
                    return MSC_REMOVE_ACTION;

                E->type = act;
            }

            return RESULT_OK;      /* otherwise, save event */
        }

        if ( E->type == MSC_EV_IN_MESSAGE )
        {
            act = msc_EventTypeTable[M->instances[E->data.msg.inst].ind][M->instances[E->instances[0]].ind];
        }
        else
        {
            act = msc_EventTypeTable[M->instances[E->instances[0]].ind][M->instances[E->data.msg.inst].ind];
        }

        if ( act == MSC_EV_EMPTY )
        {
            return MSC_REMOVE_ACTION;
        }

        if ( act == MSC_EV_UNDEFINED || act == MSC_EV_BREAK )
        {
            if ( ( M->instances[E->data.msg.inst].ind == M->instances[E->instances[0]].ind ) &&
                ( M->instances[E->instances[0]].ind == MSC_INSTANCE_ENV ) && ( flags & MSC_CHECK_ENVSELF ) )
            {
                act = MSC_EV_SEND_SIGNAL;
            }
            else
            {
                MSG_PrintError( MSG_INVALID_MESS_DIR_su, MSC_GetStr( M->msc_File, M ), E->line_Number );
                return MSC_REMOVE_ACTION;
            }
        }

        if ( E->type == MSC_EV_IN_MESSAGE )
        {
            if ( ( msc_IsIUTInstance( M->instances[E->instances[0]].ind ) && insys ) ||
                ( msc_IsEnvInstance( M->instances[E->instances[0]].ind ) && inenv ) )
            {
                E->type = act;
            /* swap instances */
                i = E->instances[0];
                E->instances[0] = E->data.msg.inst;
                E->data.msg.inst = (int)i;
                return RESULT_OK;
            }
            return MSC_REMOVE_ACTION;
        }
        else if ( E->type == MSC_EV_OUT_MESSAGE )
        {
            if ( ( msc_IsIUTInstance( M->instances[E->data.msg.inst].ind ) && !insys ) ||
                ( msc_IsEnvInstance( M->instances[E->data.msg.inst].ind ) && !inenv ) )
            {
                E->type = act;
                return RESULT_OK;
            }

            return MSC_REMOVE_ACTION;
        }
        else if ( E->type == MSC_EV_SEND_SIGNAL && M->source_Format == 1 )
        {
            E->type = act;
            return RESULT_OK;
        }
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_CompareEvents
 *
 * Description: returns true if events are semantically equal
 *
 * Returns: LOGICAL_TRUE/LOGICAL_FALSE
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_CompareEvents( const MSC_STORAGE * M, MSC_EVENT_KIND * E1, MSC_EVENT_KIND * E2 )
{
    char                   *n1, *n2;
    UINT32_T                    i;

    if ( !E1 || !E2 )
        return LOGICAL_FALSE;

    if ( E1 == E2 )
        return LOGICAL_TRUE;

    if ( E1->type == E2->type )
    {
        if ( E1->time_Calc.type != E2->time_Calc.type )
            return LOGICAL_FALSE;

        if ( E1->time_Calc.type == MSC_LABELED_TIME && E1->time_Calc.time_Label != E2->time_Calc.time_Label )
            return LOGICAL_FALSE;

        if ( E1->time_Calc.min_Time != E2->time_Calc.min_Time )
            return LOGICAL_FALSE;

        if ( E1->time_Calc.max_Time != E2->time_Calc.max_Time )
            return LOGICAL_FALSE;

        switch ( E1->type )
        {

        case MSC_EV_STOP:
        {
            if ( E1->instances[0] != E2->instances[0] )
            {
                return LOGICAL_FALSE;
            }
            return LOGICAL_TRUE;
        }
        case MSC_EV_CREATE:
        {
            if ( E1->data.create.instance != E2->data.create.instance )
            {
                return LOGICAL_FALSE;
            }
            if ( E1->data.create.param_Count != E2->data.create.param_Count )
            {
                return LOGICAL_FALSE;
            }

            for ( i = 0; i < E1->data.create.param_Count; i++ )
            {
                n1 = MSC_GetString( E1->data.create.params[i], M );
                n2 = MSC_GetString( E2->data.create.params[i], M );

                if ( ( n1 && n2 && stricmp( n1, n2 ) ) || ( ( n1 == NULL || n2 == NULL ) && n1 != n2 ) )
                    return LOGICAL_FALSE;
            }
            return LOGICAL_TRUE;
        }

        case MSC_EV_TIMER_SET:
        case MSC_EV_TIMER_RESET:
        case MSC_EV_TIMER_TIMEOUT:
        {
            if ( E1->data.timer.timing.type != E2->data.timer.timing.type )
            {
                return LOGICAL_FALSE;
            }
            if ( E1->data.timer.timing.min_Time != E2->data.timer.timing.min_Time )
            {
                return LOGICAL_FALSE;
            }
            if ( E1->data.timer.timing.max_Time != E2->data.timer.timing.max_Time )
            {
                return LOGICAL_FALSE;
            }
            if ( E1->data.timer.timing.type != MSC_SPECIAL_TIMER )
            {
                n1 = MSC_GetString( E1->data.timer.signal_Name, M );
                n2 = MSC_GetString( E2->data.timer.signal_Name, M );

                if ( ( n1 && n2 && stricmp( n1, n2 ) ) || ( ( n1 == NULL || n2 == NULL ) && n1 != n2 ) )
                    return LOGICAL_FALSE;
            }
            return LOGICAL_TRUE;
        }

        case MSC_EV_SEND_SIGNAL:
        case MSC_EV_RECV_SIGNAL:
        case MSC_EV_EXIST_SIGNAL:
        case MSC_EV_IN_MESSAGE:
        case MSC_EV_OUT_MESSAGE:
        {
            MSC_MESSAGE_STORAGE    *x1, *x2;

        /* by default, events are equal, if their ID are equal */
            if ( E1->data.msg.indicator == E2->data.msg.indicator )
            {
                return LOGICAL_TRUE;
            }
            if ( E1->data.msg.inst != E2->data.msg.inst )
                return LOGICAL_FALSE;

            if ( E1->instances[0] != E2->instances[0] )
                return LOGICAL_FALSE;

            x1 = MSC_GetMessageParams( E1->data.msg.indicator, M );
            x2 = MSC_GetMessageParams( E2->data.msg.indicator, M );

            if ( x1->param_Count != x2->param_Count )
                return LOGICAL_FALSE;

            n1 = MSC_GetString( x1->message_Name, M );
            n2 = MSC_GetString( x2->message_Name, M );

            if ( stricmp( n1, n2 ) )
                return LOGICAL_FALSE;

            for ( i = 0; i < x1->param_Count; i++ )
            {
                n1 = MSC_GetString( x1->message_Params[i], M );
                n2 = MSC_GetString( x2->message_Params[i], M );

                if ( ( n1 && n2 && stricmp( n1, n2 ) ) || ( ( n1 == NULL || n2 == NULL ) && n1 != n2 ) )
                    return LOGICAL_FALSE;
            }

            return LOGICAL_TRUE;
        }

        case MSC_EV_CODE:

        case MSC_EV_ACTION:
        {
            n1 = MSC_GetString( E1->data.text, M );
            n2 = MSC_GetString( E2->data.text, M );

            if ( ( n1 && n2 && stricmp( n1, n2 ) ) || ( ( n1 == NULL || n2 == NULL ) && n1 != n2 ) )
                return LOGICAL_FALSE;

            return LOGICAL_TRUE;
        }

        case MSC_EV_REFERENCE:
        {
            n1 = MSC_GetString( E1->data.ref.refr_Name, M );
            n2 = MSC_GetString( E2->data.ref.refr_Name, M );

            if ( stricmp( n1, n2 ) )
                return LOGICAL_FALSE;

            if ( E1->data.ref.param_Count != E2->data.ref.param_Count )
                return LOGICAL_FALSE;

            return LOGICAL_TRUE;
        }

        case MSC_EV_CONDITION:
        {
            if ( E1->inst_Count != E2->inst_Count )
                return LOGICAL_FALSE;

            n1 = MSC_GetString( E1->data.cond.text, M );

            n2 = MSC_GetString( E2->data.cond.text, M );

            if ( stricmp( n1, n2 ) )
                return LOGICAL_FALSE;

            for ( i = 0; i < E1->inst_Count; i++ )
            {
                if ( E1->instances[i] != E2->instances[i] )
                    return LOGICAL_FALSE;
            }

            if ( E1->data.cond.call || E2->data.cond.call )
            {
                n1 = MSC_GetString( E1->data.cond.call, M );
                n2 = MSC_GetString( E2->data.cond.call, M );

                if ( ( n1 && !n2 ) || ( n2 && !n1 ) )
                    return LOGICAL_FALSE;

                if ( stricmp( n1, n2 ) )
                    return LOGICAL_FALSE;
            }

            return LOGICAL_TRUE;
        }

        }
    }

    return LOGICAL_FALSE;
}

/***************************************************************************
 * Function : MSC_InsertEvent
 *
 * Description: Insert x before s so that memory allocated by s is filled with x 
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_InsertEvent( MSC_STORAGE * M, MSC_EVENT_KIND * s, MSC_EVENT_KIND * x )
{
    MSC_EVENT_KIND               p;
    size_t                  lab;

    if ( !x || !s )
        return RESULT_ERR;

    lab = s->labeled;

    memcpy( &p, s, sizeof ( MSC_EVENT_KIND ) );
    memcpy( s, x, sizeof ( MSC_EVENT_KIND ) );
    memcpy( x, &p, sizeof ( MSC_EVENT_KIND ) );

    s->next = x;

    if ( lab )
        M->labels[lab - 1].s = x;

    if ( x->type == MSC_EV_TIMER_SET || x->type == MSC_EV_TIMER_RESET || x->type == MSC_EV_TIMER_TIMEOUT )
    {
    /* fix relation to itself */
        x->data.timer.relation->data.timer.relation = x;
    }
    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_GlobalCond
 *
 * Description: generate global condition for MSC
 *
 * Returns: pointer to event's structure
 *
 * Notes: None
 *
 **************************************************************************/
MSC_EVENT_KIND              *MSC_GlobalCond( MSC_STORAGE * M )
{
    size_t                  i;
    MSC_EVENT_KIND              *s;

    s = MSC_AllocateEventType( MSC_EV_CONDITION );

    if ( !s )
        return s;

    s->data.cond.text = MSC_AddStr( MSC_GLOBAL_CONDITION_NAME, M );
    s->data.cond.generated = 1;

    s->inst_Count = M->instance_Count;

    s->instances = MEM_Allocate( M->instance_Count * sizeof ( MSC_INSTANCE_ID_TYPE ) );

    for ( i = 0; i < M->instance_Count; i++ )
        s->instances[i] = ( int ) i;

    MSC_ZeroEventTime( s );

    return s;
}

/***************************************************************************
 * Function : MSC_TimeLabelEvent
 *
 * Description: Mark event with label
 *
 * Returns: label ID
 *
 * Notes: None
 *
 **************************************************************************/
size_t MSC_TimeLabelEvent( MSC_STORAGE * M, MSC_EVENT_KIND * s, char *lab )
{
    char                    dest[TAT_MAX_ID_LEN];
    size_t                  i;

    if ( !s )
        return RESULT_ERR;

    if ( s->labeled )
        return s->labeled;

    if ( !M )
        return RESULT_ERR;

    s->labeled = M->label_Count + 1;
    M->labels = MEM_Reallocate( M->labels, sizeof ( MSC_EVENT_KIND_LABEL ) * ( M->label_Count + 1 ) );
    M->labels[M->label_Count].s = s;
    i = M->label_Count;

    do
    {
        sprintf( dest, "%s%u", lab, i );
        i++;
    }
    while ( MSC_GetTimeLabelID( dest, M ) >= 0 );

    M->labels[M->label_Count].label = MSC_AddStr( dest, M );
    M->labels[M->label_Count].unused = 0;
    M->labels[M->label_Count].generated = 1;

    ( M->label_Count )++;

    return s->labeled;
}

static int msc_CompareMessageID( const void *x1, const void *x2 )
{
    return (int)( ( MSC_MESSAGE_STORAGE * ) x1 )->indicator - ( ( MSC_MESSAGE_STORAGE * ) x2 )->indicator;
}

/***************************************************************************
 * Function : MSC_SortMess
 *
 * Description: Sort message by incrementing of indicator
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_SortMess( MSC_STORAGE * M )
{
    if ( !M )
        return RESULT_ERR;

    if ( M->message_Count )
        qsort( M->messages, M->message_Count, sizeof ( MSC_MESSAGE_STORAGE ), msc_CompareMessageID );

    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_ExpandInstance
 *
 * Description: Expand event's instances to all objects
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_ExpandInstance( MSC_EVENT_KIND * x, MSC_STORAGE * M )
{
    UINT32_T                    i, j;

    if ( !M || !x )
        return RESULT_ERR;

    if ( x->inst_Count >= M->instance_Count )
        return RESULT_OK;

    x->instances = MEM_Reallocate( x->instances, M->instance_Count * sizeof ( MSC_INSTANCE_ID_TYPE ) );

    while ( x->inst_Count < M->instance_Count )
    {
        for ( j = 0; j < M->instance_Count; j++ )
        {
            for ( i = 0; i < x->inst_Count; i++ )
                if ( x->instances[i] == j )
                    break;

            if ( i == x->inst_Count )
                x->instances[x->inst_Count++] = j;
        }
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_HugeMul
 *
 * Description: multiply with overflow checking
 *
 * Returns: min((i*j), MSC_INFINITY_CONST)
 *
 * Notes: None
 *
 **************************************************************************/
size_t MSC_HugeMul( size_t i, size_t j )
{
    size_t     maxx;

    if ( i == 0 || j == 0 )
        return 0;

    if ( i == MSC_INFINITY_CONST || j == MSC_INFINITY_CONST )
        return MSC_INFINITY_CONST;

    maxx = MSC_INFINITY_CONST / j;

    if ( i > maxx )
        return MSC_INFINITY_CONST;

    return i * j;
}

/***************************************************************************
 * Function : MSC_ConvertString
 *
 * Description: Convert MSC string
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_ConvertString( STRING_ID * x, const MSC_STORAGE * MSC, STRCOLL_PROCESS_FUNC conv )
{
    return StrCollProcess( x, MSC->str, conv );
}

