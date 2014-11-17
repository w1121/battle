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
* Functions:    MSCEXPN_SavePath
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
#include <qmemory.h>
#include <mscbase.h>
#include <partial.h>
#include <mscgen.h>
#include <fileproc.h>
#include <flowgen.h>
#include <outmess.h>
#include <mscmess.h>
#include <mscstrs.h>
#include <tatgdata.h>
#include <strproc.h>

#include "genmsc.h"

static size_t             max_Paths;     /* maximal number of paths to generate */
static size_t             start_Path;    /* number of the first path to generate */
static UINT32_T             flag_Generate; /* 0 - count possible number of MSC, 1 - generate MSC */
static UINT32_T             flag_OutTime;  /* 0 - standard MSC, 1 - MSC with time */

static MSC_EVENT_KIND      **complete_Path; /* array of pointers to events of a complete test path */

static FILE            *list_FileStream;    /* file with list of generated files */
static char            *output_Dir;    /* output directory */

static MSC_STORAGE *current_MSC;   /* pointer to loaded MSC */

static int              flag_Flowgen;

static const char      *out_FileExt[2] = { STR_DOTMPR, STR_DOTFG };

static size_t             msc_Counter;

/* line of generating MSC */
static char             buffer[TAT_MAX_LINE_LEN];

/* Generate simple MSC PR:
   ev_Number - number of events in the test path,
   counter - counter of generated test paths,
*/
static LOGICAL          tat_GenerateMSCTrace( const size_t ev_Number );

static size_t             tat_CountPaths( const MSC_EVENT_TREE * T );

/* Calculate maximal length of a complete test path:
   TM - pointer to MSC as a tree structure,
   pres_Length - presentlength of the complete test path,
   igor_Length - maximal length,
   perm - ending indicator
*/
static size_t             tat_CalcMaxLength( const MSC_EVENT_TREE * TM );

static int              tat_GenerateNextPath( MSC_EVENT_TREE * T, size_t * place,
                                             size_t nest, int incr );

static size_t tat_Max( size_t a, size_t b )
{
    return ( a > b ) ? a : b;
}

/***************************************************************************
 * Function : tat_CalcMaxLength
 *
 * Description: calculate maximal length of a complete test path
 *
 * Returns: length
 *
 * Notes: None
 *
 **************************************************************************/
static size_t tat_CalcMaxLength( const MSC_EVENT_TREE * TM )
{
    size_t                    i;         /* loop counter */
    size_t                    count, fr;

/* check input parameters */

    if ( ( TM == NULL ) )
    {
        return RESULT_ERR;
    };

    count = TM->ev_Number;

    fr = 0;

/* process operator frames */
    if ( TM->type != MSC_OP_SEQ )
    {
        if ( !TM->expand )
        {
            for ( i = 0; i < TM->var_Number; i++ )
            {
                fr = tat_Max( fr, tat_CalcMaxLength( TM->frames[i] ) );
            };

            count += fr;
        }
        else
        {
            for ( i = 0; i < TM->var_Number; i++ )
            {
                count++;
                count += tat_CalcMaxLength( TM->frames[i] );
                count++;
            };

            count++;
        }
    };

    return count + ( ( TM->next ) ? tat_CalcMaxLength( TM->next ) : 0 );
}

/***************************************************************************
 * Function : MSCEXPN_SavePath
 *
 * Description:  build complete test paths
 *               M - pointer to loaded MSC,
 *               TM - pointer to MSC as a tree structure,
 *               start_P - traces to skip,
 *               max_P - maximal number of paths,
 *               flag_Generate - flag for producing MSC PR,
 *               flag_OutTime - 0 - standard MSC, 1 - MSC with time,
 *               list_File - file with list of generated files,
 *               outp_Dir - output directory *
 * Returns: RESULT_ERR/ RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSCEXPN_SavePath( MSC_STORAGE * M,
    MSC_EVENT_TREE * TM,
    const size_t start_p,
    const size_t max_p, const UINT32_T gen_flag,
    const UINT32_T time_flag, const char *list_file, char *outp_dir )
{
    size_t                    length;    /* maximal length of a complete test path */
    char                    filename[TAT_MAX_FNAME_LEN];    /* name of list file */
    int                     result;

