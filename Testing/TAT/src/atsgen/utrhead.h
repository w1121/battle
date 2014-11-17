#ifndef UTRHEAD_H
#define UTRHEAD_H
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
* Description:  Output header for Tcl-ATS
*
* Notes:        None
*
******************************************************************************/
#include <mscbase.h>
#include <partial.h>
#include <xmlconf.h>

#ifdef __cplusplus
extern                  "C"
{
#endif                                 /* C++ */

    /***************************************************************************
     * Function : MSC_AtsTCL_StartTestSuite
     *
     * Description: Tcl start test suite header
     *
     * Returns: RESULT_OK, RESULT_ERR
     *
     * Notes: None
     *
     **************************************************************************/
    extern LOGICAL          MSC_AtsTCL_StartTestSuite( FILE * Out,
        int argc,
        char *argv[],
        char *configuration,
        char *suitename,
        size_t msccount,
        char **mscnames,
        size_t incl_count,
        char **includenames,
        TAT_XML_CONFIG_DEF * conf,
        char *prefix );

    /***************************************************************************
     * Function : MSC_AtsTCL_FinishTestSuite
     *
     * Description: Tcl start test suite header
     *
     * Returns: RESULT_OK, RESULT_ERR
     *
     * Notes: None
     *
     **************************************************************************/
    extern LOGICAL          MSC_AtsTCL_FinishTestSuite( FILE * Out,
        int argc,
        char *argv[],
        char *configuration,
        char *suitename,
        size_t msccount,
        char **mscnames,
        size_t incl_count,
        char **includenames,
        TAT_XML_CONFIG_DEF * conf,
        char *prefix );

    /***************************************************************************
     * Function : MSC_AtsOutTimer
     *
     * Description: output Timer specification
     *
     * Returns: 
     *
     * Notes: None
     *
     **************************************************************************/
    extern int              MSC_AtsOutTimer( FILE * Out, MSC_STORAGE * M, MSC_TIMER_TIME time_Calc );

    /***************************************************************************
     * Function : MSC_AtsOutTime
     *
     * Description: output event time 
     *
     * Returns: RESULT_ERR/RESULT_OK
     *
     * Notes: None
     *
     **************************************************************************/
    extern LOGICAL          MSC_AtsOutTime( FILE * Out, MSC_STORAGE * M, MSC_TIME_INTERVAL time_Calc );

    /***************************************************************************
     * Function : MSC_AtsGenKeyword
     *
     * Description: generate event type
     *
     * Returns: pointer to string
     *
     * Notes: None
     *
     **************************************************************************/
    extern char            *MSC_AtsGenKeyword( MSC_EVENT_KIND * x );

    /***************************************************************************
     * Function : MSC_AtsGenTime
     *
     * Description: Generate event's time
     *
     * Returns: None
     *
     * Notes: None
     *
     **************************************************************************/
    extern void             MSC_AtsGenTime( FILE * out, MSC_STORAGE * M, MSC_EVENT_KIND * x );

    /***************************************************************************
     * Function : MSC_AtsGenInstances
     *
     * Description: Generate list of instances
     *
     * Returns: None
     *
     * Notes: None
     *
     **************************************************************************/
    extern void             MSC_AtsGenInstances( FILE * out, MSC_STORAGE * M, MSC_EVENT_KIND * x );

    /***************************************************************************
     * Function : MSC_AtsGenData
     *
     * Description: Generate event's data
     *
     * Returns: None
     *
     * Notes: None
     *
     **************************************************************************/
    extern void             MSC_AtsGenData( FILE * out, MSC_STORAGE * M, MSC_EVENT_KIND * x );

    /***************************************************************************
     * Function : MSC_AtsGenerateCommand
     *
     * Description: Generate full command.
     *              <command>::=list <LBRASE><keyword><RBRASE>
     *                         <time><cond><instances_applied><line><comment><data> ;
     * Returns: None
     *
     * Notes: None
     *
     **************************************************************************/
    extern void             MSC_AtsGenerateCommand( MSC_STORAGE * M, MSC_EVENT_KIND * x, FILE * out );

    /***************************************************************************
     * Function : MSC_AtsGenerateSigDef
     *
     * Description: Output string table of messages in MSC(s)
     *
     * Returns: RESULT_ERR/RESULT_OK
     *
     * Notes: None
     *
     **************************************************************************/
    extern LOGICAL          MSC_AtsGenerateSigDef( FILE * Out );

    /***************************************************************************
     * Function : MSC_AtsGenTestCaseHeader
     *
     * Description: Output test case header
     *
     * Returns: RESULT_ERR/RESULT_OK
     *
     * Notes: None
     *
     **************************************************************************/

    extern LOGICAL          MSC_AtsGenTestCaseHeader( FILE * Out,
        MSC_STORAGE * M, struct MSC_TEST_CASE *X, size_t br_count, char *prefix, int mode );

    /***************************************************************************
     * Function : MSC_AtsGenTestCaseFooter
     *
     * Description: Generate test case footer
     *
     * Returns: RESULT_ERR/RESULT_OK
     *
     * Notes: None
     *
     **************************************************************************/
    extern LOGICAL          MSC_AtsGenTestCaseFooter( FILE * Out, MSC_STORAGE * M, char *prefix );

    /***************************************************************************
     * Function : MSC_AtsGenTest
     *
     * Description: Generate active test case
     *
     * Returns: RESULT_ERR/RESULT_OK
     *
     * Notes: None
     *
     **************************************************************************/

    extern LOGICAL          MSC_AtsGenTest( FILE * Out,
        MSC_STORAGE * M, struct MSC_TEST_CASE *X, char *prefix );

    /***************************************************************************
     * Function : MSC_AtsGenTestReduced
     *
     * Description: Generate flat test case
     *
     * Returns: RESULT_ERR/RESULT_OK
     *
     * Notes: None
     *
     **************************************************************************/
    extern LOGICAL          MSC_AtsGenTestReduced( FILE * Out, MSC_STORAGE * M, char *prefix );

    typedef struct 
    {
        MSC_EVENT_KIND *event;
        size_t    id;
        size_t    instance_id;
    } MSC_ATS_EVENT_LIST;

    extern size_t MSC_GetEventID(MSC_EVENT_KIND *ev);

#ifdef __cplusplus
}

#endif                                 /* C++ */

#endif                                 /* UTRHEAD_H */
