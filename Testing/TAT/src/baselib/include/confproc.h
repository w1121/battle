#ifndef MSC_CONF_PROC_H
#define MSC_CONF_PROC_H
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
* Functions:    CFG_SetDefaultConfig
*               CFG_GetInstanceID
*               CFG_GetSignalID
*               CFG_Check_MSC
*               CFG_GetDataTypeID
*               CFG_GetMacroID
*               CFG_GetMacroDT
*               CFG_GetMacroTypeInd
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  TAT configuration processing routines
*
* Notes:        None
*
******************************************************************************/
#include <xmlconf.h>

#ifdef __cplusplus
extern                  "C"
{
#endif                                 /* C++ */

    /***************************************************************************
     * Function : CFG_SetDefaultConfig
     *
     * Description: Set default configuration
     *
     * Returns: RESULT_ERR/RESULT_OK
     *
     * Notes: None
     *
     **************************************************************************/
    extern LOGICAL          CFG_SetDefaultConfig( TAT_XML_CONFIG_DEF * d );

    /***************************************************************************
     * Function : MSC_GetInstanceID
     *
     * Description: Get Instance ID from instance declarations
     *
     * Returns: ID/-1/-2 if found, -3 if not found
     *
     * Notes: None
     *
     **************************************************************************/
    extern int              CFG_GetInstanceID( const char * name );

    /***************************************************************************
     * Function : CFG_GetSignalID
     *
     * Description: retrieve signal ID by signal name
     *
     * Returns: -1/number
     *
     * Notes: None
     *
     **************************************************************************/
    extern int              CFG_GetSignalID( const char * signal );

#define CFG_MATCH_DATATYPE          0x00000001
#define CFG_MATCH_INSTANCE          0x00000002
#define CFG_MATCH_MESSAGE           0x00000004
#define CFG_MATCH_MESSAGE_PREFIX    0x00000008
#define CFG_MATCH_CREATE            0x00000010
#define CFG_MATCH_MESSAGE_PARAMS    0x00000020

    /***************************************************************************
     * Function : CFG_Check_MSC
     *
     * Description: Check MSC against default configuration
     *
     * Returns: RESULT_ERR/RESULT_OK
     *
     * Notes: None
     *
    **************************************************************************/
    extern LOGICAL          CFG_Check_MSC( MSC_STORAGE * M, int flags );

    /***************************************************************************
     * Function : CFG_GetDataTypeID
     *
     * Description: retrieve data type ID by name
     *
     * Returns: -1/number
     *
     * Notes: None
     *
     **************************************************************************/
    extern int              CFG_GetDataTypeID( const char * data );

    /***************************************************************************
     * Function : CFG_GetMacroID
     *
     * Description: retrieve macro ID by name
     *
     * Returns: -1/number
     *
     * Notes: None
     *
     **************************************************************************/
    extern int              CFG_GetMacroID( const char * data );

    /***************************************************************************
     * Function : MSC_GetMacroDT
     *
     * Description: retrieve data type ID by name
     *
     * Returns: MSC_MACRODATA_UNDEFINED/type
     *
     * Notes: None
     *
     **************************************************************************/
    extern MSC_MACRO_DATA   CFG_GetMacroDT( const char * data );

    /***************************************************************************
     * Function : MSC_GetMacroTypeInd
     *
     * Description: retrieve macro type by index
     *
     * Returns: type
     *
     * Notes: None
     *
     **************************************************************************/
    extern MSC_MACRO_TYPE   CFG_GetMacroTypeInd( const size_t i );

#ifdef __cplusplus
}

#endif                                 /* C++ */

#endif                                 /* MSC_CONF_PROC_H */
