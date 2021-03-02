#ifndef __APPINTF_H
#define __APPINTF_H

#include "tooldef.h"
#include "rstypes.h"

#ifndef RSL_DLL_NAME_LEN
    #define  RSL_DLL_NAME_LEN  9
#endif

#include "rscom/appintf.hpp"

#if defined(__cplusplus)
    #include "rscom/rscomi.hpp"
    #include "rscom/rsdata.hpp"
#endif

#ifdef __cplusplus
extern "C" {
#endif
                        
#ifndef __SRVENTRY_H
    TOOLDLMEXP void *TOOLDLM fs_getAppIntf(void);
#endif

#ifdef __cplusplus
}
#endif

#endif // __APPINTF_H

/* <-- EOF --> */