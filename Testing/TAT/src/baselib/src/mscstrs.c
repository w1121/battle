/******************************************************************************
*
*       COPYRIGHT 2001-2004 MOTOROLA, ALL RIGHTS RESERVED
*
*       MOTOROLA CONFIDENTIAL PROPRIETARY
*
* Description:  Command-line processing
*
* $RCSfile: mscstrs.c,v $
* $Revision: 1.3 $
* $Source: /home/cvsusr/cvssoft/tat_tools/baselib/src/mscstrs.c,v $
* $Date: 2004/03/05 14:37:44 $
*
*
* CHANGE HISTORY
*                                                       
* Initial version 
*
******************************************************************************/
#include <mscstrs.h>

/* string constants */
DEF_STR( STR_MSCSP, "msc " );
DEF_STR( STR_MSCDOCUMENTSP, "mscdocument " );
DEF_STR( STR_ENDMSC, "endmsc" );
DEF_STR( STR_ENDINSTANCE, "endinstance" );
DEF_STR( STR_INSTANCE, "instance" );
DEF_STR( STR_INSTANCEHEAD, "instancehead" );

DEF_STR( MSC_GLOBAL_CONDITION_NAME, "TAT_SYNCHRONIZATION" );
DEF_STR( MSC_REF_CONDITION_NAME, "TAT_REFERENCE_SYNCHRO" );
DEF_STR( MSC_CONDITION_EVENT_NAME, "TAT_SYNCHRONIZATION" );
DEF_STR( MSC_LOST_INSTANCE_NAME, "lost" );
DEF_STR( MSC_FOUND_INSTANCE_NAME, "found" );

DEF_STR( STR_ALT, "alt" );
DEF_STR( STR_PAR, "par" );
DEF_STR( STR_OPT, "opt" );
DEF_STR( STR_EXC, "exc" );
DEF_STR( STR_LOOP, "loop" );

DEF_STR( STR_EMPTY, "empty" );
DEF_STR( STR_BREAK, "break" );
DEF_STR( STR_CODE, "code" );

DEF_STR( STR_ZERO, "0" );
DEF_STR( STR_INF, "INF" );
DEF_STR( STR_ONE, "1" );

DEF_STR( STR_IGNORE, "ignore" );
DEF_STR( STR_MODEL, "model" );
DEF_STR( STR_ENV, "env" );
DEF_STR( STR_VMODEL, "vmodel" );
DEF_STR( STR_IN, "in" );
DEF_STR( STR_OUT, "out" );
DEF_STR( STR_INOUT, "inout" );

DEF_STR( STR_DECOMPOSED, "decomposed" );
DEF_STR( STR_COMMENT, "comment" );
DEF_STR( STR_COMMENTSP, "comment " );
DEF_STR( STR_SDTREF, "#SDTREF" );

DEF_STR( STR_REFERENCE, "reference" );
DEF_STR( STR_CREATE, "create" );
DEF_STR( STR_BEGIN, "begin" );
DEF_STR( STR_ACTION, "action" );
DEF_STR( STR_CONDITION, "condition" );
DEF_STR( STR_STOP, "stop" );
DEF_STR( STR_CONCURRENT, "concurrent" );
DEF_STR( STR_ENDCONCURRENT, "endconcurrent" );
DEF_STR( STR_SET, "set" );
DEF_STR( STR_STARTTIMER, "starttimer" );
DEF_STR( STR_RESET, "reset" );
DEF_STR( STR_STOPTIMER, "stoptimer" );
DEF_STR( STR_TIMEOUT, "timeout" );
DEF_STR( STR_ALL, "all" );
DEF_STR( STR_NOWSP, "now " );
DEF_STR( STR_NOWPLUS, "now+" );
DEF_STR( STR_END, "end" );
DEF_STR( STR_FROM, "from" );
DEF_STR( STR_TO, "to" );
DEF_STR( STR_TMR, "Tmr" );
DEF_STR( STR_UNTITLED, "Untitled" );
DEF_STR( STR_SEND, "send" );
DEF_STR( STR_EXIST, "exist" );
DEF_STR( STR_RECEIVE, "receive" );
DEF_STR( STR_AS, "as" );
DEF_STR( STR_TEXTSP, "text " );
DEF_STR( STR_SHAREDSP, "shared " );
DEF_STR( STR_HEAD, "head" );
DEF_STR( STR_DEFSP, "def " );
DEF_STR( STR_DOTMPR, ".mpr" );
DEF_STR( STR_DOTFG, ".fg" );
DEF_STR( STR_DOTMSC, ".msc" );

