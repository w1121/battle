#ifndef MSCEVENT_H
#define MSCEVENT_H
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
* Description:  MSC Events handling
*
* Notes:        None
*
******************************************************************************/
#include <portable.h>
#include <qmemory.h>
#include <strcoll.h>

#ifdef __cplusplus
extern                  "C"
{
#endif                                 /* C++ */

/* defines time representation in TT */
#ifndef MSC_INTEGER_TIME
    typedef double          MSC_TIME_VALUE;
#else
    typedef int             MSC_TIME_VALUE;
#endif

    typedef size_t              MSC_INSTANCE_ID_TYPE; /* instance ID for event */

#define MSC_INFINITY_CONST UINT_MAX

    typedef enum _MSC_EVENT_KIND_TYPE
    {
        MSC_EV_UNDEFINED = 0,       /*undefined element (usually error) */
        MSC_EV_EMPTY = 1,           /* empty element (used for handling removal) */
        MSC_EV_BREAK = 2,           /* 'break' event. Generated during 'exc' processing */
        MSC_EV_REFERENCE = 3,       /* MSC's reference event */
        MSC_EV_CONDITION = 4,       /* MSC's condition event */
        MSC_EV_ACTION = 5,          /* MSC's action event */
        MSC_EV_TIMER_SET = 6,       /* MSC's timer set event*/
        MSC_EV_TIMER_RESET = 7,     /* MSC's timer reset event */
        MSC_EV_TIMER_TIMEOUT = 8,   /* MSC's timer timeout event */
        MSC_EV_CONCURRENT_START = 9,  /* MSC's coregion start */
        MSC_EV_CONCURRENT_END = 10,   /* MSC's coregion finish */
        MSC_EV_OPERATOR = 11,         /* compound element for 'alt', 'loop', etc. */
        MSC_EV_IN_MESSAGE = 12,       /* MSC's input message event */
        MSC_EV_OUT_MESSAGE = 13,      /* MSC's output message event */
        MSC_EV_CREATE = 14,           /* MSC's create instance event */
        MSC_EV_STOP = 15,             /* MSC's stop instance event */

    /* special (combined) types of elements */
        MSC_EV_SEND_SIGNAL = 16,
        MSC_EV_RECV_SIGNAL = 17,
        MSC_EV_EXIST_SIGNAL = 18,
        MSC_EV_CODE = 19,
        MSC_EV_START_LOOP = 20,
        MSC_EV_END_LOOP = 21
    }
    MSC_EVENT_KIND_TYPE;

    typedef enum _MSC_TIMER_TYPE  /* MSC Timer set/reset/timeout specification */
    {
        MSC_UNKNOWN_TIMER = 0,   /* coulnd't be determined at loading phase */
        MSC_RELATIVE_TIMER = 1,  /* timing doesn't contain NOW */ 
        MSC_ABSOLUTE_TIMER = 2,  /* timing contains NOW */
        MSC_SPECIAL_TIMER = 3    /* latency specificator - numbers instead of signal name */
    }
    MSC_TIMER_TYPE;

    typedef enum _MSC_EVENT_KIND_TIME_TYPE /* MSC time label specification */
    {
        MSC_RELATIVE_TIME = 0,
        MSC_LABELED_TIME = 1
    }
    MSC_EVENT_KIND_TIME_TYPE;

    struct MSC_EVENT_KIND_STRUCTURE;

    typedef struct MSC_EVENT_KIND_STRUCTURE MSC_EVENT_KIND;

    typedef struct _MSC_TIME_INTERVAL
    {
        MSC_EVENT_KIND_TIME_TYPE     type;          /* time specification type */
        STRING_ID              str_MinTime;         /* minimal time in string form */
        STRING_ID              str_MaxTime;         /* maximal time in string form */
        size_t                  time_Label;         /* related label if needed */
    /* remember: binary forms may contain invalid information!!! */
        MSC_TIME_VALUE          min_Time;           /* minimal time in binary form */
        MSC_TIME_VALUE          max_Time;           /* maximal time in binary form */
        UINT32_T                    valid_Number;   /* =1, if time specification is number  */
        UINT32_T                    specified;      /* =1, if time was specified explicitly */
    }
    MSC_TIME_INTERVAL;

    typedef struct _MSC_TIMER_TIME
    {
        MSC_TIMER_TYPE          type;          /* timer specification type */
        STRING_ID              str_MinTime;    /* minimal time in string form */
        STRING_ID              str_MaxTime;    /* maximal time in string form */
    /* remember: binary forms may contain invalid information!!! */
        MSC_TIME_VALUE          min_Time;      /* minimal time in binary form */
        MSC_TIME_VALUE          max_Time;      /* maximal time in binary form */
    }
    MSC_TIMER_TIME;

    typedef enum _MSC_OPERATOR_TYPE /* MSC's inline expression type */
    {
        MSC_OP_SEQ,    /* MSC_OP_SEQ operator is included for compatibility issues.
                          It shouldn't be used actually. When it is detected an error will be displayed! */
        MSC_OP_ALT,    /* 'alt' - alternative composition */
        MSC_OP_PAR,    /* 'par' - parallel composition */
        MSC_OP_LOOP,   /* 'loop' - repetitive sequential composition */
        MSC_OP_OPT,    /* 'opt' - optional composition */
        MSC_OP_EXC     /* 'exc' - exceptional part */
    }
    MSC_OPERATOR_TYPE;

    typedef enum _MSC_EVENT_KIND_SYNC_TYPE /* syncronous event */
    {
        MSC_ASYNC_EVENT, /* ordinal message (as defined in z.120) by default */
        MSC_SYNC_EVENT   /* synchronized message ('~' before name)
                            partial ordering sets same dependency for in & out events 
                            which is combined one of in & out for ordinal */
    }
    MSC_EVENT_KIND_SYNC_TYPE;


    typedef struct _MSC_INLINE_EXP_EVENT
    {
        STRING_ID              l1;    /* first parameter */
        STRING_ID              l2;    /* second parameter (loops) */
        STRING_ID              name;  /* name of loop */
        STRING_ID              text;  /* exported from condition */
        size_t                 id;    /* unique ID of event */
    }
    MSC_INLINE_EXP_EVENT;

/* structure for handling message specificators */
    typedef struct _MSC_MESSAGE_PARAM
    {
        STRING_ID              varName;        /* variable name to store value */
        size_t                 valueCount;     /* number of possible values */
        STRING_ID             *values;         /* values */
    } MSC_MESSAGE_PARAM;

    typedef struct _MSC_MESSAGE_STORAGE
    {
        size_t                  indicator;      /* message unique identifier (recalculated) */
        STRING_ID              source_ID;      /* original message identifier */
        STRING_ID              message_Name;   /* message name */
        MSC_TIME_INTERVAL  delay;          /* duration field. Used only for MSC_EV_IN_MESSAGE */
        size_t                  param_Count;    /* number of actual parameters */
        STRING_ID             *message_Params; /* pointer to parameters */
        MSC_MESSAGE_PARAM      *params_Values;  /* parsed parameters */
        size_t                  line_Number;    /* line number where declared */
    }
    MSC_MESSAGE_STORAGE;

    typedef struct _MSC_MESSAGE_EVENT
    {
        size_t                  indicator;  /* message unique identifier */
        int                     inst;       /* extracted from 'to/from' field */
        int                     priority;   /* message priority */
    }
    MSC_MESSAGE_EVENT;

    typedef struct _MSC_TIMER_EVENT
    {
        MSC_TIMER_TIME          timing;         /* declaration of timer */
        size_t                  timer_ID;       /* unique TmrX field (recalculated) */
        STRING_ID              signal_Name;     /* timer signal */
        MSC_EVENT_KIND              *relation;  /* pairing set event for timeout event*/
    }
    MSC_TIMER_EVENT;

    typedef enum _MSC_REF_PARAM_TYPE  /* parameter type for Reference event */
    {
        TAT_REF_PARAM,                /* TAT syntax used / * #X=1... * / */
        PTK_REF_PARAM                 /* PTK syntax used / *<+(1,2)>* / */
    } MSC_REF_PARAM_TYPE;

    typedef struct _MSC_REFERENCE_EVENT
    {
        STRING_ID              refr_Name;      /* MSC name */
        size_t                 param_Count;    /* number of parameters */
        char                  **macro_Name;    /* real macro names */
        STRING_ID             *refr_Subst;     /* substitutions (may be macros) */
        STRING_ID              scale;          /* time scale for reference (not used) */
        MSC_REF_PARAM_TYPE     ref_Type;       /* parameter type for Reference event */
    }
    MSC_REFERENCE_EVENT;

    typedef struct _MSC_OPERATOR_EVENT
    {
        MSC_OPERATOR_TYPE       type;           /* type of operator */
        size_t                  frame_Count;    /*number of frames */
        size_t                  path_Limit;     /* number of paths to be restricted inside */
        STRING_ID              l1;              /* first parameter of loop operator*/
        STRING_ID              l2;              /* second parameter (loops) */
        MSC_EVENT_KIND         **frame;         /* link to event */
        UINT32_T                flag_End;       /* if set, means end of operator (for ATS) */
    }
    MSC_OPERATOR_EVENT;

    typedef struct _MSC_CREATE_EVENT
    {
        MSC_INSTANCE_ID_TYPE    instance;       /* idenitifier of created instance */
        size_t                  param_Count;    /* number of parameters */
        STRING_ID             *params;          /* parameters */
    }
    MSC_CREATE_EVENT;

    typedef struct _MSC_CONDITION_EVENT
    {
        STRING_ID              text;           /* ordinal part */
        STRING_ID              call;           /* something in comments */
        UINT32_T               generated;      /* condition was generated */
    }
    MSC_CONDITION_EVENT;

    typedef struct _MSC_EVENT_FILE_REFERENCE
    {
        size_t                  link_Index;     /* index to referenced file */
        size_t                  line_Number;    /* line number from that file */
    }
    MSC_EVENT_FILE_REFERENCE;

    typedef enum _MSC_VRS_VERDICT_TYPE          /* for VRS support */
    {
        MSC_VRS_NO_VERDICT = 0,
        MSC_VRS_ABSOLUTE_TIME,
        MSC_VRS_OVERLAPED_WITH,
        MSC_VRS_ANNOTATION_PROVED,
        MSC_VRS_ANNOTATION_NOT_PROVED,
        MSC_VRS_ANNOTATION_INCONCLUSIVE,
        MSC_VRS_PROTOCOL_COLLISION,
        MSC_VRS_PROTOCOL_APPLIED,
        MSC_VRS_NO_PROTOCOL
    }
    MSC_VRS_VERDICT_TYPE;

    extern char *          VRS_EventVerdictText[];
    extern char *          VRS_DiagramVerdictText[];

    typedef enum _MSC_EVENT_KIND_VERDICT
    {
        MSC_VERDICT_PASS = 0,
        MSC_VERDICT_FAIL,
        MSC_VERDICT_INCONCLUSIVE
    }
    MSC_EVENT_KIND_VERDICT;

    typedef struct _MSC_VRS_VERDICT
    {
        MSC_VRS_VERDICT_TYPE    type;           /* verdict type */
        STRING_ID               protocol_Name;  /* protocol name */
        STRING_ID               protocol_Name2; /* collision protocol name, if applied */
        size_t                  protocol_Num;
        size_t                  protocol_Num2;
        MSC_TIME_INTERVAL       time_VRS;       /* VRS calculated time */
        size_t                  trace;          /* trace number */
        size_t                  event;          /* event number in trace */
    }
    MSC_VRS_VERDICT;


    struct MSC_EVENT_KIND_STRUCTURE
    {
        MSC_EVENT_KIND_TYPE          type;      /* kind of event */
        MSC_EVENT_KIND              *next;      /* pointer to next event in chain */

        STRING_ID              comment;         /* comment associated to this item */
        MSC_EVENT_KIND_SYNC_TYPE     sync;      /* synchronization */
        STRING_ID              condition;       /* [] field of event (condition of existence) */
        MSC_TIME_INTERVAL  time;                /* time specification from MPR */
        MSC_TIME_INTERVAL  time_Calc;           /* calculated time (by default=time) */

        size_t                  verdict_Count;  /* number of VRS verdicts for event */
        MSC_VRS_VERDICT        *verdict;        /* VRS verdict */

        size_t                  event_Mark;     /* event mark for SDEA,VRS in <%d> format */
        UINT32_T                   deleted;     /* if TRUE - event shall be ignored */

        size_t                  inst_Count;     /* number of instances for which event is binded */
        MSC_INSTANCE_ID_TYPE   *instances;      /* array with instance numbers */

        size_t                  labeled;        /* if non-zero label-1 reference to this event */

        size_t                  line_Number;    /* source line in MPR file */
        size_t                  branch_ID;      /* current branch identifier (to detect cross-operator events) */

        size_t                  src_Count;      /* number of source references */
        MSC_EVENT_FILE_REFERENCE    *src_Data;  /* source reference */

        MSC_EVENT_KIND_VERDICT       event_Verdict;  /* event execution verdict: PASS, FAIL, INCONCLUSIVE */

        /* data section */
        union
        {
            MSC_MESSAGE_EVENT       msg;    /* for handling in/out messages */
            MSC_OPERATOR_EVENT      alt;    /* for handling inline expressions */
            MSC_TIMER_EVENT         timer;  /* for handling timers */
            STRING_ID              text;   /* text , action & other */
            MSC_REFERENCE_EVENT     ref;    /* references */
            MSC_CREATE_EVENT        create; /* instance creation */
            MSC_CONDITION_EVENT     cond;   /* condition information */
            MSC_INLINE_EXP_EVENT    loop;   /* loop start statement */
        }
        data;
    };


/***************************************************************************
 * Function : MSC_IsDefaultEventTime
 *
 * Description: check time for default
 *
 * Returns: RESULT_OK/RESULT_ERR
 *
 * Notes:
 *
 **************************************************************************/
    extern LOGICAL          MSC_IsDefaultEventTime( MSC_EVENT_KIND * x );

/***************************************************************************
 * Function : MSC_IsPassiveEvent
 *
 * Description: Check event to be passive type
 *
 * Returns: RESULT_OK/RESULT_ERR
 *
 * Notes: None
 *
 **************************************************************************/
    extern int              MSC_IsPassiveEvent( MSC_EVENT_KIND * E );

/***************************************************************************
 * Function : MSC_IsTimerEvent
 *
 * Description: Check event to be timer
 *
 * Returns: RESULT_OK/RESULT_ERR
 *
 * Notes: None
 *
 **************************************************************************/
    extern int              MSC_IsTimerEvent( MSC_EVENT_KIND * E );

/***************************************************************************
 * Function : MSC_AllocateEvent
 *
 * Description: allocate memory, clean memory with zero and check if memory was allocated
 *
 * Returns: pointer to storage/NULL
 *
 * Notes: None
 *
 **************************************************************************/
    extern MSC_EVENT_KIND       *MSC_AllocateEvent( );

/***************************************************************************
 * Function : MSC_ZeroEventTime
 *
 * Description: Reset time definition
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          MSC_ZeroEventTime( MSC_EVENT_KIND * x );

/***************************************************************************
 * Function : MSC_AllocateEventType
 *
 * Description: Allocate memory for event and its data
 *
 * Returns: pointer to storage/NULL
 *
 * Notes: None
 *
 **************************************************************************/
    extern MSC_EVENT_KIND       *MSC_AllocateEventType( MSC_EVENT_KIND_TYPE z );

/***************************************************************************
 * Function : MSC_FreeEventData
 *
 * Description: free memory allocated for event's data
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          MSC_FreeEventData( MSC_EVENT_KIND * s );

/***************************************************************************
 * Function : MSC_FreeEventChain
 *
 * Description: 
 *
 * Returns: 
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          MSC_FreeEventChain( MSC_EVENT_KIND * s );

/***************************************************************************
 * Function : MSC_Time2Str
 *
 * Description: Convert double to time string
 *
 * Returns: d
 *
 * Notes: None
 *
 **************************************************************************/
    extern                  char * MSC_Time2Str( MSC_TIME_VALUE t, char * d );

/***************************************************************************
 * Function : MSC_Str2Dbl
 *
 * Description: Convert string to number
 *
 * Returns: 
 *
 * Notes: None
 *
 **************************************************************************/
    extern MSC_TIME_VALUE    MSC_Str2Dbl( const char * x );

/***************************************************************************
 * Function : MSC_Str2Int
 *
 * Description: Convert string to integer number
 *
 * Returns: 
 *
 * Notes: None
 *
 **************************************************************************/
    extern UINT32_T             MSC_Str2Int( const char * x );

/***************************************************************************
 * Function : MSC_IsMessageEvent
 *
 * Description: check wherever event is message
 *
 * Returns: RESULT_OK/RESULT_ERR
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          MSC_IsMessageEvent( MSC_EVENT_KIND * x );

/***************************************************************************
 * Function : MSC_RemoveTailEvents
 *
 * Description: remove unnecessary events from tree
 *
 * Returns: 
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          MSC_RemoveTailEvents( MSC_EVENT_KIND * s );

#ifdef __cplusplus
}
#endif                                 /* C++ */

#endif                                 /*MSCEVENT_H */
