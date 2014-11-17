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
* Functions:    CFG_SetDefaultConfig
*               CFG_GetInstanceID
*               CFG_GetSignalID
*               CFG_Check_MSC
*               CFG_GetDataTypeID
*               CFG_GetMacroID
*               CFG_GetMacroDT
*               CFG_GetMacroTypeInd
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  TAT configuration processing routines
*
* Notes:        None
*
******************************************************************************/
#include <portable.h>
#include <strproc.h>
#include <mscbase.h>
#include <mscparse.h>
#include <qmemory.h>
#include <fileproc.h>
#include <cmdline.h>
#include <outmess.h>
#include <mscstrs.h>
#include <mscmess.h>
#include <xmlconf.h>
#include <confproc.h>
#include <tatgdata.h>
#include <mscproc.h>

/***************************************************************************
 * Function : CFG_SetDefaultConfig
 *
 * Description: Set default configuration
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL CFG_SetDefaultConfig( TAT_XML_CONFIG_DEF * d )
{
    CFG_DefaultConfig = d;
    return RESULT_OK;
}

/***************************************************************************
 * Function : CFG_GetInstanceID
 *
 * Description: Get Instance ID from instance declarations
 *
 * Returns: ID/-1/-2 if found, -3 if not found
 *
 * Notes: None
 *
 **************************************************************************/
int CFG_GetInstanceID( const char * name )
{
    char                    key[TAT_MAX_ID_LEN];
    size_t                  lo, hi, mid, half, num;
    int                     result;

    if ( !name )
        return MSC_UNKNOWN_INSTANCE_ID;

    /*StrUpCopy( key, name );*/
	strcpy(key,name);

    if ( !strcmp( key, "LOST" ) )
        return MSC_LOST_INSTANCE_ID;

    if ( !strcmp( key, "FOUND" ) )
        return MSC_FOUND_INSTANCE_ID;

    if ( !CFG_DefaultConfig || !CFG_DefaultConfig->instance_Count )
        return MSC_UNKNOWN_INSTANCE_ID;

    lo = 0;
    num = CFG_DefaultConfig->instance_Count;
    hi = num - 1;

    while ( lo <= hi )
    {
        half = ( num >> 1 );
        if ( half )
        {
            mid = lo + ( ( num & 1 ) ? half : ( half - 1 ) );
            result = strcmp( key, CFG_DefaultConfig->instances[mid].name );
            if ( !result ) return ( ( int ) mid );
            else if ( result < 0 )
            {
                hi = mid - 1;
                num = ( num & 1 ) ? half : half - 1;
            }
            else
            {
                lo = mid + 1;
                num = half;
            }
        }
        else if ( num )
            return (int)( strcmp( key, CFG_DefaultConfig->instances[lo].name ) ? MSC_UNKNOWN_INSTANCE_ID : lo );
        else
            break;
    }

    return MSC_UNKNOWN_INSTANCE_ID;
}

/***************************************************************************
 * Function : CFG_GetSignalID
 *
 * Description: retrieve signal ID by signal name
 *
 * Returns: -1/number
 *
 * Notes: None
 *
 **************************************************************************/
int CFG_GetSignalID( const char * signal )
{
    char                key[TAT_MAX_ID_LEN];
    size_t              lo, hi, mid, half, num;
    int                     result;

    if ( !signal || !CFG_DefaultConfig || !CFG_DefaultConfig->signal_Count )
        return -1;

	/* StrUpCopy( key, signal ); */
	strcpy(key,signal);
    lo = 0;
    num = CFG_DefaultConfig->signal_Count;
    hi = num - 1;

    while ( lo <= hi )
    {
        half = ( num >> 1 );
        if ( half )
        {
            mid = lo + ( ( num & 1 ) ? half : ( half - 1 ) );
            result = strcmp( key, CFG_DefaultConfig->signals[mid].name );
            if ( !result )
                return (int)( mid );
            else if ( result < 0 )
            {
                hi = mid - 1;
                num = ( num & 1 ) ? half : half - 1;
            }
            else
            {
                lo = mid + 1;
                num = half;
            }
        }
        else if ( num )
            return (int)( strcmp( key, CFG_DefaultConfig->signals[lo].name ) ? -1 : lo );
        else
            break;
    }

    return -1;
}

