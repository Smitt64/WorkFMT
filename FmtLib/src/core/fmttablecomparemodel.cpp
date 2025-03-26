#include "fmttablecomparemodel.h"
#include "fmtcore.h"
#include "fmttable.h"
#include "fmtfield.h"

FmtTableCompareModel::FmtTableCompareModel(QObject *parent)
    : QAbstractTableModel{parent}
{

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
    return 3;
}

QVariant FmtTableCompareModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_data.size() || index.column() >= 2)
        return QVariant();

    const FmtTableCompareElement &row = m_data[index.row()];

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if (index.column() == ColumnMine)
            return row.mine.name;
        else if (index.column() == ColumnTheir)
            return row.theirs.name;
    }
    else if (role == Qt::BackgroundRole) {
        if (row.mine.name.isEmpty() && !row.theirs.name.isEmpty())
            return QBrush(QColor(200, 255, 200)); // Зеленый для добавленных
        else if (!row.mine.name.isEmpty() && row.theirs.name.isEmpty())
            return QBrush(QColor(255, 200, 200)); // Красный для удаленных
        else if (row.mine.name != row.theirs.name) {
            return QBrush(QColor(255, 255, 200)); // Желтый для измененных
        }
    }

    return QVariant();
}

QVariant FmtTableCompareModel::headerData(int section, Qt::Orientation orientation, int role) const
{
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
}

void FmtTableCompareModel::readFmtTable(FmtTable *table, FmtFldElementVector &vec)
{
    for (int i = 0; i < table->fieldsCount(); i++)
    {
        FmtField *fld = table->field(i);

        FmtFldElement elem;
        elem.name = fld->name();
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
    QRegularExpression fieldRegex("(([\\w:<>]+)\\s*([\\*\\&]?\\s*\\w+\\s*[\\*\\&]?)(?:\\s*\\[\\s*(\\d+)\\s*\\])?\\s*;)",
                                  QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatchIterator it = fieldRegex.globalMatch(code);

    while (it.hasNext())
    {
        QRegularExpressionMatch match = it.next();
        FmtFldElement info;

        // Обработка типа
        //info.type = match.captured(1).trimmed();

        // Обработка имени и указателей
        QString namePart = match.captured(2).trimmed();
        //info.isPointer = namePart.contains('*');
        info.name = namePart.remove('*').remove('&').trimmed();

        // Обработка массива
        if (match.capturedLength(3) > 0)
            info.size = match.captured(3).toInt();

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

        if (i < mine.size() && j < theirs.size() && mine[i].name == theirs[j].name)
        {
            row.mine = mine[i];
            row.theirs = theirs[i];
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
                    if (mine[i].name == theirs[k].name)
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
                    if (theirs[j].name == mine[k].name)
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
