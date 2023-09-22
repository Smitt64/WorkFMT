#include "fmteditcontentmodel.h"
#include "fmtfield.h"
#include "connectioninfo.h"
#include "fmtcore.h"
#include <QGuiApplication>

#define ROW_ID_FLD_NAME "row_id"
#define CELL_NULL_STR "<NULL>"
#define NORMILIZED_ROLE Qt::UserRole + 1

bool QModelIndexLessThanRow(const QModelIndex &v1, const QModelIndex &v2)
{
    return v1.row() < v2.row();
}

bool QModelIndexGrThanRow(const QModelIndex &v1, const QModelIndex &v2)
{
    return v1.row() > v2.row();
}

FmtEditContentModel::FmtEditContentModel(FmtSharedTablePtr table, QObject *parent) :
    QSqlTableModel(parent, table->connection()->db())
{
    pTable = table;
}

QString FmtEditContentModel::MakeTableSqlQuery() const
{
    QString query = "select ";

    for (int i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *pFld = pTable->field(i);
        query += pFld->name() + ", ";
    }
    query += QString("ROWID as %2 from %1").arg(pTable->name(), ROW_ID_FLD_NAME);
    return query;
}

QString FmtEditContentModel::selectStatement() const
{
    QString sql = MakeTableSqlQuery();
    QString fltr = filter();

    if (!fltr.isEmpty())
        sql += QString (" WHERE %1").arg(fltr);
    if (!m_Order.isEmpty())
        sql += QString(" ORDER BY %1").arg(m_Order);

    return sql;
}

QVariant FmtEditContentModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal)
        return QSqlQueryModel::headerData(section, orientation, role);

    if (role == Qt::ToolTipRole && section < pTable->fieldsCount())
    {
        FmtField *pfld = pTable->field(section);
        return QString("<b>%1</b><p>%2</p>").arg(pfld->name().toUpper(), pfld->comment());
    }

    return QSqlTableModel::headerData(section, orientation, role);
}

QVariant FmtEditContentModel::data(const QModelIndex &index, int role) const
{
    if (index.column() < 0 || index.column() >= pTable->fieldsCount())
        return QSqlTableModel::data(index, role);

    QVariant value;
    FmtField *pfld = pTable->field(index.column());

    if (role == Qt::DisplayRole)
    {
        if (isNullCell(index))
            return CELL_NULL_STR;

        value = QSqlTableModel::data(index, role);
        if (pfld->isString())
        {
            if (pfld->type() == fmtt_UCHR)
            {
                QByteArray barr = value.toByteArray();
                return barr.toHex();
            }
            else
            {
                QString str = value.toString();
                if (str[0] == QChar(1) || str[0] == QChar(0))
                    return "<empty>";
            }
        }

        if (pfld->type() == fmtt_DATE)
        {
            QDate dt = value.toDate();
            return dt.toString("dd.MM.yyyy");
        }

        if (pfld->type() == fmtt_TIME)
        {
            QTime t = value.toTime();
            return t.toString("HH:mm:ss");
        }
        else
            return value.toString();
    }
    else if (role == Qt::EditRole)
    {
        if (pfld->type() == fmtt_DATE)
        {
            if (isNullCell(index))
                return QDate(1,1,1);

            value = QSqlTableModel::data(index, role);
            return value.toDate();
        }
        if (pfld->type() == fmtt_TIME)
        {
            if (isNullCell(index))
                return QTime(0,0);

            value = QSqlTableModel::data(index, role);
            return value.toTime();
        }
        else if (pfld->isString())
        {
            if (isNullCell(index))
                return QString();

            value = QSqlTableModel::data(index, role);
            QString str = value.toString();
            if (str[0] == QChar(1))
                return QString();
        }
    }
    else if (role == Qt::BackgroundRole)
    {
        if (index.row() % 2 != 0)
            return QBrush(QColor("#f0fff0"));//#e6e6fa
        else
            return QSqlTableModel::data(index, role);
    }
    else if (role == NORMILIZED_ROLE)
    {
        if (pfld->isString())
        {
            QVariant v = QSqlTableModel::data(index, Qt::EditRole);
            QString str = v.toString();
            if (str.isEmpty())
                return QChar(1);
            return v;
        }
        else if (pfld->type() == fmtt_CHR || pfld->type() == fmtt_UCHR)
        {
            QVariant v = QSqlTableModel::data(index, Qt::EditRole);
            QString str = v.toString();
            if (str.isEmpty())
                return QChar(0);
            else
                return str;
        }
        else if (pfld->isNumber())
        {
            QVariant v = QSqlTableModel::data(index, Qt::EditRole);
            return v;
        }
        else if (pfld->type() == fmtt_DATE)
        {
            QVariant v = QSqlTableModel::data(index, Qt::EditRole);
            if (!v.isValid())
                return QDate(1,1,1);

            if (v.toString() == "00.00.0000")
                return QDate(1,1,1);

            QDate dt = v.toDate();
            return QDate::fromString(dt.toString("dd.MM.yyyy"), "dd.MM.yyyy");
        }
        else if (pfld->type() == fmtt_TIME)
        {
            QTime tmptime;
            tmptime = QTime(0,0);

            QVariant v = QSqlTableModel::data(index, Qt::EditRole);
            tmptime = v.toTime();

            return QDateTime(QDate(1,1,1), tmptime);
        }
        else
            return QSqlTableModel::data(index, Qt::EditRole);
    }
    else if (role == Qt::ForegroundRole)
    {
        QColor color = qGuiApp->palette().color(QPalette::Disabled, QPalette::ButtonText);

        if (isNullCell(index))
            return color;

        if (pfld->isString())
        {
            QString str = QSqlQueryModel::data(index, Qt::EditRole).toString();
            if (str[0] == QChar(1))
                return color;
        }
    }
    else if (role == Qt::ToolTipRole)
    {
        QString output = QSqlQueryModel::data(index, Qt::DisplayRole).toString();
        value = QSqlTableModel::data(index, Qt::EditRole);
        if (isNullCell(index))
            output = QString(CELL_NULL_STR).toHtmlEscaped();
        else
        {
            if (pfld->isString())
            {
                if (pfld->type() == fmtt_UCHR)
                {
                    QByteArray barr = value.toByteArray().toHex();
                    QString str;

                    if (barr.size() > 20)
                        str = barr.left(10) + "..." + barr.right(10);
                    else
                        str = barr;

                    return QString("HEX(%1)").arg(str);
                }
                else
                {
                    QString str = value.toString();
                    if (str[0] == QChar(1))
                        output = tr("CHR(1)");
                    else
                        output = output.mid(0, 128);
                }
            }
            else if (pfld->isNumber())
            {
                output = QString("%2<br/>0x%1").arg(QString::number(value.toInt(), 16)).arg(value.toString());
            }
            else if (pfld->type() == fmtt_CHR || pfld->type() == fmtt_UCHR)
            {
                QString str = value.toString();
                QChar ch = str[0];
                if (ch == QChar(0))
                    output = tr("CHR(0)");
                else
                    output = QString("'%2': CHR(%1)")
                            .arg(ch.unicode())
                            .arg(ch);
            }
            else if (pfld->type() == fmtt_DATE)
            {
                QDate dt = value.toDate();
                output = QString("%1<br/>%2")
                        .arg(dt.toString("dd.MM.yyyy"))
                        .arg(dt.toString("MMMM (dddd)"));
            }
        }
        return QString("<b>%1</b><p>%2</p>").arg(fmtTypeForId(pfld->type()), output);
    }
    else if (role == Qt::TextAlignmentRole)
    {
        qint32 align = Qt::AlignVCenter;
        if (pfld->isNumber() || pfld->type() == fmtt_DATE || pfld->type() == fmtt_TIME)
            align |= Qt::AlignRight;
        return align;
    }
    return QSqlTableModel::data(index, role);
}

