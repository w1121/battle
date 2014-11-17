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
* Functions:    MSC_GenTestTTS
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  TTS ATS generation
*
* Notes:        None
*
******************************************************************************/
#include <portable.h>
#include <qmemory.h>
#include <mscbase.h>

#include <genpath.h>
#include <testtree.h>
#include <strproc.h>
#include <mscproc.h>
#include <mscstrs.h>

#include "utgflags.h"
#include "stategen.h"
#include "utrhead.h"
#include "sigopt.h"
#include <mscmess.h>
#include <outmess.h>

static MSC_STORAGE *MSC;
static size_t           tat_StateCount;
static MSC_TTS_ELEMENT *tat_State;
static size_t           tat_TagCount;
static size_t           tat_Branches;

static struct
{
    size_t                  t;

    struct MSC_TEST_CASE   *test;
    size_t                  c;
}*tat_Tags;

static size_t              tat_NextNodeCount;

static struct
{
    struct MSC_TEST_CASE   *test;
    struct MSC_TEST_CASE   *next;
}
                       *tat_NextNode;

static size_t             tat_LoopCount = 0;

static struct
{
    MSC_EVENT_KIND              *start;
    MSC_EVENT_KIND              *finish;
    size_t                  start_State;
    size_t                  finish_State;
}*tat_LoopData = NULL;

/***************************************************************************
 * Function : tat_AddLoopPair
 *
 * Description: add pair of loop events to pool
 *
 * Returns: none
 *
 * Notes: None
 *
 **************************************************************************/
static void tat_AddLoopPair( MSC_EVENT_KIND * x, MSC_EVENT_KIND * y )
{
    tat_LoopData = MEM_Reallocate( tat_LoopData, ( tat_LoopCount + 1 ) * sizeof ( tat_LoopData[0] ) );
    tat_LoopData[tat_LoopCount].start = x;
    tat_LoopData[tat_LoopCount].finish = y;
    tat_LoopCount++;
}

/***************************************************************************
 * Function : tat_MarkLoopStart
 *
 * Description: Mark loop event with state ID
 *
 * Returns: none
 *
 * Notes: None
 *
 **************************************************************************/
static void tat_MarkLoopStart( MSC_EVENT_KIND * x, size_t state )
{
    size_t                   i;

    for ( i = 0; i < tat_LoopCount; i++ )
        if ( tat_LoopData[i].start == x )
        {
            tat_LoopData[i].start_State = state;
            break;
        };
}

/***************************************************************************
 * Function : tat_MarkLoopEnd
 *
 * Description: Mark loop event with state ID
 *
 * Returns: none
 *
 * Notes: None
 *
 **************************************************************************/
static void tat_MarkLoopEnd( MSC_EVENT_KIND * x, size_t state )
{
    size_t                    i;

    for ( i = 0; i < tat_LoopCount; i++ )
        if ( tat_LoopData[i].finish == x )
        {
            tat_LoopData[i].finish_State = state;
            break;
        };
}

/***************************************************************************
 * Function : tat_GetLoopEnd
 *
 * Description: return finish state of loop
 *
 * Returns: state number
 *
 * Notes: None
 *
 **************************************************************************/
static size_t tat_GetLoopEnd( MSC_EVENT_KIND * start )
{
    size_t  i;

    for ( i = 0; i < tat_LoopCount; i++ )
        if ( tat_LoopData[i].start == start )
            return tat_LoopData[i].finish_State;

    return MSC_INFINITY_CONST;
}

/***************************************************************************
 * Function : tat_GetLoopStart
 *
 * Description: returns start state of loop
 *
 * Returns: state number
 *
 * Notes: None
 *
 **************************************************************************/
static size_t tat_GetLoopStart( MSC_EVENT_KIND * finish )
{
    size_t                    i;

    for ( i = 0; i < tat_LoopCount; i++ )
        if ( tat_LoopData[i].finish == finish )
            return tat_LoopData[i].start_State;

    return MSC_INFINITY_CONST;
}

/***************************************************************************
 * Function : tat_BuildState
 *
 * Description: build state from Super Tree
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL          tat_BuildState( struct MSC_TEST_CASENODE *x, struct MSC_TEST_CASE *X, size_t c );

/***************************************************************************
 * Function : tat_GetNextPart
 *
 * Description: return continuation of test case part
 *
 * Returns: pointer to continuation
 *
 * Notes: None
 *
 **************************************************************************/
