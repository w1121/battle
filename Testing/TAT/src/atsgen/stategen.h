#ifndef STATEGEN_H
#define STATEGEN_H
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
#include <mscbase.h>
#include <testtree.h>
#include <genpath.h>

typedef struct
{
    size_t                  state_ID;  /* ID of current state */
    size_t                  transitions;    /* number of possible transiotions */
    MSC_EVENT_KIND             **Ev;        /* transion's events */
    int                    *next_ID;   /* next ID's */
    size_t                    brc;
    size_t                    used;
} MSC_TTS_ELEMENT;

extern LOGICAL          MSC_GenTestTTS( FILE * out,
    MSC_STORAGE * M, struct MSC_TEST_CASE *X, char *prefix );

#endif
