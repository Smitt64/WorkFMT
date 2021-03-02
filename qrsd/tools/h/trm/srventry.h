#ifndef __SRVENTRY_H
#define __SRVENTRY_H

#include "platform.h"

#ifndef __EXT_H
    #include "dlm.h"
#endif
                          
#include "rscom/appintf.hpp"

// -----------------------------------------------
#ifndef __TERMEXT_H
    #ifndef __DLMDEF_H
        #include "dlm.h"
    #endif

    extern BACKENTRY  *ExeExports;
#endif

// -----------------------------------------------
#ifdef RSL_MD_FLAT
    #ifdef RSAPPSERV
        #define  APPEXP    DLMAPIC
        #define  EXPIMP32  IMP32
    #else
        #define  APPEXP    DLMAPIC
        #define  EXPIMP32  EXP32
    #endif
#else                                 
    #ifdef RSAPPSERV
        #define  APPEXP  DLMAPIC IMP
        #define  EXP32
    #else
        #define  APPEXP  DLMAPIC EXP
        #define  EXP32
    #endif
#endif


#pragma pack(push, 1)


#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------
typedef struct
      {
       char            magic[18];
       unsigned short  build;
       unsigned short  verHi;
       unsigned short  verLow;
      } TTermVersion;

extern TAppObj  *AppartmentPtr;

#define  fs_getAppIntf()  ((void *)AppartmentPtr)

EXPIMP32 int  APPEXP RslExtMessageProc(int cmd, char *inMes, char *outMes);
EXPIMP32 void APPEXP RslExtSetupProc(TAppObj *);

// -----------------------------------------------------------------------------
#if defined(RSL_MD_FLAT) && !defined(RSL_MD_DPMI32)

    #define  RSL_EXT_MES_TABLE                                                                                    \
               TAppObj  *AppartmentPtr;                                                                           \
               BACKENTRY  *ExeExports = NULL;                                                                     \
               EXP32 int DLMAPI EXP RslSetCallBacks(BACKENTRY *cmd) { ExeExports = cmd; return RSL_DLM_VERSION; } \
               EXP32 void APPEXP RslExtSetupProc(TAppObj *ptr) { AppartmentPtr = ptr; }                           \
               EXP32 int APPEXP RslExtMessageProc(int cmd, char *inMes, char *outMes)                             \
               { switch(cmd) { default: break;

#else

    #define  RSL_EXT_MES_TABLE                                                          \
               TAppObj  *AppartmentPtr;                                                 \
               EXP32 void APPEXP RslExtSetupProc(TAppObj *ptr) { AppartmentPtr = ptr; } \
               EXP32 int APPEXP RslExtMessageProc(int cmd, char *inMes, char *outMes)   \
               { RslGetInstCount(); switch(cmd) { default:  break;

#endif

// -----------------------------------------------------------------------------
#define  RSL_EXT_HANDLE(msg)       case msg: return mes_##msg(inMes,outMes);
#define  RSL_EXT_MES_LOAD          case -1:                    mes_Load(); return 0; 
#define  RSL_EXT_MES_LOADEX        case -1:                    mes_LoadEx((TAppObj*)inMes); return 0; 
#define  RSL_EXT_MES_UNLOAD        case -2:                    mes_Unload(); return 0;
#define  RSL_EXT_MES_UNLOADEX      case -2:                    mes_UnloadEx((TAppObj *)inMes); return 0;
#define  RSL_EXT_MES_CANUNLOAD     case -5:                    return mes_CanUnload();
#define  RSL_EXT_MES_CANUNLOADEX   case -5:                    return mes_CanUnloadEx((TCreateObjData *)inMes);
#define  RSL_EXT_MES_REVOKE        case SYS_CMD_REVOKE:        mes_Revoke((TCreateObjData *)inMes); return 0;
#define  RSL_EXT_MES_VERSION       case -4:                    mes_Version((TTermVersion *)inMes); return 0;
#define  RSL_EXT_MES_CREATEOBJ     case SYS_CMD_CREATE_OBJECT: return mes_CreateObj((TCreateObjData *)inMes); 
#define  RSL_EXT_MES_CREATEMETA    case SYS_CMD_CREATE_META:   return mes_CreateMetaObj((TCreateObjData *)inMes); 

// -----------------------------------------------------------------------------
#ifdef RSL_MD_FLAT
    #define  RSL_EXT_MES_END   } return 0; }
#else

    #define  RSL_EXT_MES_END   }}                                                   \
               ENTRY  cmdTable[] = {                                                \
                { "_RslExtMessageProc", RslExtMessageProc },                        \
                { "RslSetCallBacks",    RslSetCallBacks   },                        \
                { NULL, NULL } };                                                   \
                void  *malloc(size_t sz) { return _EXT_GetMem(sz); }                \
                void huge *farmalloc(unsigned long sz) { return _EXT_GetMem (sz); } \
                void free(void *ptr) { _EXT_FreeMem (ptr); }                        \
                void farfree(void huge *ptr) {_EXT_FreeMem (ptr); }                 \
                int DLMAPI EXP ExitLib(void) { return 0; }                          \
                int DLMAPI EXP InitLib(void) { return 0; }                          

#endif  // RSL_MD_FLAT


#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif // __SRVENTRY_H

/* <-- EOF --> */