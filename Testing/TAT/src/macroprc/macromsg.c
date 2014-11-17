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
* Description:  
*
* Notes:        None
*
******************************************************************************/
#include <mscstrs.h>
#include <mscmess.h>
#include "macroprc.h"

DEF_MSG( MSG_MMSC_START_LARGE, "ERROR: mmsc: start number is too large\n" );
DEF_MSG( MSG_MMSC_UNKNOWN_MACRO, "WARNING: semantic: file '%s': unknown macro '%s'\n" );
DEF_MSG( MSG_MMSC_UNKNOWN_MACRO_CMD, "WARNING: command-line: unknown macro '%s'\n" );
DEF_MSG( MSG_MMSC_UNKNOWN_INP_MACRO, "unknown macro '%s'" );
DEF_MSG( MSG_CONFIG_CAN_GENERATED_u, "REMARK: config: %u file(s) can be generated\n" );
DEF_MSG( MSG_MMSC_CANT_COMPUTE_ss, "WARNING: semantic: file '%s': can't compute value for macro '%s'\n");
DEF_MSG( MSG_MMSC_RETURNS_ERROR_sss, "ERROR: xml: file '%s': function '%s' for macro '%s' reports error\n");
DEF_MSG( MSG_MMSC_INVALID_DATATYPE_ssss, "ERROR: semantic: file '%s': function '%s' for macro '%s' returns invalid data '%s'\n");
DEF_MSG( MSG_MMSC_EXPR_SYNTAX_ss, "ERROR: syntax: file '%s': can't parse '%s'\n");
DEF_MSG( MSG_MMSC_SUSP_SYNTAX_ss, "WARNING: syntax: file '%s': suspicious macro syntax '%s'\n");
DEF_MSG( MSG_MMSC_BRACE_EXPECTED_ss, "ERROR: syntax: file '%s': '(' expected in '%s'\n" );
DEF_MSG( MSG_MMSC_CANT_PARSE_ARG_sus, "ERROR: syntax: file '%s': can't parse argument %u from '%s'\n");
DEF_MSG( MSG_MMSC_EXPECTED_ARGS_sus, "ERROR: semantic: file '%s': expected %u arguments for '%s'\n");
DEF_MSG( MSG_NO_MACRO_DEFINED, "WARNING: xml: no macros defined\n" );




