#ifndef MSCMESS_H
#define MSCMESS_H
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

/* RESULT_ERR messages */
#define DEF_MSG(name, value) const char * name=value
#define DEF_MSG_EXT(name, value) extern const char * name  /* value */

/* Command line RESULT_ERR messages */
DEF_MSG_EXT( MSG_CMD_LINE_ERR_s, "ERROR: command-line: Unknown parameter: '%s'\n" );
DEF_MSG_EXT( MSG_CMD_LINE_ERR_ss, "ERROR: command-line: Unknown parameter: '%s %s'\n" );
DEF_MSG_EXT( MSG_CMD_DATA_EXPECTED_s, "ERROR: command-line: Data expected for switch '%s'\n" );
DEF_MSG_EXT( MSG_CMD_DUP_SWITCH_s, "WARNING: command-line: duplicate usage of switch '%s'\n" );
DEF_MSG_EXT( MSG_CMD_WARN_OVRR_ssss, "WARNING: command-line: '%s %s' overrides '%s %s'\n" );
DEF_MSG_EXT( MSG_CMD_LONG_NAME_s, "ERROR: command-line: Very long file name '%s'\n" );

DEF_MSG_EXT( MSG_INVALID_SWITCH_USAGE_ss,
    "ERROR: command-line: Invalid combination of switches '%s' and '%s'\n" );
DEF_MSG_EXT( MSG_WARNING_SWITCH_USAGE_sss, "WARNING: command-line: Switch '%s' is ignored %s '%s'\n" );
DEF_MSG_EXT( MSG_INVALID_SWITCH_VALUE_s, "ERROR: command-line: Invalid value for switch '%s'\n" );

/* file related errors */
DEF_MSG_EXT( MSG_FILE_DIR_CANT_CREATE_s, "ERROR: file: '%s' couldn't be created\n" );
DEF_MSG_EXT( MSG_FILE_RELATED_ERROR_ss, "ERROR: file: '%s': %s\n" );
DEF_MSG_EXT( MSG_FILE_NO_MATCH_s, "WARNING: file: No match with '%s'\n" );

DEF_MSG_EXT( MSG_INTERNAL_PROBLEM, "ERROR: Internal problem\n" );

#ifdef _DEBUG
DEF_MSG_EXT( MSG_EMPTY_STRING_WARNING, "DEBUG: Empty string element. Work around\n" );
DEF_MSG_EXT( MSG_INVALID_STRING_NUMBER_uu,
    "DEBUG: Internal problem. Accessing string %u,but available only %u\n" );
DEF_MSG_EXT( MSG_FREE_NULL_POINTER, "DEBUG: Free() called with NULL pointer\n" );
#endif

DEF_MSG_EXT( MSG_INCORRECT_SYNTAX_su, "ERROR: syntax: file '%s(%u)'\n" );
DEF_MSG_EXT( MSG_INCORRECT_SEMANTIC_sus, "ERROR: semantic: file '%s(%u)': %s\n" );
DEF_MSG_EXT( MSG_WARNING_SEMANTIC_sus, "WARNING: semantic: file '%s(%u)': %s\n" );

DEF_MSG_EXT( MSG_ALLOCATE_FAILED_u, "ERROR: memory: Allocation of %u bytes failed\n" );

DEF_MSG_EXT( MSG_INVALID_USER_FUNC_RESULT, "ERROR:  Invalid result while processing string list\n" );
DEF_MSG_EXT( MSG_MESS_MSC_FOUND_INSTANCE_ID_ENV_su,
            "WARNING: semantic: file '%s(%u)': removed message from 'found' to environment\n" );
DEF_MSG_EXT( MSG_MESS_ENV_LOST_su, "WARNING: semantic: file '%s(%u)': removed message from environment to 'lost'\n" );

DEF_MSG_EXT( MSG_INVALID_MESS_DIR_su, "WARNING: semantic: file '%s(%u)': removed message with invalid direction\n" );
DEF_MSG_EXT( MSG_INCOMPATIBLE_TYPE_susss, "WARNING: semantic: file '%s(%u)': incompatible types '%s' and '%s' for variable '%s'\n" );

DEF_MSG_EXT( MSG_INVALID_TIME_SPEC_CALC, "ERROR: time: Invalid time calculated in msc line %u:\n" );
DEF_MSG_EXT( MSG_AMBIG_TIME_SPEC, "WARNING: time: Ambiguous time specification (current time %s+%g..%s):\n" );

