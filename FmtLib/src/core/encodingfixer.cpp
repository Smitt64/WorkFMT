#include "encodingfixer.h"
#include <QTextCodec>
#include <QSet>
#include <QDebug>

bool EncodingFixer::isRussianLetter(ushort ch)
{
    return (ch >= 0x0400 && ch <= 0x04FF) || // Кириллица
           (ch >= 0x0500 && ch <= 0x052F);   // Дополнительная
}

bool EncodingFixer::isAsciiPrintable(char ch)
{
    unsigned char uc = static_cast<unsigned char>(ch);
    return (uc >= 0x20 && uc <= 0x7E) || // Печатные ASCII
           uc == 0x09 || uc == 0x0A || uc == 0x0D; // TAB, LF, CR
}

int EncodingFixer::calculateQualityScore(const QByteArray& data, const QString& encoding)
{
    QTextCodec* codec = QTextCodec::codecForName(encoding.toLatin1());
    if (!codec) return -1;

    QString decoded = codec->toUnicode(data);
    int score = 0;

    for (const QChar& ch : qAsConst(decoded))
    {
        ushort code = ch.unicode();

        if (isRussianLetter(code))
            score += 10;  // Русские буквы - хорошо
        else if ((code >= 0x0041 && code <= 0x005A) || /* A-Z */ (code >= 0x0061 && code <= 0x007A) /* a-z */)
            score += 2;   // Латиница - допустимо
        else if (code >= 0x0030 && code <= 0x0039) // Цифры
            score += 2;
        else if (code == 0x0020 || code == 0x000A || code == 0x000D || code == 0x0009)
            score += 1;   // Пробелы
        else if (code == 0xFFFD) // Символ замены
            score -= 50;  // Большой штраф
        else if (code >= 0x0080 && code <= 0x00FF) // Подозрительные символы
            score -= 10;
    }

    return score;
}

bool EncodingFixer::isBrokenSegment(const QByteArray& segment)
{
    if (segment.isEmpty()) return false;

    bool hasHighAscii = false;
    bool hasAsciiLetters = false;
    int nonPrintable = 0;

    for (char c : segment)
    {
        unsigned char uc = static_cast<unsigned char>(c);

        if (uc > 0x7F)
            hasHighAscii = true;  // Не-ASCII символ
        else if ((uc >= 'A' && uc <= 'Z') || (uc >= 'a' && uc <= 'z'))
            hasAsciiLetters = true; // Есть латинские буквы
        else if (!isAsciiPrintable(c))
            nonPrintable++;
    }

    return hasHighAscii && (!hasAsciiLetters || nonPrintable > 2);
}

QList<QByteArray> EncodingFixer::splitToSegments(const QByteArray& data)
{
    QList<QByteArray> segments;
    QByteArray currentSegment;

    const char delimiters[] = " \t\n\r.,;:!?()[]{}/\\|@#$%^&*_=+~`'\"";
    QSet<char> delimiterSet;
    for (size_t i = 0; i < strlen(delimiters); i++)
        delimiterSet.insert(delimiters[i]);

    for (int i = 0; i < data.size(); i++)
    {
        char ch = data[i];

        if (delimiterSet.contains(ch))
        {
            if (!currentSegment.isEmpty())
            {
                segments.append(currentSegment);
                currentSegment.clear();
            }
            segments.append(QByteArray(1, ch));
        }
        else
            currentSegment.append(ch);
    }

    if (!currentSegment.isEmpty())
        segments.append(currentSegment);

    return segments;
}

QByteArray EncodingFixer::fixSegmentEncoding(
    const QByteArray& segment,
    const QString& primaryEncoding,
    const QList<QString>& secondaryEncodings)
{
    if (segment.isEmpty())
        return segment;

    int primaryScore = calculateQualityScore(segment, primaryEncoding);
    if (primaryScore > 20)
        return segment;

    int bestScore = primaryScore;
    QByteArray bestResult = segment;
    QString bestEncoding = primaryEncoding;

    QTextCodec* primaryCodec = QTextCodec::codecForName(primaryEncoding.toLatin1());
    if (!primaryCodec)
        return segment;

    for (const QString& secondaryEncoding : secondaryEncodings)
    {
        QTextCodec* secondaryCodec = QTextCodec::codecForName(secondaryEncoding.toLatin1());
        if (!secondaryCodec) continue;

        QString decoded = secondaryCodec->toUnicode(segment);
        QByteArray converted = primaryCodec->fromUnicode(decoded);
        int score = calculateQualityScore(converted, primaryEncoding);

        if (score > bestScore)
        {
            bestScore = score;
            bestResult = converted;
            bestEncoding = secondaryEncoding;
        }
    }

    return bestResult;
}

QByteArray EncodingFixer::normalizeEncoding(
    const QByteArray& rawData,
    const QString& primaryEncoding,
    const QList<QString>& secondaryEncodings)
{
    if (rawData.isEmpty())
        return QByteArray();

    // Разбиваем на сегменты
    QList<QByteArray> segments = splitToSegments(rawData);
    QByteArray result;

    for (const QByteArray& segment : qAsConst(segments))
    {
        // Разделители и короткие сегменты не обрабатываем
        if (segment.size() <= 2 || !isBrokenSegment(segment))
        {
            result.append(segment);
            continue;
        }

        // Пробуем исправить сегмент
        QByteArray fixed = fixSegmentEncoding(segment, primaryEncoding, secondaryEncodings);
        result.append(fixed);
    }

    return result;
}
