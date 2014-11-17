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
* Functions:    main
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  MSC Trace generation
*
* Notes:        None
*
******************************************************************************/
#include <portable.h>
#include <cmdline.h>
#include <partial.h>
#include <genpath.h>
#include <mscbase.h>
#include <mscproc.h>
#include <msctime.h>
#include <fileproc.h>
#include <mscgen.h>
#include <outmess.h>
#include <flowgen.h>
#include <mscstrs.h>
#include <xmlconf.h>
#include <confproc.h>
#include <tatgdata.h>
#include <qmemory.h>
#include <mscmess.h>

#include "genmsc.h"

static                  DEF_MSG( tat_ProgramName, "Trace Generator" );

DEF_MSG( MSG_CONFIG_CAN_GENERATED_u, "REMARK: config: file '%s': %u file(s) can be generated\n" );

/* main part */
static UINT32_T             tat_MSCCount = 0;
static char            *tat_OutFile = NULL;
static char           **tat_MSCNames = NULL;
static char            *tat_InputDirectory = NULL;  /* input directory */
static char            *tat_ListFile = NULL;    /* file with list of generated files */
static char            *tat_XmlFile = NULL; /* XML file name */
static char            *tat_XmlConfigName = NULL;   /* tat_ConfigStruration name from XML */
static char            *tat_SchedModule = NULL; /* user's scheduling module */

static int             tat_UseTime = 0;    /* timer flag (0 - without time, 1 - with time) */
static int             tat_CompareTimeIntervals = 0;   /* 0 - compare minimal time, 1 - compare tat_CompareTimeIntervalsals */
static int             tat_UndefinedStartTime = 0; /* defstart for calculating absolute time */
static int             tat_FilterMessage = 0;  /* 0 - without tat_FilterMessage, 1 - with tat_FilterMessage */
static int             tat_UseInSys = 0;   /* 0 - in message for system, 1 - out message for system */
static int             tat_UseInEnv = 0;   /* 0 - in message for environment, 1 - out message for environment */
static int             tat_GlobalSync = 0; /* global syncronization */
static int             tat_RemoveGeneratedCond = 0;    /* remove generated conditions */
static int             tat_UseTimeDuration = 0;    /* use tat_UseTimeDuration during time calculation */
static int             tat_UnrollLoops = 0;    /* unroll loops with known number of iterations */
static int             tat_InlineLoops = 0;    /* inline loops with counter equal to 1 */
static int             tat_SendRecvOnly = 0;   /* remove all events except labeled and send/receive */
static int             tat_RemoveTimers = 0;   /* remove timers from output */
static int             tat_RemoveConds = 0;    /* remove conditions from output */
static int             tat_SyncActions = 0;    /* force actions syncronization */
static int             tat_SyncTimers = 0; /* force timers synchronization */

static MSC_GENPATH_PARAM gen_params = { 0, 0, 0, 0, 0, 0, 0, 0 };   /* internal schedule tat_ConfigStruration */

static int             tat_OutFloatTime = 0;   /* output floating point time after time calculation */
static int             tat_SplitFlowgenMess = 0;   /* split transaction to in/out for FlowGen */

static int             tat_SaveEnvSelf = 0;    /* allow message from env to env */
static int             tat_AllCondGlobal = 0;  /* make all condition events global */
static int             tat_TranspLabel = 0;    /* increase intervals, instead of generating labels */
static TAT_GLOBAL_CONFIG MSC_Config;   /* TAT global tat_ConfigStruration */
static int             timeonly = 0;
static int              start_Path = 0;
static int              path_Number = MSC_INFINITY_CONST;   /* by default - not restricted */
static int             expn_Flag = 0;

int                    MSC_OutCalcTime = 0;    /* calculation flag (0 - no calculation, 1 - calculate absolute time) */
static int             timer_OutFlag = 0;


