#ifndef SIGOPT_H
#define SIGOPT_H
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
* Functions:    MSC_MapMessageID
*               MSC_FreeSigOpt
*               MSC_FreeSigMap
*               MSC_FillMessageMap
*               
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  Optimize signal definitions
*
* Notes:        None
*
******************************************************************************/

#include <mscbase.h>
#include <partial.h>
#include <testtree.h>

typedef struct
{
    char                   *signame;
    size_t                  par_count;
    char                   *params;
}

MSC_UNIQUE_MESSAGES;

typedef struct
{
    size_t                    original;
    size_t                    new_num;
}

MSC_UNIQUE_MAP;

extern size_t             MSC_UniqMessCountATS;
extern size_t             MSC_UniqMapCountATS;
extern MSC_UNIQUE_MESSAGES *MSC_UniqMessStorage;
extern MSC_UNIQUE_MAP  *MSC_UniqMapStorage;

extern size_t           MSC_MapMessageID( size_t key );

extern LOGICAL          MSC_FreeSigOpt(  );

extern LOGICAL          MSC_FreeSigMap(  );

extern LOGICAL          MSC_FillMessageMap( MSC_STORAGE * M );

#endif                         /* SIGOPT_H */
