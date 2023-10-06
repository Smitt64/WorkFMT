#include "rsdsqlresult.h"
#include "rsddriver.h"
#include <QSqlField>
#include <QRegularExpression>
#include <QDebug>

RsdSqlResult::RsdSqlResult(const QSqlDriver *db) :
    QSqlResult(db),
    BaseErrorSetter<RsdSqlResult>(this)
{
    QSqlDriver *cdriver = const_cast<QSqlDriver*>(db);
    m_Driver = dynamic_cast<RsdDriver*>(cdriver);
    m_Cmd.reset(new RsdCommandEx(m_Driver->connection(), m_Driver));
    //m_Cmd->setNullConversion(true);
}

RsdSqlResult::~RsdSqlResult()
{

}

QVariant RsdSqlResult::data(int index)
{
    CRsdField &fld = m_RecSet->getFld(index);
    QVariant value = GetValueFromField(fld);
    return value;
}

bool RsdSqlResult::isNull(int index)
{
    CRsdField &fld = m_RecSet->getFld(index);
    return fld.isNull();
}

int	RsdSqlResult::numRowsAffected()
{
    return -1;
}

int	RsdSqlResult::size()
{
    if (m_RecSet)
    {
        int sz = m_RecSet->getRecCount();
        return sz;
    }
    return -1;
}

bool RsdSqlResult::setCmdText(const QString &sql)
{
    if (!m_Cmd)
        return false;

    m_QueryString = QLatin1String(m_Driver->toOem866(sql));
    QSqlResult::setQuery(sql);
    return m_Cmd->setCmdText(m_QueryString.toLatin1().data());
}

bool RsdSqlResult::reset(const QString &query)
{
    bool result = true;
    m_RecSet.reset();
    m_Cmd->clearParams();
    result = setCmdText(query);

    /*if (result)
        result = exec();*/

    return result;
}

bool RsdSqlResult::fetch(int index)
{
    if (!m_RecSet || !isActive() || index < 0)
        return false;

    bool ok = true;
    if (isForwardOnly())
    {
        if (index < at())
            return false;

        while (ok && index > at())
            ok = fetchNext();

        return ok;
    }
    else
    {
        quint32 count = m_RecSet->getRecCount();
        if (index < count)
        {
            ok = m_RecSet->moveFirst();

            if (ok)
            {
                ok = m_RecSet->move(index, RSDO_RELATIVE);
                setAt(index);
            }

            return ok;
        }

        int atindex = at();
        if (atindex == QSql::BeforeFirstRow || atindex == QSql::AfterLastRow)
            ok = fetchFirst();

        if (index > atindex)
        {
            qDebug() << "fetch" << index << isForwardOnly();
            while (ok && index > at())
                ok = fetchNext();

            int pos = m_RecSet->getCurPos();

            if (!ok)
            {
                ok = m_RecSet->moveLast();
                setAt(pos - 1);
            }
            else
                setAt(pos);

            if (pos != index)
                return false;
        }
        else
        {
            ok = m_RecSet->moveFirst();

            if (ok)
            {
                ok = m_RecSet->move(index, RSDO_RELATIVE);
                setAt(index);
            }

            return ok;
        }
    }
    return true;
}

bool RsdSqlResult::fetchFirst()
{
    if (!m_RecSet || !isActive() || at() == 0)
        return false;

    bool result = m_RecSet->moveFirst();

    if (result)
        setAt(0);

    return result;
}

bool RsdSqlResult::fetchLast()
{
    if (!m_RecSet)
        return false;

    bool result = m_RecSet->moveFirst();
    while(result)
        result = fetchNext();

    return result;
}

bool RsdSqlResult::fetchNext()
{
    if (!m_RecSet || !isActive())
        return false;

    const int currentRow = m_RecSet->getCurPos();
    if (currentRow == QSql::BeforeFirstRow)
        return fetchFirst();
    if (currentRow == QSql::AfterLastRow)
        return false;

    bool result = m_RecSet->moveNext();

    if (result)
        setAt(currentRow + 1);
    else
        setAt(QSql::AfterLastRow);

    return result;
}