static struct MSC_TEST_CASE *tat_GetNextPart( struct MSC_TEST_CASE *x )
{
    size_t   i;

    i = tat_NextNodeCount;

    while ( i )
    {
        i--;

        if ( tat_NextNode[i].test == x )
            return tat_NextNode[i].next;
    };

    return NULL;
}

/***************************************************************************
 * Function : tat_GetNextState
 *
 * Description: get next state number for test case part
 *
 * Returns: state number
 *
 * Notes: None
 *
 **************************************************************************/
static size_t tat_GetNextState( struct MSC_TEST_CASE *X, size_t tag )
{
    size_t                    i;

    if ( !X )
        return MSC_INFINITY_CONST;

    if ( tag == 0 )
    {
        X = tat_GetNextPart( X );

        if ( !X )
            return MSC_INFINITY_CONST;

    /* backward search */
        i = tat_TagCount;

        while ( i )
        {
            i--;

            if ( tat_Tags[i].test == X )
                return tat_Tags[i].c;
        };

        tat_TagCount++;
        tat_Tags = MEM_Reallocate( tat_Tags, tat_TagCount * sizeof ( tat_Tags[0] ) );
        tat_Tags[tat_TagCount - 1].test = X;
        tat_Tags[tat_TagCount - 1].c = tat_StateCount;
        tat_Tags[tat_TagCount - 1].t = 0;

        i = tat_StateCount;

        tat_State = MEM_Reallocate( tat_State, ( ++tat_StateCount ) * sizeof ( tat_State[0] ) );

    /* free fields */
        memset( &tat_State[i], 0, sizeof ( tat_State[0] ) );

        tat_BuildState( X->toller, X, i );
    }
    else
    {
        for ( i = 0; i < tat_TagCount; i++ )
        {
            if ( tat_Tags[i].t == tag )
                return tat_Tags[i].c;
        };

        tat_TagCount++;

        tat_Tags = MEM_Reallocate( tat_Tags, tat_TagCount * sizeof ( tat_Tags[0] ) );

        for ( i = 0; i < X->branch_Count; i++ )
        {
            if ( X->tags[i] == tag )
            {
                X = X->branch[i];
                i = 0x7fffff;
                break;
            };
        };

        if ( i == X->branch_Count )
        {
            fprintf( MSG_ErrorStream, MSG_INTERNAL_PROBLEM );
        };

    /* skip empty toller */
        while ( X && X->toller && !X->toller->next_Count )
            X = X->next;

        tat_Tags[tat_TagCount - 1].test = X;
        tat_Tags[tat_TagCount - 1].c = tat_StateCount;
        tat_Tags[tat_TagCount - 1].t = tag;

    /* process tag */
        i = tat_StateCount;

        tat_State = MEM_Reallocate( tat_State, ( ++tat_StateCount ) * sizeof ( tat_State[0] ) );

    /* free fields */
        memset( &tat_State[i], 0, sizeof ( tat_State[0] ) );
        tat_BuildState( X->toller, X, i );
    }

    return i;
}

/***************************************************************************
 * Function : tat_BuildState
 *
 * Description: fill state transitions from Super Tree
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_BuildState( struct MSC_TEST_CASENODE *x, struct MSC_TEST_CASE *X, size_t c )
{
    size_t                    i, pos;

    if ( !x )
        return RESULT_OK;

    if ( x->next_Count )
    {

        for ( i = 0; i < x->next_Count; i++ )
        {
            pos = tat_StateCount;
            tat_State = MEM_Reallocate( tat_State, ( ++tat_StateCount ) * sizeof ( tat_State[0] ) );
        /* free fields */
            memset( &tat_State[pos], 0, sizeof ( tat_State[0] ) );

            tat_State[c].Ev =
                MEM_Reallocate( tat_State[c].Ev,
                ( tat_State[c].transitions + 1 ) * sizeof ( tat_State[c].Ev[0] ) );
            tat_State[c].next_ID =
                MEM_Reallocate( tat_State[c].next_ID,
                ( tat_State[c].transitions + 1 ) * sizeof ( tat_State[c].next_ID[0] ) );
            tat_State[c].Ev[tat_State[c].transitions] = x->next[i]->E;

            /* printf("%d, %d, %p\n", i,c, tat_State[c].next_ID ); */
            if ( !x->next[i]->E )
            {
                size_t px;
                /* to avoid strange bug in VS .NET compiler.... */
                px = tat_GetNextState( X, x->next[i]->tag );
/*                printf("%d, %d, %p, %u\n", i,c, tat_State[c].next_ID, px ); */
                tat_State[c].next_ID[tat_State[c].transitions] = (int)px;
            }
                
            else
                tat_State[c].next_ID[tat_State[c].transitions] = (int)pos;

            tat_State[c].transitions++;

            tat_BuildState( x->next[i], X, pos );
        };
    }
    else
    {
        pos = tat_GetNextState( X, x->tag );
        tat_State[c].Ev =
            MEM_Reallocate( tat_State[c].Ev, ( tat_State[c].transitions + 1 ) * sizeof ( tat_State[c].Ev[0] ) );
        tat_State[c].next_ID =
            MEM_Reallocate( tat_State[c].next_ID,
            ( tat_State[c].transitions + 1 ) * sizeof ( tat_State[c].next_ID[0] ) );
        tat_State[c].Ev[tat_State[c].transitions] = NULL;
        tat_State[c].next_ID[tat_State[c].transitions] = (int)pos;
        tat_State[c].transitions++;
    };

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_AddNextPart
 *
 * Description: add continuation of test case part in buffer
 *
 * Returns: none
 *
 * Notes: None
 *
 **************************************************************************/
