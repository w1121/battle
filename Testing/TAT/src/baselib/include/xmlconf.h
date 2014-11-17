#ifndef TAT_XMLCONF_H
#define TAT_XMLCONF_H
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
#include <cfgtypes.h>

#ifdef __cplusplus
extern                  "C"
{
#endif                                 /* C++ */

/* structure for signal definition */

    typedef struct
    {
        char                   *name;  /* signal name */

        MSC_SIGNAL_TYPE         type;  /* type: in, out, msg, ignore */
        int                     priority;

        size_t                  attr_Count; /* attributes number */
        char                   *attr_Type;  /* identificator of attributes */
        char                  **attr;  /* attributes */

        size_t                  param_Count;    /* number of parameters */
        char                  **source_Format;  /* source parameters type */
        char                  **param_TgtType;  /* target parameters type */
        char                  **param_FieldName;    /* field's names */
        size_t                  param_Extra;    /* set to 1 if allow additional params */
        size_t                  line_Number;    /* line number in XML file */

    }
    TAT_SIGNAL_DEF;

/* struct for instance */
    typedef struct
    {
        char                   *name;  /* instance name */
        MSC_INSTANCE_TYPE       type;  /* kind of instance */
        int                     priority;
        size_t                  attr_Count; /* number of additional attributes */
        char                  **attr;  /* additional attributes */
        char                   *attr_Type;  /* identificator of attributes */

        size_t                  param_Count;    /* number of parameters */
        size_t                  param_Extra;    /* set to 1 if allow additional params */
        char                  **source_Format;  /* source parameters type */
        char                  **param_TgtType;  /* target parameters type */
        char                  **param_FieldName;    /* field's names */
        size_t                  line_Number;    /* line number in XML file */
    }
    TAT_INSTANCE_DEF;

/* structure for struct member definition */
	typedef struct
	{
		char	*name;			/* name */
		char	*type;			/* type */
		char	*dimension;		/* arrays only */
	}
	TAT_MEMBER_DEF;


/* structure for data type definition */
    typedef struct
    {
        char                   *name;  /* original name */
        char                   *target_Name;    /* target name */
        size_t                  attr_Count; /* number of additional attributes */
        char                  **attr;  /* additional attributes */
        char                   *attr_Type;  /* identificator of attributes */

        char                   *prefix; /* what to add before message parameter in ATS */
        size_t                  value_Count;    /* number of available values */
        char                  **value_List; /* available values */
        char                  **enums_List;
        MSC_DATA_CLASS          data_Class; /* value type for data */
        size_t                  line_Number;    /* line number in XML file */

		char					*dimension;		/* arrays only */
		size_t					member_Count;	/* member count */
		TAT_MEMBER_DEF		   *member_List;	/* members */

    }
    TAT_DATATYPE_DEF;

    typedef struct
    {
        char                   *condition;      /* condition of existence. NULL means always */
        size_t                  data_Count;     /* number of values */
        char                  **data;           /* data values */
        size_t                  line_Number;    /* line number in XML file */
    }
    TAT_MACRODATA_DEF;

    typedef struct
    {
        char                   *name;
        MSC_MACRO_TYPE          type;  /* macro type */
        MSC_MACRO_DATA          data_Type;  /* macro data type */
        MSC_MACRO_REPL          replace_Type;   /* how to replace this macro with previous defs. */
        char                   *vector_Name;    /* dependency vector */
        char                   *annotation; /* annotation */
        int                     priority;   /* macro research priority */
        MSC_RESEARCH_TYPE       research;   /* how to research macro */
        size_t                  prio_Number;    /* number of priority numbers */
        size_t                  attr_Count; /* number of additional attributes */
        char                  **attr;  /* additional attributes */
        size_t                  data_Count; /* number of data collections */
        TAT_MACRODATA_DEF      *data;  /* data definition */
        size_t                  line_Number;    /* line number in XML file */
    }
    TAT_MACRO_DEF;

    typedef struct 
    {
        char    *name;      /* axiom name (attribute 'name' in XML) */
        char    *axiom;     /* axiom (attribute 'axiom' in XML) */
        int      check;      /* 0/1  (attribute 'check' in XML) */
        size_t                  line_Number;    /* line number in XML file */
    } TAT_AXIOM_DEF;

    typedef struct
    {
        int                     MatchTargetDataTypes;
    }
    TAT_GLOBAL_CONFIG;

    typedef struct _TAT_XML_CONFIG_DEF
    {
        char                   *name;           /* configuration name. Case Insensetive */
        char                   *base;           /* basic configuration name. Case Insensetive */
        size_t                  instance_Count; /* number of defined instances */
        size_t                  signal_Count;   /* number of defined signals */
        size_t                  data_Count;     /* number of defined data types */
        size_t                  macro_Count;    /* number of defined macros */
        size_t                  axiom_Count;    /* number of VRS axioms */
        TAT_INSTANCE_DEF       *instances;      /* instance definition storage */
        TAT_SIGNAL_DEF         *signals;        /* signal definition storage */
        TAT_DATATYPE_DEF       *data;           /* data definition storage */
        TAT_MACRO_DEF          *macro;          /* macro definition storage */
        TAT_AXIOM_DEF          *axioms;         /* axioms definitions */
        char                   *initial_Axiom;  /* initial axiom */
        TAT_GLOBAL_CONFIG       global;         /* global configuration info */
        size_t                  line_Number;    /* line number in XML file */
    }
    TAT_XML_CONFIG_DEF;         /* TAT configuration description */

    typedef struct
    {
        size_t                  config_Count;   /* number of defined configurations */
        TAT_XML_CONFIG_DEF     *config;         /* configuration storage */
        char *                  name;           /* name from PROJECT tag */
        char *                  fname;          /* file name */
    }
    TAT_XML_CONFIG;                    /* TAT configuration collection */

    extern LOGICAL          TAT_ReadXML( const char * name, TAT_XML_CONFIG * data );

    extern LOGICAL          TAT_FreeConfig( TAT_XML_CONFIG * data );

    extern TAT_XML_CONFIG_DEF *TAT_BuildConfig( const char * name, TAT_XML_CONFIG * data );

#ifdef __cplusplus
}

#endif                                 /* C++ */

#endif                                 /* TAT_XMLCONF_H */
