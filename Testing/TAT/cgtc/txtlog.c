/******************************************************************************
*
*       COPYRIGHT 2001-2003 MOTOROLA, ALL RIGHTS RESERVED
*
*       The copyright notice above does not evidence any
*       actual or intended publication of such source code.
*
* Functions:    
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  Text dumping functions for C code generation templates
*
* Notes:        None
*
******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "testkern.h"
#include "txtlog.h"

static FILE* pfileTXT;

char * TAT_testErrMSG[]= 
{
    "No errors",                                                /* TAT_OK */

    "Unknown (undeclared) signal id received",                  /* TAT_ERR_INVALID_SIGNAL_ID */
    "Signal was received from unknown(undeclared) instance",    /* TAT_ERR_INVALID_INSTANCE_ID */

    "Error calling user's send function",                       /* TAT_ERR_SEND_EVENT */
    "No possibility to continue test",                          /* TAT_ERR_CANT_CONTINUE */
    
    "Error received event",                                     /* TAT_ERR_RECV_DATA */
    "Received unexpected event",                                /* TAT_ERR_EVENT_UNEXP */

    "ASSERT condition failed",                                  /* TAT_ERR_COND_ASSERT */

    "Raised FAILED verdict",                                    /* TAT_ERR_VERDICT_FAIL */
    "Raised INCONCLUSIVE verdict",                              /* TAT_ERR_VERDICT_INC */
    "Timeout error",                                            /* TAT_ERR_TIMEOUT */
    "Test error open file",                                     /* TAT_ERR_FILE */
    "Test allocated memory",                                    /* TAT_ERR_MEMORY */

    "Test internal error",                                      /* TAT_ERR_INTERNAL */
    "Error resetting a IUT",                                    /* TAT_ERR_RESET_IUT */
    "Queue overflow"                                            /* TAT_ERR_QUEUE */
};

static char * testErrType[]= 
{
    "TAT_OK",                         /* no errors */
    "TAT_ERR_INVALID_SIGNAL_ID",      /* unknown (undeclared) signal id received */
    "TAT_ERR_INVALID_INSTANCE_ID",    /* signal was received from unknown(undeclared) instance */
    "TAT_ERR_SEND_EVENT",             /* error calling user's send function */
    "TAT_ERR_CANT_CONTINUE",          /* no possibility to continue test */
    "TAT_ERR_RECV_DATA",              /* received event */
    "TAT_ERR_EVENT_UNEXP",            /* received unexpected event */
    "TAT_ERR_COND_ASSERT",            /* ASSERT condition failed */
    "TAT_ERR_VERDICT_FAIL",           /* raised FAILED verdict */
    "TAT_ERR_VERDICT_INC",            /* raised INCONCLUSIVE verdict */
    "TAT_ERR_TIMEOUT",                /* timeout error */
     
    "TAT_ERR_FILE",                   /* test error open file */
    "TAT_ERR_MEMORY",                 /* test allocated memory */
    "TAT_ERR_INTERNAL",               /* test internal error */
    "TAT_ERR_RESET_IUT",              /* error resetting a IUT */
    "TAT_ERR_QUEUE"                   /* queue overflow  */
};

/***************************************************************************
 * Function : TAT_OpenLogTXT
 *
 * Description: open log file
 *
 * Returns: int
 *
 * Notes: None
 *
 **************************************************************************/
TAT_TEST_RESULT TAT_OpenLogTXT(TAT_TEST_INSTANCE* id)
{
    char temp[200];
    if( (id->currCaseID == 0) && (id->currCaseIteration == 0) )
    {
        sprintf(temp,"%s.txt",id->currSuiteName);
        if( (pfileTXT = fopen(temp,"w")) == NULL )
        {
            id->testStatus = TAT_ERR_FILE;
            printf("Error open text log file!\n");
            return(TAT_TEST_CRITICAL);
        }
    }
    fprintf(pfileTXT, "RMRK: Test case %s started. (Iteration:%d)\n", id->currCaseName, id->currCaseIteration);
    fflush(pfileTXT);
    return(TAT_TEST_CONTINUE);
}

/***************************************************************************
 * Function : TAT_LogFuncTXT
 *
 * Description: process text output log of a test executing
 *
 * Returns: int
 *
 * Notes: None
 *
 **************************************************************************/
