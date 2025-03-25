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
    return 0;
}

int FmtTableCompareModel::columnCount(const QModelIndex &parent) const
{
    return 3;
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

    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        FmtFldElement info;

        // Обработка типа
        //info.type = match.captured(1).trimmed();

        // Обработка имени и указателей
        QString namePart = match.captured(2).trimmed();
        //info.isPointer = namePart.contains('*');
        info.name = namePart.remove('*').remove('&').trimmed();

        // Обработка массива
        if (match.capturedLength(3) > 0) {
            info.size = match.captured(3).toInt();
        }

        // Извлекаем комментарий после поля
        int fieldEndPos = match.capturedEnd();
        info.comment = extractComments(cppcstruct, fieldEndPos);

        vec.append(info);
    }
}
