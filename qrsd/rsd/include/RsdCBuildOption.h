#ifndef RSDCBUILDOPTION_H
#define RSDCBUILDOPTION_H

#ifdef RSDC_EXPORTS
#define RSDDLL __declspec (dllexport) 
#else
#define RSDDLL __declspec (dllimport) 
#endif

#endif