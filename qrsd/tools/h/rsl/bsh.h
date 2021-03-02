/**********************************************************************
*        File Name:   bsh.h              February 16,1994            
*       Programmer:   S. Kubrin              
*      Description:   Содержит структуру словаря BSHELL
*  Public routines:  
* Private routines:  
***********************************************************************/

#ifndef __BSH_H
#define __BSH_H

#include "nwbtrv.h"
#include "listobj.h"

#include "rsl/expdef.h"
#include "rsl/bt.h"
#include "rsl/bshstruc.h"


#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
typedef struct
      {
       BUFREC        brec;
       LOBJ          elem;
       TBtDataBase  *dataBase;
      } RECLIST;

struct tagBSHSTRUC
     {
      BUFREC   file;
      CNTLIST  field_list;  // Список RECLIST - поля
      CNTLIST  key_list;    // Список RECLIST - ключи
     };

typedef struct tagBSHSTRUC  BSHSTRUC;

// -----------------------------------------------------------------------------
int bshCheck(char *PrjName, char *dataDir, FILE *xout);

_RSLLINK TBtDataBase *bshLogin(char *path, TBtMesProc aBtMesProc, char *globalPassword, bool reportError);
_RSLLINK TBtDataBase *bshLogin2(char *path, TBtMesProc aBtMesProc, char *globalPassword, bool reportError, TBtError *erInfo);

RSLLINK TBtDataBase *RSLAPI bshLoginSimple(char *path, bool rdOnly, TBtError *erInfo);

// -----------------------------------------------------------------------------
TBtDataBase *bshLoginEx(char *path, TBtMesProc aBtMesProc, TBtMemAllocProc allocPtr, TBtMemFreeProc freePtr,
                        char *globalPassword, bool reportError, bool rdOnly);

// -----------------------------------------------------------------------------
TBtDataBase *bshLoginEx2(char *path, TBtMesProc aBtMesProc, TBtMemAllocProc allocPtr, TBtMemFreeProc freePtr,
                         char *globalPassword,
                         bool reportBtrError, // if true call aBtMesProc for Btrieve errors
                         bool rdOnly, TBtError *erInfo, char *dataDir);

// -----------------------------------------------------------------------------
TBtDataBase *bshLoginEx3(char *path, TBtMesProc aBtMesProc, TBtMemAllocProc allocPtr, TBtMemFreeProc freePtr,
                         char *globalPassword,
                         bool reportBtrError, // if true call aBtMesProc for Btrieve errors
                         bool rdOnly, TBtError *erInfo, char *dataPath, bclient_h clientID);

// -----------------------------------------------------------------------------
TBtDataBase *bshLoginEx4(char *path, TBtMesProc aBtMesProc, TBtMemAllocProc allocPtr, TBtMemFreeProc freePtr,
                         char *globalPassword,
                         bool reportBtrError, // if true call aBtMesProc for Btrieve errors
                         bool rdOnly, TBtError *erInfo, char *dataPath, bclient_h clientID, BTClientID *BtrID);

// -----------------------------------------------------------------------------
TBtError *bshGetErrInfo(TBtDataBase *db, int *numInfo);

RSLLINK void RSLAPI bshLogout     (TBtDataBase *db);
        int         bshSetBlobType(TBtDataBase *db, int tp);


BSHSTRUC    *bshGetStruct    (TBtDataBase *db, char *file);
void         bshFreeStruct   (BSHSTRUC *bsh, TBtDataBase *db);
int          bshCreateFile   (TBtDataBase *db, char *strName, char *file);
int          bshCreateFileSQL(TBtDataBase *db, char *strName, char *file, bool permanent);
_RSLLINK int bshLoopFile     (TBtDataBase *db, TLoopProc proc, void *data);

_RSLLINK long bshCalcFiles(TBtDataBase *db);

void bshUnloadEngine(void);
void bshSetLockFlag(TBtDataBase *db, int reclock);

void StrucInitList (TBtDataBase *db);
void StrucCloseList(TBtDataBase *db);

int bshSetSQLMode(TBtDataBase *db, bool on);

#ifdef __cplusplus
}
#endif

#endif

/* <-- EOF --> */