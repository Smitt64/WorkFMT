#ifndef __TRNMGR_H_GUARD__
#define __TRNMGR_H_GUARD__

#ifndef __BORLANDC__
#include <transact.h>
#else
typedef struct ITransaction ITransaction;
#endif

#include "packpshn.h"

typedef enum tagRsTrnAttrs {
	/* Isolation level */
    RSTRN_IL_UNSPECIFIED		  = 0x00000000,
		RSTRN_IL_CHAOS            = 0x00000010,
		RSTRN_IL_READUNCOMMITTED  = 0x00000100,
		RSTRN_IL_BROWSE           = 0x00000100,
		RSTRN_IL_READCOMMITTED    = 0x00001000,
		RSTRN_IL_CURSORSTABILITY  = 0x00001000,
		RSTRN_IL_REPEATABLEREAD   = 0x00010000,
		RSTRN_IL_SERIALIZABLE     = 0x00100000,
		RSTRN_IL_ISOLATED         = 0x00100000,
		RSTRN_IL_MASK			  = 0x00111111,
		/* Isolation flag */
		RSTRN_IF_RETAIN_COMMIT_DC		= 0x01000000,
		RSTRN_IF_RETAIN_COMMI			= 0x02000000,
		RSTRN_IF_RETAIN_COMMIT_NO		= 0x03000000,
		RSTRN_IF_RETAIN_ABORT_DC		= 0x04000000,
		RSTRN_IF_RETAIN_ABORT			= 0x08000000,
		RSTRN_IF_RETAIN_ABORT_NO		= 0x0C000000,
		RSTRN_IF_RETAIN_DONTCARE		= 0x07000000,
		RSTRN_IF_RETAIN_BOTH			= 0x0A000000,
		RSTRN_IF_RETAIN_NONE			= 0x0D000000,
		RSTRN_IF_OPTIMISTIC				= 0x10000000,
		RSTRN_IF_READONLY				= 0x20000000
} tagRsTrnAttrs; 

typedef int TrnResult;

#define TRN_OK		0
#define TRN_FAIL	1

#define TRN_SUCCESS(rc) (!(rc))
#define TRN_FAILED(rc) (rc)

typedef struct tagRsTransaction
{
	struct tagIRsTransaction* vtbl;
} RsTransaction, *HRSTRANSACTION;

typedef struct tagIRsTransaction
{
	 TrnResult (*pBegin)(HRSTRANSACTION, int attr);
	 TrnResult (*pCommit)(HRSTRANSACTION);
	 TrnResult (*pAbort)(HRSTRANSACTION);
} IRsTransaction;

typedef struct tagRsResourceMgr
{
	struct tagIRsResourceMgr* vtbl;
} RsResourceMgr, *HRSRESOURCEMGR;

typedef struct tagIRsResourceMgr
{
	TrnResult (*pBegin)(HRSRESOURCEMGR, int attr);
	TrnResult (*pCommit)(HRSRESOURCEMGR);
	TrnResult (*pAbort)(HRSRESOURCEMGR);
	TrnResult (*CanUseMSDTC)(HRSRESOURCEMGR);
	TrnResult (*pEnlistMSDTC)(HRSRESOURCEMGR, ITransaction*);
	TrnResult (*pGetModuleName)(HRSRESOURCEMGR, char*, int);
	TrnResult (*pInit)(HRSRESOURCEMGR);
	TrnResult (*pDone)(HRSRESOURCEMGR);
} IRsResourceMgr;

typedef struct tagRsTrnMgr
{
	struct tagIRsTrnMgr* vtbl;
} RsTrnMgr, *HRSTRNMGR;

typedef struct tagIRsTrnMgr
{
	TrnResult (*pBegin)(HRSTRNMGR, int attr);
	TrnResult (*pCommit)(HRSTRNMGR);
	TrnResult (*pAbort)(HRSTRNMGR);
	TrnResult (*pAddResourceMgr)(HRSTRNMGR, HRSRESOURCEMGR);
	TrnResult (*pRemResourceMgr)(HRSTRNMGR, HRSRESOURCEMGR);
	TrnResult (*pUseMsDTC)(HRSTRNMGR, int);
	TrnResult (*pInTransaction)(HRSTRNMGR);

	TrnResult (*pGetResMgr)(HRSTRNMGR, const char* name, HRSRESOURCEMGR *pMgr);
	TrnResult (*pInsertBefore)(HRSTRNMGR, HRSRESOURCEMGR what, const char* name, const char* relative);
	TrnResult (*pInsertAfter)(HRSTRNMGR, HRSRESOURCEMGR what, const char* name, const char* relative);
	TrnResult (*pSetCommitDirection)(HRSTRNMGR, int fwd);	
	
	TrnResult (*pGetResMgrCount)(HRSTRNMGR, int* count);
	TrnResult (*pGetResMgrByIndex)(HRSTRNMGR, int index, HRSRESOURCEMGR* pMgr);
	TrnResult (*pGetResMgrName)(HRSTRNMGR, HRSRESOURCEMGR pMgr, const char **);
} IRsTrnMgr;

#ifdef RSTRNMGR_EXPORTS
#define  RSTRNMGR_EXPORT __declspec(dllexport)
#else
#define  RSTRNMGR_EXPORT __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
#endif
RSTRNMGR_EXPORT HRSTRNMGR GetTrnMgr();

typedef HRSTRNMGR(*GetTrnMgr_t)();

#include "packpop.h"

#endif /* __TRNMGR_H_GUARD__ */