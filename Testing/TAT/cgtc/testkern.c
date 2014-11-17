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
* Description:  Kernel functions for C code generation templates
*
* Notes:        None
*
******************************************************************************/

#include "testkern.h"


/***************************************************************************
 * Function : TAT_FindInstanceID
 *
 * Description: find instance id within instance list
 *
 * Returns: 0, if queue is full
 *
 * Notes: None
 *
 **************************************************************************/
TAT_INSTANCE_DEF * TAT_GetInstanceID(TAT_TEST_INSTANCE* id, TAT_INSTANCE_ID inst)
{
    size_t i;
    for (i=0; i<id->instanceCount; i++)
    {
        if ( id->instanceDef[i].instanceID == inst)
            return &id->instanceDef[i];
    }
    return NULL;
}

/***************************************************************************
 * Function : TAT_GetSignalData
 *
 * Description: search for signal data specified by signal id in sorted
 *              array of signals stored in TEST_INSTANCE
 *
 * Returns: pointer to signal data/ NULL if not found
 *
 * Notes: None
 *
 **************************************************************************/
TAT_SIGNAL_DEF * TAT_GetSignalData(TAT_TEST_INSTANCE* id, TAT_SIGNAL_ID sig)
{
    size_t                  lo, hi, mid, half, num;
    int                     result;

    if ( !id || !id->signalCount )
        return NULL;

    lo = 0;
    num = id->signalCount;
    hi = num - 1;

    while ( lo <= hi )
    {
        if ( half = ( num >> 1 ) )
        {
            mid = lo + ( ( num & 1 ) ? half : ( half - 1 ) );

            if ( !( result = sig - id->signalDef[mid].signalID ) )
                return ( &id->signalDef[mid] );
            else if ( result < 0 )
            {
                hi = mid - 1;
                num = ( num & 1 ) ? half : half - 1;
            }
            else
            {
                lo = mid + 1;
                num = half;
            }
        }
        else if ( num )
            return ( sig == id->signalDef[lo].signalID ) ? &id->signalDef[lo] : NULL;
        else
            break;
    }

    return NULL;
}

/***************************************************************************
 * Function : TAT_PushQueue 
 *
 * Description: push signal into queue
 *
 * Returns: TAT_TEST_CONTINUE/TAT_TEST_CRITICAL
 *
 * Notes: None
 *
 **************************************************************************/
static TAT_TEST_RESULT TAT_PushQueue(TAT_TEST_INSTANCE* id, TAT_EVENT_INSTANCE *evInst)
{
    TAT_SIGNAL_DEF * sigdata;
    size_t i;

    sigdata = TAT_GetSignalData(id, evInst->eventID);
    if (!sigdata) 
    {
        id->testStatus = TAT_ERR_INVALID_SIGNAL_ID;
        id->dumpEvent(id, NULL, evInst);
        return (id->testResult = TAT_TEST_CRITICAL);
    }

    if ( id->queue.length >= TAT_QUEUE_LENGTH ) 
    {
        id->testStatus = TAT_ERR_QUEUE;
        id->dumpEvent(id, NULL, evInst);
        return (id->testResult = TAT_TEST_CRITICAL);
    }

    /* copy event data */
    memcpy(&id->queue.queueData[id->queue.tail], evInst, sizeof(evInst[0]));
    /* set current time */
    id->clock = id->internalClock(id);
    id->queue.queueData[id->queue.tail].eventTime.timeCalc.minTime = id->clock;
    id->queue.queueData[id->queue.tail].eventTime.timeCalc.maxTime = id->clock;

    /* copy message data if exist */
    if ( sigdata->dataSize )
    {
        if ( (id->queue.queueData[id->queue.tail].eventData = malloc(sigdata->dataSize)) == NULL )
        {
            id->testStatus = TAT_ERR_MEMORY;
            id->dumpEvent(id, NULL, evInst);
            return (id->testResult = TAT_TEST_CRITICAL);
        }
        memcpy(id->queue.queueData[id->queue.tail].eventData, evInst->eventData, sigdata->dataSize);
    }

#ifdef TAT_INSTANCE_SUPPORT
    for (i=0; i<evInst->eventInstanceSize; i++)
    {
        if (!TAT_GetInstanceID(id, evInst->eventInstance[i]))
        {
            id->testStatus = TAT_ERR_INVALID_INSTANCE_ID;
            id->dumpEvent(id, NULL, evInst);
            return (id->testResult = TAT_TEST_CRITICAL);
        }
    }
    if ( (id->queue.queueData[id->queue.tail].eventInstance = malloc(sizeof(TAT_INSTANCE_ID)*evInst->eventInstanceSize)) == NULL )
    {
        id->testStatus = TAT_ERR_MEMORY;
        id->dumpEvent(id, NULL, evInst);
        return (id->testResult = TAT_TEST_CRITICAL);
    }
    memcpy(id->queue.queueData[id->queue.tail].eventInstance, evInst->eventInstance,
        sizeof(TAT_INSTANCE_ID)*evInst->eventInstanceSize);
#endif
    
    id->queue.tail++;
	if (id->queue.tail == TAT_QUEUE_LENGTH)
	{
		id->queue.tail = 0;
	}
	id->queue.length++;
	return TAT_TEST_CONTINUE;
}


