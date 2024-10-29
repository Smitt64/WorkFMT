// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the QRSDCORE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// QRSDCORE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef QRSDCORE_EXPORTS
#define QRSDCORE_API __declspec(dllexport)
#else
#define QRSDCORE_API __declspec(dllimport)
#endif

#define RSD_ENV "RSDENV"
#define RSD_CON "RSDCON"
#define RSD_CMD "RSDCMD"
#define RSD_REC "RSDREC"
#define RSD_ERR "RSDERR"
#define RSD_FLD "RSDFLD"

class CCoreObject;
typedef struct
{
	char magic[6];
	CCoreObject *ptr;
}RSDCOREHANDLE;

bool CheckHandle(void *handle, const char *magic);

extern "C"
{
QRSDCORE_API void coreFreeHandle(void **handle);

// connection
QRSDCORE_API bool coreConnOpen(const char *dsn, const char *user, const char *pswd, const char *options, void **handle);
QRSDCORE_API void coreConnClose(void *handle);
QRSDCORE_API bool coreConnIsUnicode(void *handle);
QRSDCORE_API bool coreConnIsPostgres(void *handle);
QRSDCORE_API bool coreConnIsOpen(void *handle);

QRSDCORE_API bool coreConnBeginTrn(void *handle);
QRSDCORE_API bool coreConnCommitTrn(void *handle);
QRSDCORE_API bool coreConnRollbackTrn(void *handle);

// command
QRSDCORE_API void coreCmdInit(void **handle, void *connection);
QRSDCORE_API bool coreCmdSetCmdText(void *handle, const char *cmd);
QRSDCORE_API void coreCmdClearParams(void *handle);
QRSDCORE_API void coreCmdAddParam(void *handle, const char* name, 
		int type, void* value, long* indlen, 
		long len, int dir, bool isWide);
QRSDCORE_API void coreCmdInsertParam(void *handle, int index, const char* name, 
		int type, void* value, long* indlen, 
		long len, int dir, bool isWide);
QRSDCORE_API void coreCmdDisableConverter(void *handle, bool value);
QRSDCORE_API int coreCmdExecute(void *handle);

// recordset
QRSDCORE_API void coreRecInit(void **handle, void *cmd, bool isForwardOnly);
QRSDCORE_API bool coreRecIsNull(void *handle);
QRSDCORE_API int coreRecCurPos(void *handle);
QRSDCORE_API int coreRecFldCount(void *handle);
QRSDCORE_API int coreRecCount(void *handle);
QRSDCORE_API bool coreRecMove(void *handle, int index, int dir);
QRSDCORE_API bool coreRecFirst(void *handle);
QRSDCORE_API bool coreRecLast(void *handle);
QRSDCORE_API bool coreRecNext(void *handle);
QRSDCORE_API bool coreRecPrev(void *handle);
QRSDCORE_API void coreRecGetFld(void *handle, int index, void **field);

QRSDCORE_API const char *coreFldName(void *handle);
QRSDCORE_API int coreFldType(void *handle);
QRSDCORE_API bool coreFldIsNull(void *handle);

QRSDCORE_API char coreFldAsByte(void *handle);
QRSDCORE_API short coreFldAsShort(void *handle);
QRSDCORE_API unsigned short coreFldAsUShort(void *handle);
QRSDCORE_API int coreFldAsLong(void *handle);
QRSDCORE_API unsigned int coreFldAsULong(void *handle);
QRSDCORE_API long long coreFldAsBigInt(void *handle);
QRSDCORE_API float coreFldAsFloat(void *handle);
QRSDCORE_API double coreFldAsDouble(void *handle);
QRSDCORE_API const char *coreFldAsString(void *handle);
QRSDCORE_API char coreFldAsChar(void *handle);

QRSDCORE_API int coreFldBufferLen(void *handle);
QRSDCORE_API void *coreFldAsBinary(void *handle);
QRSDCORE_API void coreFldRead(void *handle, void *ptr, int size);

QRSDCORE_API void coreFldAsDate(void *handle, void *tmp);
QRSDCORE_API void coreFldAsTime(void *handle, void *tmp);
QRSDCORE_API void coreFldAsDateTime(void *handle, void *tmp);

// errors
QRSDCORE_API void coreGetLastError(void *handle, char *buffer, int len);
QRSDCORE_API void coreGetErrorHandle(void *connection, void **handle);
QRSDCORE_API int coreGetErrorCount(void *handle);
QRSDCORE_API void coreGetErrorText(void *handle, int index, char *buffer, int len);
};