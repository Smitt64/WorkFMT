#ifndef FMTGENINTERFACE_H
#define FMTGENINTERFACE_H

#include <QtCore>
#include <QUuid>
#include "fmttable.h"
#include "fmtlibfactory.h"
#include "loggingcategories.h"

class FmtGenFinishEvent : public QEvent
{
public:
    FmtGenFinishEvent(const QByteArray &data);
    static int getEventRegType();

    QByteArray data() const { return _data; }

private:
    static QEvent::Type EventType;
    QByteArray _data;
};

class FmtGenInterface : public QObject
{
    Q_OBJECT
    friend class FmtGenInterfaceRunnable;
    friend void registerFmtGenInterface(const QString &alias);
public:
    enum GenType
    {
        GenText,
        GenSql,
        GenCpp
    };

    FmtGenInterface();

    virtual GenType getContentType() const { return GenText; }
    void start(QSharedPointer<FmtTable> pTable);
    virtual bool event(QEvent *e);

    virtual void propertyEditor(QWidget *parent) { /*return NULL;*/ }
    virtual bool hasPropertes() const { return false; }

signals:
    void finish(const QByteArray&);

protected:
    virtual QByteArray makeContent(QSharedPointer<FmtTable> pTable) = 0;

public:
    static FmtLibFactory<FmtGenInterface,QString> m_pGenInterfaceFactory;
};

template<class T>
void registerFmtGenInterface(const QString &alias)
{
    FmtGenInterface::m_pGenInterfaceFactory.add<T>(alias);
    qInfo(logCore()) << "Interface registered: " << alias;
}

FmtGenInterface *fmtGenInterfaceCreate(const QString &alias);
QStringList fmtGenInterfaces();
QTextStream &tab(QTextStream &s);

#endif // FMTGENINTERFACE_H
