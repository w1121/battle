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

static char           **reference_Names;
static MSC_EVENT_KIND      **reference_Events;
static UINT32_T             reference_Count;
static char            *tat_CurrPath = NULL;

/***************************************************************************
 * Function : MSC_LoadMSCbyName
 *
 * Description: load MSC by path and file name specification
 *
 * Returns: NULL/pointer to storage are
 *
 * Notes: None
 *
 **************************************************************************/
MSC_STORAGE *MSC_LoadMSCbyName( char * path, char * name )
{
    MSC_STORAGE        *e, *u;

    char                    f[TAT_MAX_FNAME_LEN];

    f[0] = 0;

    if ( path )
    {
        strcpy( f, path );
    }
    strcat( f, name );

    if ( !strchr( name, '.' ) )
    {
        strcat( f, STR_DOTMPR );
    }

    e = ( MSC_STORAGE * ) MEM_Allocate( sizeof ( MSC_STORAGE ) );

    u = TAT_Load( f, e, 0 );

    if ( u == NULL )
    {
        MEM_Free( e );
        return NULL;
    };

    return e;
}

/***************************************************************************
 * Function : MSC_LoadMSCinPool
 *
 * Description: load MSC in MSC pool
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_LoadMSCinPool( char * path, char * name )
{
    MSC_Pool =
        ( MSC_MSC_POOL * ) MEM_Reallocate( MSC_Pool, ( MSC_PoolCount + 1 ) * sizeof ( MSC_MSC_POOL ) );

    if ( !MSC_FlagTextInput )
    {
        MSC_Pool[MSC_PoolCount].M = MSC_LoadMSCbyName( path, name );

        if ( MSC_Pool[MSC_PoolCount].M == NULL )
        {
            return RESULT_ERR;
        }
    }
    else
        MSC_Pool[MSC_PoolCount].M = NULL;

    MSC_Pool[MSC_PoolCount].name = MEM_CopyStr( name );
    MSC_Pool[MSC_PoolCount].path = MEM_CopyStr( path );
    MSC_Pool[MSC_PoolCount].initial = 0;
    MSC_Pool[MSC_PoolCount].in_process = 0;
    MSC_Pool[MSC_PoolCount].outname = MEM_Allocate(TAT_MAX_FNAME_LEN);
    MSC_Pool[MSC_PoolCount].full_name = MEM_Allocate(TAT_MAX_FNAME_LEN);
    MSC_PoolCount++;

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_CompareDir
 *
 * Description: compare directory names
 *
 * Returns: -1/0/1
 *
 * Notes: None
 *
 **************************************************************************/
static int tat_CompareDir( char * p1, char * p2 )
{
    if ( ( !p1 || ( p1 && p1[0] == 0 ) ) && ( !p2 || ( p2 && p2[0] == 0 ) ) )
        return 0;

    if ( p1 && ( !stricmp( p1, ".\\" ) || !stricmp( p1, "./" ) ) )
        p1 = NULL;

    if ( p2 && ( !stricmp( p2, ".\\" ) || !stricmp( p2, "./" ) ) )
        p2 = NULL;

    if ( ( !p1 || ( p1 && p1[0] == 0 ) ) && ( !p2 || ( p2 && p2[0] == 0 ) ) )
        return 0;

    return stricmp( p1, p2 );
}

/***************************************************************************
 * Function : tat_GetMSCPoolIndex
 *
 * Description: return index of MSC from pool by path and name, 
 *              load if not found
 *
 * Returns: index/-1 if not found
 *
 * Notes: None
 *
 **************************************************************************/
static int tat_GetMSCPoolIndex( char * path, char * name )
{
    int                     i;

    for ( i = 0; i < MSC_PoolCount; i++ )
    {
        if ( !stricmp( name, MSC_Pool[i].name ) )
            if ( !tat_CompareDir( path, MSC_Pool[i].path ) )
                return i;
    };

    if ( !MSC_LoadMSCinPool( path, name ) )
    {
        return -1;
    };

    return MSC_PoolCount - 1;
}

/***************************************************************************
 * Function : MSC_GetMacroExpression
 *
 * Description: get whole macro expression and skip it
 *
 * Returns: pointer to new position
 *
 * Notes: None
 *
 **************************************************************************/
