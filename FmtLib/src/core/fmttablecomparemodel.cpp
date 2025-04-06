#include "fmttablecomparemodel.h"
#include "fmtcore.h"
#include "fmttable.h"
#include "fmtfield.h"
#include <QStyle>
#include <QApplication>

FmtTableCompareModel::FmtTableCompareModel(QObject *parent)
    : QAbstractTableModel{parent}
{
    QStyle *AppStyle = qApp->style();
    m_Warning = AppStyle->standardIcon(QStyle::SP_MessageBoxWarning);
}

FmtTableCompareModel::~FmtTableCompareModel()
{

}

int FmtTableCompareModel::rowCount(const QModelIndex &parent) const
{
    return m_data.size();
}

int FmtTableCompareModel::columnCount(const QModelIndex &parent) const
{
    return ColumnCount;
}

Qt::ItemFlags FmtTableCompareModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags f = QAbstractTableModel::flags(index);
    f &= ~Qt::ItemIsUserCheckable;
    return f;
}

bool FmtTableCompareModel::isAddition(const FmtTableCompareElement &row) const
{
    return row.mine.name.isEmpty() && !row.theirs.name.isEmpty();
}

bool FmtTableCompareModel::isDeleton(const FmtTableCompareElement &row) const
{
    return !row.mine.name.isEmpty() && row.theirs.name.isEmpty();
}

QVariant FmtTableCompareModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_data.size() || index.column() >= ColumnCount)
        return QVariant();

    const FmtTableCompareElement &row = m_data[index.row()];

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if (index.column() == ColumnId)
            return index.row() + 1;
        else if (index.column() == ColumnMineName && !isAddition(row))
            return row.mine.name;
        else if (index.column() == ColumnMineType && !isAddition(row))
            return fmtTypeNameForType(row.mine.type);
        else if (index.column() == ColumnMineSize && !isAddition(row))
            return row.mine.size;

        else if (index.column() == ColumnTheirsName && !isDeleton(row))
            return row.theirs.name;
        else if (index.column() == ColumnTheirsType && !isDeleton(row))
            return fmtTypeNameForType(row.theirs.type);
        else if (index.column() == ColumnTheirsSize && !isDeleton(row))
            return row.theirs.size;
    }
    else if (role == Qt::BackgroundRole)
    {
        if (isAddition(row))
            return QBrush(QColor(200, 255, 200)); // Зеленый для добавленных
        else if (isDeleton(row))
            return QBrush(QColor(255, 200, 200)); // Красный для удаленных
        else if ((row.mine.type != row.theirs.type) || (row.mine.size != row.theirs.size))
            return QBrush(QColor(255, 255, 200)); // Желтый для измененных
    }
    else if (role == Qt::DecorationRole)
    {
        if (!isAddition(row) && !isDeleton(row))
        {
            if (row.mine.type != row.theirs.type)
            {
                if (index.column() == ColumnMineType || index.column() == ColumnTheirsType)
                    return m_Warning;
            }
            else if (row.mine.size != row.theirs.size)
            {
                if (index.column() == ColumnMineSize || index.column() == ColumnTheirsSize)
                    return m_Warning;
            }
        }
    }

    return QVariant();
}

QVariant FmtTableCompareModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    static QStringList header =
    {
        tr("Имя столбца (mine)"),
        tr("Тип (mine)")
    };
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return section == 0 ? tr("Original") : tr("Modified");

    return QVariant();
}

void FmtTableCompareModel::setLists(FmtTable *table, const QString &cppcstruct)
{
    FmtFldElementVector tablevec, structvec;
    readFmtTable(table, tablevec);
    readTableStruct(cppcstruct, structvec);

    makeCompareData(tablevec, structvec);
}

void FmtTableCompareModel::setLists(FmtTable *table1, FmtTable *table2)
{
    FmtFldElementVector tablevec, structvec;
    readFmtTable(table1, tablevec);
    readFmtTable(table2, structvec);

    makeCompareData(tablevec, structvec);
}

void FmtTableCompareModel::readFmtTable(FmtTable *table, FmtFldElementVector &vec)
{
    for (int i = 0; i < table->fieldsCount(); i++)
    {
        FmtField *fld = table->field(i);

        FmtFldElement elem;
        elem.name = fld->undecorateName();
        elem.comment = fld->comment();
        elem.size = fld->size();
        elem.type = fld->type();

        vec.append(elem);
    }
}

