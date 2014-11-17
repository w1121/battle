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
* Functions:    FP_SplitPath
*               FP_NormalizePath
*               FP_MakeDir
*               FP_OpenFile
*               FP_FileSize
*
* History:      Use the ClearCase command "History"
*               to display revision history information.
*               
* Description:  File-processing functions
*
* Notes:        None
*
******************************************************************************/
#include <portable.h>
#include <fileproc.h>
#include <outmess.h>
#include <mscmess.h>
#include <qmemory.h>
#include <strproc.h>
#include <tatgdata.h>


#if OSTYPE!=WIN32
static char             tat_ConsoleName[] = "/dev/con";
#else
static char             tat_ConsoleName[] = "con";
#endif


const char *FP_WRITETEXT = "wt";
const char *FP_READTEXT = "rt";
const char *FP_WRITEBIN = "wb";
const char *FP_READBIN = "rb";


/***************************************************************************
 * Function : FP_SplitPath
 *
 * Description: splits  file name by path, name, extension
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes:
 *
 **************************************************************************/
LOGICAL FP_SplitPath( const char * path, char * dir, char * fname, char * ext )
{
    register int            p, last_slash, dot, path_start;
    size_t                  len;

    if ( !path )
    {
        return RESULT_ERR;
    }

    path_start = 0;
    last_slash = -1;
    dot = -1;

/* look for dot in name and end of path specification */
    for ( p = 0; path[p]; p++ )
    {
        if ( path[p] == '/' || path[p] == '\\' )
        {
            last_slash = p + 1;
        }
        else if ( path[p] == '.' )
        {
            dot = p;
        }
    }

    if ( last_slash != -1 )     /* directory exists */
    {
    /* if we have storage we can save path */
        if ( dir )
        {
            len = ( last_slash < TAT_MAX_FNAME_LEN - 1 ) ? last_slash : TAT_MAX_FNAME_LEN - 1;
            strncpy( dir, path, len );
            dir[len] = 0;
        }
        path_start = last_slash;
    }
    else if ( dir )
    {
        *dir = 0;
    }

    if ( ( dot != -1 ) && ( dot >= path_start ) )
    {
        if ( fname )
        {
            len = ( dot - path_start ) < TAT_MAX_FNAME_LEN - 1 ? ( dot - path_start ) : TAT_MAX_FNAME_LEN - 1;
            strncpy( fname, &path[path_start], len );
            fname[len] = 0;
        }

        if ( ext )
        {
            len = ( p - dot ) < TAT_MAX_FNAME_LEN - 1 ? ( p - dot ) : TAT_MAX_FNAME_LEN - 1;
            strncpy( ext, &path[dot], len );
            ext[len] = 0;
        }
    }
    else
    {
    /* save file name */
        if ( fname )
        {
            len = ( p - path_start ) < TAT_MAX_FNAME_LEN - 1 ? ( p - path_start ) : TAT_MAX_FNAME_LEN - 1;
            strncpy( fname, &path[path_start], len );
            fname[len] = 0;
        }

    /* save extension */
        if ( ext )
        {
            *ext = 0;
        }
    }

    return RESULT_OK;
}

/***************************************************************************
 * Function : FP_NormalizePath
 *
 * Description: change slashes to native for OS
 *
 * Returns: pointer to path
 *
 * Notes: normalize slashes
 *
 **************************************************************************/
char  *FP_NormalizePath( char * path )
{
    char                   *t;

    if ( !path )
        return NULL;

    t = path;

#if OTHER_FILE_SLASH != NORMAL_FILE_SLASH
    while ( *path )
    {
        if ( *path == OTHER_FILE_SLASH )
            *path = NORMAL_FILE_SLASH;

        path++;
    }
#endif

    return t;
}



/***************************************************************************
 * Function : FP_MakeDirW
 *
 * Description: create directories recursively
 *
 * Return: RESULT_ERROR/RESULT_OK
 *
 * Notes:
 *
 **************************************************************************/