static void tat_AddNextPart( struct MSC_TEST_CASE *X, struct MSC_TEST_CASE *X2 )
{
    size_t                     i;

/* find if exists */
    i = tat_NextNodeCount;

    while ( i )
    {
        i--;

        if ( tat_NextNode[i].test == X )
        {
            tat_NextNode[i].next = X2;
            return;
        };
    };

    tat_NextNode = MEM_Reallocate( tat_NextNode, ( tat_NextNodeCount + 1 ) * sizeof ( tat_NextNode[0] ) );
    tat_NextNode[tat_NextNodeCount].test = X;
    tat_NextNode[tat_NextNodeCount].next = X2;
    tat_NextNodeCount++;
}

/***************************************************************************
 * Function : tat_BuildNextPart
 *
 * Description: build TTS from Super Tree part and continuation
 *
 * Returns: none
 *
 * Notes: None
 *
 **************************************************************************/
static void tat_BuildNextPart( struct MSC_TEST_CASE *X, struct MSC_TEST_CASE *gnext )
{
    size_t                    i;

/* add high-level links */

    while ( X )
    {

        if ( X->next )
        {
            for ( i = 0; i < X->branch_Count; i++ )
            {
                tat_BuildNextPart( X->branch[i], X->next );
            };

            tat_AddNextPart( X, X->next );
        }
        else
        {
            for ( i = 0; i < X->branch_Count; i++ )
            {
                tat_BuildNextPart( X->branch[i], gnext );
            };

            tat_AddNextPart( X, gnext );
        }

        X = X->next;
    };
}

