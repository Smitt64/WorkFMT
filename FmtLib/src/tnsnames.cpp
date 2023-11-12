#include "tnsnames.h"
#include <QFile>

TnsNames::TnsNames(const QString &oraHome) :
    _oraHome(oraHome)
{
    load();
}

TnsNames::~TnsNames()
{

}

QString TnsNames::clearString(QString str)
{
    QString s = str.remove("\n").remove("\r").remove(")").remove("(").remove("=").remove(" ").trimmed().simplified();
    return s;
}

int TnsNames::findClosePose(const QString &str, int pos)
{
    int opened = 0;
    int mpos = pos + 1;

    bool next = true;
    while(next)
    {
        if (mpos >= str.length())
        {
            next = false;
        }
        else
        {
            if (str[mpos] == '(')
            {
                opened ++;
            }
            else if (str[mpos] == ')')
            {
                opened --;

                if (opened == 0)
                {
                    next = false;
                }
            }

            mpos ++;
        }
    }

    return mpos;
}

TnsNames::TnsElement TnsNames::element(const QString &str) const
{
    return m_List[str];
}

void TnsNames::load()
{
    QString tnsNamesOraFilePath = _oraHome.absoluteFilePath("NETWORK\\ADMIN\\TNSNAMES.ORA");
    QString RegExPattern = "[\\n]*[\\s]*[^\\(][a-zA-Z0-9_.-]+[\\s]*=[\\s]*\\(";

    QFile f(tnsNamesOraFilePath);
    if (f.open(QIODevice::ReadOnly))
    {
        int pos = 0;
        QString in = f.readAll();
        QRegExp rx(RegExPattern);

        while ((pos = rx.indexIn(in, pos)) != -1)
        {
            TnsElement element;
            element._Name = clearString(rx.cap(0));

            QRegExp rx2("[^\\(][a-zA-Z0-9_.-]+[\\s]*=[\\s]*[a-zA-Z0-9_.-]*\\)");
            int pos2 = 0;
            QString subStr = in.mid(pos, findClosePose(in, pos) - pos);
            while ((pos2 = rx2.indexIn(subStr, pos2, QRegExp::CaretAtZero)) != -1)
            {
                QString cap = rx2.cap(0);

                if (cap.contains("PROTOCOL", Qt::CaseInsensitive))
                    element._Protocol = clearString(cap).remove("PROTOCOL", Qt::CaseInsensitive);

                if (cap.contains("SERVICE_NAME", Qt::CaseInsensitive))
                    element._ServiceName = clearString(cap).remove("SERVICE_NAME", Qt::CaseInsensitive);

                if (cap.contains("HOST", Qt::CaseInsensitive))
                    element._Host = clearString(cap).remove("HOST", Qt::CaseInsensitive);

                if (cap.contains("PORT", Qt::CaseInsensitive))
                    element._Port = clearString(cap).remove("PORT", Qt::CaseInsensitive).toInt();

                pos2 += rx2.matchedLength();
            }

            m_List.insert(element._Name, element);
            pos += subStr.length();
        }

        f.close();
    }
}
