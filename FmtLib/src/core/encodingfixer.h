#ifndef ENCODINGFIXER_H
#define ENCODINGFIXER_H

#include <QByteArray>
#include <QString>
#include <QList>

class EncodingFixer
{
public:
    /**
     * @brief Приводит смешанную кодировку к основной
     * @param rawData Сырые байты файла со смешанными кодировками
     * @param primaryEncoding Основная кодировка (например, "UTF-8")
     * @param secondaryEncodings Вторичные кодировки для исправления ошибок
     * @return QByteArray в основной кодировке
     *
     * Пример:
     * QByteArray fixed = EncodingFixer::normalizeEncoding(
     *     rawData,
     *     "UTF-8",
     *     {"Windows-1251", "IBM 866"}
     * );
     */
    static QByteArray normalizeEncoding(
        const QByteArray& rawData,
        const QString& primaryEncoding,
        const QList<QString>& secondaryEncodings = {"Windows-1251", "IBM 866"}
    );

    /**
     * @brief Исправляет отдельный сегмент с ошибкой кодировки
     * @param segment Битый сегмент
     * @param primaryEncoding Основная кодировка
     * @param secondaryEncodings Вторичные кодировки
     * @return Исправленный сегмент в основной кодировке
     */
    static QByteArray fixSegmentEncoding(
        const QByteArray& segment,
        const QString& primaryEncoding,
        const QList<QString>& secondaryEncodings
    );

    /**
     * @brief Определяет, похож ли сегмент на битый
     */
    static bool isBrokenSegment(const QByteArray& segment);

private:
    static QList<QByteArray> splitToSegments(const QByteArray& data);
    static int calculateQualityScore(const QByteArray& data, const QString& encoding);
    static bool isRussianLetter(ushort ch);
    static bool isAsciiPrintable(char ch);
};

#endif // ENCODINGFIXER_H
