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
* Description:  Abstract Test Generator
*
* Notes:        None
*
******************************************************************************/
#include <portable.h>
#include <qmemory.h>
#include <mscbase.h>


#include <fileproc.h>
#include <strproc.h>

#include <partial.h>
#include <genpath.h>
#include <mscparse.h>
#include <mscproc.h>
#include <msctime.h>
#include <outmess.h>
#include <cmdline.h>
#include <testtree.h>
#include <flowgen.h>
#include <xmlconf.h>
#include <confproc.h>
#include <mscstrs.h>
#include <tatgdata.h>
#include <mscmess.h>
#include <mscgen.h>

#include "stategen.h"
#include "sigopt.h"
#include "testopt.h"
#include "utrhead.h"
#include "utgflags.h"
#include "mscchk.h"
#include "gendebug.h"

static                  DEF_MSG( tat_ProgramName, "Abstract Test Generator" );

static FILE            *tat_OutStream = NULL;

/* main part */
static size_t             tat_MSCCount = 0;
static size_t             tat_IncludeCount = 0;
static char            *tat_OutFile = NULL;
static char           **tat_MSCNames = NULL;
static char           **tat_IncludeNames = NULL;
static char            *tat_InputDirectory = NULL;  /* input directory */
static char            *tat_ConfigStr = NULL;   /* tat_ConfigStruration string */
static char            *tat_ListFile = NULL;    /* file with list of generated files */
static char            *tat_XmlFile = NULL; /* XML file name */
static char            *tat_XmlConfigName = NULL;   /* tat_ConfigStruration name from XML */
static char            *tat_SchedModule = NULL; /* user's scheduling module */
static char            *tat_PrefixStr = NULL;   /* tat_PrefixStr test case names in ATS */

static Bool             tat_UseTime = 0;    /* timer flag (0 - without time, 1 - with time) */
static Bool             tat_CompareTimeIntervals = 0;   /* 0 - compare minimal time, 1 - compare tat_CompareTimeIntervalsals */
static Bool             tat_UndefinedStartTime = 0; /* defstart for calculating absolute time */
static Bool             tat_FilterMessage = 0;  /* 0 - without tat_FilterMessage, 1 - with tat_FilterMessage */
static Bool             tat_UseInSys = 0;   /* 0 - in message for system, 1 - out message for system */
static Bool             tat_UseInEnv = 0;   /* 0 - in message for environment, 1 - out message for environment */
static Bool             tat_GlobalSync = 0; /* global syncronization */
static Bool             tat_RemoveGeneratedCond = 0;    /* remove generated conditions */
static Bool             tat_UseTimeDuration = 0;    /* use tat_UseTimeDuration during time calculation */
static Bool             tat_UnrollLoops = 0;    /* unroll loops with known number of iterations */
static Bool             tat_SendRecvOnly = 0;   /* remove all events except labeled and send/receive */
static Bool             tat_RemoveTimers = 0;   /* remove timers from output */
static Bool             tat_RemoveConds = 0;    /* remove conditions from output */
static Bool             tat_SyncActions = 0;    /* force actions syncronization */
static Bool             tat_SyncTimers = 0; /* force timers synchronization */

static MSC_GENPATH_PARAM gen_params = { 0, 0, 0, 0, 0, 0, 1, 0 };   /* internal schedule tat_ConfigStruration */

static Bool             tat_OutFloatTime = 0;   /* output floating point time after time calculation */
static Bool             tat_SplitFlowgenMess = 0;   /* split transaction to in/out for FlowGen */
static Bool             tat_DirectATS = 0;  /* use MSC-direct output format */

static Bool             tat_SaveEnvSelf = 0;    /* allow message from env to env */
static Bool             tat_TTS_ATS = 0;    /* TTS generation */
static Bool             tat_AllCondGlobal = 0;  /* make all condition events global */
static Bool             tat_TranspLabel = 0;    /* increase intervals, instead of generating labels */
static TAT_GLOBAL_CONFIG MSC_Config;   /* TAT global Configuration */
static Bool             tat_DebugInfo; /* output debug info */

