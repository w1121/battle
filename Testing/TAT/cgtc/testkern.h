#ifndef TAT_TEST_KERNEL_H
#define TAT_TEST_KERNEL_H
/******************************************************************************
*
*       COPYRIGHT 2001-2003 MOTOROLA, ALL RIGHTS RESERVED
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
* Description:  TAT TTST kernel header
*
* Notes:        None
*
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <stddef.h>

/* use '/DTAT_QUEUE_LENGTH = <value>' to define maximum queue length for incoming messages */
#ifndef TAT_QUEUE_LENGTH
#define TAT_QUEUE_LENGTH 256
#endif /* TAT_QUEUE_LENGTH */

#ifndef TAT_COMMENTS_LENGTH
#define TAT_COMMENTS_LENGTH 4096
#endif /* TAT_COMMENTS_LENGTH */

#ifndef TAT_TEST_CASE_NAME
#define TAT_TEST_CASE_NAME 4096
#endif /* TAT_TEST_CASE_NAME */

/* use /DMSC_MAX_WAIT_TIME = <value> or Set_Max_Wait_Time to alter timeout value */
#ifndef MSC_MAX_WAIT_TIME
#define MSC_MAX_WAIT_TIME 1000
#endif /* MSC_MAX_WAIT_TIME */

/* use /DLOG_FUNC_MAX_LEN = <value> to set maximum number of logging functions*/
#ifndef LOG_FUNC_MAX_LEN
#define LOG_FUNC_MAX_LEN 3
#endif /* LOG_FUNC_MAX_LEN */

#define TAT_LOST_INSTANCE       (-1)
#define TAT_FOUND_INSTANCE      (-2)
#define TAT_FINAL_STATE (-1)


/* use /DTAT_FLOAT_TIME to use floating-point times */
#ifndef TAT_FLOAT_TIME
typedef unsigned int TAT_TIME_TYPE;
#else
typedef float TAT_TIME_TYPE;
#endif

#define TAT_INST_FROM   0
#define TAT_INST_TO     1

typedef unsigned int TAT_SIGNAL_ID;
typedef unsigned int TAT_INSTANCE_ID;
typedef unsigned int TAT_EVENT_ID;

/* event types, executed in tests */
typedef enum
{
    TAT_UNKNOWN_EV          = 0,
    TAT_SEND_EV             = 1,
    TAT_RECV_EV             = 2,
    TAT_CONDITION_EV        = 3,
    TAT_ACTION_EV           = 4,
    TAT_REFERENCE_EV        = 5,
    TAT_CREATE_EV           = 6,
    TAT_STOP_EV             = 7
} TAT_EVENT_TYPE;


/* test status */
typedef enum
{
    TAT_OK = 0,                         /* no errors */
    TAT_ERR_INVALID_SIGNAL_ID = 1,      /* unknown (undeclared) signal id received */
    TAT_ERR_INVALID_INSTANCE_ID = 2,    /* signal was received from unknown(undeclared) instance */
    TAT_ERR_SEND_EVENT = 3,             /* error calling user's send function */
    TAT_ERR_CANT_CONTINUE = 4,          /* no possibility to continue test */
    TAT_ERR_RECV_DATA = 5,              /* received event */
    TAT_ERR_EVENT_UNEXP = 6,            /* received unexpected event */
    TAT_ERR_COND_ASSERT = 7,            /* ASSERT condition failed */
    TAT_ERR_VERDICT_FAIL = 8,           /* raised FAILED verdict */
    TAT_ERR_VERDICT_INC = 9,            /* raised INCONCLUSIVE verdict */
    TAT_ERR_TIMEOUT = 10,               /* timeout error */
    
    TAT_ERR_FILE = 11,                  /* test error open file */
    TAT_ERR_MEMORY = 12,                /* test allocated memory */
    TAT_ERR_INTERNAL = 13,              /* test internal error */
    TAT_ERR_RESET_IUT = 14,             /* error resetting a IUT */
    TAT_ERR_QUEUE = 15                  /* queue overflow  */
} TAT_TEST_ERROR;

#define TAT_VRS_TIME_WARN   0x00000001
#define TAT_VRS_ANNO_WARN   0x00000002

typedef enum
{
    TAT_TEST_CONTINUE = 0,              /* environment shall call CONTINUE function */
    TAT_TEST_FINISHED = 1,              /* test execution finished, verdict obtained */
    TAT_TEST_CRITICAL = 2               /* test execution aborted due to critical error */
} TAT_TEST_RESULT;

/* result on event execution */
typedef enum
{
    TAT_VERDICT_PASS = 0,
    TAT_VERDICT_FAIL = 1,
    TAT_VERDICT_INC = 2
} TAT_VERDICT_TYPE;

