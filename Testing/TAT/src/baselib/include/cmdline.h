#ifndef CMD_LINE_H
#define CMD_LINE_H
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
* Functions:    CMD_Parse
*               CMD_FreeMem
*               CMD_PrintHelp
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  Command-line processing
*
* Notes:        None
*
******************************************************************************/
#ifdef __cplusplus
extern                  "C"
{
#endif                                 /* C++ */
#include <portable.h>

    typedef enum _CMD_SWITCH_TYPE
    {
        CMD_BOOLEAN,                   /* storage points to integer flag. */
        CMD_INTEGER,                   /* storage points to integer */
        CMD_NATURAL,                   /* storage points to unsigned integer */
        CMD_FLOAT,                     /* storage points to float */
        CMD_STRING,                    /* storage points to string */
        CMD_INTEGER_LIST,
        CMD_STRING_LIST,
        CMD_FILE_LIST,
        CMD_FILE_NAME,                 /* storage points to string */
        CMD_PATH_NAME,                 /* storage points to string */
        CMD_PATH_LIST,                 /* storage points to string */
        CMD_GROUP_START                /* switch group separator */
    }
    CMD_SWITCH_TYPE;

    typedef struct _CMD_LINE
    {
        char                   *switch_name;    /* name of switch */
        CMD_SWITCH_TYPE         switch_type;    /* type of switch */
        size_t                 *counter;    /* pointer to counter storage */
        void                   *storage;    /* pointer to values storage */
        char                   *comments;   /* help message */
    }
    CMD_LINE;

    extern LOGICAL          CMD_Parse( const size_t switches, CMD_LINE * cmd, const int argc, char *argv[] );

/*    extern LOGICAL          CMD_FreeMem( const int switches, CMD_LINE * cmd ); */

    extern LOGICAL          CMD_PrintHelp( const size_t switches, CMD_LINE * cmd );

#ifdef __cplusplus
}
#endif                                 /* C++ */
#endif                                 /* CMD_LINE_H */


