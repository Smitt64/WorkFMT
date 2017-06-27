#ifndef FMTDBGHELP_H
#define FMTDBGHELP_H

#include "fmtdbghelp_global.h"
#include <QtCore>

#ifdef Q_OS_WIN
#include <Windows.h>
class FMTDBGHELPSHARED_EXPORT FmtDbgHelp : public QObject
{
    Q_OBJECT
public:
    FmtDbgHelp(QObject *parent = NULL);
    virtual ~FmtDbgHelp();

public slots:
    bool WritMiniDump(const QString &dumpFileName, void *pExInfo);
    QString ExceptionCodeShort(const int &ExceptionCode);
    QString ExceptionCodeText(const int &ExceptionCode);
    void GetExceptionInfo(void *pExInfo, QString &module, QString &function, QString &file, int &line);
    //void RetrieveCallStack(QStringList *stackList);

private:
    int GetSymbolNameByAddrPrivate(DWORD64 ExceptionAddress, void *pSymbol);
    QMap<int,QString> m_ExceptionCodes, m_ExceptionText;
    HANDLE hProcess;
};

extern "C" FMTDBGHELPSHARED_EXPORT FmtDbgHelp *CreateDbgHelpObject(QObject *parent);

#endif

#endif // FMTDBGHELP_H