/***************************************************************************
* Function : CFG_GetDataTypeID
*
* Description: retrieve data type ID by name
*
* Returns: -1/number
*
* Notes: None
*
**************************************************************************/
int CFG_GetDataTypeID( const char * data )
{
    char                    key[TAT_MAX_ID_LEN];
    size_t                  lo, hi, mid, half, num;
    int                     result;

    if ( !data || !CFG_DefaultConfig || !CFG_DefaultConfig->data_Count )
        return -1;

/* assume, that at least 1 element exist */
    /*StrUpCopy( key, data );*/
	strcpy(key,data);
    lo = 0;
    num = CFG_DefaultConfig->data_Count;
    hi = num - 1;

    while ( lo <= hi )
    {
        half = ( num >> 1 );
        if ( half )
        {
            mid = lo + ( ( num & 1 ) ? half : ( half - 1 ) );
            result = strcmp( key, CFG_DefaultConfig->data[mid].name);

            if ( !result ) return (int)( mid );
            else if ( result < 0 )
            {
                hi = mid - 1;
                num = ( num & 1 ) ? half : half - 1;
            }
            else
            {
                lo = mid + 1;
                num = half;
            }
        }
        else if ( num )
            return (int)( strcmp( key, CFG_DefaultConfig->data[lo].name ) ? -1 : lo );
        else
            break;
    }

    return -1;
}

/***************************************************************************
 * Function : CFG_GetMacroID
 *
 * Description: retrieve macro ID by name
 *
 * Returns: -1/number
 *
 * Notes: None
 *
 **************************************************************************/
int CFG_GetMacroID( const char * data )
{
    size_t                  i;
    char                    buf[TAT_MAX_ID_LEN];

    if ( !data || !CFG_DefaultConfig )
        return -1;

    StrUpCopy( buf, data );
    for ( i = 0; i < CFG_DefaultConfig->macro_Count; i++ )
    {
        if ( CFG_DefaultConfig->macro[i].name && !strcmp( buf, CFG_DefaultConfig->macro[i].name ) )
            return (int)i;
    }

    return -1;
}

/***************************************************************************
 * Function : CFG_GetMacroTypeInd
 *
 * Description: retrieve macro type by index
 *
 * Returns: type
 *
 * Notes: None
 *
 **************************************************************************/
MSC_MACRO_TYPE CFG_GetMacroTypeInd( const size_t i )
{
    if ( !CFG_DefaultConfig || i >= CFG_DefaultConfig->macro_Count )
        return MSC_MACROTYPE_UNDEFINED;

    return CFG_DefaultConfig->macro[i].type;
}

/***************************************************************************
 * Function : CFG_GetMacroDT
 *
 * Description: retrieve data type ID by name
 *
 * Returns: MSC_MACRODATA_UNDEFINED/type
 *
 * Notes: None
 *
 **************************************************************************/
MSC_MACRO_DATA CFG_GetMacroDT( const char * data )
{
    size_t                  i;
    char                    buf[TAT_MAX_ID_LEN];

    if ( !data || !CFG_DefaultConfig )
        return MSC_MACRODATA_UNDEFINED;

    StrUpCopy( buf, data );
    for ( i = 0; i < CFG_DefaultConfig->macro_Count; i++ )
    {
        if ( CFG_DefaultConfig->macro[i].name && !strcmp( buf, CFG_DefaultConfig->macro[i].name ) )
            return CFG_DefaultConfig->macro[i].data_Type;
    }

    return MSC_MACRODATA_UNDEFINED;
}

/***************************************************************************
 * Function : tat_MatchMscData
 *
 * Description: Match MSC data types against specified in XML configuration
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_MatchMscData( MSC_STORAGE * M )
{
    size_t                  i;
    int                     d_type;

/* process variables */
    for ( i = 0; i < M->var_Count; i++ )
    {
        M->vars[i].pv_TypeSrc = M->vars[i].pv_Type;
        d_type = CFG_GetDataTypeID( MSC_GetString( M->vars[i].pv_Type, M ) );
        if ( d_type != -1 )
        {
            M->vars[i].pv_Type = MSC_AddStr( CFG_DefaultConfig->data[d_type].target_Name, M );
        }
    }