/***************************************************************************
 * Function : TAT_GetQueue
 *
 * Description: get a pointer to signal in the queue
 *
 * Returns: pointer / NULL if queue is empty
 *
 * Notes:
 *
 **************************************************************************/
static TAT_EVENT_INSTANCE * TAT_GetQueue(TAT_TEST_INSTANCE* id)
{
	if ( (id->queue.length == 0) || !id)
	{
		return NULL;
	}
    return &id->queue.queueData[id->queue.head];
}


/***************************************************************************
 * Function : TAT_PopQueue
 *
 * Description: free queue cell element if right signal is got
 *
 * Returns: none
 *
 * Notes:
 *
 **************************************************************************/
static void TAT_PopQueue(TAT_TEST_INSTANCE* id)
{
	if (id && id->queue.length)
    {
        id->queue.queueData[id->queue.head].eventType = TAT_UNKNOWN_EV;
        id->queue.queueData[id->queue.head].eventID = 0;

        free(id->queue.queueData[id->queue.head].eventData);

#ifdef TAT_INSTANCE_SUPPORT
        id->queue.queueData[id->queue.head].eventInstanceSize = 0;
        free(id->queue.queueData[id->queue.head].eventInstance);
#endif
        id->queue.head++;
        /* check for overflow */
        if (id->queue.head == id->queue.limit) id->queue.head = 0;
        id->queue.length--;
    }
}

/***************************************************************************
 * Function : TAT_ResetQueue
 *
 * Description: free all queue cell elements
 *
 * Returns:
 *
 * Notes:
 *
 **************************************************************************/
static int TAT_ResetQueue(TAT_TEST_INSTANCE* id)
{
	while (id->queue.length) TAT_PopQueue(id);
	id->queue.tail = 0;
	id->queue.head = 0;
	return(1);
}


/***************************************************************************
 * Function : TAT_ReceiveMessage
 *
 * Description: push a signal into the queue via TAT_PushQueue func
 *
 * Returns: TAT_TEST_RESULT
 *
 * Notes: None
 *
 **************************************************************************/
TAT_TEST_RESULT TAT_ReceiveMessage(TAT_TEST_INSTANCE* id, TAT_EVENT_INSTANCE *evInst)
{
    TAT_TEST_RESULT res;

    /* push message in queue and check for error */
    res = TAT_PushQueue(id, evInst);

    if (res != TAT_TEST_CONTINUE)
        return res;

    /* start execution only if it is not started yet */
    if (id->runned == 0)
    {
        id->runned=1;
        res=id->testCaseCall(id);
        id->runned=0;
    }

    return res;
}