DEF_MSG_EXT( MSG_FEATURE_NOT_SUPP,
    "ERROR: partial-order: Feature not supported: operators within parallel frames\n" );
DEF_MSG_EXT( MSG_MESS_WIN_ALT_MIXED, "ERROR: partial-order: Messages in 'alt' frames are mixed\n" );

DEF_MSG_EXT( MSG_VARIANCE_IN_PO, "ERROR: partial-order: Variance in partial order specification\n" );
DEF_MSG_EXT( MSG_PATHS_CAN_GENERATE, "REMARK: At least %u paths can be generated\n" );

DEF_MSG_EXT( MSG_NO_DEF_INSTANCE_s, "ERROR: semantic: No definition for instance '%s'\n" );

/* MSC semantic errors */
DEF_MSG_EXT( MSG_CREATE_INSIDE_LOOP, "create instance inside loop is invalid" );
DEF_MSG_EXT( MSG_EVENT_USE_STOPPED_INST, "event used stopped instance" );
DEF_MSG_EXT( MSG_INSTANCE_ALREADY_DEFINED, "instance already defined" );
DEF_MSG_EXT( MSG_MESS_IN_WO_OUT, "'in' without appropriate 'out'" );
DEF_MSG_EXT( MSG_MESS_OUT_WO_IN, "'out' without appropriate 'in'" );
DEF_MSG_EXT( MSG_MSC_WO_MSCDOCUMENT, "MSC without mscdocument keyword" );
DEF_MSG_EXT( MSG_INCOMPATIBLE_MSCNAME, "incompatible msc name" );
DEF_MSG_EXT( MSG_MSCBODY_WO_MSC, "MSC without header" );
DEF_MSG_EXT( MSG_UNDECLARED_INSTANCE, "undeclared instance" );
DEF_MSG_EXT( MSG_ENDMSC_WO_MSC, "'endmsc' without 'msc'" );
DEF_MSG_EXT( MSG_NO_ENDMSC_FIELD, "missing 'endmsc' statement\n" );

DEF_MSG_EXT( MSG_INVALID_VARNAME, "incorrect variable name" );
DEF_MSG_EXT( MSG_OPENED_COREGION, "coregion is opened, but not closed" );
DEF_MSG_EXT( MSG_INCONSISTENT_TIMER, "inconsistent timer declaration" );
DEF_MSG_EXT( MSG_RESERVED_NAME, "reserved name as identifier" );
DEF_MSG_EXT( MSG_UNKNOWN_LABEL, "undeclared time label" );
DEF_MSG_EXT( MSG_MISSING_BEGIN, "missing 'begin' keyword\n" );
DEF_MSG_EXT( MSG_DUP_LABEL_DEF, "duplicate time label definition" );
DEF_MSG_EXT( MSG_INCORRECT_LABEL_DEF, "incorrect time label definition" );
DEF_MSG_EXT( MSG_INCORRECT_DUR_SPEC, "incorrect duration specification" );
DEF_MSG_EXT( MSG_MESS_EXCHNUM_EXP, "message exchange number expected" );
DEF_MSG_EXT( MSG_MESS_LABEL_DUPLICATED, "duplicated message exchange number" );

