#ifndef MSCBASE_H
#define MSCBASE_H
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
* Description:  MSC Storage handling
*
* Notes:        None
*
******************************************************************************/
#include <mscevent.h>
#include <limits.h>
#include <strcoll.h>
#include <cfgtypes.h>

#ifdef __cplusplus
extern                  "C"
{
#endif                                 /* C++ */

#define MAX_LOOP_PARAMETERS 2

#define MSC_MACRO_START '#'

#ifndef MSC_REPEAT_ACTION
#define MSC_REPEAT_ACTION (int)2
#endif

#ifndef MSC_REMOVE_ACTION
#define MSC_REMOVE_ACTION (int)2
#endif

#ifndef MSC_NOT_END
#define MSC_NOT_END (int)3
#endif

#ifndef MSC_NOT_FOUND
#define MSC_NOT_FOUND (int)-1
#endif


/* message source/destination type */
#define MSC_LOST_INSTANCE_ID (-1)
#define MSC_FOUND_INSTANCE_ID (-2)
#define MSC_UNKNOWN_INSTANCE_ID (-3)


/* define MSC_TestEvent flags */
#define MSC_CHECK_INSYS        0x00000001
#define MSC_CHECK_INENV        0x00000002
#define MSC_CHECK_ENVSELF      0x00000004
#define MSC_CHECK_DEL_COND     0x00000008
#define MSC_CHECK_SAVESR       0x00000010
#define MSC_CHECK_DEL_ALL_COND 0x00000020
#define MSC_CHECK_DEL_TIMERS   0x00000040
       typedef enum _MSC_VRS_MSC_STATUS
    {
        MSC_VRS_CONSISTENT,
        MSC_VRS_TIME_INCONSISTENCY,
        MSC_VRS_ANNO_INCONSISTENCY,
        MSC_VRS_ANNO_CORRECTNESS
    }
    MSC_VRS_MSC_STATUS;

    typedef struct _MSC_VRS_MSC_TRACES
    {
        size_t                  trace;
        MSC_VRS_MSC_STATUS      status;
    }
    MSC_VRS_MSC_TRACES;


    typedef struct _MSC_INSTANCE_STORAGE
    {
        STRING_ID              name;       /* instance name */
        STRING_ID              type;       /* additional parameters (standard field) */
        STRING_ID              ratio;      /* time ratio (min value) */
        STRING_ID              ratio_max;  /* time ratio (max value) */
        STRING_ID              decompose;  /* instance decomposition */
        STRING_ID              comment;    /* instance comments */
        MSC_INSTANCE_TYPE       ind;        /* instance kind */
        int                     priority;   /* instance priority */
        size_t                  line_number;/* line number in source MSC file */
    }
    MSC_INSTANCE_STORAGE;

    typedef struct _MSC_EVENT_KIND_LABEL
    {
        STRING_ID                   label; /* label name */
        MSC_EVENT_KIND              *s;     /* related event */
        UINT32_T                    unused; /* if 1 - label is unused and may be freely deleted */
        UINT32_T                    generated;  /* if 1 - label was automatically generated */
    }
    MSC_EVENT_KIND_LABEL;

    typedef struct _MSC_SOURCE_REFERENCE
    {
        STRING_ID              link;  /* link to file */
        size_t                  level; /* source_level */
        size_t                  first_Line; /* first active line */
        size_t                  last_Line;  /* last active line */
    }
    MSC_SOURCE_REFERENCE;

    typedef enum _MSC_PARAMS_TYPE
    {
        TAT_PARAMS,
        TTCN2_PARAMS,
        TTCN3_PARAMS
    } MSC_PARAMS_TYPE;

    typedef struct _MSC_VAR_PARAMS
    {
        STRING_ID             pv_Name;      /* MSC'2000 in current: text 'params  '; */
        STRING_ID             pv_Type;      /* data type */
        STRING_ID             pv_TypeSrc;   /* back-up of source data type */
        size_t                line_number;  /* line number in source MSC file */
    } MSC_VAR_PARAMS;

    struct _MSC_STRUCTURE
    {
        STRING_ID              msc_File;   /* optional: name of file with MSC */
        STRING_ID              msc_Document;   /* form 'mscdocument' tag */
        STRING_ID              msc_Name;   /* from 'msc' tag */
        size_t                  instance_Count; /* number of instances */

        MSC_INSTANCE_STORAGE   *instances;

        MSC_EVENT_KIND              *start; /* first event reference */

        size_t                  message_Count;  /* allocated dynamically */
        MSC_MESSAGE_STORAGE    *messages;   /* parameters of messages */
        size_t                  message_Number; /* last number of message */

        STRING_COLL           *str;   /* string collection */

        size_t                  last_TimerNum;  /* last timer number */

        STRING_ID              time_Offset;    /* time offset MSC'2000 */
        STRING_ID              time_Scale;      /* time scale */

        size_t                  label_Count;    /* event labels */
        MSC_EVENT_KIND_LABEL        *labels;

        size_t                  var_Count;      /* MSC'2000 variables */
        MSC_VAR_PARAMS         *vars;


        size_t                  param_Count;    /* MSC'2000 parameters */
        MSC_VAR_PARAMS         *params;
        MSC_PARAMS_TYPE         params_Type;

        size_t                  text_Count;     /* number of TEXT statements */
        STRING_ID              *texts;          /* TEXT statements */

        size_t                  source_Format;
        size_t                  src_Count;
        MSC_SOURCE_REFERENCE   *src_Files;
        size_t                  src_MscLevel;

        size_t                  trace_Count;    /* number of VRS traces in MSC */
        MSC_VRS_MSC_TRACES     *traces;         /* VRS trace defintions  */
    };

    typedef struct _MSC_STRUCTURE MSC_STORAGE;

/* function will be called for each EVENT */
    typedef                 LOGICAL( *MSC_EVENT_KIND_PROCESS_FUNC ) ( MSC_STORAGE *, MSC_EVENT_KIND * );

    typedef int             ( *MSC_PREVNEXT_PROCESS_FUNC ) ( MSC_STORAGE *,
        MSC_EVENT_KIND **, MSC_EVENT_KIND *, int );

/***************************************************************************
 * Function : MSC_Load
 *
 * Description: High-Level function for MSC-PR loading
 *
 * Return: RESULT_ERR/RESULT_OK
 *
 * Notes:
 *
 **************************************************************************/
    extern LOGICAL          MSC_Load( const char * filename, MSC_STORAGE * MSC, const UINT32_T flags);


/***************************************************************************
 * Function : MSC_GetString
 *
 * Description: returns pointer to string
 *
 * Returns: pointer to string / NULL
 *
 * Notes:
 *
**************************************************************************/
    extern char            *MSC_GetString( const STRING_ID x, const MSC_STORAGE * M );

/***************************************************************************
 * Function : MSC_GetStr
 *
 * Description: returns pointer to string
 *
 * Returns: pointer to string / Zero_String
 *
 * Notes:
 *
**************************************************************************/
    extern char            *MSC_GetStr( const STRING_ID s, const MSC_STORAGE * M );

/***************************************************************************
 * Function : MSC_AddStr
 *
 * Description: allocate memory in string collection of MSC
 *
 * Returns: index of string
 *
 * Notes:
 *
**************************************************************************/
    extern STRING_ID       MSC_AddStr( const char * s, MSC_STORAGE * M );

/***************************************************************************
 * Function : MSC_GetTimeLabelID
 *
 * Description: look for time label definition
 *
 * Returns: return positive number if found, -1 if not
 *
 * Notes: None
 *
 **************************************************************************/
    extern int              MSC_GetTimeLabelID( const char * label, const MSC_STORAGE * M );

/***************************************************************************
 * Function : MSC_GetInstanceID
 *
 * Description: return MSC instance identifier
 *
 * Returns: MSC_UNKNOWN_INSTANCE_ID/MSC_LOST_INSTANCE_ID/MSC_FOUND_INSTANCE_ID
 *          or instance index in M
 *
 * Notes: None
 *
 **************************************************************************/
    extern int              MSC_GetInstanceID( const char * instance, const MSC_STORAGE * M );

/***************************************************************************
 * Function : MSC_GetLinkID
 *
 * Description: returns ID of source reference
 *
 * Returns: number
 *
 * Notes: None
 *
 **************************************************************************/
    extern int              MSC_GetLinkID( const char * link, const MSC_STORAGE * M );

/***************************************************************************
 * Function : MSC_GetInstanceName
 *
 * Description: get instance name by number
 *
 * Returns: pointer to instance name
 *
 * Notes: None
 *
 **************************************************************************/
    extern char            *MSC_GetInstanceName( int inst, MSC_STORAGE * M );

/***************************************************************************
 * Function : MSC_GetMessageParams
 *
 * Description: get message params by number
 *
 * Returns: pointer to data structure
 *
 * Notes: None
 *
 **************************************************************************/
    extern MSC_MESSAGE_STORAGE *MSC_GetMessageParams( const size_t ind, const MSC_STORAGE * M );

/***************************************************************************
 * Function : MSC_AddMessage
 *
 * Description: Add message ID to MSC storage
 *
 * Returns: 
 *
 * Notes: None
 *
 **************************************************************************/
    extern MSC_MESSAGE_STORAGE *MSC_AddMessage( MSC_MESSAGE_STORAGE * S, MSC_STORAGE * M );

/***************************************************************************
 * Function : MSC_CopyMessage
 *
 * Description: Copy message from SRC to M
 *
 * Returns: new message ID
 *
 * Notes: None
 *
 **************************************************************************/
    extern size_t   MSC_CopyMessage( size_t ind, MSC_STORAGE * M, MSC_STORAGE * SRC );


/***************************************************************************
 * Function : MSC_FreeMemory
 *
 * Description: clean allocated memory
 *
 * Return: none
 *
 * Notes:
 *
 **************************************************************************/
    extern LOGICAL          MSC_FreeMemory( MSC_STORAGE * M );


/***************************************************************************
 * Function : MSC_Allocate
 *
 * Description: Initialize MSC storage structure
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          MSC_Allocate( char *filename, MSC_STORAGE * M );

/***************************************************************************
 * Function : MSC_TestEvent
 *
 * Description: check event for existence criteria against flags
 *
 * Returns: RESULT_ERR/RESULT_OK/MSC_REMOVE_ACTION
 *
 * Notes: None
 *
 **************************************************************************/
    extern int              MSC_TestEvent( const MSC_STORAGE * M, MSC_EVENT_KIND * E, const UINT32_T flags );

/***************************************************************************
 * Function : MSC_CompareEvents
 *
 * Description: returns true if events are semantically equal
 *
 * Returns: RESULT_OK/RESULT_ERR
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          MSC_CompareEvents( const MSC_STORAGE * M, MSC_EVENT_KIND * E1, MSC_EVENT_KIND * E2 );

/***************************************************************************
 * Function : MSC_InsertEvent
 *
 * Description: Insert x before s so that memory allocated by s is filled with x 
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          MSC_InsertEvent( MSC_STORAGE * M, MSC_EVENT_KIND * s, MSC_EVENT_KIND * x );

/***************************************************************************
 * Function : MSC_GlobalCond
 *
 * Description: generate global condition for MSC
 *
 * Returns: pointer to event's structure
 *
 * Notes: None
 *
 **************************************************************************/
    extern MSC_EVENT_KIND       *MSC_GlobalCond( MSC_STORAGE * M );

/***************************************************************************
 * Function : MSC_TimeLabelEvent
 *
 * Description: Mark event with label
 *
 * Returns: label ID
 *
 * Notes: None
 *
 **************************************************************************/
    extern size_t              MSC_TimeLabelEvent( MSC_STORAGE * M, MSC_EVENT_KIND * s, char *lab );

/***************************************************************************
 * Function : MSC_SortMess
 *
 * Description: Sort message by incrementing of indicator
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          MSC_SortMess( MSC_STORAGE * M );

/***************************************************************************
 * Function : MSC_ExpandInstance
 *
 * Description: Expand event's instances to all objects
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          MSC_ExpandInstance( MSC_EVENT_KIND * x, MSC_STORAGE * M );

/***************************************************************************
 * Function : TAT_Load
 *
 * Description: Load FlowGen/MPR based on extension, allocate memory if MSC==NULL
 *
 * Returns: pointer to data storage/NULL if error
 *
 * Notes: None
 *
 **************************************************************************/
    extern MSC_STORAGE * TAT_Load( const char * name, MSC_STORAGE * MSC, const UINT32_T flags );

/***************************************************************************
 * Function : TAT_Save
 *
 * Description: save MSC on disk in format,specified by source file
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          TAT_Save( const char * name, MSC_STORAGE * M, const UINT32_T flags );


/***************************************************************************
 * Function : MSC_HugeMul
 *
 * Description: multiply with overflow checking
 *
 * Returns: min((i*j), MSC_INFINITY_CONST)
 *
 * Notes: None
 *
 **************************************************************************/
    extern size_t             MSC_HugeMul( size_t i, size_t j );

/***************************************************************************
 * Function : MSC_ConvertString
 *
 * Description: Convert MSC string
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          MSC_ConvertString( STRING_ID * x,
        const MSC_STORAGE * MSC, STRCOLL_PROCESS_FUNC conv );

    typedef LOGICAL (* MSC_SEQ_FILE_PROC)(const char * name, MSC_STORAGE * data, const UINT32_T flags);

    extern size_t MSC_RegisterFormat(const char * ext, MSC_SEQ_FILE_PROC readf, MSC_SEQ_FILE_PROC writef);

    /* predefined IDs */
    extern size_t MSC_MprFormatID;
    extern size_t MSC_FGFormatID;

#ifdef __cplusplus
}

#endif                                 /* C++ */

#endif                                 /*MSCBASE_H */
