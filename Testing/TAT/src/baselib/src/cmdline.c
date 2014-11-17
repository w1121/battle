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

#include <portable.h>
#include <cmdline.h>
#include <qmemory.h>
#include <fileproc.h>
#include <strproc.h>
#include <outmess.h>
#include <mscmess.h>
#include <tatgdata.h>

/***************************************************************************
 * Function : CMD_LoadList
 *
 * Description: load names from file
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes:
 *
 **************************************************************************/
static LOGICAL CMD_LoadList( char * fname, void *storage, size_t * counter )
{
    FILE                    *f;
    char                dest[TAT_MAX_FNAME_LEN];
    char                pathf[TAT_MAX_FNAME_LEN];
    char                pn[TAT_MAX_FNAME_LEN];

    f = FP_OpenFile( fname, "r" );

    if ( !f )
        return LOGICAL_FALSE;

    FP_SplitPath( fname, pathf, NULL, NULL );

/* until there are lines */
    while ( !feof( f ) )
    {
        if ( !fgets( dest, TAT_MAX_FNAME_LEN - 1, f ) )
            break;

    /* remove unnecessary spaces */
        StrCleanEOL( dest );

        StrTrimSpaces( dest );

    /* if it is not empty */
        if ( dest[0] )
        {
            if ( dest[0] == '@' )
            {
                FP_SplitPath( dest + 1, pn, NULL, NULL );

                if ( pn[0] == 0 && pathf[0] != 0 )
                {
                    strcpy( pn, pathf );
                    strcat( pn, dest + 1 );

                    if ( stricmp( fname, pn ) )
                        CMD_LoadList( pn, storage, counter );
                }
                else if ( stricmp( fname, dest + 1 ) )
                    CMD_LoadList( dest + 1, storage, counter );
            }
            else
            {
                /* handle case, when file name is in quotes */
                if (*dest=='\"')
                {
                    size_t ilen=strlen(dest);
                    if (dest[ilen-1]=='\"')
                    {
                        dest[ilen-1]=0;
                        memmove(&dest, &dest[1], ilen-1);
                    }
                }
            /* add new filename into list */
                FP_SplitPath( dest, pn, NULL, NULL );
                *( ( void ** ) storage ) =
                    MEM_AddObject( *( ( void ** ) storage ), ( ( *counter ) + 1 ), sizeof ( char * ) );

                if ( pn[0] == 0 && pathf[0] != 0 )
                {
                    strcpy( pn, pathf );
                    strcat( pn, dest );
                /* check for maximal file name length */
                    if ( strlen( pn ) > TAT_MAX_FNAME_LEN )
                    {
                        MSG_PrintError( MSG_CMD_LONG_NAME_s, pn );
                        return LOGICAL_FALSE;
                    }
                    ( *( ( char *** ) storage ) )[*counter] = MEM_CopyStr( pn );
                }
                else
                {
                /* check for maximal file name length */

                    if ( strlen( dest ) > TAT_MAX_FNAME_LEN )
                    {
                        MSG_PrintError( MSG_CMD_LONG_NAME_s, dest );
                        return LOGICAL_FALSE;
                    }
                    ( *( ( char *** ) storage ) )[*counter] = MEM_CopyStr( dest );
                }
                FP_NormalizePath( ( *( ( char *** ) storage ) )[*counter] );
            /* increment filelist counter */
                ( *counter )++;
            }
        }
    }

    fclose( f );
    return LOGICAL_TRUE;
}

/***************************************************************************
 * Function : tat_RemoveDupNames
 *
 * Description: remove duplicated file names from the list
 *
 * Returns: none
 *
 * Notes: O(N^2) time complexity
 *
 **************************************************************************/
static void tat_RemoveDupNames( size_t * count, char **m )
{
    size_t  i, j, n;

    n = *count;
    i = 0;

    /* remove empty elements */
    while (n && !m[i])
    {
        memcpy( m + i, m + i + 1, ( n - i ) * sizeof ( char * ) );
        n--;
    }

    while ( n && ( i < n - 1 ) )
    {

        j = i + 1;

        while ( j < n )
        {
        /* ignore case of file names for compatibility */

            if ( !m[j] || !stricmp( m[i], m[j] ) )
            {
                memcpy( m + j, m + j + 1, ( n - j ) * sizeof ( char * ) );
                n--;
            }
            else
                j++;
        }

        i++;
    }

/* propagate counter */
    *count = n;
}

