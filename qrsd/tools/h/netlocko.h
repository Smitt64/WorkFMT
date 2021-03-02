//***********************************************************************
//        Group locks for NetWare applications.
//                        VECsoft 06.11.92
//***********************************************************************

#ifndef _NETLOCKO_H_
#define _NETLOCKO_H_

#include "nwsync.h"
#include "nwconn.h"

#include "apptldef.h"


extern _APPTLDATA BYTE  SAddr[6];         // Node address
extern _APPTLDATA WORD  CNum;        // Connection number

void initNetwGr        // Network group initialisation.
               (
                char *grn               // Group name. Truncated to 8 characters!
               );

void ClearGrLock(void);     // Clear group resource lock.

unsigned char GrLock   // Lock group resource.
                    (                             // 0 - OK  1 - Failed  2 - Network error
                     unsigned char excl           // ON - Full lock needed
                    );

#endif // _NETLOCKO_H_

/* <-- EOF --> */