/******************************************************************************
*
*       COPYRIGHT 2001-2002 MOTOROLA, ALL RIGHTS RESERVED
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
* Description:  Generate ATS header/footer
*
* Notes:        None
*
******************************************************************************/

#include <portable.h>
#include <mscbase.h>

#include <mscparse.h>
#include <qmemory.h>
#include <strproc.h>
#include <fileproc.h>
#include <cmdline.h>
#include <outmess.h>
#include <cfgtypes.h>
#include <confproc.h>
#include <tatgdata.h>

#include "utrhead.h"

/***************************************************************************
 * Function : MSC_AtsTCL_gen_signal_list
 *
 * Description: generates Tcl template signal list
 *
 * Returns: RESULT_OK, RESULT_ERR
 *
 * Notes: None
 *
 **************************************************************************/
int MSC_AtsTCL_gen_signal_list( FILE * out, TAT_XML_CONFIG_DEF * conf )
{
    size_t                    i, j;
    char                    outline[TAT_MAX_LINE_LEN];

    if ( !conf || !out )
        return RESULT_ERR;

    fprintf( out, 
        "set SIGNAL_NAME 0\n"
        "set SIGNAL_TYPE 1\n"
        "set SIGNAL_ATTR 2\n"
        "set SIGNAL_PARAMS 3\n"
        "set SIGNAL_STYPES 4\n"
        "set SIGNAL_FIELDS 5\n"
        "set SIGNAL_PARAMTYPE 6\n"
        "set SIGNAL_VARARG 7\n"
        "set SIGNAL_PRIORITY 8\n"
        "set SIGNAL_RECORD_SIZE 9\n" );

    fprintf( out, "set SignalSet [list \\\n" );

    /*-- moving over SDL structure --*/
    for ( i = 0; i < conf->signal_Count; i++ )
    {

        fprintf( out, "\"%s\"  \"%s\"  [list", conf->signals[i].name,
            MSC_SignalType2Str( conf->signals[i].type ) );

        for ( j = 0; j < conf->signals[i].attr_Count; j++ )
        {
            StrEmbedLineQuotes( outline, conf->signals[i].attr[j] );
            fprintf( out, " %s ", outline );
        };

    /* output source data type's names */
        fprintf( out, " ] [ list" );

        for ( j = 0; j < conf->signals[i].param_Count; j++ )
        {
            if ( !conf->signals[i].source_Format[j] )
                return RESULT_ERR;
            StrEmbedLineQuotes( outline, conf->signals[i].source_Format[j] );

            fprintf( out, " %s", outline );
        }

    /* output source language data type's names */
        fprintf( out, " ] [ list" );

        for ( j = 0; j < conf->signals[i].param_Count; j++ )
        {
            if ( !conf->signals[i].source_Format[j] )
                return RESULT_ERR;

            fprintf( out, " %d", CFG_GetDataTypeID( conf->signals[i].source_Format[j] ) );
        }

    /* output field's names */
        fprintf( out, " ] [ list" );

        for ( j = 0; j < conf->signals[i].param_Count; j++ )
        {
            if ( !conf->signals[i].param_FieldName[j] )
                return RESULT_ERR;

            fprintf( out, " %s", conf->signals[i].param_FieldName[j] );
        }

        fprintf( out, " ] \"%s\" %u %u\\\n",
            ( conf->signals[i].attr_Type ) ? conf->signals[i].attr_Type : "",
            conf->signals[i].param_Extra, conf->signals[i].priority );
    }

    fprintf( out, "]\n" );

    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_AtsTCL_gen_includes
 *
 * Description: generate a list with included file names
 *
 * Returns: RESULT_OK, RESULT_ERR
 *
 * Notes: None
 *
 **************************************************************************/
int MSC_AtsTCL_gen_includes( FILE * out, size_t num,  char **includes )
{
    size_t                     i, j;

    if ( !out )
        return RESULT_ERR;

    fprintf( out, "set IncludeFiles [ list " );

    for ( i = 0; i < num; i++ )
    {
        j = 0;

        while ( includes[i][j] )
        {
            if ( includes[i][j] == '\\' )
                includes[i][j] = '/';

            j++;
        }

        fprintf( out, "\"%s\" ", includes[i] );
    }

    fprintf( out, " ]\n" );
    return RESULT_OK;
}

/***************************************************************************
* Function : MSC_AtsTCL_StartTestSuite
*
* Description: Tcl start test suite header
*
* Returns: RESULT_OK, RESULT_ERR
*
* Notes: None
*
**************************************************************************/
LOGICAL MSC_AtsTCL_gen_instances( FILE * out, size_t num, TAT_INSTANCE_DEF * inst )
{
    size_t                      i, j;
    char                        outline[TAT_MAX_LINE_LEN];

    fprintf( out, "set INSTANCE_NAME 0\n"
        "set INSTANCE_TYPE 1\n"
        "set INSTANCE_ATTR 2\n"
        "set INSTANCE_PARAMS 3\n"
        "set INSTANCE_STYPES 4\n"
        "set INSTANCE_FIELDS 5\n"
        "set INSTANCE_PARAMTYPE 6\n"
        "set INSTANCE_VARARG 7\n"
        "set INSTANCE_PRIORITY 8\n"
        "set INSTANCE_RECORD_SIZE 9\n" );

    fprintf( out, "set InstanceSet [ list \\\n" );

    for ( i = 0; i < num; i++ )
    {
        fprintf( out, " \"%s\" \"%s\" [ list ", inst[i].name, MSC_InstanceType2Str( inst[i].type ) );

        for ( j = 0; j < inst[i].attr_Count; j++ )
        {
            StrEmbedLineQuotes( outline, inst[i].attr[j] );
            fprintf( out, "%s ", outline );
        };

    /* output target language data type's names */
        fprintf( out, " ] [ list" );

        for ( j = 0; j < inst[i].param_Count; j++ )
        {
            if ( !inst[i].param_TgtType[j] )
                return RESULT_ERR;

            StrEmbedLineQuotes( outline, inst[i].param_TgtType[j] );

            fprintf( out, " %s", outline );
        }

    /* output source language data type's names */
        fprintf( out, " ] [ list" );

        for ( j = 0; j < inst[i].param_Count; j++ )
        {
            if ( !inst[i].source_Format[j] )
                return RESULT_ERR;

            fprintf( out, " %d", CFG_GetDataTypeID( inst[i].source_Format[j] ) );
        }

    /* output field's names */
        fprintf( out, " ] [ list" );

        for ( j = 0; j < inst[i].param_Count; j++ )
        {
            if ( !inst[i].param_FieldName[j] )
                return RESULT_ERR;

            fprintf( out, " %s", inst[i].param_FieldName[j] );
        }

        fprintf( out, " ] \"%s\" %u %u\\\n", ( inst[i].attr_Type ) ? inst[i].attr_Type : "",
            inst[i].param_Extra, inst[i].priority );
    };

    fprintf( out, " ]\n" );     /* close INSTANCES */

    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_AtsTCL_gen_test_names
 *
 * Description: Tcl start test suite header
 *
 * Returns: RESULT_OK, RESULT_ERR
 *
 * Notes: None
 *
 **************************************************************************/
int MSC_AtsTCL_gen_test_names( size_t msc_count, char **names, char *prefix, FILE * out )
{
    size_t                    i;
    char                    f_name[TAT_MAX_FNAME_LEN];

    if ( !msc_count )
        return RESULT_OK;

    if ( !out || !names )
        return RESULT_ERR;

    fprintf( out, "set TestCaseNames [list" );

    for ( i = 0; i < msc_count; i++ )
    {
        FP_SplitPath( names[i], NULL, f_name, NULL );
        fprintf( out, " \"%s%s\"", ( prefix ) ? prefix : "", f_name );
    }

    fprintf( out, "]\n" );
    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_AtsTCL_gen_udf_table
 *
 * Description: Tcl start test suite header
 *
 * Returns: RESULT_OK, RESULT_ERR
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_AtsTCL_gen_udf_table( TAT_XML_CONFIG_DEF * u, FILE * out )
{
    size_t                  i, j;
    char                    d1[TAT_MAX_ID_LEN];
	
    if ( !u || !out )
        return RESULT_ERR;

    fprintf( out, "set DATATYPE_NAME 0\n"
		          "set DATATYPE_TYPE 1\n"
				  "set DATATYPE_DIMENSION 2\n"
				  "set DATATYPE_STYPE 3\n"
                  "set DATATYPE_ATTR 4\n"
                  "set DATATYPE_ENUM 5\n"
                  "set DATATYPE_PARAMTYPE 6\n"
                  "set DATATYPE_DATACLASS 7\n"
				  "set DATATYPE_MEMBERS 8\n"
                  "set DATATYPE_RECORD_SIZE 9\n" );

    fprintf( out, "set DataTypeSet [list \\\n" );

    for ( i = 0; i < u->data_Count; i++ )
    {
        StrEmbedLineQuotes( d1, u->data[i].name );
		fprintf( out, "\t\t%s ", d1 );
		StrEmbedLineQuotes( d1, u->data[i].target_Name );
		fprintf( out, "%s ", d1);
		if(u->data[i].dimension)
		{
			fprintf( out, "\"%s\" ",u->data[i].dimension );
		}
		else
		{
			fprintf( out, "\"\" ");
		}
		fprintf(out,"%d [list ",CFG_GetDataTypeID(u->data[i].target_Name));

        /* output attributes */
        for ( j = 0; j < u->data[i].attr_Count; j++ )
        {
            StrEmbedLineQuotes( d1, u->data[i].attr[j] );
            fprintf( out, "%s ", d1 );
        }
        fprintf( out, "] [ list " );

        /* output possible value and parameters */
        for ( j = 0; j < u->data[i].value_Count; j++ )
        {
            StrEmbedLineQuotes( d1, u->data[i].value_List[j] );
            fprintf( out, "%s ", d1 );
            StrEmbedLineQuotes( d1, u->data[i].enums_List[j] );
            fprintf( out, "%s ", d1 );
        }
        /* output type and data class */
        fprintf( out, "] \"%s\" \"%s\" [list", ( u->data[i].attr_Type ) ? u->data[i].attr_Type : "",
            CFG_GetDataClassTypeStr( u->data[i].data_Class ));

		/* output members */
		for(j=0;j<u->data[i].member_Count;j++)
			if(u->data[i].member_List[j].dimension)
				fprintf(out, " [list \"%s\" \"%s\" \"%s\" %d]",u->data[i].member_List[j].name,u->data[i].member_List[j].type,u->data[i].member_List[j].dimension,CFG_GetDataTypeID(u->data[i].member_List[j].type));
			else
				fprintf(out, " [list \"%s\" \"%s\" \"\" %d]",u->data[i].member_List[j].name,u->data[i].member_List[j].type,CFG_GetDataTypeID(u->data[i].member_List[j].type));
		fprintf(out,"]\\\n");

    }

    fputs("]\n", out);
    return RESULT_OK;
}

/***************************************************************************
 * Function : MSC_AtsTCL_StartTestSuite
 *
 * Description: Tcl start test suite header
 *
 * Returns: RESULT_OK, RESULT_ERR
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_AtsTCL_StartTestSuite( FILE * Out,
    int argc,
    char *argv[],
    char *configuration,
    char *suitename,
    size_t MSC_count,
    char **mscnames, size_t INCL_count, char **includenames, TAT_XML_CONFIG_DEF * conf, char *prefix )
{
    int                     i;
    char                    str_buffer[TAT_MAX_LINE_LEN];
    LOGICAL                 err = RESULT_OK;

    fprintf( Out, "###############################################################################\n"
        "#\n"
        "# COPYRIGHT 2001-2004 MOTOROLA, ALL RIGHTS RESERVED\n"
        "#\n"
        "# Description: Abstract Test Suite '%s'\n"
        "#\n" "###############################################################################\n", suitename );
    fprintf( Out, "# command line: '" );

    for ( i = 0; i < argc; i++ )
    {
        fprintf( Out, "%s ", argv[i] );
    };

    fprintf( Out, "'\n\n" );

    if ( !MSC_AtsTCL_gen_instances( Out, conf->instance_Count, conf->instances ) )
        err = RESULT_ERR;

    if ( !MSC_AtsTCL_gen_signal_list( Out, conf ) )
        err = RESULT_ERR;

    if ( !MSC_AtsTCL_gen_test_names( MSC_count, mscnames, prefix, Out ) )
        err = RESULT_ERR;

    if ( !MSC_AtsTCL_gen_udf_table( conf, Out ) )
        err = RESULT_ERR;

    if ( !MSC_AtsTCL_gen_includes( Out, INCL_count, includenames ) )
        err = RESULT_ERR;

    StrEmbedLineQuotes( str_buffer, configuration );

    fprintf( Out, "set CONFIGURATION %s\n", str_buffer );

    fprintf( Out, "set ATS_EVENT_TYPE 0\n"
        "set ATS_EVENT_TIME 1\n"
        "set ATS_EVENT_OTIME 2\n"
        "set ATS_EVENT_PTIME 3\n"
        "set ATS_EVENT_COND 4\n"
        "set ATS_EVENT_INST 5\n"
        "set ATS_EVENT_VERD 6\n"

        "set ATS_EVENT_SREF 7\n"
        "set ATS_EVENT_COMMENT 8\n"
        "set ATS_EVENT_LABEL 9\n"
        "set ATS_EVENT_DATA 10\n" );

    fprintf( Out, "\nStartTestSuite \"%s\"\n", suitename );

    return err;
}

/***************************************************************************
 * Function : MSC_AtsTCL_FinishTestSuite
 *
 * Description: Tcl start test suite header
 *
 * Returns: RESULT_OK, RESULT_ERR
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL MSC_AtsTCL_FinishTestSuite( FILE * Out,
    int argc,
    char *argv[],
    char *configuration,
    char *suitename,
    size_t MSC_count,
    char **mscnames,
    size_t INCL_count, char **includenames, TAT_XML_CONFIG_DEF * conf, char *prefix )
{
    if ( !suitename || !Out )
        return RESULT_ERR;

    fprintf( Out, "\nEndTestSuite \"%s\"\n", suitename );

    return RESULT_OK;
}
