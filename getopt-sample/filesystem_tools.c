/* -*- Mode: C -*-
 * --------------------------------------------------------------------------
 * Copyright  (c) Dipl.-Ing. Joerg Desch <github@jdesch.de>
 * --------------------------------------------------------------------------
 * PROJECT: **common POSIX helpers**
 * MODULE.: filesystem_tools.c
 * AUTHOR.: Dipl.-Ing. Joerg Desch
 * CREATED: 21.06.2011 07:34:12 CEST
 * LICENSE: GPL 3.0
 * --------------------------------------------------------------------------
 * DESCRIPTION:
 *
 * --------------------------------------------------------------------------
 * COMPILER-FLAGS:
 *
 * --------------------------------------------------------------------------
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>                   /* stat() */
#include <sys/statvfs.h>
#include <dirent.h>




#define __FILESYSTEM_TOOLS_C__

#include "config.h"
#include "filesystem_tools.h"
#ifdef WANT_REPORTING
#  include "reporting.h"
#endif

#ifdef WANT_ALIX_FS_COMMANDS
#  include <sys/mount.h>
#endif


/*+=========================================================================+*/
/*|                      CONSTANT AND MACRO DEFINITIONS                     |*/
/*`========================================================================='*/
//{{{
//}}}

/*             .-----------------------------------------------.             */
/* ___________/  local macro declaration                        \___________ */
/*            `-------------------------------------------------'            */
//{{{

#ifndef WANT_REPORTING
#define __CRITICAL(msg,args...)                do{printf("critical: ");printf(msg,##args);printf("\n");}while(0)
#define __ERROR(msg,args...)                   do{printf("error: ");printf(msg,##args);printf("\n");}while(0)
#define __WARN(msg,args...)                    do{printf("warning: ");printf(msg,##args);printf("\n");}while(0)
#define __NOTICE(msg,args...)                  do{printf("notice: ");printf(msg,##args);printf("\n");}while(0)
#define __INFO(msg,args...)                    do{printf("info: ");printf(msg,##args);printf("\n");}while(0)
#define __DIAGNOSE(msg,args...)                do{printf("diag: ");printf(msg,##args);printf("\n");}while(0)
#define __TRACE(msg,args...)                   do{printf("trace: ");printf(msg,##args);printf("\n");}while(0)
#define __ASSERT(cond,msg,args...)             do{if(cond){printf("critical: ");printf(msg,##args);printf("\n");return;}}while(0)
#define __ASSERT2(cond,msg,retval)             do{if(cond){printf("critical: ");printf(msg,##args);printf("\n");return(retval);}}while(0)
#if defined(BETA) || defined(DEBUG)
# define __DEBUG(msg,args...)                  do{printf("debug: ");printf(msg,##args);printf("\n");}while(0)
# define __DBGASSERT(cond,msg,args...)         do{if(cond){printf("critical: ");printf(msg,##args);printf("\n");return;}}while(0)
# define __DBGASSERT2(cond,retval,msg,args...) do{if(cond){printf("critical: ");printf(msg,##args);printf("\n");return(retval);}}while(0)
#else
# define __DEBUG(msg,args...)
# define __DBGASSERT(cond,msg,args...)
# define __DBGASSERT2(cond,retval,msg,args...)
#endif
#endif

//}}}

/*+=========================================================================+*/
/*|                          LOCAL TYPEDECLARATIONS                         |*/
/*`========================================================================='*/
//{{{
//}}}

/*+=========================================================================+*/
/*|                            PUBLIC VARIABLES                             |*/
/*`========================================================================='*/
//{{{
//}}}

/*+=========================================================================+*/
/*|                             LOCAL VARIABLES                             |*/
/*`========================================================================='*/
//{{{
//}}}

/*+=========================================================================+*/
/*|                      PROTOTYPES OF LOCAL FUNCTIONS                      |*/
/*`========================================================================='*/
//{{{


//}}}

