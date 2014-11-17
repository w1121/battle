#ifndef MSCGENERATE_H
#define MSCGENERATE_H
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
* Functions:    MSC_GenerateLine
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  MSC Events ->string  conversion routines
*
* Notes:        None
*
******************************************************************************/
#include <mscbase.h>

#ifdef __cplusplus
extern                  "C"
{
#endif                                 /* C++ */

/* flags for producing MSC strings */
/* Msc generation flags */
#define MSC_MPR_OUTPUT_TIME             0x00000001
#define MSC_MPR_EXCLUDE_STIMER          0x00000002
#define MSC_MPR_OUTPUT_DURATION         0x00000004
#define MSC_MPR_OUTPUT_SYNC             0x00000008
#define MSC_MPR_OUTPUT_EVCOND           0x00000010
#define MSC_MPR_OUTPUT_REF_PAR          0x00000020
#define MSC_MPR_CORRECT_NAME            0x00000040
#define MSC_MPR_OUTPUT_CALCTIME         0x00000100
#define MSC_MPR_OUTPUT_DIAG             0x00000200
#define MSC_MPR_OUTPUT_ALL_EVENTS       0x00000400
#define MSC_MPR_OUTPUT_SREF             0x00000800
#define MSC_MPR_OUTPUT_VRSTIME          0x00001000
#define MSC_MPR_OUTPUT_VERDICT          0x00002000
#define MSC_MPR_OUTPUT_EVMARK           0x00004000
#define MSC_MPR_OUTPUT_EVCOMMENT        0x00008000


#ifndef TAT_NSOURCE_REF
/* all - MSC_OUTPUT_2000 */
#define MSC_MPR_VERBOSE_OUT             0x00004c7d
#define MSC_MPR_STD_OUT                 0x00004842
#define MSC_MPR_TIMED_OUT               0x0000487d

#else
#define MSC_MPR_VERBOSE_OUT             0x0000047d
#define MSC_MPR_STD_OUT                 0x00000042
#define MSC_MPR_TIMED_OUT               0x0000007d
#endif

    /* specify verdict to output in MPR */
    extern int              MSC_VerdictTrace;

    /***************************************************************************
     * Function : MSC_GenerateLine
     *
     * Description: generate MSC-PR line from event specification
     *
     * Returns: dest/NULL if error occured
     *
     * Notes: None
     *
     **************************************************************************/
    extern char *          MSC_GenerateLine( char * dest, MSC_EVENT_KIND * E, MSC_STORAGE * M, const UINT32_T flags );

    /***************************************************************************
     * Function : MSC_Save
     *
     * Description: store MSC data in file in MSC-PR format
     *
     * Returns: RESULT_ERR/RESULT_OK
     *
     * Notes: None
     *
     **************************************************************************/
    extern LOGICAL          MSC_Save( const char * filename, MSC_STORAGE * M, const UINT32_T flags );

    extern LOGICAL          MSC_GenStartOp( char * dest, MSC_EVENT_KIND * E, MSC_STORAGE * M, const UINT32_T flags );

    extern LOGICAL          MSC_GenEndOp( char * dest, MSC_EVENT_KIND * E, MSC_STORAGE * M, const UINT32_T flags );

    extern LOGICAL          MSC_GenSepOp( char * dest, MSC_EVENT_KIND * E, MSC_STORAGE * M, const UINT32_T flags );

    /***************************************************************************
     * Function : MSC_GenComment
     *
     * Description: generate comment string from event specification
     *
     * Returns: dest/NULL if error occured
     *
     * Notes: None
     *
     **************************************************************************/
    extern char *          MSC_GenComment( char * dest, MSC_EVENT_KIND * event, MSC_STORAGE * M, const UINT32_T flags );

#ifdef __cplusplus
}

#endif                                 /* C++ */
#endif                                 /* MSCGENERATE_H */
