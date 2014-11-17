#ifndef XMLMESS_H
#define XMLMESS_H
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
* Functions:    MSG_XMLError
*               MSG_XMLErrorA
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  XML related messages
*
* Notes:        None
*
******************************************************************************/
#include <portable.h>
#ifdef __cplusplus
extern                  "C"
{
#endif                                 /* C++ */

DEF_MSG_EXT( MSG_XML_MACRO_SUBSET_ss, "incorrect subset for macro '%s', value '%s'" );
DEF_MSG_EXT( MSG_XML_RECURSIVE_CONF, "recursive configuration definition" );
DEF_MSG_EXT( MSG_XML_INVALID_BASE_CONF_s, "invalid base configuration '%s'" );
DEF_MSG_EXT( MSG_XML_MACRO_FIELDS_s, "not all required fields defined correctly for macro '%s'" );
DEF_MSG_EXT( MSG_XML_MACRO_DATA_ss, "invalid data '%s' for macro '%s'" );
DEF_MSG_EXT( MSG_XML_MACRO_OBSOLETE_ss, "obsolete tag (attribute) '%s' for macro '%s'" );
DEF_MSG_EXT( MSG_XML_SYNTAX_ERROR, "syntax error");
DEF_MSG_EXT( MSG_XML_DUPLICATED_ID_s, "duplicated identifier '%s'" );
DEF_MSG_EXT( MSG_XML_REQUIRED_ATTR_s, "required attribute '%s' is not specified" );
DEF_MSG_EXT( MSG_XML_INVALID_ATTR_s, "invalid value for attribute '%s'" );

DEF_MSG_EXT( MSG_INCORRECT_XML_sus, "ERROR: xml: file '%s(%u)': %s\n" );
DEF_MSG_EXT( MSG_WARNING_XML_sus, "WARNING: xml: file '%s(%u)': %s\n" );

extern void MSG_XMLError( const char * s, ... );
extern void MSG_XMLErrorA( const char * fname, const size_t line, const char * s, ... );

#ifdef __cplusplus
}
#endif                                 /* C++ */

#endif                         /* XMLMESS_H */
