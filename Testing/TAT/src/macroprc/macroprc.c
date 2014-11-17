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
* Functions:    MSC_StartMacroGeneration
*               MSC_ProcessCommandLineMMSC
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  Macro processing unit
*
* Notes:        None
*
******************************************************************************/
#include <portable.h>
#include <mscbase.h>
#include <mscparse.h>
#include <mscproc.h>
#include <qmemory.h>
#include <fileproc.h>
#include <strproc.h>
#include <outmess.h>
#include <mscmess.h>
#include <cmdline.h>
#include <mscgen.h>
#include <mscstrs.h>
#include <tatgdata.h>
#include <flowgen.h>
#include <xmlconf.h>
#include <cfgtypes.h>
#include <confproc.h>
#include "macroprc.h"

static                  DEF_MSG( tat_ProgramName, "Macro Processor" );

static char           **includes = NULL;
char                   *MSC_ConfigXML = NULL;
static char            *configname = NULL;
static char            *output_FileName = NULL;
UINT32_T                    MSC_LibSetSize = 0;
DYNLIB_HANDLE        * MSC_LibSet = NULL;
static char           **tat_LibNames = NULL;



/* MSC pool handling */
UINT32_T              MSC_PoolCount = 0;
MSC_MSC_POOL    *MSC_Pool = NULL;

/* by default - current directory */
static char             tat_MscOutPath[TAT_MAX_FNAME_LEN] = "./";
static int              tat_MscOutPathLength = 2;
UINT32_T                    MSC_OutFlags;

UINT32_T                    MSC_TransformFlags = MSC_TRANS_USE_SYNC | MSC_TRANS_USE_COND;

UINT32_T                    tat_MscCount;
char                  **MSC_Name = NULL;
char                  **MSC_Path = NULL;
char                  **MSC_Files = NULL;
int                     MSC_FlagTextInput = 0;
int              flag_ExcTime = 0;
int              flag_ForceSync = 0;
int              flag_IgnoreCond = 0;
int              flag_RefSync = 0;
int              flag_Globm = 0;
int              flag_Count = 0;
int              flag_Excount = 0;
int              flag_Debug = 0;
int              flag_SaveZeroLoop = 0;
int              tat_InlineLoops = 0;
int              tat_UnrollLoops = 0;
int              start_Number = 0;
int              quantity = -1;

int              flag_InlineAllRef = 0;
int              flag_SkipFuncRef = 0;
int              flag_PlainDir = 0;
int              seed_Number = 0;
int              flag_IgnoreNotFoundRef = 0;
char            *output = NULL;
char            *listf = NULL;
FILE                   *lst = NULL;

int              flag_NoRefSubst = 0;
char            *flag_LoadVar = NULL;
FILE            *lvv = NULL;
UINT32_T             incl_MacroCount = 0;
UINT32_T             excl_MacroCount = 0;

static char           **incl_MSC_MacroNames = NULL;
static char           **excl_MSC_MacroNames = NULL;

int                     MSC_MacroRandomFlag = 0;

static char           **unknown_Macro = NULL;
static char           **unknown_SourceMSC = NULL;
static UINT32_T              unknown_MacroCount = 0;


static TAT_XML_CONFIG_DEF   *configdata = NULL;

UINT32_T  MSC_StaticMacroTableSize;  
MSC_STATIC_MACRO_INDEX * MSC_StaticMacroTable;

UINT32_T  MSC_ActiveTableSize;  
MSC_STATIC_MACRO_INDEX * MSC_ActiveTable;

UINT32_T  MSC_DynMacroTableSize;
MSC_DYNAMIC_MACRO_INDEX * MSC_DynMacroTable;

/***************************************************************************
 * Function : MSC_GetSubstNumber
 *
 * Description: return macro ID
 *
 * Returns: number/ -1
 *
 * Notes: None
 *
 **************************************************************************/
int MSC_GetSubstNumber( const char * macro )
{
    char                    key[TAT_MAX_ID_LEN];
    UINT32_T                    lo, hi, mid, half, num;
    int                     result;

    if ( !macro || !MSC_StaticMacroTable )
        return -1;

    StrUpCopy( key, macro );

    lo = 0;
    num = MSC_StaticMacroTableSize;
    hi = num - 1;

    while ( lo <= hi )
    {
        if ( half = ( num >> 1 ) )
        {
            mid = lo + ( ( num & 1 ) ? half : ( half - 1 ) );

            if ( !( result = strcmp( key, MSC_StaticMacroTable[mid].name ) ) )
                return ( mid );
            else if ( result < 0 )
            {
                hi = mid - 1;
                num = ( num & 1 ) ? half : half - 1;
            }
            else
            {
                lo = mid + 1;
                num = half;
            }
        }
        else if ( num )
            return ( strcmp( key, MSC_StaticMacroTable[lo].name ) ? -1 : lo );
        else
            break;
    }

    return -1;
}

