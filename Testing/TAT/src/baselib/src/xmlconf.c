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
* Functions:    TAT_ReadXML
*               TAT_FreeConfig
*               TAT_BuildConfig               
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  XML configuration loader and analyser
*
* Notes:        None
*
******************************************************************************/
#include <portable.h>
#include <qmemory.h>
#include <strproc.h>
#include <fileproc.h>
#include <xmlconf.h>
#include <cfgtypes.h>
#include <mscstrs.h>
#include <mscparse.h>
#include <outmess.h>
#include <mscmess.h>
#include <tatgdata.h>
#include <xmlparse.h>
#include <xmlmess.h>


typedef enum
{
    TATXML_START,           /* initial state */
    TATXML_PROJECT,         /* PROJECT tag */
    TATXML_CONFIG,          /* CONFIG tag */
    TATXML_INSTANCES,       /* INSTANCES tag */
    TATXML_INSTANCE,        /* INSTANCE tag */
    TATXML_ATTRIBUTE,       /* ATTRIBUTE tag everywhere */
    TATXML_PARAMETER,       /* PARAMETER tag everywhere */
    TATXML_SIGNALS,         /* SIGNALS tag */
    TATXML_SIGNAL,          /* SIGNAL tag */
    TATXML_MACROS,          /* MACROS tag */
    TATXML_MACRO,           /* MACRO tag */
    TATXML_VALUELIST,       /* VALUELIST tag */
    TATXML_VALUE,           /* VALUE tag */
    TATXML_DATATYPE,        /* DATATYPE tag */
    TATXML_TYPEDEF,         /* TYPEDEF tag */
    TATXML_AXIOMS,          /* AXIOMS tag */
    TATXML_AXIOM,           /* AXIOM tag */
    TATXML_INITIAL,         /* INITIAL tag */
	TATXML_MEMBER,			/* MEMBER tag */
	TATXML_SPECFILES,		/* SPECFILES tag */
	TATXML_SET,				/* SET tag */
	TATXML_FILE,			/* FILE tag */
	TATXML_TOOLS,			/* TOOLS tag */
	TATXML_TOOL,			/* TOOL tag */
	TATXML_INPUT,			/* INPUT tag */
	TATXML_OUTPUT,			/* OUTPUT tag */
	TATXML_TOOLCONFIG,		/* TOOLCONFIG tag */
	TATXML_EXEC,			/* EXEC tag */
	TATXML_ATTACHIN,		/* ATTACHIN tag */
	TATXML_ATTACHOUT,		/* ATTACHOUT tag */
	TATXML_TESTSUITE,		/* TESTSUITE tag */
	TATXML_TESTCASE,		/* TESTCASE tag */
	TATXML_PIPE,			/* PIPE tag */
	TATXML_DATAIN,			/* DATAIN tag */
	TATXML_DATAOUT,			/* DATAOUT tag */
}
TAT_CONFIG_STATE;

/* define tags representation in XML */
static XML_TAGLIST tat_XMLtags[]={
    {TATXML_PROJECT, "TATPROJECT"},
    {TATXML_CONFIG, "CONFIG"},
    {TATXML_INSTANCES,"INSTANCES"},
    {TATXML_INSTANCE,"INSTANCE"},
    {TATXML_ATTRIBUTE, "ATTRIBUTE"},
    {TATXML_PARAMETER, "PARAMETER"},
    {TATXML_SIGNALS,"SIGNALS"},
    {TATXML_SIGNAL,"SIGNAL"},
    {TATXML_MACROS,"MACROS"},
    {TATXML_MACRO,"MACRO"},
    {TATXML_VALUELIST,"VALUELIST"},
    {TATXML_VALUE,"VALUE"},
    {TATXML_DATATYPE,"DATATYPE"},
    {TATXML_TYPEDEF,"TYPEDEF"},
    {TATXML_AXIOMS,"AXIOMS"},
    {TATXML_AXIOM,"AXIOM"},
    {TATXML_INITIAL,"INITIAL"},
	{TATXML_MEMBER,"MEMBER"},
	{TATXML_SPECFILES,"SPECFILES"},
	{TATXML_SET,"SET"},
	{TATXML_FILE,"FILE"},
	{TATXML_TOOLS,"TOOLS"},
	{TATXML_TOOL,"TOOL"},
	{TATXML_INPUT,"INPUT"},
	{TATXML_OUTPUT,"OUTPUT"},
	{TATXML_TOOLCONFIG,"TOOLCONFIG"},
	{TATXML_EXEC,"EXEC"},
	{TATXML_ATTACHIN,"ATTACHIN"},
	{TATXML_ATTACHOUT,"ATTACHOUT"},
	{TATXML_TESTSUITE,"TESTSUITE"},
	{TATXML_TESTCASE,"TESTCASE"},
	{TATXML_PIPE,"PIPE"},
	{TATXML_DATAIN,"DATAIN"},
	{TATXML_DATAOUT,"DATAOUT"}
};

#define tat_tagCount sizeof(tat_XMLtags)/sizeof(tat_XMLtags[0])

