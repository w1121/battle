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
* Functions:    
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  
*
* Notes:        None
*
******************************************************************************/
#include <portable.h>
#include <mscbase.h>
#include <mscparse.h>
#include <mscproc.h>
#include <qmemory.h>
#include <fileproc.h>
#include <strproc.h>
#include <outmess.h>
#include <mscmess.h>
#include <cmdline.h>
#include <mscgen.h>
#include <mscstrs.h>
#include <tatgdata.h>
#include <flowgen.h>
#include <xmlconf.h>
#include <cfgtypes.h>
#include <confproc.h>
#include "macroprc.h"


/***************************************************************************
 * Function : MSC_MacroGenSet
 *
 * Description: calculate next permutation of macro values
 *
 * Returns: 0 -error, 1- ok, can continue, 2-ok, last set
 *
 * Notes: None
 *
 **************************************************************************/
int MSC_MacroGenSet(TAT_XML_CONFIG_DEF * configdata, UINT32_T *path, UINT32_T pathlen, UINT32_T counter, UINT32_T * next_start_i )
{
    UINT32_T i, j, k, p, ii;
    UINT32_T carry, last_set, try_count;
    UINT32_T dataset;
    int res;

    /* set-up zero counter */
    if (counter == 0)
    {
        carry = 0;
    }
    else
    {
        carry = 1;
    }

    try_count = 0;
    last_set = 0;
L1:
    
    try_count++;
    
    for (i=*next_start_i; i<pathlen; i++)
    {
        /* skip INDEX and FUNCTION macro */
        if (path[i]==-1) continue;

        p = path[i] & 0x7fffffff;
        if (path[i] & 0x80000000)
        {
            /* process ABORT macro */
            for (j=0; j<configdata->macro[p].data_Count; j++)
            {
                res = MSC_MacroCalc(configdata->macro[p].data[j].condition);
                if (res == -1)
                {
                    MSG_PrintError(MSG_MMSC_CANT_COMPUTE_ss, MSC_ConfigXML, configdata->macro[p].data[j].condition );
                    return RESULT_ERR;
                }

                if (res) goto L_abort;
            }
        }
        else
        {
            /* set the default value */
            MSC_StaticMacroTable[p].repl[0]=MSC_MACRO_START;
            strcpy(MSC_StaticMacroTable[p].repl+1, MSC_StaticMacroTable[p].name);

            if (!MSC_StaticMacroTable[p].active) continue;

            p = MSC_StaticMacroTable[p].index;
            
            switch (configdata->macro[p].type)
            {
                case MSC_MACROTYPE_DERIVE:
                    {
                        char * * buffer;
                        buffer = MEM_Allocate(configdata->macro[p].data[0].data_Count * sizeof(buffer[0]));
                        for (j=0; j<configdata->macro[p].data[0].data_Count; j++)
                        {
                            if (configdata->macro[p].data[0].data[j][0]==MSC_MACRO_START)
                            {
                                /* get the Macro Reference in buffer */
                                k = MSC_GetSubstNumber(&(configdata->macro[p].data[0].data[j][1]));
                                buffer[j] = MEM_CopyStr(MSC_StaticMacroTable[k].repl);
                            }
                            else
                            {
                                buffer[j] = MEM_CopyStr(configdata->macro[p].data[0].data[j]);
                            }
                        }

                        /* invoke user's function */
                        if (!MSC_StaticMacroTable[path[i]].fpc(configdata->macro[p].data[0].data_Count,
                            buffer, MSC_StaticMacroTable[path[i]].repl))
                        {
                            MSG_PrintError(MSG_MMSC_RETURNS_ERROR_sss, MSC_ConfigXML,
                                configdata->macro[p].attr[0], configdata->macro[p].name);
                            return RESULT_ERR;
                        }

                        if ( !CFG_CheckMacroData( configdata->macro[p].data_Type, MSC_StaticMacroTable[path[i]].repl ) )
                        {
                            MSG_PrintError(MSG_MMSC_INVALID_DATATYPE_ssss, MSC_ConfigXML,
                                configdata->macro[p].attr[0], configdata->macro[p].name, MSC_StaticMacroTable[path[i]].repl);
                            return RESULT_ERR;
                        }

                        MEM_FreeStrArray(configdata->macro[p].data[0].data_Count, buffer);
                        break;
                    }
                case MSC_MACROTYPE_LIST:
                    {
                        /* determine active dataset list */
                        for (dataset=0; dataset<configdata->macro[p].data_Count; dataset++)
                        {
                            if (!configdata->macro[p].data[dataset].condition)
                            {
                                break;
                            }
                            res = MSC_MacroCalc(configdata->macro[p].data[dataset].condition);
                            if (res == -1)
                                return RESULT_ERR;
                            if (res == 1)
                                break;
                        }

                        /* if no dataset is active */
                        if (dataset >= configdata->macro[p].data_Count)
                        {
                            /* check if not possible to find dataset */
                            MSG_PrintError(MSG_MMSC_CANT_COMPUTE_ss, MSC_ConfigXML, configdata->macro[p].name );
                            continue;
                        }

                        MSC_StaticMacroTable[path[i]].activeset = dataset;

                        if (MSC_StaticMacroTable[path[i]].vectornum!=-1)
                        {
                            UINT32_T vector_mac;
                            /* set position equal to vectoring macro, and do not update carry flag */
                            vector_mac = MSC_StaticMacroTable[path[i]].vectornum;

                            MSC_StaticMacroTable[path[i]].currpos[dataset] =
                                MSC_StaticMacroTable[vector_mac].currpos[MSC_StaticMacroTable[vector_mac].activeset] %
                                configdata->macro[p].data[dataset].data_Count;
                        }
                        else
                        {
                            switch (configdata->macro[p].research)
                            {
                            case MSC_RESEARCH_PRIORITY:
                            case MSC_RESEARCH_SEQUENTIAL:
                                {
                                    if (MSC_MacroRandomFlag)
                                    {
                                        MSC_StaticMacroTable[path[i]].currpos[dataset] = rand() % configdata->macro[p].data[dataset].data_Count;
                                    }
                                    else
                                    {
                                        MSC_StaticMacroTable[path[i]].currpos[dataset]+=carry;
                                        /* check for overflow of counter */

                                        if (MSC_StaticMacroTable[path[i]].currpos[dataset] >=
                                            MSC_StaticMacroTable[path[i]].dataset[dataset])
                                        {
                                            /* MSC_StaticMacroTable[path[i]].currpos[dataset] = 0; */
                                            MSC_StaticMacroTable[path[i]].currpos[dataset] = 0;
                                            MSC_StaticMacroTable[path[i]].overflow[dataset] = 1;
                                            carry = 1;
                                            *next_start_i = -1;
                                            /* search previous level */
                                            for (ii=i-1; (int)ii>=0; ii--)
                                            {
                                                /* skip INDEX and FUNCTION macro */
                                                if (path[ii]==-1) continue;

                                                /* skip ABORT macro */
                                                if (!(path[ii] & 0x80000000))
                                                {
                                                    p = path[ii] & 0x7fffffff;
                                                    if (configdata->macro[MSC_StaticMacroTable[p].index].type == MSC_MACROTYPE_LIST)
                                                    {
                                                        if (!MSC_StaticMacroTable[p].overflow[MSC_StaticMacroTable[p].activeset])
                                                        {
                                                            *next_start_i = ii;
                                                            break;
                                                        }
                                                    }
                                                }
                                            }
                                            if (*next_start_i == -1) return 2;
                                            goto L1;
                                        }
                                        else
                                        {
                                            *next_start_i = i;
                                            MSC_StaticMacroTable[path[i]].overflow[dataset] = 0;
                                            carry = 0;
                                        }
                                    }
                                    break;
                                }
                            case MSC_RESEARCH_RANDOM:
                                {
                                    MSC_StaticMacroTable[path[i]].currpos[dataset] = rand() % configdata->macro[p].data[dataset].data_Count;
                                    break;
                                }
                            }
                        }
                        /* set the value for macro */
                        strcpy(MSC_StaticMacroTable[path[i]].repl, configdata->macro[p].data[dataset].data[MSC_StaticMacroTable[path[i]].currpos[dataset]]);

                        if ((last_set < i) && (MSC_StaticMacroTable[path[i]].vectornum==-1))
                        {
                            last_set = i;
                        }
                        break;
                    }
            }
        }
    }

    if (!MSC_MacroRandomFlag)
    {
        if ( *next_start_i!=0)
            return RESULT_OK;

        if (counter != 0)
        {
            for (i=0; i<pathlen; i++)
            {
                if ((path[i] & 0x80000000) == 0)
                {

                    if (MSC_StaticMacroTable[path[i]].currpos && 
                        (MSC_StaticMacroTable[path[i]].currpos[MSC_StaticMacroTable[path[i]].activeset] !=0 )
                        && MSC_StaticMacroTable[path[i]].active)
                        return RESULT_OK;
                }
            }
        }
    }
    else
        return 1;

    return (counter != 0)?2:1;

L_abort:
    /* check for possible continuation */
    for (i = 0; i <= last_set; i++)
    {
        if (((path[i] & 0x80000000) == 0 ) && !MSC_StaticMacroTable[path[i]].fpc
            && (MSC_StaticMacroTable[path[i]].vectornum == -1))
        {
            if (MSC_StaticMacroTable[path[i]].currpos[MSC_StaticMacroTable[path[i]].activeset] != 0)
            {
                carry = 1;
                goto L1;
            }
        }
    }

    if (!counter && (try_count == 1) )
    {
        carry = 1;
        goto L1;
    }
    /* that's all */
    return 2;
}
