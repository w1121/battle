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
* Functions:    MSC_MacroBuild
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  Compiles macro declaration in internal data structures
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
#include <tatgdata.h>
#include "macroprc.h"

static DEF_MSG(tat_FUNCTION_NOT_FOUND, "ERROR: xml: function '%s' for macro '%s' is not found\n");

int MSC_CompStaticData( const void *x1, const void *x2 )
{
    return strcmp( ( ( MSC_STATIC_MACRO_INDEX * ) x1 )->name, ( ( MSC_STATIC_MACRO_INDEX * ) x2 )->name );
}

static int tat_CompDynData( const void *x1, const void *x2 )
{
    return strcmp( ( ( MSC_DYNAMIC_MACRO_INDEX * ) x1 )->name, ( ( MSC_DYNAMIC_MACRO_INDEX * ) x2 )->name );
}

/***************************************************************************
* Function : MSC_MacroBuild
*
* Description: Fills MSC_StaticMacroTable, MSC_DynMacroTable,
*              updates path specification and pathlen loads
*              loads functions from shared libraries
*
* Returns: RESULT_OK/RESULT_ERR
*
* Notes: None
*
**************************************************************************/
LOGICAL MSC_MacroBuild(TAT_XML_CONFIG_DEF * configdata, UINT32_T *path, UINT32_T *pathlen )
{
    UINT32_T i, j, p, k;

    MSC_StaticMacroTableSize = 0;
    MSC_StaticMacroTable = NULL;
    MSC_DynMacroTableSize = 0;
    MSC_DynMacroTable = NULL;

    for (i=0; i<configdata->macro_Count; i++)
    {
        p = path[i];
        switch (configdata->macro[p].type)
        {
        case MSC_MACROTYPE_FUNCTION:
        case MSC_MACROTYPE_INDEX:
            {
                MSC_DynMacroTable = MEM_AddObject(MSC_DynMacroTable, MSC_DynMacroTableSize+1, sizeof(MSC_DynMacroTable[0]));
                MSC_DynMacroTable[MSC_DynMacroTableSize].name = MEM_CopyStr(&configdata->macro[p].name[1]);
                MSC_DynMacroTable[MSC_DynMacroTableSize].index = p;
                MSC_DynMacroTable[MSC_DynMacroTableSize].fpc = NULL;
                MSC_DynMacroTable[MSC_DynMacroTableSize].datatype = configdata->macro[p].data_Type;
                if (configdata->macro[p].type == MSC_MACROTYPE_FUNCTION)
                {
                    /* configdata->macro[i].attr[0] */
                    MSC_DynMacroTable[MSC_DynMacroTableSize].param_number = atoi(configdata->macro[p].attr[1]);
                    for (j=0; j<MSC_LibSetSize; j++)
                    {
                        MSC_DynMacroTable[MSC_DynMacroTableSize].fpc =
                            (MSC_MACRO_FUNCTION) Dyn_GetProcAddr(MSC_LibSet[j], configdata->macro[p].attr[0]);
                        if (MSC_DynMacroTable[MSC_DynMacroTableSize].fpc)
                            break;
                    }

                    if (!MSC_DynMacroTable[MSC_DynMacroTableSize].fpc)
                    {
                        MSG_PrintError(tat_FUNCTION_NOT_FOUND,configdata->macro[p].attr[0],configdata->macro[p].name );
                        return RESULT_ERR;
                    }
                }
                else
                {
                    /* index always allows single parameter */
                    MSC_DynMacroTable[MSC_DynMacroTableSize].param_number = 1;

                }
                MSC_DynMacroTableSize++;
                break;
            }

        case MSC_MACROTYPE_DERIVE:
        case MSC_MACROTYPE_LIST:
            {
                MSC_StaticMacroTable = MEM_AddObject(MSC_StaticMacroTable, MSC_StaticMacroTableSize+1, sizeof(MSC_StaticMacroTable[0]));
                MSC_StaticMacroTable[MSC_StaticMacroTableSize].name = MEM_CopyStr(&configdata->macro[p].name[1]);
                MSC_StaticMacroTable[MSC_StaticMacroTableSize].index = p;
                MSC_StaticMacroTable[MSC_StaticMacroTableSize].currpos = NULL;
                MSC_StaticMacroTable[MSC_StaticMacroTableSize].overflow = NULL;
                MSC_StaticMacroTable[MSC_StaticMacroTableSize].active = 0;
                MSC_StaticMacroTable[MSC_StaticMacroTableSize].activeset = 0;
                MSC_StaticMacroTable[MSC_StaticMacroTableSize].dataset = NULL;
                MSC_StaticMacroTable[MSC_StaticMacroTableSize].fpc = NULL;
                MSC_StaticMacroTable[MSC_StaticMacroTableSize].repl = MEM_Allocate(TAT_MAX_LINE_LEN);
                /*  make the result equal to original name */
                strcpy (MSC_StaticMacroTable[MSC_StaticMacroTableSize].repl, MSC_StaticMacroTable[MSC_StaticMacroTableSize].name);
                MSC_StaticMacroTable[MSC_StaticMacroTableSize].datatype = configdata->macro[p].data_Type;


                if (configdata->macro[p].type == MSC_MACROTYPE_DERIVE)
                {
                    for (j=0; j<MSC_LibSetSize; j++)
                    {
                        MSC_StaticMacroTable[MSC_StaticMacroTableSize].fpc =
                            (MSC_MACRO_FUNCTION)Dyn_GetProcAddr(MSC_LibSet[j], configdata->macro[p].attr[0]);

                        if (MSC_StaticMacroTable[MSC_StaticMacroTableSize].fpc)
                            break;
                    }
                    if (!MSC_StaticMacroTable[MSC_StaticMacroTableSize].fpc)
                    {
                        MSG_PrintError(tat_FUNCTION_NOT_FOUND,configdata->macro[p].attr[0],configdata->macro[p].name );
                        return RESULT_ERR;
                    }
                }
                else
                {
                    /* initialize counter limits for each data set */
                    MSC_StaticMacroTable[MSC_StaticMacroTableSize].datasetcount = configdata->macro[p].data_Count;

                    MSC_StaticMacroTable[MSC_StaticMacroTableSize].dataset = MEM_Allocate(configdata->macro[p].data_Count * 
                        sizeof(MSC_StaticMacroTable[MSC_StaticMacroTableSize].dataset[0]));

                    MSC_StaticMacroTable[MSC_StaticMacroTableSize].currpos = MEM_Allocate(configdata->macro[p].data_Count * 
                        sizeof(MSC_StaticMacroTable[MSC_StaticMacroTableSize].currpos[0]));

                    MSC_StaticMacroTable[MSC_StaticMacroTableSize].overflow = MEM_Allocate(configdata->macro[p].data_Count * 
                        sizeof(MSC_StaticMacroTable[MSC_StaticMacroTableSize].overflow[0]));

                    for (j=0; j<configdata->macro[p].data_Count; j++)
                    {
                        MSC_StaticMacroTable[MSC_StaticMacroTableSize].dataset[j]=configdata->macro[p].data[j].data_Count;
                    }
                }

                MSC_StaticMacroTableSize++;
                break;
            }

        case MSC_MACROTYPE_ABORT:
            {
                break;
            }
        }
    }

    if (MSC_StaticMacroTable)
    {
        qsort(MSC_StaticMacroTable, MSC_StaticMacroTableSize, sizeof(MSC_StaticMacroTable[0]), MSC_CompStaticData);
    }

    if (MSC_DynMacroTable)
    {
        qsort(MSC_DynMacroTable, MSC_DynMacroTableSize, sizeof(MSC_DynMacroTable[0]), tat_CompDynData);
    }

    /* updates path references */
    for (j=0, i=0; i<configdata->macro_Count; i++)
    {
        p = path[i];
        switch (configdata->macro[p].type)
        {
        case MSC_MACROTYPE_FUNCTION:
        case MSC_MACROTYPE_INDEX:
            {
                path[i] = -1;
                break;
            }

        case MSC_MACROTYPE_DERIVE:
        case MSC_MACROTYPE_LIST:
            {
                /* update number */
                k = MSC_GetSubstNumber( &configdata->macro[p].name[1] );
                path[j++]=k;
                if (configdata->macro[p].vector_Name)
                {
                    MSC_StaticMacroTable[k].vectornum = MSC_GetSubstNumber( &configdata->macro[p].vector_Name[1] );
                }
                else
                {
                    MSC_StaticMacroTable[k].vectornum = -1;
                }
                break;
            }

        case MSC_MACROTYPE_ABORT:
            {
                path[j++]= path[i] | 0x80000000;
                break;
            }
        }
    }
    *pathlen = j;
    return RESULT_OK;
}

