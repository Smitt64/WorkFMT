#include "rsdsqlresult.h"
#include "rsddriver.h"
#include <QSqlField>
#include <QDebug>

RsdSqlResult::RsdSqlResult(const QSqlDriver *db) :
    QSqlResult(db),
    BaseErrorSetter<RsdSqlResult>(this)
{
    QSqlDriver *cdriver = const_cast<QSqlDriver*>(db);
    RsdDriver *drv = dynamic_cast<RsdDriver*>(cdriver);
    m_Cmd.reset(new RsdCommandEx(drv->connection()));
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
    return -1;
}

bool RsdSqlResult::reset(const QString &query)
{
    m_RecSet.reset();
    m_Cmd->clearParams();
    return m_Cmd->setCmdText(query.toLocal8Bit().data());
}

bool RsdSqlResult::fetch(int index)
{
    if (!m_RecSet)
        return false;

    qDebug() << m_RecSet->getRecCount();
    if (m_RecSet->getRecCount() < index)
        return false;

    return m_RecSet->move(index, RSDO_ABSOLUTE);
}

bool RsdSqlResult::fetchFirst()
{
    if (!m_RecSet)
        return false;

    return m_RecSet->moveFirst();
}

bool RsdSqlResult::fetchLast()
{
    if (!m_RecSet)
        return false;

    return m_RecSet->moveLast();
}

bool RsdSqlResult::fetchNext()
{
    if (!m_RecSet)
        return false;

    return m_RecSet->moveNext();
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
        m_RecSet.reset();
        m_Cmd->setCmdText(lastQuery().toLocal8Bit().data());

        int size = boundValueCount();
        for (int i = 0; i < size; i++)
        {
            QVariant value = boundValue(i);
            QSql::ParamType type = bindValueType(i);
            m_Cmd->bindValue(boundValueName(i), value, type);
        }

        m_Cmd->setNullConversion(true);
        //qDebug() << "QRSD: " << lastQuery();
        m_Cmd->execute();
        setSelect(true);
        setActive(true);

        //if (isSelect())
            m_RecSet.reset(new CRsdRecordset(*m_Cmd.data(), RSDVAL_CLIENT, RSDVAL_STATIC));
            m_RecSet->open();
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

QVariant RsdSqlResult::GetValueFromField(const CRsdField &cfld)
{
    QVariant retVal;
    CRsdField &fld = const_cast<CRsdField&>(cfld);

    if (!fld.isNull())
    {
        try
        {
            RSDValType_t fldType = fld.getType();
            bool isUnicodeField = fld.isUnicodeField();

            switch(fldType)
            {
            case RSDPT_BYTE:
                retVal = fld.AsRSDBYTE();
                break;
            case RSDPT_SHORT:
                retVal = fld.AsRSDSHORT();
                break;
            case RSDPT_LONG:
                retVal = static_cast<qint32>(fld.AsRSDLONG());
                break;
            case RSDPT_FLOAT:
                retVal = fld.AsRSDFLOAT();
                break;
            case RSDPT_DOUBLE:
                retVal = fld.AsRSDDOUBLE();
                break;
            case RSDPT_LPSTR:
                retVal = fld.AsRSDLPSTR();
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

QSqlField RsdSqlResult::MakeField(const CRsdField &fld)
{
    QSqlField qfld;
    QVariant value = GetValueFromField(fld);
    qfld.setName(fld.getName());
    qfld.setValue(value);

    return qfld;
}

QSqlRecord RsdSqlResult::record() const
{
    QSqlRecord rec;

    if (isSelect())
    {
        RsdSqlResult *pThis = const_cast<RsdSqlResult*>(this);
        quint32 fldcount = m_RecSet->getFldCount();

        for (int i = 0; i < fldcount; i++)
        {
            const CRsdField &fld = m_RecSet->getFld(i);
            rec.append(pThis->MakeField(fld));
            rec.setNull(i);
        }
    }

    return rec;
}