bool RsdSqlResult::fetchPrevious()
{
    if (!m_RecSet)
        return false;

    const int currentRow = at();
    bool result = m_RecSet->movePrev();

    if (result)
        setAt(currentRow - 1);
    else
        setAt(QSql::BeforeFirstRow);

    return result;
}

bool RsdSqlResult::checkWord(const QString &sql, const QString &word) const
{
    QRegularExpression rx(QString("\\b%1\\b").arg(word),  QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption);

    return sql.contains(rx);
}

bool RsdSqlResult::checkInsert(const QString &sql) const
{
    if (checkWord(sql, "insert") && (checkWord(sql, "into") || checkWord(sql, "all")))
        return true;
    return false;
}

bool RsdSqlResult::checkUpdate(const QString &sql) const
{
    if (checkWord(sql, "update") && checkWord(sql, "set"))
        return true;
    return false;
}

bool RsdSqlResult::checkDelete(const QString &sql) const
{
    if (checkWord(sql, "delete") && checkWord(sql, "from"))
        return true;
    return false;
}

bool RsdSqlResult::checkMerge(const QString &sql) const
{
    if (checkWord(sql, "merge") && checkWord(sql, "into"))
        return true;
    return false;
}

bool RsdSqlResult::checkCOMMENT(const QString &sql) const
{
    if (checkWord(sql, "COMMENT") && checkWord(sql, "ON"))
        return true;
    return false;
}

bool RsdSqlResult::checkAlter(const QString &sql) const
{
    static QStringList patterns = QStringList()
            << "CLUSTER" << "DATABASE" << "DIMENSION" << "DISKGROUP" << "FLASHBACK"
            << "FUNCTION" << "INDEX" << "JAVA" << "LIBRARY" << "MATERIALIZED" << "OPERATOR"
            << "PACKAGE" << "PROCEDURE" << "RESOURCE" << "ROLE" << "ROLLBACK" << "SEQUENCE"
            << "SESSION" << "SYSTEM" << "TABLE" << "TABLESPACE" << "TRIGGER" << "TYPE"
            << "USER" << "VIEW";

    if (checkWord(sql, "ALTER"))
    {
        for (const QString &str : qAsConst(patterns))
        {
            if (checkWord(sql, str))
                return true;
        }
    }
    return false;
}

bool RsdSqlResult::checkCall(const QString &sql) const
{
    return checkWord(sql.simplified().trimmed(), "CALL");
}

bool RsdSqlResult::checkCreate(const QString &sql) const
{
    static QStringList patterns = QStringList()
            << "CLUSTER" << "CONTEXT" << "CONTROLFILE" << "DATABASE" << "DIMENSION"
            << "DIRECTORY" << "DISKGROUP" << "EDITION" << "FLASHBACK" << "FUNCTION"
            << "index" << "indextype" << "JAVA" << "LIBRARY" << "MATERIALIZED"
            << "OPERATOR" << "OUTLINE" << "PACKAGE" << "PFILE" << "PROCEDURE"
            << "PROFILE" << "RESTORE" << "ROLE" << "ROLLBACK" << "SCHEMA"
            << "SEQUENCE" << "SPFILE" << "SYNONYM" << "TABLE" << "TRIGGER"
            << "TYPE" << "USER" << "VIEW";

    if (checkWord(sql, "CREATE"))
    {
        for (const QString &str : qAsConst(patterns))
        {
            if (checkWord(sql, str))
                return true;
        }
    }
    return false;
}