/***************************************************************************
 * Function : CMD_Parse
 *
 * Description: parse command line parameters
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes:
 *
 **************************************************************************/
LOGICAL cmd_Parse( const size_t switches, CMD_LINE * cmd, const int argc, char *argv[] )
{
    int                     brk, temp, should_data, may_data;
    size_t                  i, j, mode, key;
    int                    *keys;
    char                    buffer[TAT_MAX_FNAME_LEN];

/* check input parameters */

    if ( ( argc < 1 ) || !argv || !cmd || ( switches <= 0 ) )
    {
        return LOGICAL_FALSE;
    }

/* set default impossible mode */
    mode = switches;
    should_data = 0;
    may_data = 0;

    /* hard-coded switch for build verification */
    if (!strcmp(argv[1], "--v"))
    {
        fprintf( stdout, "Built on " __DATE__" " __TIME__"\n\n");
        return LOGICAL_FALSE;
    }

    keys = MEM_Allocate( switches * sizeof ( int ) );
    for ( i = 1; i < (size_t) argc; i++ )
    {
        if ( !argv[i] )
            return LOGICAL_FALSE;

    /* check for keys */
        brk = 0;

    /* look for switch with such name */
        for ( key = 0; key < switches; key++ )
        {
            if ( cmd[key].switch_type == CMD_GROUP_START )
                continue;

            if ( !strcmp( cmd[key].switch_name, argv[i] ) )
            {
            /* if data expected output error message */

                if ( should_data )
                {
                    MSG_PrintError( MSG_CMD_DATA_EXPECTED_s, cmd[mode].switch_name );
                    return LOGICAL_FALSE;
                }

                mode = key;
                brk = 1;

                if ( keys[key] )
                {
                    MSG_PrintError( MSG_CMD_DUP_SWITCH_s, argv[i] );
                }

                keys[key] = 1;

                switch ( cmd[mode].switch_type )
                {

                case CMD_BOOLEAN:
                {
                /* reset semaphores */
                    should_data = 0;
                    may_data = 0;
                    break;
                }

                case CMD_INTEGER:
                case CMD_NATURAL:
                case CMD_STRING:
                case CMD_FILE_NAME:
                case CMD_PATH_NAME:
                case CMD_PATH_LIST:
                case CMD_FLOAT:
                case CMD_FILE_LIST:
                case CMD_INTEGER_LIST:
                case CMD_STRING_LIST:
                {
                /* set all semaphores - data shall be provided */
                    should_data = 1;
                    may_data = 1;
                    break;
                }
                }

                break;
            }
        }

    /* look for unbinded element */
        if ( mode == switches ||
            ( cmd[mode].switch_type == CMD_BOOLEAN && strcmp( cmd[mode].switch_name, argv[i] ) ) )
        {
            MSG_PrintError( ( i > 1 ) ? MSG_CMD_LINE_ERR_ss : MSG_CMD_LINE_ERR_s,
                ( i > 1 ) ? argv[i - 1] : argv[i], argv[i] );
            MEM_Free( keys );
            return LOGICAL_FALSE;
        }

    /* set boolean switch */
        if ( cmd[mode].switch_type == CMD_BOOLEAN )
        {
            *( ( int * ) ( cmd[mode].storage ) ) = 1;
        }

    /* check for data out of switch */
        if ( !brk && mode != switches && !should_data && !may_data )
        {
            MSG_PrintError( ( i > 1 ) ? MSG_CMD_LINE_ERR_ss : MSG_CMD_LINE_ERR_s,
                ( i > 1 ) ? argv[i - 1] : argv[i], argv[i] );
            MEM_Free( keys );
            return LOGICAL_FALSE;
        }

        if ( brk )
            continue;

        switch ( cmd[mode].switch_type )
        {

        case CMD_NATURAL:
        {
        /* check for correct number */

            if ( !StrIsNaturalNumber( argv[i] ) )
            {
                MSG_PrintError( MSG_INVALID_SWITCH_VALUE_s, cmd[mode].switch_name );
                MEM_Free( keys );
                return LOGICAL_FALSE;
            }

            *( ( UINT32_T * ) ( cmd[mode].storage ) ) = ( unsigned int ) StrToInt( argv[i] );
            should_data = 0;
            may_data = 0;
            break;
        }
        case CMD_INTEGER:
        {
        /* check for correct number */

            if ( !StrIsIntegerNumber( argv[i] ) )
            {
                MSG_PrintError( MSG_INVALID_SWITCH_VALUE_s, cmd[mode].switch_name );
                MEM_Free( keys );
                return LOGICAL_FALSE;
            }

            *( ( int * ) ( cmd[mode].storage ) ) = StrToInt( argv[i] );
            should_data = 0;
            may_data = 0;
            break;
        }

        case CMD_FLOAT:
        {
        /* check for correct number */

            if ( !StrIsAnyNumber( argv[i] ) )
            {
                MSG_PrintError( MSG_INVALID_SWITCH_VALUE_s, cmd[mode].switch_name );
                MEM_Free( keys );
                return LOGICAL_FALSE;
            }

            *( ( float * ) ( cmd[mode].storage ) ) = ( float ) StrToDouble( argv[i] );
            should_data = 0;
            may_data = 0;
            break;
        }

        case CMD_FILE_NAME:
        case CMD_PATH_NAME:
        case CMD_STRING:
        {
        /* check for duplicated definition of string */

            if ( *( ( char ** ) ( cmd[mode].storage ) ) )
            {
                MSG_PrintError( MSG_CMD_WARN_OVRR_ssss, cmd[mode].switch_name, argv[i],
                    cmd[mode].switch_name, *( ( char ** ) ( cmd[mode].storage ) ) );
                MEM_Free( *( ( char ** ) ( cmd[mode].storage ) ) );
            }

        /* check for file/path name and normalize it */
            if ( ( cmd[mode].switch_type == CMD_FILE_NAME ) || ( cmd[mode].switch_type == CMD_PATH_NAME ) )
            {
            /* check for file name length to avoid further crash */
                j = strlen( argv[i] );
                if ( j >= TAT_MAX_FNAME_LEN )
                {
                    MSG_PrintError( MSG_CMD_LONG_NAME_s, argv[i] );
                    MEM_Free( keys );
                    return LOGICAL_FALSE;
                }
                strcpy( buffer, argv[i] );
                if ( cmd[mode].switch_type == CMD_PATH_NAME )
                {
                    if ( buffer[j - 1] != '/' && buffer[j - 1] != '\\' )
                    {
                        buffer[j] = '/';
                        buffer[j + 1] = 0;
                    }
                }
                FP_NormalizePath( buffer );
                *( ( char ** ) ( cmd[mode].storage ) ) = MEM_CopyStr( buffer );
            }
            else
                *( ( char ** ) ( cmd[mode].storage ) ) = MEM_CopyStr( argv[i] );

            should_data = 0;
            may_data = 0;
            break;
        }

        case CMD_INTEGER_LIST:
        {
            if ( !may_data )
            {
                MSG_PrintError( ( i > 1 ) ? MSG_CMD_LINE_ERR_ss : MSG_CMD_LINE_ERR_s,
                    ( i > 1 ) ? argv[i - 1] : argv[i], argv[i] );
                MEM_Free( keys );
                return LOGICAL_FALSE;
            }

        /* check for correct number */
            if ( !StrIsIntegerNumber( argv[i] ) )
            {
                MSG_PrintError( MSG_INVALID_SWITCH_VALUE_s, cmd[mode].switch_name );
                MEM_Free( keys );
                return LOGICAL_FALSE;
            }

            temp = atoi( argv[i] );
            *( ( void ** ) ( cmd[mode].storage ) ) =
                MEM_AddObject( *( ( void ** ) ( cmd[mode].storage ) ),
                ( *( cmd[mode].counter ) + 1 ), sizeof ( int ) );

            ( *( ( int ** ) ( cmd[mode].storage ) ) )[*( cmd[mode].counter )] = temp;
            ( *( cmd[mode].counter ) )++;
            should_data = 0;
            may_data = 1;
            break;
        }

        case CMD_STRING_LIST:
        {
            if ( !may_data )
            {
                MSG_PrintError( ( i > 1 ) ? MSG_CMD_LINE_ERR_ss : MSG_CMD_LINE_ERR_s,
                    ( i > 1 ) ? argv[i - 1] : argv[i], argv[i] );
                MEM_Free( keys );
                return LOGICAL_FALSE;
            }

            *( ( void ** ) ( cmd[mode].storage ) ) =
                MEM_AddObject( *( ( void ** ) ( cmd[mode].storage ) ),
                ( *( cmd[mode].counter ) + 1 ), sizeof ( char * ) );

            ( *( ( char *** ) ( cmd[mode].storage ) ) )[*( cmd[mode].counter )] = MEM_CopyStr( argv[i] );
            ( *( cmd[mode].counter ) )++;
            should_data = 0;
            may_data = 1;
            break;
        }

        case CMD_FILE_LIST:
        case CMD_PATH_LIST:
        {
            if ( !may_data )
            {
                MSG_PrintError( ( i > 1 ) ? MSG_CMD_LINE_ERR_ss : MSG_CMD_LINE_ERR_s,
                    ( i > 1 ) ? argv[i - 1] : argv[i], argv[i] );
                MEM_Free( keys );
                return LOGICAL_FALSE;
            }

        /* check for filelist specification */
            if ( cmd[mode].switch_type==CMD_FILE_LIST && (argv[i][0] == '@') )
            {
                if ( !CMD_LoadList( argv[i] + 1, cmd[mode].storage, cmd[mode].counter ) )
                {
                    MEM_Free( keys );
                    return LOGICAL_FALSE;
                }
            }
            else
            {
                j = strlen( argv[i] );
                if ( j >= TAT_MAX_FNAME_LEN )
                {
                    MSG_PrintError( MSG_CMD_LONG_NAME_s, argv[i] );
                    MEM_Free( keys );
                    return LOGICAL_FALSE;
                }
                j = *cmd[mode].counter;

                if ((cmd[mode].switch_type==CMD_FILE_LIST) &&
                    !FP_ExpandMask( argv[i], cmd[mode].counter, ( char *** ) ( cmd[mode].storage ) ) )
                {
                    MEM_Free( keys );
                    return LOGICAL_FALSE;
                };
            /* if nothing was added, add the element itself */
                if ( j == *cmd[mode].counter )
                {
                    *( ( void ** ) ( cmd[mode].storage ) ) =
                        MEM_AddObject( *( ( void ** ) ( cmd[mode].storage ) ), ( j + 1 ), sizeof ( char * ) );

                    if ( cmd[mode].switch_type == CMD_PATH_LIST )
                    {
                        size_t end_str;
                        strncpy(buffer, argv[i], sizeof(buffer));
                        end_str=strlen(buffer);
                        if ( buffer[end_str - 1] != '/' && buffer[end_str - 1] != '\\' )
                        {
                            buffer[end_str] = '/';
                            buffer[end_str + 1] = 0;
                        }
                        FP_NormalizePath( buffer );
                        ( *( ( char *** ) ( cmd[mode].storage ) ) )[j] = MEM_CopyStr( buffer );
                    }
                    else
                    {
                        ( *( ( char *** ) ( cmd[mode].storage ) ) )[j] = MEM_CopyStr( argv[i] );
                        FP_NormalizePath( ( *( ( char *** ) cmd[mode].storage ) )[j] );
                    }
                    /* increment number of elements */
                    ( *cmd[mode].counter )++;
                }
            }

            should_data = 0;
            may_data = 1;
            break;
        }
        }
    }

    if ( should_data )
    {
        MSG_PrintError( MSG_CMD_DATA_EXPECTED_s, cmd[mode].switch_name );
        return LOGICAL_FALSE;
    }

    for ( key = 0; key < switches; key++ )
    {
        if ( cmd[key].switch_type == CMD_FILE_LIST ||
             cmd[key].switch_type == CMD_PATH_LIST
            )
            tat_RemoveDupNames( cmd[key].counter, *( char *** ) ( cmd[key].storage ) );
    }

    MEM_Free( keys );

    return LOGICAL_TRUE;
}

