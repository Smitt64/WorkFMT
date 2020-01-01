#include "fmtfildsmodel.h"
#include "fmtfield.h"
#include "fmttable.h"
#include "fmtcore.h"
#include <QStyle>
#include <QApplication>

FmtFildsModel::FmtFildsModel(FmtTable *parent) :
    QAbstractItemModel((QObject*)parent)
{
    pTable = parent;
}

FmtFildsModel::~FmtFildsModel()
{

}

int FmtFildsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 5 + 1;
}

QVariant FmtFildsModel::data(const QModelIndex &index, int role) const
{
    QVariant value;

    if (index.row() >= pTable->m_pFields.size())
        return QVariant();

    FmtField *fld = pTable->m_pFields[index.row()];
    if (role != Qt::DisplayRole && role != Qt::EditRole && role != FindSubTextRole)
    {
        bool isValidName = true; int pos = 0;
        QRegExpValidator validator(QRegExp("([a-zA-Z_][a-zA-Z0-9]*)*"));

        QString name = fld->name();
        isValidName = !(validator.validate(name, pos) == QValidator::Invalid);

        if (role == Qt::DecorationRole && index.column() == FmtFildsModel::fld_Name)
        {
            if (fld->name().contains("unnamedfld"))
                return QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning);

            // проверить допустимость имени
            if (!isValidName)
                return QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical);
        }
        if (role == Qt::ToolTipRole && index.column() == FmtFildsModel::fld_Name)
        {
            if (!isValidName)
                return tr("Имя столбца имеет недопустимое значение");
        }

        return QVariant();
    }

    switch(index.column())
    {
    case FmtFildsModel::fld_Name:
    case FmtFildsModel::fld_Comment:
        value = ProcessHighlightFields(index, role);
        break;
    case FmtFildsModel::fld_DbName:
        value = fld->name();
        break;
    case FmtFildsModel::fld_Size:
        value = fld->size();
        break;
    case FmtFildsModel::fld_Type:
        if (role == Qt::EditRole)
            value = fld->type();
        else
            value = fmtTypeForId(fld->type());
        break;
    case FmtFildsModel::fld_Offset:
        value = fld->offset();
        break;
    case FmtFildsModel::fld_Outlen:
        value = fld->outlen();
        break;
    case FmtFildsModel::fld_Decpoint:
        value = fld->decpoint();
        break;
    case FmtFildsModel::fld_Hidden:
        value = fld->isHidden();
        break;
    case FmtFildsModel::fld_Id:
        value = fld->id();
        break;
    case FmtFildsModel::fld_FmtId:
        value = fld->tableId();
        break;
    case FmtFildsModel::fld_TypeIndex:
        value = fld->typeIndex();
        break;
    }

    return value;
}

QVariant FmtFildsModel::ProcessHighlightFieldsEx(const QString &text, const QString &HighlightText, const QString &color) const
{
    QVariant value = text;
    if (!HighlightText.isEmpty())
    {
        QString str = value.toString();
        // (<[^>]+>|\\n+)*(date)(<[^>]+>|\\n+)*
        QString rxstring = QString("(<[^>]+>|\\\\n+)*(%1)(<[^>]+>|\\\\n+)*").arg(HighlightText);
        QRegExp rx(rxstring);
        rx.setCaseSensitivity(Qt::CaseInsensitive);
        value = str.replace(rx, QString("<span style=\"background-color:#%1;\">\\2</span>").arg(color));
    }
    return value;
}

QVariant FmtFildsModel::ProcessHighlightFields(const QModelIndex &index, int role, const QString &HighlightText, const QString &color) const
{
    if (index.row() >= pTable->m_pFields.size())
        return QVariant();

    QVariant value;
    FmtField *fld = pTable->m_pFields[index.row()];

    switch(index.column())
    {
    case FmtFildsModel::fld_Name:
        value =  fld->undecorateName();
        break;
    case FmtFildsModel::fld_Comment:
        value = fld->comment();
        break;
    }

    if (role == FindSubTextRole && !HighlightText.isEmpty())
        value = ProcessHighlightFieldsEx(value.toString(), HighlightText, color);

    return value;
}

