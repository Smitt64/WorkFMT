#include "functioninfo.h"
#include "dbspelling.h"

FunctionParamInfo::FunctionParamInfo(QObject *parent)
    : QObject(parent), m_ParamType(0), m_IsBlob(false), m_Direction(FuncDirIn)
{
}

FunctionParamInfo::FunctionParamInfo(const QString &name, int type, QObject *parent)
    : QObject(parent), m_ParamType(type), m_ParamName(name), m_IsBlob(false), m_Direction(FuncDirIn)
{
}

int FunctionParamInfo::paramType() const
{
    return m_ParamType;
}

void FunctionParamInfo::setParamType(int type)
{
    if (m_ParamType != type)
        m_ParamType = type;
}

QString FunctionParamInfo::paramName() const
{
    return m_ParamName;
}

void FunctionParamInfo::setParamName(const QString &name)
{
    if (m_ParamName != name)
        m_ParamName = name;
}

int FunctionParamInfo::direction() const
{
    return m_Direction;
}

void FunctionParamInfo::setDirection(const int &dir)
{
    m_Direction = dir;
}

QString FunctionParamInfo::userType() const
{
    return m_UserType;
}

void FunctionParamInfo::setUserType(const QString &type)
{
    if (m_UserType != type)
        m_UserType = type;
}

bool FunctionParamInfo::isBlob() const
{
    return m_IsBlob;
}

void FunctionParamInfo::setIsBlob(const bool &type)
{
    m_IsBlob = type;
}

QString FunctionParamInfo::toString(DbSpelling *spelling, bool inout)
{
    QString result = m_ParamName;

    if (inout)
    {
        switch(m_Direction)
        {
        case FuncDirIn:
            result += QString(" IN");
            break;
        case FuncDirOut:
            result += QString(" OUT");
            break;
        case FuncDirInOut:
            result += QString(" INOUT");
            break;
        }
    }

    if (!m_UserType.isEmpty())
        result += " " + m_UserType;
    else
    {
        result += " ";

        if (m_IsBlob)
            result += spelling->blobTypeName(m_ParamType);
        else
            result += spelling->functionParamType(m_ParamType);
    }

    return result;
}

// -------------------------------------------------------

QString FunctionParamInfoList::toString(DbSpelling *spelling)
{
    QStringList result;

    for (auto prm : *this)
        result.append(prm->toString(spelling));

    return result.join(", ");
}

// -------------------------------------------------------

FunctionInfo::FunctionInfo(QObject *parent)
    : QObject(parent), m_ReturnType(0)
{
}

QString FunctionInfo::functionName() const
{
    return m_FunctionName;
}

void FunctionInfo::setFunctionName(const QString &name)
{
    if (m_FunctionName != name)
        m_FunctionName = name;
}

QString FunctionInfo::scheme() const
{
    return m_Scheme;
}

void FunctionInfo::setScheme(const QString &name)
{
    if (m_Scheme != name)
        m_Scheme = name;
}

FunctionParamInfo *FunctionInfo::returnType()
{
    return &m_ReturnType;
}

FunctionParamInfoList *FunctionInfo::inputParams()
{
    return &m_InputParams;
}

void FunctionInfo::addInputParam(const QString &name, int type, bool IsBlob)
{
    auto param = new FunctionParamInfo(name, type, this);
    param->setIsBlob(IsBlob);
    addInputParam(param);
}

void FunctionInfo::addInputParam(FunctionParamInfo *param)
{
    if (param && !m_InputParams.contains(param))
        m_InputParams.append(param);
}

void FunctionInfo::clearInputParams()
{
    qDeleteAll(m_InputParams);
    m_InputParams.clear();
}

void FunctionInfo::functionChunks(DbSpelling *spelling, QStringList &BeginCreateReplace, QStringList &EndCreateReplace)
{
    QString returnType;
    QString params = m_InputParams.toString(spelling);

    if (!m_ReturnType.userType().isEmpty())
        returnType = spelling->getProcReturnKeyWord(m_ReturnType.userType());
    else if (m_ReturnType.paramType())
        returnType = m_ReturnType.toString(spelling, false);

    spelling->functionChunks(BeginCreateReplace, EndCreateReplace, m_FunctionName, params, returnType);
}

QString FunctionInfo::drop(DbSpelling *spelling, const QString &scheme)
{
    QString fullname = toString(spelling, scheme);
    return spelling->dropFunction(fullname);
}

QString FunctionInfo::toString(DbSpelling *spelling, const QString &scheme)
{
    QString declaration;

    bool retType = !m_ReturnType.userType().isEmpty() || m_ReturnType.paramType();
    declaration += spelling->getProcKeyWord(retType);

    if (!m_Scheme.isEmpty() || !scheme.isEmpty())
    {
        if (!scheme.isEmpty())
            declaration += scheme + ".";
        else
        {
            if (!m_Scheme.isEmpty())
                declaration += m_Scheme + ".";
        }
    }

    declaration += m_FunctionName + " (" + m_InputParams.toString(spelling) + ") ";

    if (!m_ReturnType.userType().isEmpty())
        declaration += spelling->getProcReturnKeyWord(m_ReturnType.userType());
    else if (m_ReturnType.paramType())
    {
        QString retType = m_ReturnType.toString(spelling, false);
        declaration += spelling->getProcReturnKeyWord(retType);
    }

    return declaration;
}