DEF_MSG_EXT( MSG_EVENT_WO_INST, "unattached event" );
DEF_MSG_EXT( MSG_INCONSISTENT_MESSAGE, "inconsistent message declaration" );
DEF_MSG_EXT( MSG_INCORRECT_FORM, "invalid message with lost/found" );
DEF_MSG_EXT( MSG_MULTIPLE_INSTANCES, "multiple instances for single-instance event" );
DEF_MSG_EXT( MSG_INST_STOP_NOTCREATE, "instance stopped without being created" );
DEF_MSG_EXT( MSG_DOUBLE_INST_CREATION, "instance creation duplicated" );
DEF_MSG_EXT( MSG_INST_STOP_TWICE, "instance stopped twice" );
DEF_MSG_EXT( MSG_CREATE_WO_STOP, "'stop' is missed for created instance" );
DEF_MSG_EXT( MSG_NESTED_COREGIONS, "nested coregions" );
DEF_MSG_EXT( MSG_UNOPENED_COREGION, "coregion is closed without being opened" );
DEF_MSG_EXT( MSG_DUPLICATED_TIMER, "duplicated timer identifier" );
DEF_MSG_EXT( MSG_UNDEFINED_TIMER, "undefined timer" );
DEF_MSG_EXT( MSG_OPER_IN_COREGION, "operator in coregion" );
DEF_MSG_EXT( MSG_INVALID_TIME_LABEL_LOOP, "time label loop" );
DEF_MSG_EXT( MSG_TERM_MISSING, "inline operator's terminator is missing" );
DEF_MSG_EXT( MSG_MISSED_ENDINSTANCE, "'endinstance' keyword is missed" );
DEF_MSG_EXT( MSG_CANT_CREATE_SELF, "instance can't create itself" );
DEF_MSG_EXT( MSG_INVALID_TIME_EXPR, "invalid time expression" );
DEF_MSG_EXT( MSG_EXPR_WITHIN_ANOTHER, "inconsistent inline expression" );
DEF_MSG_EXT( MSG_INVALID_COUNTER, "invalid loop counter" );
DEF_MSG_EXT( MSG_REFERENCE_TO_LABEL_LOOP, "label outside block" );
DEF_MSG_EXT( MSG_BAD_PATH_CONTROL, "invalid path constraint" );
DEF_MSG_EXT( MSG_REQUIRES_EXPRESSION, "special condition requires expression" );
DEF_MSG_EXT( MSG_STOP_OUTSIDE_BLOCK, "'stop' outside 'create' block" );
DEF_MSG_EXT( MSG_INST_NOT_DECL, "instance used without being declared" );
DEF_MSG_EXT( MSG_STOP_INSIDE_LOOP, "'stop' inside loop block" );
DEF_MSG_EXT( MSG_CREATE_USED_INST, "creation of used instance" );

DEF_MSG_EXT( MSG_CONFIG_MESS_PARAMS_susuu,
    "ERROR: semantic: file '%s(%u)': inconsistent number of parameters for '%s' - %u vs. declared %u\n" );
DEF_MSG_EXT( MSG_CONFIG_VAR_DATATYPE_susss,
    "ERROR: semantic: file '%s(%u)': variable '%s' data type mismatch - parameter types '%s' and '%s'\n" );
DEF_MSG_EXT( MSG_CONFIG_VAR_ASSIGN_OUT_sus,
    "ERROR: semantic: file '%s(%u)': variable can't be assigned in output: '%s'\n" );

DEF_MSG_EXT( MSG_CONFIG_UNDEF_SIGNAL_sus, "ERROR: semantic: file '%s(%u)': undefined signal '%s'\n" );

DEF_MSG_EXT( MSG_SEM_UNDECL_VAR_sus, "ERROR: semantic: file '%s(%u)': undeclared variable '%s'\n" );
DEF_MSG_EXT( MSG_INVALID_VALUE_suss, "ERROR: semantic: file '%s(%u)': invalid value '%s' for type '%s'\n" );
DEF_MSG_EXT( MSG_INVALID_SOURCEREF, "invalid source reference" );

DEF_MSG_EXT( MSG_LICENSE_CORRUPTED_s, "ERROR: license: file '%s': corrupted\n" );
DEF_MSG_EXT( MSG_CONFIG_CAN_GENERATED_u, "REMARK: config: %u file(s) can be generated" );

DEF_MSG_EXT( MSG_LICENSE_EXPIRED, "ERROR: license: period expired\n" );

DEF_MSG_EXT( MSG_NO_LICENSE_FILE, "ERROR: license: file not found\n" );
DEF_MSG_EXT( MSG_RELATIVE_TIME_TF_su, "WARNING: semantic: file '%s(%u)': 'relative' time is not ordered\n" );
DEF_MSG_EXT( MSG_MSC_DUPL_DECLARATION, "duplicate MSC declaration" );

DEF_MSG_EXT( MSG_NO_HOST_NAME, "ERROR: license: can't get host name\n" );
DEF_MSG_EXT( MSG_CONFIG_UNDEF_INSTANCE_TYPE_suss, "WARNING: config: file '%s(%u)': type of '%s' instance set as '%s'\n");
DEF_MSG_EXT( MSG_UNABLE_DETERMINE_FILEFORMAT, "unable to determine file format");
DEF_MSG_EXT( MSG_VALUE_TOO_LARGE, "value too large");

#endif                         /* MSCMESS_H */