typedef struct
{
    TAT_TIME_TYPE minTime;
    TAT_TIME_TYPE maxTime;
} TAT_TIME_INTERVAL;

typedef struct
{
    TAT_TIME_INTERVAL timeCalc;
    TAT_TIME_INTERVAL timeVRS;
    char*  timeUser;
} TAT_TIME_STRUCT;


typedef struct 
{
    char * filename;
    unsigned int level;
    unsigned int firstline;
    unsigned int lastline;
} TAT_SRC_FILE_DEF;

typedef struct
{
    unsigned int fileNumber;    /* number of mpr file for dump curr event*/
    unsigned int lineNumber;    /* number of line in mpr file for dump curr event*/
} TAT_SRC_REF_STRUCT;

typedef struct
{
    TAT_EVENT_TYPE      eventType;          /* event kind */
    TAT_EVENT_ID        eventID;            /* only for eventType={TAT_SEND|TAT_RECV}*/
    void*               eventData;          /* pointer to event's data */
#ifdef TAT_TIME_SUPPORT
    TAT_TIME_STRUCT     eventTime;
#endif
    size_t              eventInstanceSize;  /* number of instances event attached to */
    TAT_INSTANCE_ID    *eventInstance;      /* pointer to event's instances */
    char*               eventComment;       /* comment text for event */
    size_t              srcRefInfLen;       /* number of source references */
    TAT_SRC_REF_STRUCT  *srcRefInf;         /* pointer to source references */
    TAT_VERDICT_TYPE    eventVerdict;       /* PASS, FAIL, INCONCLUSIVE */
    int                 eventTimeLabel;     /* =-1, if no label, otherwise label number */
    char**              eventInitialText;   /* used for Messages */
    char**              eventParamsComment; /* used for Message`s parameters*/
} TAT_EVENT_INSTANCE;


/* TAT queue for incoming messages from IUT */
typedef struct
{
    TAT_EVENT_INSTANCE  *queueData;         /* storage for queue elements */
    size_t length;                          /* current length of queue */
    size_t head;                            /* head of queue */
    size_t tail;                            /* tail of queue */
    size_t limit;                           /* limit size of queue */
} TAT_QUEUE;

/* prototype  structure definition */
/* struct TAT_TEST_INSTANCE_STRUCT; */

/* test instance definition */
typedef struct TAT_TEST_INSTANCE_STRUCT TAT_TEST_INSTANCE;

/* TAT_TEST_FUNC is set to current test-implementation function */
typedef TAT_TEST_RESULT (* TAT_TEST_FUNC)(TAT_TEST_INSTANCE*);

/* description of test cases */
typedef struct
{
    const char *testCaseName;               /* name of a test case */
    const TAT_TEST_FUNC testCallFunc;       /* test invocation */
    const TAT_TEST_FUNC testInitFunc;       /* test initialization */
} TAT_TEST_TABLE;

/* system reset function type */
typedef TAT_TEST_RESULT (* TAT_SYSTEM_INIT)(TAT_TEST_INSTANCE*);

/* user's dumping function type */
/***************************************************************************
 * Function type: TAT_DUMP_FUNCTION
 *
 * Description: converts abstract data to string
 *
 * Returns: 0 if error
 *
 * Notes: None
 *
 **************************************************************************/
typedef int (* TAT_DUMP_FUNCTION)(void * data, char * dest);

#ifndef offsetof
#define offsetof(type, mem) ((size_t) ((char *)&((type *)0)->mem - (char *)(type *)0))
#endif

typedef struct 
{
    int         paramID;        /* parameter's ID */
    char       *paramName;      /* parameter name */
    size_t      dataSize;       /* size of parameter */
    size_t      dataOffset;     /* offset from data structure, calculated dynamically */
    TAT_DUMP_FUNCTION dumpFunc; /* function for dumping parameter */
} TAT_SIG_PARAM_DEF;

typedef struct 
{
    TAT_SIGNAL_ID   signalID;   /* signal identifier */
    char           *signalName; /* signal name */
    size_t          dataSize;   /* size of data structure */
    int             paramCount; /* number of parameters */
    TAT_SIG_PARAM_DEF *params;  /* reference to parameters definition */
    int             priority;   /* signal priority */
} TAT_SIGNAL_DEF;

typedef struct 
{
    TAT_INSTANCE_ID  instanceID;    /* signal identifier */
    char            *instanceName;  /* signal name */
    int              priority;      /* instance priority */
} TAT_INSTANCE_DEF;

typedef struct 
{
    char            *name;          /* time label text */
    int             generated;      /* true, if generated */
} TAT_TIME_LABEL_DEF;

typedef struct 
{
    int             varID;          /* variable's ID */
    char           *name;           /* time label text */
    size_t          dataSize;       /* size of parameter */
    TAT_DUMP_FUNCTION dumpFunc;     /* function for dumping parameter */
} TAT_VARIABLE_DEF;

