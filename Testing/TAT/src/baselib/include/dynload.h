#ifndef MSC_DYNLOAD_H
#define MSC_DYNLOAD_H
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

#ifdef __cplusplus
extern                  "C"
{
#endif                                 /* C++ */

typedef void * DYNLIB_HANDLE;
typedef void * DYNLIB_PROC_ADDR;


/***************************************************************************
* Function : Dyn_LoadLibrary
*
* Description: Load DLL/SO library
*
* Returns: handle to library
*
* Notes: None
*
**************************************************************************/
extern DYNLIB_HANDLE Dyn_LoadLibrary( const char * name );


/***************************************************************************
* Function : Dyn_CloseLibrary
*
* Description: Unload DLL/SO
*
* Returns: RESULT_ERR/RESULT_OK
*
* Notes: None
*
**************************************************************************/
extern LOGICAL Dyn_CloseLibrary( DYNLIB_HANDLE handle );


/***************************************************************************
* Function : Dyn_GetProcAddr
*
* Description: get the address of function in DLL by name
*
* Returns: address of procedure or NULL
*
* Notes: None
*
**************************************************************************/
extern DYNLIB_PROC_ADDR Dyn_GetProcAddr( const DYNLIB_HANDLE handle, const char * name );

#ifdef __cplusplus
}
#endif                                 /* C++ */

#endif /* MSC_DYNLOAD_H */
