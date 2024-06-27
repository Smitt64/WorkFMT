#ifndef DBBUILDER_H
#define DBBUILDER_H

#include <QString>
#include "diffcore.h"
#include "difffield.h"

class DbSpelling
{
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

    virtual QString toDate(const QString& value);
    virtual QString toBlob(const QString& value);
    virtual QString chr();
    virtual QString nvl();
    virtual void setSpelling(const DiffFields& fields, QStringList& values);
    virtual QStringList getDeclare() = 0;
    virtual QStringList getBegin() = 0;
    virtual QStringList getEnd() = 0;
    virtual QString callProcedure(const QString &proc);
    virtual QString blobTypeName(const int &type) = 0;

    virtual QString getExceptionName(const ExcceptionType &type) = 0;

    virtual bool needDropFunctions() const;
    virtual QString dropFunction(const QString &proc, const QString &returnType) const;
    virtual FunctionDeclarePos functionDeclare() const;
    virtual QString functionParamType(const qint16 &type) = 0;
    virtual void functionChunks(QStringList &BeginCreateReplace,
                                QStringList &EndCreateReplace,
                                const QString &name,
                                const QString &params,
                                const QString &returnType) = 0;

    virtual QString getDefaultValueForType(const qint16 &Type, const int &size) = 0;

protected:
    virtual QString funcDeclType(const QString& decl);
};

#endif // DBBUILDER_H
