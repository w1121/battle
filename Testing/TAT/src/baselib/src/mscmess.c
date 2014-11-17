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
#include <mscmess.h>

DEF_MSG( MSG_INCOMPATIBLE_TYPE_susss, "WARNING: semantic: file '%s(%u)': incompatible types '%s' and '%s' for variable '%s'\n" );

/* MSC semantic errors */
DEF_MSG( MSG_CREATE_INSIDE_LOOP, "create instance inside loop is invalid" );
DEF_MSG( MSG_EVENT_USE_STOPPED_INST, "event used stopped instance" );
DEF_MSG( MSG_INSTANCE_ALREADY_DEFINED, "instance already defined" );
DEF_MSG( MSG_MESS_IN_WO_OUT, "'in' without appropriate 'out'" );
DEF_MSG( MSG_MESS_OUT_WO_IN, "'out' without appropriate 'in'" );
DEF_MSG( MSG_MSC_WO_MSCDOCUMENT, "MSC without 'mscdocument' keyword" );
DEF_MSG( MSG_INCOMPATIBLE_MSCNAME, "incompatible msc name" );
DEF_MSG( MSG_MSCBODY_WO_MSC, "MSC without header" );
DEF_MSG( MSG_UNDECLARED_INSTANCE, "undeclared instance" );
DEF_MSG( MSG_ENDMSC_WO_MSC, "'endmsc' without 'msc'" );
DEF_MSG( MSG_NO_ENDMSC_FIELD, "missing 'endmsc' statement" );

DEF_MSG( MSG_INVALID_VARNAME, "invalid variable name" );
DEF_MSG( MSG_OPENED_COREGION, "coregion is opened, but not closed" );
DEF_MSG( MSG_INCONSISTENT_TIMER, "inconsistent timer declaration" );
DEF_MSG( MSG_RESERVED_NAME, "reserved name as identifier" );
DEF_MSG( MSG_UNKNOWN_LABEL, "undeclared time label" );
DEF_MSG( MSG_MISSING_BEGIN, "missing 'begin' keyword" );
DEF_MSG( MSG_DUP_LABEL_DEF, "duplicate time label definition" );
DEF_MSG( MSG_INCORRECT_LABEL_DEF, "incorrect time label definition" );
DEF_MSG( MSG_INCORRECT_DUR_SPEC, "incorrect duration specification" );
DEF_MSG( MSG_MESS_EXCHNUM_EXP, "message exchange number expected" );
DEF_MSG( MSG_MESS_LABEL_DUPLICATED, "duplicated message exchange number" );

DEF_MSG( MSG_EVENT_WO_INST, "unattached event" );
DEF_MSG( MSG_INCONSISTENT_MESSAGE, "inconsistent message declaration" );
DEF_MSG( MSG_INCORRECT_FORM, "invalid message with lost/found" );
DEF_MSG( MSG_MULTIPLE_INSTANCES, "multiple instances for single-instance event" );
DEF_MSG( MSG_INST_STOP_NOTCREATE, "instance stopped without being created" );
DEF_MSG( MSG_INST_NOT_DECL, "instance used without being declared" );
DEF_MSG( MSG_DOUBLE_INST_CREATION, "instance creation duplicated" );
DEF_MSG( MSG_INST_STOP_TWICE, "instance stopped twice" );
DEF_MSG( MSG_CREATE_WO_STOP, "'stop' is missed for created instance" );
DEF_MSG( MSG_NESTED_COREGIONS, "nested coregions" );
DEF_MSG( MSG_UNOPENED_COREGION, "coregion is closed without being opened" );
DEF_MSG( MSG_DUPLICATED_TIMER, "duplicated timer identifier" );
DEF_MSG( MSG_UNDEFINED_TIMER, "undefined timer" );
DEF_MSG( MSG_OPER_IN_COREGION, "operator in coregion" );
DEF_MSG( MSG_INVALID_TIME_LABEL_LOOP, "time label loop" );
DEF_MSG( MSG_TERM_MISSING, "inline operator's terminator is missing" );
DEF_MSG( MSG_MISSED_ENDINSTANCE, "'endinstance' keyword is missed" );
DEF_MSG( MSG_CANT_CREATE_SELF, "instance can't create itself" );
DEF_MSG( MSG_INVALID_TIME_EXPR, "invalid time expression" );
DEF_MSG( MSG_EXPR_WITHIN_ANOTHER, "inconsistent inline expression" );
DEF_MSG( MSG_INVALID_COUNTER, "invalid loop counter" );
DEF_MSG( MSG_REFERENCE_TO_LABEL_LOOP, "label outside block" );
DEF_MSG( MSG_BAD_PATH_CONTROL, "invalid path constraint" );
DEF_MSG( MSG_REQUIRES_EXPRESSION, "special condition requires expression" );
DEF_MSG( MSG_INVALID_SOURCEREF, "invalid source reference" );
DEF_MSG( MSG_STOP_OUTSIDE_BLOCK, "'stop' outside 'create' block" );
DEF_MSG( MSG_STOP_INSIDE_LOOP, "'stop' inside loop block" );
DEF_MSG( MSG_CREATE_USED_INST, "creation of used instance" );
DEF_MSG( MSG_MSC_DUPL_DECLARATION, "duplicate MSC declaration" );
DEF_MSG( MSG_VALUE_TOO_LARGE, "value too large");