/*+=========================================================================+*/
/*|                     IMPLEMENTATION OF THE FUNCTIONS                     |*/
/*`========================================================================='*/
//{{{

/* Copy the file \c src into the file \c dst. If the destination already
 * exists, the old destination is removed first.
 *
 * return values:
 *     1   copy process done
 *     0   no source file
 *    -1   NULL pointer parameter
 *    -2   failed to remove existing destination
 *    -3   IO error
 */
int copy_file ( const char* src, const char* dst )
{
    FILE *from, *to;
    char ch;
    int rc;

    if ( !src || !dst )
    {
        __CRITICAL("copy_file: NULL pointer");
        return -1;
    }
    if ( is_file(src)<=0 )
    {
        __INFO("copy_file: no src `%s'",src);
        return 0;
    }
    if ( is_file(dst)==1 )
    {
        __INFO("copy_file: unlink dst `%s'",dst);
        if ( unlink(dst)<0 )
        {
            __ERROR("copy_file: unlink failed!");
            return -2;
        }
    }

    if ( (from=fopen(src,"rb"))==NULL)
    {
        __ERROR("copy_file: open src failed!");
        return -3;
    }
    if ( (to=fopen(dst,"wb"))==NULL )
    {
        __ERROR("copy_file: open dst failed!");
        fclose(from);
        return -3;
    }
    rc = 1;
    while ( !feof(from) )
    {
        ch = fgetc(from);
        if ( ferror(from) )
        {
            __ERROR("copy_file: read(src) failed!");
            rc = -3;
            break;
        }
        if ( !feof(from) )
            fputc(ch, to);
        if ( ferror(to) )
        {
            __ERROR("copy_file: write(ds) failed!");
            rc = -3;
            break;
        }
    }
    /* allways try to close both files.
     */
    if ( fclose(from)==EOF )
    {
        __ERROR("copy_file: close src failed!");
        rc = -3;
    }
    if ( fclose(to)==EOF )
    {
        __ERROR("copy_file: close dst failed!");
        rc = -3;
    }
    return rc;
}

/* Checks \c dir_path as directory.
 *
 * returns (1) if true, (0) if it doesn't exist, (-1) if it isn't a directory,
 *        (-2) if an error occured.
 */
int is_directory ( const char* dir_path )
{
    struct stat attribut;

    if ( !dir_path )
    {
        __CRITICAL("is_directory: NULL pointer");
        return -2;
    }
    if ( stat(dir_path,&attribut) == -1 )
    {
        if ( errno==ENOENT )
            __DEBUG("is_directory: '%s' does not exist!",dir_path);
        else
            __WARN("is_directory: stat failed.");
        return errno==ENOENT ? 0 : -1;
    }
    if ( S_ISDIR(attribut.st_mode) )
    {
        return 1;
    }
    else
    {
        __NOTICE("is_directory: '%s' exists, but is not a directory!",dir_path);
        return -1;
    }
}

/* This function tries to ensure that the directory \c dir_path exists. If
 * there is no such directory entry, it tries to create the directory.
 *
 * returns (1) if the directory exists, and (<0) if an error occured.
 */
int ensure_dir ( const char* dir_path )
{
    struct stat attribut;

    if ( !dir_path )
    {
        __CRITICAL("ensure_dir: NULL pointer");
        return -2;
    }
    if ( stat(dir_path,&attribut) == -1 )
    {
        if ( errno==ENOENT )
        {
            if ( mkdir(dir_path,0600)==-1 && errno!=EEXIST )
            {
                __ERROR("ensure_dir: mkdir() failed.");
            }
            else
            {
                if ( errno!=EEXIST )
                    __DEBUG("ensure_dir: `%s' created.",dir_path);
                return 1;
            }
        }
        else
        {
            __WARN("ensure_dir: stat failed.");
        }
        return -1;
    }
    if ( S_ISDIR(attribut.st_mode) )
    {
        __DEBUG("ensure_dir: `%s' exists.",dir_path);
        return 1;
    }
    else
    {
        __NOTICE("ensure_dir: '%s' exists, but is not a directory!",dir_path);
        return -1;
    }
}

