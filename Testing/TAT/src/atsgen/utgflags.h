#ifndef UTG_FLAGS_H
#define UTG_FLAGS_H
/******************************************************************************
*
*       COPYRIGHT 2001-2004 MOTOROLA, ALL RIGHTS RESERVED
*
*       The code is the property of Motorola St.Petersburg Software Development
*       and is Motorola Confidential Proprietary Information.
*
*       The copyright notice above does not evidence any
*       actual or extern Boolended publication of such source code.
*
* Functions:    None
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  UTG command line parameter flags
*
* Notes:        None
*
******************************************************************************/
#include <portable.h>

extern int             MSC_SingleMessTable;    /* single message table flag */
extern int             MSC_OutCalcTime;    /* calculate time, and output calculated */
extern int             MSC_CountMaxPath;   /* calculate upper-bound of paths in test case */

#endif                         /* UTG_FLAGS_H */
