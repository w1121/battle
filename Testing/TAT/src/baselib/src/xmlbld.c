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
#include <xmlmess.h>

static int xml_CompSignals( const void *x1,
    const void *x2 )
{
    return strcmp( ( ( TAT_SIGNAL_DEF * ) x1 )->name, ( ( TAT_SIGNAL_DEF * ) x2 )->name );
}

static int xml_CompInstances( const void *x1,
    const void *x2 )
{
    return strcmp( ( ( TAT_INSTANCE_DEF * ) x1 )->name, ( ( TAT_INSTANCE_DEF * ) x2 )->name );
}

static int xml_CompAxioms( const void *x1,
                             const void *x2 )
{
    return strcmp( ( ( TAT_AXIOM_DEF * ) x1 )->name, ( ( TAT_AXIOM_DEF * ) x2 )->name );
}

static int xml_CompData( const void *x1,
    const void *x2 )
{
    return strcmp( ( ( TAT_DATATYPE_DEF * ) x1 )->name, ( ( TAT_DATATYPE_DEF * ) x2 )->name );
}

/***************************************************************************
 * Function : xml_CompMacro
 *
 * Description: compare macro definitions using priority and name
 *
 * Returns: <0/0/>0
 *
 * Notes: None
 *
 **************************************************************************/
static int xml_CompMacro( const void *x1,
    const void *x2 )
{
    int                     i1, i2;

    if ( ( ( TAT_MACRO_DEF * ) x1 )->priority != ( ( TAT_MACRO_DEF * ) x2 )->priority )
        return ( ( TAT_MACRO_DEF * ) x1 )->priority - ( ( TAT_MACRO_DEF * ) x2 )->priority;

    i1 = ( ( TAT_MACRO_DEF * ) x1 )->name != NULL;
    i2 = ( ( TAT_MACRO_DEF * ) x2 )->name != NULL;

    if ( i1 != i2 )
        return i1 - i2;

    if ( i1 && i2 )
        return strcmp( ( ( TAT_MACRO_DEF * ) x1 )->name, ( ( TAT_MACRO_DEF * ) x2 )->name );

    return 0;

}

/***************************************************************************
 * Function : tat_BuildConfigInstances
 *
 * Description: build configuration from instances
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_BuildConfigInstances( TAT_XML_CONFIG_DEF * c1,
    TAT_XML_CONFIG_DEF * c2 )
{
    size_t                    i, j, k;

/* copy instance definitions from base configuration to target */

    for ( i = 0; i < c2->instance_Count; i++ )
    {
        for ( j = 0; j < c1->instance_Count; j++ )
        {                       /* look for previous definition */

            if ( !strcmp( c2->instances[i].name, c1->instances[j].name ) )
                break;
        }

        if ( j == c1->instance_Count )
        {                       /* create new element */
            c1->instances =
                MEM_AddObject( c1->instances, ( c1->instance_Count + 1 ), sizeof ( c1->instances[0] ) );
            memset( &c1->instances[j], 0, sizeof ( c1->instances[0] ) );
            c1->instances[j].name = MEM_CopyStr( c2->instances[i].name );
            c1->instance_Count++;
        }

        if ( !c1->instances[j].type )
            c1->instances[j].type = c2->instances[i].type;

        if ( !c1->instances[j].priority )
            c1->instances[j].priority = c2->instances[i].priority;

        if ( !c1->instances[j].attr_Type )
            c1->instances[j].attr_Type = MEM_CopyStr( c2->instances[i].attr_Type );

    /* expand number of attributes */
        if ( c1->instances[j].attr_Count < c2->instances[i].attr_Count )
        {
            c1->instances[j].attr =
                MEM_AddObject( c1->instances[j].attr, c2->instances[i].attr_Count,
                sizeof ( c1->instances[0].attr[0] ) );
            memset( &c1->instances[j].attr[c1->instances[j].attr_Count], 0,
                ( c2->instances[i].attr_Count -
                    c1->instances[j].attr_Count ) * sizeof ( c1->instances[0].attr[0] ) );
            c1->instances[j].attr_Count = c2->instances[i].attr_Count;
        }

    /* copy attributes. after prev. step c2.attr_Count<=c1.attr_Count */
        for ( k = 0; k < c2->instances[i].attr_Count; k++ )
        {
            if ( !c1->instances[j].attr[k] )
                c1->instances[j].attr[k] = MEM_CopyStr( c2->instances[i].attr[k] );
        }

    /* if no parameters are specified, copy it from previous definition */
        if ( !c1->instances[j].param_Count && !c1->instances[j].param_Extra )
        {
        /* copy parameters attributes */
            c1->instances[j].param_Count = c2->instances[i].param_Count;
            c1->instances[j].param_Extra = c2->instances[i].param_Extra;

            if ( c1->instances[j].param_Count )
            {
                c1->instances[j].source_Format =
                    MEM_CopyStrArray( c1->instances[j].param_Count, c2->instances[i].source_Format );
                c1->instances[j].param_TgtType =
                    MEM_CopyStrArray( c1->instances[j].param_Count, c2->instances[i].param_TgtType );
                c1->instances[j].param_FieldName =
                    MEM_CopyStrArray( c1->instances[j].param_Count, c2->instances[i].param_FieldName );
            }
        }

    }                           /* finish instance processing */

    return RESULT_OK;
}

