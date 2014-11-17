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
* Functions:    None
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  Shared message text
*
* Notes:        None
*
******************************************************************************/
#include <portable.h>
#include <mscmess.h>
#include <xmlmess.h>
#include <tatgdata.h>
#include <outmess.h>

DEF_MSG( MSG_XML_MACRO_SUBSET_ss, "incorrect subset for macro '%s', value '%s'" );

DEF_MSG( MSG_XML_INVALID_BASE_CONF_s, "invalid base configuration '%s'" );
DEF_MSG( MSG_XML_RECURSIVE_CONF, "recursive configuration definition" );
/* DEF_MSG( MSG_XML_MACRO_REPLACE_s, "ERROR: xml: Invalid 'replace' specification for  macro '%s'\n" );*/

DEF_MSG( MSG_XML_MACRO_FIELDS_s, "not all required fields defined correctly for macro '%s'" );

DEF_MSG( MSG_XML_MACRO_DATA_ss, "invalid data '%s' for macro '%s'" );
DEF_MSG( MSG_XML_MACRO_OBSOLETE_ss, "obsolete tag (attribute) '%s' for macro '%s'" );



DEF_MSG( MSG_XML_SYNTAX_ERROR, "syntax error");
DEF_MSG( MSG_XML_DUPLICATED_ID_s, "duplicated identifier '%s'" );

DEF_MSG( MSG_XML_REQUIRED_ATTR_s, "required attribute '%s' is not specified" );
DEF_MSG( MSG_XML_INVALID_ATTR_s, "invalid value for attribute '%s'" );


DEF_MSG( MSG_INCORRECT_XML_sus, "ERROR: xml: file '%s(%u)': %s\n" );
DEF_MSG( MSG_WARNING_XML_sus, "WARNING: xml: file '%s(%u)': %s\n" );

/***************************************************************************
 * Function : MSG_XMLError
 *
 * Description: Output semantic error position, and description
 *
 * Returns: None
 *
 * Notes: None
 *
 **************************************************************************/
void MSG_XMLError( const char * s, ... )
{
    char                    buffer[TAT_MAX_LINE_LEN];
    char                    *fname;

    va_list                 arglist;

    fname=MSG_GetFileName();
    if ( !fname || !s)
    {
        MSG_PrintError( MSG_INTERNAL_PROBLEM );
        exit( SYS_ERROR_EXIT_CODE );
    }

    va_start( arglist, s );
    vsprintf( buffer, s, arglist );

    MSG_PrintError( MSG_INCORRECT_XML_sus, fname,
        MSG_GetLineNumber(), buffer );

    return;
}

/***************************************************************************
 * Function : MSG_XMLErrorA
 *
 * Description: Output semantic error position, and description
 *
 * Returns: None
 *
 * Notes: None
 *
 **************************************************************************/
void MSG_XMLErrorA( const char * fname, const size_t line, const char * s, ... )
{
    char                    buffer[TAT_MAX_LINE_LEN];

    va_list                 arglist;

    if ( !fname || !s)
    {
        MSG_PrintError( MSG_INTERNAL_PROBLEM );
        exit( SYS_ERROR_EXIT_CODE );
    }

    va_start( arglist, s );
    vsprintf( buffer, s, arglist );

    MSG_PrintError( MSG_INCORRECT_XML_sus, fname,  line, buffer );

    return;
}
