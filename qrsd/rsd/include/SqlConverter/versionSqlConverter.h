#ifndef VERSIONSQLCONVERTER_H
#define VERSIONSQLCONVERTER_H

#ifdef _WIN64
  #define ARCH_X64
#else
  #define ARCH_X32
#endif


  #ifdef _DEBUG
    #ifdef ARCH_X64
      #include "versionSqlConverter_Debug_x64.h"
    #else
      #include "versionSqlConverter_Debug_x32.h"
    #endif
  #else
    #ifdef RELWITHDEBINFO
      #ifdef ARCH_X64
         #include "versionSqlConverter_RelWithDebInfo_x64.h"
      #else
         #include "versionSqlConverter_RelWithDebInfo_x32.h"
      #endif
    #else
      #ifdef ARCH_X64
         #include "versionSqlConverter_Release_x64.h"
      #else
         #include "versionSqlConverter_Release_x32.h"
      #endif
    #endif
  #endif

#endif