char * MSC_GetMacroExpression( char * s, char * name, char * full )
{
    char                   *r, *f;

    if ( !s )
        return NULL;

    if ( !name )
        return NULL;

    if ( !full )
        return NULL;

    StrSkipComments( &s );

    r = s;

    f = full;

/* This is not an macro expression! */
    if ( *s != MSC_MACRO_START )
    {
        while ( *s && *s != MSC_MACRO_START && *s != ',' && *s != ')' )
        {
            *full++ = *s++;
        }

        *full = 0;
        *name = 0;
        return s;
    }

/* skip macro name prefix ('#') */
    *full++ = *s;
    *name++ = *s;
    s++;
    while ( *s && ( ( !SymIsDelim( *s ) && *s != MSC_MACRO_START ) || *s == '_' ) )
    {
        *full++ = *s;
        *name++ = *s;
        s++;
    }

    *name = 0;                  /* finish macro name */

    StrSkipComments( &s );

    if ( *s != '(' )
    {
        *full = 0;
        return f;
    };

    r = s;                      /* save start position */

    s = StrFindCloseBrace( s ); /* find end of line */
    strncpy( full, r, s - r );
    full[s - r] = 0;
    return f;
}


/***************************************************************************
 * Function : MSC_MacroProcessString
 *
 * Description: iteratively substitute macros in string
 *
 * Returns: d / NULL if error, 
 *
 * Notes: None
 *
 **************************************************************************/
char * MSC_MacroProcessString( char * d, char * s )
{
    int                     subst_count;
    char                    str_copy[TAT_MAX_LINE_LEN];

    char                   *t;
    int                     i;

    if ( !s || !d)
        return NULL;

    t = d;                      /* save original pointer */
    subst_count = 0;

/* make local copy of string */
    strcpy( str_copy, s );

    do
    {
        d = t;
        if ( subst_count )
        {
        /* process result string */
            strcpy( str_copy, t );
        }

        s = &str_copy[0];

        while ( *s )
        {
        /* copy characters until */
            while ( *s && *s != MSC_MACRO_START )
                *d++ = *s++;

            i = 0;

            *d = 0;             /* mark as end of line */

            if ( *s )
                if ( !MSC_MacroEval( s, d, &i, MSC_ActiveTableSize, MSC_ActiveTable ) )
                {
                    return NULL;
                }

            s += i;             /* skip parsed expression */

            d += strlen( d );   /* go to the end */
        }
        *d = 0;

        subst_count++;

    }
    while ( strchr( t, MSC_MACRO_START ) && subst_count < TAT_MAX_ID_LEN && strcmp( str_copy, t ) );

    return t;
}

