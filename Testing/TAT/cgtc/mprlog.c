/******************************************************************************
*
*       COPYRIGHT 2001-2002 MOTOROLA, ALL RIGHTS RESERVED
*
*       The copyright notice above does not evidence any
*       actual or intended publication of such source code.
*
* Functions:    
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  MSC-PR dumping functions for C code generation templates
*
* Notes:        None
*
******************************************************************************/
#include <stdlib.h>
#include "testkern.h"
#include "mprlog.h"

char * TAT_testErrMSG_TXT[]= 
{
	"No errors",                                                /* TAT_OK */

    "Unknown (undeclared) signal id received",                  /* TAT_ERR_INVALID_SIGNAL_ID */
    "Signal was received from unknown(undeclared) instance",    /* TAT_ERR_INVALID_INSTANCE_ID */

    "Error calling user's send function",                       /* TAT_ERR_SEND_EVENT */
    "No possibility to continue test",                          /* TAT_ERR_CANT_CONTINUE */
	
	"Error received event",                                     /* TAT_ERR_RECV_DATA */
	"Received unexpected event",		                        /* TAT_ERR_EVENT_UNEXP */

	"ASSERT condition failed",			                        /* TAT_ERR_COND_ASSERT */

	"Raised FAILED verdict",	    		                    /* TAT_ERR_VERDICT_FAIL */
	"Raised INCONCLUSIVE verdict",		                        /* TAT_ERR_VERDICT_INC */
	"Timeout error",	    	                                /* TAT_ERR_TIMEOUT */
	"Test error open file",			                            /* TAT_ERR_FILE */
    "Test allocated memory",			                        /* TAT_ERR_MEMORY */

    "Test internal error",                                      /* TAT_ERR_INTERNAL */
    "Error resetting a IUT",                                    /* TAT_ERR_RESET_IUT */
    "Queue overflow"                                            /* TAT_ERR_QUEUE */
};

char * TAT_testErrTAG_TXT[]= 
{
	"TAT_OK",

    "TAT_ERR_INVALID_SIGNAL_ID",
    "TAT_ERR_INVALID_INSTANCE_ID",

    "TAT_ERR_SEND_EVENT",
    "TAT_ERR_CANT_CONTINUE",
	
	"TAT_ERR_RECV_DATA",
	"TAT_ERR_EVENT_UNEXP",

	"TAT_ERR_COND_ASSERT",

	"TAT_ERR_VERDICT_FAIL",
	"TAT_ERR_VERDICT_INC",
	"TAT_ERR_TIMEOUT",
	"TAT_ERR_FILE",
    "TAT_ERR_MEMORY",

    "TAT_ERR_INTERNAL",
    "TAT_ERR_RESET_IUT",
    "TAT_ERR_QUEUE"
};

/*file pointer to mpr list file*/
FILE *TAT_mprListFile;
FILE *TAT_mprFile;
unsigned int TAT_mprLineIndex;
unsigned int TAT_mprEventIndex;

/***************************************************************************
 * Function : TAT_OpenLogMPR
 *
 * Description: open mpr log (.mpr) file
 *
 * Returns: int
 *
 * Notes: None
 *
 **************************************************************************/
TAT_TEST_RESULT TAT_OpenLogMPR(TAT_TEST_INSTANCE* id)
{
	char mprFileName[200];
    unsigned int i;

    /*open mpr list file*/
    if( (id->currCaseID == 0) && (id->currCaseIteration == 0) )
    {
	    if ( (TAT_mprListFile=fopen("mpr_list.lst","w")) == NULL )
        {
            id->testStatus = TAT_ERR_FILE;
            printf("Error open mpr list file!\n");
            return(TAT_TEST_CRITICAL);
        }
    }

    /* open mpr file for mpr trace log support */
	sprintf(mprFileName, "%s_%u.mpr", id->currCaseName, id->currCaseIteration);
	if ( (TAT_mprFile=fopen(mprFileName, "w")) == NULL )
    {
        id->testStatus = TAT_ERR_FILE;
        printf("Error open mpr log file for test case %s, iteration %u!\n", id->currCaseName, id->currCaseIteration);
        return(TAT_TEST_CRITICAL);
    }

    TAT_mprLineIndex = 1;
    TAT_mprEventIndex = 1;

    /* output mpr file name to list file*/
    fprintf(TAT_mprListFile,"%s\n",mprFileName);

    fprintf(TAT_mprFile,"mscdocument %s;\n",id->currCaseName);
    fprintf(TAT_mprFile,"msc %s;\n",id->currCaseName);
    for( i=0; i<id->instanceCount; i++ )
        fprintf(TAT_mprFile,"%s: instance;\n",id->instanceDef[i].instanceName);

    fflush(TAT_mprFile);
    return(TAT_TEST_CONTINUE);
}

