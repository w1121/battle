#ifndef MSCTIME_H
#define MSCTIME_H
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
* Functions:    MSC_CalcTime
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  MSC Time calculation
*
* Notes:        None
*
******************************************************************************/
#include <mscbase.h>

#ifdef __cplusplus
extern                  "C"
{
#endif                                 /* C++ */

/***************************************************************************
 * Function : MSC_AddTime_,MSC_MulTime_,MSC_AddMulTime_,MSC_CompareTime_,
 *            MSC_MinTime_, MSC_MaxTime_
 *
 * Description: fast macro for time algebraic operations
 *
 * Returns: time
 *
 * Notes: None
 *
 **************************************************************************/
#define MSC_AddTime_(x, y) ((x==MSC_INFINITY_CONST || y==MSC_INFINITY_CONST)?MSC_INFINITY_CONST:x+y)

#define MSC_MulTime_(x, y) ((x==MSC_INFINITY_CONST || y==MSC_INFINITY_CONST)?MSC_INFINITY_CONST:x*y)

#define MSC_AddMulTime_(x, y, z) (MSC_AddTime_(x, MSC_MulTime_(y, z)))

#define MSC_CompareTime_(x, y) ((x<y)?(-1):((x==y)?0:1))

#define MSC_MinTime_(x, y) ((x<=y)?x:y)

#define MSC_MaxTime_(x, y) ((x>y)? x:y)

/* time calculation specificators */
#define MSCTIME_USE_DURATION        0x00000001
#define MSCTIME_DEFINED_START       0x00000002
#define MSCTIME_ZERO_PAR_TIME       0x00000004
#define MSCTIME_TRUNCATE            0x00000008
#define MSCTIME_TRANS_LABEL         0x00000010

/***************************************************************************
 * Function : MSC_AddTime
 *
 * Description: calculate sum of two times with overflow control
 *
 * Returns: time
 *
 * Notes: None
 *
 **************************************************************************/
    extern MSC_TIME_VALUE           MSC_AddTime( MSC_TIME_VALUE x, MSC_TIME_VALUE y );

/***************************************************************************
 * Function : MSC_MulTime
 *
 * Description: calculates multiplication of two times with 
 *              overflow control
 *
 * Returns: time
 *
 * Notes: None
 *
 **************************************************************************/
    extern MSC_TIME_VALUE           MSC_MulTime( MSC_TIME_VALUE x, MSC_TIME_VALUE y );

/***************************************************************************
 * Function : MSC_AddMulTime
 *
 * Description: calculates x+(y*z) with overflow control
 *
 * Returns: time
 *
 * Notes: None
 *
 **************************************************************************/
    extern MSC_TIME_VALUE           MSC_AddMulTime( MSC_TIME_VALUE x, MSC_TIME_VALUE y, MSC_TIME_VALUE z );

/***************************************************************************
 * Function : MSC_CompareTime
 *
 * Description: compare two times
 *
 * Returns: time
 *
 * Notes: None
 *
 **************************************************************************/
    extern int              MSC_CompareTime( MSC_TIME_VALUE x, MSC_TIME_VALUE y );

/***************************************************************************
 * Function : MSC_MinTime
 *
 * Description: get minimal time
 *
 * Returns: time
 *
 * Notes: None
 *
 **************************************************************************/
    extern MSC_TIME_VALUE           MSC_MinTime( MSC_TIME_VALUE x, MSC_TIME_VALUE y );

/***************************************************************************
 * Function : MSC_MaxTime
 *
 * Description: get maximal time 
 *
 * Returns: time
 *
 * Notes: None
 *
 **************************************************************************/
    extern MSC_TIME_VALUE           MSC_MaxTime( MSC_TIME_VALUE x, MSC_TIME_VALUE y );

/***************************************************************************
 * Function : MSC_CalcTime
 *
 * Description: Calculate and verify time specifications in MSC
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          MSC_CalcTime( MSC_STORAGE * M, int flags );

#ifdef __cplusplus
}

#endif                                 /* C++ */
#endif                                 /* MSCTIME_H */
