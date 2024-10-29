#ifndef BASEERRORSETTER_HPP
#define BASEERRORSETTER_HPP

#include <QtSql>
#include <QTextStream>
#include <RsdC.h>
#include <QTextCodec>
#include <rsdcore.h>

template<class T>class BaseErrorSetter
{
public:
    BaseErrorSetter(T *ptr) :
        m_Ptr(ptr)
    {
        codec1251 = QTextCodec::codecForName("Windows-1251");
        codec866 = QTextCodec::codecForName("IBM 866");
    }

    void setLastRsdError(Qt::HANDLE handle)
    {
        Qt::HANDLE err = nullptr;

        char msg[ERR_LEN] = {'\0'};
        coreGetLastError(handle, msg, ERR_LEN);

        if (msg[0] == '\0')
        {
            coreGetErrorHandle(handle, &err);

            if (err)
            {
                int count = coreGetErrorCount(err);

                QString ErrorString;
                QTextStream stream(&ErrorString);

                for (int i = 0; i < count; i++)
                {
                    coreGetErrorText(err, i, msg, ERR_LEN);

                    QTextCodec::ConverterState state;
                    QString value = codec1251->toUnicode(msg, qstrlen(msg), &state);

                    if (state.invalidChars > 0)
                        value = codec866->toUnicode(msg, qstrlen(msg), &state);

                    stream << value << Qt::endl;
                }

                m_Ptr->setLastError(QSqlError(ErrorString,
                                       QString(),
                                       QSqlError::ConnectionError));
            };
        }
    }

    void setLastUnforeseenError(const QSqlError::ErrorType &errorType = QSqlError::UnknownError)
    {
        QString err = QObject::tr("Непредусмотренное исключение");
        m_Ptr->setLastError(QSqlError(err, QString(), errorType));
    }

protected:
    void setUnicode(bool f)
    {
        m_Unicode = f;
    }

private:
    bool m_Unicode;
    QTextCodec *codec1251, *codec866;
    T *m_Ptr;
};

#endif // BASEERRORSETTER_HPP