LOGICAL FP_MakeDir( const char * path )
{
#if (COMPILER != GNU) && (OSTYPE == WIN32)
    char            p[PATH_MAX], currentdir[PATH_MAX];
    char            *start, *t, *pos;
    size_t             i;
    int                curd;

    if (!path) return RESULT_ERR;
    i = strlen(path);
    if (i>=PATH_MAX) 
    {
        MSG_PrintError(MSG_CMD_LONG_NAME_s, path);
        return RESULT_ERR;
    }

    if (!FP_FullName(p, path))
    {
        strcpy(p, path);
        FP_NormalizePath(p);
    }
    else
         i = strlen(p);

    if (p[i-1]=='\\') p[--i]=0;
    if (!p[0]) return RESULT_OK;

    /* save current drive and directory */
    curd = _getdrive();
    if (!getcwd(currentdir, PATH_MAX))
    {
        MSG_PrintFileError(path);
        return RESULT_ERR;
    }
    start=&p[0];

    /* check for share names */
    if (p[0]=='\\' && p[1]=='\\' && p[2]!='?') 
    {
        start = strchr(p+2, '\\');

        /* if only server is specified */        
        if (!start)
        {
            /* check if server exists */
            if (chdir(p)<0)
            {
                MSG_PrintError(MSG_FILE_DIR_CANT_CREATE_s, path);
                /* if only share name is specified */
                return RESULT_ERR;
            }
            _chdrive( curd);
            chdir( currentdir );
            return RESULT_OK;
        }
        /* skip sequential slashes */
        while ( '\\'== *start) start++;

        start = strchr(start, '\\');

        /* if only server and share name specified */
        if (!start)
        {
            /* check if server exists */
            if (chdir(p)<0)
            {
                MSG_PrintError(MSG_FILE_DIR_CANT_CREATE_s, path);
                /* if only share name is specified */
                return RESULT_ERR;
            }
            _chdrive( curd);
            chdir( currentdir );
            return RESULT_OK;
        }
        *start = 0;
        if (chdir(p)<0)
        {
            MSG_PrintError(MSG_FILE_DIR_CANT_CREATE_s, path);
            /* if only share name is specified */
            return RESULT_ERR;
        }
        *start = '\\';
        /* skip sequential slashes */
        while ( '\\'== *start) start++;
    }
    else if ((toupper(p[0])-'A')<26 && p[1]==':')
    {
        start = &p[2];
        if (_chdrive(toupper(p[0])-'A'+1)==-1)
        {
            MSG_PrintError(MSG_FILE_DIR_CANT_CREATE_s, path);
            return RESULT_ERR;
        };
        if (*start == '\\') 
        {
            start++;
            chdir("\\");
        }
    }
    t=&p[i-1]; pos = t+2;

    /* find last directory which exists to workaround POSIX */
    while (-1 == chdir(p))
    {
        *pos='\\';
        /* look backward for previous directory name */
        while ('\\' != *t && t!=start) t--;
        if (t==start) break; /* we found the start of name */
        pos = t;
        *t = 0;
        t--;
    }

    *pos='\\';

    if (t==&p[i-1])
    {
        _chdrive( curd);
        chdir( currentdir );
        return RESULT_OK;
    }

    if ('\\' == t[1]) start = t+2;

    /* ASSERTION: start directory is really exists */
    do
    {
        t = strchr( start, '\\' );
        if ( t )
            *t = 0;

        if ( -1 == chdir( start ) )
        {
            if (-1 == mkdir( start ) && errno!=EEXIST)
            {
                MSG_PrintError(MSG_FILE_DIR_CANT_CREATE_s, path);
                _chdrive( curd);
                chdir( currentdir );
                return RESULT_ERR;
            }
            /* try to enter a directory*/
            if (-1 == chdir( start ))
            {
                MSG_PrintError(MSG_FILE_DIR_CANT_CREATE_s, path);
                _chdrive( curd);
                chdir( currentdir );
                return RESULT_ERR;
            }
        }

        if ( t )
        {
            *t = '\\';
            start = t + 1;
            /* skip sequential slasses */
            while ('\\' == *start) start++;
        }
    }
    while ( t );


    /* restore previous directory */
    _chdrive( curd);
    chdir( currentdir );

    return RESULT_OK;
#else  /* next part is for non WINDOWS OS */
    char               p[PATH_MAX], currentdir[PATH_MAX];
    char              *start, *t, *pos;
    size_t             i;

    if (!path) return RESULT_ERR;

    i = strlen(path);
    if (i>=PATH_MAX) 
    {
        MSG_PrintError(MSG_CMD_LONG_NAME_s, path);
        return RESULT_ERR;
    }
    strcpy(currentdir, path);

    if (!realpath(currentdir, p))
    {
        strcpy(p, currentdir);
        FP_NormalizePath(p);
    }
    else
        i = strlen(p);
    
    if (p[i-1]=='/') p[--i]=0;
    if (!p[0]) return RESULT_OK;

    if (!getcwd(currentdir, PATH_MAX))
    {
        MSG_PrintFileError(path);
        return RESULT_ERR;
    }
    start=&p[0];
    t=&p[i-1]; pos = t+2;

    /* find last directory which exists to workaround POSIX */
    while (-1 == chdir(p))
    {
        *pos='/';
        /* look backward for previous directory name */
        while (*t!='/' && t!=start) t--;
        if (t==start) break; /* we found the start of name */
        pos = t;
        *t = 0;
        t--;
    }

    *pos='/';

    if (t==&p[i-1])
    {
        /* nothing to create */
        chdir( currentdir );
        return RESULT_OK;
    }

    if ('/' == t[1]) start = t+2;

    do
    {
        t = strchr( start, '/' );
        if ( t )
            *t = 0;

        if ( -1 == chdir( start ) )
        {
            if ( -1 == mkdir(start, 0777 ) && errno!=EEXIST)
            {
                chdir( currentdir );
                return RESULT_ERR;
            }
            if (-1 == chdir( start ))
            {
                MSG_PrintError(MSG_FILE_DIR_CANT_CREATE_s, path);
                chdir( currentdir );
                return RESULT_ERR;
            }
        }

        if ( t )
        {
            *t = '/';
            start = t + 1;
            /* skip sequential slasses */
            while ('/' == start) start++;
        }
    }
    while ( t );


    /* restore previous directory */
    chdir( currentdir );
    return RESULT_OK;
    
#endif
}

