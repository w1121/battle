#ifndef FLOWGEN_H
#define FLOWGEN_H
/******************************************************************************
*
*       COPYRIGHT 2001-2004 MOTOROLA, ALL RIGHTS RESERVED
*
*       MOTOROLA CONFIDENTIAL PROPRIETARY
*
* Description:  Command-line processing
*
* $RCSfile: flowgen.h,v $
* $Revision: 1.3 $
* $Source: /home/cvsusr/cvssoft/tat_tools/baselib/include/flowgen.h,v $
* $Date: 2004/03/05 14:37:40 $
*
*
* CHANGE HISTORY
*                                                       
* Initial version 
*
******************************************************************************/
#include <portable.h>

#ifdef __cplusplus
extern                  "C"
{
#endif                                 /* C++ */

    extern LOGICAL          FG_Load( const char * filename, MSC_STORAGE * M, const UINT32_T comb );

    extern char *            FG_GenerateLine( char *dest, MSC_EVENT_KIND * E, MSC_STORAGE * M, const UINT32_T flags );

    extern LOGICAL          FG_Save( const char * filename, MSC_STORAGE * M, const UINT32_T flags );

#ifdef __cplusplus
}

#endif                                 /* C++ */

#endif
