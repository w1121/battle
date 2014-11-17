#ifndef MMSCPP_H
#define MMSCPP_H
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
* Description:  TAT Macro processing
*
* Notes:        None
*
******************************************************************************/
#include <portable.h>
#include <mscbase.h>
#include <xmlconf.h>
#include <mscmess.h>
#include <dynload.h>

extern LOGICAL TATMACRO_Extract( char *line, UINT32_T * num, char ***dest );

extern LOGICAL TATMACRO_BuildDepend( TAT_XML_CONFIG_DEF * configdata, UINT32_T ** path );

extern int MSC_MacroCalc(char * s);

extern int MSC_GetSubstNumber( const char * macro );

typedef int (* MSC_MACRO_FUNCTION)(int argc, const char *argv[], char *dest);

extern LOGICAL MSC_MacroBuild(TAT_XML_CONFIG_DEF * configdata, UINT32_T *path, UINT32_T *pathlen );

extern int MSC_MacroGenSet(TAT_XML_CONFIG_DEF * configdata, UINT32_T *path, UINT32_T pathlen, UINT32_T counter, UINT32_T *next_start_i );

extern LOGICAL MSC_ExpandIntervals( TAT_XML_CONFIG_DEF * configdata );

extern MSC_STORAGE *MSC_LoadMSCbyName( char * path, char * name );

extern LOGICAL MSC_LoadMSCinPool( char * path, char * name );

typedef struct 
{
    char *     name;           /* macro name, w/o leading '#' */
    UINT32_T        index;          /* actual index */
    char *     repl;           /* replace text */
    UINT32_T        active;         /* activity flags */
    MSC_MACRO_DATA datatype;    /* type of data */
    MSC_MACRO_FUNCTION   fpc;   /* pointer to function */
    int         vectornum;      /* -1 / or vectoring number */
    UINT32_T        activeset;      /* number of active set */
    UINT32_T        datasetcount;   /* number of conditions */
    UINT32_T       *dataset;        /* last number for iteration */
    int        *currpos;        /* current position in each list */
    UINT32_T       *overflow;       /* =1, if overflow took place on specific dataset*/
} MSC_STATIC_MACRO_INDEX;

typedef struct 
{
    char *                 name;           /* macro name, w/o leading '#' */
    UINT32_T                    index;          /* actual index in XML */
    UINT32_T                    param_number;   /* number of parameters */
    MSC_MACRO_FUNCTION      fpc;            /* pointer to function */
    MSC_MACRO_DATA          datatype;       /* type of data */
} MSC_DYNAMIC_MACRO_INDEX;

typedef struct
{
    MSC_STORAGE        *M;
    char *                 path;
    char *                 name;
    char *                 outname;
    char *                 full_name;
    int                     initial;
    int                     in_process;
}
MSC_MSC_POOL;

extern UINT32_T              MSC_PoolCount;
extern MSC_MSC_POOL    *MSC_Pool;
extern UINT32_T  MSC_StaticMacroTableSize;  
extern MSC_STATIC_MACRO_INDEX * MSC_StaticMacroTable;
extern UINT32_T  MSC_DynMacroTableSize;
extern MSC_DYNAMIC_MACRO_INDEX * MSC_DynMacroTable;
extern UINT32_T MSC_LibSetSize;
extern DYNLIB_HANDLE * MSC_LibSet;
extern UINT32_T                    MSC_OutFlags;
extern  UINT32_T                MSC_TransformFlags;


extern int MSC_FlagTextInput;
extern int MSC_MacroRandomFlag;

extern UINT32_T  MSC_ActiveTableSize;  
extern MSC_STATIC_MACRO_INDEX * MSC_ActiveTable;

extern char            *MSC_ConfigXML;
extern int              flag_ExcTime;
extern int              flag_ForceSync;
extern int              flag_IgnoreCond;
extern int              flag_RefSync;
extern int              flag_Globm;
extern int              flag_Count;
extern int              flag_Excount;
extern int              flag_Debug;
extern int              flag_SaveZeroLoop;
extern int              tat_InlineLoops;
extern int              tat_UnrollLoops;
extern int              start_Number;
extern int              quantity;

