/******************************************************************************
*
*       COPYRIGHT 2001-2004 MOTOROLA, ALL RIGHTS RESERVED
*
*       MOTOROLA CONFIDENTIAL PROPRIETARY
*
* Description:  Command-line processing
*
* $RCSfile: parsereg.c,v $
* $Revision: 1.3 $
* $Source: /home/cvsusr/cvssoft/tat_tools/baselib/src/parsereg.c,v $
* $Date: 2004/03/05 14:37:44 $
*
*
* CHANGE HISTORY
*                                                       
* Initial version 
*
******************************************************************************/
#include <portable.h>
#include <mscbase.h>
#include <mscparse.h>
#include <mscproc.h>
#include <qmemory.h>
#include <fileproc.h>
#include <strproc.h>
#include <outmess.h>
#include <mscmess.h>
#include <mscstrs.h>
#include <tatgdata.h>

#include <qmemory.h>
#include <mscgen.h>
#include <flowgen.h>


typedef struct _PTK_FORMAT_COLL
{
    char * format_Ext;
    MSC_SEQ_FILE_PROC read_File;
    MSC_SEQ_FILE_PROC write_File;
} PTK_FORMAT_COLL;

static size_t tat_FormatCount = 0;
static PTK_FORMAT_COLL * tat_FormatColl = NULL;

/* predefined IDs */
size_t MSC_MprFormatID = 0;
size_t MSC_FGFormatID = 1;

/***************************************************************************
 * Function : tat_FreeFormatInfo
 *
 * Description: Free memory allocated for storing file format information
 *
 * Returns: none
 *
 * Notes: None
 *
 **************************************************************************/
static void tat_FreeFormatInfo(void)
{
    UINT32_T i;
    for (i=0; i<tat_FormatCount; i++)
    {
        MEM_Free(tat_FormatColl[i].format_Ext);
    }
    tat_FormatCount = 0;
    MEM_Free(tat_FormatColl);
    tat_FormatColl = NULL;
}

/***************************************************************************
 * Function : MSC_RegisterFormat
 *
 * Description: Add file format to collection
 *
 * Returns: 
 *
 * Notes: None
 *
 **************************************************************************/
size_t MSC_RegisterFormat(const char * ext, MSC_SEQ_FILE_PROC readf, MSC_SEQ_FILE_PROC writef)
{
    size_t i;

    for (i=0; i<tat_FormatCount; i++)
    {
        if (!stricmp(ext, tat_FormatColl[i].format_Ext))
        {
            tat_FormatColl[i].read_File = readf;
            tat_FormatColl[i].write_File = writef;
            return RESULT_OK;
        }
    }

    /* first time register function for memory free */
    if (!tat_FormatColl)
    {
        atexit(tat_FreeFormatInfo);
    }

    tat_FormatColl = MEM_Reallocate(tat_FormatColl, (i+1)*sizeof(tat_FormatColl[0]) );
    tat_FormatColl[i].format_Ext = MEM_CopyStr(ext);
    tat_FormatColl[i].read_File = readf;
    tat_FormatColl[i].write_File = writef;

    /* set up predefined formats */
    if (!stricmp(ext,STR_DOTMPR))
        MSC_MprFormatID = i;
    if (!stricmp(ext,STR_DOTFG))
        MSC_FGFormatID = i;

    tat_FormatCount++;
    return i;
}

/***************************************************************************
 * Function : TAT_Load
 *
 * Description: Load file with test definition
 *
 * Returns: NULL / sequence of events
 *
 * Notes: None
 *
 **************************************************************************/
MSC_STORAGE * TAT_Load(const char * name, MSC_STORAGE * data, const UINT32_T flags)
{
    char                    fext[TAT_MAX_FNAME_LEN];
    MSC_STORAGE        *msc;
    size_t                  i;
    LOGICAL                 res = RESULT_ERR;

    if ( !name || name[0] == 0 )
    {
        return NULL;
    }

    /* get file extension */
    FP_SplitPath( name, NULL, NULL, fext );

    if ( !data )
        msc = MEM_Allocate( sizeof ( MSC_STORAGE ) );
    else
    {
        /* clean destination data */
        memset( data, 0, sizeof ( data[0] ) );
        msc = data;
    }

    MSG_PushProcessedFile( name );

    /* search for format type */
    for (i = tat_FormatCount - 1; (int)i>=0; i--)
    {
        if (!stricmp(fext, tat_FormatColl[i].format_Ext))
        {
            if (tat_FormatColl[i].read_File(name, msc, flags))
            {
                msc->source_Format = i;
                res = RESULT_OK;
            }
            else
            {
                MSC_FreeMemory( msc );
            }
            break;
        }
    }
    if ((int)i<0)
    {
        MSG_SemanticError(MSG_UNABLE_DETERMINE_FILEFORMAT);
    }
    MSG_PopProcessedFile(  );

    if (!data && res==RESULT_ERR)
    {
        MEM_Free(msc);
    }
    
    return (res==RESULT_OK)?msc:NULL;
}


/***************************************************************************
 * Function : TAT_Save
 *
 * Description: save MSC on disk in format,specified by source file
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL TAT_Save( const char * name, MSC_STORAGE * M, const UINT32_T flags )
{
    char                    fname[TAT_MAX_FNAME_LEN];
    char *                 name_s;

    FP_SplitPath( name, NULL, NULL, fname );

    name_s = name;

    if ( fname[0] == 0 )
    {
    /* no extension specified */
        strcpy( fname, name );
        strcat( fname, tat_FormatColl[M->source_Format].format_Ext);
        name_s = &fname[0];
    }

    return tat_FormatColl[M->source_Format].write_File(name, M, flags);
}

/***************************************************************************
* Function : PTK_SaveFormat
*
* Description: save MSC on disk in format,specified by source file
*
* Returns: RESULT_ERR/RESULT_OK
*
* Notes: None
*
**************************************************************************/
LOGICAL PTK_SaveFormat( const char * name, MSC_STORAGE * M, const UINT32_T flags, const UINT32_T fspec )
{
    char                    fname[TAT_MAX_FNAME_LEN];
    char *                 name_s;

    FP_SplitPath( name, NULL, NULL, fname );

    name_s = name;

    if ( fname[0] == 0 )
    {
        /* no extension specified */
        strcpy( fname, name );
        strcat( fname, tat_FormatColl[fspec].format_Ext);
        name_s = &fname[0];
    }

    return tat_FormatColl[M->source_Format].write_File(name, M, flags);
}
