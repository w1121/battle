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
* Description:  Macro MSC preprocessor
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
#include <outmess.h>
#include <cmdline.h>
#include <xmlconf.h>
#include <confproc.h>
#include <mscstrs.h>
#include <mscmess.h>
#include "macroprc.h"

static                  DEF_MSG( MSG_MMSCPP_UNKNOWN_MACRO,
                                "ERROR: xml: unknown macro reference '%s' in macro '%s'\n" );
DEF_MSG( MSG_MMSCPP_INVALID_EXPR, "ERROR: xml: invalid expression '%s'\n" );
static                  DEF_MSG( MSG_MMSCPP_RECURSIVE_DEF,
                                "ERROR: xml: recursive definition between macros '%s' and '%s'\n" );
static                  DEF_MSG( MSG_MMSCPP_INVALID_MACRO,
                                "ERROR: xml: invalid macro type for '%s' in expression '%s'\n" );

/***************************************************************************
* Function : TATMACRO_BuildDepend
*
* Description: Build dependencies matrix, generate order of calculation
*
* Returns: RESULT_ERR/RESULT_OK
*
* Notes: None
*
**************************************************************************/
LOGICAL TATMACRO_BuildDepend( TAT_XML_CONFIG_DEF * configdata, UINT32_T ** path )
{
    LOGICAL                 ExitCode = RESULT_OK;
    UINT32_T                    i, j, k, l, cc;
    int                     ii;
    char                  **names;

    Byte                   *arr = NULL, *s;
    UINT32_T                   *p, pcount;

    Byte                   *rowi;
    int                    *incident, level;
    UINT32_T                    new_set;

    cc = configdata->macro_Count;

    arr = MEM_Allocate( cc * cc * sizeof ( arr[0] ) );
    p = MEM_Allocate( cc * sizeof ( p[0] ) );
    s = MEM_Allocate( cc * sizeof ( s[0] ) );

    for ( i = 0; i < cc; i++ )
    {
        switch ( configdata->macro[i].type )
        {
        case MSC_MACROTYPE_INDEX:
            {
                for ( j = 0; j < configdata->macro[i].data_Count; j++ )
                {
                    if ( configdata->macro[i].data[j].condition )
                    {
                        if ( !TATMACRO_Extract( configdata->macro[i].data[j].condition, &k, &names ) )
                        {
                            MSG_PrintError( MSG_MMSCPP_INVALID_EXPR, configdata->macro[i].data[j].condition );
                            ExitCode = RESULT_ERR;
                            goto Exit;
                        }

                        for ( l = 0; l < k; l++ )
                        {
                            ii = CFG_GetMacroID( names[l] );

                            if ( ii == -1 )
                            {
                                MSG_PrintError( MSG_MMSCPP_UNKNOWN_MACRO, names[l], configdata->macro[i].name );
                                ExitCode = RESULT_ERR;
                                goto Exit;
                            };

                            if ( configdata->macro[ii].type != MSC_MACROTYPE_LIST &&
                                configdata->macro[ii].type != MSC_MACROTYPE_DERIVE )
                            {
                                /* can't use FUNCTION, ABORT in conditions */
                                MSG_PrintError( MSG_MMSCPP_INVALID_MACRO, configdata->macro[ii].name,
                                    configdata->macro[i].data[j].condition );
                                ExitCode = RESULT_ERR;
                                goto Exit;
                            }
                        }

                        MEM_FreeStrArray( k, names );
                    }
                }
                break;
            }

        case MSC_MACROTYPE_LIST:
            {
                if ( configdata->macro[i].vector_Name )
                {
                    ii = CFG_GetMacroID( configdata->macro[i].vector_Name );

                    if ( ii == -1 )
                    {
                        MSG_PrintError( MSG_MMSCPP_UNKNOWN_MACRO, configdata->macro[i].vector_Name,
                            configdata->macro[i].name );
                        ExitCode = RESULT_ERR;
                        goto Exit;
                    };

                    if ( configdata->macro[ii].type != MSC_MACROTYPE_LIST &&
                        configdata->macro[ii].type != MSC_MACROTYPE_DERIVE )
                    {
                        /* can't use FUNCTION, ABORT in conditions */
                        MSG_PrintError( MSG_MMSCPP_INVALID_MACRO, configdata->macro[ii].name,
                            configdata->macro[i].vector_Name );
                        ExitCode = RESULT_ERR;
                        goto Exit;
                    }
                    arr[ii * cc + i] = 1;
                }

                for ( j = 0; j < configdata->macro[i].data_Count; j++ )
                {
                    if ( configdata->macro[i].data[j].condition )
                    {
                        if ( !TATMACRO_Extract( configdata->macro[i].data[j].condition, &k, &names ) )
                        {
                            MSG_PrintError( MSG_MMSCPP_INVALID_EXPR, configdata->macro[i].data[j].condition );
                            ExitCode = RESULT_ERR;
                            goto Exit;
                        }

                        for ( l = 0; l < k; l++ )
                        {
                            ii = CFG_GetMacroID( names[l] );

                            if ( ii == -1 )
                            {
                                MSG_PrintError( MSG_MMSCPP_UNKNOWN_MACRO, names[l], configdata->macro[i].name );
                                ExitCode = RESULT_ERR;
                                goto Exit;
                            };

                            if ( configdata->macro[ii].type != MSC_MACROTYPE_LIST &&
                                configdata->macro[ii].type != MSC_MACROTYPE_DERIVE )
                            {
                                /* can't use FUNCTION, ABORT in conditions */
                                MSG_PrintError( MSG_MMSCPP_INVALID_MACRO, configdata->macro[ii].name,
                                    configdata->macro[i].data[j].condition );
                                ExitCode = RESULT_ERR;
                                goto Exit;
                            }

                            arr[ii * cc + i] = 1;
                        }

                        MEM_FreeStrArray( k, names );
                    }
                }

                break;
            }

        case MSC_MACROTYPE_DERIVE:
            {
                for ( j = 0; j < configdata->macro[i].data[0].data_Count; j++ )
                {
                    if ( configdata->macro[i].data[0].data[j][0] == '#' )
                    {
                        ii = CFG_GetMacroID( configdata->macro[i].data[0].data[j] );

                        if ( ii == -1 )
                        {
                            MSG_PrintError( MSG_MMSCPP_UNKNOWN_MACRO,
                                configdata->macro[i].data[0].data[j], configdata->macro[i].name );
                            ExitCode = RESULT_ERR;
                            goto Exit;
                        };

                        if ( configdata->macro[ii].type != MSC_MACROTYPE_LIST &&
                            configdata->macro[ii].type != MSC_MACROTYPE_DERIVE )
                        {
                            /* can't use FUNCTION, ABORT in conditions */
                            MSG_PrintError( MSG_MMSCPP_INVALID_MACRO, configdata->macro[ii].name,
                                configdata->macro[i].data[0].data[j] );
                            ExitCode = RESULT_ERR;
                            goto Exit;
                        }

                        arr[ii * cc + i] = 1;

                    }
                }

                break;
            }

        case MSC_MACROTYPE_FUNCTION:
            {
                break;
            }

        case MSC_MACROTYPE_ABORT:
            {
                for ( j = 0; j < configdata->macro[i].data_Count; j++ )
                {
                    if ( configdata->macro[i].data[j].condition )
                    {
                        if ( !TATMACRO_Extract( configdata->macro[i].data[j].condition, &k, &names ) )
                        {
                            MSG_PrintError( MSG_MMSCPP_INVALID_EXPR, configdata->macro[i].data[j].condition );
                            ExitCode = RESULT_ERR;
                            goto Exit;
                        }

                        for ( l = 0; l < k; l++ )
                        {
                            ii = CFG_GetMacroID( names[l] );

                            if ( ii == -1 )
                            {
                                ExitCode = RESULT_ERR;
                                goto Exit;
                            };

                            if ( configdata->macro[ii].type != MSC_MACROTYPE_LIST &&
                                configdata->macro[ii].type != MSC_MACROTYPE_DERIVE )
                            {
                                /* can't use FUNCTION, ABORT in conditions */
                                MSG_PrintError( MSG_MMSCPP_INVALID_MACRO, configdata->macro[ii].name,
                                    configdata->macro[i].data[j].condition );
                                ExitCode = RESULT_ERR;
                                goto Exit;
                            }

                            arr[ii * cc + i] = 1;
                        }

                        MEM_FreeStrArray( k, names );
                    }
                }

                break;
            }
        }
    }

    /* check transitive closing for inconsistecy detection */
    for ( i = 0; i < cc; i++ )
    {
        j = cc;
        while ( j )
        {
            j--;

            if ( arr[i * cc + j] && arr[j * cc + i] )
            {
                /* recursive macro definition */
                MSG_PrintError( MSG_MMSCPP_RECURSIVE_DEF, configdata->macro[i].name,
                    configdata->macro[j].name );
                ExitCode = RESULT_ERR;
                goto Exit;
            }
        }
    }


    incident = MEM_Allocate( ( cc * sizeof ( incident[0] ) ) << 1 );
    pcount = 0;
/* calculate initial incidents array */
    for ( i = 0; i < cc; i++ )
    {
        rowi = arr + i;
        level = 0;
        j = cc;
        while ( j )
        {
            level += *rowi;
            rowi += cc;
            j--;
        }
        incident[i] = level;
    }
/* initialize topological sort by levels */
    level = 1;
    k = cc;
    while ( k )
    {
        new_set = 0;
    /* find input vertexes of next level */
        for ( i = 0; i < cc; i++ )
        {
            if ( incident[i] == 0 )
            {
                incident[i] = -1;
                p[pcount++] = i;
                incident[new_set + cc] = i;
                new_set++;
                k--;
            }
        }

        if ( !k )
            break;

        /* exclude set of input vertexes */
        for ( i = cc; i < cc + new_set; i++ )
        {
            l = incident[i] * cc;
            for ( j = 0; j < cc; j++ )
            {
                incident[j] -= ( UINT32_T ) arr[l + j];
            }
        }
        level++;
    }

    MEM_Free( incident );

    &((MSC_STATIC_MACRO_INDEX *)(0))->fpc;

    *path = p;
    /*    for (i=0; i<cc; i++)
    puts(configdata->macro[p[i]].name); */

Exit:
    if ( arr ) MEM_Free( arr );
    if ( !ExitCode && p ) MEM_Free( p );
    if ( s ) MEM_Free( s );
    return ExitCode;
}