/* process MSC parameters */
    for ( i = 0; i < M->param_Count; i++ )
    {
        M->params[i].pv_TypeSrc = M->params[i].pv_Type;
        d_type = CFG_GetDataTypeID( MSC_GetString( M->params[i].pv_Type, M ) );
        if ( d_type != -1 )
        {
            M->params[i].pv_Type = MSC_AddStr( CFG_DefaultConfig->data[d_type].target_Name, M );
        }
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_MatchMscInstance
 *
 * Description: Match objects in MSC with instance definitions
 *
 * Returns: RESULT_ERR/ RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_MatchMscInstance( MSC_STORAGE * M )
{
    size_t                  i;         /* loop counters */
    char                   *name;      /* instance name */
    size_t                  err = 0;
    int                     inst_id;

/* check input parameters */

    if ( !M )
        return RESULT_ERR;

    if ( !CFG_DefaultConfig->instance_Count || !M->instance_Count )
        return RESULT_OK;

/* go through all loaded instances */
    for ( i = 0; i < M->instance_Count; i++ )
    {
        name = MSC_GetString( M->instances[i].name, M );
        inst_id = CFG_GetInstanceID( name );
        if ( inst_id >= 0 )
        {
            M->instances[i].ind = CFG_DefaultConfig->instances[inst_id].type;
        }
        else
        {
            if ( M->instances[i].ind == MSC_INSTANCE_UNDEF )
            {
                MSG_PrintError( MSG_NO_DEF_INSTANCE_s, name );
                fflush( MSG_ErrorStream );
                err++;
            }
            else
            {
                MSG_PrintError(MSG_CONFIG_UNDEF_INSTANCE_TYPE_suss,
                    MSC_GetString( M->msc_File, M ), M->instances[i].line_number, 
                    MSC_GetString( M->instances[i].name, M ), 
                    MSC_InstanceType2Str(M->instances[i].ind) );
            }

        }

    }

    return ( err == 0 );
}

/***************************************************************************
 * Function : tat_CheckMscSignals
 *
 * Description: checking signals for correctness
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_CheckMscSignals( MSC_STORAGE * M )
{
    size_t                  i, k, mess_count = 0, err = 0, mn, l;
    int                     j;

    struct
    {
        char                   *name;
        size_t                  line;
    }
                           *messc = NULL;

    char                   *s;

    if ( !M || !CFG_DefaultConfig )
        return RESULT_ERR;

    for ( i = 0; i < M->message_Count; i++ )
    {
        s = MSC_GetString( M->messages[i].message_Name, M );
        j = CFG_GetSignalID( s );
        if ( j >= 0 )
        {
        /* check number of parameters */
            if ( ( CFG_DefaultConfig->signals[j].param_Count < M->messages[i].param_Count ) &&
                CFG_DefaultConfig->signals[j].param_Extra != 1 )
            {
                MSG_PrintError( MSG_CONFIG_MESS_PARAMS_susuu,
                    MSC_GetString( M->msc_File, M ), M->messages[i].line_Number,
                    s, M->messages[i].param_Count, CFG_DefaultConfig->signals[j].param_Count );
                return RESULT_ERR;
            }
            else
            {
                mn = CFG_DefaultConfig->signals[j].param_Count;

                if ( mn > M->messages[i].param_Count )
                    mn = M->messages[i].param_Count;

                for ( k = 0; k < mn; k++ )
                {
                    s = MSC_GetString(M->messages[i].params_Values[k].varName, M);

                    if (s)
                    {
                        for ( l = 0; l < M->var_Count; l++ )
                        {
                            if ( !strcmp( s, MSC_GetString( M->vars[l].pv_Name, M ) ) )
                            {
                                if ( stricmp( CFG_DefaultConfig->signals[j].source_Format[k],
                                    MSC_GetString( M->vars[l].pv_Type, M ) ) &&
                                    ( !CFG_DefaultConfig->global.MatchTargetDataTypes ||
                                    ( CFG_DefaultConfig->global.MatchTargetDataTypes &&
                                    stricmp( CFG_DefaultConfig->signals[j].param_TgtType[k],
                                    MSC_GetString( M->vars[l].pv_Type, M ) ) ) ) )
                                {
                                    MSG_PrintError( MSG_CONFIG_VAR_DATATYPE_susss,
                                        MSC_GetString( M->msc_File, M ), M->messages[i].line_Number,
                                        MSC_GetString( M->vars[l].pv_Name, M ),
                                        CFG_DefaultConfig->signals[j].source_Format[k],
                                        MSC_GetString( M->vars[l].pv_Type, M ) );
                                    return RESULT_ERR;
                                };

                                break;
                            };
                        }

                        if ( l == M->var_Count )
                            return RESULT_ERR;
                        if ( CFG_DefaultConfig->signals[j].type == MSC_SIG_IN )
                        {
                            MSG_PrintError( MSG_CONFIG_VAR_ASSIGN_OUT_sus,
                                MSC_GetString( M->msc_File, M ), M->messages[i].line_Number,
                                MSC_GetString( M->messages[i].message_Params[k], M ) );
                            return RESULT_ERR;
                        }

                    }
                }
            }
        }
        else                    /* message not found */
        {
            /* add unknown signal to list */
            for ( k = 0; k < mess_count; k++ )
            {
                if ( !stricmp( messc[k].name, s ) )
                    break;
            }
            if ( k == mess_count )
            {
                MEM_ADD_OBJECT(messc, k);
                messc[k].name = s;
                messc[k].line = M->messages[i].line_Number;
                mess_count++;
            }
        }
    }
    /* dump first message only */
    if (mess_count)
    {
        MSG_PrintError( MSG_CONFIG_UNDEF_SIGNAL_sus,
            MSC_GetString( M->msc_File, M ), messc[0].line, messc[0] );
        err = 1;
    }