CMD_LINE                cmd[] = {
    {"", CMD_GROUP_START, NULL, NULL, "Input file options:"},
    {"-i", CMD_PATH_NAME, NULL, &tat_InputDirectory, "set input directory"},
    {"-m", CMD_FILE_LIST, &tat_MSCCount, &tat_MSCNames, "set names of input files"},
    {"-config", CMD_FILE_NAME, NULL, &tat_XmlFile, "specify XML configuration file"},
    {"-cname", CMD_STRING, NULL, &tat_XmlConfigName, "set configuration name (default - \"default\")"},
    {"-sfg", CMD_BOOLEAN, NULL, &tat_SplitFlowgenMess, "split events in FlowGen files"},

    {"", CMD_GROUP_START, NULL, NULL, "Output file options:"},
    {"-o", CMD_PATH_NAME, NULL, &tat_OutFile, "output directory name"},
    {"-envself", CMD_BOOLEAN, NULL, &tat_SaveEnvSelf, "save events from environment to environment"},
    {"-list", CMD_FILE_NAME, NULL, &tat_ListFile, "specify file for list of generated files"},
    {"-n", CMD_NATURAL, NULL, &path_Number, "number of traces to generate"},

    {"-sro", CMD_BOOLEAN, NULL, &tat_SendRecvOnly, "output only send/receive/labeled events"},
    {"-rmvt", CMD_BOOLEAN, NULL, &tat_RemoveTimers, "exclude timers without labels from output"},
    {"-rmvc", CMD_BOOLEAN, NULL, &tat_RemoveConds, "exclude conditions without labels from output"},
    {"-gf", CMD_BOOLEAN, NULL, &gen_params.flag_CountTracesOnly,
        "generate output files (otherwise just count)"},
    {"-s", CMD_NATURAL, NULL, &start_Path, "skip specified number of traces"},
    {"-expn", CMD_BOOLEAN, NULL, &expn_Flag, "save operators with passive alternative"},

    {"", CMD_GROUP_START, NULL, NULL, "Time processing options:"},
    {"-tf", CMD_BOOLEAN, NULL, &tat_UseTime, "consider time while path generation"},
    {"-ct", CMD_BOOLEAN, NULL, &MSC_OutCalcTime, "calculate time"},
    {"-nosync", CMD_BOOLEAN, NULL, &tat_TranspLabel, "increase intervals instead of labels"},
    {"-interv", CMD_BOOLEAN, NULL, &tat_CompareTimeIntervals,
        "compare time intervals (minimal time - by default)"},
    {"-ndefst", CMD_BOOLEAN, NULL, &tat_UndefinedStartTime, "start time is {0..INF}, otherwise {0..0}"},
    {"-duration", CMD_BOOLEAN, NULL, &tat_UseTimeDuration, "use duration field for time calculation"},
    {"-to", CMD_BOOLEAN, NULL, &timeonly, "stop after time calculation"},
    {"-notime", CMD_BOOLEAN, NULL, &timer_OutFlag, "not output time declarations"},
    {"-floatt", CMD_BOOLEAN, NULL, &tat_OutFloatTime, "support floating point time in output"},

    {"", CMD_GROUP_START, NULL, NULL, "Loop processing options:"},
    {"-il", CMD_BOOLEAN, NULL, &tat_InlineLoops, "inline loops with counter equal to <1> into plain code"},
    {"-ul", CMD_BOOLEAN, NULL, &tat_UnrollLoops, "unroll loops with constant number of iterations"},

    {"", CMD_GROUP_START, NULL, NULL, "Synchronization options:"},
    {"-mf", CMD_BOOLEAN, NULL, &tat_FilterMessage, "association of in/out events into send/receive signals"},
    {"-is", CMD_BOOLEAN, NULL, &tat_UseInSys, "filter react on consuming messages from environment"},
    {"-ie", CMD_BOOLEAN, NULL, &tat_UseInEnv, "filter react on consuming messages from model"},
    {"-ec", CMD_BOOLEAN, NULL, &tat_AllCondGlobal, "make all condition events global"},

    {"", CMD_GROUP_START, NULL, NULL, "Trace generation options:"},
    {"-sa", CMD_NATURAL, NULL, &gen_params.flag_LimitActive, "restrict max active branches"},
    {"-sp", CMD_NATURAL, NULL, &gen_params.flag_LimitPassive, "restrict max passive branches"},
    {"-sf", CMD_BOOLEAN, NULL, &gen_params.flag_ActiveFirst, "active events in start"},
    {"-rf", CMD_BOOLEAN, NULL, &gen_params.flag_PassiveFirst, "passive events in start"},
    {"-nia", CMD_BOOLEAN, NULL, &gen_params.flag_ActionNonInterleave, "do not interleave actions"},
    {"-nic", CMD_BOOLEAN, NULL, &gen_params.flag_ConditionNonInterleave, "do not interleave conditions"},
/*    {"-sched", CMD_FILE_NAME, NULL, &tat_SchedModule, "specify user-defined scheduler"}, */

/* hidden options */
/* {"-splitc", CMD_INTEGER, NULL, &MSC_SplitCount,NULL}, */
    {"-!gsync", CMD_BOOLEAN, NULL, &tat_GlobalSync, NULL},
    {"-!rmvgc", CMD_BOOLEAN, NULL, &tat_RemoveGeneratedCond, NULL},
    {"-!synct", CMD_BOOLEAN, NULL, &tat_SyncTimers, NULL},
    {"-!synca", CMD_BOOLEAN, NULL, &tat_SyncActions, NULL},
    {"-!debug", CMD_BOOLEAN, NULL, &TAT_DEBUG_FILE_IO, NULL}

};