bool RsdSqlResult::checkDrop(const QString &sql) const
{
    static QStringList patterns = QStringList()
            << "CLUSTER" << "CONTEXT" << "CONTROLFILE" << "DATABASE" << "DIMENSION"
            << "DIRECTORY" << "DISKGROUP" << "EDITION" << "FLASHBACK" << "FUNCTION"
            << "index" << "indextype" << "JAVA" << "LIBRARY" << "MATERIALIZED"
            << "OPERATOR" << "OUTLINE" << "PACKAGE" << "PFILE" << "PROCEDURE"
            << "PROFILE" << "RESTORE" << "ROLE" << "ROLLBACK" << "SCHEMA"
            << "SEQUENCE" << "SPFILE" << "SYNONYM" << "TABLE" << "TRIGGER"
            << "TYPE" << "USER" << "VIEW";

    if (checkWord(sql, "DROP"))
    {
        for (const QString &str : qAsConst(patterns))
        {
            if (checkWord(sql, str))
                return true;
        }
    }
    return false;
}

bool RsdSqlResult::checkSelect(const QString &sql) const
{
    bool hr = false;

    hr = checkInsert(sql);
    if (!hr) hr = checkUpdate(sql);
    if (!hr) hr = checkDelete(sql);
    if (!hr) hr = checkMerge(sql);
    if (!hr) hr = checkDrop(sql);
    if (!hr) hr = checkCOMMENT(sql);
    if (!hr) hr = checkAlter(sql);
    if (!hr) hr = checkCall(sql);
    if (!hr) hr = checkCreate(sql);

    return !hr;
}

void RsdSqlResult::makeRecordSetFromCmd(QScopedPointer<RsdCommandEx> &cmd)
{
    try
    {
        m_RecSet.reset(new CRsdRecordset(*m_Cmd.data(), RSDVAL_CLIENT, RSDVAL_STATIC));
        m_RecSet->open();
        setSelect(true);
        if (m_RecSet->getCursorType() == RSDVAL_FORWARD_ONLY)
            setForwardOnly(true);
    }
    catch (...)
    {
        m_RecSet.reset();
        setSelect(false);
    }
}

bool RsdSqlResult::exec()
{
    bool result = true;

    setSelect(false);
    setActive(false);

    try
    {
        int size = boundValueCount();
        QSqlResult::BindingSyntax syntax = bindingSyntax();

        for (int i = 0; i < size; i++)
        {
            QVariant value = boundValue(i);
            QSql::ParamType type = bindValueType(i);

            if (syntax == QSqlResult::NamedBinding)
                m_Cmd->bindValue(boundValueName(i), value, type);
            else
                m_Cmd->bindValue(i, value, type);
        }

        QSqlResult::setQuery(m_QueryString);
        m_Cmd->execute();

        setActive(true);
        setAt(QSql::BeforeFirstRow);

        if (checkSelect(m_QueryString))
            makeRecordSetFromCmd(m_Cmd);
    }
    catch(XRsdError& e)
    {
        result = false;
        setLastRsdError(e, QSqlError::StatementError);
    }
    catch(...)
    {
        result = false;
        setLastUnforeseenError(QSqlError::StatementError);
    }

    return result;
}

bool RsdSqlResult::prepare(const QString &query)
{
    bool result = true;
    try
    {
        m_RecSet.reset();
        //m_QueryString = QLatin1String(m_Driver->toOem866(query));
        m_QueryString = query;

        QString str = QLatin1String(m_Driver->toOem866(query));
        m_Cmd->setCmdText(str.toLatin1());

        //m_Cmd->setNullConversion(true);
    }
    catch(XRsdError& e)
    {
        result = false;
        setLastRsdError(e, QSqlError::StatementError);
    }
    catch(...)
    {
        result = false;
        setLastUnforeseenError(QSqlError::StatementError);
    }

    return result;
}

QVariant RsdSqlResult::rsBinaryToVariantQByteArray(CRsdField &fld)
{
    Q_ASSERT(fld.getType() == RSDPT_BINARY);
    return QVariant(QByteArray(reinterpret_cast<const char*>(fld.AsRSDBINARY()), fld.getBufferLen()));
}