/***************************************************************************
* Function : TAT_LogManagerOpen
*
* Description: open files for process output information
*
* Returns: int
*
* Notes: None
*
**************************************************************************/
static TAT_TEST_RESULT TAT_LogManagerOpen(TAT_TEST_INSTANCE* id)
{
    size_t i = 0;
    while( i < id->logMethodsNumber )
    {
        if( (id->logMethods[i].openLog(id)) != TAT_TEST_CONTINUE )
            return TAT_TEST_CRITICAL;
        i++;
    }
    return TAT_TEST_CONTINUE;
}

/***************************************************************************
* Function : TAT_LogManager
*
* Description: process output information
*
* Returns: int
*
* Notes: None
*
**************************************************************************/
static TAT_TEST_RESULT TAT_LogManagerDump(TAT_TEST_INSTANCE* id, TAT_EVENT_INSTANCE *evInst, TAT_EVENT_INSTANCE *evInstReal)
{
    size_t i = 0;
    while( i < id->logMethodsNumber )
    {
        if( (id->logMethods[i].logFunc(id, evInst, evInstReal)) != TAT_TEST_CONTINUE )
            return TAT_TEST_CRITICAL;
        i++;
    }
    return TAT_TEST_CONTINUE;
}

/***************************************************************************
* Function : TAT_LogManagerClose
*
* Description: close files of process output information
*
* Returns: int
*
* Notes: None
*
**************************************************************************/
static TAT_TEST_RESULT TAT_LogManagerClose(TAT_TEST_INSTANCE* id)
{
    size_t i = 0;
    while( i < id->logMethodsNumber )
    {
        if( (id->logMethods[i].closeLog(id)) != TAT_TEST_CONTINUE )
            return TAT_TEST_CRITICAL;
        i++;
    }
    return TAT_TEST_CONTINUE;
}

/***************************************************************************
 * Function : TAT_CreateTestInstance
 *
 * Description: initialize test instance structure
 *              qsize - maximal size of queue
 *              logsize - maximal number of dumping functions
 *
 * Returns: pointer to instance / NULL
 *
 * Notes: None
 *
 **************************************************************************/
TAT_TEST_INSTANCE* TAT_CreateTestInstance(size_t qsize, size_t logsize, size_t timelimit,
                                          TAT_SYSTEM_INIT resetSystem,
                                          TAT_SEND_CALL sendMessage,
                                          TAT_CLOCK_FUNC clockFunc,
                                          TAT_TEST_STARTED testStart,
                                          TAT_TEST_FINISH testFinish )
{
    TAT_TEST_INSTANCE *testInst;
    static size_t testInstanceID = 0;

    /* check input parameters */
    if (!qsize || !sendMessage || !clockFunc)
        return NULL;

    /* allocate memory and initialize it with zero */
    testInst = (TAT_TEST_INSTANCE*)calloc(1, sizeof(testInst[0]));
    if ( !testInst ) return(NULL);

    testInst->instanceID = testInstanceID++;

    testInst->resetSystem = resetSystem;
    testInst->testCaseStart = testStart;
    testInst->testCaseFinish = testFinish;
    testInst->sendMessage = sendMessage;
    testInst->internalClock = clockFunc;
    testInst->timeoutLimit = timelimit;

    testInst->queue.limit = qsize;
    testInst->queue.queueData = calloc(qsize, sizeof(testInst->queue.queueData[0]));

    if (!testInst->queue.queueData)
    {
        free(testInst);
        return NULL;
    }

    if (logsize)
    {
        testInst->logMethods = calloc(logsize, sizeof(testInst->logMethods[0]));
        if (!testInst->logMethods)
        {
            free(testInst->queue.queueData);
            free(testInst);
            return NULL;
        }
    }

    testInst->logMethodsLimit = logsize;

    /* set no cases are initialized */
    testInst->initedCaseID = (size_t)-1;

    /* initialize addresses of Kernel functions */
    testInst->getQueue = TAT_GetQueue;
    testInst->popQueue = TAT_PopQueue;
    testInst->openLog = TAT_LogManagerOpen;
    testInst->closeLog = TAT_LogManagerClose;
    testInst->dumpEvent = TAT_LogManagerDump;

    return testInst;
}

