/////////////////////////////////////////////////////////////////////////////////
// For support global cache in playrep
/////////////////////////////////////////////////////////////////////////////////

#ifndef __GCACHE_H
#define __GCACHE_H

#ifdef __cplusplus
extern "C" {
#endif

RSLLINK void RSLAPI SetFlagNeedDelete(HRSLCACHE hCache, bool flag);
RSLLINK bool RSLAPI GetFlagNeedDelete(HRSLCACHE hCache);

#ifdef __cplusplus
}
#endif

#endif // __GCACHE_H

/* <-- EOF --> */