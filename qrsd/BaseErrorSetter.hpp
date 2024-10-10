#ifndef BASEERRORSETTER_HPP
#define BASEERRORSETTER_HPP

#include <QtSql>
#include <QTextStream>
#include <RsdC.h>
#include <QTextCodec>

template<class T>class BaseErrorSetter
{
public:
    BaseErrorSetter(T *ptr) :
        m_Ptr(ptr)
    {
        codec1251 = QTextCodec::codecForName("Windows-1251");
        codec866 = QTextCodec::codecForName("IBM 866");
    }

    void setLastRsdError(const XRsdError &e, const QSqlError::ErrorType &errorType = QSqlError::UnknownError)
    {
        XRsdError &cError = const_cast<XRsdError&>(e);
        CRsdEnvironment *env = const_cast<CRsdEnvironment*>(cError.getEnv());

        if (!env)
        {
            m_Ptr->setLastError(QSqlError(e.what(), QString(), errorType));

            return;
        }

        QString err;
        QTextStream stream(&err);
        int errcount = env->getErrorCount();
        for (int i = 0; i < errcount; i++)
        {
            const CRsdError &error = env->getError(i);
            const char *descr = error.getDescr();

            if (m_Unicode)
                stream << codec866->toUnicode(descr) << Qt::endl;
            else
                stream << codec1251->toUnicode(descr) << Qt::endl;
        }

        if (!err.isEmpty())
            m_Ptr->setLastError(QSqlError(err, QString(), errorType));
        else
            m_Ptr->setLastError(QSqlError(e.what(), QString(), errorType));
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