#define cmd_size (sizeof(cmd)/sizeof(cmd[0]))

static MSC_STORAGE  M;
static MSC_EVENT_TREE   TM;

/***************************************************************************
 * Function : tat_CheckCmd
 *
 * Description: check combinations of command-line options
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_CheckCmd(  )
{
    LOGICAL                 err = RESULT_OK;

    if ( tat_XmlFile == NULL )
    {
        MSG_PrintError( MSG_INVALID_SWITCH_VALUE_s, "-config" );
        err = RESULT_ERR;
    };

/* inverse parameter */
    gen_params.flag_CountTracesOnly = 1 - gen_params.flag_CountTracesOnly;

    if ( tat_FilterMessage && tat_UseTimeDuration )
    {
        MSG_PrintError( MSG_WARNING_SWITCH_USAGE_sss, "-duration", STR_WITH, "-mf" );
        tat_UseTimeDuration = 0;
    }

    if ( path_Number == 0 && !gen_params.flag_CountTracesOnly )
    {
        MSG_PrintError( MSG_INVALID_SWITCH_VALUE_s, "-n" );
        err = RESULT_ERR;
    }

    if ( !tat_MSCCount )
    {
        MSG_PrintError( MSG_INVALID_SWITCH_VALUE_s, "-m" );
        err = RESULT_ERR;
    }

    if ( gen_params.flag_ActiveFirst && gen_params.flag_PassiveFirst )
    {
        MSG_PrintError( MSG_INVALID_SWITCH_USAGE_ss, "-sf", "-rf" );
        err = RESULT_ERR;
    }
    if ( timeonly )
    {
        MSC_OutCalcTime = 1;
    }

    if ( timeonly && timer_OutFlag )
    {
        MSG_PrintError( MSG_WARNING_SWITCH_USAGE_sss, "-to", STR_WITH, "-notime" );
    }

    if ( !tat_UseTime && timer_OutFlag && MSC_OutCalcTime )
    {
        MSG_PrintError( MSG_WARNING_SWITCH_USAGE_sss, "-ct", STR_WITH, "-notime" );
    }

    if ( timeonly && tat_UseTime )
    {
        MSG_PrintError( MSG_WARNING_SWITCH_USAGE_sss, "-tf", STR_WITH, "-to" );
        tat_UseTime = 0;
    }

    if ( !MSC_OutCalcTime && tat_OutFloatTime )
    {
        MSG_PrintError( MSG_WARNING_SWITCH_USAGE_sss, "-floatt", STR_WITHOUT, "-ct" );
    }

    if ( tat_UseTimeDuration && !MSC_OutCalcTime )
    {
        MSG_PrintError( MSG_WARNING_SWITCH_USAGE_sss, "-duration", STR_WITHOUT, "-ct" );
    };

    if ( tat_UndefinedStartTime && !MSC_OutCalcTime )
    {
        MSG_PrintError( MSG_WARNING_SWITCH_USAGE_sss, "-ndefst", STR_WITHOUT, "-ct" );
    };

    if ( tat_CompareTimeIntervals && !tat_UseTime )
    {
        MSG_PrintError( MSG_WARNING_SWITCH_USAGE_sss, "-interv", STR_WITHOUT, "-tf" );
    };

    if ( tat_TranspLabel && !MSC_OutCalcTime )
    {
        MSG_PrintError( MSG_WARNING_SWITCH_USAGE_sss, "-nosync", STR_WITHOUT, "-ct" );
    }

    if ( tat_UseInSys && !tat_FilterMessage )
    {
        MSG_PrintError( MSG_WARNING_SWITCH_USAGE_sss, "-is", STR_WITHOUT, "-mf" );
    }

    if ( tat_UseInEnv && !tat_FilterMessage )
    {
        MSG_PrintError( MSG_WARNING_SWITCH_USAGE_sss, "-ie", STR_WITHOUT, "-mf" );
    }

    if ( tat_GlobalSync && tat_SyncTimers )
    {
        MSG_PrintError( MSG_WARNING_SWITCH_USAGE_sss, "-synct", STR_WITH, "-gsync" );
        tat_SyncTimers = 0;
    }

    if ( tat_GlobalSync && tat_SyncActions )
    {
        MSG_PrintError( MSG_WARNING_SWITCH_USAGE_sss, "-synca", STR_WITH, "-gsync" );
        tat_SyncActions = 0;
    }

    fflush( stdout );
    return err;
}