/***************************************************************************
 * Function type: TAT_CLOCK_FUNC
 *
 * Description: called by Test Kernel to get current time
 *
 * Returns: environment's time for test instance
 *
 * Notes: None
 *
 **************************************************************************/
typedef TAT_TIME_TYPE (* TAT_CLOCK_FUNC)(TAT_TEST_INSTANCE*);

/* suite initialize function */
typedef TAT_TEST_RESULT (* TAT_SUITE_INIT)(TAT_TEST_INSTANCE*);

/* msc state function */
typedef TAT_TEST_RESULT (* TAT_STATE_FUNC)(TAT_TEST_INSTANCE*);

/* TAT_TEST_FINISH shall be called after test termination */
typedef TAT_TEST_RESULT (* TAT_TEST_FINISH)(TAT_TEST_INSTANCE*);

/* TAT_TEST_STARTED shall be called just after selecting test case for execution */
typedef TAT_TEST_RESULT (* TAT_TEST_STARTED)(TAT_TEST_INSTANCE*);

/* TAT_SEND_CALL shall be used for sending messages to IUT */
typedef TAT_TEST_RESULT (* TAT_SEND_CALL)(TAT_TEST_INSTANCE*, TAT_EVENT_INSTANCE*);

/***************************************************************************
 * Function type : TAT_TEST_OPEN_LOG
 *
 * Description: function of this type called before each test case started
 *
 * Returns: status of execution
 *
 * Notes: None
 *
 **************************************************************************/
typedef TAT_TEST_RESULT (* TAT_TEST_OPEN_LOG) (TAT_TEST_INSTANCE*);

typedef TAT_TEST_RESULT (* TAT_TEST_LOG) (TAT_TEST_INSTANCE*, TAT_EVENT_INSTANCE*, TAT_EVENT_INSTANCE*);
typedef TAT_TEST_RESULT (* TAT_TEST_CLOSE_LOG) (TAT_TEST_INSTANCE*);

typedef TAT_EVENT_INSTANCE * (* TAT_GET_QUEUE) (TAT_TEST_INSTANCE*);
typedef void (* TAT_POP_QUEUE)(TAT_TEST_INSTANCE* id);

typedef struct
{
    TAT_TEST_OPEN_LOG openLog;
    TAT_TEST_LOG logFunc;
    TAT_TEST_CLOSE_LOG closeLog;
} TAT_LOG_FUNC_STRUCT;



struct TAT_TEST_INSTANCE_STRUCT
{
    size_t              instanceID;                     /* unique identifier of test instance */
    TAT_SYSTEM_INIT     resetSystem;                    /* user's system reset */
    TAT_TEST_FUNC       testCaseCall;                   /* test case invocation */
    TAT_TEST_STARTED    testCaseStart;                  /* pointer to test_Start function */
    TAT_TEST_FINISH     testCaseFinish;                 /* pointer to test_Finish functin */
    TAT_SEND_CALL       sendMessage;                    /* pointer to function for sending signals */

    TAT_CLOCK_FUNC      internalClock;                  /* user's defined clock function */

    TAT_TIME_TYPE       clock;                          /* current clock */
	TAT_TIME_TYPE		lastEvent;						/* last event */

    size_t              timeoutCounter;                 /* timeout counter */
    size_t              timeoutLimit;                   /* timeout limit */
    size_t              execFlag;                       /* specific information about VRS */

    unsigned int        runned;                         /* set up when a test agent is being executed */
   
    TAT_QUEUE           queue;                          /* queue for receiving signals */
    TAT_LOG_FUNC_STRUCT *logMethods;                    /* storage for log methods */   
    size_t              logMethodsNumber;               /* number of actually registred logs */
    size_t              logMethodsLimit;                /* maximal number of logs */


    /* Kernel functions */
    TAT_GET_QUEUE       getQueue;                       /* function for getting queue element */
    TAT_POP_QUEUE       popQueue;                       /* function for removing first queue element */

    TAT_TEST_OPEN_LOG   openLog;                        /* Kernel function for creating logs */
    TAT_TEST_LOG        dumpEvent;                      /* Kernel function for dumping logs */
    TAT_TEST_CLOSE_LOG  closeLog;                       /* Kernel function for closing logs */

    /* Suite specific information */
    size_t              signalCount;                    /* number of signals in Test Suite */
    TAT_SIGNAL_DEF     *signalDef;                      /* signal definitions for proper work of dump functions */

    size_t              instanceCount;                  /* number of MSC instances */
    TAT_INSTANCE_DEF   *instanceDef;                    /* MSC instance definition */

    TAT_TEST_ERROR      testStatus;                     /* current test status */
    TAT_TEST_RESULT     testResult;                     /* current test result */