extern int              flag_InlineAllRef;
extern int              flag_SkipFuncRef;
extern int              flag_PlainDir;
extern int              seed_Number;
extern int              flag_IgnoreNotFoundRef;
extern char            *output;
extern char            *listf;
extern FILE                   *lst;

extern int              flag_NoRefSubst;
extern char            *flag_LoadVar;
extern FILE            *lvv;
extern UINT32_T             incl_MacroCount;
extern UINT32_T             excl_MacroCount;


extern void MSC_AddUnknownMacro( char * x);

extern int MSC_GetSubstNumberLocal( const char * macro, UINT32_T  MSC_StaticMacroTableSize,
                                      MSC_STATIC_MACRO_INDEX * MSC_StaticMacroTable );

extern void MSC_SetCurrMscName(char * curr_mscname);

extern char * MSC_MacroEval(char * s, char * d, int *pos, int BCount, MSC_STATIC_MACRO_INDEX * Subst);

extern char * MSC_GetMacroExpression( char * s, char * name, char * full );

extern LOGICAL MSC_MacroProcess( int i, int slen, MSC_STATIC_MACRO_INDEX * x );

extern int MSC_GetMacroValues( FILE *lvv );

extern LOGICAL MSC_MacroProcessText( int i, int slen, MSC_STATIC_MACRO_INDEX * x );

extern char * MSC_MacroProcessString( char * d, char * s );

extern int MSC_CompStaticData( const void *x1, const void *x2 );

extern int MSC_GetMacroValues( FILE *lvv );


DEF_MSG_EXT( MSG_MMSC_START_LARGE, "ERROR: mmsc: start number is too large\n" );
DEF_MSG_EXT( MSG_MMSC_UNKNOWN_MACRO, "WARNING: mmsc: unknown macro '%s' in msc '%s'\n" );
DEF_MSG_EXT( MSG_MMSC_UNKNOWN_MACRO_CMD, "WARNING: mmsc: unknown macro '%s' in command-line\n" );
DEF_MSG_EXT( MSG_MMSC_UNKNOWN_INP_MACRO, "unknown macro '%s'" );
DEF_MSG_EXT( MSG_CONFIG_CAN_GENERATED_u, "REMARK: config: %u file(s) can be generated\n" );
DEF_MSG_EXT( MSG_MMSC_CANT_COMPUTE_ss, "WARNING: semantic: file '%s': can't compute value for macro '%s'\n");
DEF_MSG_EXT( MSG_MMSC_RETURNS_ERROR_sss, "ERROR: xml: file '%s': function '%s' for macro '%s' reports error\n");
DEF_MSG_EXT( MSG_MMSC_INVALID_DATATYPE_ssss, "ERROR: semantic: file '%s': function '%s' for macro '%s' returns invalid data '%s'\n");
DEF_MSG_EXT( MSG_MMSC_EXPR_SYNTAX_ss, "ERROR: syntax: file '%s': can't parse '%s'\n");
DEF_MSG_EXT( MSG_MMSC_SUSP_SYNTAX_ss, "WARNING: syntax: file '%s': suspicious macro syntax '%s'\n");
DEF_MSG_EXT( MSG_MMSC_BRACE_EXPECTED_ss, "ERROR: syntax: file '%s': '(' expected in '%s'\n" );
DEF_MSG_EXT( MSG_MMSC_CANT_PARSE_ARG_sus, "ERROR: syntax: file '%s': can't parse argument %u from '%s'\n");
DEF_MSG_EXT( MSG_MMSC_EXPECTED_ARGS_sus, "ERROR: semantic: file '%s': expected %u arguments for '%s'\n");
DEF_MSG_EXT( MSG_NO_MACRO_DEFINED, "WARNING: xml: no macros defined\n" );





#endif                         /* MMSCPP_H */
