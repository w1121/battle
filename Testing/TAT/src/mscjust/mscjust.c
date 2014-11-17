/******************************************************************************
*
*       COPYRIGHT 2001-2002 MOTOROLA, ALL RIGHTS RESERVED
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
* Description:  MSC verification, convertion
*
* Notes:        None
*
******************************************************************************/

#include <portable.h>
#include <mscbase.h>
#include <fileproc.h>
#include <cmdline.h>
#include <mscproc.h>
#include <mscgen.h>
#include <msctime.h>
#include <qmemory.h>
#include <outmess.h>
#include <mscstrs.h>
#include <tatgdata.h>
#include <mscmess.h>
#include <flowgen.h>

static                  DEF_MSG( tat_ProgramName, "MSC Transformer" );

static size_t           tat_MscCount = 0;
static int              flag_Ext = 0;
static int              flag_SaveName = 0;
static int              flag_OutMsc = 0;
static int              flag_OutFlowgen = 0;
static int              flag_Verify = 0;

static char            *prefix = NULL;

static char           **tat_MscNames = NULL;

static char            *tat_Output = NULL;
static char            *tat_ListFile = NULL;
static int              flag_Nsrf = 0;
static int              flag_Mark = 0;
static int              flag_Verdict = 0;
static int              flag_VerdTime = 0;
static int              flag_CommTime = 0;

static MSC_STORAGE  msc_X;

CMD_LINE                cmd[] = {
    {"-m", CMD_FILE_LIST, &tat_MscCount, &tat_MscNames, "set input file names"},
    {"-mmsc", CMD_BOOLEAN, NULL, &flag_Ext, "output macro MSC extensions"},
    {"-mark", CMD_BOOLEAN, NULL, &flag_Mark, "add debug ID to events"},
    {"-nsrf", CMD_BOOLEAN, NULL, &flag_Nsrf, "do not output source references"},
    {"-prefix", CMD_STRING, NULL, &prefix, "prefix to output file names"},
    {"-savename", CMD_BOOLEAN, NULL, &flag_SaveName, "save values of 'msc' field"},
    {"-ompr", CMD_BOOLEAN, NULL, &flag_OutMsc, "force mpr output"},
    {"-ofg", CMD_BOOLEAN, NULL, &flag_OutFlowgen, "force flowgen output"},
    {"-verf", CMD_BOOLEAN, NULL, &flag_Verify, "verify only"},
    {"-verd", CMD_BOOLEAN, NULL, &flag_Verdict, "output verdict"},
    {"-vtm", CMD_BOOLEAN, NULL, &flag_VerdTime, "substitute time with verdict time"},
    {"-comtm", CMD_BOOLEAN, NULL, &flag_CommTime, "output time in MSC comments"},
    {"-list", CMD_FILE_NAME, NULL, &tat_ListFile, "specify file for list of generated files"}
};

#define cmd_params sizeof(cmd)/sizeof(cmd[0])

static UINT32_T             event_Count;

static tat_MarkComment( MSC_STORAGE * M, MSC_EVENT_KIND * s )
{
    s->event_Mark = event_Count;
    event_Count++;
    return RESULT_OK;
}