/***************************************************************************
* Function : Close_TestInstance
*
* Description: Flushes a test instance data struct
*
* Returns: 1 if success, 0 otherwise
*
* Notes: Exported
*
**************************************************************************/
int TAT_FreeTestInstance(TAT_TEST_INSTANCE* id)
{
    if (!id)
        return 0;
    
    /* clean queue */
    TAT_ResetQueue(id);

    free(id->queue.queueData);
    free(id->logMethods);


    free(id);

    return 1;
}

/***************************************************************************
* Function : TAT_RegisterLogFunc
*
* Description: register function for logging test process
*
* Returns: TAT_TEST_CONTINUE/TAT_TEST_CRITICAL
*
* Notes: None
*
**************************************************************************/
TAT_TEST_RESULT TAT_RegisterLogFunc(TAT_TEST_INSTANCE* id, TAT_TEST_OPEN_LOG openLog, TAT_TEST_LOG logFunc, TAT_TEST_CLOSE_LOG closeLog)
{
    if( (id == NULL) || (id->logMethodsNumber >= id->logMethodsLimit) )
    {
        return TAT_TEST_CRITICAL;
    }

    id->logMethods[id->logMethodsNumber].openLog = openLog;
    id->logMethods[id->logMethodsNumber].logFunc = logFunc;
    id->logMethods[id->logMethodsNumber].closeLog = closeLog;
    id->logMethodsNumber++;
    return TAT_TEST_CONTINUE;
}

/***************************************************************************
* Function : TAT_StartSuite
*
* Description: Initilaize test instance with specific test suite
*
* Returns: TAT_TEST_CONTINUED/TAT_TEST_CRITICAL
*
* Notes: None
*
**************************************************************************/
TAT_TEST_RESULT TAT_StartSuite(TAT_TEST_INSTANCE* id, TAT_SUITE_INIT init)
{
    if (!id || !init)
        return TAT_TEST_CRITICAL;

    if (init(id)!=TAT_TEST_CONTINUE)
        return TAT_TEST_CRITICAL;
    
    id->initedCaseID=(size_t)-1;

    return TAT_TEST_CONTINUE;
}

TAT_TEST_RESULT TAT_CloseTest(TAT_TEST_INSTANCE* id)
{
    size_t i;
    if (!id) return TAT_TEST_CRITICAL;
    if (id->initedCaseID==(size_t)-1) return TAT_TEST_CONTINUE;
    for (i=0; i<id->varCount; i++)
    {
        free(id->vars[i]);
    }
    free(id->vars);
    id->vars = NULL;
    free(id->timeLV);
    id->timeLV = NULL;
    free(id->testCaseBranches);
    id->testCaseBranches = NULL;
    free(id->testCaseTimers);
    id->testCaseTimers =NULL;
    free(id->testCaseLoops);
    id->testCaseLoops = NULL;
    id->initedCaseID=(size_t)-1;
    return TAT_TEST_CONTINUE;
}

/***************************************************************************
 * Function : TAT_StartTest
 *
 * Description: Initialize specific test case
 *
 * Returns: 
 *
 * Notes: None
 *
 **************************************************************************/
TAT_TEST_RESULT TAT_StartTest(TAT_TEST_INSTANCE* id, size_t testid)
{
    size_t i;
    if (!id || testid>=id->testCount)
        return TAT_TEST_CRITICAL;

    /* clean data structure if needed */
    if (id->initedCaseID!=(size_t)-1)
    {
        if (TAT_CloseTest(id)!=TAT_TEST_CONTINUE)
            return TAT_TEST_CRITICAL;
    }

    if (id->testTable[testid].testInitFunc(id)!=TAT_TEST_CONTINUE)
    {
        return TAT_TEST_CRITICAL;
    }
    id->initedCaseID = testid;
    id->currCaseID = testid;

    /* allocate memory for test case variables */
    id->vars = malloc(id->varCount*sizeof(id->vars[0]));
    for (i=0; i<id->varCount; i++)
    {
        id->vars[i]=calloc(1, id->varDefs[i].dataSize);
    }
    id->testCaseCall = id->testTable[testid].testCallFunc;
    id->currCaseName = id->testTable[testid].testCaseName;

    id->testCaseBranches = calloc(id->branchCount, sizeof(id->testCaseBranches[0]));
    if (id->branchCount && !id->testCaseBranches)
        return TAT_TEST_CRITICAL;

    id->testCaseTimers = calloc(id->timerCount, sizeof(id->testCaseTimers[0]));
    if (id->timerCount && !id->testCaseTimers)
        return TAT_TEST_CRITICAL;

    id->testCaseLoops = calloc(id->loopCount, sizeof(id->testCaseLoops[0]));
    if (id->loopCount && !id->testCaseLoops)
        return TAT_TEST_CRITICAL;
    
    id->timeLV = calloc(id->timeLabelCount, sizeof(id->timeLV[0]));
    if (id->timeLabelCount && !id->timeLV)
        return TAT_TEST_CRITICAL;

    /* nothing is initialized */
    id->currCaseIteration = (size_t) -1;
    id->nextState = 0;

    return TAT_TEST_CONTINUE;
}