/***************************************************************************
* Function : MSC_GetSubstNumber
*
* Description: return macro ID
*
* Returns: number/ -1
*
* Notes: None
*
**************************************************************************/
int MSC_GetSubstNumberLocal( const char * macro, UINT32_T  MSC_StaticMacroTableSize,
                                MSC_STATIC_MACRO_INDEX * MSC_StaticMacroTable )
{
    char                    key[TAT_MAX_ID_LEN];
    UINT32_T                    lo, hi, mid, half, num;
    int                     result;

    if ( !macro || !MSC_StaticMacroTable )
        return -1;

    StrUpCopy( key, macro );

    lo = 0;
    num = MSC_StaticMacroTableSize;
    hi = num - 1;

    while ( lo <= hi )
    {
        if ( half = ( num >> 1 ) )
        {
            mid = lo + ( ( num & 1 ) ? half : ( half - 1 ) );

            if ( !( result = strcmp( key, MSC_StaticMacroTable[mid].name ) ) )
                return ( mid );
            else if ( result < 0 )
            {
                hi = mid - 1;
                num = ( num & 1 ) ? half : half - 1;
            }
            else
            {
                lo = mid + 1;
                num = half;
            }
        }
        else if ( num )
            return ( strcmp( key, MSC_StaticMacroTable[lo].name ) ? -1 : lo );
        else
            break;
    }

    return -1;
}

/***************************************************************************
 * Function : MSC_GetFunctionNumber
 *
 * Description: return function ID
 *
 * Returns: number/ -1 if not found
 *
 * Notes: None
 *
 **************************************************************************/
int MSC_GetFunctionNumber( const char * macro )
{
    char                    key[TAT_MAX_ID_LEN];
    UINT32_T                    lo, hi, mid, half, num;
    int                     result;

    if ( !macro || !MSC_DynMacroTable )
        return -1;

    StrUpCopy( key, macro );

    lo = 0;
    num = MSC_DynMacroTableSize;
    hi = num - 1;

    while ( lo <= hi )
    {
        if ( half = ( num >> 1 ) )
        {
            mid = lo + ( ( num & 1 ) ? half : ( half - 1 ) );

            if ( !( result = strcmp( key, MSC_DynMacroTable[mid].name ) ) )
                return ( mid );
            else if ( result < 0 )
            {
                hi = mid - 1;
                num = ( num & 1 ) ? half : half - 1;
            }
            else
            {
                lo = mid + 1;
                num = half;
            }
        }
        else if ( num )
            return ( strcmp( key, MSC_DynMacroTable[lo].name ) ? -1 : lo );
        else
            break;
    }
    return -1;
}


/***************************************************************************
 * Function : MSC_GetSubst
 *
 * Description: return value of substitution by name
 *
 * Returns: string/ ""
 *
 * Notes: None
 *
 **************************************************************************/
char * MSC_GetSubst( const char * macro )
{
    int                     num;
    num = MSC_GetSubstNumber( macro );
    return ( num != -1 ) ? MSC_StaticMacroTable[num].repl : "";
}

static char tat_CurrMscName[TAT_MAX_FNAME_LEN];

/***************************************************************************
 * Function : MSC_SetCurrMscName
 *
 * Description: store current MSC name for further diagnostic messages
 *
 * Returns: none
 *
 * Notes: None
 *
 **************************************************************************/
void MSC_SetCurrMscName(char * curr_mscname)
{
    if (curr_mscname)
    {
        strcpy(tat_CurrMscName, curr_mscname);
    }
    else
        tat_CurrMscName[0]=0;;
}

/***************************************************************************
 * Function : MSC_MacroEval
 *
 * Description: calculate macro substituions
 *
 * Returns: d/NULL
 *
 * Notes: None
 *
 **************************************************************************/
