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
* Description:  MSC Metric calculator
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

static                  DEF_MSG( tat_ProgramName, "MSC LocMeter" );

UINT32_T                    MSC_Count = 0;
char                  **MSCs = NULL;

CMD_LINE                cmd[] = {
    {"-m", CMD_FILE_LIST, &MSC_Count, &MSCs, "set input file names"}
};

#define cmd_params sizeof(cmd)/sizeof(cmd[0])

static size_t             locs = 0;
static size_t             frames = 0;
static size_t             instances = 0;

/***************************************************************************
 * Function : calc
 *
 * Description: calculate LOCs for event
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL calc( MSC_STORAGE * msc, MSC_EVENT_KIND * x )
{
    if ( x->type == MSC_EV_OPERATOR )
    {
        frames += x->data.alt.frame_Count;
    }
    else if ( x->type == MSC_EV_OUT_MESSAGE )
    {
        locs++;
    }
    else if ( x->type == MSC_EV_IN_MESSAGE )
    {
        if ( x->data.msg.inst == MSC_FOUND_INSTANCE_ID )
            locs++;
    }
    else
        locs++;

    return RESULT_OK;
}

int main( int argc, char *argv[] )
{
    MSC_STORAGE         M;

    UINT32_T                     i;

    MSG_Copyright( tat_ProgramName, argv[0] );
    MSC_RegisterFormat(STR_DOTMPR,  MSC_Load,  MSC_Save);
    MSC_RegisterFormat(STR_DOTFG,  FG_Load,  FG_Save);

    if ( ( argc == 1 ) || !CMD_Parse( cmd_params, ( CMD_LINE * ) & cmd, argc, argv ) )
    {
        CMD_PrintHelp( cmd_params, ( CMD_LINE * ) & cmd );
        return argc != 1;
    };

    if ( !MSC_Count )
    {
        MSG_PrintError( MSG_INVALID_SWITCH_VALUE_s, "-m" );
        return SYS_ERROR_EXIT_CODE;
    }

    for ( i = 0; i < MSC_Count; i++ )
    {
        if ( !MSC_Load( MSCs[i], &M, 0 ) )
        {
            return SYS_ERROR_EXIT_CODE;
        }

        MSC_Process( &M, calc );
        MSC_FreeMemory( &M );
    };
    printf( "%u MSCs\n", MSC_Count );
    printf( "%u locs\n", locs + instances );
    printf( "%u frames\n", frames );

    return SYS_NORMAL_EXIT_CODE;
}