/***************************************************************************
 * Function : FP_OpenFile
 *
 * Description: open file, check for error and allocate large in/out buffer
 *
 * Returns: file handle
 *
 * Notes:
 *
 **************************************************************************/
FILE                   *FP_OpenFile( const char * s, const char * mode )
{
    FILE                   *x;
    char                    fpath[TAT_MAX_FNAME_LEN];

    if ( !s || !mode || ( s[0] == 0 ) || ( mode[0] == 0 ) )
        return NULL;

/* if file is opened for write, we can create directories */
    if ( mode[0] == 'w' )
    {
        FP_SplitPath( s, fpath, NULL, NULL );

        if ( !FP_MakeDir( fpath ) )
            return NULL;
    }

/* open/create file */
    x = fopen( s, mode );

/* if error, print file related error message */
    if ( x == NULL )
    {
        MSG_PrintFileError( s );
    }
    else
    {
    /* if it is not console, allocate large buffer */

        if ( stricmp( s, tat_ConsoleName ) && !TAT_DEBUG_FILE_IO )
            setvbuf( x, NULL, _IOFBF, 65536 );
        else
            setvbuf( x, NULL, _IONBF, 0 );
    }

    return x;
}

/***************************************************************************
 * Function : FP_FileSize
 *
 * Description: return size of opened file
 *
 * Returns: size/0 if null
 *
 * Notes: None
 *
 **************************************************************************/