/***************************************************************************
* Function : tat_BuildConfigAxioms
*
* Description: build configuration from instances
*
* Returns: RESULT_ERR/RESULT_OK
*
* Notes: None
*
**************************************************************************/
static LOGICAL tat_BuildConfigAxioms( TAT_XML_CONFIG_DEF * c1,
                                      TAT_XML_CONFIG_DEF * c2 )
{
    size_t                  i, j;

    /* copy instance definitions from base configuration to target */

    for ( i = 0; i < c2->axiom_Count; i++ )
    {
        for ( j = 0; j < c1->axiom_Count; j++ )
        {                       /* look for previous definition */

            if ( !strcmp( c2->axioms[i].name, c1->axioms[j].name ) )
                break;
        }

        if ( j == c1->axiom_Count )
        {                       /* create new element */
            c1->axioms =
                MEM_AddObject( c1->axioms, ( c1->axiom_Count + 1 ), sizeof ( c1->axioms[0] ) );
            memset( &c1->axioms[j], 0, sizeof ( c1->axioms[0] ) );
            c1->axioms[j].name = MEM_CopyStr( c2->axioms[i].name );
            c1->axioms[j].axiom = MEM_CopyStr( c2->axioms[i].axiom );
            c1->axiom_Count++;
        }

    }                           /* finish instance processing */

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_BuildConfigSignals
 *
 * Description: build signals configuration
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_BuildConfigSignals( TAT_XML_CONFIG_DEF * c1,
    TAT_XML_CONFIG_DEF * c2 )
{
    size_t                    i, j, k;

/* copy signal definitions from base configuration to target */

    for ( i = 0; i < c2->signal_Count; i++ )
    {
        for ( j = 0; j < c1->signal_Count; j++ )
        {                       /* look for previous definition */

            if ( !strcmp( c2->signals[i].name, c1->signals[j].name ) )
                break;
        }

        if ( j == c1->signal_Count )
        {                       /* create new element */
            c1->signals = MEM_AddObject( c1->signals, ( c1->signal_Count + 1 ), sizeof ( c1->signals[0] ) );
            memset( &c1->signals[j], 0, sizeof ( c1->signals[0] ) );
            c1->signals[j].name = MEM_CopyStr( c2->signals[i].name );
            c1->signal_Count++;
        }

        if ( !c1->signals[j].type )
            c1->signals[j].type = c2->signals[i].type;

        if ( !c1->signals[j].attr_Type )
            c1->signals[j].attr_Type = MEM_CopyStr( c2->signals[i].attr_Type );

        if ( !c1->signals[j].priority )
            c1->signals[j].priority = c2->signals[i].priority;

    /* if no parameters are specified, copy it from previous definition */
        if ( !c1->signals[j].param_Count && !c1->signals[j].param_Extra )
        {
        /* copy parameters attributes */
            c1->signals[j].param_Count = c2->signals[i].param_Count;
            c1->signals[j].param_Extra = c2->signals[i].param_Extra;

            if ( c1->signals[j].param_Count )
            {
                c1->signals[j].source_Format =
                    MEM_CopyStrArray( c1->signals[j].param_Count, c2->signals[i].source_Format );
                c1->signals[j].param_TgtType =
                    MEM_CopyStrArray( c1->signals[j].param_Count, c2->signals[i].param_TgtType );
                c1->signals[j].param_FieldName =
                    MEM_CopyStrArray( c1->signals[j].param_Count, c2->signals[i].param_FieldName );
            }
        }

    /* expand number of attributes */
        if ( c1->signals[j].attr_Count < c2->signals[i].attr_Count )
        {
            c1->signals[j].attr =
                MEM_AddObject( c1->signals[j].attr, c2->signals[i].attr_Count,
                sizeof ( c1->signals[0].attr[0] ) );
            memset( &c1->signals[j].attr[c1->signals[j].attr_Count], 0,
                ( c2->signals[i].attr_Count - c1->signals[j].attr_Count ) * sizeof ( c1->signals[0].attr[0] ) );
            c1->signals[j].attr_Count = c2->signals[i].attr_Count;
        }

    /* copy attributes. after prev. step c2.attr_Count<=c1.attr_Count */
        for ( k = 0; k < c2->signals[i].attr_Count; k++ )
        {
            if ( !c1->signals[j].attr[k] )
                c1->signals[j].attr[k] = MEM_CopyStr( c2->signals[i].attr[k] );
        }
    }                           /* finish signals processing */

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_BuildConfigData
 *
 * Description: build data configuration
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_BuildConfigData( TAT_XML_CONFIG_DEF * c1,
    TAT_XML_CONFIG_DEF * c2 )
{
    size_t                    i, j, k;

/* copy data definitions from base configuration to target */

    for ( i = 0; i < c2->data_Count; i++ )
    {
        for ( j = 0; j < c1->data_Count; j++ )
        {
        /* look for previous definition */
            if ( !strcmp( c2->data[i].name, c1->data[j].name ) )
                break;
        }

        if ( j == c1->data_Count )
        {                       /* create new element */
            c1->data = MEM_AddObject( c1->data, ( c1->data_Count + 1 ), sizeof ( c1->data[0] ) );
            memset( &c1->data[j], 0, sizeof ( c1->data[0] ) );
            c1->data[j].name = MEM_CopyStr( c2->data[i].name );
            c1->data_Count++;
        }

    /* copy target name, if not specified */
        if ( !c1->data[j].target_Name )
            c1->data[j].target_Name = MEM_CopyStr( c2->data[i].target_Name );

        if ( !c1->data[j].attr_Type )
            c1->data[j].attr_Type = MEM_CopyStr( c2->data[i].attr_Type );

        if ( !c1->data[j].prefix )
            c1->data[j].prefix = MEM_CopyStr( c2->data[i].prefix );

    /* copy list of values, if it was not defined */
        if ( !c1->data[j].value_Count && c2->data[i].value_Count )
        {
            c1->data[j].value_List = MEM_CopyStrArray( c2->data[i].value_Count, c2->data[i].value_List );
            c1->data[j].enums_List = MEM_CopyStrArray( c2->data[i].value_Count, c2->data[i].enums_List );
            c1->data[j].value_Count = c2->data[i].value_Count;
        }

        if ( c1->data[j].data_Class == MSC_DATA_UNKNOWN )
            c1->data[j].data_Class = c2->data[i].data_Class;

    /* expand number of attributes */
        if ( c1->data[j].attr_Count < c2->data[i].attr_Count )
        {
            c1->data[j].attr =
                MEM_AddObject( c1->data[j].attr, c2->data[i].attr_Count, sizeof ( c1->data[0].attr[0] ) );
            memset( &c1->data[j].attr[c1->data[j].attr_Count], 0,
                ( c2->data[i].attr_Count - c1->data[j].attr_Count ) * sizeof ( c1->data[0].attr[0] ) );
            c1->data[j].attr_Count = c2->data[i].attr_Count;
        }

    /* copy attributes. after prev. step c2.attr_Count<=c1.attr_Count */
        for ( k = 0; k < c2->data[i].attr_Count; k++ )
        {
            if ( !c1->data[j].attr[k] )
                c1->data[j].attr[k] = MEM_CopyStr( c2->data[i].attr[k] );
        }

	/* copy array dimension and structure members */
		c1->data[j].dimension=MEM_CopyStr(c2->data[i].dimension);
		c1->data[j].member_Count=c2->data[i].member_Count;
		MEM_AddObject(c1->data[j].member_List,c1->data[j].member_Count,sizeof(c1->data[j].member_List[0]));
		for(k=0;k<c1->data[j].member_Count;k++)
		{
			c1->data[j].member_List[k].dimension=MEM_CopyStr(c2->data[i].member_List[k].dimension);
			c1->data[j].member_List[k].name=MEM_CopyStr(c2->data[i].member_List[k].name);
			c1->data[j].member_List[k].type=MEM_CopyStr(c2->data[i].member_List[k].type);
		}
    }                           /* finish data type processing */

    return RESULT_OK;
}

/***************************************************************************
 * Function : tat_BuildConfigMacro
 *
 * Description: Build configuration for macros
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL tat_BuildConfigMacro( const char * fname, TAT_XML_CONFIG_DEF * c1,   /* target configuration */

    TAT_XML_CONFIG_DEF * c2 )
{
    size_t                    i, j, k;

    for ( i = 0; i < c2->macro_Count; i++ )
    {
        for ( j = 0; j < c1->macro_Count; j++ )
        {                       /* look for previous definition */

            if ( c2->macro[i].name && c1->macro[j].name && !strcmp( c2->macro[i].name, c1->macro[j].name ) )
                break;
        }

        if ( j == c1->macro_Count ) /* if not found */
        {                       /* create new element */
            c1->macro = MEM_AddObject( c1->macro, ( c1->macro_Count + 1 ), sizeof ( c1->macro[0] ) );
            memset( &c1->macro[j], 0, sizeof ( c1->macro[0] ) );
            c1->macro[j].name = MEM_CopyStr( c2->macro[i].name );
            c1->macro_Count++;
        }
        switch ( c1->macro[j].replace_Type )
        {

        case MSC_REPL_UNDEFINED:
        {
        /* copy, if inherited from parent */
            c1->macro[j].type = c2->macro[i].type;
            c1->macro[j].data_Type = c2->macro[i].data_Type;
            c1->macro[j].replace_Type = c2->macro[i].replace_Type;
            c1->macro[j].vector_Name = MEM_CopyStr( c2->macro[i].vector_Name );
            c1->macro[j].priority = c2->macro[i].priority;
            c1->macro[j].attr_Count = c2->macro[i].attr_Count;
            c1->macro[j].research = c2->macro[i].research;
            c1->macro[j].prio_Number = c2->macro[i].prio_Number;

            c1->macro[j].attr = MEM_CopyStrArray( c1->macro[j].attr_Count, c2->macro[i].attr );

            c1->macro[j].data_Count = c2->macro[i].data_Count;
            c1->macro[j].data = MEM_Allocate( c1->macro[j].data_Count * sizeof ( c1->macro[j].data[0] ) );

            for ( k = 0; k < c1->macro[j].data_Count; k++ )
            {
                c1->macro[j].data[k].condition = MEM_CopyStr( c2->macro[i].data[k].condition );
                c1->macro[j].data[k].data_Count = c2->macro[i].data[k].data_Count;
                c1->macro[j].data[k].data =
                    MEM_CopyStrArray( c1->macro[j].data[k].data_Count, c2->macro[i].data[k].data );
            }
            break;
        }

        case MSC_REPL_REPLACE:
        {
        /* nothing to do, cause full replacement */
            break;
        }

        case MSC_REPL_EXTEND:

        case MSC_REPL_SUBSET:
        {
            size_t                    ii, jj, kk;

        /* check for compatible types of parent and child */

            if ( c1->macro[j].type != c2->macro[i].type ||
                c1->macro[j].data_Type != c2->macro[i].data_Type || c1->macro[j].type != MSC_MACROTYPE_LIST )
                break;

            if ( c1->macro[j].replace_Type == MSC_REPL_SUBSET )
            {
            /* subset means check that new is containing in old */

                for ( k = 0; k < c1->macro[j].data_Count; k++ )
                {
                    for ( kk = 0; kk < c2->macro[i].data_Count; kk++ )
                    {
                        if ( ( c1->macro[j].data[k].condition && c2->macro[i].data[kk].condition &&
                                !strcmp( c1->macro[j].data[k].condition, c2->macro[i].data[kk].condition ) )
                            || ( !c1->macro[j].data[k].condition && !c2->macro[i].data[kk].condition ) )
                            break;
                    }

                    if ( kk == c2->macro[i].data_Count )
                    {
                        MSG_XMLErrorA( fname, c1->macro[j].line_Number,
                            MSG_XML_MACRO_SUBSET_ss, c1->macro[j].name,
                            c1->macro[j].data[k].condition );
                        return RESULT_ERR;   /* is not subset */
                    }

                    for ( ii = 0; ii < c1->macro[j].data[k].data_Count; ii++ )
                    {
                        for ( jj = 0; jj < c2->macro[i].data[kk].data_Count; jj++ )
                        {
                            if ( !strcmp( c1->macro[j].data[k].data[ii], c2->macro[i].data[kk].data[jj] ) )
                                break;
                        }

                        if ( jj == c2->macro[i].data[kk].data_Count )
                        {
                            MSG_XMLErrorA( fname, c1->macro[j].line_Number,
                                MSG_XML_MACRO_SUBSET_ss, c1->macro[j].name,
                                c1->macro[j].data[k].data[ii] );
                            return RESULT_ERR;   /* is not subset */
                        }
                    }
                }
            }
            else
            {                   /* MSC_REPL_EXTEND */
            /* Extend means that new=new+old */

                for ( kk = 0; kk < c2->macro[i].data_Count; kk++ )
                {
                    for ( k = 0; k < c1->macro[j].data_Count; k++ )
                    {
                        if ( ( c1->macro[j].data[k].condition && c2->macro[i].data[kk].condition &&
                                !strcmp( c1->macro[j].data[k].condition, c2->macro[i].data[kk].condition ) )
                            || ( !c1->macro[j].data[k].condition && !c2->macro[i].data[kk].condition ) )
                            break;
                    }

                    if ( k == c1->macro[j].data_Count )
                    {           /* add whole condition */
                        c1->macro[j].data =
                            MEM_AddObject( c1->macro[j].data, ( k + 1 ), sizeof ( c1->macro[0].data[0] ) );
                        c1->macro[j].data[k].condition = MEM_CopyStr( c2->macro[i].data[kk].condition );
                        c1->macro[j].data[k].data_Count = c2->macro[i].data[kk].data_Count;
                        c1->macro[j].data[k].data =
                            MEM_CopyStrArray( c2->macro[i].data[kk].data_Count, c2->macro[i].data[kk].data );
                        c1->macro[j].data_Count = k + 1;
                    }
                    else
                    {           /* add only values in condition */

                        for ( jj = 0; jj < c2->macro[i].data[kk].data_Count; jj++ )
                        {
                            for ( ii = 0; ii < c1->macro[j].data[k].data_Count; ii++ )
                            {
                                if ( !strcmp( c1->macro[j].data[k].data[ii], c2->macro[i].data[kk].data[jj] ) )
                                    break;
                            }

                            if ( ii == c1->macro[j].data[k].data_Count )
                            {
                                c1->macro[j].data[k].data =
                                    MEM_AddObject( c1->macro[j].data[k].data, ( ii + 1 ),
                                    sizeof ( c1->macro[0].data[0].data[0] ) );
                                c1->macro[j].data[k].data[ii] = MEM_CopyStr( c2->macro[i].data[kk].data[jj] );
                                c1->macro[j].data[k].data_Count = ii + 1;
                            }
                        }
                    }
                }
            }

            break;
        }

        case MSC_REPL_REMOVE:
        {
            if ( c1->macro[j].name )
                MEM_Free( c1->macro[j].name );

            if ( c1->macro[j].vector_Name )
                MEM_Free( c1->macro[j].vector_Name );

            MEM_FreeStrArray( c1->macro[j].attr_Count, c1->macro[j].attr );

            if ( c1->macro[j].data_Count )
            {
                for ( k = 0; k < c1->macro[j].data_Count; k++ )
                {
                    if ( c1->macro[j].data[k].condition )
                        MEM_Free( c1->macro[j].data[k].condition );

                    MEM_FreeStrArray( c1->macro[j].data[k].data_Count, c1->macro[j].data[k].data );
                }

                MEM_Free( c1->macro[j].data );
            };

            memmove( &c1->macro[j], &c1->macro[j + 1], ( c1->macro_Count - j - 1 ) * sizeof ( c1->macro[0] ) );
            c1->macro_Count--;
            break;
        }
        }
    }

/* correct priority numbers to fit in number of elements */
    for ( i = 0; i < c1->macro_Count; i++ )
    {
        if ( c1->macro[i].type == MSC_MACROTYPE_LIST && c1->macro[i].research == MSC_RESEARCH_PRIORITY )
        {
            for ( j = 0; j < c1->macro[i].data_Count; j++ )
            {
                if ( c1->macro[i].prio_Number > c1->macro[i].data[j].data_Count )
                    c1->macro[i].prio_Number = c1->macro[i].data[j].data_Count;
            }

        }
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : TAT_BuildConfig
 *
 * Description: Build specified configuration by merging data from basic's,
 *              Match Signals target parameters data type
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
TAT_XML_CONFIG_DEF     *TAT_BuildConfig( const char * name,
    TAT_XML_CONFIG * data )
{
    size_t                    i, j, k, cc;
    TAT_XML_CONFIG_DEF     *c1, *c2;

    cc = data->config_Count;

    for ( i = 0; i < cc; i++ )
    {
        if ( !stricmp( name, data->config[i].name ) )
            break;
    }

    if ( i == cc )
    {
        MSG_XMLErrorA( data->fname, 1, MSG_XML_INVALID_BASE_CONF_s, name );
        return NULL;
    }

    c1 = &data->config[i];

    while ( c1->base )
    {
        for ( i = 0; i < cc; i++ )
        {
            if ( !stricmp( c1->base, data->config[i].name ) )
                break;
        }                       /* shall be found */

        c2 = &data->config[i];

    /* move base pointer to base of base configuration */
        MEM_Free( c1->base );

        c1->base = MEM_CopyStr( c2->base );

        if ( !tat_BuildConfigInstances( c1, c2 ) )
            return NULL;

        if ( !tat_BuildConfigSignals( c1, c2 ) )
            return NULL;

        if ( !tat_BuildConfigData( c1, c2 ) )
            return NULL;

        if ( !tat_BuildConfigMacro( data->fname, c1, c2 ) )
        {
            return NULL;
        }

        if ( !tat_BuildConfigAxioms( c1, c2 ) )
        {
            return NULL;
        }
    }
/* finish configuration processing */

/* match SDL/UDF */
    for ( j = 0; j < c1->signal_Count; j++ )
    {
        for ( k = 0; k < c1->signals[j].param_Count; k++ )
        {
            for ( i = 0; i < c1->data_Count; i++ )
            {
            /* check that declared names are equal */

                if ( !stricmp( c1->signals[j].source_Format[k], c1->data[i].name ) )
                {
                    MEM_Free( c1->signals[j].param_TgtType[k] );
                /* modify target name */
                    c1->signals[j].param_TgtType[k] = MEM_CopyStr( c1->data[i].target_Name );
                    break;
                }
            };
        };
    }

/* sort definitions by names */
    if ( c1->signal_Count )
        qsort( c1->signals, c1->signal_Count, sizeof ( c1->signals[0] ), xml_CompSignals );

    if ( c1->instance_Count )
        qsort( c1->instances, c1->instance_Count, sizeof ( c1->instances[0] ), xml_CompInstances );

    if ( c1->axiom_Count )
        qsort( c1->axioms, c1->axiom_Count, sizeof ( c1->axioms[0] ), xml_CompAxioms );

    if ( c1->data_Count )
        qsort( c1->data, c1->data_Count, sizeof ( c1->data[0] ), xml_CompData );

    if ( c1->macro_Count )
        qsort( c1->macro, c1->macro_Count, sizeof ( c1->macro[0] ), xml_CompMacro );

    return c1;
}