TAT_TEST_RESULT TAT_LogFuncTXT(TAT_TEST_INSTANCE* id, TAT_EVENT_INSTANCE *evInst, TAT_EVENT_INSTANCE *evInstReal)
{
    char paramStr[TAT_MAX_TXT_LOG_STR_LEN] = "";
	char strBuffer[TAT_MAX_TXT_LOG_STR_LEN] = "";
    char tmpStr[TAT_MAX_TXT_LOG_STR_LEN] = "";
    unsigned int lineMPR;
    int i;
    char prefix[11] = "";
    TAT_INSTANCE_DEF *instFrom, *instTo;
    TAT_SIGNAL_DEF * sigdata;
    TAT_EVENT_INSTANCE *_evInst;

	if( (evInst == NULL) && (evInstReal == NULL) )
    {
        fprintf(pfileTXT, "ERROR: %s\n", TAT_testErrMSG[id->testStatus]);
		if ( id->queue.length != 0 )
		{
			if ( id->queue.length == 1 )
				fprintf(pfileTXT, "There is 1 signal left in the Queue:\n");
			else 
				fprintf(pfileTXT, "There are %d signals left in the Queue:\n", id->queue.length);

			TAT_GetQueueSignals(id);
		}
        return(TAT_TEST_CONTINUE);
    }

    if( evInstReal->srcRefInf != NULL )
        lineMPR = evInstReal->srcRefInf[evInstReal->srcRefInfLen-1].lineNumber;
    else
        lineMPR = 0;

    if( (evInst == NULL) && (evInstReal != NULL) )
        _evInst = evInstReal;
    else
        _evInst = evInst;

    if( (evInst != evInstReal) && (evInst != NULL) )
    {
        switch( _evInst->eventType )
        {
            case TAT_SEND_EV:
                sigdata = TAT_GetSignalData(id, _evInst->eventID);
                if (!sigdata) 
                {
                    id->testStatus = TAT_ERR_INVALID_SIGNAL_ID;
                    id->dumpEvent(id, NULL, _evInst);
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

						/* Output signal`s parameteres comments if exist*/
						if ( _evInst->eventParamsComment != NULL)
							strcat(paramStr, _evInst->eventParamsComment[i]);

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
                #ifdef TAT_TIME_SUPPORT
				#ifdef TAT_FLOAT_TIME
					sprintf(strBuffer, "%s{%06f}[%04u] SEND: %s%s <%s->%s>",
				#else
					sprintf(strBuffer, "%s{%06u}[%04u] SEND: %s%s <%s->%s>",
				#endif
                                    prefix,
                                    id->internalClock(id),
                                    lineMPR,
                                    sigdata->signalName,
                                    paramStr,
                                    instFrom->instanceName,
                                    instTo->instanceName);
                #else
                sprintf(strBuffer, "%s[%04u] SEND: %s%s <%s->%s>",
                                    prefix,
                                    lineMPR,
                                    sigdata->signalName,
                                    paramStr,
                                    instFrom->instanceName,
                                    instTo->instanceName);
                #endif
            break;
            case TAT_RECV_EV:
                sigdata = TAT_GetSignalData(id, _evInst->eventID);
                if (!sigdata) 
                {
                    id->testStatus = TAT_ERR_INVALID_SIGNAL_ID;
                    id->dumpEvent(id, NULL, _evInst);
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

						/* Output signal`s parameteres comments if exist*/
						if ( evInstReal->eventParamsComment != NULL)
							strcat(paramStr, evInstReal->eventParamsComment[i]);

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
                #ifdef TAT_TIME_SUPPORT
				#ifdef TAT_FLOAT_TIME
					sprintf(strBuffer, "%s{%06f}[%04u] RECV: %s%s <%s->%s>",
				#else
					sprintf(strBuffer, "%s{%06u}[%04u] RECV: %s%s <%s->%s>",
				#endif
									prefix,
                                    id->internalClock(id),
                                    lineMPR,
                                    sigdata->signalName,
                                    paramStr,
                                    instFrom->instanceName,
                                    instTo->instanceName);
                #else
                sprintf(strBuffer, "%s[%04u] RECV: %s%s <%s->%s>",
                                    prefix,
                                    lineMPR,
                                    sigdata->signalName,
                                    paramStr,
                                    instFrom->instanceName,
                                    instTo->instanceName);
                #endif
           break;
           default:
                sprintf(strBuffer, "ERROR: Unknow event type.\n");
                id->testStatus = TAT_ERR_INTERNAL;
        }
    } else {
        switch( _evInst->eventType )
        {
        case TAT_CONDITION_EV:
            {
                if( TAT_GetInstListTXT(id, _evInst, paramStr) )
                {
                    id->testStatus = TAT_ERR_INVALID_INSTANCE_ID;
                    return(TAT_TEST_CRITICAL);
                }
                if( _evInst->eventInitialText != NULL )
                    sprintf(tmpStr," /* %s */",*(_evInst->eventInitialText));

                #ifdef TAT_TIME_SUPPORT
				#ifdef TAT_FLOAT_TIME
					sprintf(strBuffer, "{%06f}[%04u] COND: %s%s <%s>",
				#else
					sprintf(strBuffer, "{%06u}[%04u] COND: %s%s <%s>",
				#endif
                                    id->internalClock(id),
                                    lineMPR,
                                    (char *)(_evInst->eventData),
                                    tmpStr,
                                    paramStr);
                #else
                sprintf(strBuffer, "[%04u] COND: %s%s <%s>",
                                    lineMPR,
                                    (char *)(_evInst->eventData),
                                    tmpStr,
                                    paramStr);
                #endif
                break;
            }
        case TAT_ACTION_EV:
            {
                if( TAT_GetInstListTXT(id, _evInst, paramStr) )
                {
                    id->testStatus = TAT_ERR_INVALID_INSTANCE_ID;
                    return(TAT_TEST_CRITICAL);
                }
                if( _evInst->eventInitialText != NULL )
                    strcpy(tmpStr, *(_evInst->eventInitialText));

                #ifdef TAT_TIME_SUPPORT
				#ifdef TAT_FLOAT_TIME
					sprintf(strBuffer, "{%06f}[%04u] ACTN: %s %s <%s>",
				#else
					sprintf(strBuffer, "{%06u}[%04u] ACTN: %s %s <%s>",
				#endif
                                    id->internalClock(id),
                                    lineMPR,
									(char *)_evInst->eventData,
                                    tmpStr,
                                    paramStr);
                #else
                sprintf(strBuffer, "[%04u] ACTN: %s <%s>",
                                    lineMPR,
                                    tmpStr,
                                    paramStr);
                #endif
                break;
            }
        case TAT_REFERENCE_EV:
            {
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
                sprintf(strBuffer, "ERROR: Unknown event type.\n");
                id->testStatus = TAT_ERR_INTERNAL;
            }
        }
    }
    fprintf(pfileTXT, "%s\n", strBuffer);
    fflush(pfileTXT);
    return(TAT_TEST_CONTINUE);
}

/***************************************************************************
 * Function : TAT_CloseLogTXT
 *
 * Description: close log file
 *
 * Returns: int
 *
 * Notes: None
 *
 **************************************************************************/
TAT_TEST_RESULT TAT_CloseLogTXT(TAT_TEST_INSTANCE* id)
{
    fprintf(pfileTXT, "RMRK: Test %s finished. Status=%s (Iteration: %d)\n\n", id->currCaseName, testErrType[id->testStatus], id->currCaseIteration);
    fflush(pfileTXT);
    if( (id->currCaseID == (id->testCount-1)) && (id->currCaseIteration == (id->repeatCount-1)) )
        fclose(pfileTXT);
    return(TAT_TEST_CONTINUE);
}

int TAT_GetInstListTXT(TAT_TEST_INSTANCE* id, TAT_EVENT_INSTANCE *evInst, char* dest)
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


int TAT_GetQueueSignals (TAT_TEST_INSTANCE* id)
{
	unsigned int i,j;
    TAT_INSTANCE_DEF *instFrom, *instTo;
    TAT_SIGNAL_DEF * sigdata;
    TAT_EVENT_INSTANCE *evInst;

    char paramStr[TAT_MAX_TXT_LOG_STR_LEN] = "";
    char strBuffer[TAT_MAX_TXT_LOG_STR_LEN] = "";
    char tmpStr[TAT_MAX_TXT_LOG_STR_LEN] = "";
    char prefix[11] = "";

	for (j = 0; j < id->queue.length; j++)
	{

		evInst = &(id->queue.queueData[id->queue.head + j]);
        
		sigdata = TAT_GetSignalData(id, evInst->eventID);
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
                sigdata->params[i].dumpFunc((char *)(evInst->eventData) + sigdata->params[i].dataOffset, tmpStr);
                strcat(paramStr, tmpStr);
				strcpy(tmpStr,"");
                if( (sigdata->paramCount - i) >= 2 )
                    strcat(paramStr, ",");
            }
            strcat(paramStr, ")");
        }

        if( ((instFrom = TAT_GetInstanceID(id, evInst->eventInstance[TAT_INST_FROM])) == NULL) ||
            ((instTo = TAT_GetInstanceID(id, evInst->eventInstance[TAT_INST_TO])) == NULL) )
        {
            id->testStatus = TAT_ERR_INVALID_INSTANCE_ID;
            return(TAT_TEST_CRITICAL);
        }
        #ifdef TAT_TIME_SUPPORT
        sprintf(strBuffer, "%s{%06u} RECV: %s%s <%s->%s>",
                            prefix,
                            id->internalClock(id),
                            sigdata->signalName,
                            paramStr,
                            instFrom->instanceName,
                            instTo->instanceName);
        #else
        sprintf(strBuffer, "%s RECV: %s%s <%s->%s>",
                            prefix,
                            sigdata->signalName,
                            paramStr,
                            instFrom->instanceName,
                            instTo->instanceName);
        #endif

		fprintf(pfileTXT, "-- %s\n", strBuffer);
	}
	return TAT_TEST_CONTINUE;
}