/* Ensure a complete path of directories is available. All directories in
 * \c path are checked and created if needed.
 *
 * Code taken directly from mkdir(1). Sadly, the code is hard to read...
 *
 * On success, zero is returned. On error, -1 is returned.
 */
int ensure_full_path ( const char *full_path )
{
    struct stat sb;
    char *path = NULL;
    char *slash;
    int done = 0;

    path = strdup(full_path);
    if ( !path )
    {
        __ERROR("ensure_full_path: malloc failed");
        return (-1);
    }
    slash = path;
    while ( !done )
    {
        slash += strspn(slash,"/");
        slash += strcspn(slash,"/");
        done = (*slash == '\0');
        *slash = '\0';
        if ( stat(path,&sb) == -1 )
        {
            if ( errno!=ENOENT || (mkdir(path,0777) && errno!=EEXIST) )
            {
                __ERROR("ensure_full_path: failed %s",path);
                return (-1);
            }
        }
        else if ( !S_ISDIR(sb.st_mode) )
        {
            __ERROR("ensure_full_path: %s: %s",path,strerror(ENOTDIR));
            return (-1);
        }

        *slash = '/';
    }
    return (0);
}


/* Checks if \c file_path is a existing file.
 *
 * returns (1) if it is a existing file, (0) if there no such entry, (-1) if it
 * is not a regular file, (-2) is a case of an error.
 */
int is_file ( const char* file_path )
{
    struct stat attribut;

    if ( !file_path )
    {
        __CRITICAL("is_file: NULL pointer");
        return -2;
    }
    if ( stat(file_path,&attribut) == -1 )
    {
        __DEBUG("is_file: stat failed. '%s' does not exist!",file_path);
        return 0;
    }
    if ( S_ISREG(attribut.st_mode) )
    {
        return 1;
    }
    else
    {
        __NOTICE("is_file: '%s' exists, but is not a reg. file!",file_path);
        return -1;
    }
}


/* Checks if \c file_path is a existing special file. For FST_IS_DEVICE, the
 * return value distigueses between char device (1) and block device (2).
 *
 * returns 1 or 2 if it is a special file, (0) if there no such entry, (-1) if
 * type doesn't match, (-2) is a case of an error.
 */
int is_special_file ( const char* file_path, int type )
{
    struct stat attribut;
    struct stat attribut2;

    if ( !file_path )
    {
        __CRITICAL("is_special_file: NULL pointer");
        return -2;
    }
    if ( stat(file_path,&attribut) == -1 )
    {
        __DEBUG("is_special_file: stat failed. '%s' does not exist!",file_path);
        return 0;
    }
    if ( lstat(file_path,&attribut2) == -1 )
    {
        __DEBUG("is_special_file: lstat failed. '%s' does not exist!",file_path);
        return 0;
    }
    switch ( type)
    {
        case FST_IS_DEVICE:
            if ( S_ISCHR(attribut.st_mode) )
                return 1;
            else if ( S_ISBLK(attribut.st_mode) )
                return 2;
            break;
        case FST_IS_FIFO:
            if ( S_ISFIFO(attribut.st_mode) )
                return 1;
            break;
        case FST_IS_LINK:
            if ( S_ISLNK(attribut2.st_mode) )
                return 1;
            break;
        case FST_IS_SOCKET:
            if ( S_ISSOCK(attribut.st_mode) )
                return 1;
            break;
        default:
            __NOTICE("is_special_file: invalid type parm!");
            return -2;
    }
    __NOTICE("is_special_file: '%s' exists, but is of different type!",file_path);
    switch (attribut.st_mode & S_IFMT) {
        case S_IFBLK:  __NOTICE("  it is a block device");     break;
        case S_IFCHR:  __NOTICE("  it is a character device"); break;
        case S_IFDIR:  __NOTICE("  it is a directory");        break;
        case S_IFIFO:  __NOTICE("  it is a FIFO/pipe");        break;
        case S_IFLNK:  __NOTICE("  it is a symlink");          break;
        case S_IFREG:  __NOTICE("  it is a regular file");     break;
        case S_IFSOCK: __NOTICE("  it is a socket");           break;
        default:       __NOTICE("  it is unknown..");          break;
    }
    return -1;
}