/***************************************************************************
 * Function : cmd_FreeMem
 *
 * Description: free memory allocated by preceding CMD_Parse call
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes:
 *
 **************************************************************************/
static LOGICAL cmd_FreeMem( const size_t switches, CMD_LINE * cmd )
{
    size_t                  i;
    size_t                  j;

/* look at all switches */

    for ( i = 0; i < switches; i++ )
    {
        switch ( cmd[i].switch_type )
        {

        case CMD_FILE_NAME:
        case CMD_PATH_NAME:
        case CMD_STRING:
        {
            if ( *( ( char ** ) ( cmd[i].storage ) ) )
            {
                MEM_Free( *( ( char ** ) ( cmd[i].storage ) ) );
                *( ( char ** ) ( cmd[i].storage ) ) = NULL;
            }

            break;
        }

        case CMD_INTEGER_LIST:
        {
            if ( *( ( void ** ) ( cmd[i].storage ) ) )
            {
                MEM_Free( *( ( void ** ) ( cmd[i].storage ) ) );
                *( ( void ** ) ( cmd[i].storage ) ) = NULL;
            }

            break;
        }

        case CMD_FILE_LIST:
        case CMD_STRING_LIST:
        case CMD_PATH_LIST:
        {                       /* remove strings first */

            for ( j = 0; j < *( cmd[i].counter ); j++ )
            {
                MEM_Free( ( *( ( char *** ) ( cmd[i].storage ) ) )[j] );
                ( *( ( char *** ) ( cmd[i].storage ) ) )[j] = NULL;
            }

            if ( *( ( void ** ) ( cmd[i].storage ) ) )
            {
                MEM_Free( *( ( void ** ) ( cmd[i].storage ) ) );
                *( ( void ** ) ( cmd[i].storage ) ) = NULL;
            }
            *( cmd[i].counter ) = 0;

            break;
        }

        }
    }

    return LOGICAL_TRUE;
}