/***************************************************************************
 * Function : tat_AddStateEvent
 *
 * Description: add new transition to state
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_AddStateEvent( MSC_STORAGE * M, MSC_TTS_ELEMENT * d, MSC_TTS_ELEMENT * s, size_t n )
{
    size_t                    i;

    for ( i = 0; i < d->transitions; i++ )
    {
        if ( MSC_CompareEvents( M, d->Ev[i], s->Ev[n] ) )
            return RESULT_OK;
    }

    d->Ev = MEM_Reallocate( d->Ev, ( d->transitions + 1 ) * sizeof ( d->Ev[0] ) );
    d->next_ID = MEM_Reallocate( d->next_ID, ( d->transitions + 1 ) * sizeof ( d->next_ID[0] ) );
    d->Ev[d->transitions] = s->Ev[n];
    d->next_ID[d->transitions] = s->next_ID[n];
    d->transitions++;

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_CompareState
 *
 * Description: returns RESULT_OK, if states are equal in terms of transitions
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_CompareState( size_t i, size_t j, MSC_STORAGE * M )
{
    size_t                    k, l, n = 0;

    for ( k = 0; k < tat_State[i].transitions; k++ )
    {
        for ( l = 0; l < tat_State[j].transitions; l++ )
        {
            if ( ( tat_State[i].next_ID[k] == tat_State[j].next_ID[l] ) &&
                MSC_CompareEvents( M, tat_State[i].Ev[k], tat_State[j].Ev[l] ) )
            {
                n++;
                break;
            };
        };

        if ( l == tat_State[j].transitions )
            return RESULT_ERR;
    };

    return ( n == tat_State[i].transitions ) && ( n == tat_State[j].transitions );
}

/***************************************************************************
 * Function : tat_BuildTTS
 *
 * Description: Build TTS from behavior Super Tree
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_BuildTTS( struct MSC_TEST_CASE *X, MSC_STORAGE * M )
{
    size_t                    i, j, bc;
    size_t                    *st_eq;
    int                     next_i, ch;

    tat_StateCount = 0;
    tat_State = NULL;
    MSC = M;
    tat_TagCount = 0;
    tat_Tags = NULL;

    tat_State = MEM_Allocate( sizeof ( MSC_TTS_ELEMENT ) );
    tat_StateCount = 1;

    tat_NextNode = NULL;
    tat_NextNodeCount = 0;

    tat_BuildNextPart( X, NULL );

    if ( !tat_BuildState( X->toller, X, 0 ) )
        return RESULT_ERR;

    tat_Branches = 0;

#define SDEBUG_
#ifndef SDEBUG
/* optimize links (reduce states) */
    i = tat_StateCount;

    tat_State[0].used = 1;
    while ( i )
    {
        i--;

        j = 0;

        while ( j < tat_State[i].transitions )
        {
        /* if current event is linked */

            if ( tat_State[i].next_ID[j] != -1 )
            {
                next_i = tat_State[i].next_ID[j];

                while ( (size_t) next_i != i && tat_State[i].next_ID[j] != -1 && tat_State[next_i].transitions == 1 &&
                    ( !tat_State[next_i].Ev[0] || tat_State[next_i].Ev[0]->type == MSC_EV_EMPTY ) )
                {
                    tat_State[i].next_ID[j] = tat_State[next_i].next_ID[0];
                    next_i = tat_State[i].next_ID[j];
                }
            };

        /*
           if (tat_State[i].next_ID[j]!=-1 && tat_State[tat_State[i].next_ID[j]].transitions==1 &&
           (!tat_State[tat_State[i].next_ID[j]].Ev[0] || 
           tat_State[tat_State[i].next_ID[j]].Ev[0]->type==MSC_EV_EMPTY )) tat_State[i].next_ID[j]=-1;
         */

        /*            if (link_states)
           { */
            if ( ( !tat_State[i].Ev[j] || tat_State[i].Ev[j]->type == MSC_EV_EMPTY ) &&
                tat_State[i].next_ID[j] != -1 )
            {
                size_t                     k;

                next_i = tat_State[i].next_ID[j];

                if ( (size_t) next_i != i &&
                    !( tat_State[i].Ev && tat_State[i].Ev[0] &&
                        tat_State[i].Ev[0]->type == MSC_EV_START_LOOP ) )
                {
                    memcpy( &( tat_State[i].Ev[j] ), &( tat_State[i].Ev[j + 1] ),
                        ( tat_State[i].transitions - j - 1 ) * sizeof ( tat_State[0].Ev[0] ) );
                    memcpy( &( tat_State[i].next_ID[j] ), &( tat_State[i].next_ID[j + 1] ),
                        ( tat_State[i].transitions - j - 1 ) * sizeof ( tat_State[0].next_ID[0] ) );
                    tat_State[i].transitions--;
                    j--;

                    for ( k = 0; k < tat_State[next_i].transitions; k++ )
                    {
                        tat_AddStateEvent( M, &tat_State[i], &tat_State[next_i], k );
                    }
                }
            }

        /*            }; */
            j++;
        };

    /* mark used states */
        for ( j = 0; j < tat_State[i].transitions; j++ )
        {
            if ( tat_State[i].next_ID[j] != -1 )
                tat_State[tat_State[i].next_ID[j]].used = 1;
        };

    };

/*
   if (optimize_states)
   { */
    ch = 1;

/* remove identical states */
    st_eq = MEM_Allocate( tat_StateCount * sizeof ( st_eq[0] ) );

    while ( ch )
    {
        ch = 0;

        for ( i = 0; i < tat_StateCount; i++ )
        {
            st_eq[i] = i;

            if ( tat_State[i].used )
            {
                for ( j = 0; j < i; j++ )
                {
                    if ( tat_State[j].used && tat_State[i].transitions == tat_State[j].transitions &&
                        tat_CompareState( i, j, M ) )
                    {
                        st_eq[i] = j;
                        tat_State[i].used = 0;
                        ch = 1;
                        break;
                    }
                }
            }
        };

        for ( i = 0; i < tat_StateCount; i++ )
        {
            if ( tat_State[i].used )
            {
                for ( j = 0; j < tat_State[i].transitions; j++ )
                {
                    if ( tat_State[i].next_ID[j] != -1 )
                        tat_State[i].next_ID[j] = (int)st_eq[tat_State[i].next_ID[j]];
                }
            }
        };
    };

    MEM_Free( st_eq );

