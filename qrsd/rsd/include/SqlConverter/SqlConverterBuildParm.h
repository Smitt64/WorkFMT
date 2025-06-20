#ifndef _SQLCONVERTERBUILDPARM_H
#define _SQLCONVERTERBUILDPARM_H


#ifdef SqlConverter_EXPORTS
   #define __SHARED_DIRECTION __declspec(dllexport)
#else
   #ifdef SqlConverter_IMPORTS
      #define __SHARED_DIRECTION __declspec(dllimport)
   #else
      #define __SHARED_DIRECTION
   #endif
#endif


#endif