UINT32_T FP_FileSize( FILE * f )
{
    UINT32_T                    pos, size;

    if ( !f )
        return 0;

    pos = ftell( f );
    fseek( f, 0, SEEK_END );
    size = ftell( f );
    fseek( f, pos, SEEK_SET );

    return size;
}

/***************************************************************************
 * Function : FP_AddFileName
 *
 * Description: Add unique filename to array
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL FP_AddFileName(const char * name, size_t *count, char ***arr)
{
    size_t i;
    char buffer[PATH_MAX];

    if (!name || !count || !arr) 
        return RESULT_ERR;

    strncpy(buffer, name, PATH_MAX);  
    buffer[PATH_MAX-1]=0;
    FP_NormalizePath(buffer);

    /* check for duplicated names */
    for (i=0; i<*count; i++)
    {
        if (!FILE_NAME_COMPARE(buffer, ( *arr )[i]))
            return RESULT_OK;
    }

    *arr = MEM_Reallocate( *arr, ( (*count) + 1 )* sizeof ( ( *arr )[0] ) );
    ( *arr )[*count] = MEM_CopyStr( buffer );
    ( *count )++;

    return RESULT_OK;
        
}

/***************************************************************************
 * Function : FP_ExpandMask
 *
 * Description: Expand file mask into set of files
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
#if (COMPILER==MSVC) && (OSTYPE==WIN32)
LOGICAL FP_ExpandMask( const char * path, size_t * count, char * ** arr )
{

    struct _finddata_t      fileinfo;
    long                    flag;
    char                    dpath[TAT_MAX_FNAME_LEN], name[TAT_MAX_FNAME_LEN];

    if ( !path || !arr )
        return RESULT_ERR;

    /* if no file mask is specified */
    if (!strchr( path, '*' ) && !strchr( path, '?' ) )
    {
        return FP_AddFileName(path, count, arr);
    }

    FP_SplitPath( path, dpath, NULL, NULL );

    flag = _findfirst( path, &fileinfo );

    if ( flag != -1 )
    {
        do
        {
            if (!(fileinfo.attrib & _A_SUBDIR))
            {
                strcpy( name, dpath );
                strcat( name, fileinfo.name );
                FP_AddFileName(name, count, arr);
            }
        }
        while ( _findnext( flag, &fileinfo ) == 0 );

        _findclose( flag );
    }
    else
    {
        if ( errno != ENOENT )
        {
            MSG_PrintFileError( path );
            return RESULT_ERR;
        }
        else
        {
            if ( strchr( path, '*' ) || strchr( path, '?' ) )
                MSG_PrintError( MSG_FILE_NO_MATCH_s, path );
        }
    };

    return RESULT_OK;
}

#else /* next part is for non WINDOWS OSes */
LOGICAL FP_ExpandMask( const char * path, size_t * count, char * ** arr )
{
    DIR                    *dirp;

    struct dirent          *dp;
    struct stat            buf;
    
    char                    dpath[TAT_MAX_FNAME_LEN];
    char                    fmask[TAT_MAX_FNAME_LEN], ext[TAT_MAX_FNAME_LEN];
    size_t                  fcount = 0;

    if ( !path || !arr )
        return RESULT_ERR;

    /* if no file mask is specified */
    if (!strchr( path, '*' ) && !strchr( path, '?' ) )
    {
        return FP_AddFileName(path, count, arr);
    }

    FP_SplitPath( path, dpath, fmask, ext );

    strcat( fmask, ext );

/* if no path is specified, use current */
    if ( dpath[0] == 0 )
    {
        dpath[0] = '.';
        dpath[1] = '/';
        dpath[2] = 0;
    };

    FP_NormalizePath( dpath );

    dirp = opendir( dpath );

    if ( dirp )
    {
        errno = 0;

        while ( ( dp = readdir( dirp ) ) != NULL )
        {
            if ( StrMatch( dp->d_name, fmask ) )
            {
                strcpy( ext, dpath );
                strcat( ext, dp->d_name );
                if ( stat(ext, &buf)!=-1 && !(buf.st_mode & S_IFDIR) )
                {
                    FP_AddFileName(ext, count, arr);
                    fcount++;
                }
            }
        }

        if ( errno == 0 )
        {
            if ( fcount == 0 )
            {
                if ( strchr( path, '*' ) || strchr( path, '?' ) )
                    MSG_PrintError( MSG_FILE_NO_MATCH_s, path );
            }

            closedir( dirp );
            return RESULT_OK;
        }
        else
        {
            MSG_PrintFileError( path );
            closedir( dirp );
        }
    }
    else
    {
        MSG_PrintFileError( path );
    }

    return RESULT_ERR;
}