/***************************************************************************
 * Function : tat_AddReferenceEvent
 *
 * Description: Add 'reference' event to list of references
 *
 * Returns: RESULT_ERR/RESULT_OK 
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_AddReferenceEvent( MSC_EVENT_KIND * s, MSC_STORAGE * M )
{
    reference_Names = MEM_Reallocate( reference_Names, ( reference_Count + 1 ) * sizeof ( char * ) );
    reference_Events = MEM_Reallocate( reference_Events, ( reference_Count + 1 ) * sizeof ( MSC_EVENT_KIND * ) );
    reference_Events[reference_Count] = s;
    reference_Names[reference_Count++] = MEM_CopyStr( MSC_GetString( s->data.ref.refr_Name, M ) );
    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_SearchReferences
 *
 * Description: Look-up for reference reference_Events in MSC
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_SearchReferences( MSC_EVENT_KIND * s, MSC_STORAGE * M )
{
    while ( s )
    {
        if ( s->type == MSC_EV_REFERENCE )
        {
            tat_AddReferenceEvent( s, M );
        }
        else if ( s->type == MSC_EV_OPERATOR )
        {
            UINT32_T                    i;

            for ( i = 0; i < s->data.alt.frame_Count; i++ )
                tat_SearchReferences( s->data.alt.frame[i], M );
        };

        s = s->next;
    };

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_ProcessStrEvent
 *
 * Description: substitute values for strings
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_ProcessStrEvent( MSC_STORAGE * M, MSC_EVENT_KIND * s )
{
    UINT32_T                    i;

    if (!MSC_ConvertString( &s->condition, M, MSC_MacroProcessString )) goto error_ex;
    if (!MSC_ConvertString( &s->comment, M, MSC_MacroProcessString )) goto error_ex;
    if (!MSC_ConvertString( &s->time.str_MinTime, M, MSC_MacroProcessString )) goto error_ex;
    if (!MSC_ConvertString( &s->time.str_MaxTime, M, MSC_MacroProcessString )) goto error_ex;
    if (!MSC_ConvertString( &s->time_Calc.str_MinTime, M, MSC_MacroProcessString )) goto error_ex;
    if (!MSC_ConvertString( &s->time_Calc.str_MaxTime, M, MSC_MacroProcessString )) goto error_ex;
    switch ( s->type )
    {
    case MSC_EV_REFERENCE:
    {
        if (!MSC_ConvertString( &s->data.ref.refr_Name, M, MSC_MacroProcessString )) goto error_ex;
        for ( i = 0; i < s->data.ref.param_Count; i++ )
        {
            if (!MSC_ConvertString( &s->data.ref.refr_Subst[i], M, MSC_MacroProcessString )) goto error_ex;
        }
        break;
    }

    case MSC_EV_TIMER_SET:
    case MSC_EV_TIMER_RESET:
    case MSC_EV_TIMER_TIMEOUT:
    {

        if (!MSC_ConvertString( &s->data.timer.signal_Name, M, MSC_MacroProcessString )) goto error_ex;
        if (!MSC_ConvertString( &s->data.timer.timing.str_MinTime, M, MSC_MacroProcessString )) goto error_ex;
        if (!MSC_ConvertString( &s->data.timer.timing.str_MaxTime, M, MSC_MacroProcessString )) goto error_ex;
        break;
    }

    case MSC_EV_OPERATOR:
    {
        if (!MSC_ConvertString( &s->data.alt.l1, M, MSC_MacroProcessString )) goto error_ex;
        if (!MSC_ConvertString( &s->data.alt.l2, M, MSC_MacroProcessString )) goto error_ex;
        break;
    }

    case MSC_EV_CREATE:
    {
        for ( i = 0; i < s->data.create.param_Count; i++ )
        {
            if ( s->data.create.params[i] )
            {
                if (!MSC_ConvertString( &s->data.create.params[i], M, MSC_MacroProcessString )) goto error_ex;
            }
        }

        break;
    }

    case MSC_EV_CONDITION:
    {
        if (!MSC_ConvertString( &s->data.cond.call, M, MSC_MacroProcessString )) goto error_ex;
        if (!MSC_ConvertString( &s->data.cond.text, M, MSC_MacroProcessString )) goto error_ex;
        break;
    }

    case MSC_EV_ACTION:
    case MSC_EV_CODE:
    {
        if (!MSC_ConvertString( &s->data.text, M, MSC_MacroProcessString )) goto error_ex;
        break;
    }

    case MSC_EV_START_LOOP:
    {
        if (!MSC_ConvertString( &s->data.loop.l1, M, MSC_MacroProcessString )) goto error_ex;
        if (!MSC_ConvertString( &s->data.loop.l2, M, MSC_MacroProcessString )) goto error_ex;
        if (!MSC_ConvertString( &s->data.loop.text, M, MSC_MacroProcessString )) goto error_ex;
        break;
    }
    }
    return RESULT_OK;

error_ex:
    return RESULT_ERR;
}

/***************************************************************************
 * Function : tat_SubstStringMSC
 *
 * Description: process strings in MSC
 *
 * Returns: new MSC
 *
 * Notes: None
 *
 **************************************************************************/
