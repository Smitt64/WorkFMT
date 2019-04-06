#include "tablesdock.h"
#include "fmtcore.h"
#include "fmttablesmodel.h"
#include "tablesdockwidget.h"
#include <QtSql>

#define FILTER_TITLE "Флтр#"

TablesDock::TablesDock(const QString &title, QWidget *parent, Qt::WindowFlags flags) :
    QDockWidget(title, parent, flags),
    pInfo(Q_NULLPTR),
    pDelegate(Q_NULLPTR),
    pEventFilter(Q_NULLPTR)
{
    pLayout = new QHBoxLayout();
    pBtnContainer = new QFrame(this);
    pTabBar = new QTabWidget(this);
    pAddButton = new QPushButton(this);
    TablesDockWidget *tablesWidget = new TablesDockWidget();
    pWidget.append(tablesWidget);

    pTabBar->setTabShape(QTabWidget::Triangular);
    pTabBar->setTabsClosable(true);
    pTabBar->addTab(tablesWidget, tr("Фильтр"));
    setObjectName("TablesDock");
    setWidget(pTabBar);

    pAddButton->setStyleSheet(AddTabButtonCss());
    pAddButton->setToolTip(tr("Новый фильтр"));

    pLayout = new QHBoxLayout();
    pLayout->addWidget(pAddButton);
    pLayout->setContentsMargins(0, 0, 0, 2);
    pBtnContainer->setLayout(pLayout);

    QTabBar *tabBar = pTabBar->findChild<QTabBar*>();
    tabBar->setTabButton(0, QTabBar::RightSide, Q_NULLPTR);
    pTabBar->setCornerWidget(pBtnContainer);
    pAddButton->setEnabled(false);

    connect(tablesWidget, SIGNAL(tableDbClicked(quint32)), SIGNAL(tableDbClicked(quint32)));
    connect(pAddButton, &QPushButton::clicked, this, &TablesDock::addFilterTab);
    connect(pTabBar, &QTabWidget::tabCloseRequested, this, &TablesDock::tabCloseRequested);
}

TablesDock::~TablesDock()
{
}

void TablesDock::setEventFilter(QObject *objEventFilter)
{
    pEventFilter = objEventFilter;
}

void TablesDock::setModel(FmtTablesModel *model)
{
    TablesDockWidget *tablesWidget = pWidget[pTabBar->currentIndex()];
    tablesWidget->setModel(model);
}

TablesDockWidget *TablesDock::tablesWidget()
{
    int index = pTabBar->currentIndex();

    if (index < 0 || index >= pWidget.size())
        return Q_NULLPTR;

    return pWidget[index];
}

void TablesDock::setConnection(ConnectionInfo *info)
{
    for (int i = pWidget.size() - 1; i >= 1; i--)
    {
        TablesDockWidget* widget = pWidget.takeAt(i);
        delete widget;
        pTabBar->removeTab(i);
    }

    pInfo = info;

    for (int i = 0; i < pInfo->modelCount(); i++)
    {
        if (i == 0)
        {
            TablesDockWidget *tablesWidget = pWidget[0];
            tablesWidget->setModel(pInfo->getModel(0));
            tablesWidget->applyFilter(pInfo->getModel(0)->filterString());
            tablesWidget->installEventFilter(pEventFilter);
        }
        else
        {
            TablesDockWidget *tablesWidget = new TablesDockWidget();
            pWidget.append(tablesWidget);

            FmtTablesModel *model = pInfo->getModel(i);
            tablesWidget->setModel(model);
            tablesWidget->applyFilter(model->filterString());
            tablesWidget->installEventFilter(pEventFilter);
            pTabBar->addTab(tablesWidget, QString(FILTER_TITLE) + QString::number(pInfo->getFilterIndex(i)));
            connect(tablesWidget, SIGNAL(tableDbClicked(quint32)), SIGNAL(tableDbClicked(quint32)));
        }
    }

    pAddButton->setEnabled(true);
}

void TablesDock::setItemDelegate(QAbstractItemDelegate *delegate)
{
    pDelegate = delegate;
    foreach(TablesDockWidget *item, pWidget)
    {
        item->listView()->setItemDelegate(delegate);
    }
}

void TablesDock::addFilterTab()
{
    TablesDockWidget *tablesWidget = new TablesDockWidget();
    tablesWidget->listView()->setItemDelegate(pDelegate);
    pWidget.append(tablesWidget);

    FmtTablesModel *model = pInfo->addModel();
    model->updateFmtList();
    tablesWidget->setModel(model);
    tablesWidget->installEventFilter(pEventFilter);
    pTabBar->addTab(tablesWidget, QString(FILTER_TITLE) + QString::number(pInfo->lastIndex()));
    connect(tablesWidget, SIGNAL(tableDbClicked(quint32)), SIGNAL(tableDbClicked(quint32)));
    pTabBar->setCurrentIndex(pTabBar->count() - 1);
}

void TablesDock::tabCloseRequested(const int &index)
{
    TablesDockWidget* widget = pWidget.takeAt(index);
    delete widget;
    pInfo->deleteModel(index);
}

void TablesDock::forceRepaint()
{
    foreach(TablesDockWidget *item, pWidget)
    {
        item->listView()->viewport()->repaint();
    }
}