Bool                    MSC_SingleMessTable = 0;    /* output single message table for several test cases */
Bool                    MSC_OutCalcTime = 0;    /* calculation flag (0 - no calculation, 1 - calculate absolute time) */
Bool                    MSC_CountMaxPath = 0;   /* count maximal number of iterations for test case */

static Bool             tat_SingleTestCase = 0; /* output separate ATS for each MSC */

Bool					tat_ASN1 = 0;			/* Enable ASN.1 processing in MSC */

CMD_LINE                cmd[] = {
    {"", CMD_GROUP_START, NULL, NULL, "Input file options:"},
    {"-i", CMD_PATH_NAME, NULL, &tat_InputDirectory, "set input directory"},
    {"-m", CMD_FILE_LIST, &tat_MSCCount, &tat_MSCNames, "set names of input files"},
    {"-incl", CMD_FILE_LIST, &tat_IncludeCount, &tat_IncludeNames, "set included files"},
    {"-config", CMD_FILE_NAME, NULL, &tat_XmlFile, "specify XML configuration file"},
    {"-cname", CMD_STRING, NULL, &tat_XmlConfigName, "set configuration name (default - \"default\")"},
    {"-tt", CMD_BOOLEAN, NULL, &MSC_Config.MatchTargetDataTypes, "check variables via target types too"},
    {"-sfg", CMD_BOOLEAN, NULL, &tat_SplitFlowgenMess, "split events in FlowGen files"},
	{"-asn1",CMD_BOOLEAN, NULL, &tat_ASN1, "enable ASN.1 processing in MSC"},

    {"", CMD_GROUP_START, NULL, NULL, "Output file options:"},
    {"-o", CMD_FILE_NAME, NULL, &tat_OutFile, "specify output file name"},
    {"-conf", CMD_STRING, NULL, &tat_ConfigStr, "set configuration tag specification"},
    {"-envself", CMD_BOOLEAN, NULL, &tat_SaveEnvSelf, "save events from environment to environment"},
    {"-list", CMD_FILE_NAME, NULL, &tat_ListFile, "specify file for list of generated files"},
    {"-prefix", CMD_STRING, NULL, &tat_PrefixStr, "prefix for test case names in ATS"},

    {"-optexc", CMD_BOOLEAN, NULL, &MSC_OptimizeExclude, "optimize 'exc' inline expressions"},
    {"-state", CMD_BOOLEAN, NULL, &tat_TTS_ATS, "generate TTS ATS"},
    {"-flat", CMD_BOOLEAN, NULL, &tat_DirectATS, "generate direct ATS"},
    {"-cm", CMD_BOOLEAN, NULL, &MSC_CountMaxPath, "use max. possible number of paths for testcase"},
    {"-sro", CMD_BOOLEAN, NULL, &tat_SendRecvOnly, "output only send/receive/labeled events"},
    {"-rmvt", CMD_BOOLEAN, NULL, &tat_RemoveTimers, "exclude timers without labels from output"},
    {"-rmvc", CMD_BOOLEAN, NULL, &tat_RemoveConds, "exclude conditions without labels from output"},
    {"-smt", CMD_BOOLEAN, NULL, &MSC_SingleMessTable, "single message table for test cases"},

    {"", CMD_GROUP_START, NULL, NULL, "Time processing options:"},
    {"-tf", CMD_BOOLEAN, NULL, &tat_UseTime, "consider time while path generation"},
    {"-ct", CMD_BOOLEAN, NULL, &MSC_OutCalcTime, "calculate time"},
    {"-nosync", CMD_BOOLEAN, NULL, &tat_TranspLabel, "increase intervals instead of labels"},
    {"-interv", CMD_BOOLEAN, NULL, &tat_CompareTimeIntervals,
        "compare time intervals (minimal time - by default)"},
    {"-ndefst", CMD_BOOLEAN, NULL, &tat_UndefinedStartTime, "start time is {0..INF}, otherwise {0..0}"},
    {"-duration", CMD_BOOLEAN, NULL, &tat_UseTimeDuration, "use duration field for time calculation"},
    {"-floatt", CMD_BOOLEAN, NULL, &tat_OutFloatTime, "support floating point time in output"},

    {"", CMD_GROUP_START, NULL, NULL, "Loop processing options:"},
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
	
/* hidden options */
/* {"-splitc", CMD_INTEGER, NULL, &MSC_SplitCount,NULL}, */
/* {"-sched", CMD_FILE_NAME, NULL, &tat_SchedModule, "specify user-defined scheduler"}, */
    {"-g", CMD_BOOLEAN, NULL, &tat_DebugInfo, "generate debug info for MSC"},
    {"-dbgio", CMD_BOOLEAN, NULL, &TAT_DEBUG_FILE_IO, NULL},
    {"-!rmvgc", CMD_BOOLEAN, NULL, &tat_RemoveGeneratedCond, NULL},
    {"-!gsync", CMD_BOOLEAN, NULL, &tat_GlobalSync, NULL},
    {"-!synct", CMD_BOOLEAN, NULL, &tat_SyncTimers, NULL},
    {"-!synca", CMD_BOOLEAN, NULL, &tat_SyncActions, NULL}
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

    if ( tat_OutFile == NULL )
    {
        fprintf( MSG_ErrorStream, MSG_INVALID_SWITCH_VALUE_s, "-o" );
        err = RESULT_ERR;
    };

    if ( tat_XmlFile == NULL )
    {
        fprintf( MSG_ErrorStream, MSG_INVALID_SWITCH_VALUE_s, "-config" );
        err = RESULT_ERR;
    };

    if ( tat_TTS_ATS && tat_DirectATS )
    {
        fprintf( MSG_ErrorStream, MSG_INVALID_SWITCH_USAGE_ss, "-state", "-flat" );
        err = RESULT_ERR;
    };

    if ( tat_FilterMessage && tat_UseTimeDuration )
    {
        fprintf( MSG_ErrorStream, MSG_WARNING_SWITCH_USAGE_sss, "-duration", STR_WITH, "-mf" );
        tat_UseTimeDuration = 0;
    }

    if ( gen_params.flag_ActiveFirst && gen_params.flag_PassiveFirst )
    {
        fprintf( MSG_ErrorStream, MSG_INVALID_SWITCH_USAGE_ss, "-sf", "-rf" );
        err = RESULT_ERR;
    }

    if ( !MSC_OutCalcTime && tat_OutFloatTime )
    {
        fprintf( MSG_ErrorStream, MSG_WARNING_SWITCH_USAGE_sss, "-floatt", STR_WITHOUT, "-ct" );
    }

    if ( tat_UseTimeDuration && !MSC_OutCalcTime )
    {
        fprintf( MSG_ErrorStream, MSG_WARNING_SWITCH_USAGE_sss, "-duration", STR_WITHOUT, "-ct" );
    };

    if ( tat_UndefinedStartTime && !MSC_OutCalcTime )
    {
        fprintf( MSG_ErrorStream, MSG_WARNING_SWITCH_USAGE_sss, "-ndefst", STR_WITHOUT, "-ct" );
    };

    if ( tat_CompareTimeIntervals && !tat_UseTime )
    {
        fprintf( MSG_ErrorStream, MSG_WARNING_SWITCH_USAGE_sss, "-interv", STR_WITHOUT, "-tf" );
    };

    if ( tat_UseInSys && !tat_FilterMessage )
    {
        fprintf( MSG_ErrorStream, MSG_WARNING_SWITCH_USAGE_sss, "-is", STR_WITHOUT, "-mf" );
    }

    if ( tat_TranspLabel && !MSC_OutCalcTime )
    {
        fprintf( MSG_ErrorStream, MSG_WARNING_SWITCH_USAGE_sss, "-nosync", STR_WITHOUT, "-ct" );
    }

    if ( tat_UseInEnv && !tat_FilterMessage )
    {
        fprintf( MSG_ErrorStream, MSG_WARNING_SWITCH_USAGE_sss, "-ie", STR_WITHOUT, "-mf" );
    }

    if ( tat_GlobalSync && tat_SyncTimers )
    {
        fprintf( MSG_ErrorStream, MSG_WARNING_SWITCH_USAGE_sss, "-synct", STR_WITH, "-gsync" );
        tat_SyncTimers = 0;
    }

    if ( tat_GlobalSync && tat_SyncActions )
    {
        fprintf( MSG_ErrorStream, MSG_WARNING_SWITCH_USAGE_sss, "-synca", STR_WITH, "-gsync" );
        tat_SyncActions = 0;
    }

    fflush( MSG_ErrorStream );
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
    size_t                     i;
    int                     ExitCode = SYS_NORMAL_EXIT_CODE;
    UINT32_T                    transform_flags, time_flags, sfilter_flags, filter_flags;
    FILE                   *listf = NULL;
    int                     ignored_signals = 0;
    char                  **ignored_signame = NULL;

    TAT_XML_CONFIG          xmldata;
    TAT_XML_CONFIG_DEF     *config_data = NULL;
    struct MSC_TEST_CASE   *tat_test;

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

/* read configuration from XML */
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

/* set this tat_ConfigStruration as default */
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

    tat_OutStream = FP_OpenFile( tat_OutFile, "w" );

    if ( !tat_OutStream )
    {
        ExitCode = SYS_ERROR_EXIT_CODE;
        goto PExit;
    }

/* implement -list switch */
    if ( tat_ListFile )
    {
        listf = FP_OpenFile( tat_ListFile, "w" );
        if ( !listf )
        {
            ExitCode = SYS_ERROR_EXIT_CODE;
            goto PExit;
        }
        fprintf( listf, "%s\n", tat_OutFile );
        fclose( listf );
    }

    FP_SplitPath( tat_OutFile, NULL, suitename, NULL );

    fflush( stdout );

/* use indirect call further */
    if ( !MSC_AtsTCL_StartTestSuite
        ( tat_OutStream, argc, argv, tat_ConfigStr, suitename, tat_MSCCount, tat_MSCNames, tat_IncludeCount,
            tat_IncludeNames, config_data, tat_PrefixStr ) )
    {
        ExitCode = SYS_ERROR_EXIT_CODE;
        goto PExit;
    }

    fflush( stdout );

    if ( MSC_SingleMessTable )
    {
        for ( i = 0; i < tat_MSCCount; i++ )
        {
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

            if ( !TAT_Load( string, &M, 1 - tat_SplitFlowgenMess ) )
            {
                ExitCode = SYS_ERROR_EXIT_CODE;
                goto PExit;
            }

            MSC_FillMessageMap( &M );
            MSC_FreeSigMap(  );
            MSC_FreeMemory( &M );
        };
        MSC_AtsGenerateSigDef( tat_OutStream );
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
        transform_flags |= MSC_TRANS_MOVE_LAB_TM;
    }

    if ( tat_SendRecvOnly )
    {
        transform_flags |= MSC_TRANS_MOVE_LAB_CS;
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

    /* prevent from duplicated output of semantic warnings */
        if ( MSC_SingleMessTable )
            MSG_EnableSemanticWarning( 0 );
        if ( !TAT_Load( string, &M, 1 - tat_SplitFlowgenMess ) )
        {
            ExitCode = SYS_ERROR_EXIT_CODE;
            goto PExit;
        }
        if ( MSC_SingleMessTable )
            MSG_EnableSemanticWarning( 1 );

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

        if ( !CFG_Check_MSC( &M, CFG_MATCH_DATATYPE | CFG_MATCH_MESSAGE | CFG_MATCH_CREATE ) )
        {
            ExitCode = SYS_ERROR_EXIT_CODE;
            goto PExit;
        };

        if ( !CFG_Check_MSC( &M, CFG_MATCH_MESSAGE_PREFIX ) )
        {
            ExitCode = SYS_ERROR_EXIT_CODE;
            goto PExit;
        }

        if ( !CFG_Check_MSC( &M, CFG_MATCH_MESSAGE_PARAMS ) )
        {
            ExitCode = SYS_ERROR_EXIT_CODE;
            goto PExit;
        }

        
        MSC_Transform( &M, transform_flags );

    /* do it after expanding */
        
        if ( tat_GlobalSync )
            MSC_Transform( &M, MSC_TRANS_GLOBAL_SYNC );

        
        if ( tat_UnrollLoops )
            MSC_Transform( &M, MSC_TRANS_UNROLL_LOOP );

        
        MSC_Transform( &M, MSC_TRANS_INLINE_LOOP );

        if ( MSC_OutCalcTime )
        {
            if ( !MSC_CalcTime( &M, time_flags ) )
            {
                ExitCode = SYS_ERROR_EXIT_CODE;
                goto PExit;
            };
        };

        
        MSC_FillMessageMap( &M );

        if ( tat_DirectATS )
        {
            UINT32_T                    mflags = 0;

            if ( tat_UseInSys )
                mflags |= MSC_CHECK_INSYS;

            if ( tat_UseInEnv )
                mflags |= MSC_CHECK_INENV;

            if ( tat_RemoveGeneratedCond )
                mflags |= MSC_CHECK_DEL_COND;

            if ( tat_SaveEnvSelf )
                mflags |= MSC_CHECK_ENVSELF;

            if ( !tat_FilterMessage )
                mflags |= MSC_CHECK_SAVESR;

            if ( tat_RemoveConds )
                mflags |= MSC_CHECK_DEL_ALL_COND;

            if ( tat_RemoveTimers )
                mflags |= MSC_CHECK_DEL_TIMERS;

        /* do the same as MSC_ReduceEventTree */
            MSC_ReduceEvents( &M, mflags );
            MSC_AtsGenTestReduced( tat_OutStream, &M, tat_PrefixStr );
        }
        else
        {
            MSC_Transform( &M, MSC_TRANS_OPER_SYNC );

            if ( !MSC_BuildLinearPart( &M, &TM, tat_UseTime, tat_CompareTimeIntervals, 2 ) )
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

            if ( !MSC_CheckEvents( &M, &TM, config_data ) )
            {
                ExitCode = SYS_ERROR_EXIT_CODE;
                goto PExit;
            };

            if ( !MSC_ResearchPaths( &TM, gen_params, MSC_INFINITY_CONST ) )
            {
                ExitCode = SYS_ERROR_EXIT_CODE;
                goto PExit;
            }

            tat_test = MSC_BuildExecutionSuperTree( &M, &TM );

            
            if ( !tat_TTS_ATS )
                MSC_AtsGenTest( tat_OutStream, &M, tat_test, tat_PrefixStr );
            else
            /* generate state representation of test */
                MSC_GenTestTTS( tat_OutStream, &M, tat_test, tat_PrefixStr );

        /* free memory from MSC_EVENT_TREE */
            MSC_CleanLinearPart( &TM );
            MSC_FreeExecutionSuperTree( tat_test );
        };                       /* if (tat_DirectATS) */

    /* free signal memory */
        if ( !MSC_SingleMessTable )
            MSC_FreeSigOpt(  );

        MSC_FreeSigMap(  );

        if ( tat_DebugInfo )
        {
            if ( !MSC_GenDebug( config_data, &M ) )
            {
                ExitCode = SYS_ERROR_EXIT_CODE;
                goto PExit;
            };
        }

        MSC_FreeMemory( &M );
    }

    ;                           /* tat_MSCCount */
    if ( MSC_SingleMessTable )
        MSC_FreeSigOpt(  );

    if ( !tat_SingleTestCase )
    {
        if ( !MSC_AtsTCL_FinishTestSuite( tat_OutStream, argc, argv, tat_ConfigStr, suitename,
                tat_MSCCount, tat_MSCNames, tat_IncludeCount, tat_IncludeNames, config_data, tat_PrefixStr ) )
        {
            ExitCode = SYS_ERROR_EXIT_CODE;
            goto PExit;
        }

        fclose( tat_OutStream );
        tat_OutStream = NULL;
    }

  PExit:

    if ( tat_OutStream )
        fclose( tat_OutStream );
    if ( ExitCode != SYS_NORMAL_EXIT_CODE )
    {
        if ( tat_ListFile )
            remove( tat_ListFile );

        remove( tat_OutFile );
    }

    TAT_FreeConfig( &xmldata );

    if ( ignored_signame )
        MEM_Free( ignored_signame );

    return ExitCode;
}