/***************************************************************************
 * Function : tat_parseProject
 *
 * Description: parse <PROJECT> tag
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_parseProject(const XML_ELEMENT element_id, const XML_TAG *xmldata, void * outdata)
{
    TAT_XML_CONFIG * config;
    if (!outdata) return RESULT_ERR;

    config = (TAT_XML_CONFIG *) outdata;
    memset(config, 0, sizeof(config[0]));

    config->name = MEM_CopyStr(XML_GetAttrValue(STR_XML_NAME, xmldata));

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_parseConfig
 *
 * Description: parse <CONFIG> tag
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_parseConfig(const XML_ELEMENT element_id, const XML_TAG *xmldata, void * outdata)
{
    TAT_XML_CONFIG * data = (TAT_XML_CONFIG *) outdata;
    size_t  i;
    char * name;

    if (!outdata) return RESULT_ERR;

    name = XML_GetAttrValue( STR_XML_NAME, xmldata );

    if (!name) 
    {
        MSG_XMLError(MSG_XML_REQUIRED_ATTR_s, STR_XML_NAME);
        return RESULT_ERR;
    }

    if (!StrCheckIDName(name))
    {
        MSG_XMLError(MSG_XML_INVALID_ATTR_s, STR_XML_NAME);
        return RESULT_ERR;
    }

/* check for duplicate config name */
    for ( i = 0; i < data->config_Count; i++ )
    {
        if ( !stricmp( name, data->config[i].name ) )
        {
            MSG_XMLError(MSG_XML_DUPLICATED_ID_s, name);
            return RESULT_ERR;
        }
    };
    
    MEM_ADD_OBJECT ( data->config, data->config_Count);
    
    data->config[data->config_Count].line_Number = xmldata->line_number;

    data->config[data->config_Count].name = MEM_CopyStr( name );

    /* if 'name' is empty, add 'default' string */
    if ( !data->config[data->config_Count].name )
        data->config[data->config_Count].name = MEM_CopyStr( STR_XML_DEFAULT );

    data->config[data->config_Count].base = MEM_CopyStr( XML_GetAttrValue( STR_XML_BASE, xmldata ) );

    if (data->config[data->config_Count].base && !StrCheckIDName(data->config[data->config_Count].base))
    {
        MSG_XMLError(MSG_XML_INVALID_ATTR_s, STR_XML_BASE);
        return RESULT_ERR;
    }

    data->config_Count++;

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_parseEmpty
 *
 * Description: parse <CONFIG> tag
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_parseEmpty(const XML_ELEMENT element_id, const XML_TAG *xmldata, void * outdata)
{
    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_parseInstance
 *
 * Description: parse <INSTANCE> tag
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_parseInstance(const XML_ELEMENT element_id, const XML_TAG *xmldata, void * outdata)
{
    TAT_XML_CONFIG * data = (TAT_XML_CONFIG *) outdata;
    size_t  i;
    char * name;
    TAT_XML_CONFIG_DEF * conf;  /* current <CONFIG> tag */
    if (!outdata) return RESULT_ERR;
    conf = &data->config[data->config_Count-1];
    
    name = XML_GetAttrValue( STR_XML_NAME, xmldata );

    if (!name) 
    {
        MSG_XMLError(MSG_XML_REQUIRED_ATTR_s, STR_XML_NAME);
        return RESULT_ERR;
    }

    if (!StrCheckIDName(name))
    {
        MSG_XMLError(MSG_XML_INVALID_ATTR_s, STR_XML_NAME);
        return RESULT_ERR;
    }

    /*StrToUpper( name );*/

    for ( i = 0; i < conf->instance_Count; i++ )
    {
        if ( !strcmp( conf->instances[i].name, name ) )
        {
            MSG_XMLError(MSG_XML_DUPLICATED_ID_s, name);
            return RESULT_ERR;
        }
    };

    MEM_ADD_OBJECT(conf->instances, conf->instance_Count);

    conf->instances[i].line_Number = xmldata->line_number;

    conf->instances[i].name = MEM_CopyStr( name );

    conf->instances[i].type = CFG_GetInstanceType( XML_GetAttrValue( STR_XML_TYPE, xmldata ) );

    if ( conf->instances[i].type == MSC_INSTANCE_UNDEF )
    {
        MSG_XMLError(MSG_XML_INVALID_ATTR_s, STR_XML_TYPE);
        return RESULT_ERR;
    }

    conf->instances[i].attr_Type = MEM_CopyStr( XML_GetAttrValue( STR_XML_PARAMSTYPE, xmldata ) );

    name = XML_GetAttrValue( STR_XML_PRIORITY, xmldata );

    if ( name )
    {
        if ( !StrIsIntegerNumber( name ) )
        {
            MSG_XMLError(MSG_XML_INVALID_ATTR_s, STR_XML_PRIORITY);
            return RESULT_ERR;
        }
        else
        {
            conf->instances[i].priority = atoi( name );

            if ( conf->instances[i].priority < MSC_MIN_PRIORITY )
                conf->instances[i].priority = MSC_MIN_PRIORITY;

            if ( conf->instances[i].priority > MSC_MAX_PRIORITY )
                conf->instances[i].priority = MSC_MAX_PRIORITY;
        }
    }
    else
        conf->instances[i].priority = MSC_DEFAULT_PRIORITY;

    conf->instance_Count++;
    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_parseInstanceAttr
 *
 * Description: parse <INSTANCE>::<ATTRIBUTE> tag
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_parseInstanceAttr(const XML_ELEMENT element_id, const XML_TAG *xmldata, void * outdata)
{
    TAT_XML_CONFIG * data = (TAT_XML_CONFIG *) outdata;
    size_t  item;
    char * name;
    TAT_XML_CONFIG_DEF * conf;  /* current <CONFIG> tag */
    if (!outdata) return RESULT_ERR;
    conf = &data->config[data->config_Count-1];
    item = conf->instance_Count-1;

    name = XML_GetAttrValue( STR_XML_VALUE, xmldata );

    MEM_ADD_OBJECT (conf->instances[item].attr, conf->instances[item].attr_Count);

    conf->instances[item].attr[conf->instances[item].attr_Count] = MEM_CopyStr( name );
    conf->instances[item].attr_Count++;

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_parseInstanceParm
 *
 * Description: parse <INSTANCE>::<PARAMETER> tag
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_parseInstanceParm(const XML_ELEMENT element_id, const XML_TAG *xmldata, void * outdata)
{
    TAT_XML_CONFIG * data = (TAT_XML_CONFIG *) outdata;
    size_t  i, item;
    char * name;
    TAT_XML_CONFIG_DEF * conf;  /* current <CONFIG> tag */
    if (!outdata) return RESULT_ERR;
    conf = &data->config[data->config_Count-1];
    item = conf->instance_Count-1;

    name = XML_GetAttrValue( STR_XML_TYPE, xmldata );

    if ( !name )
    {
        MSG_XMLError(MSG_XML_REQUIRED_ATTR_s, STR_XML_TYPE);
        return RESULT_ERR;
    }

    if ( (!StrCheckSDLName( name ) && strcmp( name, "..." ) ) || conf->instances[item].param_Extra )
    {
        MSG_XMLError(MSG_XML_INVALID_ATTR_s, STR_XML_TYPE);
        return RESULT_ERR;
    }

    if ( !strcmp( name, "..." ) )
    {
        conf->instances[item].param_Extra = 1;
    }
    else
    {
    /* allocate memory for SDL type name */
        MEM_ADD_OBJECT(conf->instances[item].source_Format, conf->instances[item].param_Count);
    /* allocate memory for target type name */
        MEM_ADD_OBJECT(conf->instances[item].param_TgtType, conf->instances[item].param_Count);
    /* allocate memory for field name */
        MEM_ADD_OBJECT(conf->instances[item].param_FieldName, conf->instances[item].param_Count);

        conf->instances[item].source_Format[conf->instances[item].param_Count] = MEM_CopyStr( name );
        conf->instances[item].param_TgtType[conf->instances[item].param_Count] = MEM_CopyStr( name );

        name = XML_GetAttrValue( STR_XML_FIELD, xmldata );

        if ( name )
        {
            if ( !StrCheckSDLName( name ) )
            {
                MSG_XMLError(MSG_XML_INVALID_ATTR_s, STR_XML_FIELD);
                return RESULT_ERR;
            }

        /* Check for duplicated message parameter ID */
            for ( i = 0; i < conf->instances[item].param_Count; i++ )
            {
                if ( !strcmp( conf->instances[item].param_FieldName[i], name ) )
                {
                    MSG_XMLError(MSG_XML_DUPLICATED_ID_s, name);
                    return RESULT_ERR;
                }
            }
            conf->instances[item].param_FieldName[i] = MEM_CopyStr( name );
        }
        else
        {
            size_t   k = conf->instances[item].param_Count;

        /* use predefined field name */
            name = MEM_Allocate( 16 );
            do
            {
            /* try to generated unique name */
                sprintf( name, "x%u", k );

            /* Check for duplicated message parameter ID */
                for ( i = 0; i < conf->instances[item].param_Count; i++ )
                {
                    if ( !strcmp( conf->instances[item].param_FieldName[i], name ) )
                    {
                        k++;
                        break;
                    }
                }

            }
            while ( i != conf->instances[item].param_Count );
            conf->instances[item].param_FieldName[conf->instances[item].param_Count] = name;
        }
        conf->instances[item].param_Count++;
    }
    return RESULT_OK;
}


/***************************************************************************
 * Function : tat_parseSignal
 *
 * Description: parse <SIGNAL> tag
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_parseSignal(const XML_ELEMENT element_id, const XML_TAG *xmldata, void * outdata)
{
    TAT_XML_CONFIG * data = (TAT_XML_CONFIG *) outdata;
    size_t  i, item;
    char * name;
    TAT_XML_CONFIG_DEF * conf;  /* current <CONFIG> tag */
    if (!outdata) return RESULT_ERR;
    conf = &data->config[data->config_Count-1];
    item = conf->signal_Count;


    name = XML_GetAttrValue( STR_XML_NAME, xmldata );
    
    if ( !name )
    {
        MSG_XMLError(MSG_XML_REQUIRED_ATTR_s, STR_XML_NAME);
        return RESULT_ERR;
    }

    if (!StrCheckIDName( name ) )
    {
        MSG_XMLError(MSG_XML_INVALID_ATTR_s, STR_XML_NAME);
        return RESULT_ERR;
    }

    /* StrToUpper( name ); */

    for ( i = 0; i < item; i++ )
    {
        if ( !strcmp( conf->signals[i].name, name ) )
        {
            MSG_XMLError(MSG_XML_DUPLICATED_ID_s, name);
            return RESULT_ERR;
        }
    };
    
    MEM_ADD_OBJECT( conf->signals, item);
    
    conf->signals[i].line_Number = xmldata->line_number;

    conf->signals[i].name = MEM_CopyStr( name );
    
    conf->signals[i].type = CFG_GetSignalType( XML_GetAttrValue( STR_XML_TYPE, xmldata ) );

    if ( conf->signals[i].type == MSC_SIG_UNDEFINED )
    {
        MSG_XMLError(MSG_XML_INVALID_ATTR_s, STR_XML_TYPE);
        return RESULT_ERR;
    }

    conf->signals[i].attr_Type = MEM_CopyStr( XML_GetAttrValue( STR_XML_PARAMSTYPE, xmldata ) );

    name = XML_GetAttrValue( STR_XML_PRIORITY, xmldata );

    if ( name )
    {
        if ( !StrIsIntegerNumber( name ) )
        {
            MSG_XMLError(MSG_XML_INVALID_ATTR_s, STR_XML_PRIORITY);
            return RESULT_ERR;
        }
        else
        {
            conf->signals[i].priority = atoi( name );

            if ( conf->signals[i].priority < MSC_MIN_PRIORITY )
                conf->signals[i].priority = MSC_MIN_PRIORITY;

            if ( conf->signals[i].priority > MSC_MAX_PRIORITY )
                conf->signals[i].priority = MSC_MAX_PRIORITY;
        }
    }
    else
        conf->signals[i].priority = MSC_DEFAULT_PRIORITY;

    conf->signal_Count++;

    return RESULT_OK;
}


/***************************************************************************
 * Function : tat_parseSignalAttr
 *
 * Description: parse <SIGNAL>::<ATTRIBUTE> tag
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_parseSignalAttr(const XML_ELEMENT element_id, const XML_TAG *xmldata, void * outdata)
{
    TAT_XML_CONFIG * data = (TAT_XML_CONFIG *) outdata;
    size_t  item;
    char * name;
    TAT_XML_CONFIG_DEF * conf;  /* current <CONFIG> tag */
    if (!outdata) return RESULT_ERR;
    conf = &data->config[data->config_Count-1];
    item = conf->signal_Count-1;

    name = XML_GetAttrValue( STR_XML_VALUE, xmldata );

    MEM_ADD_OBJECT (conf->signals[item].attr, conf->signals[item].attr_Count);

    conf->signals[item].attr[conf->signals[item].attr_Count] = MEM_CopyStr( name );
    conf->signals[item].attr_Count++;

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_parseSignalParm
 *
 * Description: parse <SIGNAL>::<PARAMETER> tag
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_parseSignalParm(const XML_ELEMENT element_id, const XML_TAG *xmldata, void * outdata)
{
    TAT_XML_CONFIG * data = (TAT_XML_CONFIG *) outdata;
    size_t  i, item;
    char * name;
    TAT_XML_CONFIG_DEF * conf;  /* current <CONFIG> tag */
    if (!outdata) return RESULT_ERR;
    conf = &data->config[data->config_Count-1];
    item = conf->signal_Count-1;

    name = XML_GetAttrValue( STR_XML_TYPE, xmldata );

    if ( !name )
    {
        MSG_XMLError(MSG_XML_REQUIRED_ATTR_s, STR_XML_TYPE);
        return RESULT_ERR;
    }

    if ( (!StrCheckSDLName( name ) && strcmp( name, "..." ) ) || conf->signals[item].param_Extra )
    {
        MSG_XMLError(MSG_XML_INVALID_ATTR_s, STR_XML_TYPE);
        return RESULT_ERR;
    }

    if ( !strcmp( name, "..." ) )
    {
        conf->signals[item].param_Extra = 1;
    }
    else
    {
    /* allocate memory for SDL type name */
        MEM_ADD_OBJECT(conf->signals[item].source_Format, conf->signals[item].param_Count);
    /* allocate memory for target type name */
        MEM_ADD_OBJECT(conf->signals[item].param_TgtType, conf->signals[item].param_Count);
    /* allocate memory for field name */
        MEM_ADD_OBJECT(conf->signals[item].param_FieldName, conf->signals[item].param_Count);

        conf->signals[item].source_Format[conf->signals[item].param_Count] = MEM_CopyStr( name );
        conf->signals[item].param_TgtType[conf->signals[item].param_Count] = MEM_CopyStr( name );

        name = XML_GetAttrValue( STR_XML_FIELD, xmldata );

        if ( name )
        {
            if ( !StrCheckSDLName( name ) )
            {
                MSG_XMLError(MSG_XML_INVALID_ATTR_s, STR_XML_FIELD);
                return RESULT_ERR;
            }

        /* Check for duplicated message parameter ID */
            for ( i = 0; i < conf->signals[item].param_Count; i++ )
            {
                if ( !strcmp( conf->signals[item].param_FieldName[i], name ) )
                {
                    MSG_XMLError(MSG_XML_DUPLICATED_ID_s, name);
                    return RESULT_ERR;
                }
            }
            conf->signals[item].param_FieldName[i] = MEM_CopyStr( name );
        }
        else
        {
            size_t   k = conf->signals[item].param_Count;

        /* use predefined field name */
            name = MEM_Allocate( 16 );
            do
            {
            /* try to generated unique name */
                sprintf( name, "x%u", k );

            /* Check for duplicated message parameter ID */
                for ( i = 0; i < conf->signals[item].param_Count; i++ )
                {
                    if ( !strcmp( conf->signals[item].param_FieldName[i], name ) )
                    {
                        k++;
                        break;
                    }
                }

            }
            while ( i != conf->signals[item].param_Count );
            conf->signals[item].param_FieldName[conf->signals[item].param_Count] = name;
        }
        conf->signals[item].param_Count++;
    }
    return RESULT_OK;
}


/***************************************************************************
 * Function : tat_parseMacro
 *
 * Description: parse <CONFIG>::<MACRO> tag
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_parseMacro(const XML_ELEMENT element_id, const XML_TAG *xmldata, void * outdata)
{
    TAT_XML_CONFIG * data = (TAT_XML_CONFIG *) outdata;
    size_t  i, item;
    char * name;
    TAT_XML_CONFIG_DEF * conf;  /* current <CONFIG> tag */
    if (!outdata) return RESULT_ERR;
    conf = &data->config[data->config_Count-1];

    name = XML_GetAttrValue( STR_XML_NAME, xmldata );

    if ( name )
    {
        /* check macro name */
        if ( name[0] != '#' || !StrCheckSDLName( name + 1 ) )
        {
            MSG_XMLError(MSG_XML_INVALID_ATTR_s, STR_XML_NAME);
            return RESULT_ERR;
        }

        StrToUpper( name );

    /* check for duplicate definition */
        for ( i = 0; i < conf->macro_Count; i++ )
        {
            if ( conf->macro[i].name && !strcmp( conf->macro[i].name, name ) )
            {
                MSG_XMLError(MSG_XML_DUPLICATED_ID_s, name);
                return RESULT_ERR;
            }
        };
    }
    else
    {
    /* only ABORT macro is allowed w/o name !!! */

        if ( CFG_GetMacroType( XML_GetAttrValue( STR_XML_TYPE, xmldata ) ) !=
            MSC_MACROTYPE_ABORT )
        {
            MSG_XMLError(MSG_XML_REQUIRED_ATTR_s, STR_XML_NAME);
            return RESULT_ERR;
        }
    }
    
    /* allocate and clean memory */
    MEM_ADD_OBJECT(conf->macro, conf->macro_Count );
    i = conf->macro_Count;

    conf->macro[i].line_Number = xmldata->line_number;

    /* fill structures */
    conf->macro[i].name = MEM_CopyStr( name );

    conf->macro[i].annotation = MEM_CopyStr( XML_GetAttrValue( STR_ANNOTATION, xmldata ) );

    name = XML_GetAttrValue( STR_XML_TYPE, xmldata );

    conf->macro[i].type = CFG_GetMacroType( name );

    if ( conf->macro[i].type == MSC_MACROTYPE_UNDEFINED )
    {
        MSG_XMLError(MSG_XML_INVALID_ATTR_s, STR_XML_TYPE);
        return RESULT_ERR;
    }

    name = XML_GetAttrValue( STR_MACRO_VECTOR, xmldata );

    if ( name && ( name[0] != '#' || !StrCheckSDLName( name + 1 ) ) )
    {
        MSG_XMLError(MSG_XML_INVALID_ATTR_s, STR_MACRO_VECTOR);
        return RESULT_ERR;
    }

    conf->macro[i].vector_Name = MEM_CopyStr( name );

    name = XML_GetAttrValue( STR_MACRO_DATATYPE, xmldata );

    conf->macro[i].data_Type =
        ( name ) ? CFG_GetMacroDataType( name ) : MSC_MACRODATA_UNDEF;

    if ( conf->macro[i].data_Type == MSC_MACRODATA_UNDEFINED )
    {
        MSG_XMLError(MSG_XML_INVALID_ATTR_s, STR_XML_DATATYPE);
        return RESULT_ERR;
    }


    conf->macro[i].research = CFG_GetResearchType( XML_GetAttrValue( STR_MACRO_RESEARCH, xmldata ) );

    if ( conf->macro[i].research == MSC_RESEARCH_UNKNOWN )
    {
        MSG_XMLError(MSG_XML_INVALID_ATTR_s, STR_MACRO_RESEARCH);
        return RESULT_ERR;
    }

/* if specified 'priority', extract this number */
    if ( conf->macro[i].research == MSC_RESEARCH_PRIORITY )
    {
        name = XML_GetAttrValue( STR_MACRO_PRIOLIST, xmldata );
        if ( !name || !StrIsIntegerNumber( name ) ||
            ( conf->macro[i].type != MSC_MACROTYPE_LIST ) )
        {
            MSG_XMLError(MSG_XML_INVALID_ATTR_s, STR_MACRO_PRIOLIST);
            return RESULT_ERR;
        }
        conf->macro[i].prio_Number = ( unsigned int ) StrToInt( name );
        if ( conf->macro[i].prio_Number <= 0 )
        {
            MSG_XMLError(MSG_XML_INVALID_ATTR_s, STR_MACRO_PRIOLIST);
            return RESULT_ERR;
        }
    }

    name = XML_GetAttrValue( STR_MACRO_REPLACE, xmldata );

    conf->macro[i].replace_Type = ( name ) ? CFG_GetReplaceType( name ) : MSC_REPL_REPLACE;

    if ( conf->macro[i].replace_Type == MSC_REPL_UNDEFINED )
    {
        MSG_XMLError(MSG_XML_INVALID_ATTR_s, STR_MACRO_REPLACE);
        return RESULT_ERR;
    }

    if ( ( ( conf->macro[i].replace_Type == MSC_REPL_EXTEND ) ||
            ( conf->macro[i].replace_Type == MSC_REPL_SUBSET ) ) &&
        conf->macro[i].type != MSC_MACROTYPE_LIST )
    {
        MSG_XMLError(MSG_XML_INVALID_ATTR_s, STR_MACRO_REPLACE);
        return RESULT_ERR;
    }

    name = XML_GetAttrValue( STR_XML_PRIORITY, xmldata );

    if ( name )
    {
        if ( !StrIsIntegerNumber( name ) )
        {
            MSG_XMLError(MSG_XML_INVALID_ATTR_s, STR_XML_PRIORITY);
            return RESULT_ERR;
        }
        else
        {
            conf->macro[i].priority = atoi( name );

            if ( conf->macro[i].priority < MSC_MIN_PRIORITY )
                conf->macro[i].priority = MSC_MIN_PRIORITY;

            if ( conf->macro[i].priority > MSC_MAX_PRIORITY )
                conf->macro[i].priority = MSC_MAX_PRIORITY;
        }
    }
    else
        conf->macro[i].priority = MSC_DEFAULT_PRIORITY;

    conf->macro_Count++;
    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_parseValuelist
 *
 * Description: parse <MACRO>::<VALUELIST> tag
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_parseValuelist(const XML_ELEMENT element_id, const XML_TAG *xmldata, void * outdata)
{
    TAT_XML_CONFIG * data = (TAT_XML_CONFIG *) outdata;
    size_t  i;
    char * name;
    TAT_XML_CONFIG_DEF * conf;  /* current <CONFIG> tag */
	if (!outdata) return RESULT_ERR;
    conf = &data->config[data->config_Count-1];
    i = conf->macro_Count-1;

    if ( conf->macro[i].type != MSC_MACROTYPE_FUNCTION )
    {
        /* PROJECT -> CONFIG -> MACROS -> MACRO -> VALUELIST */
        name = XML_GetAttrValue( STR_XML_CONDITION, xmldata );
		MEM_ADD_OBJECT( conf->macro[i].data, conf->macro[i].data_Count );
        conf->macro[i].data[conf->macro[i].data_Count].condition = MEM_CopyStr( name );
        /* parse attributes and parameters */

        conf->macro[i].data_Count++;
    }
    else
    {
        MSG_XMLError( MSG_XML_MACRO_OBSOLETE_ss, STR_XML_VALUELIST, conf->macro[i].name);
        return RESULT_ERR;
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_parseValue
 *
 * Description: parse <MACRO>::<VALUE> tag
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_parseValue(const XML_ELEMENT element_id, const XML_TAG *xmldata, void * outdata)
{
    TAT_XML_CONFIG * data = (TAT_XML_CONFIG *) outdata;
    size_t  i, item,j;
    char * name;
    TAT_XML_CONFIG_DEF * conf;  /* current <CONFIG> tag */
	if (!outdata) return RESULT_ERR;
	conf = &data->config[data->config_Count-1];
    i = conf->macro_Count-1;
	j = conf->macro[i].data_Count-1;

    if ( conf->macro[i].type != MSC_MACROTYPE_ABORT && conf->macro[i].type != MSC_MACROTYPE_FUNCTION )
    {
        name = XML_GetAttrValue( STR_XML_VALUE, xmldata );
		
        if ( !name || name[0] == 0 )
        {
            MSG_XMLError(MSG_XML_REQUIRED_ATTR_s, STR_XML_VALUE);
            return RESULT_ERR;
        }

        if ( conf->macro[i].type != MSC_MACROTYPE_DERIVE && 
            !CFG_CheckMacroData( conf->macro[i].data_Type, name ) )
        {
            MSG_XMLError( MSG_XML_MACRO_DATA_ss, name, conf->macro[i].name );
            return RESULT_ERR;
        }
        MEM_ADD_OBJECT(conf->macro[i].data[j].data,conf->macro[i].data[j].data_Count);
        conf->macro[i].data[j].data[conf->macro[i].data[j].data_Count] = MEM_CopyStr( name );
        conf->macro[i].data[j].data_Count++;
    }
    else
    {
        MSG_XMLError( MSG_XML_MACRO_OBSOLETE_ss, STR_MACRO_VALUE, STR_MACRO_ABORT );
        return RESULT_ERR;
    }
	return RESULT_OK;
}

/***************************************************************************
 * Function : tat_parseMacroAttr
 *
 * Description: parse <MACRO>::<ATTRIBUTE> tag
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_parseMacroAttr(const XML_ELEMENT element_id, const XML_TAG *xmldata, void * outdata)
{
    TAT_XML_CONFIG * data = (TAT_XML_CONFIG *) outdata;
    size_t  i, item;
    char * name;
    TAT_XML_CONFIG_DEF * conf;  /* current <CONFIG> tag */
    if (!outdata) return RESULT_ERR;
    conf = &data->config[data->config_Count-1];
    i = conf->macro_Count-1;

    if ( conf->macro[i].type != MSC_MACROTYPE_LIST && conf->macro[i].type != MSC_MACROTYPE_ABORT &&
        conf->macro[i].type != MSC_MACROTYPE_INDEX )
    {
        name = XML_GetAttrValue( STR_XML_VALUE, xmldata );
        conf->macro[i].attr =
            MEM_AddObject( conf->macro[i].attr, ( conf->macro[i].attr_Count + 1 ),
            sizeof ( conf->macro[0].attr[0] ) );
        conf->macro[i].attr[conf->macro[i].attr_Count] = MEM_CopyStr( name );
        conf->macro[i].attr_Count++;
    }
    else
    {
        MSG_XMLError( MSG_XML_MACRO_OBSOLETE_ss, STR_XML_ATTRIBUTE, 
            ( conf->macro[i].type != MSC_MACROTYPE_ABORT ) ? conf->macro[i].name : STR_MACRO_ABORT );
        return RESULT_ERR;
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_parseInitial
 *
 * Description: parse <CONFIG>::<INITIAL> tag
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_parseInitial(const XML_ELEMENT element_id, const XML_TAG *xmldata, void * outdata)
{
    TAT_XML_CONFIG * data = (TAT_XML_CONFIG *) outdata;
    size_t  i, item;
    char * name;
    TAT_XML_CONFIG_DEF * conf;  /* current <CONFIG> tag */
    if (!outdata) return RESULT_ERR;
    conf = &data->config[data->config_Count-1];

    name = XML_GetAttrValue( STR_INITIAL, xmldata );
    
    if ( !name )
    {
        MSG_XMLError(MSG_XML_REQUIRED_ATTR_s, STR_XML_INITIAL);
        return RESULT_ERR;
    }

    /* check for duplicate definition */
    if (conf->initial_Axiom)
    {
        MSG_XMLError( MSG_XML_DUPLICATED_ID_s, name );
        return RESULT_ERR;
    }

    conf->initial_Axiom = MEM_CopyStr(name);

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_parseAxiom
 *
 * Description: parse <CONFIG>::<AXIOM> tag
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_parseAxiom(const XML_ELEMENT element_id, const XML_TAG *xmldata, void * outdata)
{
    TAT_XML_CONFIG * data = (TAT_XML_CONFIG *) outdata;
    size_t  i, item;
    char * name;
    TAT_XML_CONFIG_DEF * conf;  /* current <CONFIG> tag */
    if (!outdata) return RESULT_ERR;
    conf = &data->config[data->config_Count-1];

    item = conf->axiom_Count;
    name = XML_GetAttrValue( STR_XML_NAME, xmldata );

    if ( !name )
    {
        MSG_XMLError(MSG_XML_REQUIRED_ATTR_s, STR_XML_NAME);
        return RESULT_ERR;
    }
        
    if (!StrCheckIDName( name ) )
    {
        MSG_XMLError(MSG_XML_INVALID_ATTR_s, STR_XML_NAME);
        return RESULT_ERR;
    }


    for ( i = 0; i < item; i++ )
    {
        if ( !strcmp( conf->axioms[i].name, name ) )
        {
            MSG_XMLError( MSG_XML_DUPLICATED_ID_s, name );
            return RESULT_ERR;
        }
    };

    MEM_ADD_OBJECT( conf->axioms, item );

    conf->axioms[i].name = MEM_CopyStr( name );

    name = XML_GetAttrValue( STR_AXIOM, xmldata );
    if ( !name )
    {
        MSG_XMLError(MSG_XML_REQUIRED_ATTR_s, STR_AXIOM);
        return RESULT_ERR;
    }

    conf->axioms[i].axiom = MEM_CopyStr( name );

    name = XML_GetAttrValue( STR_CHECK, xmldata );

    if ( !name || !StrIsNaturalNumber(name))
    {
        MSG_XMLError(MSG_XML_INVALID_ATTR_s, STR_CHECK);
        return RESULT_ERR;
    }

    conf->axioms[i].check = atoi( name );
    conf->axiom_Count++;

    return RESULT_OK;
}


/***************************************************************************
 * Function : tat_parseTypedef
 *
 * Description: parse <CONFIG>::<TYPEDEF> tag
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_parseTypedef(const XML_ELEMENT element_id, const XML_TAG *xmldata, void * outdata)
{
    TAT_XML_CONFIG * data = (TAT_XML_CONFIG *) outdata;
    size_t  i;
    char * name;
    TAT_XML_CONFIG_DEF * conf;  /* current <CONFIG> tag */
    if (!outdata) return RESULT_ERR;
    conf = &data->config[data->config_Count-1];

    name = XML_GetAttrValue( STR_XML_NAME, xmldata );

    if ( !name )
    {
        MSG_XMLError(MSG_XML_REQUIRED_ATTR_s, STR_XML_NAME);
        return RESULT_ERR;
    }
        
    if ( !StrCheckSDLName( name ) )
    {
        MSG_XMLError(MSG_XML_INVALID_ATTR_s, STR_XML_NAME);
        return RESULT_ERR;
    }

    /*StrToUpper( name );*/

    /* check for duplicate definition */
    for ( i = 0; i < conf->data_Count; i++ )
    {
        if ( !strcmp( conf->data[i].name, name ) )
        {
            MSG_XMLError( MSG_XML_DUPLICATED_ID_s, name );
            return RESULT_ERR;
        }
    };

    /* allocate and clean memory */
    MEM_ADD_OBJECT( conf->data, conf->data_Count);

    conf->data[conf->data_Count].line_Number = xmldata->line_number;

    /* fill structures */
    conf->data[conf->data_Count].name = MEM_CopyStr( name );

    name = XML_GetAttrValue( STR_XML_TYPE, xmldata );

    if ( !name )
    {
        MSG_XMLError(MSG_XML_REQUIRED_ATTR_s, STR_XML_TYPE);
        return RESULT_ERR;
    }


    conf->data[conf->data_Count].target_Name = MEM_CopyStr( name );

    conf->data[i].attr_Type =
        MEM_CopyStr( XML_GetAttrValue( STR_XML_PARAMSTYPE, xmldata ) );

    conf->data[i].prefix = MEM_CopyStr( XML_GetAttrValue( STR_XML_PREFIX, xmldata ) );

	/* extract data class */
    name = XML_GetAttrValue( STR_DATA_DATACLASS, xmldata );
    conf->data[i].data_Class = MSC_DATA_UNKNOWN;

    if ( name )
    {
        conf->data[i].data_Class = CFG_GetDataClassType( name );
        if ( conf->data[i].data_Class == MSC_DATA_UNKNOWN )
        {
            MSG_XMLError(MSG_XML_INVALID_ATTR_s, STR_DATA_DATACLASS);
            return RESULT_ERR;
        }
    }

	/* extract dimension */
	name=XML_GetAttrValue(STR_XML_DIMENSION,xmldata);
	conf->data[i].dimension=MEM_CopyStr(name);
	
    conf->data_Count++;

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_parseTypedefAttr
 *
 * Description: parse <TYPEDEF>::<ATTRIBUTE> tag
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_parseTypedefAttr(const XML_ELEMENT element_id, const XML_TAG *xmldata, void * outdata)
{
    TAT_XML_CONFIG * data = (TAT_XML_CONFIG *) outdata;
    size_t  i;
    char * name;
    TAT_XML_CONFIG_DEF * conf;  /* current <CONFIG> tag */
    if (!outdata) return RESULT_ERR;
    conf = &data->config[data->config_Count-1];
    i = conf->data_Count - 1 ;

    name = XML_GetAttrValue( STR_XML_VALUE, xmldata );
    
    /* allocate and clean memory */
    MEM_ADD_OBJECT( conf->data[i].attr, conf->data[i].attr_Count);

    conf->data[i].attr[conf->data[i].attr_Count] = MEM_CopyStr( name );
    conf->data[i].attr_Count++;

	return RESULT_OK;
}

/***************************************************************************
 * Function : tat_parseTypedefMember
 *
 * Description: parse <TYPEDEF>::<MEMBER> tag
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_parseTypedefMember(const XML_ELEMENT element_id, const XML_TAG *xmldata, void * outdata)
{
    TAT_XML_CONFIG * data = (TAT_XML_CONFIG *) outdata;
    size_t  i;
    char * name;
	char * type;
	char * dimstr;
    TAT_XML_CONFIG_DEF * conf;  /* current <CONFIG> tag */
    if (!outdata) return RESULT_ERR;
    conf = &data->config[data->config_Count-1];
    i = conf->data_Count - 1 ;

    name = XML_GetAttrValue( STR_XML_NAME, xmldata );
	type = XML_GetAttrValue( STR_XML_TYPE, xmldata );

	if(!name || !type) return RESULT_ERR;
   
    dimstr=XML_GetAttrValue( STR_XML_DIMENSION, xmldata );

	/* allocate and clean memory */
    MEM_ADD_OBJECT(conf->data[i].member_List,conf->data[i].member_Count);

	conf->data[i].member_List[conf->data[i].member_Count].name = MEM_CopyStr(name);
	conf->data[i].member_List[conf->data[i].member_Count].type = MEM_CopyStr(type);
	conf->data[i].member_List[conf->data[i].member_Count].dimension = MEM_CopyStr(dimstr);

	conf->data[i].member_Count++;

	return RESULT_OK;
}

/***************************************************************************
 * Function : tat_parseTypedefValue
 *
 * Description: parse <TYPEDEF>::<VALUE> tag
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_parseTypedefValue(const XML_ELEMENT element_id, const XML_TAG *xmldata, void * outdata)
{
    TAT_XML_CONFIG * data = (TAT_XML_CONFIG *) outdata;
    size_t  i, item;
    char * name;
    TAT_XML_CONFIG_DEF * conf;  /* current <CONFIG> tag */
    if (!outdata) return RESULT_ERR;
    conf = &data->config[data->config_Count-1];
    
    i = conf->data_Count - 1 ;

    name = XML_GetAttrValue( STR_XML_VALUE, xmldata );

    if ( !name )
    {
        MSG_XMLError(MSG_XML_REQUIRED_ATTR_s, STR_XML_VALUE);
        return RESULT_ERR;
    }

/* allocate and clean memory */
    MEM_ADD_OBJECT (conf->data[i].value_List, conf->data[i].value_Count);

    MEM_ADD_OBJECT (conf->data[i].enums_List, conf->data[i].value_Count);

    conf->data[i].value_List[conf->data[i].value_Count] = MEM_CopyStr( name );

    name = XML_GetAttrValue( STR_DATA_ENUM, xmldata );

    if ( !name )
    {
        MSG_XMLError(MSG_XML_REQUIRED_ATTR_s, STR_DATA_ENUM);
        return RESULT_ERR;
    }

    conf->data[i].enums_List[conf->data[i].value_Count] = MEM_CopyStr( name );
    conf->data[i].value_Count++;
    
    return RESULT_OK;
}

/* this table specifies tag nesting in form of FSM
   <current state> <next state> <iterator> <function> */
static XML_DTD tat_XMLdtd[]={
    {XML_INIT_STATE, TATXML_PROJECT, XML_ONE, tat_parseProject},
    {TATXML_PROJECT, TATXML_CONFIG, XML_ONEORMANY, tat_parseConfig},

    {TATXML_CONFIG, TATXML_INSTANCES, XML_ANY, tat_parseEmpty},
    {TATXML_CONFIG, TATXML_INSTANCE, XML_ANY, tat_parseInstance},
    {TATXML_INSTANCES, TATXML_INSTANCE, XML_ONEORMANY, tat_parseInstance},
    {TATXML_INSTANCE, TATXML_ATTRIBUTE, XML_ANY, tat_parseInstanceAttr},
    {TATXML_INSTANCE, TATXML_PARAMETER, XML_ANY, tat_parseInstanceParm},

    {TATXML_CONFIG, TATXML_SIGNALS, XML_ANY, tat_parseEmpty},
    {TATXML_CONFIG, TATXML_SIGNAL, XML_ONEORMANY, tat_parseSignal},
    {TATXML_SIGNALS, TATXML_SIGNAL, XML_ONEORMANY, tat_parseSignal},
    {TATXML_SIGNAL, TATXML_ATTRIBUTE, XML_ANY, tat_parseSignalAttr},
    {TATXML_SIGNAL, TATXML_PARAMETER, XML_ANY, tat_parseSignalParm},

    {TATXML_CONFIG, TATXML_DATATYPE, XML_ANY, tat_parseEmpty},
    {TATXML_CONFIG, TATXML_TYPEDEF, XML_ANY, tat_parseTypedef},
    {TATXML_DATATYPE, TATXML_TYPEDEF, XML_ONEORMANY, tat_parseTypedef},
    {TATXML_TYPEDEF, TATXML_ATTRIBUTE, XML_ANY, tat_parseTypedefAttr},
    {TATXML_TYPEDEF, TATXML_VALUE, XML_ANY, tat_parseTypedefValue},
	{TATXML_TYPEDEF, TATXML_MEMBER, XML_ANY, tat_parseTypedefMember},

    {TATXML_CONFIG, TATXML_MACROS, XML_ONEORMANY, tat_parseEmpty},
    {TATXML_CONFIG, TATXML_MACRO, XML_ANY, tat_parseMacro},
    {TATXML_MACROS, TATXML_MACRO, XML_ONEORMANY, tat_parseMacro},
    {TATXML_MACRO, TATXML_VALUELIST, XML_ONEORMANY, tat_parseValuelist},
    {TATXML_MACRO, TATXML_ATTRIBUTE, XML_ANY, tat_parseMacroAttr},
    {TATXML_VALUELIST, TATXML_VALUE, XML_ONEORMANY, tat_parseValue},


    {TATXML_CONFIG, TATXML_AXIOMS, XML_ONEORMANY, tat_parseEmpty},
    {TATXML_CONFIG, TATXML_AXIOM, XML_ONEORMANY, tat_parseAxiom},
    {TATXML_AXIOMS, TATXML_AXIOM, XML_ONEORMANY, tat_parseAxiom},

    {TATXML_CONFIG, TATXML_INITIAL, XML_ONEORMANY, tat_parseInitial},

	/* The following lines allow XML parser to ignore GUI specific markup */
	{TATXML_PROJECT, TATXML_SPECFILES, XML_ANY, tat_parseEmpty},
	{TATXML_SPECFILES, TATXML_SET, XML_ANY, tat_parseEmpty},
	{TATXML_SET, TATXML_FILE, XML_ANY, tat_parseEmpty},
	
	{TATXML_PROJECT, TATXML_TOOLS, XML_ANY, tat_parseEmpty},
	{TATXML_TOOLS, TATXML_TOOL, XML_ANY, tat_parseEmpty},
	{TATXML_TOOL, TATXML_INPUT, XML_ANY, tat_parseEmpty},
	{TATXML_TOOL, TATXML_OUTPUT, XML_ANY, tat_parseEmpty},
	{TATXML_TOOL, TATXML_TOOLCONFIG, XML_ANY, tat_parseEmpty},
	{TATXML_TOOLCONFIG, TATXML_ATTRIBUTE, XML_ANY, tat_parseEmpty},
	{TATXML_TOOL, TATXML_EXEC, XML_ANY, tat_parseEmpty},
	{TATXML_EXEC, TATXML_ATTACHIN, XML_ANY, tat_parseEmpty},
	{TATXML_EXEC, TATXML_ATTACHOUT, XML_ANY, tat_parseEmpty},
	
	{TATXML_PROJECT, TATXML_TESTSUITE, XML_ANY, tat_parseEmpty},
	{TATXML_TESTSUITE, TATXML_TESTCASE, XML_ANY, tat_parseEmpty},
	{TATXML_TESTCASE, TATXML_PIPE, XML_ANY, tat_parseEmpty},
	{TATXML_PIPE, TATXML_DATAIN, XML_ANY, tat_parseEmpty},
	{TATXML_PIPE, TATXML_DATAOUT, XML_ANY, tat_parseEmpty}
};

#define tat_dtdCount sizeof(tat_XMLdtd)/sizeof(tat_XMLdtd[0])

/***************************************************************************
 * Function : TAT_FreeConfig
 *
 * Description: free memory allocated to XML configuration storage
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL TAT_FreeConfig( TAT_XML_CONFIG * data )
{
    size_t i, j, k;

    if ( !data )
        return RESULT_OK;

    for ( i = 0; i < data->config_Count; i++ )
    {
        for ( j = 0; j < data->config[i].instance_Count; j++ )
        {
            if ( data->config[i].instances[j].name )
                MEM_Free( data->config[i].instances[j].name );

            if ( data->config[i].instances[j].attr_Type )
                MEM_Free( data->config[i].instances[j].attr_Type );

            MEM_FreeStrArray( data->config[i].instances[j].param_Count,
                data->config[i].instances[j].param_TgtType );
            MEM_FreeStrArray( data->config[i].instances[j].param_Count,
                data->config[i].instances[j].source_Format );
            MEM_FreeStrArray( data->config[i].instances[j].param_Count,
                data->config[i].instances[j].param_FieldName );
            MEM_FreeStrArray( data->config[i].instances[j].attr_Count,
                data->config[i].instances[j].attr );
        }

        for ( j = 0; j < data->config[i].signal_Count; j++ )
        {
            if ( data->config[i].signals[j].name )
                MEM_Free( data->config[i].signals[j].name );

            if ( data->config[i].signals[j].attr_Type )
                MEM_Free( data->config[i].signals[j].attr_Type );

            MEM_FreeStrArray( data->config[i].signals[j].param_Count,
                data->config[i].signals[j].param_TgtType );
            MEM_FreeStrArray( data->config[i].signals[j].param_Count,
                data->config[i].signals[j].source_Format );
            MEM_FreeStrArray( data->config[i].signals[j].param_Count,
                data->config[i].signals[j].param_FieldName );
            MEM_FreeStrArray( data->config[i].signals[j].attr_Count, data->config[i].signals[j].attr );
        }

        for ( j = 0; j < data->config[i].data_Count; j++ )
        {
            for(k = 0; k < data->config[i].data[j].member_Count; k++)
			{
				if (data->config[i].data[j].member_List[k].name)
					MEM_Free(data->config[i].data[j].member_List[k].name);

				if (data->config[i].data[j].member_List[k].type)
					MEM_Free(data->config[i].data[j].member_List[k].type);

				if (data->config[i].data[j].member_List[k].dimension)
					MEM_Free(data->config[i].data[j].member_List[k].dimension);
			}

			if ( data->config[i].data[j].member_List )
				MEM_Free(data->config[i].data[j].member_List);
			
			if ( data->config[i].data[j].name )
                MEM_Free( data->config[i].data[j].name );

			if ( data->config[i].data[j].dimension )
                MEM_Free( data->config[i].data[j].dimension );

            if ( data->config[i].data[j].attr_Type )
                MEM_Free( data->config[i].data[j].attr_Type );

            if ( data->config[i].data[j].target_Name )
                MEM_Free( data->config[i].data[j].target_Name );

            if ( data->config[i].data[j].prefix )
                MEM_Free( data->config[i].data[j].prefix );

            MEM_FreeStrArray( data->config[i].data[j].attr_Count, data->config[i].data[j].attr );
            MEM_FreeStrArray( data->config[i].data[j].value_Count, data->config[i].data[j].value_List );
        }

        for ( j = 0; j < data->config[i].macro_Count; j++ )
        {
            if ( data->config[i].macro[j].name )
                MEM_Free( data->config[i].macro[j].name );

            if ( data->config[i].macro[j].annotation )
                MEM_Free( data->config[i].macro[j].annotation );

            if ( data->config[i].macro[j].vector_Name )
                MEM_Free( data->config[i].macro[j].vector_Name );
            MEM_FreeStrArray( data->config[i].macro[j].attr_Count, data->config[i].macro[j].attr );

            if ( data->config[i].macro[j].data_Count )
            {
                for ( k = 0; k < data->config[i].macro[j].data_Count; k++ )
                {
                    if ( data->config[i].macro[j].data[k].condition )
                        MEM_Free( data->config[i].macro[j].data[k].condition );
                    MEM_FreeStrArray( data->config[i].macro[j].data[k].data_Count,
                        data->config[i].macro[j].data[k].data );
                }

                MEM_Free( data->config[i].macro[j].data );
            }
        }

        for ( j = 0; j < data->config[i].axiom_Count; j++ )
        {
            if ( data->config[i].axioms[j].name )
                MEM_Free( data->config[i].axioms[j].name );

            if ( data->config[i].axioms[j].axiom )
                MEM_Free( data->config[i].axioms[j].axiom );
        }

        if ( data->config[i].signals )
            MEM_Free( data->config[i].signals );

        data->config[i].signals = NULL;

        if ( data->config[i].instances )
            MEM_Free( data->config[i].instances );

        data->config[i].instances = NULL;

        if ( data->config[i].data )
            MEM_Free( data->config[i].data );

        data->config[i].data = NULL;

        if ( data->config[i].macro )
            MEM_Free( data->config[i].macro );

        data->config[i].macro = NULL;

        if ( data->config[i].axioms )
            MEM_Free( data->config[i].axioms );

        data->config[i].axioms = NULL;

    }

    if ( data->config )
        MEM_Free( data->config );

    data->config = NULL;
    data->config_Count = 0;
    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_CheckConfig
 *
 * Description: check configuration for inconsistent or recursive definition
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_CheckConfig( const char * fname, TAT_XML_CONFIG * data )
{
    UINT8_T                *trans;
    size_t                  i, j, l, cc;

    if ( !data )
        return RESULT_OK;

    if ( !data->config_Count )
        return RESULT_ERR;       /* missing configuration */

    cc = data->config_Count;

    trans = MEM_Allocate( cc * cc * sizeof ( trans[0] ) );

    for ( i = 0; i < cc; i++ )
    {
        if ( data->config[i].base )
        {
            for ( j = 0; j < cc; j++ )
            {
                if ( !stricmp( data->config[j].name, data->config[i].base ) )
                    break;
            }

            if ( j == cc )
            {
                MSG_XMLErrorA( fname, data->config[i].line_Number,
                    MSG_XML_INVALID_BASE_CONF_s, data->config[i].base );
                MEM_Free( trans );
                return RESULT_ERR;   /* invalid base configuration */
            };

            trans[i * cc + j] = 1;  /* mark conf. i is depend on conf. j */
        }
    };

/* perform transitive closing */
    for ( i = 0; i < cc; i++ )
        for ( j = 0; j < cc; j++ )
            if ( trans[j * cc + i] )
                for ( l = 0; l < cc; l++ )
                    trans[j * cc + l] |= trans[i * cc + l];

    for ( i = 0; i < cc; i++ )
    {
        for ( j = 0; j < cc; j++ )
        {
            if ( trans[i * cc + j] && trans[j * cc + i] )
            {
                MSG_XMLErrorA( fname, data->config[j].line_Number,  MSG_XML_RECURSIVE_CONF );
                MEM_Free( trans );
                return RESULT_ERR;   /* recursive configuration */
            }
        }
    };

    MEM_Free( trans );

    for ( i = 0; i < cc; i++ )
    {
        for ( j = 0; j < data->config[i].macro_Count; j++ )
        {
            if ( data->config[i].macro[j].type != MSC_MACROTYPE_LIST && data->config[i].macro[j].vector_Name )
            {
                MEM_Free( data->config[i].macro[j].vector_Name );
                data->config[i].macro[j].vector_Name = NULL;
                MSG_XMLErrorA( fname, data->config[i].macro[j].line_Number,
                               MSG_XML_MACRO_OBSOLETE_ss, STR_MACRO_VECTOR,
                    ( data->config[i].macro[j].name ) ? ( data->config[i].macro[j].name ) : STR_MACRO_ABORT );

                return RESULT_ERR;
            }

            switch ( data->config[i].macro[j].type )
            {

            case MSC_MACROTYPE_LIST:
            case MSC_MACROTYPE_INDEX:
            {
            /* list shall be specified */
                if ( !data->config[i].macro[j].data_Count )
                    goto MacroError;
            /* at least 1 value shall be specified in each data set */
                for ( l = 0; l < data->config[i].macro[j].data_Count; l++ )
                {
                    if ( !data->config[i].macro[j].data[l].data_Count )
                    {
                        goto MacroError;
                    }
                }

                break;
            }

            case MSC_MACROTYPE_FUNCTION:
            {
                if ( ( data->config[i].macro[j].attr_Count != 2 ) ||
                    ( !StrCheckCName( data->config[i].macro[j].attr[0] ) ) ||
                    ( !StrIsNaturalNumber( data->config[i].macro[j].attr[1] ) ) )
                    goto MacroError;

                break;
            }

            case MSC_MACROTYPE_DERIVE:
            {
            /* list of parameters and attribute shall be specified */

                if ( ( data->config[i].macro[j].data_Count != 1 ) ||
                    ( data->config[i].macro[j].attr_Count != 1 ) ||
                    ( !StrCheckCName( data->config[i].macro[j].attr[0] ) ) )
                    goto MacroError;

                break;
            }

            case MSC_MACROTYPE_ABORT:
            {
                if ( !data->config[i].macro[j].data_Count )
                    goto MacroError;    /* list shall be specified */

                for ( l = 0; l < data->config[i].macro[j].data_Count; l++ )
                {
                /* conditions shall be specified. Otherwise, no macro sets available */

                    if ( !data->config[i].macro[j].data[l].condition )
                        goto MacroError;
                }
            }
            }
        }
    }

    return RESULT_OK;

  MacroError:
    MSG_XMLErrorA( fname, data->config[i].macro[j].line_Number,
                   MSG_XML_MACRO_FIELDS_s, data->config[i].macro[j].name );
    return RESULT_ERR;
}

/***************************************************************************
 * Function : TAT_ReadXML
 *
 * Description: load TAT configuration from XML file
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL TAT_ReadXML( const char * name, TAT_XML_CONFIG * data )
{
    if (!XML_Load(name, tat_tagCount, &tat_XMLtags[0],  tat_dtdCount, &tat_XMLdtd[0], data))
        return RESULT_ERR;
    data->fname = MEM_CopyStr(name);

    return tat_CheckConfig( name, data );
}