static char            *CMD_get_param( CMD_SWITCH_TYPE x )
{
    switch ( x )
    {

    case CMD_NATURAL:
    case CMD_INTEGER:
        return "<int>";

    case CMD_FLOAT:
        return "<float>";

    case CMD_STRING:
        return "<string>";

    case CMD_FILE_LIST:
        return "<files>";

    case CMD_STRING_LIST:
        return "<strings>";

    case CMD_FILE_NAME:
        return "<file>";

    case CMD_PATH_NAME:
        return "<path>";

    case CMD_PATH_LIST:
        return "<paths>";

    case CMD_INTEGER_LIST:
        return "<ints>";
    };

    return "";
}

/***************************************************************************
 * Function : CMD_PrintHelp
 *
 * Description: free memory allocated by preceding CMD_Parse call
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes:
 *
 **************************************************************************/
LOGICAL CMD_PrintHelp( const size_t switches, CMD_LINE * cmd )
{
    size_t   i;

    if ( !switches )
        return RESULT_OK;

    if ( !cmd )
        return RESULT_ERR;

    for ( i = 0; i < switches; i++ )
    {
        if ( cmd[i].switch_type == CMD_GROUP_START )
        {
            printf( "\n%s\n\n", cmd[i].comments );
        }
        else
        {
            if ( cmd[i].comments )  /* do not output hidden switches */
            {
                printf( "%-10s %-9s - %s\n", cmd[i].switch_name,
                    CMD_get_param( cmd[i].switch_type ), cmd[i].comments );
            }
        }
    }

    return RESULT_OK;
}

static size_t save_switches = 0;
static CMD_LINE * save_cmd = NULL;

static void cmd_Free(void)
{
    cmd_FreeMem(save_switches, save_cmd);
    save_switches = 0;
    save_cmd = NULL;
}

/***************************************************************************
 * Function : CMD_Parse
 *
 * Description: parse command line parameters
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes:
 *
 **************************************************************************/
LOGICAL CMD_Parse( const size_t switches, CMD_LINE * cmd, const int argc, char *argv[] )
{
    if (!cmd_Parse(switches, cmd, argc, argv))
    {
        cmd_FreeMem(switches, cmd);
        return RESULT_ERR;
    }

    if (!save_switches)
    {
        save_switches = switches;
        save_cmd = cmd;
        atexit(cmd_Free);
    }
    return RESULT_OK;
}