char * MSC_MacroEval(char * s, char * d, int *pos, int BCount, MSC_STATIC_MACRO_INDEX * Subst)
{
    int     i = *pos, l;
    UINT32_T    u, k;
    char    L[TAT_MAX_LINE_LEN];
    char    T[TAT_MAX_ID_LEN];
    char   **par, *buff;
    char   *r;

    r = d;
    while (s[i] && SymIsSpace(s[i])) i++;
    *pos=i;

        if (!MSC_GetMacroExpression(&s[i],T,L))
        {
            MSG_PrintError(MSG_MMSC_EXPR_SYNTAX_ss, tat_CurrMscName, &s[i]);
            return NULL;
        };

    if (L[0] != '#')
    { /* this is not macro */
        strcat(d, L);
        *pos += strlen(L);
        return r;
    };

    k = strlen(T);  /* length of macro name */
    u = strlen(L);  /* length of full expression */

    i += k;         /* update position for parsing */
    *pos += u;      /* update external position */

    /* try variable */
    l = MSC_GetSubstNumberLocal(&T[1], BCount, Subst);

    if (l!=-1)
    {
        /* correctly handle case when '(' ')' used with non-function macro */
        if (k != u)
        {
            MSG_PrintError(MSG_MMSC_SUSP_SYNTAX_ss, tat_CurrMscName, L);
            *pos -= u-k;
        };
        
        strcat(d, Subst[l].repl);
        return r;
    }

    /* try function number */
    l = MSC_GetFunctionNumber(&T[1]);

    /* if found function with this name */
    if (l!=-1)
    {
        if (k == u)
        {
            MSG_PrintError(MSG_MMSC_BRACE_EXPECTED_ss, tat_CurrMscName, L);
            return NULL;
        };
        while (s[i] && SymIsSpace(s[i])) i++;
        if (s[i] != '(')
        {
            MSG_PrintError(MSG_MMSC_BRACE_EXPECTED_ss, tat_CurrMscName, L);
            return NULL;
        };
        i++;
        /* allocate memory for parameters */
        par = MEM_Allocate((MSC_DynMacroTable[l].param_number+1)*sizeof(par[0]));
		buff = MEM_Allocate(TAT_MAX_LINE_LEN);

        for (u = 0; u < MSC_DynMacroTable[l].param_number; u++)
        {
            par[u] = MEM_Allocate(TAT_MAX_LINE_LEN);
            par[u][0] = 0;  /* clean allocated memory */
			buff[0]=0;

            if (!MSC_MacroEval(s, buff, &i, BCount,Subst))
            {
                MSG_PrintError(MSG_MMSC_CANT_PARSE_ARG_sus, tat_CurrMscName, u + 1, L);
                MEM_FreeStrArray(MSC_DynMacroTable[l].param_number, par);
                return NULL;
            }
			/* expand nested macros */
			MSC_MacroProcessString(par[u], buff);

            while (s[i] && SymIsSpace(s[i])) i++;  /* skip space before braces */

            if (s[i] == ')' && (u < (MSC_DynMacroTable[l].param_number - 1)))
            {
                MSG_PrintError(MSG_MMSC_EXPECTED_ARGS_sus,
                    tat_CurrMscName, MSC_DynMacroTable[l].param_number, L);
                MEM_FreeStrArray(MSC_DynMacroTable[l].param_number+1, par);
                return NULL;
            };

            if ((s[i] == ',') && (u == (MSC_DynMacroTable[l].param_number - 1)))
            {
                MSG_PrintError(MSG_MMSC_EXPECTED_ARGS_sus,
                    tat_CurrMscName, MSC_DynMacroTable[l].param_number, L);
                MEM_FreeStrArray(MSC_DynMacroTable[l].param_number+1, par);
                return NULL;
            };

            i++;
        };
		MEM_Free(buff);
        par[MSC_DynMacroTable[l].param_number] = MEM_Allocate(TAT_MAX_LINE_LEN);
        
        if (MSC_DynMacroTable[l].fpc)
        {
            /* if macro is user's function,  call user's function */
            if (!MSC_DynMacroTable[l].fpc(MSC_DynMacroTable[l].param_number, par, par[MSC_DynMacroTable[l].param_number]))
            {
                MSG_PrintError(MSG_MMSC_RETURNS_ERROR_sss, MSC_ConfigXML,
                    configdata->macro[MSC_DynMacroTable[l].index].attr[0], configdata->macro[MSC_DynMacroTable[l].index].name);
                MEM_FreeStrArray(MSC_DynMacroTable[l].param_number, par);
                return NULL;
            }

            /* check correctness of user's function result */
            if ( !CFG_CheckMacroData( configdata->macro[MSC_DynMacroTable[l].index].data_Type,
                par[MSC_DynMacroTable[l].param_number] ) )
            {
                MSG_PrintError(MSG_MMSC_INVALID_DATATYPE_ssss, MSC_ConfigXML,
                    configdata->macro[MSC_DynMacroTable[l].index].attr[0],
                    configdata->macro[MSC_DynMacroTable[l].index].name,
                    par[MSC_DynMacroTable[l].param_number]);
                MEM_FreeStrArray(MSC_DynMacroTable[l].param_number, par);
                return NULL;
            }

        }
        else
        {
            UINT32_T dataset, p;
            int res;

            p = MSC_DynMacroTable[l].index;

            /* if macro is index */
            for (dataset=0; dataset<configdata->macro[p].data_Count; dataset++)
            {
                if (!configdata->macro[p].data[dataset].condition)
                {
                    break;
                }
                res = MSC_MacroCalc(configdata->macro[p].data[dataset].condition);
                if (res == -1)
                    return NULL;
                if (res == 1)
                    break;
            }

            /* if no dataset is active */
            if (dataset >= configdata->macro[p].data_Count)
            {
                MSG_PrintError("ERROR: semantic: file '%s': no data set is active for 'index' macro '%s'\n",
                    tat_CurrMscName,  L);
                MEM_FreeStrArray(MSC_DynMacroTable[l].param_number+1, par);
                return NULL;
            }
            
            res = atoi(par[0]);
            if (par[0][0]=='$')
            {
                strcpy(par[MSC_DynMacroTable[l].param_number], L);
            }
            else
            {
                if (!StrIsNaturalNumber(par[0]) || res>=configdata->macro[p].data[dataset].data_Count)
                {
                    MSG_PrintError("ERROR: semantic: file '%s': invalid index '%d' for macro '%s'\n",
                        tat_CurrMscName, res, L);
                    MEM_FreeStrArray(MSC_DynMacroTable[l].param_number, par);
                    return NULL;
                }
                strcpy(par[MSC_DynMacroTable[l].param_number], configdata->macro[p].data[dataset].data[res]);
            }
        }

        /* add generated value to the end of string */
        strcat(d, par[MSC_DynMacroTable[l].param_number]);

        /* free temporarily allocated memory */
        MEM_FreeStrArray(MSC_DynMacroTable[l].param_number+1, par);

        return r;
    }

    MSC_AddUnknownMacro(T);
    strcat(d, L);
    return r;
}


