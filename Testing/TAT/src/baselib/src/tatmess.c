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

/* Command line ERROR messages */
DEF_MSG( MSG_CMD_LINE_ERR_s, "ERROR: command-line: unknown parameter: '%s'\n" );
DEF_MSG( MSG_CMD_LINE_ERR_ss, "ERROR: command-line: unknown parameter: '%s %s'\n" );
DEF_MSG( MSG_CMD_DATA_EXPECTED_s, "ERROR: command-line: data expected for switch '%s'\n" );
DEF_MSG( MSG_CMD_DUP_SWITCH_s, "WARNING: command-line: duplicate usage of switch '%s'\n" );
DEF_MSG( MSG_CMD_WARN_OVRR_ssss, "WARNING: command-line: '%s %s' overrides '%s %s'\n" );
DEF_MSG( MSG_CMD_LONG_NAME_s, "ERROR: command-line: very long file name '%s'\n" );

DEF_MSG( MSG_INVALID_SWITCH_USAGE_ss, "ERROR: command-line: invalid combination of switches '%s' and '%s'\n" );
DEF_MSG( MSG_WARNING_SWITCH_USAGE_sss, "WARNING: command-line: switch '%s' is ignored %s '%s'\n" );
DEF_MSG( MSG_INVALID_SWITCH_VALUE_s, "ERROR: command-line: invalid value for switch '%s'\n" );

/* file related errors */
DEF_MSG( MSG_FILE_DIR_CANT_CREATE_s, "ERROR: file: '%s' couldn't be created\n" );
DEF_MSG( MSG_FILE_RELATED_ERROR_ss, "ERROR: file: '%s': %s\n" );
DEF_MSG( MSG_FILE_NO_MATCH_s, "WARNING: file: no match with '%s'\n" );

DEF_MSG( MSG_INTERNAL_PROBLEM, "ERROR: internal problem\n" );

#ifdef _DEBUG
DEF_MSG( MSG_EMPTY_STRING_WARNING, "DEBUG: Empty string element. Work around\n" );
DEF_MSG( MSG_INVALID_STRING_NUMBER_uu, "DEBUG: Internal problem. Accessing string %u,but available only %u\n" );
DEF_MSG( MSG_FREE_NULL_POINTER, "DEBUG: Free() called with NULL pointer\n" );
#endif

DEF_MSG( MSG_INCORRECT_SYNTAX_su, "ERROR: syntax: file '%s(%u)'\n" );
DEF_MSG( MSG_INCORRECT_SEMANTIC_sus, "ERROR: semantic: file '%s(%u)': %s\n" );
DEF_MSG( MSG_WARNING_SEMANTIC_sus, "WARNING: semantic: file '%s(%u)': %s\n" );

DEF_MSG( MSG_ALLOCATE_FAILED_u, "ERROR: memory: allocation of %u bytes failed\n" );
DEF_MSG( MSG_INVALID_USER_FUNC_RESULT, "ERROR:  invalid result while processing string list\n" );

DEF_MSG( MSG_INVALID_TIME_SPEC_CALC, "WARNING: time: file '%s(%u)': invalid time specification\n" );
DEF_MSG( MSG_AMBIG_TIME_SPEC,
    "WARNING: time: file '%s(%u)': event shares different time bases '%s' and '%s'\n" );


DEF_MSG( MSG_LICENSE_CORRUPTED_s, "\nERROR: license: file '%s': corrupted\n" );
DEF_MSG( MSG_LICENSE_EXPIRED, "\nERROR: license: period expired\n" );
DEF_MSG( MSG_NO_LICENSE_FILE, "\nERROR: license: file not found\n" );
DEF_MSG( MSG_NO_HOST_NAME, "\nERROR: license: can't get host name\n" );


DEF_MSG( MSG_MESS_MSC_FOUND_INSTANCE_ID_ENV_su,
            "WARNING: semantic: file '%s(%u)': removed message from 'found' to environment\n" );
DEF_MSG( MSG_MESS_ENV_LOST_su, "WARNING: semantic: file '%s(%u)': removed message from environment to 'lost'\n" );

DEF_MSG( MSG_INVALID_MESS_DIR_su, "WARNING: semantic: file '%s(%u)': removed message with invalid direction\n" );

DEF_MSG( MSG_SEM_UNDECL_VAR_sus, "ERROR: semantic: file '%s(%u)': undeclared variable '%s'\n" );
DEF_MSG( MSG_NO_DEF_INSTANCE_s, "ERROR: semantic: no definition for instance '%s'\n" );
DEF_MSG( MSG_UNABLE_DETERMINE_FILEFORMAT, "unable to determine file format");

DEF_MSG( MSG_CONFIG_MESS_PARAMS_susuu,
        "ERROR: semantic: file '%s(%u)': inconsistent number of parameters for '%s' - %u vs. declared %u\n" );

DEF_MSG( MSG_CONFIG_VAR_DATATYPE_susss,
        "ERROR: semantic: file '%s(%u)': variable '%s' data type mismatch - parameter types '%s' and '%s'\n" );
DEF_MSG( MSG_CONFIG_VAR_ASSIGN_OUT_sus,
        "ERROR: semantic: file '%s(%u)': variable can't be assigned in output: '%s'\n" );

DEF_MSG( MSG_CONFIG_UNDEF_SIGNAL_sus, "ERROR: semantic: file '%s(%u)': undefined signal '%s'\n" );
DEF_MSG( MSG_CONFIG_UNDEF_INSTANCE_TYPE_suss, "WARNING: config: file '%s(%u)': type of '%s' instance set as '%s'\n");


DEF_MSG( MSG_INVALID_VALUE_suss, "ERROR: semantic: file '%s(%u)': invalid value '%s' for type '%s'\n" );