/* check input parameters */
    if ( ( M == NULL ) || ( TM == NULL ) )
    {
        return RESULT_ERR;
    }

/* zero fill */
    memset( filename, 0, sizeof ( filename ) );

/* initialization */
    max_Paths = ( gen_flag ) ? max_p : MSC_INFINITY_CONST;

    flag_Generate = gen_flag;

    flag_OutTime = ( time_flag & 1 ) ? MSC_MPR_TIMED_OUT : MSC_MPR_STD_OUT;

    flag_OutTime |= ( time_flag & 2 ) ? MSC_MPR_OUTPUT_CALCTIME : 0;

    flag_Flowgen = M->source_Format;

    start_Path = start_p;
    current_MSC = M;
    output_Dir = outp_dir;

/* counter for MSC */
    msc_Counter = 0;

/* count maximal length of a complete test path */
    complete_Path = NULL;

    length = tat_CalcMaxLength( TM );

/* allocate memory */
    if ( flag_Generate )
        complete_Path = ( MSC_EVENT_KIND ** ) MEM_Allocate( length * sizeof ( complete_Path[0] ) );

/* create list file */
    if ( ( list_file != NULL ) && ( flag_Generate ) )
    {
    /* create with path */

        if ( output_Dir != NULL )
        {
        /* create output directory */

            if ( !FP_MakeDir( output_Dir ) )
            {
                return RESULT_ERR;
            }

            strcpy( filename, output_Dir );
            strcat( filename, list_file );
        }
        else
        {
            strcpy( filename, list_file );
        }

        list_FileStream = FP_OpenFile( filename, "a+" );

        if ( list_FileStream == NULL )
            return RESULT_ERR;
    }
    else
    {
        list_FileStream = NULL;
    }

/* generate MSC if needed */
    if ( flag_Generate )
    {
        length = 0;
        if ( max_Paths != MSC_INFINITY_CONST )
        {
            max_Paths += start_Path;
        }
        while ( msc_Counter < max_Paths )
        {
            result = tat_GenerateNextPath( TM, &length, 0, 1 );

            if ( result == -1 )
            {
                return RESULT_ERR;
            }

            if ( result == 1 )
            {
                break;
            }
        }
    }
    else
    {
    /* calculate number of possible traces */
        msc_Counter = tat_CountPaths( TM );
    }

/* free a memory */
    if ( flag_Generate && complete_Path )
        MEM_Free( complete_Path );

    if ( list_FileStream != NULL )
    {
        fclose( list_FileStream );
    }