/***************************************************************************
* Function : MSC_AddUnknownMacro
*
* Description: Add macro name to list of unknown macro
*
* Returns: None
*
* Notes: None
*
**************************************************************************/
void MSC_AddUnknownMacro( char * x)
{
    int                     i;

    /* check for disabled macro */
    i = MSC_GetSubstNumber( x );
    if ( i != -1 && !MSC_StaticMacroTable[i].active )
        return;

    /* look for already known macro */
    if ( unknown_MacroCount )
    {
        for ( i = 0; i < unknown_MacroCount; i++ )
        {
            if ( !stricmp( unknown_Macro[i], x ) && !stricmp( tat_CurrMscName, unknown_SourceMSC[i] ) )
                return;
        }
    }
    /* add new macro name */
    unknown_Macro = MEM_AddObject( unknown_Macro, ( unknown_MacroCount + 1 ), sizeof ( char * ) );
    unknown_SourceMSC = MEM_AddObject( unknown_SourceMSC, ( unknown_MacroCount + 1 ), sizeof ( char * ) );
    unknown_Macro[unknown_MacroCount] = MEM_CopyStr( x );
    unknown_SourceMSC[unknown_MacroCount] = MEM_CopyStr( tat_CurrMscName );
    unknown_MacroCount++;
}


/***************************************************************************
* Function : MSC_GenerateNewName
*
* Description: generate new name for MSC 'i' from pool using MSC_MacroGenCounter
*
* Returns: dest
*
* Notes: None
*
**************************************************************************/
char * MSC_GenerateNewName( int i, char * dest, int counter )
{
    int                     j;
    char                   *x;
    char                    fname[TAT_MAX_FNAME_LEN];
    char                    ext[TAT_MAX_FNAME_LEN];

    x = dest;

    if ( tat_MscOutPath && tat_MscOutPath[0] != 0 )
    {                           /* tat_MscOutPath should finished with slash */
        strcpy( dest, tat_MscOutPath );
        dest += tat_MscOutPathLength;
    };

    /* mangle directory if necessary */
    if ( MSC_Pool[i].path && MSC_Pool[i].path[0] != 0 && !flag_PlainDir )
    {
        j = 0;

        if ( ( MSC_Pool[i].path[0] == '.' ) &&
            ( MSC_Pool[i].path[1] == '/' || MSC_Pool[i].path[1] == '\\' ) )
            j = 2;

        while ( MSC_Pool[i].path[j] )
        {
            switch ( MSC_Pool[i].path[j] )
            {

            case '\\':
            case '/':
            case ':':
            case '.':
                {
                    break;
                };

            default:
                {
                    *dest++ = MSC_Pool[i].path[j];
                    break;
                };
            };
            j++;
        };
    };

    FP_SplitPath( MSC_Pool[i].name, NULL, fname, ext );
    strcpy( dest, fname );
    dest += strlen( dest );
    sprintf( dest, "N%u", counter );
    dest += strlen( dest );

    if ( ext[0] == 0 )
    {
        strcat( dest, STR_DOTMPR );
    }
    else
    {
        strcpy( dest, ext );
    };
    return x;
}