/*  uncomment this if multiple error reports required 
    for ( i = 0; i < mess_count; i++ )
    {
        MSG_PrintError( MSG_CONFIG_UNDEF_SIGNAL_sus,
            MSC_GetString( M->msc_File, M ), messc[i].line, messc[i] );
        err = 1;
    }
*/
    if ( messc )
        MEM_Free( messc );

    return err == 0;
}

/***************************************************************************
 * Function : tat_PrefixMscSignals
 *
 * Description: checking signals for correctness
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_PrefixMscSignals( MSC_STORAGE * M )
{
    size_t                  i, j;
    int                     sig, data;
    char                   *param;
    char                    dest[TAT_MAX_LINE_LEN];

    if ( !M || !CFG_DefaultConfig )
        return RESULT_ERR;

    for ( i = 0; i < M->message_Count; i++ )
    {
		sig = CFG_GetSignalID( MSC_GetString( M->messages[i].message_Name, M ) );
        if ( sig == -1 )
            return RESULT_ERR;

        for ( j = 0; ( j < CFG_DefaultConfig->signals[sig].param_Count ) &&
            ( j < M->messages[i].param_Count ); j++ )
        {
            param = MSC_GetString( M->messages[i].message_Params[j], M );
            if ( param && param[0] != '&' )
            {
                data = CFG_GetDataTypeID( CFG_DefaultConfig->signals[sig].source_Format[j] );
                if ( data != -1 && CFG_DefaultConfig->data[data].prefix )
                {
                    strcpy( dest, CFG_DefaultConfig->data[data].prefix );
                    strcat( dest, param );
                    M->messages[i].message_Params[j] = MSC_AddStr( dest, M );
                }
            }
        }
    }
    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_CheckMscSignalParams
 *
 * Description: checking signals for correctness
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_CheckMscSignalParams( MSC_STORAGE * M )
{
    size_t                  i, j, k;
    int                     sig, data;
    char                   *param;

    if ( !M || !CFG_DefaultConfig )
        return RESULT_ERR;

    for ( i = 0; i < M->message_Count; i++ )
    {
		sig = CFG_GetSignalID( MSC_GetString( M->messages[i].message_Name, M ) );
        if ( sig == -1 )
            return RESULT_ERR;

        for ( j = 0; ( j < CFG_DefaultConfig->signals[sig].param_Count ) &&
            ( j < M->messages[i].param_Count ); j++ )
        {
            param = MSC_GetString( M->messages[i].message_Params[j], M );
            if ( param && param[0] != '&' )
            {
                data = CFG_GetDataTypeID( CFG_DefaultConfig->signals[sig].source_Format[j] );

                if ( data != -1 && CFG_DefaultConfig->data[data].value_Count )
                {
                    for ( k = 0; k < CFG_DefaultConfig->data[data].value_Count; k++ )
                    {
                        if ( !strcmp( param, CFG_DefaultConfig->data[data].value_List[k] ) )
                            break;
                    }
                /* invalid value */
                    if ( k == CFG_DefaultConfig->data[data].value_Count )
                    {
                        MSG_PrintError( MSG_INVALID_VALUE_suss,
                            MSC_GetString( M->msc_File, M ), M->messages[i].line_Number,
                            param, CFG_DefaultConfig->signals[sig].source_Format[j] );

                        return RESULT_ERR;
                    }
                }
            }
        }
    }
    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_CheckCreateEvent
 *
 * Description: verify 'create' events in MSC
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_CheckCreateEvent( MSC_STORAGE * M, MSC_EVENT_KIND * E )
{
    int                     instance_id;

    if ( E->type == MSC_EV_CREATE )
    {
        instance_id = CFG_GetInstanceID( MSC_GetString( M->instances[E->data.create.instance].name, M ) );
        if ( instance_id == -1 )
            return RESULT_ERR;
        if ( ( E->data.create.param_Count > CFG_DefaultConfig->instances[instance_id].param_Count ) &&
            !CFG_DefaultConfig->instances[instance_id].param_Extra )
        {
            MSG_PrintError( MSG_CONFIG_MESS_PARAMS_susuu,
                MSC_GetString( M->msc_File, M ), E->line_Number,
                MSC_GetString( M->instances[E->data.create.instance].name, M ),
                E->data.create.param_Count, CFG_DefaultConfig->instances[instance_id].param_Count );
            return RESULT_ERR;
        }
    }
    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_MscCheckCreate
 *
 * Description: checking instance creations for correctness
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes:
 *
 **************************************************************************/
static LOGICAL tat_MscCheckCreate( MSC_STORAGE * M )
{
    if ( !M || !CFG_DefaultConfig )
        return RESULT_ERR;

    return MSC_Process( M, tat_CheckCreateEvent );
}

/***************************************************************************
 * Function : CFG_Check_MSC
 *
 * Description: Check MSC against configuration
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL CFG_Check_MSC( MSC_STORAGE * M, int flags )
{
    if ( !M || !CFG_DefaultConfig )
        return RESULT_ERR;

    if ( ( flags & CFG_MATCH_MESSAGE ) && !tat_CheckMscSignals( M ) )
    {
        return RESULT_ERR;
    }

    if ( ( flags & CFG_MATCH_MESSAGE_PREFIX ) && !tat_PrefixMscSignals( M ) )
    {
        return RESULT_ERR;
    }

    if ( ( flags & CFG_MATCH_CREATE ) && !tat_MscCheckCreate( M ) )
    {
        return RESULT_ERR;
    }

    if ( ( flags & CFG_MATCH_DATATYPE ) && !tat_MatchMscData( M ) )
    {
        return RESULT_ERR;
    }

    if ( ( flags & CFG_MATCH_INSTANCE ) && !tat_MatchMscInstance( M ) )
    {
        return RESULT_ERR;
    }

    if ( ( flags & CFG_MATCH_MESSAGE_PARAMS ) && !tat_CheckMscSignalParams( M ) )
    {
        return RESULT_ERR;
    }

    return RESULT_OK;
}