#endif


/***************************************************************************
 * Function : FP_FullName
 *
 * Description: expand relative path to absolute one
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL FP_FullName( char * dest, const char * src )
{
#if (OSTYPE!=WIN32)  || (COMPILER!=MSVC)
    size_t                     i;
    char                       *home;

#endif
    if ( !dest )
        return RESULT_ERR;
    *dest = 0;

    if ( !src )
    {
        getcwd( dest, PATH_MAX );
        return RESULT_OK;
    }

#if (OSTYPE==WIN32) && (COMPILER==MSVC)
    if (!_fullpath(dest, src, PATH_MAX))
        return RESULT_ERR;
    return RESULT_OK;
#else
    /*
     The realpath() function derives, from the  pathname  pointed
     to  by  file_name,  an absolute pathname that names the same
     file, whose resolution does not involve ".", "..",  or  sym-
     bolic links.  The generated pathname is stored, up to a max-
     imum  of  PATH_MAX  bytes,  in  the  buffer  pointed  to  by
     resolved_name.

     The realpath() function can handle both relative  and  abso-
     lute  path  names.  For absolute path names and the relative
     names whose resolved name  cannot  be  expressed  relatively
     (for  example,  ../../reldir), it returns the resolved abso-
     lute name. For the other relative path names, it returns the
     resolved relative name.

     RETURN VALUES
     On successful completion, realpath() returns  a  pointer  to
     the  resolved  name.   Otherwise,  realpath() returns a null
     pointer and sets errno to indicate the error, and  the  con-
     tents  of  the  buffer pointed to by resolved_name are unde-
     fined.
     */

    if (src[0]=='~')
    {
        /* check for explicit "~" */
        if (src[1]=='/' || src[1]==0)
        {
            home = getenv("HOME");
            dest[0] = 0;
            if (home)
            {
                strcpy(dest, home);
            }
            strcat(dest, &src[1]);
            return RESULT_OK;
        }
        else
        {
            strcpy(dest, src);
            return RESULT_OK;
        }
    }
#if OSTYPE==WIN32
    cygwin_conv_to_full_win32_path(src,dest);
    FP_NormalizePath(dest);
    return RESULT_OK;
#else
    if (realpath(src, dest))
    {
        return RESULT_OK;
    }
    else
    {
        /* if path is relative, add current dir before */
        if (src[0]!='/')
        {
            getcwd( dest, PATH_MAX );
            strcat( dest, "/");
            strcat( dest, src);
            return RESULT_OK;
        }
        else
        {
            strcpy(dest, src);
            return RESULT_OK;
        }
    }
#endif
    return RESULT_ERR;
#endif

}

/***************************************************************************
 * Function : FP_FileExist
 *
 * Description: check file for existence
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL FP_FileExist(const char * filename)
{
    FILE * fp;
    fp = fopen(filename, "r");
    if (!fp) {
        if (EACCES==errno || EPERM==errno)
            return RESULT_OK;
        return RESULT_ERR;
    }
    fclose(fp);
    return RESULT_OK;
}

/***************************************************************************
 * Function : FP_FileSearch
 *
 * Description: search file for existence in specified directories
 * with specified extensions.
 * If pathcount==0, search will be conducted in the initial directory
 * If extcount==0, search will be conducted with initial extension
 *
 * Returns: RESULT_ERR/RESULT_OK
 *
 * Notes: None
 *
 **************************************************************************/

