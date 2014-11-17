#ifndef MSCPARSE_H
#define MSCPARSE_H
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
* Description:  MSC Events parsers
*
* Notes:        None
*
******************************************************************************/
#include <mscbase.h>

#ifdef __cplusplus
extern                  "C"
{
#endif                                 /* C++ */

    typedef struct _MSC_INSTANCE_STATE
    {
        UINT32_T                    instance_Declared:1;    /* flag on instance keyword */
        UINT32_T                    instance_Ended:1;   /* flag on endinstance keyword */
        UINT32_T                    instance_Created:1; /* flag on create keyword */
        UINT32_T                    instance_Stopped:1; /* flag on stop keyword */
        UINT32_T                    instance_Used:1;    /* flag on instance usage */
        size_t                  branch_ID; /* branch ID where it was created */
    }
    MSC_INSTANCE_STATE;
    
    /* internal static counters */
    typedef struct _MSC_LOAD_DATA
    {
        size_t                  line_Counter;
        int                     msc_Started;
        int                     mscdocument_Started;
        int                     msc_Ended;
        size_t                  branch_ID;
        size_t                  last_ID;
        
        size_t                  timer_Counter;
        size_t                  uniq_TimerID;
        
        struct
        {
            size_t                  timer_Index;
            MSC_TIMER_TYPE          timer_Type;
            MSC_EVENT_KIND              *event_Pointer;
        }
        *timers;
        
        int                    *coregion_State; /* coregion balance */
        
        MSC_INSTANCE_STATE *instance_State; /* instance creation/stop balance */
        
        size_t                  loop_NestLevel;
        
        size_t                  signal_Counter; /* number of signals */
        size_t                  open_Signals;   /* number of unclosed in/out pairs */
        struct
        {
            char                   *message_Label;  /* original message label */
            MSC_TIME_INTERVAL  time;           /* time */
            STRING_ID              cond;           /* condition string */
            int                     count;          /*  pairing */
            size_t                  line_Number;    /* line number of first event */
            MSC_EVENT_KIND_SYNC_TYPE     tat_Sync;       /* event synchronization flag */
            size_t                  inst1, inst2;   /* src,dest instances */
        }
        *signal_Storage;
        
        size_t                     variable_Counter;
        struct
        {
            char                   *var_Name;
            size_t                  line_Number;
        }
        *var_Storage;
        
        /* *************************************** */
        char                   *source_Buffer;
        char                   *current_Position;
        LOGICAL                 error_State;
        
        /* handling label definitions */
        size_t                  nest_Level;
        size_t                 *nest_Path;
        
        size_t                  msc_LabelCount;
        
        struct
        {
            size_t                  path_Length;
            size_t                 *path;
        }
        *msc_LabelDeclPath;
        
    }
    MSC_LOAD_DATA;
    
/***************************************************************************
 * Function : MSC_ParseInstances
 *
 * Description: extract instance names delimited with commas and put their
 *              numbers int the array (x).
 *              if 'all' specified, only active instances added x
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL MSC_ParseInstances( char * s, size_t *icount, MSC_INSTANCE_ID_TYPE ** x,
        MSC_STORAGE * M, MSC_LOAD_DATA * data );

    /***************************************************************************
     * Function : MSC_ParseParams
     *
     * Description: extract complex parameters from p. Allocate storage and save them
     *              set pointer after last character
     *
     * Returns: RESULT_ERR/RESULT_OK, number of parsed parameters in pcount
     *
     * Notes: None
     *
     **************************************************************************/
    extern LOGICAL MSC_ParseParams( char * *p, size_t *pcount, char * **P );

    extern LOGICAL          MSC_ParseCOND( char **line, MSC_STORAGE * M, MSC_EVENT_KIND * S );
    extern LOGICAL          MSC_ParseTime( char **d, MSC_STORAGE * M, MSC_TIME_INTERVAL * T );
    extern LOGICAL          MSC_ParseEventSpec( char **line, MSC_STORAGE * M, MSC_EVENT_KIND * s );
    extern LOGICAL          MSC_StripComment( char *p, MSC_STORAGE * M, MSC_EVENT_KIND * m );
    extern LOGICAL          MSC_ParseTimer( char *line, MSC_EVENT_KIND * s, MSC_STORAGE * M );
    extern LOGICAL          MSC_ParseAction( char *line, MSC_EVENT_KIND * s, MSC_STORAGE * M );
    extern LOGICAL          MSC_ParseCondition( char *line, MSC_EVENT_KIND * s, MSC_STORAGE * M, MSC_LOAD_DATA *data );
    extern LOGICAL          MSC_ParseReference( char *line, MSC_EVENT_KIND * event, MSC_STORAGE * M );
    extern LOGICAL          MSC_ParseCreate( char *line, MSC_EVENT_KIND * s, MSC_STORAGE * M );

    extern int              MSC_CheckOperator( char *line, MSC_OPERATOR_TYPE Curr );
    extern MSC_EVENT_KIND_TYPE   MSC_GetEventType( char **p, MSC_STORAGE * M, MSC_EVENT_KIND * st );
    extern LOGICAL          MSC_CheckName( const char * s );

    extern LOGICAL          MSC_ParseSourceRef( char *line, MSC_EVENT_KIND * s, MSC_STORAGE * M );

/***************************************************************************
 * Function : MSC_CheckEventSpecification
 *
 * Description: check that all necessary fields are filled with correct
 *              information.
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
extern LOGICAL MSC_CheckEventSpecification( MSC_EVENT_KIND *event, MSC_STORAGE *M, MSC_LOAD_DATA * data);

/* set this variable, if you don't want 'text' statements to be parsed */
extern int PTK_DoNotParseText;

#ifdef __cplusplus
}

#endif                                 /* C++ */

#endif    /* MSCPARSE_H */
