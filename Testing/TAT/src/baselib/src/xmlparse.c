/******************************************************************************
*
*       COPYRIGHT 2001-2004 MOTOROLA, ALL RIGHTS RESERVED
*
*       MOTOROLA CONFIDENTIAL PROPRIETARY
*
* Description:  Command-line processing
*
* $RCSfile: xmlparse.c,v $
* $Revision: 1.4 $
* $Source: /home/cvsusr/cvssoft/ptk_tools/baselib/src/xmlparse.c,v $
* $Date: 2004/03/05 14:37:44 $
*
*
* CHANGE HISTORY
*                                                       
* Initial version 
*
******************************************************************************/
#include <tatgdata.h>
#include <xmlparse.h>
#include <strproc.h>
#include <qmemory.h>
#include <outmess.h>
#include <fileproc.h>
#include <xmlmess.h>


static size_t             tat_XMLLineCounter;
static size_t             tat_ErrorOut;

/***************************************************************************
 * Function : xml_SkipSpaces
 *
 * Description: skip spaces with counting '\n' characters
 *
 * Returns: None
 *
 * Notes: None
 *
 **************************************************************************/
static void xml_SkipSpaces( char * * s )
{
    char *                 p = *s;

    while ( *p && *p < 33 )
    {
        if ( *p == '\n' )
        {
            tat_XMLLineCounter++;
        }
        p++;
    }
    *s = p;
}

/***************************************************************************
 * Function : xml_SkipTag
 *
 * Description: skip comment or special tag type
 *
 * Returns: RESULT_ERR/RESULT_OK (based on correct syntax)
 *
 * Notes: None
 *
 **************************************************************************/