bool FmtEditContentModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!m_ChangedIndexes.contains(index))
        m_ChangedIndexes.append(index);

    return QSqlTableModel::setData(index, value, role);
}

bool FmtEditContentModel::insertRows(int row, int count, const QModelIndex &parent)
{
    bool hr = QSqlTableModel::insertRows(row, count, parent);

    if (hr)
    {
        for (int i = 0; i < count; i++)
        {
            for (int j = 0; j < pTable->fieldsCount(); j++)
            {
                FmtField *pfld = pTable->field(j);
                QModelIndex indx = index(row + i, j);

                QVariant val;
                if (pfld->isString())
                    val = QChar(1);
                else if (pfld->isNumber())
                    val = (qreal)0.0;
                else if (pfld->type() == fmtt_DATE)
                    val = QDate(1,1,1);
                else if (pfld->type() == fmtt_TIME)
                    val = QTime(0, 0);

                setData(indx, val);
            }
        }
    }
    return hr;
}

QString FmtEditContentModel::recordRowID(const QModelIndex &index) const
{
    return QSqlTableModel::data(QSqlTableModel::index(index.row(), record().indexOf(ROW_ID_FLD_NAME))).toString();
}

bool FmtEditContentModel::submit()
{
    return QSqlTableModel::submit();
}