QVariant RsdSqlResult::rsCharToVariantQChar(CRsdField &fld)
{
    Q_ASSERT(fld.getType() == RSDPT_CHAR);

    char ch = fld.AsRSDCHAR();
    QTextCodec *codec866 = QTextCodec::codecForName("IBM 866");
    return QVariant(codec866->toUnicode(QByteArray(1, ch)).front());
}

QVariant RsdSqlResult::rsNumericToVariant(CRsdField &fld)
{
    Q_ASSERT(fld.getType() == RSDPT_NUMERIC);
    Numeric _rsNumeric = fld.AsRSDNUMERIC();

    QVariant retVal;
    retVal.setValue(Numeric96::convertToDouble(_rsNumeric));

    return retVal;
}

QVariant RsdSqlResult::GetValueFromField(const CRsdField &cfld)
{
    QVariant retVal;
    CRsdField &fld = const_cast<CRsdField&>(cfld);

    if (!fld.isNull())
    {
        try
        {
            RSDValType_t fldType = fld.getType();
            //bool isUnicodeField = fld.isUnicodeField();

            switch(fldType)
            {
            case RSDPT_BYTE:
                retVal = fld.AsRSDBYTE();
                break;
            case RSDPT_SHORT:
                retVal = fld.AsRSDSHORT();
                break;
            case RSDPT_USHORT:
                retVal.setValue(fld.AsRSDUSHORT());
                break;
            case RSDPT_LONG:
                retVal = static_cast<qint32>(fld.AsRSDLONG());
                break;
            case RSDPT_ULONG:
                retVal.setValue(fld.AsRSDULONG());
                break;
            case RSDPT_BIGINT:
                retVal.setValue(fld.AsRSDBIGINT());
                break;
            case RSDPT_UBIGINT:
                retVal.setValue(fld.AsRSDUBIGINT());
                break;
            case RSDPT_FLOAT:
                retVal = fld.AsRSDFLOAT();
                break;
            case RSDPT_DOUBLE:
                retVal = fld.AsRSDDOUBLE();
                break;
            case RSDPT_NUMERIC:
                retVal = rsNumericToVariant(fld);
                break;
            case RSDPT_LPSTR:
                retVal = m_Driver->fromOem866(QLatin1String(fld.AsRSDLPSTR()));
                break;
            case RSDPT_DATE:
                retVal = rsDateToQDate(fld.AsRSDDATE());
                break;
            case RSDPT_TIME:
                retVal = rsTimeToQTime(fld.AsRSDTIME());
                break;
            case RSDPT_TIMESTAMP:
                retVal = rsTimeStampToQDateTime(fld.AsRSDTIMESTAMP());
                break;
            case RSDPT_BINARY:
                retVal = rsBinaryToVariantQByteArray(fld);
                break;
            case RSDPT_CHAR:
                retVal = rsCharToVariantQChar(fld);
                break;
            }
        }
        catch(XRsdError& e)
        {
            setLastRsdError(e, QSqlError::StatementError);
        }
        catch(...)
        {
            setLastUnforeseenError(QSqlError::StatementError);
        }
    }

    return retVal;
}

QSqlField RsdSqlResult::MakeField(const CRsdField &fld, const bool &isnull)
{
    QSqlField qfld;
    QString fldname = fld.getName();
    qfld.setName(fldname);

    if (!isnull)
    {
        QVariant value = GetValueFromField(fld);
        qfld.setValue(value);
    }
    return qfld;
}

QSqlRecord RsdSqlResult::record() const
{
    QSqlRecord rec;

    if (isSelect() && isActive())
    {
        RsdSqlResult *pThis = const_cast<RsdSqlResult*>(this);
        long fldcount = m_RecSet->getFldCount();

        for (long i = 0; i < fldcount; i++)
        {
            const CRsdField &fld = m_RecSet->getFld(i);
            rec.append(pThis->MakeField(fld, fld.isNull()));
            rec.setGenerated(i, true);

            if (fld.isNull())
                rec.setNull(i);
        }
    }

    return rec;
}

void RsdSqlResult::setQuery(const QString &query)
{
    setCmdText(query);
}