/***************************************************************************
* Function : MSC_ExpandIntervals
*
* Description: Expand number intervals
*
* Returns: RESULT_ERR/RESULT_OK
*
* Notes: None
*
**************************************************************************/
LOGICAL MSC_ExpandIntervals( TAT_XML_CONFIG_DEF * configdata )
{
    UINT32_T                    i, j, k, inc, hex;
    double                  start, end, step;
    char                    temp[TAT_MAX_ID_LEN], *p;

    for ( i = 0; i < configdata->macro_Count; i++ )
    {
        if ( configdata->macro[i].type == MSC_MACROTYPE_LIST &&
            configdata->macro[i].data_Type == MSC_MACRODATA_NUMBER )
        {
            for ( j = 0; j < configdata->macro[i].data_Count; j++ )
            {
                k = 0;

                while ( k < configdata->macro[i].data[j].data_Count )
                {
                    if ( !StrIsAnyNumber( configdata->macro[i].data[j].data[k] ) )
                    {
                        p = configdata->macro[i].data[j].data[k];
                        if (!StrExtractDigit( &p, temp ) || (temp[0]==0))
                            return RESULT_ERR;
                        
                        hex = ( temp[0] == '0' && ( temp[1] == 'x' || temp[1] == 'X' ) );
                        start = StrToDouble( temp );
                        StrSkipComments( &p );

                        if ( *p != '{' )
                            return RESULT_ERR;

                        p++;
                        if (!StrExtractDigit( &p, temp ) || temp[0]==0)
                            return RESULT_ERR;

                        step = StrToDouble( temp );
                        StrSkipComments( &p );

                        if ( *p != '}' )
                            return RESULT_ERR;

                        p++;
                        if (!StrExtractDigit( &p, temp ) || temp[0]==0)
                            return RESULT_ERR;
                        end = StrToDouble( temp );

                        if ( step == 0 )
                        {
                            MSG_PrintError( "ERROR: mmsc: zero step for macro '%s' is illegal\n",
                                configdata->macro[i].name );
                            return RESULT_ERR;
                        }

                        if ( step < 0 )
                            step = -step;

                        if ( end < start )
                        {
                            double                  tt;

                            tt = end;
                            end = start;
                            start = tt;
                        }

                        MEM_Free( configdata->macro[i].data[j].data[k] );
                        hex = hex && ( floor( step ) == step ) && ( floor( end ) == end );
                        inc = ( UINT32_T ) floor( ( end - start ) / step );

                        if ( ( start + ( step * inc ) ) != end )
                            inc++;

                        configdata->macro[i].data[j].data =
                            MEM_Reallocate( configdata->macro[i].data[j].data,
                            ( inc + configdata->macro[i].data[j].data_Count ) *
                            sizeof ( configdata->macro[0].data[0].data[0] ) );

                        memmove( &configdata->macro[i].data[j].data[k + inc + 1],
                            &configdata->macro[i].data[j].data[k + 1],
                            ( configdata->macro[i].data[j].data_Count - k -
                            1 ) * sizeof ( configdata->macro[0].data[0].data[0] ) );

                        configdata->macro[i].data[j].data_Count += inc;

                        do
                        {
                            if ( !hex )
                                sprintf( temp, "%.10g", start );
                            else
                                sprintf( temp, "0x%x", ( UINT32_T ) start );

                            configdata->macro[i].data[j].data[k++] = MEM_CopyStr( temp );

                            start += step;
                        }
                        while ( start <= end );

                        /* if we finished sequence without end value then add end value */
                        if ( ( start - step ) != end )
                        {
                            if ( !hex )
                                sprintf( temp, "%.10g", end );
                            else
                                sprintf( temp, "0x%x", ( UINT32_T ) end );

                            configdata->macro[i].data[j].data[k++] = MEM_CopyStr( temp );
                        };

                    }           /* !StrAnyNumber */
                    else
                        k++;
                }
            }
        }
    }

    return RESULT_OK;
}