CMD_LINE                cmd[] = {
    {"", CMD_GROUP_START, NULL, NULL, "Input file options:"},
    {"-incl", CMD_FILE_LIST, &MSC_LibSetSize, &tat_LibNames, "set dynamic library files"},
    {"-config", CMD_FILE_NAME, NULL, &MSC_ConfigXML, "set configuration file"},
    {"-cname", CMD_STRING, NULL, &configname, "set configuration name (default - \"default\")"},
    {"-m", CMD_FILE_LIST, &tat_MscCount, &MSC_Files, "set files to process"},
    {"-text", CMD_BOOLEAN, NULL, &MSC_FlagTextInput, "treat input files as text"},
    {"-ignorenf", CMD_BOOLEAN, NULL, &flag_IgnoreNotFoundRef, "ignore references to non-existing files"},

    {"", CMD_GROUP_START, NULL, NULL, "Value generation options:"},
    {"-count", CMD_BOOLEAN, NULL, &flag_Count, "count number of permutations"},
    {"-debug", CMD_BOOLEAN, NULL, &flag_Debug, "generate set and print it"},
    {"-lv", CMD_FILE_NAME, NULL, &flag_LoadVar, "load values of variables from file"},
    {"-n", CMD_INTEGER, NULL, &quantity, "maximal number of tests to generate"},
    {"-rnd", CMD_BOOLEAN, NULL, &MSC_MacroRandomFlag, "generate random permutations of macro"},
    {"-s", CMD_NATURAL, NULL, &start_Number, "set start test number"},
    {"-seed", CMD_INTEGER, NULL, &seed_Number, "set seed number for random generator"},

    {"", CMD_GROUP_START, NULL, NULL, "Output file options:"},
    {"-c", CMD_BOOLEAN, NULL, &flag_IgnoreCond, "ignore conditional field in reference_Events specification"},
    {"-il", CMD_BOOLEAN, NULL, &tat_InlineLoops, "inline loops with counter equal to <1> into plain code"},
    {"-ul", CMD_BOOLEAN, NULL, &tat_UnrollLoops, "unroll loops with constant number of iterations"},
    {"-f", CMD_BOOLEAN, NULL, &flag_ForceSync, "force output synchronization condition"},
    {"-fr", CMD_BOOLEAN, NULL, &flag_SkipFuncRef,  "do not substitute references started with '$'"},
    {"-glob", CMD_BOOLEAN, NULL, &flag_Globm, "macros are global for included MSC"},
    {"-iar", CMD_BOOLEAN, NULL, &flag_InlineAllRef, "substitute all references"},
    {"-l", CMD_BOOLEAN, NULL, &flag_SaveZeroLoop, "save loops with zero iterator"},
    {"-list", CMD_FILE_NAME, NULL, &listf, "name of file containing list of generated files"},
    {"-notime", CMD_BOOLEAN, NULL, &flag_ExcTime, "exclude time specifications in generated files"},
    {"-nrf", CMD_BOOLEAN, NULL, &flag_NoRefSubst, "do not expand any references at all"},
    {"-o", CMD_PATH_NAME, NULL, &output, "set output directory"},
    {"-plaindir", CMD_BOOLEAN, NULL, &flag_PlainDir, "do not append directory to generated file names"},
    {"-rf", CMD_BOOLEAN, NULL, &flag_RefSync, "frame substitued references in conditions"},
    {"-use", CMD_STRING_LIST, &incl_MacroCount, &incl_MSC_MacroNames, "set active macro names"},
    {"-excl", CMD_STRING_LIST, &excl_MacroCount, &excl_MSC_MacroNames, "set excluded macro names"}

};

#define cmd_size (sizeof(cmd)/sizeof(cmd[0]))



