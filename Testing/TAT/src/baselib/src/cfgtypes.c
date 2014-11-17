/******************************************************************************
*
*       COPYRIGHT 2001-2002 MOTOROLA, ALL RIGHTS RESERVED
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
#include <portable.h>
#include <mscbase.h>
#include <mscstrs.h>
#include <cfgtypes.h>
#include <strproc.h>
#include <tatgdata.h>

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
MSC_INSTANCE_TYPE CFG_GetInstanceType( const char *s )
{
    if ( !s )
        return MSC_INSTANCE_UNDEF;

    if ( !stricmp( s, STR_ENV ) )
        return MSC_INSTANCE_ENV;

    if ( !stricmp( s, STR_MODEL ) || !stricmp( s, STR_IUT ) )
        return MSC_INSTANCE_IUT;

    if ( !stricmp( s, STR_MONITOR ) )
        return MSC_INSTANCE_MONITOR;

    if ( !stricmp( s, STR_VMODEL ) )
        return MSC_INSTANCE_VMODEL;

    if ( !stricmp( s, STR_IGNORE ) )
        return MSC_INSTANCE_IGNORE;

    return MSC_INSTANCE_UNDEF;
}

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
MSC_SIGNAL_TYPE CFG_GetSignalType( const char *s )
{
    if ( !s )
        return MSC_SIG_UNDEFINED;

    if ( !stricmp( s, STR_INOUT ) )
        return MSC_SIG_INOUT;

    if ( !stricmp( s, STR_OUT ) )
        return MSC_SIG_OUT;

    if ( !stricmp( s, STR_IN ) )
        return MSC_SIG_IN;

    if ( !stricmp( s, STR_IGNORE ) )
        return MSC_SIG_IGNORE;

    return MSC_SIG_UNDEFINED;
}

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
const char * MSC_InstanceType2Str( MSC_INSTANCE_TYPE x )
{
    switch ( x )
    {

    case MSC_INSTANCE_ENV:
        return (const char *)STR_ENV;

    case MSC_INSTANCE_IUT:
        return (const char *)STR_MODEL;

    case MSC_INSTANCE_VMODEL:
        return (const char *)STR_VMODEL;

    case MSC_INSTANCE_MONITOR:
        return (const char *)STR_MONITOR;

    case MSC_INSTANCE_IGNORE:
        return (const char *)STR_IGNORE;

    case MSC_INSTANCE_UNDEF:
        return TAT_ZeroStr;
    }

    return TAT_ZeroStr;
}

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
const char * MSC_SignalType2Str( MSC_SIGNAL_TYPE x )
{
    switch ( x )
    {

    case MSC_SIG_INOUT:
        return (const char *)STR_INOUT;

    case MSC_SIG_IN:
        return (const char *)STR_IN;

    case MSC_SIG_OUT:
        return (const char *)STR_OUT;

    case MSC_SIG_IGNORE:
        return (const char *)STR_IGNORE;

    case MSC_SIG_UNDEFINED:
        return TAT_ZeroStr;
    }

    return TAT_ZeroStr;
}

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
MSC_MACRO_TYPE CFG_GetMacroType( const char *s )
{
    if ( !s )
        return MSC_MACROTYPE_UNDEFINED;

    if ( !stricmp( s, STR_MACRO_LIST ) )
        return MSC_MACROTYPE_LIST;

    if ( !stricmp( s, STR_MACRO_FUNCTION ) )
        return MSC_MACROTYPE_FUNCTION;

    if ( !stricmp( s, STR_MACRO_DERIVE ) )
        return MSC_MACROTYPE_DERIVE;

    if ( !stricmp( s, STR_MACRO_ABORT ) )
        return MSC_MACROTYPE_ABORT;

    if ( !stricmp( s, STR_MACRO_INDEX ) )
        return MSC_MACROTYPE_INDEX;

    return MSC_MACROTYPE_UNDEFINED;
}

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
MSC_MACRO_DATA CFG_GetMacroDataType( const char *s )
{
    if ( !s )
        return MSC_MACRODATA_UNDEFINED;

    if ( !stricmp( s, STR_MACRO_STRING ) )
        return MSC_MACRODATA_STRING;

    if ( !stricmp( s, STR_MACRO_NUMBER ) )
        return MSC_MACRODATA_NUMBER;

    if ( !stricmp( s, STR_MACRO_ID ) )
        return MSC_MACRODATA_ID;

    if ( !stricmp( s, STR_MACRO_UNDEFINED ) )
        return MSC_MACRODATA_UNDEF;

    return MSC_MACRODATA_UNDEFINED;
}

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
const char * CFG_GetMacroDataTypeStr( MSC_MACRO_DATA tp )
{
    switch ( tp )
    {

    case MSC_MACRODATA_STRING:
        return (const char *)STR_MACRO_STRING;

    case MSC_MACRODATA_NUMBER:
        return (const char *)STR_MACRO_NUMBER;

    case MSC_MACRODATA_ID:
        return (const char *)STR_MACRO_ID;

    case MSC_MACRODATA_UNDEF:
        return (const char *)STR_MACRO_UNDEFINED;
    }

    return TAT_ZeroStr;
}

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
MSC_MACRO_REPL CFG_GetReplaceType( const char *s )
{
    if ( !s )
        return MSC_REPL_UNDEFINED;

    if ( !stricmp( s, STR_MACRO_REPLACE ) )
        return MSC_REPL_REPLACE;

    if ( !stricmp( s, STR_MACRO_EXTEND ) )
        return MSC_REPL_EXTEND;

    if ( !stricmp( s, STR_MACRO_SUBSET ) )
        return MSC_REPL_SUBSET;

    if ( !stricmp( s, STR_MACRO_REMOVE ) )
        return MSC_REPL_REMOVE;

    return MSC_REPL_UNDEFINED;
}

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
MSC_RESEARCH_TYPE CFG_GetResearchType( const char *s )
{
    if ( !s )
        return MSC_RESEARCH_SEQUENTIAL;

    if ( !stricmp( s, STR_MACRO_SEQUENTIAL ) )
        return MSC_RESEARCH_SEQUENTIAL;

    if ( !stricmp( s, STR_MACRO_RANDOM ) )
        return MSC_RESEARCH_RANDOM;

    if ( !stricmp( s, STR_MACRO_PRIORITY ) )
        return MSC_RESEARCH_PRIORITY;

    return MSC_RESEARCH_UNKNOWN;
}

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
MSC_DATA_CLASS CFG_GetDataClassType( const char *s )
{
    if ( !s )
        return MSC_DATA_UNKNOWN;

    if ( !stricmp( s, STR_DATA_INTEGER ) )
        return MSC_DATA_INTEGER;

    if ( !stricmp( s, STR_DATA_FLOAT ) )
        return MSC_DATA_FLOAT;

    if ( !stricmp( s, STR_DATA_SYMBOL ) )
        return MSC_DATA_SYMBOL;

    if ( !stricmp( s, STR_DATA_STRING ) )
        return MSC_DATA_STRING;

    if ( !stricmp( s, STR_DATA_IDENTIFIER ) )
        return MSC_DATA_IDENTIFIER;

    
    return MSC_DATA_UNKNOWN;
}

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
const char * CFG_GetDataClassTypeStr( MSC_DATA_CLASS x )
{

    switch ( x )
    {
    case MSC_DATA_INTEGER:
        return (const char *)STR_DATA_INTEGER;
    case MSC_DATA_FLOAT:
        return (const char *)STR_DATA_FLOAT;
    case MSC_DATA_SYMBOL:
        return (const char *)STR_DATA_SYMBOL;
    case MSC_DATA_STRING:
        return (const char *)STR_DATA_STRING;
    case MSC_DATA_IDENTIFIER:
        return (const char *)STR_DATA_IDENTIFIER;
    }

    return TAT_ZeroStr;
}

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
LOGICAL CFG_CheckMacroData( MSC_MACRO_DATA t, char *data )
{
    switch ( t )
    {

    case MSC_MACRODATA_STRING:
    {
        if ( data[0] != '\"' )
            return RESULT_ERR;

        if ( !StrExtractString( &data, '\"', NULL ) || ( data[0] != 0 ) )
            return RESULT_ERR;

        break;
    }

    case MSC_MACRODATA_NUMBER:
    {
        if ( !StrExtractDigit( &data, NULL ) )
            return RESULT_ERR;

        StrSkipComments( &data );

        if ( *data == 0 )
            break;

        if ( *data != '{' )
            return RESULT_ERR;

        data++;

        if ( !StrExtractDigit( &data, NULL ) )
            return RESULT_ERR;

        StrSkipComments( &data );

        if ( *data != '}' )
            return RESULT_ERR;

        data++;

        if ( !StrExtractDigit( &data, NULL ) )
            return RESULT_ERR;

        StrSkipComments( &data );

        if ( *data != 0 )
            return RESULT_ERR;

        break;
    }

    case MSC_MACRODATA_ID:
    {
        if ( !StrExtractLabel( &data, NULL ) || ( data[0] != 0 ) )
            return RESULT_ERR;

        break;
    }

    case MSC_MACRODATA_UNDEF:
    {
        if ( data[0] == '\"' )
        {
            if ( !StrExtractString( &data, '\"', NULL ) || ( data[0] != 0 ) )
                return RESULT_ERR;
        }
        else
        /* identifier or digit */
        if ( data[0] == '+' || data[0] == '-' || SymIsDigit( data[0] ) )
        {
            if ( !StrExtractDigit( &data, NULL ) )
            {
                return RESULT_ERR;
            }
        }

        break;
    }
    }

    return RESULT_OK;
}