static MSC_STORAGE *tat_SubstStringMSC( MSC_STORAGE * M, int slen, MSC_STATIC_MACRO_INDEX * x )
{
    MSC_STORAGE        *X;
    UINT32_T                    i, j;

    X = MSC_Copy( M );

    MSC_ActiveTableSize = slen;
    MSC_ActiveTable = x;

/* convert instances */
    for ( i = 0; i < X->instance_Count; i++ )
    {
        if (!MSC_ConvertString( &X->instances[i].name, X, MSC_MacroProcessString )) goto error_ex;
        if (!MSC_ConvertString( &X->instances[i].ratio, X, MSC_MacroProcessString )) goto error_ex;
        if (!MSC_ConvertString( &X->instances[i].comment, X, MSC_MacroProcessString )) goto error_ex;
    }

    for ( i = 0; i < X->label_Count; i++ )
    {
        if (!MSC_ConvertString( &X->labels[i].label, X, MSC_MacroProcessString )) goto error_ex;
    }

    for ( i = 0; i < X->message_Count; i++ )
    {
        if (!MSC_ConvertString( &X->messages[i].message_Name, X, MSC_MacroProcessString )) goto error_ex;
        for ( j = 0; j < X->messages[i].param_Count; j++ )
        {
            if (!MSC_ConvertString( &X->messages[i].message_Params[j], X, MSC_MacroProcessString )) goto error_ex;
        }
        if (!MSC_ConvertString( &X->messages[i].delay.str_MinTime, X, MSC_MacroProcessString )) goto error_ex;
        if (!MSC_ConvertString( &X->messages[i].delay.str_MaxTime, X, MSC_MacroProcessString )) goto error_ex;
    }

    if (!MSC_Process( X, tat_ProcessStrEvent )) goto error_ex;

    MSC_ActiveTableSize = 0;
    MSC_ActiveTable = NULL;
    return X;
    
error_ex:
    MSC_FreeMemory(X);
    return NULL;
}

