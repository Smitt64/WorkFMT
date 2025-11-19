#ifndef DIFFMODEPARSER_H
#define DIFFMODEPARSER_H

#include <QString>
#include <QVector>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonArray>
#include <QXmlStreamWriter>
#include "DiffToScriptl_global.h"

typedef struct DIFFTOSCRIPTL_EXPORT DiffLine
{
    enum Type
    {
        Index,      // Информация о файле
        Update,     // Обновление строки
        Delete,     // Удаление строки
        Insert      // Вставка строки
    };

    Type type;
    QString oldValue; // Значение до изменения (для Update и Delete)
    QString newValue; // Значение после изменения (для Update и Insert)

    QJsonObject toJson() const;
    void toXml(QXmlStreamWriter& writer) const;
    QString typeToString(Type type) const;
} DiffLine;

class DIFFTOSCRIPTL_EXPORT DiffModeParser
{
public:
    DiffModeParser(QTextStream &stream);

    const QVector<DiffLine>& getResult() const;

    QJsonArray toJson() const;
    void toXml(QXmlStreamWriter& writer) const;
    void serialize(QTextStream& outputStream, const QString& format) const;

private:
    QVector<DiffLine> diffLines;
    QVector<QString> deletedLines;

    void parse(QTextStream& stream);
    bool isPotentialUpdate(const QString& deletedLine, const QString& addedLine);
};

#endif // DIFFMODEPARSER_H
