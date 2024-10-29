#include <rsdcore.h>
#include <Windows.h>
#include <QDebug>
#include <QTextCodec>
#include <windows.h>
#include <strsafe.h>

Q_LOGGING_CATEGORY(SqlRsdDriver, "Sql.Rsd.Driver")

coreFreeHandle_t coreFreeHandle = nullptr;

coreConnOpen_t coreConnOpen = nullptr;
coreConnClose_t coreConnClose = nullptr;
coreConnIsUnicode_t coreConnIsUnicode = nullptr;
coreConnIsPostgres_t coreConnIsPostgres = nullptr;
coreConnIsOpen_t coreConnIsOpen = nullptr;
coreConnBeginTrn_t coreConnBeginTrn = nullptr;
coreConnCommitTrn_t coreConnCommitTrn = nullptr;
coreConnRollbackTrn_t coreConnRollbackTrn = nullptr;

// command
coreCmdInit_t coreCmdInit = nullptr;
coreCmdSetCmdText_t coreCmdSetCmdText = nullptr;
coreCmdClearParams_t coreCmdClearParams = nullptr;
coreCmdAddParam_t coreCmdAddParam = nullptr;
coreCmdInsertParam_t coreCmdInsertParam = nullptr;
coreCmdDisableConverter_t coreCmdDisableConverter = nullptr;
coreCmdExecute_t coreCmdExecute = nullptr;

// recordset
coreRecInit_t coreRecInit = nullptr;
coreRecIsNull_t coreRecIsNull = nullptr;
coreRecCurPos_t coreRecCurPos = nullptr;
coreRecFldCount_t coreRecFldCount = nullptr;
coreRecCount_t coreRecCount = nullptr;
coreRecMove_t coreRecMove = nullptr;
coreRecFirst_t coreRecFirst = nullptr;
coreRecLast_t coreRecLast = nullptr;
coreRecNext_t coreRecNext = nullptr;
coreRecPrev_t coreRecPrev = nullptr;
coreRecGetFld_t coreRecGetFld = nullptr;

coreFldName_t coreFldName = nullptr;
coreFldType_t coreFldType = nullptr;
coreFldIsNull_t coreFldIsNull = nullptr;
coreFldRead_t coreFldRead = nullptr;

coreFldAsByte_t coreFldAsByte = nullptr;
coreFldAsShort_t coreFldAsShort = nullptr;
coreFldAsUShort_t coreFldAsUShort = nullptr;
coreFldAsLong_t coreFldAsLong = nullptr;
coreFldAsULong_t coreFldAsULong = nullptr;
coreFldAsBigInt_t coreFldAsBigInt = nullptr;

coreFldAsFloat_t coreFldAsFloat = nullptr;
coreFldAsDouble_t coreFldAsDouble = nullptr;
coreFldAsString_t coreFldAsString = nullptr;
coreFldAsChar_t coreFldAsChar = nullptr;
coreFldBufferLen_t coreFldBufferLen = nullptr;
coreFldAsBinary_t coreFldAsBinary = nullptr;

// recordset
coreFldAsDate_t coreFldAsDate = nullptr;
coreFldAsTime_t coreFldAsTime = nullptr;
coreFldAsDateTime_t coreFldAsDateTime = nullptr;

coreGetLastError_t coreGetLastError = nullptr;
coreGetErrorHandle_t coreGetErrorHandle = nullptr;
coreGetErrorCount_t coreGetErrorCount = nullptr;
coreGetErrorText_t coreGetErrorText = nullptr;

#define CALL_RESOLVE(name) name = resolve<name##_t>(#name)

CRsdCoreLoader::CRsdCoreLoader()
{
    hModule = nullptr;
    codec = QTextCodec::codecForName("Windows-1251");
}

bool CRsdCoreLoader::isLoaded()
{
    return hModule;
}

std::string CRsdCoreLoader::GetErrorMessage()
{
    char *err;
    DWORD errCode = GetLastError();

    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                               NULL,
                               errCode,
                               MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), // default language
                               (LPSTR) &err,
                               0,
                               NULL);
    std::string msg = err;
    LocalFree(err);

    return msg;
}

bool CRsdCoreLoader::load()
{
    hModule = LoadLibrary(L"d:\\Build\\qrsdtest\\qrsd\\qrsdcore\\release\\qrsdcore.dll");

    if (!hModule)
    {
        qCritical(SqlRsdDriver()) << GetErrorMessage().c_str();
        return false;
    }

    try
    {
        CALL_RESOLVE(coreFreeHandle);

        CALL_RESOLVE(coreConnOpen);
        CALL_RESOLVE(coreConnClose);
        CALL_RESOLVE(coreConnIsUnicode);
        CALL_RESOLVE(coreConnIsPostgres);
        CALL_RESOLVE(coreConnIsOpen);

        CALL_RESOLVE(coreConnBeginTrn);
        CALL_RESOLVE(coreConnCommitTrn);
        CALL_RESOLVE(coreConnRollbackTrn);

        CALL_RESOLVE(coreCmdInit);
        CALL_RESOLVE(coreCmdSetCmdText);
        CALL_RESOLVE(coreCmdAddParam);
        CALL_RESOLVE(coreCmdInsertParam);
        CALL_RESOLVE(coreCmdClearParams);
        CALL_RESOLVE(coreCmdDisableConverter);
        CALL_RESOLVE(coreCmdExecute);

        CALL_RESOLVE(coreRecInit);
        CALL_RESOLVE(coreRecIsNull);
        CALL_RESOLVE(coreRecCurPos);
        CALL_RESOLVE(coreRecFldCount);
        CALL_RESOLVE(coreRecCount);
        CALL_RESOLVE(coreRecMove);
        CALL_RESOLVE(coreRecFirst);
        CALL_RESOLVE(coreRecLast);
        CALL_RESOLVE(coreRecNext);
        CALL_RESOLVE(coreRecPrev);
        CALL_RESOLVE(coreRecGetFld);
        CALL_RESOLVE(coreFldAsChar);
        CALL_RESOLVE(coreFldBufferLen);
        CALL_RESOLVE(coreFldAsBinary);
        CALL_RESOLVE(coreFldRead);
        CALL_RESOLVE(coreFldName);
        CALL_RESOLVE(coreFldType);
        CALL_RESOLVE(coreFldIsNull);

        CALL_RESOLVE(coreFldAsByte);
        CALL_RESOLVE(coreFldAsShort);
        CALL_RESOLVE(coreFldAsUShort);
        CALL_RESOLVE(coreFldAsLong);
        CALL_RESOLVE(coreFldAsULong);
        CALL_RESOLVE(coreFldAsBigInt);
        CALL_RESOLVE(coreFldAsFloat);
        CALL_RESOLVE(coreFldAsDouble);
        CALL_RESOLVE(coreFldAsString);
        CALL_RESOLVE(coreFldAsChar);

        CALL_RESOLVE(coreFldAsDate);
        CALL_RESOLVE(coreFldAsTime);
        CALL_RESOLVE(coreFldAsDateTime);

        CALL_RESOLVE(coreGetLastError);
        CALL_RESOLVE(coreGetErrorHandle);
        CALL_RESOLVE(coreGetErrorCount);
        CALL_RESOLVE(coreGetErrorText);
    }
    catch(std::exception &e)
    {
        qCritical(SqlRsdDriver()) << e.what();
        return false;
    }

    return true;
}
