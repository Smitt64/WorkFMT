#ifndef DBBUILDER_H
#define DBBUILDER_H

#include <QObject>
#include <QString>
#include "diffcore.h"
#include "difffield.h"

class DbSpelling : public QObject
{
    Q_OBJECT
public:
    enum ExcceptionType
    {
        ExceptDupValOnIndex = 1,
    };

    enum FunctionDeclarePos
    {
        FunctionBeforeBlocks = 0,
        FunctionInDeclareBlock,
        FunctionInBeginBlock,
    };

    Q_INVOKABLE virtual QString toDate(const QString& value);
    Q_INVOKABLE virtual QString toBlob(const QString& value);
    Q_INVOKABLE virtual QString chr();
    Q_INVOKABLE virtual QString nvl();
    Q_INVOKABLE virtual void setSpelling(const DiffFields& fields, QStringList& values);
    Q_INVOKABLE virtual QString callProcedure(const QString &proc);

    Q_INVOKABLE virtual bool needDropFunctions() const;
    Q_INVOKABLE virtual QString dropFunction(const QString &proc, const QString &fullproc, const QString &returnType) const;
    Q_INVOKABLE virtual int functionDeclare() const;

    Q_INVOKABLE virtual void functionChunks(QStringList &BeginCreateReplace,
                                QStringList &EndCreateReplace,
                                const QString &name,
                                const QString &params,
                                const QString &returnType) = 0;

    Q_INVOKABLE virtual QStringList getDeclare() = 0;
    Q_INVOKABLE virtual QStringList getBegin() = 0;
    Q_INVOKABLE virtual QStringList getEnd() = 0;

    Q_INVOKABLE virtual QString blobTypeName(const int &type) = 0;
    Q_INVOKABLE virtual QString functionParamType(const qint16 &type) = 0;
    Q_INVOKABLE virtual QString getExceptionName(const int &type) = 0;

    Q_INVOKABLE virtual QString getDefaultValueForType(const qint16 &Type, const int &size) = 0;
    Q_INVOKABLE virtual QStringList getExceptionInfo(const QString &varname) = 0;

protected:
    virtual QString funcDeclType(const QString& decl);
};

#endif // DBBUILDER_H