/***************************************************************************
 * Function : main
 *
 * Description: Invoke all activities
 *
 * Returns: exit code
 *
 * Notes: None
 *
 **************************************************************************/
int main( int argc, char *argv[] )
{
    static char             suitename[TAT_MAX_FNAME_LEN];
    static char             string[TAT_MAX_FNAME_LEN], mscname[TAT_MAX_FNAME_LEN], mscext[TAT_MAX_FNAME_LEN];
    UINT32_T                    i;
    int                     ExitCode = SYS_NORMAL_EXIT_CODE;
    UINT32_T                    transform_flags, time_flags, sfilter_flags, filter_flags, to_flags = 0;
    UINT32_T                    ct_transform = 0;
    int                     ignored_signals = 0;
    char                  **ignored_signame = NULL;

    TAT_XML_CONFIG          xmldata;
    TAT_XML_CONFIG_DEF     *config_data = NULL;

    /*****************************************************************************************/

    MSG_Copyright( tat_ProgramName, argv[0] );

    MSC_RegisterFormat(STR_DOTMPR,  MSC_Load,  MSC_Save);
    MSC_RegisterFormat(STR_DOTFG,  FG_Load,  FG_Save);

    memset( &M, 0, sizeof ( M ) );
    memset( &TM, 0, sizeof ( TM ) );
    memset( &xmldata, 0, sizeof ( xmldata ) );

/* check command-line */

    if ( ( argc == 1 ) || !CMD_Parse( cmd_size, ( CMD_LINE * ) & cmd[0], argc, argv ) )
    {
    /* output help message, if errors encounted */
        CMD_PrintHelp( cmd_size, ( CMD_LINE * ) & cmd[0] );
        return argc != 1;
    }

/* check required command-line parameters */
    if ( !tat_CheckCmd(  ) )    /* Check command line restrictions */
    {
        ExitCode = SYS_ERROR_EXIT_CODE;
        goto PExit;
    }

/* read tat_ConfigStruration from XML */
    if ( !TAT_ReadXML( tat_XmlFile, &xmldata ) )
    {
        ExitCode = SYS_ERROR_EXIT_CODE;
        goto PExit;
    };

    config_data = TAT_BuildConfig( ( tat_XmlConfigName ) ? tat_XmlConfigName : STR_XML_DEFAULT, &xmldata );

    if ( !config_data )
    {
        ExitCode = SYS_ERROR_EXIT_CODE;
        goto PExit;
    };

    config_data->global = MSC_Config;

/* set this configuration as default */
    CFG_SetDefaultConfig( config_data );

/* remove file if exists */
    remove( tat_OutFile );

/* remove tat_ListFile, if specified */
    if ( tat_ListFile )
        remove( tat_ListFile );

/* add ignored signals to list */
    for ( i = 0; i < ( int ) config_data->signal_Count; i++ )
    {
        if ( config_data->signals[i].type == MSC_SIG_IGNORE )
        {
            ignored_signame = MEM_Reallocate( ignored_signame, ( ignored_signals + 1 ) * sizeof ( char * ) );
            ignored_signame[ignored_signals++] = config_data->signals[i].name;
        }
    }

/* select processing flags */
    time_flags = MSCTIME_ZERO_PAR_TIME;

    if ( !tat_UndefinedStartTime )
        time_flags |= MSCTIME_DEFINED_START;

    if ( tat_UseTimeDuration )
        time_flags |= MSCTIME_USE_DURATION;

    if ( tat_TranspLabel )
        time_flags |= MSCTIME_TRANS_LABEL;

    if ( !tat_OutFloatTime )
        time_flags |= MSCTIME_TRUNCATE;

    transform_flags = MSC_TRANS_REMOVE_ZERO_LOOP | MSC_TRANS_USE_COND;

    if ( !tat_GlobalSync )
        transform_flags |= MSC_TRANS_USE_SYNC;

    if ( tat_SyncTimers )
        transform_flags |= MSC_TRANS_TIMER_SYNC;

    if ( tat_SyncActions )
        transform_flags |= MSC_TRANS_ACTION_SYNC;

    if ( tat_AllCondGlobal )
        transform_flags |= MSC_TRANS_EXPN_CONDITION;

    filter_flags = GP_NONE;

    if ( tat_FilterMessage )
        filter_flags |= GP_FILTER_SR_EVENTS;

    if ( tat_RemoveGeneratedCond )
        filter_flags |= GP_FILTER_GEN_COND;

    if ( tat_RemoveTimers )
        filter_flags |= GP_FILTER_TIMERS;

    if ( tat_RemoveConds )
        filter_flags |= GP_FILTER_CONDITIONS;

    if ( tat_SendRecvOnly )
        filter_flags |= GP_FILTER_ESR_EVENTS;

    if ( tat_SaveEnvSelf )
        filter_flags |= GP_SAVE_ENVSELF;

    if ( tat_RemoveTimers || tat_SendRecvOnly )
    {
        ct_transform |= MSC_TRANS_MOVE_LAB_TM;
    }

    if ( tat_SendRecvOnly )
    {
        ct_transform |= MSC_TRANS_MOVE_LAB_CS;
    }

/* set -timeonly specific flags */
    if ( timeonly )
    {
        to_flags = MSC_TRANS_REM_GEN_CND; 


        if ( tat_SendRecvOnly )
        {
            to_flags |= MSC_TRANS_SRO;
        }

        if ( tat_RemoveTimers )
        {
            to_flags |= MSC_TRANS_REM_FR_TMR;
        }

        if ( tat_RemoveConds )
        {
            to_flags |= MSC_TRANS_REM_FR_COND;
        }
    }

    sfilter_flags = filter_flags;

    for ( i = 0; i < tat_MSCCount; i++ )
    {
        filter_flags = sfilter_flags;
    /* get full file path */

        if ( tat_InputDirectory != NULL )
        {
            strcpy( string, tat_InputDirectory );
            FP_SplitPath( tat_MSCNames[i], NULL, mscname, mscext );
            strcat( string, mscname );
            strcat( string, mscext );
        }
        else
        {
            strcpy( string, tat_MSCNames[i] );
        }

        if ( !MSC_Load( string, &M, 1 - tat_SplitFlowgenMess ) )
        {
            ExitCode = SYS_ERROR_EXIT_CODE;
            goto PExit;
        }
/*
        if ( M.source_Format && tat_SplitFlowgenMess && !tat_FilterMessage )
        {
            if ( tat_UseInSys )
                filter_flags |= GP_FILTER_OUT_MESS;
            else
                filter_flags |= GP_FILTER_IN_MESS;
        }
*/
        if ( !CFG_Check_MSC( &M, CFG_MATCH_INSTANCE ) &&
            ( tat_FilterMessage || config_data->instance_Count ) )
        {
            MSC_FreeMemory( &M );
            ExitCode = SYS_ERROR_EXIT_CODE;
            goto PExit;
        };

        MSC_Transform( &M, transform_flags );

    /* do it after expanding */
        if ( tat_GlobalSync )
            MSC_Transform( &M, MSC_TRANS_GLOBAL_SYNC );

        if ( tat_UnrollLoops )
            MSC_Transform( &M, MSC_TRANS_UNROLL_LOOP );

        if ( tat_InlineLoops )
            MSC_Transform( &M, MSC_TRANS_INLINE_LOOP );

        if ( MSC_OutCalcTime || timeonly )
        {
            if ( !MSC_CalcTime( &M, time_flags ) )
            {
                ExitCode = SYS_ERROR_EXIT_CODE;
                goto PExit;
            };
        };

        MSC_Transform( &M, ct_transform );

        if ( !timeonly )
        {
            MSC_Transform( &M, MSC_TRANS_OPER_SYNC );

            if ( !MSC_BuildLinearPart( &M, &TM, tat_UseTime, tat_CompareTimeIntervals, 2 | expn_Flag ) )
            {
                ExitCode = SYS_ERROR_EXIT_CODE;
                goto PExit;
            };

        /* use pre-tat_FilterMessage */
            if ( !MSC_ReduceEventTreeSigIgn( &M, &TM, filter_flags, tat_UseInSys,
                    tat_UseInEnv, ignored_signals, ignored_signame ) )
            {
                ExitCode = SYS_ERROR_EXIT_CODE;
                goto PExit;
            }

            if ( !MSC_ResearchPaths( &TM, gen_params, path_Number ) )
            {
                ExitCode = SYS_ERROR_EXIT_CODE;
                goto PExit;
            }

        /* generate complete test paths */
            if ( !MSCEXPN_SavePath( &M, &TM, start_Path, path_Number, 1 - gen_params.flag_CountTracesOnly,
                    ( ( 1 - timer_OutFlag ) ) | ( MSC_OutCalcTime << 1 ), tat_ListFile, tat_OutFile ) )
            {
                ExitCode = SYS_ERROR_EXIT_CODE;
                goto PExit;
            }

        }
        else
        {
            char                   *s;
            char                    filename[TAT_MAX_FNAME_LEN], fullname[TAT_MAX_FNAME_LEN];

            s = MSC_GetString( M.msc_File, &M );

            if ( !s )
            {
                return SYS_ERROR_EXIT_CODE;
            }

            FP_SplitPath( s, NULL, fullname, NULL );

        /* form a new file name */
            sprintf( filename, "%sN0", fullname );

            if ( tat_OutFile != NULL )
            {
                sprintf( fullname, "%s%s%s", tat_OutFile, filename, STR_DOTMPR );
            }
            else
            {
                sprintf( fullname, "%s%s", filename, STR_DOTMPR );
            }

            MSC_Transform( &M, to_flags ); 

            MSC_Save( fullname, &M,
                ( ( timer_OutFlag ) ? MSC_MPR_STD_OUT : MSC_MPR_TIMED_OUT ) | MSC_MPR_OUTPUT_CALCTIME );
        }

    /* free memory from MSC_EVENT_TREE */
        MSC_CleanLinearPart( &TM );

        MSC_FreeMemory( &M );
    }

  PExit:

    if ( ExitCode != SYS_NORMAL_EXIT_CODE )
    {
        if ( tat_ListFile )
            remove( tat_ListFile );
        remove( tat_OutFile );
    }

/*    MEM_Dump(); */
    TAT_FreeConfig( &xmldata );

    if ( ignored_signame )
        MEM_Free( ignored_signame );

    return ExitCode;
}