DEF_STR( STR_MONITOR, "monitor" );
DEF_STR( STR_MSCSTART, "MSCSTART" );
DEF_STR( STR_PARAMSSP, "params " );
DEF_STR( STR_IUT, "iut" );
DEF_STR( STR_WITH, "with" );
DEF_STR( STR_WITHOUT, "without" );

DEF_STR( STR_XML_PROJECT, "TATPROJECT" );
DEF_STR( STR_XML_CONFIG, "CONFIG" );
DEF_STR( STR_XML_INSTANCE, "INSTANCE" );
DEF_STR( STR_XML_INSTANCES, "INSTANCES" );
DEF_STR( STR_XML_SIGNALS, "SIGNALS" );
DEF_STR( STR_XML_SIGNAL, "SIGNAL" );
DEF_STR( STR_XML_PARAMETER, "PARAMETER" );
DEF_STR( STR_XML_ATTRIBUTE, "ATTRIBUTE" );
DEF_STR( STR_XML_MACROS, "MACROS" );
DEF_STR( STR_XML_MACRO, "MACRO" );
DEF_STR( STR_XML_DATATYPE, "DATATYPE" );
DEF_STR( STR_XML_TYPEDEF, "TYPEDEF" );
DEF_STR( STR_XML_VALUELIST, "VALUELIST" );
DEF_STR( STR_XML_CONDITION, "condition" );

DEF_STR( STR_XML_TYPE, "type" );
DEF_STR( STR_MACRO_DATATYPE, "datatype" );
DEF_STR( STR_MACRO_VECTOR, "vector" );

DEF_STR( STR_XML_DEFAULT, "default" );
DEF_STR( STR_XML_PARAMSTYPE, "paramstype" );

DEF_STR( STR_XML_NAME, "name" );
DEF_STR( STR_XML_BASE, "base" );
DEF_STR( STR_XML_VALUE, "value" );
DEF_STR( STR_XML_FIELD, "field" );
DEF_STR( STR_XML_PRIORITY, "priority" );

DEF_STR( STR_MACRO_REPLACE, "replace" );
DEF_STR( STR_MACRO_EXTEND, "extend" );
DEF_STR( STR_MACRO_SUBSET, "subset" );
DEF_STR( STR_MACRO_REMOVE, "remove" );

DEF_STR( STR_MACRO_LIST, "list" );
DEF_STR( STR_MACRO_DERIVE, "derive" );
DEF_STR( STR_MACRO_FUNCTION, "function" );
DEF_STR( STR_MACRO_ABORT, "abort" );
DEF_STR( STR_MACRO_INDEX, "index" );

DEF_STR( STR_MACRO_STRING, "string" );
DEF_STR( STR_MACRO_NUMBER, "number" );
DEF_STR( STR_MACRO_ID, "id" );
DEF_STR( STR_MACRO_UNDEFINED, "undefined" );

DEF_STR( STR_MACRO_SEQUENTIAL, "sequential" );
DEF_STR( STR_MACRO_RANDOM, "random" );
DEF_STR( STR_MACRO_RESEARCH, "research" );
DEF_STR( STR_MACRO_VALUE, "VALUE" );
DEF_STR( STR_MACRO_PRIORITY, "priority" );
DEF_STR( STR_MACRO_PRIOLIST, "priolist" );

DEF_STR( STR_DATA_DATACLASS, "dataclass" );

DEF_STR( STR_XML_PREFIX, "prefix" );

DEF_STR( STR_WAIT, "wait" );
DEF_STR( STR_WHEN, "when" );
DEF_STR( STR_IF, "if" );
DEF_STR( STR_WHILE, "while" );
DEF_STR( STR_ASSERT, "assert" );

DEF_STR( STR_DATA_INTEGER, "integer" );
DEF_STR( STR_DATA_FLOAT, "float" );
DEF_STR( STR_DATA_SYMBOL, "symbol" );
DEF_STR( STR_DATA_STRING, "string" );
DEF_STR( STR_DATA_IDENTIFIER, "identifier" );
DEF_STR( STR_DATA_ENUM, "enum" );

DEF_STR( STR_ANNO, "ANNO" );
DEF_STR( STR_PRE, "PRE" );
DEF_STR( STR_POST, "POST" );

DEF_STR( STR_ANNOTATION, "annotation");

DEF_STR( STR_XML_AXIOM, "AXIOM");
DEF_STR( STR_XML_AXIOMS, "AXIOMS");
DEF_STR( STR_AXIOM, "axiom");
DEF_STR( STR_INITIAL, "initial");
DEF_STR( STR_XML_INITIAL, "INITIAL");
DEF_STR( STR_CHECK, "check");

DEF_STR( STR_XML_MEMBER, "MEMBER" );
DEF_STR( STR_XML_DIMENSION, "dimension" );

