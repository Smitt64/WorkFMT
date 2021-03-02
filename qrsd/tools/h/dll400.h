/*************************************************************************
   File Name      : dll400.h
   Programmer     : Alexey Lebedev

   Creation       : 29 Oct 2001
   Description    : AS/400 support LoadLibrary, FreeLibrary,
                     GetProcAddress and etc.

   Copyright(C) 2002 by R-Style Software Lab.
   All Rights Reserved.

*************************************************************************/


/* ================================================================== */
/*     dll.h - ILE C/400 Library header file              V4R3M0      */
/*                                                                    */
/*  This header contains function definitions that provide the        */
/*  support for Dynamic Load Libraries (DLL).  One the AS/400 a DLL   */
/*  is manifested as a Service Program (*SRVPGM) which can contain    */
/*  one or more C functions.  The functions that are prototuped in    */
/*  in this include are:                                              */
/*    LoadLibrary -- activate a bound service program                 */
/*    GetProcAddress - retrieve the function pointer associated with  */
/*                     the loaded service program                     */
/*    FreeLibrary -- Free up loaded library - partial function        */
/*                                                                    */
/*  Copyright: (c) IBM Corp 1990, 1993, 1999                          */
/* ================================================================== */

#pragma nomargins nosequence
#pragma checkout(suspend)

#ifndef _DLL400_H_
#define _DLL400_H_

#include "systypes.h"

#ifdef __cplusplus
 extern "C" {
#endif

/* ============================================================== */
/*  Function: LoadLibrary                                         */
/*                                                                */
/*  Description: Dynamically load a service program (DLL) by name */
/*               and activate it into the running process.        */
/*  Input Parameters:                                             */
/*     LibName - C null terminated string indicating the name     */
/*               of the service program to be loaded. This parm   */
/*               can be of two forms:                             */
/*        "name"                                                  */
/*        "library/name"                                          */
/*  Output:                                                       */
/*     HMODULE - pointer to a handle to the activation of the     */
/*               service program. This is basically the data      */
/*               returned from ACTBPGM instruction.               */
/* ============================================================== */

HMODULE  LoadLibrary(const char *LibName);



/* ============================================================== */
/*  Function: GetProcAddress                                      */
/*                                                                */
/*  Description: Resolve by name or ordinal number a function in  */
/*               the service program that has been previously     */
/*               loaded by the LoadLibrary function               */
/*  Input Parameters:                                             */
/*     HMODULE - pointer to a handle to the activation of the     */
/*               service program. This is basically the data      */
/*               returned from ACTBPGM instruction.               */
/*     ProcName is a null terminated string that identifies the   */
/*              name of the function whose address is to be       */
/*              returned.                                         */
/* Output:                                                        */
/*     FARPROC - function pointer to requested function. If the   */
/*     function does not exist, a NULL address is returned.       */
/*     alternative to passing the name of the function to find is */
/*     to pass the ordinal number of the function as a 4-byte     */
/*     integer where the leftmost two bytes of the number must be */
/*     zero and the right most two bytes is the relative function */
/*     number starting with 1 for the first function going to N   */
/*     where N is the number of exported functions in the service */
/*     program.                                                   */
/* ============================================================== */

FARPROC  GetProcAddress(HMODULE handle, const char *ProcName);



/* ============================================================== */
/*  Function: FreeLibrary                                         */
/*                                                                */
/*  Description:                                                  */
/*     Free a loaded DLL library.  This code just frees the handle*/
/*     to the activated service program and does not unload the   */
/*     routine.  It is provided to clean up the handle.           */
/*  Input:                                                        */
/*     handle to the loaded library - from LoadLibrary            */
/*  Output:                                                       */
/*     Returns true if successful -- false if not                 */
/* ============================================================== */

BOOL     FreeLibrary(HMODULE handle);



/* ============================================================== */
/*  Function: GetModuleFileName                                   */
/*                                                                */
/*  Description:                                                  */
/*     Retrieve the full path and file name for the file          */
/*     containing the specified module.                           */
/*  Input:                                                        */
/*     handle to the loaded library - from LoadLibrary            */
/*     file name of module - pointer to a buffer                  */
/*     size of buffer - specifies the length of the buffer        */
/*  Output:                                                       */
/*     Returns length of the string copied to the buffer or zero, */
/*     if the function fails.                                     */
/* ============================================================== */

DWORD    GetModuleFileName(HMODULE hModule, char *FileName, DWORD nSize);

#ifdef __cplusplus
 }
#endif

#endif // _DLL400_H_

#pragma checkout(resume)

/* <-- EOF --> */