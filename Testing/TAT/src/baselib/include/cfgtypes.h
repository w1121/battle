#ifndef CFGTYPES_H
#define CFGTYPES_H
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
* Functions:    CFG_GetInstanceType
*               CFG_GetDataClassTypeStr
*               CFG_GetDataClassType
*               CFG_GetMacroDataTypeStr
*               CFG_GetSignalType
*               CFG_GetMacroType
*               CFG_GetMacroDataType
*               CFG_GetReplaceType
*               CFG_GetResearchType
*               CFG_CheckMacroData
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  String to Binary identificator converters
*
* Notes:        None
*
******************************************************************************/
#ifdef __cplusplus
extern                  "C"
{
#endif                                 /* C++ */

#define MSC_DEFAULT_PRIORITY 100
#define MSC_MAX_PRIORITY 255
#define MSC_MIN_PRIORITY 0

    typedef enum
    {
        MSC_INSTANCE_UNDEF = 0,
        MSC_INSTANCE_IUT = 1,
        MSC_INSTANCE_ENV = 2,
        MSC_INSTANCE_MONITOR = 3,
        MSC_INSTANCE_VMODEL = 4,
        MSC_INSTANCE_IGNORE = 5
    }
    MSC_INSTANCE_TYPE;                 /* definition of instance's type */

    typedef enum
    {
        MSC_SIG_UNDEFINED = 0,
        MSC_SIG_IN = 1,
        MSC_SIG_OUT = 2,
        MSC_SIG_IGNORE = 3,
        MSC_SIG_INOUT = 4
    }
    MSC_SIGNAL_TYPE;                   /* definition of signal's type */

    typedef enum
    {
        MSC_MACROTYPE_UNDEFINED = 0,   /* incorrect specification */
        MSC_MACROTYPE_LIST,            /* 'list' specification */
        MSC_MACROTYPE_FUNCTION,        /* 'function' specification  */
        MSC_MACROTYPE_DERIVE,          /* 'derive' specification */
        MSC_MACROTYPE_ABORT,           /* 'abort' specification (disable rule) */
        MSC_MACROTYPE_INDEX            /* 'index' specification */
    }
    MSC_MACRO_TYPE;

    typedef enum
    {
        MSC_MACRODATA_UNDEFINED = 0,   /* incorrect specification */
        MSC_MACRODATA_STRING,          /* macro has a string value */
        MSC_MACRODATA_NUMBER,          /* macro is decimal/hexadecimal number */
        MSC_MACRODATA_ID,              /* macro is user's identifier */
        MSC_MACRODATA_UNDEF            /* macro may have any value */
    }
    MSC_MACRO_DATA;

    typedef enum
    {
        MSC_REPL_UNDEFINED = 0,        /* incorrect data */
        MSC_REPL_REPLACE,              /* replace previous declaration */
        MSC_REPL_EXTEND,               /* extend previous declaration */
        MSC_REPL_SUBSET,               /* check to be subset of previous declaration */
        MSC_REPL_REMOVE                /* remove this declaration */
    }
    MSC_MACRO_REPL;

    typedef enum
    {
        MSC_RESEARCH_UNKNOWN = 0,
        MSC_RESEARCH_SEQUENTIAL,
        MSC_RESEARCH_RANDOM,
        MSC_RESEARCH_PRIORITY
    }
    MSC_RESEARCH_TYPE;

    typedef enum
    {
        MSC_DATA_UNKNOWN = 0,
        MSC_DATA_INTEGER,
        MSC_DATA_FLOAT,
        MSC_DATA_SYMBOL,
        MSC_DATA_STRING,
        MSC_DATA_IDENTIFIER
    }
    MSC_DATA_CLASS;

    /***************************************************************************
     * Function : CFG_GetInstanceType
     *
     * Description: extract instance type from string
     *
     * Returns: instance type
     *
     * Notes: None
     *
     **************************************************************************/
    extern MSC_INSTANCE_TYPE CFG_GetInstanceType( const char *s );

    /***************************************************************************
     * Function : CFG_GetSignalType
     *
     * Description: extract instance type from string
     *
     * Returns: instance type
     *
     * Notes: None
     *
     **************************************************************************/
    extern MSC_SIGNAL_TYPE  CFG_GetSignalType( const char *s );

    /***************************************************************************
     * Function : MSC_InstanceType2Str 
     *
     * Description: convert instance type to string
     *
     * Returns: 
     *
     * Notes: None
     *
     **************************************************************************/
    extern const char * MSC_InstanceType2Str( MSC_INSTANCE_TYPE x );

    /***************************************************************************
     * Function : CFG_GetSignalType
     *
     * Description: extract instance type from string
     *
     * Returns: instance type
     *
     * Notes: None
     *
     **************************************************************************/
    extern const char * MSC_SignalType2Str( MSC_SIGNAL_TYPE x );

    /***************************************************************************
     * Function : CFG_GetMacroType
     *
     * Description: extract macro type from string
     *
     * Returns: MSC_MACRO_TYPE
     *
     * Notes: None
     *
     **************************************************************************/
    extern MSC_MACRO_TYPE   CFG_GetMacroType( const char *s );

    /***************************************************************************
     * Function : CFG_GetMacroDataType
     *
     * Description: extract macro data type from string
     *
     * Returns: MSC_MACRO_DATA
     *
     * Notes: None
     *
     **************************************************************************/
    extern MSC_MACRO_DATA   CFG_GetMacroDataType( const char *s );

    /***************************************************************************
     * Function : CFG_GetReplaceType
     *
     * Description: extract data replace type from string
     *
     * Returns: MSC_MACRO_REPL
     *
     * Notes: None
     *
     **************************************************************************/
    extern MSC_MACRO_REPL   CFG_GetReplaceType( const char *s );

    /***************************************************************************
     * Function : CFG_GetResearchType
     *
     * Description: extract macro research type
     *
     * Returns: MSC_RESEARCH_TYPE
     *
     * Notes: None
     *
     **************************************************************************/
    extern MSC_RESEARCH_TYPE CFG_GetResearchType( const char *s );

    /***************************************************************************
     * Function : CFG_CheckMacroData
     *
     * Description: Check that data corresponds to container
     *
     * Returns: 
     *
     * Notes: None
     *
     **************************************************************************/
    extern LOGICAL          CFG_CheckMacroData( MSC_MACRO_DATA t, char *data );

    /***************************************************************************
     * Function : CFG_GetMacroDataTypeStr
     *
     * Description: extract macro data type from string
     *
     * Returns: MSC_MACRO_DATA
     *
     * Notes: None
     *
     **************************************************************************/
    extern const char * CFG_GetMacroDataTypeStr( MSC_MACRO_DATA tp );

    /***************************************************************************
    * Function : CFG_GetDataClassType
    *
    * Description: extract data class type
    *
    * Returns: MSC_DATA_CLASS
    *
    * Notes: None
    *
    **************************************************************************/
    extern MSC_DATA_CLASS   CFG_GetDataClassType( const char *s );

    /***************************************************************************
    * Function : CFG_GetDataClassTypeStr
    *
    * Description: string representation of data class
    *
    * Returns: string
    *
    * Notes: None
    *  
    **************************************************************************/
    extern const char * CFG_GetDataClassTypeStr( MSC_DATA_CLASS x );

#ifdef __cplusplus
}

#endif                                 /* C++ */

#endif                                 /* CFGTYPES_H */