TAT_TEST_RESULT TAT_StartNextTrace(TAT_TEST_INSTANCE* id)
{
    if (!id || (id->initedCaseID==(size_t)-1))
        return TAT_TEST_CRITICAL;

    /* start next iteration */
    if (id->currCaseIteration==(size_t)-1)
    {
        id->currCaseIteration = 0;
    }
    else
        id->currCaseIteration++;

    if (id->currCaseIteration >= id->repeatCount)
        return TAT_TEST_FINISHED;

    /* clean queue */
    TAT_ResetQueue(id);

    /* reset IUT by calling user's function */
    if (id->resetSystem && id->resetSystem(id)!=TAT_TEST_CONTINUE)
    {
        id->testStatus = TAT_ERR_RESET_IUT;
        return id->testResult = TAT_TEST_CRITICAL;
    }
    /* clean error status */
    id->testStatus = TAT_OK;
    id->testResult = TAT_TEST_CONTINUE;
    id->timeoutCounter = 0;
    id->execFlag = 0;
    id->nextState = 0;
    id->tag = 0;
    if (id->testCaseStart && id->testCaseStart(id)!=TAT_TEST_CONTINUE)
    {
        id->testStatus = TAT_ERR_RESET_IUT;
        return id->testResult = TAT_TEST_CRITICAL;
    }

    if (id->openLog(id)!=TAT_TEST_CONTINUE)
        return TAT_TEST_CRITICAL;

    return TAT_TEST_CONTINUE;
}

TAT_TEST_RESULT TAT_ContinueTrace(TAT_TEST_INSTANCE* id)
{
    TAT_TEST_RESULT res = TAT_TEST_CONTINUE;
    int pr_state;

    if (!id || (id->initedCaseID==(size_t)-1) || (id->currCaseIteration==(size_t)-1))
        return TAT_TEST_CRITICAL;

    if (id->runned)
        return id->testResult;

    id->runned=1;
    while (res == TAT_TEST_CONTINUE)
    {
        pr_state = id->nextState;

        /* update time */
        id->clock = id->internalClock(id);

        res = id->testCaseCall(id);


        if (res == TAT_TEST_CONTINUE)
        {
            /* check for timeout in the same state */
            if (pr_state == id->nextState)
                id->timeoutCounter++;
            else
                id->timeoutCounter=0;

            if (id->timeoutCounter >= id->timeoutLimit)
            {
                id->testStatus = TAT_ERR_TIMEOUT;
                id->testResult = TAT_TEST_FINISHED;
                if (id->dumpEvent(id, NULL,NULL)!=TAT_TEST_CONTINUE)
                    return TAT_TEST_CRITICAL;

                /* call user's function */
                if (id->testCaseFinish && id->testCaseFinish(id)!=TAT_TEST_CONTINUE)
                    return TAT_TEST_CRITICAL;

                /* close dump */
                if (id->closeLog(id)!=TAT_TEST_CONTINUE)
                    return TAT_TEST_CRITICAL;

                id->runned=0;
                return TAT_TEST_FINISHED;
            }
            id->runned=0;
            return TAT_TEST_CONTINUE;
        }
            

        /* if test case finished */
        if (res == TAT_TEST_FINISHED)
        {
            /* check for signals in queue */
            if (id->queue.length)
            {
                id->testStatus = TAT_ERR_EVENT_UNEXP;
                id->testResult = TAT_TEST_FINISHED;
                if (id->dumpEvent(id, NULL,NULL)!=TAT_TEST_CONTINUE)
                    return TAT_TEST_CRITICAL;
            }
            
            /* call user's function */
            if (id->testCaseFinish && id->testCaseFinish(id)!=TAT_TEST_CONTINUE)
                return TAT_TEST_CRITICAL;

            /* close dump */
            if (id->closeLog(id)!=TAT_TEST_CONTINUE)
                return TAT_TEST_CRITICAL;

            id->runned=0;
            return TAT_TEST_FINISHED;
        }

    }
    id->runned=0;
    return res;
}