static LOGICAL xml_SkipTag( char **str )
{
    int                 tagtype = 0;
    char                   *s;

    s = *str;
    xml_SkipSpaces( &s );

    if ( *s == '<' && ( *( s + 1 ) == '!' || *( s + 1 ) == '?' ) )
    {
        s++;
    /* determine special construction type */

        if ( *s == '!' && *( s + 1 ) == '-' && *( s + 2 ) == '-' )
        {
            tagtype = 0;
            s += 3;
        }
        else if ( *s == '!' && ( *( s + 1 ) != '-' || *( s + 2 ) != '-' ) )
        {
            tagtype = 1;
            s += 3;
        }
        else if ( *s == '?' )
        {
            tagtype = 2;
            s += 1;
        }

        while ( *s )
        {
            if ( tagtype == 0 && *s == '-' && *( s + 1 ) == '-' && *( s + 2 ) == '>' )
            {
                s += 2;
                break;
            };

            if ( tagtype == 1 && *s == '>' )
            {
                break;
            };

            if ( tagtype == 2 && *s == '?' && *( s + 1 ) == '>' )
            {
                s++;
                break;
            };

            s++;
        }

        if ( *s != '>' )
            return RESULT_ERR;

        *str = s + 1;
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : xml_GetTag
 *
 * Description: parse XML branch and fill structure
 *
 * Returns: pointer to next token
 *
 * Notes: None
 *
 **************************************************************************/
static char            *xml_GetTag( char *str, XML_TAG * tag )
{
    char                   *b;
    char                    label[TAT_MAX_ID_LEN];
    char                    value[TAT_MAX_LINE_LEN];

    if ( !tag || !str || *str != '<' )
        return NULL;

    memset( tag, 0, sizeof ( tag[0] ) );

    str++;

    xml_SkipSpaces( &str );

    if ( !StrExtractLabel( &str, label ) )
        goto tat_ERROR_EXIT;

/* check for namespace ??? */
    if ( *str == ':' )
    {
        str++;                  /* just ignore it */

        if ( !StrExtractLabel( &str, label ) )
            goto tat_ERROR_EXIT;
    }

    tag->name = MEM_CopyStr( label );

    while ( *str )
    {
        xml_SkipSpaces( &str );

        if ( *str == '>' || ( ( *str == '/' ) && ( *( str + 1 ) == '>' ) ) )
            break;

        if ( !StrExtractLabel( &str, label ) || SymIsDigit( label[0] ) )
        {
            return NULL;
        };

        xml_SkipSpaces( &str );

        if ( *str != '=' )
            goto tat_ERROR_EXIT;

        str++;

        xml_SkipSpaces( &str );

        if ( *str == '\"' || *str == '\'' )
        {
            if ( !StrExtractXMLString( &str, value ) )
                goto tat_ERROR_EXIT;
        }
        else
            goto tat_ERROR_EXIT;

        tag->attr_Name = MEM_AddObject( tag->attr_Name, ( tag->attributes + 1 ), sizeof ( tag->attr_Name[0] ) );

        tag->attr_Value =
            MEM_AddObject( tag->attr_Value, ( tag->attributes + 1 ), sizeof ( tag->attr_Value[0] ) );

        tag->attr_Value[tag->attributes] = MEM_CopyStr( value );

        tag->attr_Name[tag->attributes] = MEM_CopyStr( label );

        tag->attributes++;
    };

    if ( ( *str == '/' ) && ( *( str + 1 ) == '>' ) )
        return str + 2;         /* tag is self-closed */

    str++;

/* generate finish tag */
    sprintf( label, "</%s>", tag->name );

/* search for finish */
    b = strstr( str, label );   /* modified strstr shall be used ... */

    if ( !b )
        goto tat_ERROR_EXIT;

/* copy content */
    tag->content = (char *)MEM_Allocate( b - str + 1 );
    memcpy( tag->content, str, b - str );
    tag->content[b - str] = 0;
    return b + strlen( label );

  tat_ERROR_EXIT:
/*    MSG_PrintError( MSG_XML_INVALID_TAG_ssu,tag->name); */
    return NULL;
}

/***************************************************************************
 * Function : xml_FreeTag
 *
 * Description: free memory allocated for XML tag
 *
 * Returns: 
 *
 * Notes: None
 *
 **************************************************************************/
static void xml_FreeTag( XML_TAG * tag )
{
    if ( !tag )
        return;

    if ( tag->name )
        MEM_Free( tag->name );

    if ( tag->content )
        MEM_Free( tag->content );

    MEM_FreeStrArray( tag->attributes, tag->attr_Name );
    MEM_FreeStrArray( tag->attributes, tag->attr_Value );
}

/***************************************************************************
 * Function : XML_GetAttrValue
 *
 * Description: get value of tag attribute
 *
 * Returns: NULL/ value
 *
 * Notes: None
 *
 **************************************************************************/
const char  *XML_GetAttrValue( const char *tag, const XML_TAG * data )
{
    size_t                    i;

    if ( !data || !tag )
        return NULL;

    for ( i = 0; i < data->attributes; i++ )
    {
        if ( !strcmp( tag, data->attr_Name[i] ) )
            return data->attr_Value[i];
    };

    return NULL;
}

/***************************************************************************
 * Function : xml_GetTagID
 *
 * Description: map tag name to specific ID
 *
 * Returns: NULL/ value
 *
 * Notes: None
 *
 **************************************************************************/
static XML_ELEMENT xml_GetTagID(char *name, const size_t tagcount, const XML_TAGLIST *taglist)
{
    size_t i;

    for (i=0; i<tagcount; i++)
        if (!strcmp(name, taglist[i].element_name)) return taglist[i].element_id;

    return XML_INIT_STATE;
}

/***************************************************************************
 * Function : xml_GetProcessFunc
 *
 * Description: get index of function to process specific tag in current state
 *
 * Returns: NULL/ value
 *
 * Notes: None
 *
 **************************************************************************/
static size_t xml_GetProcessFunc(XML_ELEMENT state, XML_ELEMENT tag,
                                    const size_t dtdsize, const XML_DTD * dtd)
{
    size_t i;
    for (i=0; i<dtdsize; i++)
        if (dtd[i].current_id==state && dtd[i].embedded_id==tag)
        {
            return i;
        }
    return 0xffffffff;
}

/***************************************************************************
 * Function : xml_Parse
 *
 * Description: load information in XML configuration structure
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: from hiher level shall be invoked with state=tat_START
 *
 **************************************************************************/
static LOGICAL xml_Parse(   char *str, void * data, XML_ELEMENT state,
                            const size_t tagcount, const XML_TAGLIST *taglist,
                            const size_t dtdsize, const XML_DTD * dtd, int * counter)
{
    XML_TAG                 xdata;
    XML_ELEMENT             tag_id;
    size_t                  i;

    xml_SkipSpaces( &str );

    while ( *str )
    {
        if ( *str == '<' )
        {
            if ( *( str + 1 ) != '?' && *( str + 1 ) != '!' )   /* tag start */
            {

                str = xml_GetTag( str, &xdata );

                if ( !str )
                    goto tat_ERROR_EXIT1;

                tag_id=xml_GetTagID(xdata.name, tagcount, taglist);

                /* not defined tag is found */
                if (XML_INIT_STATE==tag_id)
                    goto tat_ERROR_EXIT1;

                i=xml_GetProcessFunc(state, tag_id, dtdsize, dtd);

                /* if there is no such transition in DTD */
                if (0xffffffff==i)
                    goto tat_ERROR_EXIT1;

                counter[i]++;

                /* set line counter */
                xdata.line_number=tat_XMLLineCounter;
                MSG_SetLineNumber( tat_XMLLineCounter );

                if (dtd[i].parse_func)
                {
                    if (!dtd[i].parse_func(tag_id, &xdata, data))
                    {
                        tat_ErrorOut = 1; /* error already printed */
                        goto tat_ERROR_EXIT1;                   
                    }
                }

                if ( xdata.content )
                {
                     if ( !xml_Parse( xdata.content, data, tag_id,
                         tagcount, taglist, dtdsize, dtd, counter) )
                     {
                         tat_ErrorOut = 1; /* error already printed */
                         goto tat_ERROR_EXIT1;
                     }
                }

                xml_FreeTag( &xdata );

                
            }
            else
            /* skip special comments */
            if ( !xml_SkipTag( &str ) )
                return RESULT_ERR;
            xml_SkipSpaces( &str );
        }
        else
        {
            if ( state == XML_INIT_STATE )
            {
                goto tat_ERROR_EXIT1;
            }
            str++;              /* skip unknown tag content */
        }
    }

    return RESULT_OK;

  tat_ERROR_EXIT1:

/* up-to-date error messaging */
    if ( !tat_ErrorOut )
    {
        MSG_SetLineNumber( tat_XMLLineCounter );
        MSG_XMLError( MSG_XML_SYNTAX_ERROR );
        tat_ErrorOut = 1;
    }
    xml_FreeTag( &xdata );
    return RESULT_ERR;
}


/***************************************************************************
 * Function : XML_Load
 *
 * Description: load XML file
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL XML_Load(const char * filename, const size_t tagcount, const XML_TAGLIST *taglist,
                 const size_t dtdsize, const XML_DTD * dtd, void * data)
{
    FILE                   *f;
    char *                  buffer;
    size_t                  fsize;
    LOGICAL                 res = RESULT_OK;
    int                     *counter = NULL;    

/* check input parameters */

    if ( !filename || !data )
        return RESULT_ERR;


    f = FP_OpenFile( filename, FP_READTEXT );

    if ( !f )
    {
        return RESULT_ERR;
    }

    fsize = FP_FileSize( f );

    if ( !fsize )
    {
        fclose( f );
        return RESULT_ERR;
    };

    buffer = MEM_Allocate( fsize + 1 );
    fread( buffer, fsize, 1, f );

/* finish buffer */
    buffer[fsize] = 0;

/* reset XML line counter */
    tat_XMLLineCounter = 1;
    tat_ErrorOut = 0;
    MSG_PushProcessedFile( filename );
    counter = MEM_Allocate(dtdsize * sizeof(int));
    res = xml_Parse( buffer, data, XML_INIT_STATE, tagcount, taglist, dtdsize, dtd, counter );
    MEM_Free(counter);
    MEM_Free( buffer );
    fclose( f );

    MSG_PopProcessedFile(  );

    return res;
}
