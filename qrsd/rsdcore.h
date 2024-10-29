#ifndef RSDCORE_H
#define RSDCORE_H

#include <QObject>
#include <QDebug>
#include <stdexcept>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(SqlRsdDriver);

typedef void (*coreFreeHandle_t)(void **handle);

typedef bool (*coreConnOpen_t)(const char *dsn, const char *user, const char *pswd, const char *options, void **handle);
typedef void (*coreConnClose_t)(void *handle);
typedef bool (*coreConnIsUnicode_t)(void *handle);
typedef bool (*coreConnIsPostgres_t)(void *handle);
typedef bool (*coreConnIsOpen_t)(void *handle);
typedef bool (*coreConnBeginTrn_t)(void *handle);
typedef bool (*coreConnCommitTrn_t)(void *handle);
typedef bool (*coreConnRollbackTrn_t)(void *handle);

// command
typedef void (*coreCmdInit_t)(void **handle, void *connection);
typedef bool (*coreCmdSetCmdText_t)(void *handle, const char *cmd);
typedef void (*coreCmdClearParams_t)(void *handle);
typedef void (*coreCmdAddParam_t)(void *handle, const char* name,
        int type, void* value, long* indlen,
        long len, int dir, bool isWide);
typedef void (*coreCmdInsertParam_t)(void *handle, int index,
        const char* name,
        int type, void* value, long* indlen,
        long len, int dir, bool isWide);
typedef void (*coreCmdDisableConverter_t)(void *handle, bool value);
typedef int (*coreCmdExecute_t)(void *handle);

// recordset
typedef void (*coreRecInit_t)(void **handle, void *cmd, bool isForwardOnly);
typedef bool (*coreRecIsNull_t)(void *handle);
typedef int (*coreRecCurPos_t)(void *handle);
typedef int (*coreRecFldCount_t)(void *handle);
typedef int (*coreRecCount_t)(void *handle);
typedef bool (*coreRecMove_t)(void *handle, int index, int dir);
typedef bool (*coreRecFirst_t)(void *handle);
typedef bool (*coreRecLast_t)(void *handle);
typedef bool (*coreRecNext_t)(void *handle);
typedef bool (*coreRecPrev_t)(void *handle);
typedef void (*coreRecGetFld_t)(void *handle, int index, void **field);

typedef const char* (*coreFldName_t)(void *handle);
typedef int (*coreFldType_t)(void *handle);
typedef bool (*coreFldIsNull_t)(void *handle);

typedef char (*coreFldAsByte_t)(void *handle);
typedef short (*coreFldAsShort_t)(void *handle);
typedef unsigned short (*coreFldAsUShort_t)(void *handle);
typedef int (*coreFldAsLong_t)(void *handle);
typedef unsigned int (*coreFldAsULong_t)(void *handle);
typedef long long (*coreFldAsBigInt_t)(void *handle);

typedef float (*coreFldAsFloat_t)(void *handle);
typedef double (*coreFldAsDouble_t)(void *handle);
typedef const char* (*coreFldAsString_t)(void *handle);
typedef char (*coreFldAsChar_t)(void *handle);
typedef int (*coreFldBufferLen_t)(void *handle);
typedef void *(*coreFldAsBinary_t)(void *handle);
typedef void (*coreFldRead_t)(void *handle, void *ptr, int size);

typedef void (*coreFldAsDate_t)(void *handle, void *tmp);
typedef void (*coreFldAsTime_t)(void *handle, void *tmp);
typedef void (*coreFldAsDateTime_t)(void *handle, void *tmp);

typedef void (*coreGetLastError_t)(void *handle, char *buffer, int len);
typedef void (*coreGetErrorHandle_t)(void *connection, void **handle);
typedef int (*coreGetErrorCount_t)(void *handle);
typedef void (*coreGetErrorText_t)(void *handle, int index, char *buffer, int len);

extern coreFreeHandle_t coreFreeHandle;

extern coreGetLastError_t coreGetLastError;
extern coreGetErrorHandle_t coreGetErrorHandle;
extern coreGetErrorCount_t coreGetErrorCount;
extern coreGetErrorText_t coreGetErrorText;

extern coreConnOpen_t coreConnOpen;
extern coreConnClose_t coreConnClose;
extern coreConnIsUnicode_t coreConnIsUnicode;
extern coreConnIsPostgres_t coreConnIsPostgres;
extern coreConnIsOpen_t coreConnIsOpen;
extern coreConnBeginTrn_t coreConnBeginTrn;
extern coreConnCommitTrn_t coreConnCommitTrn;
extern coreConnRollbackTrn_t coreConnRollbackTrn;

// command
extern coreCmdInit_t coreCmdInit;
extern coreCmdSetCmdText_t coreCmdSetCmdText;
extern coreCmdClearParams_t coreCmdClearParams;
extern coreCmdAddParam_t coreCmdAddParam;
extern coreCmdInsertParam_t coreCmdInsertParam;
extern coreCmdDisableConverter_t coreCmdDisableConverter;
extern coreCmdExecute_t coreCmdExecute;

// recordset
extern coreRecInit_t coreRecInit;
extern coreRecIsNull_t coreRecIsNull;
extern coreRecCurPos_t coreRecCurPos;
extern coreRecFldCount_t coreRecFldCount;
extern coreRecCount_t coreRecCount;
extern coreRecMove_t coreRecMove;
extern coreRecFirst_t coreRecFirst;
extern coreRecLast_t coreRecLast;
extern coreRecNext_t coreRecNext;
extern coreRecPrev_t coreRecPrev;
extern coreRecGetFld_t coreRecGetFld;

extern coreFldName_t coreFldName;
extern coreFldType_t coreFldType;
extern coreFldIsNull_t coreFldIsNull;

extern coreFldAsByte_t coreFldAsByte;
extern coreFldAsShort_t coreFldAsShort;
extern coreFldAsUShort_t coreFldAsUShort;
extern coreFldAsLong_t coreFldAsLong;
extern coreFldAsULong_t coreFldAsULong;
extern coreFldAsBigInt_t coreFldAsBigInt;
extern coreFldRead_t coreFldRead;

extern coreFldAsFloat_t coreFldAsFloat;
extern coreFldAsDouble_t coreFldAsDouble;
extern coreFldAsString_t coreFldAsString;
extern coreFldAsChar_t coreFldAsChar;
extern coreFldBufferLen_t coreFldBufferLen;
extern coreFldAsBinary_t coreFldAsBinary;

extern coreFldAsDate_t coreFldAsDate;
extern coreFldAsTime_t coreFldAsTime;
extern coreFldAsDateTime_t coreFldAsDateTime;

#define ERR_LEN (1024 * 5)

class QTextCodec;
class CRsdCoreLoader
{
public:
    CRsdCoreLoader();

    bool isLoaded();
    bool load();

private:
    std::string GetErrorMessage();
    template<class T>T resolve(const char *name)
    {
        T func = (T)GetProcAddress((HMODULE)hModule, name);

        if (!func)
        {
            std::string msg = std::string("Can't resolve symbol ") +
                    std::string(name) + std::string(": ") +
                    GetErrorMessage();

            throw std::runtime_error(msg);
        }

        return func;
    }

    QTextCodec *codec;
    Qt::HANDLE hModule;
};

#endif // RSDCORE_H