TAT_TEST_RESULT TAT_ContinueTest(TAT_TEST_INSTANCE* id)
{
    TAT_TEST_RESULT res = TAT_TEST_CONTINUE;

    if (!id || (id->initedCaseID==(size_t)-1))
        return TAT_TEST_CRITICAL;

    if (id->currCaseIteration==(size_t)-1)
    {
        res = TAT_StartNextTrace(id);
        if (res!=TAT_TEST_CONTINUE)
            return TAT_TEST_CRITICAL;
    }

    while (res==TAT_TEST_CONTINUE)
    {
        res = TAT_ContinueTrace(id);
        if (res == TAT_TEST_CRITICAL)
            return TAT_TEST_CRITICAL;

        if (res == TAT_TEST_CONTINUE)
            return TAT_TEST_CONTINUE;

        if (res == TAT_TEST_FINISHED)
        {
            res = TAT_StartNextTrace(id);
        }
    }
    return res;
}

TAT_TEST_RESULT TAT_ContinueSuite(TAT_TEST_INSTANCE* id)
{
    TAT_TEST_RESULT res = TAT_TEST_CONTINUE;

    if (!id)
        return TAT_TEST_CRITICAL;

    if (id->initedCaseID == (size_t)-1)
    {
        res = TAT_StartTest(id, 0);
        if (res!=TAT_TEST_CONTINUE)
            return TAT_TEST_CRITICAL;
    }

    while (res==TAT_TEST_CONTINUE)
    {
        res = TAT_ContinueTest(id);
        if (res == TAT_TEST_CRITICAL)
            return TAT_TEST_CRITICAL;

        if (res == TAT_TEST_CONTINUE)
            return TAT_TEST_CONTINUE;

        if (res == TAT_TEST_FINISHED)
        {
            id->initedCaseID++;
            if (id->initedCaseID>=id->testCount)
                return TAT_TEST_FINISHED;

            res = TAT_StartTest(id, id->initedCaseID);
        }
    }
    return res;
}


int TAT_Dump_int (void * data, char *dest)
{
	sprintf(dest+strlen(dest), "%d", *((int *)data));
	return 1;
}

int TAT_Dump_Uint (void *data, char *dest)
{
	sprintf(dest+strlen(dest), "%u", *((unsigned int *)data));
	return 1;
}

int TAT_Dump_double (void *data, char *dest)
{
	sprintf(dest+strlen(dest), "%g", *((double *)data));
	return 1;
}

int TAT_Dump_float (void *data, char *dest)
{
	sprintf(dest+strlen(dest), "%f", *((float *)data));
	return 1;
}

int TAT_Dump_char (void *data, char *dest)
{
	sprintf(dest+strlen(dest), "'%c'", *((char *)data));
	return 1;
}

int TAT_Dump_String (void *data, char *dest)
{
	strcat(dest,"\"");
	strcat(dest, *((char **)data));
	strcat(dest,"\"");
	return 1;
}

int TAT_Dump_Uchar (void *data, char *dest)
{
	sprintf(dest+strlen(dest), "0x%02x", *((unsigned char *)data));
	return 1;
}
