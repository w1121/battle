#ifndef TATCALC_H
#define TATCALC_H
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
* Functions:    MSC_Calc
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  TAT+ symbolic calculator
*
* Notes:        None
*
******************************************************************************/
#ifdef __cplusplus
extern                  "C"
{
#endif                                 /* C++ */

#include <mscbase.h>

/***************************************************************************
 * Function : MSC_Calc
 *
 * Description: Compute, if possible, expression 'expr', save result in 'result'
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL          MSC_Calc( char * expr, double *result );

/***************************************************************************
 * Function : MSC_CalcMacro
 *
 * Description: check macro syntax
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
    extern LOGICAL MSC_CalcMacro( char *expr );

#ifdef __cplusplus
}
#endif                                 /* C++ */

#endif                                 /* TATCALC_H */