/* put result */
    if ( !flag_Generate )
    {
        printf( MSG_CONFIG_CAN_GENERATED_u, MSC_GetStr(M->msc_File, M), ( msc_Counter > start_Path ) ? msc_Counter - start_Path : 0 );
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_CountPaths
 *
 * Description: Count number of possible complete traces
 *
 * Returns: number <= MSC_INFINITY_CONST
 *
 * Notes: None
 *
 **************************************************************************/
static size_t tat_CountPaths( const MSC_EVENT_TREE * T )
{
    size_t                    count, i, p, exc = 0;

    count = T->path_Number;

    if ( !count )
        count = 1;

    if ( T->oper )
    {
        if ( !T->expand )
        {
            p = 0;

            for ( i = 0; i < T->var_Number; i++ )
                p += tat_CountPaths( T->frames[i] );

            if ( T->type == MSC_OP_OPT )
                p++;

            if ( T->type != MSC_OP_EXC )
                count = MSC_HugeMul( count, p );
            else
                exc = p;
        }
        else
        {
            for ( i = 0; i < T->var_Number; i++ )
            {
                p = tat_CountPaths( T->frames[i] );

                if ( p )
                    count = MSC_HugeMul( count, p );
            }
        }
    }

    if ( T->next )
    {
        p = tat_CountPaths( T->next );

        if ( T->type == MSC_OP_EXC )
        {
            count = MSC_HugeMul( count, exc + p );
        }
        else if ( p )
            count = MSC_HugeMul( p, count );
    };

    return count;
}

/***************************************************************************
 * Function : tat_GenerateNextPath
 *
 * Description: generate next trace for T
 *
 * Returns: 0, if all paths have been generated at this and nested levels
 *          1, if there are any paths at this and nested levels 
 *          -1, if any error occured
 *
 * Notes: None
 *
 **************************************************************************/
static int tat_GenerateNextPath( MSC_EVENT_TREE * T, size_t * place, size_t nest, int incr )
{
    size_t                    start_place, *pointer, i, k;
    int                     res_x = 0;

    start_place = *place;
/* copy path at this level */
    pointer = ( T->paths ) ? T->paths[T->path_Limiter] : NULL;
    k = *place;

    for ( i = 0; i < T->ev_Number; i++ )
        complete_Path[k++] = T->linear[*pointer++];

    *place += T->ev_Number;

/* increment path counter */
    T->path_Limiter += incr;

    if ( T->path_Limiter >= T->path_Number )
    {
        incr = 1;
        T->path_Limiter = 0;
    }
    else
    {
        incr = 0;               /* if incr==0 at entry, it is still zero */
    }

    if ( T->var_Number )
    {
        if ( T->expand || T->type == MSC_OP_PAR )
        {
            if ( !T->frame_PathCounter )
            {
                T->frame_PathCounter = MEM_Allocate( T->var_Number * sizeof ( UINT32_T ) );
            }

        /* initialize number of paths in frames */
            if ( !T->frame_FullCounter )
            {
                T->frame_FullCounter = MEM_Allocate( T->var_Number * sizeof ( UINT32_T ) );

                for ( i = 0; i < T->var_Number; i++ )
                {
                    T->frame_FullCounter[i] = tat_CountPaths( T->frames[i] );
                }
            }
        /* start of inline expression */
            complete_Path[( *place )++] = T->oper;

            res_x = incr;
            for ( i = 0; i < T->var_Number; i++ )
            {
                k = tat_GenerateNextPath( T->frames[i], place, nest + 1, res_x );
                if ( k == -1 )
                {
                    return -1;
                }

                res_x &= k;

            /* separator of inline expression */
                complete_Path[( *place )++] = NULL;
            }
        /* end of inline expression */
            complete_Path[( *place )++] = NULL;

            k = 1;              /* carry */
            for ( i = 0; i < T->var_Number && k; i++ )
            {
                T->frame_PathCounter[i] += k;

                if ( T->frame_PathCounter[i] >= T->frame_FullCounter[i] )
                {
                    T->frame_PathCounter[i] = 0;
                    k = 1;
                }
                else
                {
                    k = 0;
                }
            }

        /* neccessity of incrementing rest part */
            incr = (int)k;
        }
        else
        {
            switch ( T->type )
            {

            case MSC_OP_OPT:
            case MSC_OP_EXC:
            {
                if ( T->frame_Counter == 0 )
                {
                /*                        printf("MSC_OP_OPT selected. incr= %d\n",incr); */
                    res_x = tat_GenerateNextPath( T->frames[0], place, nest + 1, incr );

                    if ( res_x == -1 )
                        return -1;  /* check for errors */

                    if ( res_x == 0 )
                        incr = 0;

                /*if (T->type == MSC_OP_EXC) return incr; */
                }
                else
                {
                /*                        printf("MSC_OP_OPT not selected. incr= %d\n",incr); */
                };

                break;
            };

            case MSC_OP_ALT:
            case MSC_OP_LOOP:
            {
                if ( T->frame_Counter >= T->var_Number )
                {
                    T->frame_Counter = 0;
                };

            /*                    printf("MSC_OP_ALT %d selected. incr= %d\n",T->frame_Counter, incr); */
                res_x = tat_GenerateNextPath( T->frames[T->frame_Counter], place, nest + 1, incr );

                if ( res_x == -1 )
                    return -1;

                if ( res_x == 0 )
                    incr = 0;
            };
            }
        }
    };

    if ( T->next &&
        ( !T->oper ||
            ( T->oper && ( ( T->type == MSC_OP_EXC && T->frame_Counter && !T->expand )
                    || T->type != MSC_OP_EXC || T->expand ) ) ) )
    {
        res_x = tat_GenerateNextPath( T->next, place, nest, incr );

        if ( res_x == -1 )
            return -1;

        if ( res_x == 0 )
            incr = 0;
    }
    else
    {
        if ( !T->oper || T->type != MSC_OP_EXC || T->frame_Counter != 0 )
            res_x = incr;
    }

/* if this is the end of diagram */
    if ( ( T->next == NULL || ( T->oper && T->type == MSC_OP_EXC && T->frame_Counter == 0 && !T->expand ) )
        && nest == 0 )
    {
        if ( msc_Counter >= start_Path )
        {
            if ( !tat_GenerateMSCTrace( *place ) )
                return -1;
        }

        msc_Counter++;

    }

    if ( res_x && T->oper && !T->expand )
    {
        T->frame_Counter += incr;

        if ( T->type == MSC_OP_OPT || T->type == MSC_OP_EXC )
        {
            if ( T->frame_Counter > 1 )
            {
                T->frame_Counter = 0;
                incr = 1;
            }
            else
                incr = 0;
        }
        else
        {
            if ( T->frame_Counter >= T->var_Number )
            {
                T->frame_Counter = 0;
                incr = 1;
            }
            else
                incr = 0;
        }
    }

    if ( nest == 0 )
    {
        *place = start_place;
    }

    return incr;
}

/***************************************************************************
 * Function : tat_GenerateMSCTrace
 *
 * Description: Save simple MSC trace
 *
 * Returns: RESULT_ERR/ RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_GenerateMSCTrace( const UINT32_T ev_number )
{
    FILE                   *f;
    char                   *s, *pdest;
    char                    filename[TAT_MAX_FNAME_LEN],    /* name of MSC file */
                            fullname[TAT_MAX_FNAME_LEN];    /* full name */

    UINT32_T                    i,         /* loop counter */
                            sp = 0;    /* stack pointer (to the first empty element) */

    MSC_EVENT_KIND              *stack[128]; /* stack for nested operators */

/* get a loaded MSC file name */
    s = MSC_GetString( current_MSC->msc_File, current_MSC );

    if ( !s )
    {
        MSG_PrintError( MSG_INTERNAL_PROBLEM );
        return RESULT_ERR;
    }

    FP_SplitPath( s, NULL, fullname, NULL );

/* generate a new file name */
    sprintf( filename, "%sN%u", fullname, msc_Counter );

    if ( output_Dir != NULL )
    {
        sprintf( fullname, "%s%s%s", output_Dir, filename, out_FileExt[flag_Flowgen] );
    }
    else
    {
        sprintf( fullname, "%s%s", filename, out_FileExt[flag_Flowgen] );
    }

    f = FP_OpenFile( fullname, "w" );

    if ( f == NULL )
        return RESULT_ERR;

    if ( !flag_Flowgen )
    {
    /* output base source references */
        buffer[0] = 0;
        pdest = &buffer[0];

        if ( current_MSC->src_Count && ( flag_OutTime & MSC_MPR_OUTPUT_SREF ) )
        {
            *pdest++ = '/';
            *pdest++ = '*';

            for ( i = 0; i < current_MSC->src_Count; i++ )
            {
                sprintf( pdest, "[%s,%u,%u,%u]", MSC_GetStr( current_MSC->src_Files[i].link, current_MSC ),
                    current_MSC->src_Files[i].level,
                    current_MSC->src_Files[i].first_Line, current_MSC->src_Files[i].last_Line );
                pdest += strlen( pdest );
            }
            *pdest++ = '*';
            *pdest++ = '/';
            *pdest = 0;
        }

        /***** generate tatbase.header *****/
        fprintf( f, "mscdocument %s;\n", filename );
        fprintf( f, "msc %s%s;\n", filename, buffer );

        /***** generate instances *****/
        for ( i = 0; i < current_MSC->instance_Count; i++ )
        {
            fprintf( f, "%s: instance", MSC_GetStr( current_MSC->instances[i].name, current_MSC ) );

            if ( current_MSC->instances[i].type )
                fprintf( f, " %s", MSC_GetStr( current_MSC->instances[i].type, current_MSC ) );

            if ( !current_MSC->instances[i].type && current_MSC->instances[i].ratio != STRCOLL_STR_ID_1 )
                fprintf( f, " " );

            if ( current_MSC->instances[i].ratio != STRCOLL_STR_ID_1 )
                fprintf( f, "{%s}", MSC_GetStr( current_MSC->instances[i].ratio, current_MSC ) );

            fprintf( f, ";\n" );
        }

        /***** output text ******/
        for ( i = 0; i < current_MSC->text_Count; i++ )
        {
            fputs( "text '", f );
            StrEmbedMSCtext( buffer, MSC_GetStr( current_MSC->texts[i], current_MSC ) );
            fputs( buffer, f );
            fputs( "';\n", f );
        }

        for ( i = 0; i < current_MSC->param_Count; i++ )
        {
            fprintf( f, "text 'params %s %s';\n", MSC_GetStr( current_MSC->params[i].pv_Type, current_MSC ),
                MSC_GetStr( current_MSC->params[i].pv_Name, current_MSC ) );
        }

        for ( i = 0; i < current_MSC->var_Count; i++ )
        {
            fprintf( f, "text 'def %s %s';\n", MSC_GetString( current_MSC->vars[i].pv_Type, current_MSC ),
                MSC_GetString( current_MSC->vars[i].pv_Name, current_MSC ) );
        };
    }
    else
    {
        for ( i = 0; i < current_MSC->text_Count; i++ )
        {
            fprintf( f, "#\t%s\n", MSC_GetString( current_MSC->texts[i], current_MSC ) );
        };

        if ( current_MSC->text_Count )
            fprintf( f, "\n\n" );
    }

    /***** generate messages *****/
    for ( i = 0; i < ev_number; i++ )
    {
        if ( ( !complete_Path[i] ) ||
            ( ( complete_Path[i] ) && ( complete_Path[i]->type != MSC_EV_CONCURRENT_START ) &&
                ( complete_Path[i]->type != MSC_EV_CONCURRENT_END ) ) )
        {
            if ( ( complete_Path[i] ) && ( complete_Path[i]->type == MSC_EV_OPERATOR ) )
            {
            /* add to operator's stack */
                stack[sp] = complete_Path[i];
                sp++;
            /* generate operator itself */
                MSC_GenStartOp( buffer, complete_Path[i], current_MSC, flag_OutTime );
                fprintf( f, "%s\n", buffer );
            }
            else
            {
                if ( complete_Path[i] == NULL )
                {
                    if ( complete_Path[i + 1] == NULL ) /* end of operator */
                    {
                        MSC_GenEndOp( buffer, stack[--sp], current_MSC, flag_OutTime );
                        fprintf( f, "%s\n", buffer );
                        i++;
                    }
                    else        /* end of a frame */
                    {
                        MSC_GenSepOp( buffer, stack[sp - 1], current_MSC, flag_OutTime );
                        fprintf( f, "%s\n", buffer );
                    }
                }
                else
                {
                    s = ( !flag_Flowgen ) ? MSC_GenerateLine( buffer, complete_Path[i], current_MSC,
                        flag_OutTime ) : FG_GenerateLine( buffer, complete_Path[i], current_MSC,
                        flag_OutTime );

                    if ( !s )
                    {
                        MSG_PrintError( MSG_INTERNAL_PROBLEM );
                        return RESULT_ERR;
                    }
                    else
                    {
                        if ( s[0] )
                            fprintf( f, "%s\n", s );
                    }
                }
            }
        }
    }

    if ( !flag_Flowgen )
    {
        /***** generate ending *****/

        for ( i = 0; i < current_MSC->instance_Count; i++ )
        {
            fprintf( f, "%s: endinstance;\n", MSC_GetString( current_MSC->instances[i].name, current_MSC ) );
        };

        fprintf( f, "endmsc;\n" );
    }

    fclose( f );

/* save information about generated file */

    if ( list_FileStream != NULL )
    {
        fprintf( list_FileStream, "%s%s\n", filename, out_FileExt[flag_Flowgen] );
    };
    return RESULT_OK;
}