/***************************************************************************
* Function : main
*
* Description: Invoke all activities
*
* Returns: exit code
*
* Notes: None
*
**************************************************************************/
int main( int argc, char *argv[] )
{
    int                     ExitCode = SYS_NORMAL_EXIT_CODE;
    TAT_XML_CONFIG          xmldata;
    UINT32_T                   *path = NULL;
    UINT32_T                    pathlen = 0;
    UINT32_T                    counter, res, i;
    char                    extname[TAT_MAX_FNAME_LEN], tempname[TAT_MAX_FNAME_LEN];
    UINT32_T                    next_start_i = 0;


    /*****************************************************************************************/

    MSG_Copyright( tat_ProgramName, argv[0] );
    memset( &xmldata, 0, sizeof ( xmldata ) );

    /* check command-line */

    if ( ( argc == 1 ) || !CMD_Parse( cmd_size, ( CMD_LINE * ) & cmd[0], argc, argv ) )
    {
        /* output help message, if errors encounted */
        CMD_PrintHelp( cmd_size, ( CMD_LINE * ) & cmd[0] );
        return argc != 1;
    }

    /* register built-in parsers */
    MSC_RegisterFormat(STR_DOTMPR,  MSC_Load,  MSC_Save);
    MSC_RegisterFormat(STR_DOTFG,  FG_Load,  FG_Save);

    if ( !MSC_ConfigXML )
    {
        MSG_PrintError( MSG_INVALID_SWITCH_VALUE_s, "-config" );
        ExitCode = SYS_ERROR_EXIT_CODE;
        goto PExit;
    }

    /* load dynamic libraries */
    MSC_LibSet = MEM_Allocate(MSC_LibSetSize*sizeof(MSC_LibSet[0]));
    for (i=0; i<MSC_LibSetSize; i++)
    {
        MSC_LibSet[i] = Dyn_LoadLibrary( tat_LibNames[i] );
        if (!MSC_LibSet[i])
        {
            ExitCode = SYS_ERROR_EXIT_CODE;
            goto PExit;
        }
    }

    /* read configuration from XML */
    if ( !TAT_ReadXML( MSC_ConfigXML, &xmldata ) )
    {
        ExitCode = SYS_ERROR_EXIT_CODE;
        goto PExit;
    }

    configdata = TAT_BuildConfig( ( configname ) ? configname : STR_XML_DEFAULT, &xmldata );

    if ( !configdata )
    {
        ExitCode = SYS_ERROR_EXIT_CODE;
        goto PExit;
    }

    if ( configdata->macro_Count == 0 )
    {
        MSG_PrintError( MSG_NO_MACRO_DEFINED );
    }

    /* set this configuration as default */
    CFG_SetDefaultConfig( configdata );

    if (configdata->macro_Count)
    {
        /* build only if there is something to build */
        if ( !MSC_ExpandIntervals( configdata ) )
        {
            ExitCode = SYS_ERROR_EXIT_CODE;
            goto PExit;
        };

        if ( !TATMACRO_BuildDepend( configdata, &path ) || !path )
        {
            ExitCode = SYS_ERROR_EXIT_CODE;
            goto PExit;
        };

        if (!MSC_MacroBuild(configdata, path, &pathlen ))    
        {
            ExitCode = SYS_ERROR_EXIT_CODE;
            goto PExit;
        };
    }

    if ( quantity < -1 )
    {
        MSG_PrintError( MSG_INVALID_SWITCH_VALUE_s, "-n" );
        return SYS_ERROR_EXIT_CODE;
    }

    if ( seed_Number && !MSC_MacroRandomFlag )
    {
        MSG_PrintError( MSG_WARNING_SWITCH_USAGE_sss, "-seed", STR_WITHOUT, "-rnd" );
    }

    if ( flag_LoadVar && MSC_MacroRandomFlag )
    {
        MSG_PrintError( MSG_WARNING_SWITCH_USAGE_sss, "-lv", STR_WITH, "-rnd" );
        MEM_Free( flag_LoadVar );
        flag_LoadVar = NULL;
    }

    if ( flag_ForceSync && flag_RefSync )
    {
        MSG_PrintError( MSG_WARNING_SWITCH_USAGE_sss, "-rf", STR_WITH, "-f" );
        flag_RefSync = 0;
    }
    if ( quantity == -1 && MSC_MacroRandomFlag )
    {
        MSG_PrintError( MSG_INVALID_SWITCH_USAGE_ss, "-rnd", "-n" );
        return SYS_ERROR_EXIT_CODE;
    }

    if ( output )
    {
        if ( !FP_MakeDir( output ) )
        {
            return SYS_ERROR_EXIT_CODE;
        }

        strcpy( tat_MscOutPath, output );

        tat_MscOutPathLength = strlen( tat_MscOutPath );

        if ( tat_MscOutPathLength && tat_MscOutPath[tat_MscOutPathLength - 1] != '\\' &&
            tat_MscOutPath[tat_MscOutPathLength - 1] != '/' )
        {
#ifdef __win32__
            tat_MscOutPath[tat_MscOutPathLength] = '\\';
#else

            tat_MscOutPath[tat_MscOutPathLength] = '/';
#endif

            tat_MscOutPath[tat_MscOutPathLength + 1] = 0;
            tat_MscOutPathLength++;
        };
    }

    if ( seed_Number )
    {
        srand( seed_Number );
    }

    if ( flag_ForceSync )
    {
        MSC_TransformFlags |= MSC_TRANS_GLOBAL_SYNC;
    }

    MSC_TransformFlags &= ~MSC_TRANS_USE_SYNC;

    if ( flag_IgnoreCond )
    {
        MSC_TransformFlags &= ~MSC_TRANS_USE_COND;
    }

    if ( tat_UnrollLoops )
    {
        MSC_TransformFlags |= MSC_TRANS_UNROLL_LOOP;
    }

    if ( tat_InlineLoops )
    {
        MSC_TransformFlags |= MSC_TRANS_INLINE_LOOP;
    }


    /* if include list is not defined, mark all macro active */
    if ( !incl_MacroCount )
    {
        for ( i = 0; i < MSC_StaticMacroTableSize; i++ )
            MSC_StaticMacroTable[i].active = 1;
    }
    else
    {
        for ( i = 0; i < incl_MacroCount; i++ )
        {
            res = MSC_GetSubstNumber( incl_MSC_MacroNames[i] );
            if (res == -1)
            {
                MSG_PrintError( MSG_MMSC_UNKNOWN_MACRO_CMD, incl_MSC_MacroNames[i] );
                ExitCode = SYS_ERROR_EXIT_CODE;
                goto PExit;
            }
            MSC_StaticMacroTable[res].active = 1;
        }
    }

    if ( excl_MacroCount )
    {

        for ( i = 0; i < excl_MacroCount; i++ )
        {
            res = MSC_GetSubstNumber( excl_MSC_MacroNames[i] );
            if (res == -1)
            {
                MSG_PrintError( MSG_MMSC_UNKNOWN_MACRO_CMD, excl_MSC_MacroNames[i] );
                ExitCode = SYS_ERROR_EXIT_CODE;
                goto PExit;
            }
            MSC_StaticMacroTable[res].active = 0;
        }
    }

    if ( flag_Debug )
    {
        counter = 0;
        res = MSC_MacroGenSet(configdata, path, pathlen, counter, &next_start_i);
        while ( res !=2 && res!=-1 && ( counter < (UINT32_T)quantity || quantity == -1 ))
        {
            if (!res)
            {
                ExitCode = SYS_ERROR_EXIT_CODE;
                goto PExit;
            }
            for (i=0; i<MSC_StaticMacroTableSize; i++)
            {
                printf("#%s=%s \t", MSC_StaticMacroTable[i].name, MSC_StaticMacroTable[i].repl );
            }
            puts("");

            counter++;
            res = MSC_MacroGenSet(configdata, path, pathlen, counter, &next_start_i);
        };

        printf( MSG_CONFIG_CAN_GENERATED_u, counter );
        goto PExit;
    }

    if (flag_Count || flag_Excount)
    {
        counter = 0;

        res = MSC_MacroGenSet(configdata, path, pathlen, counter, &next_start_i);
        while ( res !=2 && res!=-1 && ( counter < (UINT32_T)quantity || quantity == -1 ) )
        {
            if (!res)
            {
                ExitCode = SYS_ERROR_EXIT_CODE;
                goto PExit;
            }
            counter++;
            res = MSC_MacroGenSet(configdata, path, pathlen, counter, &next_start_i);
        };

        printf( MSG_CONFIG_CAN_GENERATED_u, counter );
        goto PExit;
    }

    if ( tat_MscCount == 0 )
    {
        MSG_PrintError( MSG_INVALID_SWITCH_VALUE_s, "-m" );
        ExitCode = SYS_ERROR_EXIT_CODE;
        goto PExit;
    }


    MSC_Path = MEM_Allocate( tat_MscCount * sizeof ( MSC_Path[0] ) );
    MSC_Name = MEM_Allocate( tat_MscCount * sizeof ( MSC_Name[0] ) );

    for ( i = 0; i < tat_MscCount; i++ )
    {
        MSC_Path[i] = MEM_Allocate( TAT_MAX_FNAME_LEN );
        MSC_Name[i] = MEM_Allocate( TAT_MAX_FNAME_LEN );
        FP_SplitPath( MSC_Files[i], MSC_Path[i], MSC_Name[i], extname );
        strcat( MSC_Name[i], extname );
    }

    /* read all files */
    for ( i = 0; i < tat_MscCount; i++ )
    {
        if ( !MSC_LoadMSCinPool( MSC_Path[i], MSC_Name[i] ) )
        {
            ExitCode = SYS_ERROR_EXIT_CODE;
            goto PExit;
        }
        MSC_Pool[MSC_PoolCount-1].initial = 1;
        MSC_Pool[MSC_PoolCount-1].in_process = 0;
    }

    counter = 0;

    MSC_OutFlags = ( flag_ExcTime ) ? MSC_MPR_STD_OUT : MSC_MPR_TIMED_OUT;
    MSC_OutFlags |= MSC_MPR_OUTPUT_SYNC;

    if (listf)
    {
        lst = FP_OpenFile(listf,"w");
        if (!lst)
        {
            ExitCode = SYS_ERROR_EXIT_CODE;
            goto PExit;
        }
    }

    if (flag_LoadVar)
    {
        lvv = FP_OpenFile(flag_LoadVar, "r");
        if (!lvv)
        {
            ExitCode = SYS_ERROR_EXIT_CODE;
            goto PExit;
        }
        MSG_PushProcessedFile(flag_LoadVar);
    }

    /* process -s switch */
    if (start_Number)
    {
        res = 1;
        do
        {
            if (flag_LoadVar)
            {
                res = MSC_GetMacroValues(lvv);
            }
            else
            {
                res = MSC_MacroGenSet(configdata, path, pathlen, counter, &next_start_i);
            }
            if (!res)
            {
                ExitCode = SYS_ERROR_EXIT_CODE;
                goto PExit;
            }
            counter++;
        } while ( res !=2 && ( (counter < (UINT32_T) start_Number) ) );

        if (res==2)
        {
            MSG_PrintError(MSG_INVALID_SWITCH_VALUE_s, "-s");
            ExitCode = SYS_ERROR_EXIT_CODE;
            goto PExit;
        }

        if (quantity!=-1)
        {
            quantity+=counter;
            /* handle overflow */
            if (quantity<counter) quantity=-1;
        }
    }

    if (flag_LoadVar)
    {
        res = MSC_GetMacroValues(lvv);
        while ( res !=2 && ( (counter < (UINT32_T) quantity) || quantity == -1 ) )
        {
            if (!res)
            {
                ExitCode = SYS_ERROR_EXIT_CODE;
                goto PExit;
            }

            /* generate new file names */
            for ( i = 0; i < tat_MscCount; i++ )
            {
                MSC_GenerateNewName( i, tempname, counter );
                FP_SplitPath( tempname, NULL, MSC_Pool[i].outname, NULL );
                strcpy(MSC_Pool[i].full_name, tempname);
            };

            /* process MSCs */
            for ( i = 0; i < tat_MscCount; i++ )
            {
                if ( !MSC_FlagTextInput )
                {
                    if ( !MSC_MacroProcess( i, MSC_StaticMacroTableSize, MSC_StaticMacroTable ) )
                    {
                        ExitCode = SYS_ERROR_EXIT_CODE;
                        goto PExit;
                    }
                }
                else
                {
                    if ( !MSC_MacroProcessText( i, MSC_StaticMacroTableSize, MSC_StaticMacroTable ) )
                    {
                        ExitCode = SYS_ERROR_EXIT_CODE;
                        goto PExit;
                    }
                }
            };

            counter++;
            res = MSC_GetMacroValues(lvv);
        };
        
    }
    else
    {
        res = MSC_MacroGenSet(configdata, path, pathlen, counter, &next_start_i);
        while ( res !=2 && res!=-1 && ( counter < (UINT32_T)quantity || quantity == -1 ) )
        {
            if (!res)
            {
                ExitCode = SYS_ERROR_EXIT_CODE;
                goto PExit;
            }

            /* generate new file names */
            for ( i = 0; i < tat_MscCount; i++ )
            {
                MSC_GenerateNewName( i, tempname, counter );
                FP_SplitPath( tempname, NULL, MSC_Pool[i].outname, NULL );
                strcpy(MSC_Pool[i].full_name, tempname);
            };

            /* process MSCs */
            for ( i = 0; i < tat_MscCount; i++ )
            {

                if ( !MSC_FlagTextInput )
                {
                    if ( !MSC_MacroProcess( i, MSC_StaticMacroTableSize, MSC_StaticMacroTable ) )
                    {
                        ExitCode = SYS_ERROR_EXIT_CODE;
                        goto PExit;
                    }
                }
                else
                {
                    if ( !MSC_MacroProcessText( i, MSC_StaticMacroTableSize, MSC_StaticMacroTable ) )
                    {
                        ExitCode = SYS_ERROR_EXIT_CODE;
                        goto PExit;
                    }
                }
            };

            counter++;
            res = MSC_MacroGenSet(configdata, path, pathlen, counter, &next_start_i);
        };
    }


    for ( i = 0; i < unknown_MacroCount; i++ )
    {
        MSG_PrintError( MSG_MMSC_UNKNOWN_MACRO, unknown_SourceMSC[i], unknown_Macro[i]  );
        MEM_Free( unknown_Macro[i] );
        MEM_Free( unknown_SourceMSC[i] );
    };

    if ( unknown_Macro )
    {
        MEM_Free( unknown_Macro );
        unknown_Macro = NULL;
    }

    if ( unknown_SourceMSC )
    {
        MEM_Free( unknown_SourceMSC );
        unknown_SourceMSC = NULL;
    }

    /* MEM_Free all loaded MSC */
    if ( MSC_FlagTextInput )
    {
        for ( i = 0; i < MSC_PoolCount; i++ )
        {
            MEM_Free( MSC_Pool[i].name );

            if ( MSC_Pool[i].path )
                MEM_Free( MSC_Pool[i].path );
        }
    }
    else
    {
        for ( i = 0; i < MSC_PoolCount; i++ )
        {
            MSC_FreeMemory( MSC_Pool[i].M );
            MEM_Free( MSC_Pool[i].M );
            MEM_Free( MSC_Pool[i].name );

            if ( MSC_Pool[i].path )
                MEM_Free( MSC_Pool[i].path );
        }
    }

    MEM_Free( MSC_Pool );

    if ( tat_MscCount )
    {
        MEM_FreeStrArray( tat_MscCount, MSC_Path );
        MEM_FreeStrArray( tat_MscCount, MSC_Name );
    }

    if ( lst )
        fclose( lst );

    if ( lvv )
    {
        fclose( lvv );
        lvv = NULL;
    }

    if ( flag_LoadVar )
        MSG_PopProcessedFile(  );

    
PExit:
    if ( MSC_StaticMacroTable )
    {
        for ( i = 0; i < MSC_StaticMacroTableSize; i++ )
        {
            MEM_Free( MSC_StaticMacroTable[i].repl );
            MEM_Free( MSC_StaticMacroTable[i].name );
            if (MSC_StaticMacroTable[i].dataset) MEM_Free( MSC_StaticMacroTable[i].dataset );
            if (MSC_StaticMacroTable[i].currpos) MEM_Free( MSC_StaticMacroTable[i].currpos );
            if (MSC_StaticMacroTable[i].overflow) MEM_Free( MSC_StaticMacroTable[i].overflow );

        };
        MEM_Free( MSC_StaticMacroTable );
    };

    if ( MSC_DynMacroTable )
    {
        for ( i = 0; i < MSC_DynMacroTableSize; i++ )
        {
            MEM_Free( MSC_DynMacroTable[i].name );
        };
        MEM_Free( MSC_DynMacroTable );
    };

    if (MSC_LibSet)
    {
        for (i=0; i<MSC_LibSetSize; i++)
        {
            if (MSC_LibSet[i]) Dyn_CloseLibrary( MSC_LibSet[i] );
        }
        MEM_Free(MSC_LibSet);

    }

    TAT_FreeConfig( &xmldata );

    if ( path )
        MEM_Free( path );


    return ExitCode;
}
