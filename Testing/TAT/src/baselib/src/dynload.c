/******************************************************************************
*
*       COPYRIGHT 2001-2004 MOTOROLA, ALL RIGHTS RESERVED
*
*       MOTOROLA CONFIDENTIAL PROPRIETARY
*
* Description:  Command-line processing
*
* $RCSfile: dynload.c,v $
* $Revision: 1.3 $
* $Source: /home/cvsusr/cvssoft/tat_tools/baselib/src/dynload.c,v $
* $Date: 2004/03/05 14:37:44 $
*
*
* CHANGE HISTORY
*                                                       
* Initial version 
*
******************************************************************************/
#include <portable.h>
#include <dynload.h>
#include <outmess.h>

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
DYNLIB_HANDLE Dyn_LoadLibrary( const char * name )
{
    DYNLIB_HANDLE res;

#if OSTYPE==1
    char * lpMsgBuf = NULL;


    res = LoadLibraryA(name);
    
    if (!res)
    {
        FormatMessageA( 0x00001300, NULL, GetLastError(),  MAKELANGID(0x00, 0x01), &lpMsgBuf,  0, NULL );
        MSG_PrintError("ERROR: file '%s': %s\n", name, lpMsgBuf);
        LocalFree(lpMsgBuf);
    }
    return res;
#else
     res = dlopen(name, RTLD_NOW);
     if (!res)
     {
         MSG_PrintError("ERROR: file '%s': %s\n", name, dlerror());
     }
     return res;
#endif
}

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
LOGICAL Dyn_CloseLibrary( DYNLIB_HANDLE handle )
{
    if (!handle) return RESULT_ERR;
#if OSTYPE==1
    return FreeLibrary(handle);
#else
    return dlclose(handle);
#endif
}

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
DYNLIB_PROC_ADDR Dyn_GetProcAddr( const DYNLIB_HANDLE handle, const char * name )
{
    if (!handle || !name) return NULL;
#if OSTYPE==1
    return GetProcAddress(handle, name);
#else
    return dlsym(handle, name);
#endif

}