bool FmtEditContentModel::insertRowIntoTable(const QSqlRecord &values)
{
    bool hr = true;
    qSort(m_ChangedIndexes.begin(), m_ChangedIndexes.end(), QModelIndexLessThanRow);
    QList<qint32> rows;

    foreach (const QModelIndex &indx, m_ChangedIndexes) {
        if (!rows.contains(indx.row()) && recordRowID(indx).isEmpty())
            rows.append(indx.row());
    }

    auto f = [this]()
    {
        QString hr;
        for (int i = 0; i < pTable->fieldsCount(); i++)
        {
            FmtField *pFld = pTable->field(i);

            if (i)
                hr += ",";
            hr += pFld->name();
        }
        return hr;
    };

    QString sql = QString("insert into %1(%2) values(")
            .arg(pTable->name())
            .arg(f());

    QString plsql_template = QString("declare \n"
                            "ri VARCHAR(36);  \n"
                            "begin %1 RETURNING ROWID INTO ri;  \n"
                            "? := ri; \n "
                            "end;");

    foreach (const qint32 &row, rows)
    {
        QVariantList bindValues;
        QSqlRecord rec = record();
        for (int i = 0; i < values.count() - 1; i++)
        {
            FmtField *pFld = pTable->field(i);
            QVariant val = data(index(row, i), NORMILIZED_ROLE);

            if (i)
                sql += ",";

            if (pFld->type() == fmtt_UCHR)
            {
                if (val.toString().at(0) == QChar(0))
                    sql += QString("UTL_RAW.CAST_TO_RAW(CHR(0))");
                else
                {
                    sql += "UTL_RAW.CAST_TO_RAW(?)";
                    bindValues.append(val);
                }
            }
            else if (pFld->type() == fmtt_CHR)
            {
                if (val.toString().at(0) == QChar(0))
                    sql += QString("CHR(0)");
                else
                {
                    sql += "?";
                    bindValues.append(val);
                }
            }
            else if (pFld->type() == fmtt_DATE)
            {
                sql += "TRIM(?)";
                bindValues.append(val);
            }
            else
            {
                sql += "?";
                bindValues.append(val);
            }
            rec.setValue(pFld->name(), val);
        }
        sql += ")";
        QString plsql = plsql_template.arg(sql);
        QSqlQuery query(pTable->connection()->db());
        query.prepare(plsql);
        int p = 0;
        for(; p < bindValues.size(); p++)
            query.bindValue(p, bindValues[p]);
        query.bindValue(p, QString(36, QChar(0)), QSql::Out);
        hr = !ExecuteQuery(&query);

        if (!rec.isEmpty())
        {
            rec.setValue(ROW_ID_FLD_NAME, query.boundValue(p).toString());
            setRecord(row, rec);
        }
    }

    return hr;
}

bool FmtEditContentModel::updateRowInTable(int row, const QSqlRecord &values)
{
    bool hr = true;

    QString rowid = recordRowID(index(row, 0));
    if (rowid.isEmpty())
        return false;

    QSqlQuery query(pTable->connection()->db());
    QString sql = QString("update %1 set ").arg(pTable->name());
    QVariantList params;

    for (int i = 0; i < values.count() - 1; i++)
    {
        FmtField *pFld = pTable->field(i);
        if (!pTable->isFieldAutoInc(i))
        {
            QSqlField sFld = values.field(i);
            QVariant val = data(index(row, i), NORMILIZED_ROLE);
            if (!params.isEmpty())
                sql += ", ";

            bool customValue = false, needBindValue = true;
            if (pFld->type() == fmtt_CHR)
            {
                if (val.toString().at(0) == QChar(0))
                {
                    sql += QString("%1 = CHR(0)").arg(sFld.name());
                    customValue = true;
                    needBindValue = false;
                }
            }
            if (pFld->type() == fmtt_UCHR)
            {
                if (val.toChar() == 0)
                {
                    sql += QString("%1 = UTL_RAW.CAST_TO_RAW(CHR(0))").arg(sFld.name());
                    customValue = true;
                    needBindValue = false;
                }
                else
                    sql += QString("%1 = UTL_RAW.CAST_TO_RAW(?)").arg(sFld.name());
            }

            if (!customValue)
                sql += QString("%1 = ?").arg(sFld.name());

            if (needBindValue)
                params.append(val);
        }
    }

    if (!params.isEmpty())
    {
        sql += QString(" where ROWID = ?");
        query.prepare(sql);
        foreach (const QVariant &val, params) {
            query.addBindValue(val);
        }
        query.addBindValue(rowid);
        hr = !ExecuteQuery(&query);
    }

    return hr;
}

bool FmtEditContentModel::deleteRowFromTable(int row)
{
    bool hr = true;
    emit beforeDelete(row);
    QString rowid = recordRowID(index(row, 0));

    if (!rowid.isEmpty())
    {
        QSqlQuery query(pTable->connection()->db());
        query.prepare(QString("delete from %1 where ROWID = ?").arg(pTable->name()));
        query.bindValue(0, rowid);

        hr = !ExecuteQuery(&query);
    }

    return hr;
}

bool FmtEditContentModel::selectRow(int row)
{
    return QSqlTableModel::selectRow(row);
}

bool FmtEditContentModel::select()
{
    QString sqlerr;
    m_Query = QSqlQuery(selectStatement(), pTable->connection()->db());
    bool hr = !ExecuteQuery(&m_Query, &sqlerr);
    if (hr)
    {
        beginResetModel();
        setQuery(m_Query);
        endResetModel();
        queryChange();
        //hr = QSqlTableModel::select();
    }
    else
        throw sqlerr;

    return hr;
}

void FmtEditContentModel::setOrder(const QString &order)
{
    m_Order = order;
}

QString FmtEditContentModel::order() const
{
    return m_Order;
}

bool FmtEditContentModel::isNullCell(const QModelIndex &index) const
{
    bool hr = false;

    FmtEditContentModel *pThis = const_cast<FmtEditContentModel*>(this);
    pThis->m_Query.seek(index.row());
    QSqlRecord rec = pThis->m_Query.record();
    if (rec.field(index.column()).isNull())
        hr = true;

    return hr;
}
