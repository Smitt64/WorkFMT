#include "rsdsqlresult.h"
#include "rsddriver.h"
#include <stdlib.h>
#include <QSqlField>
#include <QRegularExpression>
#include <QDebug>

#define BLOB_MAX_LEN 32768

RsdSqlResult::RsdSqlResult(const QSqlDriver *db) :
    QSqlResult(db),
    BaseErrorSetter<RsdSqlResult>(this),
    m_Cmd(nullptr),
    m_RecSet(nullptr)
{
    QSqlDriver *cdriver = const_cast<QSqlDriver*>(db);
    m_Driver = dynamic_cast<RsdDriver*>(cdriver);

    m_Cmd.reset(new RsdCommandEx(m_Driver->connection(), m_Driver));
    //m_Cmd->setNullConversion(true);
    m_Driver->addResult(this);
    setUnicode(m_Driver->isUnicode());
}

RsdSqlResult::~RsdSqlResult()
{
    m_Driver->removeResult(this);
}

QVariant RsdSqlResult::data(int index)
{
    QSqlRecord rec = record();
    return rec.value(index);
}

bool RsdSqlResult::isNull(int index)
{
    if (!m_RecSet)
        return true;

    bool result = false;
    Qt::HANDLE handle = nullptr;
    coreRecGetFld(m_RecSet, index, &handle);
    result = coreRecIsNull(handle);
    coreFreeHandle(&handle);

    return result;
}

int	RsdSqlResult::numRowsAffected()
{
    return -1;
}

int	RsdSqlResult::size()
{
    if (!m_RecSet)
        return -1;

    return coreRecCount(m_RecSet);
}

bool RsdSqlResult::setCmdText(const QString &sql)
{
    if (!m_Cmd)
        return false;

    QSqlResult::setQuery(sql);
    m_QueryString = QLatin1String(m_Driver->toOem866(sql));
    return m_Cmd->setCmdText(m_QueryString.toLatin1().data());
}