    char*               currSuiteName;                  /* name of test suite */

    size_t              testCount;                      /* number of tests in Test Suite */
    TAT_TEST_TABLE     *testTable;                      /* pointer to test table */

    /* Test Case specific information */
    char*               currCaseName;                   /* current test index in TAT_Tests table */
    size_t              currCaseID;                     /* ID of current test case from table */
    size_t              initedCaseID;                   /* ID of initialized test case */

    size_t              currCaseIteration;              /* current iteration for current test case */
    int                 nextState;                      /* current state in FSM */
    size_t              tag;                            /* branch tag */
    size_t              branchCount;                    /*  number of branches in current test case */
    size_t              repeatCount;                    /*  number of repetitions for current test case */
    size_t              timerCount;                     /*  number of timers in current test case */
    size_t              loopCount;                      /*  number of loops in current test case */

    size_t             *testCaseBranches;               /* allocated test branches */   
    size_t             *testCaseTimers;                 /* allocated timers */
    size_t             *testCaseLoops;                  /* allocated loops */
  
    size_t              timeLabelCount;                 /* number of time labels */
    TAT_TIME_LABEL_DEF *timeLabels;                     /* time label's definition */
    TAT_TIME_TYPE      *timeLV;                         /* values for time labels */

    size_t              varCount;                       /* number of variables */
    TAT_VARIABLE_DEF   *varDefs;                        /* variable's definition */
    void              **vars;                           /* values for variables */

    size_t  srcFilesCount;                              /* number of source files */
    TAT_SRC_FILE_DEF *srcFiles;                         /* definition of source files */
};


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
extern TAT_SIGNAL_DEF * TAT_GetSignalData(TAT_TEST_INSTANCE* id, TAT_SIGNAL_ID sig);

/***************************************************************************
* Function : TAT_ReceiveMessage
*
* Description: push message in test instance's queue
*
* Returns: status of action
*
* Notes: None
*
**************************************************************************/
extern TAT_TEST_RESULT TAT_ReceiveMessage(TAT_TEST_INSTANCE*, TAT_EVENT_INSTANCE*);

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
extern TAT_TEST_INSTANCE* TAT_CreateTestInstance(size_t qsize, size_t logsize, size_t timelimit,
                                             TAT_SYSTEM_INIT, TAT_SEND_CALL, TAT_CLOCK_FUNC,
                                             TAT_TEST_STARTED, TAT_TEST_FINISH );

/***************************************************************************
 * Function : TAT_FreeTestInstance
 *
 * Description: free allocated memory structure
 *
 * Returns: 0/1
 *
 * Notes: None
 *
 **************************************************************************/
extern int TAT_FreeTestInstance(TAT_TEST_INSTANCE*);

/***************************************************************************
 * Function : TAT_RegisterLogFunc
 *
 * Description: register user's dumping function
 *
 * Returns: TAT_TEST_CONTINUED/TAT_TEST_CRITICAL
 *
 * Notes: None
 *
 **************************************************************************/
extern TAT_TEST_RESULT TAT_RegisterLogFunc(TAT_TEST_INSTANCE*,
                                           TAT_TEST_OPEN_LOG, TAT_TEST_LOG, TAT_TEST_CLOSE_LOG);

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
extern TAT_TEST_RESULT TAT_StartSuite(TAT_TEST_INSTANCE*, TAT_SUITE_INIT );

/***************************************************************************
 * Function : TAT_ContinueSuite
 *
 * Description: pass control to test suite, switch among test cases and
 *              test runs automatically. 
 *
 * Returns: TAT_TEST_FINISHED after all tests/runs executed
 *
 * Notes: None
 *
 **************************************************************************/
extern TAT_TEST_RESULT TAT_ContinueSuite(TAT_TEST_INSTANCE*);

extern TAT_TEST_RESULT TAT_StartTest(TAT_TEST_INSTANCE*, size_t id);
extern TAT_TEST_RESULT TAT_ContinueTest(TAT_TEST_INSTANCE*);

extern TAT_TEST_RESULT TAT_StartNextTrace(TAT_TEST_INSTANCE*);
extern TAT_TEST_RESULT TAT_ContinueTrace(TAT_TEST_INSTANCE*);

extern int TAT_Dump_int (void *, char *);
extern int TAT_Dump_Uint (void *, char *);
extern int TAT_Dump_double (void *, char *);
extern int TAT_Dump_float (void *, char *);
extern int TAT_Dump_char (void *, char *);
extern int TAT_Dump_String (void *, char *);
extern int TAT_Dump_Uchar (void *, char *);

extern TAT_INSTANCE_DEF * TAT_GetInstanceID(TAT_TEST_INSTANCE*, TAT_INSTANCE_ID);

#endif /* TAT_TEST_KERNEL_H */

