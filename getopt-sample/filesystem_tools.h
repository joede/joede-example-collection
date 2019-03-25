/* -*- Mode: C -*-
 * --------------------------------------------------------------------------
 * Copyright  (c) Dipl.-Ing. Joerg Desch <github@jdesch.de>
 * --------------------------------------------------------------------------
 * PROJECT: **common POSIX helpers**
 * MODULE.: filesystem_tools.h
 * AUTHOR.: Dipl.-Ing. Joerg Desch
 * CREATED: 21.06.2011 07:35:06 CEST
 * LICENSE: GPL 3.0
 * --------------------------------------------------------------------------
 * DESCRIPTION:
 *
 *
 *
 * --------------------------------------------------------------------------
 * HISTORY:
 *
 * R3 (jd/25.3.2019)
 * make reporting module optional. The compiler flag WNAT_REPORTING enables
 * support/usage.
 *
 * R2 (jd/10.2.2017)
 * add function ensure_full_path() inspiered by mkpath
 *
 * R1 (jd)
 * first revision with history
 *
 * --------------------------------------------------------------------------
 * COMPILER-FLAGS:
 *
 *
 * --------------------------------------------------------------------------
 */

#ifndef __FILESYSTEM_TOOLS_H__
#define __FILESYSTEM_TOOLS_H__ 1

#include <stdbool.h>


/*+=========================================================================+*/
/*|                      CONSTANT AND MACRO DEFINITIONS                     |*/
/*`========================================================================='*/
//{{{

#ifndef MAXPATH
#define MAXPATH 200
#endif

//}}}

/*             .-----------------------------------------------.             */
/* ___________/  macro declaration                              \___________ */
/*            `-------------------------------------------------'            */
//{{{
//}}}

/*+=========================================================================+*/
/*|                            TYPEDECLARATIONS                             |*/
/*`========================================================================='*/
//{{{

enum tagFST_SPECIAL_FILE_TYPES
{
    FST_IS_DEVICE=0,
    FST_IS_FIFO,
    FST_IS_LINK,
    FST_IS_SOCKET,
    __MAX_FST_SPECIAL_FILE_TYPES
};

//}}}

/*+=========================================================================+*/
/*|                            PUBLIC VARIABLES                             |*/
/*`========================================================================='*/
//{{{
//}}}

/*+=========================================================================+*/
/*|                     PROTOTYPES OF GLOBAL FUNCTIONS                      |*/
/*`========================================================================='*/
//{{{

int copy_file ( const char* src, const char* dst );
int is_directory ( const char* dir_path );
int ensure_dir ( const char* dir_path );
int ensure_full_path ( const char *full_path );
int is_file ( const char* file_path );
int is_special_file ( const char* file_path, int type );
unsigned long file_size ( const char* file_path );
int free_disk_space ( const char* mount_point, unsigned long *avail, unsigned long *blk_size );

#ifdef WANT_ALIX_FS_COMMANDS
bool remount_root_rw (void);
bool remount_root_ro (void);
#endif

//}}}

#endif
/* ==[End of file]========================================================== */
