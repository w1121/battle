#ifndef MSCSTRS_H
#define MSCSTRS_H
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
* Description:  MSC string constants
*
* Notes:        None
*
******************************************************************************/
#include <portable.h>

#define DEF_STR(name, value) const char name[]=value;const int name##_LEN=sizeof(name)-1

#define DEF_STR_EXT(name, value) extern const char name[]; /*value */ extern const int name##_LEN

#define APPEND_STR(dest, name) memcpy(dest,name,name##_LEN);dest+=name##_LEN

#define APPEND_NSTR(dest, src) {register char *_s=src; register int _len=strlen(_s); memcpy(dest,_s,_len); dest+=_len;}

/* string constants */
DEF_STR_EXT( STR_MSCSP, "msc " );
DEF_STR_EXT( STR_MSCDOCUMENTSP, "mscdocument " );
DEF_STR_EXT( STR_ENDMSC, "endmsc" );
DEF_STR_EXT( STR_ENDINSTANCE, "endinstance" );
DEF_STR_EXT( STR_INSTANCE, "instance" );
DEF_STR_EXT( STR_INSTANCEHEAD, "instancehead" );

DEF_STR_EXT( MSC_GLOBAL_CONDITION_NAME, "TAT_SYNCHRONIZATION" );
DEF_STR_EXT( MSC_REF_CONDITION_NAME, "TAT_REFERENCE_SYNCHRO" );
DEF_STR_EXT( MSC_CONDITION_EVENT_NAME, "TAT_SYNCHRONIZATION" );
DEF_STR_EXT( MSC_LOST_INSTANCE_NAME, "lost" );
DEF_STR_EXT( MSC_FOUND_INSTANCE_NAME, "found" );

DEF_STR_EXT( STR_ALT, "alt" );
DEF_STR_EXT( STR_PAR, "par" );
DEF_STR_EXT( STR_OPT, "opt" );
DEF_STR_EXT( STR_EXC, "exc" );
DEF_STR_EXT( STR_LOOP, "loop" );

DEF_STR_EXT( STR_EMPTY, "empty" );
DEF_STR_EXT( STR_BREAK, "break" );
DEF_STR_EXT( STR_CODE, "code" );

DEF_STR_EXT( STR_ZERO, "0" );
DEF_STR_EXT( STR_INF, "INF" );
DEF_STR_EXT( STR_ONE, "1" );

DEF_STR_EXT( STR_IGNORE, "ignore" );
DEF_STR_EXT( STR_MODEL, "model" );
DEF_STR_EXT( STR_ENV, "env" );
DEF_STR_EXT( STR_VMODEL, "vmodel" );
DEF_STR_EXT( STR_IN, "in" );
DEF_STR_EXT( STR_OUT, "out" );
DEF_STR_EXT( STR_INOUT, "in_out" );

DEF_STR_EXT( STR_DECOMPOSED, "decomposed" );
DEF_STR_EXT( STR_COMMENT, "comment" );
DEF_STR_EXT( STR_COMMENTSP, "comment " );
DEF_STR_EXT( STR_SDTREF, "#SDTREF" );

DEF_STR_EXT( STR_REFERENCE, "reference" );
DEF_STR_EXT( STR_CREATE, "create" );
DEF_STR_EXT( STR_BEGIN, "begin" );
DEF_STR_EXT( STR_ACTION, "action" );
DEF_STR_EXT( STR_CONDITION, "condition" );
DEF_STR_EXT( STR_STOP, "stop" );
DEF_STR_EXT( STR_CONCURRENT, "concurrent" );
DEF_STR_EXT( STR_ENDCONCURRENT, "endconcurrent" );
DEF_STR_EXT( STR_SET, "set" );
DEF_STR_EXT( STR_STARTTIMER, "starttimer" );
DEF_STR_EXT( STR_RESET, "reset" );
DEF_STR_EXT( STR_STOPTIMER, "stoptimer" );
DEF_STR_EXT( STR_TIMEOUT, "timeout" );
DEF_STR_EXT( STR_ALL, "all" );
DEF_STR_EXT( STR_NOWSP, "now " );
DEF_STR_EXT( STR_NOWPLUS, "now+" );
DEF_STR_EXT( STR_END, "end" );
DEF_STR_EXT( STR_FROM, "from" );
DEF_STR_EXT( STR_TO, "to" );
DEF_STR_EXT( STR_TMR, "Tmr" );
DEF_STR_EXT( STR_UNTITLED, "Untitled" );
DEF_STR_EXT( STR_SEND, "send" );
DEF_STR_EXT( STR_EXIST, "exist" );
DEF_STR_EXT( STR_RECEIVE, "receive" );
DEF_STR_EXT( STR_AS, "as" );
DEF_STR_EXT( STR_TEXTSP, "text " );
DEF_STR_EXT( STR_SHAREDSP, "shared " );
DEF_STR_EXT( STR_HEAD, "head" );
DEF_STR_EXT( STR_DEFSP, "def " );
DEF_STR_EXT( STR_DOTMPR, ".mpr" );
DEF_STR_EXT( STR_DOTFG, ".fg" );
DEF_STR_EXT( STR_MONITOR, "monitor" );
DEF_STR_EXT( STR_MSCSTART, "MSCSTART" );
DEF_STR_EXT( STR_PARAMSSP, "params " );
DEF_STR_EXT( STR_IUT, "iut" );