LOGICAL FP_FileSearch(size_t *resCount, char *** result, const char * filename, size_t pathcount, char * paths[],
                      size_t extcount, char * exts[])
{
    char dirname[PATH_MAX], fname[PATH_MAX], ext[PATH_MAX], newname[PATH_MAX], fullname[PATH_MAX];
    size_t i, j;

    if (!resCount || !result || !filename ||
        (pathcount && !paths) || (extcount && !exts))
        return RESULT_ERR;

    *resCount = 0;
    *result = NULL;

    FP_SplitPath(filename, dirname, fname, ext);
    /* at first, search using specified data */
    if ((ext[0]!=0 || extcount==0) && FP_FileExist(filename)) {
        FP_FullName(fullname, filename);
        FP_AddFileName(fullname, resCount, result);
    }

    /* search in initial directory  */
    for (i=0; i<extcount; i++)
    {
        strcpy(newname, dirname);
        strcat(newname, fname);
        strcat(newname, exts[i]);
        if (FP_FileExist(newname)) {
            FP_FullName(fullname, newname);
            FP_AddFileName(fullname, resCount, result);
        }
    }
    
    for (i=0; i<pathcount; i++)
    {
        if (ext[0]!=0) {
        strcpy(newname, paths[i]);
        FP_NormalizePath(newname);
        strcat(newname, fname);
        strcat(newname, ext);
        if (FP_FileExist(newname)) {
            FP_FullName(fullname, newname);
            FP_AddFileName(fullname, resCount, result);
        }
        }
        for (j=0; j<extcount; j++)
        {
            strcpy(newname, paths[i]);
            FP_NormalizePath(newname);
            strcat(newname, fname);
            strcat(newname, exts[j]);
            if (FP_FileExist(newname)) {
                FP_FullName(fullname, newname);
                FP_AddFileName(fullname, resCount, result);
            }
        }
    }

    /* not found */
    return (!resCount || *resCount==0)? RESULT_ERR: RESULT_OK;
}

/***************************************************************************
 * Function : FP_GetFileMtime
 *
 * Description: get file modification time
 *
 * Returns: time / 0 
 *
 * Notes: None
 *
 **************************************************************************/
time_t FP_GetFileMtime(const char * fname)
{
    struct stat buff;
    time_t timevalue;
    if (-1 == stat(fname, &buff))
    {
        memset(&timevalue, 0, sizeof(timevalue));
    }
    else
        timevalue=buff.st_mtime;
    return timevalue;
}


/***************************************************************************
 * Function : FP_AddDirectorySlash
 *
 * Description: add trailing slash to directory name.
 *              buffer shall have enough space.
 *
 * Returns: pointer to dirname
 *
 * Notes: None
 *
 **************************************************************************/
char * FP_AddDirectorySlash(char * dirname)
{
    size_t length;
    if (!dirname)
        return dirname;
    length=strlen(dirname);
    if (!length)
        return dirname;

    /* if there is slash already, do nothing */
    if (dirname[length-1]==NORMAL_FILE_SLASH || dirname[length-1]==OTHER_FILE_SLASH)
        return dirname;
    
    dirname[length]=NORMAL_FILE_SLASH;
    dirname[length+1]=0;
    return dirname;
}

/***************************************************************************
 * Function : FP_IsWriteable
 *
 * Description: check write permissions for file
 *
 * Returns: RESULT_OK - file is writeable
 *
 * Notes: None
 *
 **************************************************************************/
LOGICAL FP_IsWriteable(const char *filename)
{
    if (access(filename, R_OK | W_OK) != -1)
        return RESULT_OK;
    return RESULT_ERR;
}
