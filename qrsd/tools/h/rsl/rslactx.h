// -------------------------------------------------------------------------------------------------

#ifndef __RSLACTX_H
#define __RSLACTX_H

#ifndef __IDispatch_FWD_DEFINED__
    typedef struct tagDummi  { int dummy; }  IDispatch;
    typedef struct tagDummi2 { int dummy; }  VARIANT;
    typedef struct tagDummi3 { int dummy; }  IUnknown;
#endif

#ifndef _INC_WINDOWS
    typedef unsigned long HRESULT;
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "packpshn.h"

// -----------------------------------------------------------------------------
typedef struct tagTRslxIntf
      {
       // Common part of all RSL interfaces
       int         (DLMAPI *GetVersion)           (void);
       char       *(DLMAPI *GetName)              (void);

       TGenObject *(DLMAPI *RslActiveXCreateEx)   (IDispatch *pDisp, const char *tpName, const char *pcName, bool doAddRef);
       IDispatch  *(DLMAPI *RslObjGetIDispatch)   (TGenObject *obj);
       TGenObject *(DLMAPI *RslObjGetTGenObj)     (IDispatch *pDisp);
       IDispatch  *(DLMAPI *RslGetActiveXDispatch)(TGenObject *obj);

       HRESULT     (DLMAPI *ValueToVariant)       (const VALUE *val, VARIANT *var);
       HRESULT     (DLMAPI *VariantToValue)       (const VARIANT *var, VALUE *val);

       void        (DLMAPI *EnableMsgLoop)        (bool enable);
      } TRslxIntf;

#include "packpop.h"

#ifdef __cplusplus
}
#endif

#endif // __RSLACTX_H

/* <-- EOF --> */