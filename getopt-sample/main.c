/* -*- Mode: C -*-
 * --------------------------------------------------------------------------
 * Copyright  (c) Dipl.-Ing. Joerg Desch <github@jdesch.de>
 * --------------------------------------------------------------------------
 * PROJECT: getopt-sample
 * MODULE.: main.c
 * AUTHOR.: Dipl.-Ing. Joerg Desch
 * CREATED: 25.03.2019 10:33:26 CET
 * LICENSE: GPL 3.0
 * --------------------------------------------------------------------------
 * DESCRIPTION:
 *
 * --------------------------------------------------------------------------
 * COMPILER-FLAGS:
 *
 * --------------------------------------------------------------------------
 */

#include <stdio.h>                       // printf(), ...
#include <stdlib.h>                      // atoi()
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>                      // strncpy(), ...
#include <unistd.h>                      // getopt()

#define __MAIN_C__
#include "config.h"
#include "filesystem_tools.h"


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

typedef struct tagSYSTEM_CONFIGURATION
{
    bool Debugging;
    int Height;
    char VideoDevice[MAXPATH+1];
} T_SystemConfiguration;

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

static T_SystemConfiguration SysConfig;

//}}}

/*+=========================================================================+*/
/*|                      PROTOTYPES OF LOCAL FUNCTIONS                      |*/
/*`========================================================================='*/
//{{{

static void enableDefaultArguments ( void );
static bool parseArguments ( int argc, char *argv[] );
static void usage ( void );

//}}}

/*+=========================================================================+*/
/*|                     IMPLEMENTATION OF THE FUNCTIONS                     |*/
/*`========================================================================='*/
//{{{

int main( int argc, char *argv[] )
{
    int rc;

    printf("getopt-sample %s -- example usage of getopt()\ncompiled: "__DATE__"\n\n",VERSION);
    enableDefaultArguments();
    if ( !parseArguments(argc,argv) )
        return 2;

    printf("SysConfig.Debugging = %s\n",SysConfig.Debugging?"enabled":"disabled");
    printf("SysConfig.Height = %d\n",SysConfig.Height);
    printf("SysConfig.VideoDevice %s\n",SysConfig.VideoDevice);

    return 0;
}


//}}}

/*             .-----------------------------------------------.             */
/* ___________/  Group...                                       \___________ */
/*            `-------------------------------------------------'            */
//{{{
//}}}

/*+=========================================================================+*/
/*|                    IMPLEMENTATION OF LOCAL FUNCTIONS                    |*/
/*`========================================================================='*/
//{{{

static void enableDefaultArguments (void)
{
    SysConfig.Debugging = false;
    SysConfig.Height = 1080;
    SysConfig.VideoDevice[0] = '\0';                        // means no device set!
}

static bool parseArguments ( int argc, char *argv[] )
{
    int Done = 0;
    optind = 1;   /* start without prog-name */
    do
    {
        switch ( getopt(argc, argv, "v:l:h:d?") )
        {
            case 'v':
                if ( optarg )
                {
                    if ( is_special_file(optarg,FST_IS_DEVICE)==1 )
                    {
                        // copy only if valid!
                        strncpy(SysConfig.VideoDevice, optarg, MAXPATH);
                        SysConfig.VideoDevice[MAXPATH] = '\0';
                        __INFO("video device: %s",SysConfig.VideoDevice);
                    }
                    else
                    {
                        __ERROR("%s is no device!",optarg);
                        return false;
                    }
                }
                else
                {
                    __ERROR("missing parameter for -v!");
                    return false;
                }
                break;
            case 'l':
                if ( optarg )
                {
                    __INFO("sample link passed!");
                    if ( is_special_file(optarg,FST_IS_LINK)==1 )
                    {
                        // just tested but not used
                        __INFO("%s is a link!",optarg);
                    }
                    else
                    {
                        __ERROR("%s is not a link!",optarg);
                        return false;
                    }
                }
                else
                {
                    fputs("error: missing parameter for -s", stderr);
                    return false;
                }
                break;
            case 'h':
                if ( optarg )
                {
                    int h = atoi(optarg);
                    if ( h==0 && *optarg!='0' )
                    {
                        __WARN("invalid height parm ignored!");
                    }
                    else
                    {
                        SysConfig.Height=h;
                        __INFO("height is set to %d",SysConfig.Height);
                    }
                }
                break;
            case 'd':
                SysConfig.Debugging=true;
                __INFO("debugging is enabled!");
                break;
            case '?':     // getopt3() reports invalid option
                usage();
                return false;
            default:
                Done = 1;
        }
    } while (!Done);
    for ( int i=optind; i<argc; i++ )
    {
        printf("remaining 'none option' parameter found: %s\n",argv[i]);
    }
    return true;
}

static void usage ( void )
{
    fprintf(stderr, "SYNOPSIS\n");
    fprintf(stderr, "\tgetopt-sample [options] [parms]\n");
    fprintf(stderr, "\nDESCRIPTION\n");
    fprintf(stderr, "\tThis program is just a sample.\n");
    fprintf(stderr, "\nOPTIONS\n");
    fprintf(stderr, "\t-h\tvalue for a height.\n");
    fprintf(stderr, "\t-v dev\tpath to a device.\n");
    fprintf(stderr, "\t-l link\tpath to a link.\n");
    fprintf(stderr, "\t-d\tenable debugging.\n");
}


//}}}

/* ==[End of file]========================================================== */