bool FmtFildsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() == pTable->m_pFields.size() && index.column() == FmtFildsModel::fld_Name)
    {
        QString str = value.toString().simplified();

        if (!str.isEmpty())
        {
            FmtField *fld = pTable->addField(value.toString(), fmtt_INT);
            fld->setName("t_" + value.toString());
            emit AddedToLast(fld, index);
        }
    }
    else
    {
        if (index.row() < pTable->m_pFields.size())
        {
            FmtField *fld = pTable->m_pFields[index.row()];
            switch(index.column())
            {
            case FmtFildsModel::fld_Name:
                fld->setName("t_" + value.toString());
                break;
            case FmtFildsModel::fld_Type:
                if (role == Qt::EditRole)
                {
                    fld->setType(value.toInt());
                    emit dataChanged(this->index(index.row(), FmtFildsModel::fld_Size), this->index(index.row(), FmtFildsModel::fld_Size));
                    emit dataChanged(this->index(index.row(), FmtFildsModel::fld_Offset), this->index(index.row(), FmtFildsModel::fld_Offset));
                }
                break;
            case FmtFildsModel::fld_Size:
                fld->setSize(value.toInt());
                break;
            case FmtFildsModel::fld_Comment:
                fld->setComment(value.toString());
                break;
            case FmtFildsModel::fld_TypeIndex:
                fld->setTypeIndex(value.toInt());
                emit dataChanged(this->index(index.row(), FmtFildsModel::fld_Type), this->index(index.row(), FmtFildsModel::fld_Type));
                emit dataChanged(this->index(index.row(), FmtFildsModel::fld_Size), this->index(index.row(), FmtFildsModel::fld_Size));
                emit dataChanged(this->index(index.row(), FmtFildsModel::fld_Offset), this->index(index.row(), FmtFildsModel::fld_Offset));
                break;
            case FmtFildsModel::fld_Offset:
                fld->setOffset(value.toInt());
                break;
            case FmtFildsModel::fld_Outlen:
                fld->setOutlen(value.toInt());
                break;
            case FmtFildsModel::fld_Decpoint:
                fld->setDecpoint(value.toInt());
                break;
            case FmtFildsModel::fld_Hidden:
                fld->setHidden(value.toBool());
                break;
            }
            return true;
        }
    }
    return QAbstractItemModel::setData(index, value, role);
}

void FmtFildsModel::setFieldPropertyChanged(const FmtField *fld, const FmtFldIndex &prop)
{
    int row = fld->index();
    //quint16 col = -1;

    QList<quint16> col;
    switch(prop)
    {
    case FmtField::fld_Id:
        col.append(fld_Id);
        break;
    case FmtField::fld_FmtId:
        col.append(fld_FmtId);
        break;
    case FmtField::fld_Name:
        col.append(fld_Name);
        break;
    case FmtField::fld_Type:
        col.append(fld_Type);
        col.append(fld_TypeIndex);
        col.append(fld_Size);
        break;
    case FmtField::fld_Size:
        col.append(fld_Size);
        break;
    case FmtField::fld_Offset:
        col.append(fld_Offset);
        break;
    case FmtField::fld_Outlen:
        col.append(fld_Outlen);
        break;
    case FmtField::fld_DecPoint:
        col.append(fld_Decpoint);
        break;
    case FmtField::fld_Hidden:
        col.append(fld_Hidden);
        break;
    case FmtField::fld_Comment:
        col.append(fld_Comment);
        break;
    }

    foreach (const quint16 &value, col)
        emit dataChanged(this->index(row, value), this->index(row, value));
}

QModelIndex FmtFildsModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return createIndex(row, column);
}

QModelIndex FmtFildsModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QModelIndex();
}

int FmtFildsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (parent.isValid())
        return 0;

    return pTable->m_pFields.size() + 1;
}

bool FmtFildsModel::isInsertRow(const QModelIndex &index) const
{
    return index.row() == pTable->m_pFields.size();
}

Qt::ItemFlags FmtFildsModel::flags(const QModelIndex &index) const
{
    if (index.column() == fld_DbName)
        return Qt::ItemIsEnabled;

    if (index.column() == fld_Custom)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    int Flags = Qt::ItemIsEnabled;
    if (index.row() < pTable->m_pFields.size())
        Flags |= Qt::ItemIsSelectable | Qt::ItemIsEditable;
    else
    {
        if (index.column() == fld_Name)
            Flags |= /*Qt::ItemIsSelectable |*/ Qt::ItemIsEditable;
    }

    return (Qt::ItemFlags)Flags;
}

QVariant FmtFildsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    static QStringList headers = QStringList()
            << "Имя столбца" << "Тип" << "Длина" << "Комментарий" << "Имя столбца в БД" << "";//<< "⚙";
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        return headers.at(section);

    if (orientation == Qt::Vertical)
    {
        if (section >= pTable->m_pFields.size())
        {
            if (role == Qt::DisplayRole)
                return QString();//QString("✳");
            if (role == Qt::DecorationRole)
                return QPixmap(":/img/new.png");
            if (role == Qt::FontRole)
                return QFont("Arial", 10);
        }
    }
    else
    {
        if (section == 5)
        {
            if (role == Qt::DecorationRole)
                return QPixmap(":/img/dialog1.png");
            else if(role == Qt::TextAlignmentRole)
                return Qt::AlignCenter;
        }
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

void FmtFildsModel::beginRemoveField(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
}

void FmtFildsModel::endRemoveField()
{
    endRemoveRows();
}

FmtTable *FmtFildsModel::fmtTable()
{
    return pTable;
}

bool FmtFildsModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    Q_UNUSED(count);
    pTable->insertField(row, "t_unnamedfld", fmtt_INT);
    return true;
}

bool FmtFildsModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    Q_UNUSED(count);
    pTable->removeField(row);
    return true;
}
