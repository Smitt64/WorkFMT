/*************************************************************************
   File Name      : rsatexit.h
   Programmers    : Alexey Lebedev

   Creation       : 02 Jun 2003
   Description    : Регистрация и выполнения AtExit-процедур.

   Copyright(C) 2003 by R-Style Software Lab.
   All Rights Reserved.

*************************************************************************/

#ifndef __RSATEXIT_H
#define __RSATEXIT_H

#include "platform.h"
#include "tooldef.h"

typedef void (rs_atexit_proc)(void);

#ifdef __cplusplus
extern "C" {
#endif

// Функция возвращает хэндл dll'и, из которой она была вызвана.
unsigned GetUserDll(void);

// Exit-процедуры выполняются после вызова RsMain, но перед закрытием fs'а.
// Функции с равным приоритетом вызываются в порядке LIFO, что соответсвует
// логике atexit. Функции с большим приоритетом вызываются позже, более
// высокий приоритет следует задавать для функций, удаляющих объекты-долгожители.
// Отрицательный приоритет, позволяет удалять объекты раньше других.

_TOOLFUNC void InitAtExit        (void);
_TOOLFUNC void DoneAtExit        (void);

_TOOLFUNC int  AddAtExitProc     (rs_atexit_proc proc);
_TOOLFUNC int  AddAtExitProcEx   (rs_atexit_proc proc, int priority);
_TOOLFUNC int  AddAtExitDllProc  (rs_atexit_proc proc, unsigned handle);
_TOOLFUNC int  AddAtExitDllProcEx(rs_atexit_proc proc, unsigned handle, int priority);

_TOOLFUNC int  DelAtExitProc     (rs_atexit_proc proc);

_TOOLFUNC void RunAtExit         (void);
_TOOLFUNC void RunAtExitEx       (int priority);
_TOOLFUNC void RunAtExitDll      (unsigned handle);

#ifdef __cplusplus
};
#endif

#if defined(RSL_MD_DLL)

    #define  RslAddExitProc(proc)        AddAtExitDllProc(proc, GetUserDll())
    #define  RslAddExitProcEx(proc, p)   AddAtExitDllProcEx(proc, GetUserDll(), p)
    #define  RslRunExitProc()            RunAtExitDll(GetUserDll())

#else

    #define  RslAddExitProc(proc)        AddAtExitProc(proc)
    #define  RslAddExitProcEx(proc, p)   AddAtExitProcEx(proc, p)
    #define  RslRunExitProc()            RunAtExit()

#endif

#define  RslDelExitProc(proc)  DelAtExitProc(proc)
#define  RslRunExitProcEx(p)   RunAtExitEx(p)

#endif //__RSATEXIT_H

/* <-- EOF --> */