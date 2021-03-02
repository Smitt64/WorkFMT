/*-----------------------------------------------------------------------*-

 File Name   : launch.h  

 Sybsystem   : TOOL  
 Description : Programm launcher

 Source      : launch.c

 Library(s)  : 


 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 April 18,1995  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifndef __LAUNCH_H
#define __LAUNCH_H

#include "tooldef.h"

typedef struct
      {
       char  *command;     
       int    status;       // Program exit code
      } TLaunchInfo;


// Launch flags
#define  LAUNCH_NEW_CON   1
#define  LAUNCH_WAIT      2
#define  LAUNCH_PIPE      4
#define  LAUNCH_DETACHED  8


                                
_TOOLEXP void InitLaunch(void);
_TOOLEXP void DoneLaunch(void);

_TOOLEXP void         RemoveLaunchedProg(TLaunchInfo *info);
_TOOLEXP TLaunchInfo *GetFinishedProg   (void);
_TOOLEXP int          LaunchError       (void); 
_TOOLEXP int          LoopRunProg       (int (*UserProc)(TLaunchInfo *elem, void *data), void *data);
_TOOLEXP int          GetNumLaunch      (void);
                       

_TOOLEXP int LaunchProg(
                        char *prog,           /*  Запускаемая программа */
                        char *com,            /*  Командная строка      */
                        int   launchFlags,    
                        char *curDir,         /*  May be NULL           */      
                        char *title           /*  Window header         */
                       );

#endif

/* <-- EOF --> */