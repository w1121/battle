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
* Functions:    MSC_ReduceEventTreeSigIgn
*               MSC_ReduceEventTree
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  Reduce events with saving of partial ordering
*
* Notes:        None
*
******************************************************************************/
#include <portable.h>
#include <qmemory.h>
#include <mscbase.h>
#include <partial.h>
#include <genpath.h>
#include <outmess.h>
#include <testtree.h>
#include <msctime.h>
#include <mscgen.h>
#include <mscmess.h>
#include <mscstrs.h>

#define TRANSITIVE_COUNT 128

static int rearrange( MSC_EVENT_TREE * T, size_t * revents, size_t re )
{
    unsigned char          *na, *adjmatr;
    size_t                   *pa;
    size_t                    i, j, iv, ivv, ev;

    /* if all events shall be removed, return empty list */
    if ( T->ev_Number == re )
    {
        if ( T->linear )
            MEM_Free( T->linear );

        if ( T->adjacency )
            MEM_Free( T->adjacency );

        T->ev_Number = 0;

        T->linear = NULL;

        T->adjacency = NULL;

        return RESULT_OK;
    }

    na = MEM_Allocate( ( T->ev_Number - re ) * ( T->ev_Number - re ) * sizeof ( na[0] ) );
    pa = MEM_Allocate( ( T->ev_Number - re ) * sizeof ( pa[0] ) );

/* copy events, but skip removed */

    for ( i = 0, j = 0; i < T->ev_Number; i++ )
    {
        if ( !revents[i] )
        {
            pa[j] = i;
            T->linear[j] = T->linear[i];
            j++;
        }
    }

    ev = T->ev_Number;
    T->ev_Number -= re;
    T->ev_NumberPr -= re;
    T->linear = MEM_Reallocate( T->linear, ( T->ev_Number ) * sizeof ( MSC_EVENT_KIND * ) );
    adjmatr = T->adjacency;

/* copy subset of original matrix */

    for ( i = 0, iv = 0; i < T->ev_Number; i++ )
    {
        ivv = pa[i] * ev;

        for ( j = 0; j < T->ev_Number; j++ )
        {
            na[iv++] = adjmatr[ivv + pa[j]];
        }
    }

    if ( T->adjacency )
        MEM_Free( T->adjacency );

    T->adjacency = na;

    if ( pa )
        MEM_Free( pa );

    return RESULT_OK;
}

LOGICAL MSC_ReduceEventTree( const MSC_STORAGE * M, MSC_EVENT_TREE * T, int reduce, int insys, int inenv )
{
    return MSC_ReduceEventTreeSigIgn( M, T, reduce, insys, inenv, 0, NULL );
}

/* make assumsption, that int = 4 char */
#if OSTYPE==WIN32 && COMPILER==MSVC
static void or_string( unsigned char *x, unsigned char *y, size_t size )
{
    register size_t   r, c;

    r = size >> 3;
    c = 0;

    for ( c = 0; c < r; c++ )
    {
        ( ( unsigned int * ) x )[0] |= ( ( unsigned int * ) y )[0];
        ( ( unsigned int * ) x )[1] |= ( ( unsigned int * ) y )[1];
        x += 8;
        y += 8;

    };

    r = ( size & 7 );

    for ( c = 0; c < r; c++ )
    {
        x[c] |= y[c];
    }
}

#else
static void or_string( unsigned char *x, unsigned char *y, size_t size )
{
    while ( size )
    {
        *x++ |= *y++;
        size--;
    }
}

#endif

