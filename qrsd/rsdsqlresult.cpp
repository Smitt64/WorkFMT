#include "rsdsqlresult.h"
#include "rsddriver.h"
#include <QSqlField>
#include <QDebug>

RsdSqlResult::RsdSqlResult(const QSqlDriver *db) :
    QSqlResult(db),
    BaseErrorSetter<RsdSqlResult>(this)
{
    QSqlDriver *cdriver = const_cast<QSqlDriver*>(db);
    m_Driver = dynamic_cast<RsdDriver*>(cdriver);
    m_Cmd.reset(new RsdCommandEx(m_Driver->connection()));
}

RsdSqlResult::~RsdSqlResult()
{

}

QVariant RsdSqlResult::data(int index)
{
    CRsdField &fld = m_RecSet->getFld(index);
    return GetValueFromField(fld);
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
    return m_RecSet->getRecCount();
}

bool RsdSqlResult::setCmdText(const QString &sql)
{
    if (!m_Cmd)
        return false;

    m_QueryString = m_Driver->toOem866(sql);
    return m_Cmd->setCmdText(m_QueryString.toLocal8Bit().data());
}

bool RsdSqlResult::reset(const QString &query)
{
    bool result = true;
    m_RecSet.reset();
    m_Cmd->clearParams();
    result = setCmdText(query);

    if (result)
        result = exec();

    return result;
}

bool RsdSqlResult::fetch(int index)
{
    if (!m_RecSet || !isActive() || index < 0 || at() == index)
        return false;

    //qDebug() << m_RecSet->getRecCount();
    /*if (m_RecSet->getRecCount() < index)
        return false;*/

    //bool result = m_RecSet->move(index, RSDO_ABSOLUTE);

    /*if (result)
        setAt(index);

    return result;*/
    if (isForwardOnly())
    {
        if (index < at())
            return false;

        bool ok = true;
        while (ok && index > at())
            ok = fetchNext();

        return ok;
    }

    setAt(index);
    return true;
}

bool RsdSqlResult::fetchFirst()
{
    if (!m_RecSet || !isActive() || at() == 0)
        return false;

    /*if (isForwardOnly())
    {
        if(at() == QSql::BeforeFirstRow)
            setAt(0);
    }*/

    //return /*m_RecSet->moveFirst()*/fetch(0);
    bool result = m_RecSet->moveFirst();

    if (result)
        setAt(0);

    return result;
}

bool RsdSqlResult::fetchLast()
{
    if (!m_RecSet)
        return false;

    return m_RecSet->moveLast();
}

bool RsdSqlResult::fetchNext()
{
    if (!m_RecSet || !isActive())
        return false;

    const int currentRow = at();
    if (currentRow == QSql::BeforeFirstRow)
        return fetchFirst();
    if (currentRow == QSql::AfterLastRow)
        return false;

    /*if (isForwardOnly())
    {

    }*/
    bool result = m_RecSet->moveNext();

    if (result)
        setAt(currentRow + 1);

    return result;
    //return m_RecSet->moveNext();
}

bool RsdSqlResult::fetchPrevious()
{
    if (!m_RecSet)
        return false;

    return m_RecSet->movePrev();
}

/*void RsdSqlResult::bindValue(const QString &placeholder, const QVariant &val, QSql::ParamType paramType)
{
    m_Cmd->bindValue(placeholder, val, paramType);
}

void RsdSqlResult::bindValue(int index, const QVariant &val, QSql::ParamType paramType)
{
    m_Cmd->bindValue(index, val, paramType);
}*/

bool RsdSqlResult::exec()
{
    bool result = true;

    try
    {
        int size = boundValueCount();
        QSqlResult::BindingSyntax syntax = bindingSyntax();
        qDebug() << syntax;
        for (int i = 0; i < size; i++)
        {
            QVariant value = boundValue(i);
            QSql::ParamType type = bindValueType(i);

            if (syntax == QSqlResult::NamedBinding)
                m_Cmd->bindValue(boundValueName(i), value, type);
            else
                m_Cmd->bindValue(i, value, type);
        }
        //qDebug() << "QRSD: " << lastQuery();
        m_Cmd->execute();
        setSelect(true);
        setActive(true);

        //if (isSelect())
            m_RecSet.reset(new CRsdRecordset(*m_Cmd.data(), RSDVAL_CLIENT, RSDVAL_STATIC));
            m_RecSet->open();
            if (m_RecSet->getCursorType() == RSDVAL_FORWARD_ONLY)
                setForwardOnly(true);
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
        m_QueryString = query;
        m_Cmd->setCmdText(m_QueryString.toLocal8Bit().data());

        m_Cmd->setNullConversion(true);
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

QDate RsdSqlResult::rsDateToQDate(const bdate &_rsDate)
{
    if (_rsDate.day == 0 && _rsDate.mon == 0 && _rsDate.year == 0)
        return QDate();

    return QDate(_rsDate.year, _rsDate.mon, _rsDate.day);
}

QTime RsdSqlResult::rsTimeToQTime(const btime &_rsTime)
{
    return QTime(_rsTime.hour, _rsTime.min, _rsTime.sec, _rsTime.hundr);
}

QVariant RsdSqlResult::rsDateToVariantQDate(CRsdField &fld)
{
    Q_ASSERT(fld.getType() == RSDPT_DATE);
    const bdate &_rsDate = fld.AsRSDDATE();

    return rsDateToQDate(_rsDate);
}

QVariant RsdSqlResult::rsTimeToVariantQTime(CRsdField &fld)
{
    Q_ASSERT(fld.getType() == RSDPT_TIME);
    btime _rsTime = fld.AsRSDTIME();
    return QVariant(QTime(_rsTime.hour, _rsTime.min, _rsTime.sec, _rsTime.hundr));
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

QVariant RsdSqlResult::rsTimeStampToVariantQDateTime(CRsdField &fld)
{
    Q_ASSERT(fld.getType() == RSDPT_TIMESTAMP);
    btimestamp _rsTimeStamp = fld.AsRSDTIMESTAMP();

    QVariant retVal;
    QDate _rsDate = rsDateToQDate(_rsTimeStamp.date);
    QTime _rsTime = rsTimeToQTime(_rsTimeStamp.time);

    return QDateTime(_rsDate, _rsTime);
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
                retVal = rsDateToVariantQDate(fld);
                break;
            case RSDPT_TIME:
                retVal = rsTimeToVariantQTime(fld);
                break;
            case RSDPT_TIMESTAMP:
                retVal = rsTimeStampToVariantQDateTime(fld);
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
    qfld.setName(fld.getName());

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

    if (isSelect())
    {
        RsdSqlResult *pThis = const_cast<RsdSqlResult*>(this);
        long fldcount = m_RecSet->getFldCount();

        for (long i = 0; i < fldcount; i++)
        {
            const CRsdField &fld = m_RecSet->getFld(i);
            rec.append(pThis->MakeField(fld, fld.isNull()));

            if (fld.isNull())
                rec.setNull(i);
        }
    }

    return rec;
}

void RsdSqlResult::setQuery(const QString &query)
{
    //m_QueryString = query;
    setCmdText(query);
}