/***************************************************************************
 * Function : TAT_LogFuncMPR
 *
 * Description: process dump output log (.fsr) of a test executing
 *
 * Returns: int
 *
 * Notes: None
 *
 **************************************************************************/
TAT_TEST_RESULT TAT_LogFuncMPR(TAT_TEST_INSTANCE* id, TAT_EVENT_INSTANCE *evInst, TAT_EVENT_INSTANCE *evInstReal)
{
    char paramStr[TAT_MAX_MPR_LOG_STR_LEN] = "";
    char tmpStr[TAT_MAX_MPR_LOG_STR_LEN] = "";
    char timeStr[TAT_MAX_MPR_LOG_STR_LEN] = "";
    char errStr[TAT_MAX_MPR_LOG_STR_LEN] = "";
    TAT_INSTANCE_DEF *instFrom, *instTo;
    int i;
    TAT_SIGNAL_DEF * sigdata;
    TAT_EVENT_INSTANCE *_evInst;

    #ifdef TAT_TIME_SUPPORT
	#ifdef TAT_FLOAT_TIME
		sprintf(timeStr,"/*{@%f}*/",id->internalClock(id));
	#else
		sprintf(timeStr,"/*{@%u}*/",id->internalClock(id));
	#endif
    #endif

    if( (evInst == NULL) && (evInstReal == NULL) )
    {
        fprintf(TAT_mprFile, "all: condition %sERROR comment '<!%s(Trace%d,Event%d)!>';\n",
                                timeStr,
                                TAT_testErrTAG_TXT[id->testStatus],
                                id->currCaseIteration,
                                TAT_mprEventIndex);
        return(TAT_TEST_CONTINUE);
    }

    if( (evInst == NULL) && (evInstReal != NULL) )
    {
        _evInst = evInstReal;
        sprintf(errStr," comment '<!%s(Trace%d,Event%d)!>'",
                                TAT_testErrTAG_TXT[id->testStatus],
                                id->currCaseIteration,
                                TAT_mprEventIndex);
    }
    else
        _evInst = evInst;

/*
    if( evInst != evInstReal )
    {
*/
        switch (_evInst->eventType)
        {
        case TAT_SEND_EV:
        case TAT_RECV_EV:
            {
                sigdata = TAT_GetSignalData(id, _evInst->eventID);
                if (!sigdata) 
                {
                    id->testStatus = TAT_ERR_INVALID_SIGNAL_ID;
                    id->dumpEvent(id, NULL, evInst);
                    return (id->testResult = TAT_TEST_CRITICAL);
                }

                if( sigdata->params != NULL )
                {
                    strcpy(paramStr, "(");
                    for( i=0; i<sigdata->paramCount; i++ )
                    {
                        sigdata->params[i].dumpFunc((char *)(_evInst->eventData) + sigdata->params[i].dataOffset, tmpStr);
                        strcat(paramStr, tmpStr);
						strcpy(tmpStr,"");
                        if( (sigdata->paramCount - i) >= 2 )
                            strcat(paramStr, ",");
                    }
                    strcat(paramStr, ")");
                }

                if( ((instFrom = TAT_GetInstanceID(id, _evInst->eventInstance[TAT_INST_FROM])) == NULL) ||
                    ((instTo = TAT_GetInstanceID(id, _evInst->eventInstance[TAT_INST_TO])) == NULL) )
                {
                    id->testStatus = TAT_ERR_INVALID_INSTANCE_ID;
                    return(TAT_TEST_CRITICAL);
                }

                fprintf(TAT_mprFile, "%s: out %s%s,%d%s to %s%s;\n",
                                        instFrom->instanceName,
                                        timeStr,
                                        sigdata->signalName,
                                        TAT_mprLineIndex,
                                        paramStr,
                                        instTo->instanceName,
                                        errStr);
                fprintf(TAT_mprFile, "%s: in %s%s,%d%s from %s;\n",
                                        instTo->instanceName,
                                        timeStr,
                                        sigdata->signalName,
                                        TAT_mprLineIndex,
                                        paramStr,
                                        instFrom->instanceName);
                TAT_mprLineIndex++;
                break;
            }
/*
        default:
            {
                break;
            }
        }
    } else {
        switch (_evInst->eventType)
        {
*/
        case TAT_CONDITION_EV:
            {
                if( TAT_GetInstListMPR(id, _evInst, paramStr) )
                {
                    id->testStatus = TAT_ERR_INVALID_INSTANCE_ID;
                    return(TAT_TEST_CRITICAL);
                }
                if( _evInst->eventInitialText != NULL )
                    sprintf(tmpStr," /* %s */",*(_evInst->eventInitialText));

                fprintf(TAT_mprFile, "%s: condition %s%s%s%s;\n",
                                        paramStr,
                                        timeStr,
                                        (char *)(_evInst->eventData),
                                        tmpStr,
                                        errStr);
                break;
            }
        case TAT_ACTION_EV:
            {
                if( TAT_GetInstListMPR(id, _evInst, paramStr) )
                {
                    id->testStatus = TAT_ERR_INVALID_INSTANCE_ID;
                    return(TAT_TEST_CRITICAL);
                }
                if( _evInst->eventInitialText != NULL )
                    strcpy(tmpStr, *(_evInst->eventInitialText));

                fprintf(TAT_mprFile, "%s: action '%s %s'%s;\n",
                                        paramStr,
										(char *)_evInst->eventData,
                                        tmpStr,
                                        errStr);
                break;
            }
        case TAT_REFERENCE_EV:
            {
                if( TAT_GetInstListMPR(id, _evInst, paramStr) )
                {
                    id->testStatus = TAT_ERR_INVALID_INSTANCE_ID;
                    return(TAT_TEST_CRITICAL);
                }
                if( _evInst->eventInitialText != NULL )
                    strcpy(tmpStr, *(_evInst->eventInitialText));

                fprintf(TAT_mprFile, "%s: reference %s%s /* %s */%s;\n",
                                        paramStr,
                                        timeStr,
                                        (char *)(_evInst->eventData),
                                        tmpStr,
                                        errStr);
                break;
            }
        case TAT_CREATE_EV:
            {
                break;
            }
        case TAT_STOP_EV:
            {
                break;
            }
        default:
            {
                break;
            }
        }
/*    } if close*/
    TAT_mprEventIndex++;
    fflush(TAT_mprFile);
    return(TAT_TEST_CONTINUE);
}

