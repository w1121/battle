#ifndef XMLPARSE_H
#define XMLPARSE_H
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
* Functions:    XML_Load
*               XML_GetAttrValue
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

#ifdef __cplusplus
extern                  "C"
{
#endif                                 /* C++ */

typedef unsigned int XML_ELEMENT;

#define XML_INIT_STATE (XML_ELEMENT)0xffffffff

typedef struct _XML_TAG_LIST
{
    XML_ELEMENT     element_id;
    char *          element_name;
} XML_TAGLIST;



typedef struct _XML_TAG
{
    char *                  name;       /* XML tag name. case sensetive */
    size_t                  line_number; /* starting line number */
    size_t                  attributes; /* number of defined attributes */
    char *                 *attr_Name;  /* storage for attributes names */
    char *                 *attr_Value; /* storage for attributes values */
    char *                  content;    /* content of tag. including nested tags */
}
XML_TAG;

typedef enum _XML_ITERATOR
{
    XML_ONE,
    XML_ONEORMANY,
    XML_OMITORONCE,
    XML_ANY
} XML_ITERATOR;

typedef LOGICAL (* XML_PROCESS_FUNC)(const XML_ELEMENT element_id, const XML_TAG *xmldata, void * outdata);

typedef struct _XML_DTD
{
    XML_ELEMENT current_id;             /* parent tag ID */
    XML_ELEMENT embedded_id;            /* child tag ID */
    XML_ITERATOR iterator;              /* iteration count */
    XML_PROCESS_FUNC parse_func;        /* function to process child tag in parent tag */
} XML_DTD;

extern LOGICAL XML_Load(const char * filename, const size_t tagcount, const XML_TAGLIST *taglist,
                 const size_t dtdsize, const XML_DTD * dtd, void * data);

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
extern const char *XML_GetAttrValue( const char *tag, const XML_TAG * data );

#ifdef __cplusplus
}
#endif                                 /* C++ */

#endif /* XMLPARSE_H */
