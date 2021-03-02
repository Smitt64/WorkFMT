/*************************************************************************
        File Name               : irhash_c.h                    22.05.2000
        Programmer              : Tokmashev A.

        Subsystem               : Interpreter RSL
        Description             : C-wrapper for class TRSLHash

        Source                  : ---

        Library(s)              : ---

        Copyright(C) 1991 - 2000 by R-Style Software Lab.
        All Rights Reserved.

        Note: 1) Class TRSLHash, converted all keys to up shift !!!!!
              
                  2) Class TRSLHash throw rsl_bad_alloc(), when out of memory.

*************************************************************************/

#ifndef __IRSLHASH_C_H
#define __IRSLHASH_C_H

#ifndef RSL_NO_HASH

    #include "platform.h"

    RSL_MAKE_HANDLE(HHASH);

    #ifdef __cplusplus
    extern "C" {
    #endif

            HHASH    CreateHash(void);
            void     AddSymbolToHash(HHASH hash, ISYMBOL *symbol);
            ISYMBOL *FindSymbolInHash(HHASH hash, char *key, bool onlyClass);
            void     DeleteHash(HHASH hash);

    #ifdef __cplusplus
    }
    #endif

#endif // RSL_NO_HASH

#endif // __IRSLHASH_C_H

/* <-- EOF --> */