/*    } */

/* optimize table */
    next_i = 0;                 /* new_number */

    for ( i = 0; i < tat_StateCount; i++ )
    {
        if ( !tat_State[i].used ||
            ( tat_State[i].transitions <= 1 &&
                ( !tat_State[i].Ev[0] || tat_State[i].Ev[0]->type == MSC_EV_EMPTY ) ) )
            continue;

        tat_State[i].state_ID = next_i++;
    }

#else
    for ( i = 0; i < tat_StateCount; i++ )
    {
        tat_State[i].state_ID = i;
    }

#endif

/* set tat_Branches field */
    for ( i = 0; i < tat_StateCount; i++ )
    {
        if ( !tat_State[i].used ||
            ( tat_State[i].transitions <= 1 &&
                ( !tat_State[i].Ev[0] || tat_State[i].Ev[0]->type == MSC_EV_EMPTY ) ) )
            continue;

        bc = 0;

        for ( j = 0; j < tat_State[i].transitions; j++ )
        {
        /* is_passive/ is_timer !! */

            if ( ( tat_State[i].Ev[j] && tat_State[i].Ev[j]->type != MSC_EV_RECV_SIGNAL &&
                    tat_State[i].Ev[j]->type != MSC_EV_TIMER_TIMEOUT ) || !tat_State[i].Ev[j] )
                bc++;

            if ( tat_State[i].Ev[j] )
            {
                if ( tat_State[i].Ev[j]->type == MSC_EV_START_LOOP )
                    tat_MarkLoopStart( tat_State[i].Ev[j], i );

                if ( tat_State[i].Ev[j]->type == MSC_EV_END_LOOP )
                    tat_MarkLoopEnd( tat_State[i].Ev[j], i );
            }
        };

        if ( bc > 1 )
        {
            tat_State[i].brc = tat_Branches;
            tat_Branches++;
        }
    };

    if ( tat_Tags )
        MEM_Free( tat_Tags );

    if ( tat_NextNode )
        MEM_Free( tat_NextNode );

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_FindLoopPair
 *
 * Description: find loop event pairs in MSC
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_FindLoopPair( MSC_STORAGE * M, MSC_EVENT_KIND * x )
{
    MSC_EVENT_KIND              *y;

    if ( x->type == MSC_EV_START_LOOP )
    {
        y = MSC_FindEndEventChain( x );

        if ( y->type == MSC_EV_END_LOOP )
            tat_AddLoopPair( x, y );
    };

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_MarkLoops
 *
 * Description: search loop event pairs in MSC
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_MarkLoops( MSC_STORAGE * M )
{
    return MSC_Process( M, tat_FindLoopPair );
}

/***************************************************************************
 * Function : MSC_GenCommandTTS
 *
 * Description: Generate TTS-specific frame for command
 *
 * Returns: None
 *
 * Notes: None
 *
 **************************************************************************/
void MSC_GenCommandTTS( MSC_STORAGE * M, MSC_EVENT_KIND * x, int next, FILE * out )
{
    size_t                    i, s, pas, v;

    fputs( "[", out );

    MSC_AtsGenerateCommand( M, x, out );

/* output next state */
    fprintf( out, " %d ", ( next != -1 ) ? tat_State[next].state_ID : -1 );

/* output TTS-specific parameters for events */
    if (x)
    {
        if ( x->type == MSC_EV_START_LOOP )
        {
            v = tat_GetLoopEnd( x );
            s = tat_State[tat_GetLoopStart( tat_State[v].Ev[0] )].next_ID[0];
            pas = 0;

            for ( i = 0; i < tat_State[s].transitions; i++ )
            {
                if ( MSC_IsPassiveEvent( tat_State[s].Ev[i] ) )
                    pas++;
            };
            fprintf( out, " %d %u",
                ( tat_State[v].next_ID[0] == -1 ) ? -1 : tat_State[tat_State[v].next_ID[0]].state_ID, pas );

        }
        else if ( x->type == MSC_EV_END_LOOP )
        {
            fprintf( out, " %d ", tat_State[tat_GetLoopStart( x )].state_ID );
        }
    }
    fputs( "]\\\n", out );
}

/***************************************************************************
 * Function : MSC_GenTestTTS
 *
 * Description: Generate TTS from X and output in Out
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/

LOGICAL MSC_GenTestTTS( FILE * Out, MSC_STORAGE * M, struct MSC_TEST_CASE *X, char *prefix )
{
    size_t                    i, j, passive, active, timers, logicals;

    tat_LoopCount = 0;
    tat_LoopData = NULL;

    tat_MarkLoops( M );

    tat_BuildTTS( X, M );

    MSC_AtsGenTestCaseHeader( Out, M, X, tat_Branches, prefix, 1 );

/* print states */

    for ( i = 0; i < tat_StateCount; i++ )
    {
    /* skip empty events */
#ifndef SDEBUG

        if ( !tat_State[i].used ||
            ( tat_State[i].transitions <= 1 &&
                ( !tat_State[i].Ev[0] || tat_State[i].Ev[0]->type == MSC_EV_EMPTY ) ) )
            continue;

#endif

        passive = 0;

        timers = 0;

        logicals = 0;

        for ( j = 0; j < tat_State[i].transitions; j++ )
        {
            if ( MSC_IsPassiveEvent( tat_State[i].Ev[j] ) )
            {
                ( tat_State[i].Ev[j]->type == MSC_EV_CONDITION ) ? logicals++ : passive++;
            }
            else if ( tat_State[i].Ev[j] && tat_State[i].Ev[j]->type == MSC_EV_TIMER_TIMEOUT )
                timers++;
        };

        active = tat_State[i].transitions - passive - timers - logicals;

    /* branch case */
        fprintf( Out, "State %u %u %u %u %u %u [list \\\n", tat_State[i].state_ID, logicals, timers, passive,
            active, tat_State[i].brc );


        if ( logicals )
        {
            for ( j = 0; j < tat_State[i].transitions; j++ )
            {
                if ( MSC_IsPassiveEvent( tat_State[i].Ev[j] ) &&
                    ( tat_State[i].Ev[j] && (tat_State[i].Ev[j]->type == MSC_EV_CONDITION) ) )
                {
                    MSC_GenCommandTTS( M, tat_State[i].Ev[j], tat_State[i].next_ID[j], Out );
                }
            }
        }

        if ( timers )
        {
            for ( j = 0; j < tat_State[i].transitions; j++ )
            {
                if ( tat_State[i].Ev[j] && tat_State[i].Ev[j]->type == MSC_EV_TIMER_TIMEOUT )
                {
                    MSC_GenCommandTTS( M, tat_State[i].Ev[j], tat_State[i].next_ID[j], Out );
                }
            }
        }

        if ( passive )
        {
            for ( j = 0; j < tat_State[i].transitions; j++ )
            {
                if ( MSC_IsPassiveEvent( tat_State[i].Ev[j] ) &&
                    ( tat_State[i].Ev[j] && (tat_State[i].Ev[j]->type != MSC_EV_CONDITION) ) )
                {
                    MSC_GenCommandTTS( M, tat_State[i].Ev[j], tat_State[i].next_ID[j], Out );
                }
            }
        }

        if ( active )
        {
            for ( j = 0; j < tat_State[i].transitions; j++ )
            {
                if ( ( !MSC_IsPassiveEvent( tat_State[i].Ev[j] ) && !MSC_IsTimerEvent( tat_State[i].Ev[j] ) ) ||
                    ( !tat_State[i].Ev[j] ) )
                {
                    MSC_GenCommandTTS( M, tat_State[i].Ev[j], tat_State[i].next_ID[j], Out );
                }
            }
        }

        fprintf( Out, "]\n\n" );    /* end of state command */

    };

    MSC_AtsGenTestCaseFooter( Out, M, prefix );

/* free memory */
    for ( i = 0; i < tat_StateCount; i++ )
    {
        if ( tat_State[i].next_ID )
            MEM_Free( tat_State[i].next_ID );

        if ( tat_State[i].Ev )
            MEM_Free( tat_State[i].Ev );
    };

    if ( tat_LoopData )
        MEM_Free( tat_LoopData );

    MEM_Free( tat_State );

    return RESULT_OK;
}