QString FmtTableCompareModel::extractComments(const QString &structCode, int fieldEndPos)
{
    QString comment;

    // Проверяем однострочный комментарий
    QRegularExpression singleLineComment(R"(\s*//([^\n]*))");
    QRegularExpressionMatch match = singleLineComment.match(structCode, fieldEndPos);

    if (match.hasMatch())
    {
        comment = match.captured(1).trimmed();
        return comment;
    }

    // Проверяем многострочный комментарий
    QRegularExpression multiLineComment(R"(\s*/\*([^*]*\*+(?:[^/*][^*]*\*+)*)/)");
    match = multiLineComment.match(structCode, fieldEndPos);

    if (match.hasMatch())
    {
        comment = match.captured(1).trimmed();
        // Удаляем * в начале каждой строки
        comment.replace(QRegularExpression(R"(^\s*\*\s*)"), "");
        comment.replace(QRegularExpression(R"(\n\s*\*\s*)"), "\n");

        return comment;
    }

    return "";
}

void FmtTableCompareModel::readTableStruct(const QString &cppcstruct, FmtFldElementVector &vec)
{
    // Создаем копию для работы
    QString code = cppcstruct;

    // Удаляем все комментарии перед полями (но сохраняем их позиции)
    code.replace(QRegularExpression(R"(^\s*//[^\n]*\n)", QRegularExpression::MultilineOption), "");

    // Основное регулярное выражение для полей структуры
    QRegularExpression fieldRegex(R"(([^{}\w][\w:<>]+)\s+([\*\&]?\s*\w+\s*[\*\&]?)(?:\s*\[\s*(\d+)\s*\])?\s*;)",
                                  QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatchIterator it = fieldRegex.globalMatch(code);

    while (it.hasNext())
    {
        QRegularExpressionMatch match = it.next();
        FmtFldElement info;

        // Обработка типа
        QString type = match.captured(1).trimmed();
        //info.type = match.captured(1).trimmed();

        // Обработка имени и указателей
        QString namePart = match.captured(2).trimmed();
        //info.isPointer = namePart.contains('*');
        info.name = namePart.remove('*').remove('&').trimmed();

        // Обработка массива
        if (match.capturedLength(3) > 0)
            info.size = match.captured(3).toInt();
        else
            info.size = 0;

        info.type = fmtTypeFromCppType(type, info.size);

        if (!info.size)
            info.size = fmtTypeSize(info.type);

        // Извлекаем комментарий после поля
        int fieldEndPos = match.capturedEnd();
        info.comment = extractComments(cppcstruct, fieldEndPos);

        vec.append(info);
    }
}

void FmtTableCompareModel::makeCompareData(const FmtFldElementVector &mine, const FmtFldElementVector &theirs)
{
    beginResetModel();

    m_data.clear();
    int i = 0, j = 0;

    while (i < mine.size() || j < theirs.size())
    {
        FmtTableCompareElement row;

        if (i < mine.size() && j < theirs.size() && mine[i].name.toUpper() == theirs[j].name.toUpper())
        {
            row.mine = mine[i];
            row.theirs = theirs[j];
            i++;
            j++;
        }
        else
        {
            bool foundInNew = false;
            bool foundInOld = false;

            // Поиск в новом списке
            if (i < mine.size())
            {
                for (int k = j; k < theirs.size(); k++)
                {
                    if (mine[i].name.toUpper() == theirs[k].name.toUpper())
                    {
                        foundInNew = true;
                        break;
                    }
                }
            }

            // Поиск в старом списке
            if (j < theirs.size())
            {
                for (int k = i; k < mine.size(); k++)
                {
                    if (theirs[j].name.toUpper() == mine[k].name.toUpper())
                    {
                        foundInOld = true;
                        break;
                    }
                }
            }

            if (!foundInNew && i < mine.size())
                row.mine = mine[i++];
            else if (!foundInOld && j < theirs.size())
                row.theirs = theirs[j++];
            else
            {
                if (i < mine.size()) i++;
                if (j < theirs.size()) j++;
            }
        }

        m_data.append(row);
    }
    endResetModel();
}