/***************************************************************************
 * Function : tat_CopyStr
 *
 * Description: make copy of string, special handle of empty strings
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static char            *tat_CopyStr( char *s )
{
    char * buf;
    size_t len;
    if ( !s || *s == 0 )
        return TAT_ZeroStr;
    len = strlen(s);
    buf = MEM_Allocate(len+1);
    StrUpCopy(buf, s);
    return buf;
}

/***************************************************************************
 * Function : tat_CopyStrU
 *
 * Description: make copy of string, special handle of empty strings
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static char            *tat_CopyStrU( char *s )
{
    if ( !s || *s == 0 )
        return TAT_ZeroStr;
    return MEM_CopyStr(s);
}

/***************************************************************************
 * Function : tat_CopySubstitution
 *
 * Description: make copy of substitution table
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_CopySubstitution( int *d, MSC_STATIC_MACRO_INDEX ** dx, int x, MSC_STATIC_MACRO_INDEX * t )
{
    int                     i;

    *d = x;
    *dx = MEM_Copy(t, x * sizeof ( MSC_STATIC_MACRO_INDEX ) );

    for ( i = 0; i < x; i++ )
    {
        /* update string */
        ( *dx )[i].name = tat_CopyStr( t[i].name );
        ( *dx )[i].repl = tat_CopyStrU( t[i].repl );
    };

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_FreeSubstitutions
 *
 * Description: free memory allocated for substitutions
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_FreeSubstitutions( int d, MSC_STATIC_MACRO_INDEX * dx )
{
    int                     i;

    for ( i = 0; i < d; i++ )
    {
        if ( dx[i].name != TAT_ZeroStr )
            MEM_Free( dx[i].name );

        if ( dx[i].repl != TAT_ZeroStr )
            MEM_Free( dx[i].repl );
    };

    MEM_Free( dx );
    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_AddSubstitution
 *
 * Description: add new substitution or replace existing
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_AddSubstitution( int *d, MSC_STATIC_MACRO_INDEX ** dx, char *t, char *r )
{
    int                     i;

    for ( i = 0; i < *d; i++ )
        if ( !stricmp( ( *dx )[i].name, t ) )
        {
            if ( ( *dx )[i].repl != TAT_ZeroStr )
                MEM_Free( ( *dx )[i].repl );

            ( *dx )[i].repl = tat_CopyStrU( r );
        /* by default add substitution which is active */
            ( *dx )[i].active = 1;

            return RESULT_OK;
        };

    i = ( *d )++;

    *dx = MEM_Reallocate( *dx, ( *d ) * sizeof ( MSC_STATIC_MACRO_INDEX ) );

    ( *dx )[i].name = tat_CopyStr( t );
    ( *dx )[i].repl = tat_CopyStrU( r );

/* by default add substitution which is active */
    ( *dx )[i].active = 1;
    ( *dx )[i].index = MSC_INFINITY_CONST;
    ( *dx )[i].datatype = MSC_MACRODATA_UNDEF;

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_SubstLoopsMSC
 *
 * Description: substitute loop parameters
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_SubstLoopsMSC( MSC_STORAGE * M, MSC_EVENT_KIND * s )
{
    UINT32_T                    i;

    if ( !M )
        return RESULT_ERR;

    while ( s )
    {
        if ( s->type == MSC_EV_OPERATOR )
        {
            if ( s->data.alt.type == MSC_OP_LOOP )
            {
                MSC_ConvertString( &s->data.alt.l1, M, MSC_MacroProcessString );
                MSC_ConvertString( &s->data.alt.l2, M, MSC_MacroProcessString );
            }

            for ( i = 0; i < s->data.alt.frame_Count; i++ )
                if ( !tat_SubstLoopsMSC( M, s->data.alt.frame[i] ) )
                    return RESULT_ERR;
        };

        s = s->next;
    };

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_SubstCondsMSC
 *
 * Description: substitute conditions of existence for reference_Events
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_SubstCondsMSC( MSC_STORAGE * M, MSC_EVENT_KIND * s )
{
    UINT32_T                    i;
    char                    st[TAT_MAX_LINE_LEN];

    if ( !M )
        return RESULT_ERR;

    while ( s )
    {
        if ( s->condition )
        {
            if ( !MSC_MacroProcessString( st, MSC_GetString( s->condition, M ) ) )
                return RESULT_ERR;

            s->condition = MSC_AddStr( st, M );
        };

        if ( s->type == MSC_EV_OPERATOR )
        {
            for ( i = 0; i < s->data.alt.frame_Count; i++ )
                if ( !tat_SubstCondsMSC( M, s->data.alt.frame[i] ) )
                    return RESULT_ERR;
        };

        s = s->next;
    };

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_SubstMacroMSC
 *
 * Description: Process MSC-PR substitutions
 *
 * Returns: pointer to new MSC
 *
 * Notes: None
 *
 **************************************************************************/
static MSC_STORAGE *tat_SubstMacroMSC( MSC_STORAGE * M, int slen, MSC_STATIC_MACRO_INDEX * x )
{
    char                  **_refr_Names;
    MSC_STORAGE        *X, *W;
    MSC_EVENT_KIND             **_events;
    int                     _num_ref, i, k;
    UINT32_T                    j;
    char                    curr_mscname[TAT_MAX_FNAME_LEN];

    W = MSC_Copy( M );

    if ( !W )
        return NULL;

    FP_SplitPath( MSC_GetString( M->msc_File, M ), NULL, curr_mscname, NULL );
    MSC_SetCurrMscName(curr_mscname);

    MSC_ActiveTableSize = slen;
    MSC_ActiveTable = x;

    if ( !tat_SubstLoopsMSC( W, W->start ) )
    {
        return NULL;
    }

   
    if ( tat_UnrollLoops )
        MSC_Transform( W, MSC_TRANS_UNROLL_LOOP );

    if ( tat_InlineLoops )
        MSC_Transform( W, MSC_TRANS_INLINE_LOOP );

   
    if ( !flag_SaveZeroLoop )
        MSC_Transform( W, MSC_TRANS_REMOVE_ZERO_LOOP );

    if ( !tat_SubstCondsMSC( W, W->start ) )
    {
        return NULL;
    }

    if ( MSC_TransformFlags )
    {
        MSC_Transform( W, MSC_TransformFlags );
    }

    X = tat_SubstStringMSC( W, slen, x );

    if ( !X )
    {
        MSC_FreeMemory( W );
        MEM_Free( W );
        return NULL;
    }

    MSC_FreeMemory( W );
    MEM_Free( W );

    if ( !X )
        return NULL;

/* now we have substituted MSC */
    reference_Names = NULL;
    reference_Count = 0;
    reference_Events = NULL;

    if ( !flag_NoRefSubst )
    {
    /* get references */
        tat_SearchReferences( X->start, X );
    /* make local copies */
        _num_ref = reference_Count;
        _events = reference_Events;
        _refr_Names = reference_Names;

        for ( i = 0; i < _num_ref; i++ )
        {
        /* handle 'functional' references */
            if ( _refr_Names[i][0] == '$' )
            {
                if ( flag_SkipFuncRef )
                {
                    continue;
                }
                else
                {
                    k = tat_GetMSCPoolIndex( tat_CurrPath, _refr_Names[i] + 1 );
                }
            }
            else
            {
                k = tat_GetMSCPoolIndex( tat_CurrPath, _refr_Names[i] );
            }

            if ( k < 0 )
            {
                if ( flag_IgnoreNotFoundRef )
                {
                    continue;
                }
                else
                    return NULL;
            }

            if (MSC_Pool[k].in_process)
            {
                MSG_PrintError("WARNING: semantic: file '%s(%u)': recursive reference\n", curr_mscname, _events[i]->line_Number );
                continue;
            }

            if ( _events[i]->data.ref.param_Count || flag_InlineAllRef )
            {
                int                     pnum;
                MSC_STATIC_MACRO_INDEX  *R;

                if ( flag_Globm )
                {
                    tat_CopySubstitution( &pnum, &R, slen, x );
                }
                else
                {
                    pnum = 0;
                    R = NULL;
                };

            /* modify parameters */
                for ( j = 0; j < _events[i]->data.ref.param_Count; j++ )
                {
                    tat_AddSubstitution( &pnum, &R,
                        &_events[i]->data.ref.macro_Name[j][1],
                        MSC_GetString( _events[i]->data.ref.refr_Subst[j], X ) );
                };
                
                /* sort substitutions */
                qsort(R, pnum, sizeof(R[0]), MSC_CompStaticData);

                MSC_Pool[k].in_process = 1;
                W = tat_SubstMacroMSC( MSC_Pool[k].M, pnum, R );
                MSC_Pool[k].in_process = 0;

                if ( !W )
                    return NULL;

                MSC_Include( X, W, _events[i], flag_RefSync );
                MSC_FreeMemory( W );
                MEM_Free( W );
                tat_FreeSubstitutions( pnum, R );
            }
            else
            {
                if ( MSC_Pool[k].initial )
                {
                /* change only name */
                    _events[i]->data.ref.refr_Name = MSC_AddStr( MSC_Pool[k].outname, X );
                }
                else
                {
                    MSC_Pool[k].in_process = 1;
                    if ( flag_Globm )
                    {
                        W = tat_SubstMacroMSC( MSC_Pool[k].M, slen, x );
                    }
                    else
                    {
                        W = tat_SubstMacroMSC( MSC_Pool[k].M, 0, NULL );
                    }
                    MSC_Pool[k].in_process = 0;

                    if ( !W )
                        return NULL;

                    MSC_Include( X, W, _events[i], flag_RefSync );
                    MSC_FreeMemory( W );
                    MEM_Free( W );
                }
            }
        }
    }

    FP_SplitPath( MSC_GetString( M->msc_File, M ), NULL, curr_mscname, NULL );
    MSC_SetCurrMscName(curr_mscname);

    if ( !flag_NoRefSubst )
    {
        for ( i = 0; i < _num_ref; i++ )
            MEM_Free( _refr_Names[i] );

        if ( _refr_Names )
            MEM_Free( _refr_Names );

        if ( _events )
            MEM_Free( _events );
    };

    return X;
}

/***************************************************************************
 * Function : tat_MacroProcessMSC
 *
 * Description: process MSC number i from pool and save it 
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_MacroProcess( int i, int slen, MSC_STATIC_MACRO_INDEX * x )
{
    char                    fn[TAT_MAX_FNAME_LEN], en[TAT_MAX_FNAME_LEN];
    MSC_STORAGE        *M;

/* make global */
    tat_CurrPath = MSC_Pool[i].path;
    MSC_Pool[i].in_process = 1;
    M = tat_SubstMacroMSC( MSC_Pool[i].M, slen, x );
    MSC_Pool[i].in_process = 0;

    if ( !M )
    {
        return RESULT_ERR;
    }

    if ( !MSC_Save( MSC_Pool[i].full_name, M, MSC_OutFlags ) )
    {
        return RESULT_ERR;
    }

    if ( lst )
    {
        FP_SplitPath( MSC_Pool[i].full_name, NULL, fn, en );
        fprintf( lst, "%s%s\n", fn, en );
    };

    MSC_FreeMemory( M );

    MEM_Free( M );

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_MacroProcessText
 *
 * Description: process text file number i from pool
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_MacroProcessText( int i, int slen, MSC_STATIC_MACRO_INDEX * x )
{
    char                    dest[TAT_MAX_LINE_LEN], outb[TAT_MAX_LINE_LEN];
    char                    f[TAT_MAX_FNAME_LEN], *line;
    FILE                   *inp, *outp;

    f[0] = 0;

    if ( MSC_Pool[i].path )
        strcpy( f, MSC_Pool[i].path );

    strcat( f, MSC_Pool[i].name );

    if ( !strchr( MSC_Pool[i].name, '.' ) )
        strcat( f, STR_DOTMPR );

    inp = FP_OpenFile( f, "r" );

    if ( !inp )
        return RESULT_ERR;

    outp = FP_OpenFile( MSC_Pool[i].full_name, "w" );

    MSC_SetCurrMscName(MSC_Pool[i].name);

    if ( !outp )
        return RESULT_ERR;

    MSC_ActiveTable = x;
    MSC_ActiveTableSize = slen;
    do
    {
        line = fgets( dest, TAT_MAX_LINE_LEN - 4, inp );

        if ( !line )
            break;


        if ( !MSC_MacroProcessString( outb, dest ) )
        {
            fclose( inp );
            fclose( outp );
            remove( MSC_Pool[i].full_name );
            return RESULT_ERR;
        };
        fputs( outb, outp );
    }
    while ( 1 );

    fclose( inp );
    fclose( outp );
    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_GetMacroValues
 *
 * Description: calculate new substitutions or load from file
 *
 * Returns: 0 / 1
 *
 * Notes: None
 *
 **************************************************************************/
int MSC_GetMacroValues( FILE *lvv )
{
    char                   *dest, *line, xx[TAT_MAX_ID_LEN];
    UINT32_T                    i;
    int                     index;
    static int              line_counter = 0;

    if (!lvv) return RESULT_ERR;

    if (feof(lvv))
    {
        return 2;
    }

    dest = MEM_Allocate( TAT_MAX_LINE_LEN * 4 * sizeof ( char ) );

    /* initialize basic macro table */
    for ( i = 0; i < MSC_StaticMacroTableSize; i++ )
    {
        MSC_StaticMacroTable[i].repl[0]=MSC_MACRO_START;
        strcpy( MSC_StaticMacroTable[i].repl+1, MSC_StaticMacroTable[i].name );
    }

    do
    {
        line = fgets( dest, TAT_MAX_LINE_LEN * 4 - 4, lvv );
        line_counter++;
        MSG_SetLineNumber( line_counter );

        if ( !line )
            return RESULT_ERR;

        line = StrTrimSpaces( line );
    }
    while ( *line == 0 && !feof(lvv));


    while ( *line )
    {
        if ( *line != MSC_MACRO_START )
        {
            MSG_SyntaxError(  );
            MEM_Free( dest );
            return RESULT_ERR;
        };

        if ( !StrExtractMacroID( &line, xx ) || xx[0] == 0 )
        {
            MSG_SyntaxError(  );
            MEM_Free( dest );
            return RESULT_ERR;
        }

        index = MSC_GetSubstNumber( xx+1 );

        if ( index == -1 )
        {
            MSG_SemanticError( MSG_MMSC_UNKNOWN_INP_MACRO, xx );
            MEM_Free( dest );
            return RESULT_ERR;
        };

        STR_SKIP_SPACES( line );

        if ( *line != '=' )
        {
            MSG_SyntaxError(  );
            MEM_Free( dest );
            return RESULT_ERR;
        };

        line++;

        STR_SKIP_SPACES( line );

    /* get value */
        if ( !StrExtractExpr( &line, xx ) )
        {
            MSG_SyntaxError(  );
            MEM_Free( dest );
            return RESULT_ERR;
        };

        strcpy( MSC_StaticMacroTable[index].repl, xx );

        STR_SKIP_SPACES( line );

        if ( *line == ',' )
        {
            line++;
            STR_SKIP_SPACES( line );
        }
    }

    MEM_Free( dest );
    return RESULT_OK;
}