/***************************************************************************
 * Function : TAT_CloseLogMPR
 *
 * Description: close mpr log (.mpr) file
 *
 * Returns: int
 *
 * Notes: None
 *
 **************************************************************************/
TAT_TEST_RESULT TAT_CloseLogMPR(TAT_TEST_INSTANCE* id)
{
    unsigned int i;

    for( i=0; i<id->instanceCount; i++ )
        fprintf(TAT_mprFile,"%s: endinstance;\n",id->instanceDef[i].instanceName);

    fprintf(TAT_mprFile,"endmsc;\n");

    /* close mpr file for mpr trace log support */
  	fclose(TAT_mprFile);

    /* close mpr list file for mpr trace log support */
    if( (id->currCaseID == (id->testCount-1)) && (id->currCaseIteration == (id->repeatCount-1)) )
    	fclose(TAT_mprListFile);

    return(TAT_TEST_CONTINUE);
}

int TAT_GetInstListMPR(TAT_TEST_INSTANCE* id, TAT_EVENT_INSTANCE *evInst, char* dest)
{
    unsigned int i;
    TAT_INSTANCE_DEF *instFrom;

    for( i=0; i<evInst->eventInstanceSize; i++ )
    {
        if( (instFrom = TAT_GetInstanceID(id, evInst->eventInstance[i])) == NULL)
            return 1;
        strcat(dest, instFrom->instanceName);
        if( (evInst->eventInstanceSize - i) >= 2 )
            strcat(dest, ",");
    }
    return 0;
}