#include "mdisubinterface.h"
#include "connectioninfo.h"
#include <SARibbon.h>

MdiSubInterface::MdiSubInterface(QWidget *parent) :
    QDialog(parent),
    pParentWnd(Q_NULLPTR),
    pConnection(Q_NULLPTR)
{

}

MdiSubInterface::~MdiSubInterface()
{
    clearRibbonTabs();
}

void MdiSubInterface::setParentWnd(QMdiSubWindow *wnd)
{
    pParentWnd = wnd;
}

ConnectionInfo *MdiSubInterface::connection() const
{
    return pConnection;
}

void MdiSubInterface::setConnection(ConnectionInfo *Connection)
{
    pConnection = Connection;
}

SARibbonContextCategory *MdiSubInterface::findCategoryByName(const QString &name)
{
    QList<SARibbonContextCategory*> lst = m_pRibbon->contextCategoryList();

    for (auto item : std::as_const(lst))
    {
        if (item->contextTitle() == name)
            return item;
    }

    return nullptr;
}

void MdiSubInterface::setRibbonBar(SARibbonBar *ribbon)
{
    m_pRibbon = ribbon;
    initRibbonPanels();
}

SARibbonBar *MdiSubInterface::ribbon()
{
    return m_pRibbon;
}

void MdiSubInterface::initRibbonPanels()
{

}

void MdiSubInterface::updateRibbonTabs()
{

}

void MdiSubInterface::clearRibbonTabs()
{

}

QList<QWidget*> MdiSubInterface::statusBarSections()
{
    return QList<QWidget*>();
}

/*QString MdiSubInterface::makeWindowTitle()
{
    QString schemeName = pTable->connection()->schemeName();
    return QString("%1@%2")
            .arg(schemeName.trimmed())
            .arg(pTable->name());
}*/

void MdiSubInterface::setupAction(QAction *act, const QString& text, const QString& iconname)
{
    act->setText(text);
    act->setIcon(QIcon::fromTheme(iconname));
    act->setObjectName(text);
}
