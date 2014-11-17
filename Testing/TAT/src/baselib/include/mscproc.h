#ifndef MSCPROCESS_H
#define MSCPROCESS_H
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
* Description:  MSC processing functions
*
* Notes:        None
*
******************************************************************************/
#include <mscbase.h>

#ifdef __cplusplus
extern                  "C"
{
#endif                                 /* C++ */

/* MSC Transform flags */
#define MSC_TRANS_USE_COND              0x00000001
#define MSC_TRANS_USE_SYNC              0x00000002
#define MSC_TRANS_OPER_SYNC             0x00000004
#define MSC_TRANS_REMOVE_ZERO_LOOP      0x00000008
#define MSC_TRANS_INLINE_LOOP           0x00000010
#define MSC_TRANS_GLOBAL_SYNC           0x00000020
#define MSC_TRANS_UNROLL_LOOP           0x00000040
#define MSC_TRANS_TIMER_SYNC            0x00000080
#define MSC_TRANS_ACTION_SYNC           0x00000100
#define MSC_TRANS_EXPN_CONDITION        0x00000200
#define MSC_TRANS_FULL_SYNC             0x00000400
#define MSC_TRANS_REM_GEN_CND           0x00000800
#define MSC_TRANS_MOVE_LAB_TM           0x00001000
#define MSC_TRANS_MOVE_LAB_CS           0x00002000
#define MSC_TRANS_REM_FR_COND           0x00004000
#define MSC_TRANS_REM_FR_TMR            0x00008000
#define MSC_TRANS_SRO                   0x00010000

/***************************************************************************
 * Function : MSC_Copy
 *
 * Description: Duplicate MSC
 *
 * Returns: pointer to new MSC
 *
 * Notes: None
 *
 **************************************************************************/
    extern MSC_STORAGE *MSC_Copy( const MSC_STORAGE * s );

/***************************************************************************
 * Function : MSC_Process
 *
 * Description: Call user's function for each event
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          MSC_Process( MSC_STORAGE * M, MSC_EVENT_KIND_PROCESS_FUNC user_func );

/***************************************************************************
 * Function : MSC_FindEndEventChain
 *
 * Description: find last event in chain
 *
 * Returns: pointer to event
 *
 * Notes: None
 *
 **************************************************************************/
    extern MSC_EVENT_KIND       *MSC_FindEndEventChain( MSC_EVENT_KIND * s );

/***************************************************************************
 * Function : MSC_PR_Transform
 *
 * Description: tree traversal on MSC, calls user's function for each
 *              pair of events
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          MSC_PR_Transform( MSC_STORAGE * M, MSC_PREVNEXT_PROCESS_FUNC f, int flags );

/***************************************************************************
 * Function : MSC_Transform
 *
 * Description: Conduct MSC processing, specified by flags
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          MSC_Transform( MSC_STORAGE * M, int flags );

/***************************************************************************
 * Function : MSC_Include
 *
 * Description: Replace event with content of another MSC
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          MSC_Include( MSC_STORAGE * D, MSC_STORAGE * S, MSC_EVENT_KIND * s, int Sync );

/***************************************************************************
 * Function : MSC_GetLastSourceLine
 *
 * Description: Get last line with event from MSC
 *
 * Returns: number
 *
 * Notes: None
 *
 **************************************************************************/
    extern size_t           MSC_GetLastSourceLine( MSC_STORAGE * M );

#ifdef __cplusplus
}

#endif                                 /* C++ */
#endif                                 /* MSCPROCESS_H */