unsigned long file_size ( const char* file_path )
{
    struct stat attribut;

    if ( !file_path )
    {
        __CRITICAL("file_size: NULL pointer");
        return 0;
    }
    if ( stat(file_path,&attribut) == -1 )
    {
        __DEBUG("file_size: stat failed. '%s' does not exist!",file_path);
        return 0;
    }
#ifdef DEBUG
    // in case of a link, stat seems to report the length of the stored path of the
    // link instead of the size of the linked file. :-(
    if ( S_ISLNK(attribut.st_mode) )
        __WARNING("file_size: '%s' is a link!",file_path);
#endif
    return attribut.st_size;
}

/* Determine the free space an a mounted device. \c references the mount point.
 * The parameters \c avail and \blk_size are pointers to variables which stores
 * the number of available blocks (\c avail) and the blocksize (\c blk_size).
 *
 * returns (1) if all is OK, (0) if the filesystem is read only and (<0) in an
 * case of an error.
 */
int free_disk_space ( const char* mount_point,
                      unsigned long *avail,
                      unsigned long *blk_size )
{
    struct statvfs stats;
    char filename[MAXPATH+1];
    int rc;

    if ( !mount_point || !avail )
    {
        __CRITICAL("free_disk_space: NULL pointer.\n");
        return -1;
    }
    if ( mount_point[0]=='\0' )
    {
        __ERROR("free_disk_space: no mount.\n");
        return -1;
    }

    snprintf(filename,MAXPATH,"%s/.",mount_point);
    filename[MAXPATH] = '\0';
    rc = statvfs(filename,&stats);
    if ( rc < 0 )
    {
#ifdef DEBUG
        __ERROR("free_disk_space: statvfs(%s) returns %d (%d)",
                   filename,rc,errno);
#else
        __ERROR("free_disk_space: statvfs() returns %d (%d)",
                   rc,errno);
#endif
        return -1;
    }
/*  ------- FOR SOME REASONS, THIS DON'T WORK ON THE UNC90 ----- */
    if ( stats.f_flag&ST_RDONLY )
    {
        __ERROR("free_disk_space: FS is read-only");
        return 0;
    }

    if ( blk_size )
    {
        *blk_size = (unsigned long)stats.f_bsize;
    }
    *avail = (unsigned long)stats.f_bavail;

    return 1;
}

#ifdef WANT_ALIX_FS_COMMANDS

#define ALIX_DEFAULT_MOUNTOPTS (MS_NOATIME)

bool remount_root_rw (void)
{
    bool result = true;
    int rc;

    rc = mount("","/","",MS_REMOUNT|ALIX_DEFAULT_MOUNTOPTS,"");
    if ( rc != 0 )
    {
        result = false;
        __ERROR("remount_root_rw: remount failed (%d)",errno);
    }
    return result;
}

bool remount_root_ro (void)
{
    bool result = true;
    int rc;
    sync();
    rc = mount("","/","",MS_REMOUNT|MS_RDONLY|ALIX_DEFAULT_MOUNTOPTS,"");
    if ( rc != 0 )
    {
        result = false;
        __ERROR("remount_root_ro: remount failed (%d)",errno);
    }
    return result;
}
#endif


//}}}

/*+=========================================================================+*/
/*|                    IMPLEMENTATION OF LOCAL FUNCTIONS                    |*/
/*`========================================================================='*/
//{{{
//}}}

/* ==[End of file]========================================================== */
