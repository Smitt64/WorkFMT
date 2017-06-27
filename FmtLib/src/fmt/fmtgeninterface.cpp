#include "fmtgeninterface.h"
#include <QThreadPool>
#include <QApplication>

FmtLibFactory<FmtGenInterface,QString> FmtGenInterface::m_pGenInterfaceFactory;
QEvent::Type FmtGenFinishEvent::EventType = QEvent::None;

FmtGenFinishEvent::FmtGenFinishEvent(const QByteArray &data)
    : QEvent((QEvent::Type)FmtGenFinishEvent::getEventRegType())
{
    _data = data;
}

int FmtGenFinishEvent::getEventRegType()
{
    if ((QEvent::Type)FmtGenFinishEvent::EventType == QEvent::None)
        FmtGenFinishEvent::EventType = (QEvent::Type)QEvent::registerEventType();

    return FmtGenFinishEvent::EventType;
}

//---------------------------------------------------------------------
class FmtGenInterfaceRunnable : public QRunnable
{
public:
    FmtGenInterfaceRunnable(QSharedPointer<FmtTable> pTable, FmtGenInterface *interface)
    {
        pInterface = interface;
        m_pTable = pTable;
        setAutoDelete(true);
    }

    virtual void run()
    {
        QByteArray data = pInterface->makeContent(m_pTable);
        QApplication::postEvent(pInterface, new FmtGenFinishEvent(data));
    }

private:
    FmtGenInterface *pInterface;
    QSharedPointer<FmtTable> m_pTable;
};

//---------------------------------------------------------------------

FmtGenInterface::FmtGenInterface()
{
}

void FmtGenInterface::start(QSharedPointer<FmtTable> pTable)
{
    if (!pTable.isNull())
        QThreadPool::globalInstance()->start(new FmtGenInterfaceRunnable(pTable, this));
    else
        emit finish(QByteArray());
}

bool FmtGenInterface::event(QEvent *e)
{
    bool result = false;

    if (e->type() == FmtGenFinishEvent::getEventRegType())
    {
        FmtGenFinishEvent *pEvent = (FmtGenFinishEvent*)e;
        emit finish(pEvent->data());
    }

    return result;
}

//---------------------------------------------------------------------

QStringList fmtGenInterfaces()
{
    return FmtGenInterface::m_pGenInterfaceFactory.getIds();
}

FmtGenInterface *fmtGenInterfaceCreate(const QString &alias)
{
    return FmtGenInterface::m_pGenInterfaceFactory.create(alias);
}
