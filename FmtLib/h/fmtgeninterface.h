#ifndef FMTGENINTERFACE_H
#define FMTGENINTERFACE_H

#include <QtCore>
#include <QUuid>
#include "fmttable.h"
#include "fmtlibfactory.h"
#include "loggingcategories.h"
#include "qobject.h"
#include <codeeditor/codehighlighter.h>
#include <codeeditor/highlighterstyle.h>
#include <QTextCharFormat>
#include <utility>
#include <QMap>

class FmtGenFinishEvent : public QEvent
{
public:
    FmtGenFinishEvent(const QMap<QString, QByteArray> &data);
    static int getEventRegType();

    QMap<QString, QByteArray> data() const { return _data; }

private:
    static QEvent::Type EventType;
    QMap<QString, QByteArray> _data;
};

class FmtGenInterface : public QObject
{
    Q_OBJECT
    friend class FmtGenInterfaceRunnable;
    friend void registerFmtGenInterface(const QString &alias);
public:
    FmtGenInterface();

    virtual int getContentType() const { return 0; }
    void start(QSharedPointer<FmtTable> pTable);
    virtual bool event(QEvent *e);

    virtual void propertyEditor(QWidget *parent) { Q_UNUSED(parent) }
    virtual bool hasPropertes() const { return false; }

    virtual QStringList tabs();
    virtual GenHighlightingRuleList highlightingRuleList() const;

signals:
    void finish(const QMap<QString, QByteArray>&);

protected:
    virtual QMap<QString, QByteArray> makeContent(QSharedPointer<FmtTable> pTable) = 0;

public:
    static FmtLibFactory<FmtGenInterface,QString> m_pGenInterfaceFactory;
};

template<class T>
void registerFmtGenInterface(const QString &alias, const QString &label)
{
    FmtGenInterface::m_pGenInterfaceFactory.add<T>(alias, label);
    qInfo(logCore()) << "Interface registered: " << alias << ", Label: " << label;
}

FmtGenInterface *fmtGenInterfaceCreate(const QString &alias);
QStringList fmtGenInterfaces();
QStringList fmtGenAliases();
qint32 fmtGenInterfacesCount();
QTextStream &tab(QTextStream &s);

#endif // FMTGENINTERFACE_H
