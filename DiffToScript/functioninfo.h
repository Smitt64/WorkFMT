#ifndef FUNCTIONINFO_H
#define FUNCTIONINFO_H

#include <QObject>
#include "rslmodule/iterableobjectbase.h"

enum FunctionDirection
{
    FuncDirIn = 0,
    FuncDirOut,
    FuncDirInOut
};

class DbSpelling;
class FunctionParamInfo : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(FunctionParamInfo)
    Q_PROPERTY(int paramType READ paramType WRITE setParamType CONSTANT)
    Q_PROPERTY(bool isBlob READ isBlob WRITE setIsBlob CONSTANT)
    Q_PROPERTY(QString paramName READ paramName WRITE setParamName CONSTANT)
    Q_PROPERTY(QString userType READ userType WRITE setUserType CONSTANT)
    Q_PROPERTY(int direction READ direction WRITE setDirection CONSTANT)
public:
    explicit FunctionParamInfo(QObject *parent = nullptr);
    Q_INVOKABLE FunctionParamInfo(const QString &name, int type, QObject *parent = nullptr);

    int paramType() const;
    void setParamType(int type);

    QString paramName() const;
    void setParamName(const QString &name);

    int direction() const;
    void setDirection(const int &name);

    QString userType() const;
    void setUserType(const QString &type);

    bool isBlob() const;
    void setIsBlob(const bool &type);

    Q_INVOKABLE QString toString(DbSpelling *spelling, bool inout = true);

private:
    int m_ParamType, m_Direction;
    bool m_IsBlob;
    QString m_ParamName;
    QString m_UserType;
};

Q_DECLARE_OPAQUE_POINTER(FunctionParamInfo)

class FunctionParamInfoList: public ListIterableObject<FunctionParamInfo*, CachedObjectDeleter>
{
    Q_OBJECT
public:
    FunctionParamInfoList() = default;
    virtual ~FunctionParamInfoList() = default;

    Q_INVOKABLE QString toString(DbSpelling *spelling);
};

Q_DECLARE_OPAQUE_POINTER(FunctionParamInfoList)

// -------------------------------------------------------------------

class FunctionInfo : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(FunctionInfo)
    Q_PROPERTY(QString functionName READ functionName WRITE setFunctionName CONSTANT)
    Q_PROPERTY(FunctionParamInfo *returnType READ returnType CONSTANT)
    Q_PROPERTY(FunctionParamInfoList *inputParams READ inputParams CONSTANT)
public:
    Q_INVOKABLE explicit FunctionInfo(QObject *parent = nullptr);

    QString functionName() const;
    void setFunctionName(const QString &name);

    FunctionParamInfo *returnType();

    FunctionParamInfoList *inputParams();
    Q_INVOKABLE void addInputParam(const QString &name, int type, bool IsBlob = false);
    void addInputParam(FunctionParamInfo *param);

    Q_INVOKABLE void clearInputParams();
    Q_INVOKABLE QString toString(DbSpelling *spelling);
    Q_INVOKABLE void functionChunks(DbSpelling *spelling, QStringList &BeginCreateReplace, QStringList &EndCreateReplace);
    Q_INVOKABLE QString drop(DbSpelling *spelling);
private:
    QString m_FunctionName;
    FunctionParamInfo m_ReturnType;
    FunctionParamInfoList m_InputParams;
};

class FunctionInfoList: public ListIterableObject<FunctionInfo*, CachedObjectDeleter>
{
    Q_OBJECT
public:
    Q_INVOKABLE FunctionInfoList() = default;
    virtual ~FunctionInfoList() = default;
};

Q_DECLARE_OPAQUE_POINTER(FunctionInfoList)

#endif // FUNCTIONINFO_H
