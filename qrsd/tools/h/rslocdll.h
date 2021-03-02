//-*--------------------------------------------------------------------------*-
//
// File Name   : rslocdll.h
//
// Copyright (c) 1991 - 2006 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// June 16,2006  Sergei Kubrin (K78) - Create file
//-*--------------------------------------------------------------------------*-

#ifndef __RSLOCDLL_H
#define __RSLOCDLL_H

#include <packpshn.h>

// Виртуальные номера кодовых страниц для нестандартных OEM и ANSI страниц.
enum RslCPVirtual
   {
    RSL_CP_OEM_VIRTUAL   = 0,
    RSL_CP_ANSI_VIRTUAL  = 1
   };

typedef int (__stdcall * WideCharToMultiByteType)(
                                                  unsigned        codePage,         // code page
                                                  unsigned long   flags,
                                                  const wchar_t  *wideCharStr,      // wide-character string
                                                  int             cchWideChar,      // number of chars in string.
                                                  char           *multiByteStr,     // buffer for new string
                                                  int             cbMultiByte,      // size of buffer
                                                  const char     *defChar,
                                                  int            *useDefChar  
                                                 );


typedef int (__stdcall *MultiByteToWideCharType)(
                                                 unsigned       codePage,         // code page
                                                 unsigned long   flags,
                                                 const char    *multiByteStr,     // string to map
                                                 int            cbMultiByte,      // number of bytes in string
                                                 wchar_t       *wideCharStr,      // wide-character buffer
                                                 int            cchWideChar       // size of buffer
                                                );



// Используемая инструментом структура с информацией о
// локализации.
typedef struct tagRsLocaleExport
      {
       int                      version;  // Для текущей версии должно быть 1.
       unsigned                 cp [4];   // RSOEM, RSANSI, LCOEM, LCANSI
       unsigned long            localeID; // LCID 

       WideCharToMultiByteType  WideCharToMultiBytePtr;
       MultiByteToWideCharType  MultiByteToWideCharPtr;
      } TRsLocaleExportTable;

#ifdef LINK_RSLOCALE_DLL
    #define  RSLOCALEIMP  __declspec(dllimport)
#else
    #define  RSLOCALEIMP  __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C"
#endif
RSLOCALEIMP TRsLocaleExportTable * __cdecl GetInterface(void);

#include <packpop.h>

#endif // __RSLOCDLL_H

/* <-- EOF --> */