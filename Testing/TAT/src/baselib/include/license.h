#ifndef MSC_LICENSE_H
#define MSC_LICENSE_H
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
* Description:  TAT+ license manager
*
* Notes:        None
*
******************************************************************************/
#include <portable.h>
#ifdef __cplusplus
extern                  "C"
{
#endif                                 /* C++ */

#ifdef MSC_USE_LICENSE
#define MSC_LICENSE_FILE    "sic.key"
#define MSC_ENV_VAR         "SIC_PATH"

#define MSC_LICENSE_SIZE    320
#define MSC_LICENSE_HEADER 0
#define MSC_LICENSE_HEADER_SIZE 16
#define MSC_LICENSE_YEAR   16
#define MSC_LICENSE_YEAR_SIZE   4
#define MSC_LICENSE_MONTH   42
#define MSC_LICENSE_MONTH_SIZE 2
#define MSC_LICENSE_DAY     66
#define MSC_LICENSE_DAY_SIZE 2
#define MSC_LICENSE_COMPANY  90
#define MSC_LICENSE_COMPANY_SIZE 64
#define MSC_LICENSE_USER     176
#define MSC_LICENSE_USER_SIZE 64
#define MSC_LICENSE_EMAIL     244
#define MSC_LICENSE_EMAIL_SIZE 64

#define MSC_LICENSE_MSC_ZeroStr 20
#define MSC_LICENSE_FNAME_LEN  154
#define MSC_LICENSE_ID_LEN     240
#define MSC_LICENSE_ID_LEN_SIZE 4
#define MSC_LICENSE_FNAME_LEN_SIZE 4

#endif /*MSC_USE_LICENSE*/
extern char             MSC_CodedChars[];

/***************************************************************************
 * Function : MSC_GetLicense
 *
 * Description: check license correctness and initialize data
 *
 * Returns: RESULT_OK / aborts execution
 *
 * Notes: None
 *
 **************************************************************************/
extern LOGICAL          MSC_GetLicense( char *execpath );

#ifdef __cplusplus
}
#endif                                 /* C++ */

#endif /* MSC_LICENSE_H */
