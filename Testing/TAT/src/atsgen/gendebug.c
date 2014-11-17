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
* Functions:    main
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  Abstract Test Generator
*
* Notes:        None
*
******************************************************************************/
#include <portable.h>
#include <qmemory.h>
#include <mscbase.h>

#include <fileproc.h>
#include <strproc.h>

#include <partial.h>
#include <genpath.h>
#include <mscparse.h>
#include <mscproc.h>
#include <msctime.h>
#include <outmess.h>
#include <cmdline.h>
#include <testtree.h>
#include <flowgen.h>
#include <xmlconf.h>
#include <confproc.h>
#include <mscstrs.h>
#include <tatgdata.h>

/***************************************************************************
 * Function : MSC_GenDebug
 *
 * Description: Generate debug info
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_GenDebug( TAT_XML_CONFIG_DEF * configdata, MSC_STORAGE * M )
{
    FILE                   *out;
    char                    path[TAT_MAX_FNAME_LEN];
    char                    fname[TAT_MAX_FNAME_LEN];
    size_t                  i, j, k;
    int                     data_type;
    char                   *dataclass;

    FP_SplitPath( MSC_GetStr( M->msc_File, M ), path, fname, NULL );
    if ( fname[0] == 0 )
    {
        return RESULT_ERR;
    }

    strcat( fname, ".dbg" );
    strcat( path, fname );
    out = FP_OpenFile( path, "w" );
    if ( !out )
        return RESULT_ERR;
    fputs( "<MSC_DEBUG_INFO>\n", out );
    fputs( "\t<FILES>\n", out );

    if ( M->src_Count )
    {
        for ( i = 0; i < M->src_Count; i++ )
        {
            fprintf( out, "\t\t<LEVEL level=\"%u\">\n", M->src_Files[i].level );

        /* remove quotes from name */
            strcpy( fname, MSC_GetStr( M->src_Files[i].link, M ) );

            k = strlen( fname );
            if ( fname[0] == '\"' )
            {
                if ( fname[k - 1] == '\"' )
                {
                    fname[k - 1] = 0;
                    k--;
                }
                memmove( fname, fname + 1, k );
            }
        /* change slashes */
            if ( k )
            {
                do
                {
                    if ( fname[k] == '\\' )
                        fname[k] = '/';
                    k--;
                } while ( k );
            }

            StrEmbedXMLLine( path, fname );

            fprintf( out, "\t\t\t<NAME start=\"%u\" end=\"%u\" path=\"%s\"/>\n",
                M->src_Files[i].first_Line + ( i << 16 ), M->src_Files[i].last_Line + ( i << 16 ), path );

            fputs( "\t\t</LEVEL>\n", out );
        }

    }

    fputs( "\t</FILES>\n", out );

    fputs( "\t<OBJECTS>\n", out );

    for ( i = 0; i < M->var_Count; i++ )
    {
        dataclass = MSC_GetString( M->vars[i].pv_TypeSrc, M );
        data_type = CFG_GetDataTypeID( dataclass );
        if ( data_type != -1 && configdata->data[data_type].data_Class != MSC_DATA_UNKNOWN )
        {
            dataclass = CFG_GetDataClassTypeStr( configdata->data[data_type].data_Class );
        }
        fprintf( out, "\t\t<OBJ name=\"%s\" type=\"%s\" storage=\"Variable.%s\" />\n",
            MSC_GetString( M->vars[i].pv_Name, M ), dataclass, MSC_GetString( M->vars[i].pv_Name, M ) );
    }

    for ( i = 1; i < M->label_Count; i++ )
    {
        if ( !M->labels[i].unused )
            fprintf( out, "\t\t<OBJ name=\"%s\" type=\"integer\" storage=\"TimeVariable.%s\" />\n",
                MSC_GetString( M->labels[i].label, M ), MSC_GetString( M->labels[i].label, M ) );
    }

    for ( i = 0; i < configdata->signal_Count; i++ )
    {
        fprintf( out, "\t\t<OBJ name=\"%s\" type=\"bool\" storage=\"SignalParam.%s\" />\n",
            configdata->signals[i].name, configdata->signals[i].name );

        for ( j = 0; j < configdata->signals[i].param_Count; j++ )
        {
            dataclass = configdata->signals[i].param_TgtType[j];
            data_type = CFG_GetDataTypeID( configdata->signals[i].source_Format[j] );
            if ( data_type != -1 && configdata->data[data_type].data_Class != MSC_DATA_UNKNOWN )
            {
                dataclass = CFG_GetDataClassTypeStr( configdata->data[data_type].data_Class );
            }

            fprintf( out, "\t\t<OBJ name=\"%s.%s\" type=\"%s\" storage=\"SignalParam.%s.%s\" />\n",
                configdata->signals[i].name, configdata->signals[i].param_FieldName[j],
                dataclass, configdata->signals[i].name, configdata->signals[i].param_FieldName[j] );
        }
    }

    fputs( "\t</OBJECTS>\n", out );

    fputs( "</MSC_DEBUG_INFO>\n", out );
    fclose( out );
    return RESULT_OK;
}
