#ifndef DBBUILDER_H
#define DBBUILDER_H

#include <QObject>
#include <QString>
#include "diffcore.h"
#include "difffield.h"

class DbSpelling : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString chr READ chr CONSTANT)
    Q_PROPERTY(QString nvl READ nvl CONSTANT)
    Q_PROPERTY(bool needDropFunctions READ needDropFunctions CONSTANT)
    Q_PROPERTY(int functionDeclare READ functionDeclare CONSTANT)

    Q_PROPERTY(QStringList declare READ getDeclare CONSTANT)
    Q_PROPERTY(QStringList begin_ READ getBegin CONSTANT)
    Q_PROPERTY(QStringList end_ READ getEnd CONSTANT)
public:
    enum ExcceptionType
    {
        ExceptDupValOnIndex = 1,
        ExceptNoDataFound,
    };

    enum FunctionDeclarePos
    {
        FunctionBeforeBlocks = 0,
        FunctionInDeclareBlock,
        FunctionInBeginBlock,
    };

    Q_INVOKABLE virtual QString toDate(const QString& value);
    Q_INVOKABLE virtual QString toBlob(const QString& value);

    virtual QString chr();
    virtual QString nvl();

    Q_INVOKABLE virtual void setSpelling(const DiffFields& fields, QStringList& values);
    Q_INVOKABLE virtual QString callProcedure(const QString &proc);

    virtual bool needDropFunctions() const;
    Q_INVOKABLE virtual QString getProcKeyWord(const bool &rettype) = 0;
    Q_INVOKABLE virtual QString getProcReturnKeyWord(const QString &returntype) = 0;
    Q_INVOKABLE virtual QString dropFunction(const QString &fullproc) const;
    virtual int functionDeclare() const;

    Q_INVOKABLE virtual void functionChunks(QStringList &BeginCreateReplace,
                                QStringList &EndCreateReplace,
                                const QString &name,
                                const QString &params,
                                const QString &returnType) = 0;

    virtual QStringList getDeclare() = 0;
    virtual QStringList getBegin() = 0;
    virtual QStringList getEnd() = 0;


    Q_INVOKABLE virtual QString blobTypeName(const int &type) = 0;
    Q_INVOKABLE virtual QString functionParamType(const qint16 &type) = 0;
    Q_INVOKABLE virtual QString getExceptionName(const int &type) = 0;

    Q_INVOKABLE virtual QString getDefaultValueForType(const qint16 &Type, const int &size) = 0;
    Q_INVOKABLE virtual QStringList getExceptionInfo(const QString &varname) = 0;

protected:
    virtual QString funcDeclType(const QString& decl);
};

#endif // DBBUILDER_H