LOGICAL MSC_ReduceEventTreeSigIgn( const MSC_STORAGE * M,
    MSC_EVENT_TREE * T, int reduce, int insys, int inenv, size_t scount, char *sname[] )
{
    size_t                   *revents;
    unsigned char          *adjacency;
    char                   *sign;

    size_t                    i, remove, l, re, vertexes;

    register size_t           iv, lv;

    int                     teflags = 0;

    if ( !T )
        return RESULT_OK;

    if ( !M )
        return RESULT_ERR;

/* extra check for correctnes of internal structures */
    if ( T->ev_Number != T->ev_NumberPr )
    {
        MSG_PrintError( NULL );
        return RESULT_ERR;
    }

    revents = MEM_Allocate( sizeof ( revents[0] ) * T->ev_Number );
    re = 0;

    if ( insys )
        teflags |= MSC_CHECK_INSYS;

    if ( inenv )
        teflags |= MSC_CHECK_INENV;

    if ( reduce & GP_FILTER_GEN_COND )
        teflags |= MSC_CHECK_DEL_COND;

    if ( reduce & GP_SAVE_ENVSELF )
        teflags |= MSC_CHECK_ENVSELF;

    if ( !( reduce & GP_FILTER_SR_EVENTS ) )
        teflags |= MSC_CHECK_SAVESR;

    if ( reduce & GP_FILTER_CONDITIONS )
        teflags |= MSC_CHECK_DEL_ALL_COND;

    if ( reduce & GP_FILTER_TIMERS )
        teflags |= MSC_CHECK_DEL_TIMERS;

/* look for events which should be removed */
    if ( reduce )
    {
        for ( i = 0; i < T->ev_Number; i++ )
        {
            remove = 0;         /* not remove by default */

        /* remove coregions unconditionally */
            if ( T->linear[i]->type == MSC_EV_CONCURRENT_START || T->linear[i]->type == MSC_EV_CONCURRENT_END )
            {
                remove = 1;
            }
            else if ( MSC_TestEvent( M, T->linear[i], teflags ) == MSC_REMOVE_ACTION )
            {
                remove = 1;
            }
            else if ( ( reduce & GP_FILTER_OUT_MESS ) && T->linear[i]->type == MSC_EV_OUT_MESSAGE )
            {
                remove = 1;
            }
            else if ( ( reduce & GP_FILTER_IN_MESS ) && T->linear[i]->type == MSC_EV_IN_MESSAGE )
            {
                remove = 1;
            }

            if ( !remove )
            {
            /* remove ignored signals */

                if ( scount &&
                    ( T->linear[i]->type == MSC_EV_IN_MESSAGE || T->linear[i]->type == MSC_EV_OUT_MESSAGE ||
                        T->linear[i]->type == MSC_EV_RECV_SIGNAL || T->linear[i]->type == MSC_EV_SEND_SIGNAL ||
                        T->linear[i]->type == MSC_EV_EXIST_SIGNAL ) )
                {
                    sign =
                        MSC_GetString( MSC_GetMessageParams( T->linear[i]->data.msg.indicator, M )->
                        message_Name, M );

                    for ( l = 0; l < scount; l++ )
                    {
                        if ( !stricmp( sign, sname[l] ) )
                        {
                            remove = 1;

                            break;
                        }
                    }
                }

            /* remove everything except message in/out */
                if ( ( reduce & GP_FILTER_ESR_EVENTS )
                    && T->linear[i]->type != MSC_EV_SEND_SIGNAL
                    && T->linear[i]->type != MSC_EV_RECV_SIGNAL
                    && T->linear[i]->type != MSC_EV_OUT_MESSAGE
                    && T->linear[i]->type != MSC_EV_EXIST_SIGNAL
                    && T->linear[i]->type != MSC_EV_IN_MESSAGE && T->linear[i]->labeled == 0 )
                {
                /* check for timer set with labels */

                    if ( !
                        ( ( T->linear[i]->type == MSC_EV_TIMER_SET || T->linear[i]->type == MSC_EV_TIMER_RESET
                                || T->linear[i]->type == MSC_EV_TIMER_TIMEOUT ) &&
                            T->linear[i]->data.timer.relation && T->linear[i]->data.timer.relation->labeled ) )
                        remove = 1;
                }
            }

            if ( remove )
            {
                /* mark event as deleted */
                T->linear[i]->deleted = 1;
                revents[i] = 1;
                re++;
            /* label saving */

                if ( T->linear[i]->labeled &&
                    ( T->linear[i]->type == MSC_EV_IN_MESSAGE || T->linear[i]->type == MSC_EV_OUT_MESSAGE ) )
                {
                    iv = T->linear[i]->data.msg.indicator;

                    for ( lv = 0; lv < T->ev_Number; lv++ )
                    {
                        if ( ( lv != i ) &&
                            ( T->linear[lv]->type == MSC_EV_RECV_SIGNAL ||
                                T->linear[lv]->type == MSC_EV_SEND_SIGNAL ||
                                T->linear[lv]->type == MSC_EV_EXIST_SIGNAL ||
                                T->linear[lv]->type == MSC_EV_IN_MESSAGE ||
                                T->linear[lv]->type == MSC_EV_OUT_MESSAGE ) )
                            if ( T->linear[lv]->data.msg.indicator == iv )
                            {
                                if ( T->linear[lv]->labeled )
                                {
                                    MSG_PrintError
                                        ( "WARNING: semantic: file '%s(%u)': Ambiguous labels '%s' and '%s'\n",
                                        MSC_GetString( M->msc_File, M ), T->linear[lv]->line_Number,
                                        MSC_GetString( M->labels[T->linear[lv]->labeled - 1].label, M ),
                                        MSC_GetString( M->labels[T->linear[i]->labeled - 1].label, M ) );

                                /* prefer to remove generated time label */
                                    if ( M->labels[T->linear[i]->labeled - 1].generated )
                                    {
                                    /* duplicate label in output */
                                        M->labels[T->linear[i]->labeled - 1].label =
                                            M->labels[T->linear[lv]->labeled - 1].label;

                                    /* mark time label as unused */
                                        M->labels[T->linear[i]->labeled - 1].unused = 1;
                                    }
                                    else
                                    {
                                    /* duplicate label in output */
                                        M->labels[T->linear[lv]->labeled - 1].label =
                                            M->labels[T->linear[i]->labeled - 1].label;

                                    /* mark time label as unused */
                                        M->labels[T->linear[lv]->labeled - 1].unused = 1;
                                    }
                                }
                                else
                                    T->linear[lv]->labeled = T->linear[i]->labeled;

                            /* correct timing of events  */
                                T->linear[lv]->time_Calc.min_Time =
                                    MSC_MaxTime( T->linear[lv]->time_Calc.min_Time,
                                    T->linear[i]->time_Calc.min_Time );

                                T->linear[lv]->time_Calc.max_Time =
                                    MSC_MaxTime( T->linear[lv]->time_Calc.max_Time,
                                    T->linear[i]->time_Calc.max_Time );

                                break;
                            }
                    }
                }

            /* comment saving  */
                if ( T->linear[i]->comment &&
                    ( T->linear[i]->type == MSC_EV_IN_MESSAGE || T->linear[i]->type == MSC_EV_OUT_MESSAGE ) )
                {
                    iv = T->linear[i]->data.msg.indicator;

                    for ( lv = 0; lv < T->ev_Number; lv++ )
                    {
                        if ( ( lv != i ) &&
                            ( T->linear[lv]->type == MSC_EV_RECV_SIGNAL ||
                                T->linear[lv]->type == MSC_EV_SEND_SIGNAL ||
                                T->linear[lv]->type == MSC_EV_EXIST_SIGNAL ||
                                T->linear[lv]->type == MSC_EV_IN_MESSAGE ||
                                T->linear[lv]->type == MSC_EV_OUT_MESSAGE ) )
                            if ( T->linear[lv]->data.msg.indicator == iv )
                            {
                                if ( T->linear[lv]->comment )
                                {
                                    MSG_PrintError
                                        ( "WARNING: semantic: file '%s(%u)': Redefine commenting '%s' and '%s'\n",
                                        MSC_GetString( M->msc_File, M ), T->linear[lv]->line_Number,
                                        MSC_GetString( T->linear[lv]->comment, M ),
                                        MSC_GetString( T->linear[i]->comment, M ) );
                                }
                                else
                                    T->linear[lv]->comment = T->linear[i]->comment;

                                break;
                            }
                    }
                }

            }
        }
    }
    else
    {                           /* reduce only basic events */

        for ( i = 0; i < T->ev_Number; i++ )
        {
            if ( T->linear[i]->type == MSC_EV_CONCURRENT_START || T->linear[i]->type == MSC_EV_CONCURRENT_END )
            {
                revents[i] = 1;
                re++;
            }
        }
    }

    vertexes = T->ev_Number;

    adjacency = T->adjacency;

/* process only excluded events */

    if ( vertexes && re )
    {
        for ( l = 0, lv = 0; l < vertexes; l++ )
        {
            if ( revents[l] )
                for ( i = 0, iv = 0; i < vertexes; i++ )
                {
                    if ( adjacency[iv + l] )
                    {
                        or_string( adjacency + iv, adjacency + lv, vertexes );
                    }
                    iv += vertexes;
                }
            lv += vertexes;
        }
    }

/*    print_matrix(vertexes, adjacency); */
    /*************************************************************/
    if ( re )
        rearrange( T, revents, re );

/* process other events */
    vertexes = T->ev_Number;

    adjacency = T->adjacency;

/*  process nested frames */
    for ( i = 0; i < T->var_Number; i++ )
    {
        MSC_ReduceEventTree( M, T->frames[i], reduce, insys, inenv );
    }

    if ( revents )
    {
        MEM_Free( revents );
    }

    return MSC_ReduceEventTree( M, T->next, reduce, insys, inenv );
}