/***************************************************************************
* Function : main
*
* Description: main function
*
* Returns: exit code
*
* Notes: None
*
**************************************************************************/
int main( int argc, char *argv[] )
{
    UINT32_T                    i, j;
    char                    output_name[TAT_MAX_FNAME_LEN];
    char                    path[TAT_MAX_FNAME_LEN], name[TAT_MAX_FNAME_LEN], count[TAT_MAX_FNAME_LEN];
    FILE                   *listf = NULL;
    int                     flags;
    int                     fg_format, mpr_format;

    MSG_Copyright( tat_ProgramName, argv[0] );
    mpr_format = MSC_RegisterFormat(STR_DOTMPR,  MSC_Load,  MSC_Save);
    fg_format = MSC_RegisterFormat(STR_DOTFG,  FG_Load,  FG_Save);

    if ( ( argc == 1 ) || !CMD_Parse( cmd_params, ( CMD_LINE * ) & cmd, argc, argv ) )
    {
        CMD_PrintHelp( cmd_params, ( CMD_LINE * ) & cmd );
        return argc != 1;
    };

    if ( !tat_MscCount )
    {
        MSG_PrintError( MSG_INVALID_SWITCH_VALUE_s, "-m" );
        return SYS_ERROR_EXIT_CODE;
    }

    flags = ( flag_Ext ) ? MSC_MPR_TIMED_OUT : MSC_MPR_STD_OUT;

    if ( flag_SaveName )
    {
        flags &= ~MSC_MPR_CORRECT_NAME;
    }

    if ( flag_Verdict )
    {
        flags |= MSC_MPR_OUTPUT_VERDICT;
    }

    if ( flag_VerdTime )
    {
        flags |= MSC_MPR_OUTPUT_VRSTIME;
    }

    if ( flag_Nsrf )
    {
        flags &= ~MSC_MPR_OUTPUT_SREF;
    }

    if  (flag_CommTime)
    {
        flags |= MSC_MPR_OUTPUT_EVCOMMENT;
    }
    
    if ( flag_OutMsc && flag_OutFlowgen )
    {
        MSG_PrintError( MSG_INVALID_SWITCH_USAGE_ss, "-ofg", "-ompr" );
        return SYS_ERROR_EXIT_CODE;
    }

    if ( flag_OutFlowgen && flag_Ext )
    {
        MSG_PrintError( MSG_INVALID_SWITCH_USAGE_ss, "-ofg", "-mmsc" );
        return SYS_ERROR_EXIT_CODE;
    }

    if ( tat_ListFile )
    {
        listf = FP_OpenFile( tat_ListFile, "w" );
        if ( !listf )
        {
            return SYS_ERROR_EXIT_CODE;
        }
    }

    for ( i = 0; i < tat_MscCount; i++ )
    {
        if ( !MSC_Load( tat_MscNames[i], &msc_X, 1 ) )
        {
            MSC_FreeMemory( &msc_X );
            if ( listf )
                fclose( listf );
            return SYS_ERROR_EXIT_CODE;
        };

        FP_SplitPath( tat_MscNames[i], output_name, name, path );

        if ( prefix )
            strcat( output_name, prefix );

        strcat( output_name, name );
        strcat( output_name, path );

        MSC_Transform( &msc_X, MSC_TRANS_USE_COND | MSC_TRANS_REMOVE_ZERO_LOOP );

        if ( flag_Mark )
        {
            event_Count = 1;
            MSC_Process( &msc_X, tat_MarkComment );
        }

        if ( !flag_Verify )
        {
            if ( flag_OutFlowgen )
            {
                FP_SplitPath( output_name, path, name, NULL );
                strcpy( output_name, path );
                strcat( output_name, name );
                strcat( output_name, STR_DOTFG );
                msc_X.source_Format = fg_format;
            }

            if ( flag_OutMsc )
            {
                FP_SplitPath( output_name, path, name, NULL );
                strcpy( output_name, path );
                strcat( output_name, name );
                strcat( output_name, STR_DOTMPR );
                msc_X.source_Format = mpr_format;
            }
            if ( !flag_VerdTime )
            {
                if ( !MSC_Save( output_name, &msc_X, flags ) )
                {
                    MSC_FreeMemory( &msc_X );
                    if ( listf )
                        fclose( listf );
                    return SYS_ERROR_EXIT_CODE;
                }

                if ( listf )
                {
                    fprintf( listf, "%s\n", output_name );
                }
            }
            else
            {
                for ( j = 0; j < msc_X.trace_Count; j++ )
                {
                    if ( msc_X.traces[j].status == MSC_VRS_CONSISTENT )
                        continue;

                    FP_SplitPath( tat_MscNames[i], output_name, name, path );

                    if ( prefix )
                        strcat( output_name, prefix );

                    strcat( output_name, name );
                    sprintf( count, "N%u", msc_X.traces[j].trace );
                    strcat( output_name, count );
                    strcat( output_name, path );
                    MSC_VerdictTrace = msc_X.traces[j].trace;

                    if ( !MSC_Save( output_name, &msc_X, flags ) )
                    {
                        MSC_FreeMemory( &msc_X );
                        if ( listf )
                            fclose( listf );
                        return SYS_ERROR_EXIT_CODE;
                    }

                    if ( listf )
                    {
                        fprintf( listf, "%s\n", output_name );
                    }
                }
            }
        }

        MSC_FreeMemory( &msc_X );
    }

    return SYS_NORMAL_EXIT_CODE;
}