bool RsdSqlResult::reset(const QString &query)
{
    bool result = true;
    m_Cmd->clearParams();
    setAt(QSql::BeforeFirstRow);
    result = setCmdText(query);

    if (m_RecSet)
        coreFreeHandle(&m_RecSet);

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
        {
            ok = fetchNext();

            if (ok)
                MakeRecord();
        }

        return ok;
    }
    else
    {
        quint32 count = coreRecCount(m_RecSet);
        if (index < count)
        {
            ok = coreRecFirst(m_RecSet);

            if (ok)
            {
                ok = coreRecMove(m_RecSet, index, RSDO_RELATIVE);
                setAt(index);

                if (ok)
                    MakeRecord();
            }

            return ok;
        }

        int atindex = at();
        if (atindex == QSql::BeforeFirstRow || atindex == QSql::AfterLastRow)
            ok = fetchFirst();

        if (index > atindex)
        {
            while (ok && index > at())
                ok = fetchNext();

            int pos = coreRecCurPos(m_RecSet);

            if (!ok)
            {
                ok = coreRecLast(m_RecSet);
                setAt(pos - 1);
            }
            else
            {
                setAt(pos);
                MakeRecord();
            }

            if (pos != index)
                return false;
        }
        else
        {
            ok = coreRecFirst(m_RecSet);

            if (ok)
            {
                ok = coreRecMove(m_RecSet, index, RSDO_RELATIVE);
                setAt(index);
                MakeRecord();
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

    bool result = false;
    if (at() == QSql::BeforeFirstRow)
        result = coreRecNext(m_RecSet);
    else
        result = coreRecFirst(m_RecSet);

    if (result)
    {
        setAt(0);
        MakeRecord();
    }

    return result;
}

bool RsdSqlResult::fetchLast()
{
    if (!m_RecSet)
        return false;

    bool result = coreRecFirst(m_RecSet);
    while(result)
    {
        result = fetchNext();

        if (result)
            MakeRecord();
    }

    return result;
}

bool RsdSqlResult::fetchNext()
{
    if (!m_RecSet || !isActive())
        return false;

    const int currentRow = coreRecCurPos(m_RecSet);
    if (currentRow == QSql::BeforeFirstRow)
        return fetchFirst();
    if (currentRow == QSql::AfterLastRow)
        return false;

    bool result = coreRecNext(m_RecSet);

    if (result)
    {
        setAt(currentRow + 1);
        MakeRecord();
    }
    else
        setAt(QSql::AfterLastRow);

    return result;
}

bool RsdSqlResult::fetchPrevious()
{
    if (!m_RecSet)
        return false;

    const int currentRow = at();
    bool result = coreRecPrev(m_RecSet);

    if (result)
    {
        setAt(currentRow - 1);
        MakeRecord();
    }
    else
        setAt(QSql::BeforeFirstRow);

    return result;
}

bool RsdSqlResult::makeRecordSetFromCmd(QScopedPointer<RsdCommandEx> &cmd)
{
    setAt(QSql::BeforeFirstRow);
    coreRecInit(&m_RecSet, m_Cmd->handle(), isForwardOnly());

    if (!coreRecIsNull(m_RecSet))
    {
        setSelect(true);
        setActive(true);

        return true;
    }
    else
        coreFreeHandle(&m_RecSet);

    return false;
}

QStringList RsdSqlResult::GetSqlHints(const QString &sql)
{
    QStringList hints;
    QRegularExpression re = QRegularExpression("\\/\\*[@](.*)\\*\\/", QRegularExpression::MultilineOption
                                               | QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatch match = re.match(sql);
    if (match.hasMatch())
    {
        QString matched = match.captured(1);
        hints = matched.split(';');
    }

    return hints;
}

QVariant RsdSqlResult::HintValue(const QString &hintparam)
{
    QVariant val;
    bool isOk = true;
    int value = hintparam.toInt(&isOk);

    if (isOk)
        val = value;
    else
    {
        qreal realval = hintparam.toDouble(&isOk);

        if (isOk)
            val = realval;
    }

    if (!isOk)
    {
        if (!hintparam.compare("true", Qt::CaseInsensitive))
            val = true;
        else if (!hintparam.compare("on", Qt::CaseInsensitive))
            val = true;
        else if (!hintparam.compare("false", Qt::CaseInsensitive))
            val = false;
        else if (!hintparam.compare("off", Qt::CaseInsensitive))
            val = false;
    }

    if (!val.isValid())
        val = hintparam;

    return val;
}

bool RsdSqlResult::HasHint(const QStringList &hints, const QString &hint, QStringList *params)
{
    bool Exists = false;
    QRegularExpression re = QRegularExpression("(.*)\\((.*)\\)", QRegularExpression::MultilineOption
                                               | QRegularExpression::CaseInsensitiveOption);

    //QRegularExpressionMatch match = re.match(hint);
    for(const QString &value : hints)
    {
        if (!value.trimmed().compare(hint, Qt::CaseInsensitive))
        {
            QRegularExpressionMatch match = re.match(value);

            if (params && !match.captured(2).isEmpty())
                *params = match.captured(2).split(',');

            Exists = true;
            break;
        }
    }

    if (params)
    {
        for (int i = 0; i < params->size(); i++)
            (*params)[i] = (*params)[i].trimmed();
    }

    return Exists;
}

bool RsdSqlResult::exec()
{
    bool result = true;
    setSelect(false);
    setActive(false);

    int size = boundValueCount();
    QSqlResult::BindingSyntax syntax = bindingSyntax();

    for (int i = 0; i < size; i++)
    {
        QVariant value = boundValue(i);
        QSql::ParamType type = bindValueType(i);

        if (syntax == QSqlResult::NamedBinding)
        {
            QString bindName = boundValueName(i).mid(1);
            m_Cmd->bindValue(bindName, value, type);
        }
        else
           m_Cmd->bindValue(i, value, type);
    }

    QSqlResult::setQuery(m_QueryString);
    setCmdText(m_QueryString);

    QStringList params;
    QStringList hints = GetSqlHints(m_QueryString);
    if (HasHint(hints, "DisConv", &params))
    {
        if (params.isEmpty())
            coreCmdDisableConverter(m_Cmd->handle(), true);
        else
        {
            QVariant val = HintValue(params.front());

            if (val.type() == QVariant::Bool)
                coreCmdDisableConverter(m_Cmd->handle(), val.toBool());
        }

    }

    result = RSD_SUCCEEDED(m_Cmd->execute());

    setActive(true);
    setAt(QSql::BeforeFirstRow);

    if (result)
        makeRecordSetFromCmd(m_Cmd);
    else
    {
        setLastRsdError(m_Cmd->handle());
    }

    return result;
}

bool RsdSqlResult::prepare(const QString &query)
{
    if (!m_Cmd)
        return false;

    QString str = QLatin1String(m_Driver->toOem866(query));
    m_Cmd->setCmdText(str.toLatin1());
    m_QueryString = query;

    return true;
}

QVariant RsdSqlResult::rsBinaryToVariantQByteArray(Qt::HANDLE fld)
{
    Q_ASSERT(coreFldType(fld) == RSDPT_BINARY);
    QByteArray data(reinterpret_cast<const char*>(coreFldAsBinary(fld)),
                     coreFldBufferLen(fld));
    return QVariant(data);
}

QVariant RsdSqlResult::rsCharToVariantQChar(Qt::HANDLE fld)
{
    Q_ASSERT(coreFldType(fld) == RSDPT_CHAR);

    char ch = coreFldAsChar(fld);
    QTextCodec *codec866 = QTextCodec::codecForName("IBM 866");
    return QVariant(codec866->toUnicode(QByteArray(1, ch)).front());
}

QVariant RsdSqlResult::GetValueFromField(Qt::HANDLE fld, const int &loblen)
{
    QVariant retVal;

    if (coreFldIsNull(fld) && !loblen)
        return QVariant();

    RSDValType_t fldType = (RSDValType_t)coreFldType(fld);
    QString name = coreFldName(fld);

    switch(fldType)
    {
    case RSDPT_BYTE:
        retVal = coreFldAsByte(fld);
        break;
    case RSDPT_SHORT:
        retVal = coreFldAsShort(fld);
        break;
    case RSDPT_USHORT:
        retVal.setValue(coreFldAsUShort(fld));
        break;
    case RSDPT_LONG:
        retVal = static_cast<qint32>(coreFldAsLong(fld));
        break;
    case RSDPT_ULONG:
        retVal.setValue(coreFldAsULong(fld));
        break;
    case RSDPT_BIGINT:
        retVal.setValue(coreFldAsBigInt(fld));
        break;
    case RSDPT_UBIGINT:
        retVal.setValue(coreFldAsBigInt(fld));
        break;
    case RSDPT_FLOAT:
        retVal = coreFldAsFloat(fld);
        break;
    case RSDPT_DOUBLE:
        retVal = coreFldAsDouble(fld);
        break;
    case RSDPT_NUMERIC:
        retVal = coreFldAsDouble(fld);
        break;
    case RSDPT_LPSTR:
        retVal = m_Driver->fromOem866(QLatin1String(coreFldAsString(fld)));
        break;
    case RSDPT_DATE:
    {
        bdate _rsdate;
        coreFldAsDate(fld, &_rsdate);
        retVal = rsDateToQDate(_rsdate);
    }
        break;
    case RSDPT_TIME:
    {
        btime _rstime;
        coreFldAsTime(fld, &_rstime);
        retVal = rsTimeToQTime(_rstime);
    }
        break;
    case RSDPT_TIMESTAMP:
    {
        btimestamp _rstimestamp;
        coreFldAsDateTime(fld, &_rstimestamp);
        retVal = rsTimeStampToQDateTime(_rstimestamp);
    }
        break;
    case RSDPT_BINARY:
        retVal = rsBinaryToVariantQByteArray(fld);
        break;
    case RSDPT_CHAR:
        retVal = rsCharToVariantQChar(fld);
        break;
    case RSDPT_CFILE:
    {
        if (loblen)
        {
            char *buf = (char*)_alloca(loblen + 1);
            memset(buf, 0, loblen + 1);
            coreFldRead(fld, buf, loblen);

            QString data = m_Driver->fromOem866(buf);
            retVal = data;

            free(buf);
        }
    }
        break;
    case RSDPT_BFILE:
    {
        if (loblen)
        {
            QByteArray data(loblen, '\0');
            coreFldRead(fld, data.data(), loblen);
            retVal = data;
        }
    }
        break;
    }

    return retVal;
}

QSqlField RsdSqlResult::MakeField(Qt::HANDLE fld, const bool &isnull, const int &loblen)
{
    QSqlField qfld;
    QString fldname = coreFldName(fld);
    qfld.setName(fldname);

    if (!isnull || loblen)
    {
        QVariant value = GetValueFromField(fld, loblen);

        if (value.isValid())
            qfld.setValue(value);
    }
    return qfld;
}

void RsdSqlResult::MakeRecord()
{
    m_Record.reset(new QSqlRecord());

    if (isSelect() && isActive() && m_RecSet)
    {
        long fldcount = coreRecFldCount(m_RecSet);

        for (long i = 0; i < fldcount; i++)
        {
            Qt::HANDLE fld = nullptr;
            coreRecGetFld(m_RecSet, i, &fld);
            QString name = coreFldName(fld);

            int Type = coreFldType(fld);
            if (Type == RSDPT_CFILE || Type == RSDPT_BFILE)
            {
                QString fldName = name;
                QString fldNameLen = fldName + "_LOBLEN";

                int pos = -1;
                for (long k = 0; k < coreRecFldCount(m_RecSet); k++)
                {
                    Qt::HANDLE fld2 = nullptr;
                    coreRecGetFld(m_RecSet, k, &fld2);
                    QString indexedName = coreFldName(fld2);
                    if (!indexedName.compare(fldNameLen, Qt::CaseInsensitive))
                    {
                        pos = k;
                        coreFreeHandle(&fld2);
                        break;
                    }
                    coreFreeHandle(&fld2);
                }

                if (pos != -1)
                {
                    Qt::HANDLE fld2 = nullptr;
                    coreRecGetFld(m_RecSet, pos, &fld2);
                    int len = GetValueFromField(fld2, 0).toInt();
                    coreFreeHandle(&fld2);

                    m_Record->append(MakeField(fld, false, len));
                    m_Record->setGenerated(i, true);
                }
                else
                {
                    m_Record->append(MakeField(fld, false, BLOB_MAX_LEN));
                    m_Record->setGenerated(i, true);
                }
            }
            else
            {
                bool _isNull = coreFldIsNull(fld);
                m_Record->append(MakeField(fld, _isNull, 0));
                m_Record->setGenerated(i, true);

                if (_isNull)
                    m_Record->setNull(i);
            }
        }
    }
}

QSqlRecord RsdSqlResult::record() const
{
    if (!m_RecSet)
        return QSqlRecord();

    if (m_Record)
        return *m_Record;

    long fldcount = coreRecFldCount(m_RecSet);

    QSqlRecord rec;
    for (long i = 0; i < fldcount; i++)
    {
        Qt::HANDLE fld = nullptr;
        RsdSqlResult *pThis = const_cast<RsdSqlResult*>(this);
        coreRecGetFld(m_RecSet, i, &fld);

        bool _isNull = coreFldIsNull(fld);
        rec.append(pThis->MakeField(fld, _isNull, 0));
        rec.setGenerated(i, true);

        if (_isNull)
            rec.setNull(i);

        coreFreeHandle(&fld);
    }

    return rec;
}

void RsdSqlResult::setQuery(const QString &query)
{
    setCmdText(query);
}

void RsdSqlResult::onBeforeCloseConnection()
{
    m_Cmd.reset();
}
