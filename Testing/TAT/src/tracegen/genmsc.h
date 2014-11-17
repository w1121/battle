#ifndef MSC_GENMSC_H
#define MSC_GENMSC_H
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
* Functions:    MSCEXPN_SavePath
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  Save complete expanded traces
*
* Notes:        None
*
******************************************************************************/
#ifdef __cplusplus
extern                  "C"
{
#endif                                 /* C++ */

#include <portable.h>
#include <mscbase.h>
#include <partial.h>

/***************************************************************************
 * Function : MSCEXPN_SavePath
 *
 * Description:  build complete test paths
 *               M - pointer to loaded MSC,
 *               TM - pointer to MSC as a tree structure,
 *               start_P - traces to skip,
 *               max_P - maximal number of paths,
 *               gen_Flag - flag for producing MSC PR,
 *               time_Flag - 0 - standard MSC, 1 - MSC with time,
 *               list_File - file with list of generated files,
 *               outp_Dir - output directory *
 * Returns: RESULT_ERR/ RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          MSCEXPN_SavePath( MSC_STORAGE * M,
        MSC_EVENT_TREE * TM,
        const UINT32_T start_P,
        const UINT32_T max_P, const UINT32_T gen_Flag, const UINT32_T time_Flag, const char *list_File, char *outp_Dir );

#ifdef __cplusplus
}

#endif                                 /* C++ */

#endif                                 /* MSC_GENMSC_H */
