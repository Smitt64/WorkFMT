#ifndef RSDUTLBUILDOPTION_H
#define RSDUTLBUILDOPTION_H

#ifdef RSDUtl_EXPORTS
#define DLL_DIRECTION __declspec (dllexport) 
#else
#define DLL_DIRECTION __declspec (dllimport) 
#endif

#endif