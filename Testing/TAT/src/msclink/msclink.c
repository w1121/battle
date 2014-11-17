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
* Description:  Link several MSC's
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
#include <tatgdata.h>
#include <mscstrs.h>
#include <mscmess.h>
#include <flowgen.h>

static                  DEF_MSG( tat_ProgramName, "MSC Linker" );

static size_t             tat_MscCount = 0;
static int              flag_Sync = 0;
static int              flag_NoExt = 0;
static int              flag_Par = 0;
static int              flag_Alt = 0;

static char           **tat_MscNames = NULL;

static char            *tat_Output = NULL;

static MSC_STORAGE  msc_X, msc_Y;

CMD_LINE                cmd[] = {

    {"-m", CMD_FILE_LIST, &tat_MscCount, &tat_MscNames, "set input file names"},
    {"-o", CMD_FILE_NAME, NULL, &tat_Output, "set output file name"},
    {"-sync", CMD_BOOLEAN, NULL, &flag_Sync, "separate each MSC with global condition"},
    {"-nommsc", CMD_BOOLEAN, NULL, &flag_NoExt, "do not output macro MSC extensions"},
    {"-par", CMD_BOOLEAN, NULL, &flag_Par, "parallel linking"},
    {"-alt", CMD_BOOLEAN, NULL, &flag_Alt, "alternative linking"}

};

#define cmd_params sizeof(cmd)/sizeof(cmd[0])

/***************************************************************************
 * Function : tat_DeleteExtraSpec
 *
 * Description: remove specification which shall not be output
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_DeleteExtraSpec( MSC_STORAGE * M, MSC_EVENT_KIND * s )
{
    s->condition = 0;
    s->sync = MSC_ASYNC_EVENT;

    if ( s->time.type == MSC_LABELED_TIME )
    {
        M->labels[s->time.time_Label].s = NULL;
        M->labels[s->time.time_Label].unused = 1;
        M->labels[s->time.time_Label].label = 0;
    }
    s->time.str_MinTime = STRCOLL_STR_ID_0;
    s->time.str_MaxTime = STRCOLL_STR_ID_INF;
    s->time.type = MSC_RELATIVE_TIME;
    s->time.max_Time = MSC_INFINITY_CONST;

    s->time_Calc.str_MinTime = STRCOLL_STR_ID_0;
    s->time_Calc.str_MaxTime = STRCOLL_STR_ID_INF;
    s->time_Calc.type = MSC_RELATIVE_TIME;
    s->time_Calc.max_Time = MSC_INFINITY_CONST;
    if ( s->labeled )
    {
        M->labels[s->labeled - 1].s = NULL;
        M->labels[s->labeled - 1].label = 0;
        M->labels[s->labeled - 1].unused = 1;
        s->labeled = 0;
    }
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
    size_t                    i, mpr_format;
    LOGICAL                 res;
    MSC_EVENT_KIND              *s;

    MSG_Copyright( tat_ProgramName, argv[0] );
    mpr_format = MSC_RegisterFormat(STR_DOTMPR,  MSC_Load,  MSC_Save);
    MSC_RegisterFormat(STR_DOTFG,  FG_Load,  FG_Save);

    if ( argc == 1 || !CMD_Parse( cmd_params, ( CMD_LINE * ) & cmd, argc, argv ) )
    {
        CMD_PrintHelp( cmd_params, ( CMD_LINE * ) & cmd );
        return argc != 1;
    }

    if ( !tat_MscCount || !tat_Output )
    {
        MSG_PrintError( MSG_INVALID_SWITCH_VALUE_s, ( !tat_MscCount ) ? "-m" : "-o" );
        return SYS_ERROR_EXIT_CODE;
    }

    if ( flag_Par && flag_Alt )
    {
        MSG_PrintError( MSG_INVALID_SWITCH_USAGE_ss, "-alt", "-par" );
        return SYS_ERROR_EXIT_CODE;
    }

    if ( ( flag_Par || flag_Alt ) && flag_Sync )
    {
        MSG_PrintError( MSG_INVALID_SWITCH_USAGE_ss, ( flag_Par ) ? "-par" : "-alt", "-sync" );
        return SYS_ERROR_EXIT_CODE;
    }

    if ( tat_MscCount == 1 )
    {
        if ( flag_Alt )
        {
            flag_Alt = 0;
        };

        if ( flag_Par )
        {
            flag_Par = 0;
        };
    };

    MSC_Allocate( tat_Output, &msc_X );
    msc_X.source_Format = mpr_format;
/* remove even absolute label */
    msc_X.label_Count = 0;
    MEM_Free( msc_X.labels );
    msc_X.labels = NULL;

    if ( flag_Par || flag_Alt )
    {
        msc_X.start = MSC_AllocateEvent( &msc_X );
        msc_X.start->type = MSC_EV_OPERATOR;
        msc_X.start->data.alt.type = ( flag_Par ) ? MSC_OP_PAR : MSC_OP_ALT;
        msc_X.start->data.alt.frame_Count = tat_MscCount;
        msc_X.start->data.alt.frame = MEM_Allocate( tat_MscCount * sizeof ( MSC_EVENT_KIND * ) );

        for ( i = 0; i < tat_MscCount; i++ )
        {
            if ( !MSC_Load( tat_MscNames[i], &msc_Y, 1 ) )
            {
                MSC_FreeMemory( &msc_X );
                return SYS_ERROR_EXIT_CODE;
            };

            msc_X.start->data.alt.frame[i] = MSC_AllocateEvent( &msc_X );
            msc_X.start->data.alt.frame[i]->type = MSC_EV_ACTION;
            if ( flag_NoExt )
            {
                MSC_Process( &msc_Y, tat_DeleteExtraSpec );
            }
            MSC_Include( &msc_X, &msc_Y, msc_X.start->data.alt.frame[i], 0 );
            MSC_FreeMemory( &msc_Y );
        }
    }
    else
    {

        for ( i = 0; i < tat_MscCount; i++ )
        {
            if ( !MSC_Load( tat_MscNames[i], &msc_Y, 1 ) )
            {
                MSC_FreeMemory( &msc_X );
                return SYS_ERROR_EXIT_CODE;
            };

            s = msc_X.start;

            while ( s && s->next )
                s = s->next;

            if ( !s || s->type != MSC_EV_EMPTY )
            {
                if ( !s )
                {
                    s = MSC_AllocateEvent( &msc_X );
                    msc_X.start = s;
                }
                else
                {
                /* add new event */
                    if ( !flag_Sync || s->type != MSC_EV_CONDITION || !s->data.cond.generated )
                    {
                        s->next = MSC_AllocateEvent( &msc_X );
                        s = s->next;
                    }
                }

                s->type = MSC_EV_ACTION;
            }

            if ( flag_NoExt )
            {
                MSC_Process( &msc_Y, tat_DeleteExtraSpec );
            }

            MSC_Include( &msc_X, &msc_Y, s, flag_Sync );
            MSC_FreeMemory( &msc_Y );
        }
    }

    if ( !flag_NoExt )
        res = MSC_Save( tat_Output, &msc_X, MSC_MPR_TIMED_OUT );
    else
        res = MSC_Save( tat_Output, &msc_X, MSC_MPR_STD_OUT );

    MSC_FreeMemory( &msc_X );

    if ( !res )
        return SYS_ERROR_EXIT_CODE;

    return SYS_NORMAL_EXIT_CODE;
}