/* XML configuration */
DEF_STR_EXT( STR_XML_PROJECT, "TATPROJECT" );
DEF_STR_EXT( STR_XML_CONFIG, "CONFIG" );
DEF_STR_EXT( STR_XML_INSTANCE, "INSTANCE" );
DEF_STR_EXT( STR_XML_INSTANCES, "INSTANCES" );
DEF_STR_EXT( STR_XML_SIGNALS, "SIGNALS" );
DEF_STR_EXT( STR_XML_SIGNAL, "SIGNAL" );
DEF_STR_EXT( STR_XML_PARAMETER, "PARAMETER" );
DEF_STR_EXT( STR_XML_ATTRIBUTE, "ATTRIBUTE" );
DEF_STR_EXT( STR_XML_MACROS, "MACROS" );
DEF_STR_EXT( STR_XML_MACRO, "MACRO" );
DEF_STR_EXT( STR_XML_DATATYPE, "DATATYPE" );
DEF_STR_EXT( STR_XML_TYPEDEF, "TYPEDEF" );

DEF_STR_EXT( STR_XML_TYPE, "type" );
DEF_STR_EXT( STR_XML_DEFAULT, "default" );
DEF_STR_EXT( STR_XML_NAME, "name" );
DEF_STR_EXT( STR_XML_BASE, "base" );
DEF_STR_EXT( STR_XML_VALUE, "value" );
DEF_STR_EXT( STR_XML_FIELD, "field" );
DEF_STR_EXT( STR_XML_PRIORITY, "priority" );

DEF_STR_EXT( STR_MACRO_REPLACE, "replace" );
DEF_STR_EXT( STR_MACRO_EXTEND, "extend" );
DEF_STR_EXT( STR_MACRO_SUBSET, "subset" );
DEF_STR_EXT( STR_MACRO_REMOVE, "remove" );

DEF_STR_EXT( STR_MACRO_LIST, "list" );
DEF_STR_EXT( STR_MACRO_DERIVE, "derive" );
DEF_STR_EXT( STR_MACRO_FUNCTION, "function" );
DEF_STR_EXT( STR_MACRO_ABORT, "abort" );

DEF_STR_EXT( STR_MACRO_STRING, "string" );
DEF_STR_EXT( STR_MACRO_NUMBER, "number" );
DEF_STR_EXT( STR_MACRO_ID, "id" );
DEF_STR_EXT( STR_MACRO_UNDEFINED, "undefined" );
DEF_STR_EXT( STR_MACRO_DATATYPE, "data_Type" );
DEF_STR_EXT( STR_MACRO_VECTOR, "vector" );

DEF_STR_EXT( STR_XML_VALUELIST, "VALUELIST" );
DEF_STR_EXT( STR_XML_CONDITION, "condition" );
DEF_STR_EXT( STR_XML_PARAMSTYPE, "paramstype" );
DEF_STR_EXT( STR_MACRO_INDEX, "index" );

DEF_STR_EXT( STR_MACRO_SEQUENTIAL, "sequential" );
DEF_STR_EXT( STR_MACRO_RANDOM, "random" );
DEF_STR_EXT( STR_MACRO_RESEARCH, "research" );
DEF_STR_EXT( STR_MACRO_VALUE, "VALUE" );

DEF_STR_EXT( STR_WITH, "with" );
DEF_STR_EXT( STR_WITHOUT, "without" );
DEF_STR_EXT( STR_XML_PREFIX, "prefix" );

DEF_STR_EXT( STR_WAIT, "wait" );
DEF_STR_EXT( STR_WHEN, "when" );
DEF_STR_EXT( STR_IF, "if" );
DEF_STR_EXT( STR_WHILE, "while" );
DEF_STR_EXT( STR_ASSERT, "assert" );

DEF_STR_EXT( STR_MACRO_PRIORITY, "priority" );
DEF_STR_EXT( STR_MACRO_PRIOLIST, "priolist" );
DEF_STR_EXT( STR_DATA_DATACLASS, "dataclass" );
DEF_STR_EXT( STR_DATA_INTEGER, "integer" );
DEF_STR_EXT( STR_DATA_FLOAT, "float" );
DEF_STR_EXT( STR_DATA_SYMBOL, "symbol" );
DEF_STR_EXT( STR_DATA_STRING, "string" );
DEF_STR_EXT( STR_DATA_IDENTIFIER, "identifier" );
DEF_STR_EXT( STR_DATA_ENUM, "enum" );

DEF_STR_EXT( STR_ANNO, "ANNO" );
DEF_STR_EXT( STR_PRE, "PRE" );
DEF_STR_EXT( STR_POST, "POST" );
DEF_STR_EXT( STR_ANNOTATION, "annotation");

DEF_STR_EXT( STR_XML_AXIOM, "AXIOM");
DEF_STR_EXT( STR_XML_AXIOMS, "AXIOMS");
DEF_STR_EXT( STR_AXIOM, "axiom");
DEF_STR_EXT( STR_INITIAL, "initial");
DEF_STR_EXT( STR_CHECK, "check");

DEF_STR_EXT( STR_XML_INITIAL, "INITIAL");
DEF_STR_EXT( STR_DOTMSC, ".msc" );

DEF_STR_EXT( STR_XML_MEMBER, "MEMBER" );
DEF_STR_EXT( STR_XML_DIMENSION, "dimension" );


#endif                         /* MSCSTRS_H */
