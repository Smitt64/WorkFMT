#include "tablesdock.h"
#include "fmtcore.h"
#include "fmttablesmodel.h"
#include "tablesdockwidget.h"
#include <QtSql>

#define FILTER_TITLE "Флтр#"

class ColoredTabBar : public QTabBar
{
    QColor m_startColor;
public:
    ColoredTabBar(QWidget *parent = nullptr) :
        QTabBar(parent)
    {

    }

    virtual ~ColoredTabBar()
    {

    }

    void setStartColor(const QColor &color)
    {
        if (m_startColor != color)
        {
            m_startColor = color;
            update();
        }
    }

    QColor startColor() const { return m_startColor; }

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE
    {
        QPainter painter(this);
        painter.fillRect(rect(), Qt::white);

        if (m_startColor.isValid())
        {
            QRect rc = rect().adjusted(0, 0, 0, -1);

            painter.setPen(QPen(m_startColor, 3));
            painter.drawLine(rc.bottomLeft(), rc.bottomRight());
        }

        QTabBar::paintEvent(event);
    }
};

class ColoredTabWidget : public QTabWidget
{
public:
    ColoredTabWidget(QWidget *parent = nullptr) :
        QTabWidget(parent)
    {
        m_pTabBar = new ColoredTabBar();
        m_startColor = QColor(Qt::white);
        setTabBar(m_pTabBar);
    }

    void setStartColor(const QColor &color)
    {
        if (m_startColor != color)
        {
            m_startColor = color;
            m_pTabBar->setStartColor(m_startColor);
            update();
        }
    }

    QColor startColor() const { return m_startColor; }

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE
    {
        QTabWidget::paintEvent(event);
    }

private:
    QColor m_startColor;
    ColoredTabBar *m_pTabBar;
};

TablesDock::TablesDock(const QString &title, QWidget *parent, Qt::WindowFlags flags) :
    QDockWidget(title, parent, flags),
    pInfo(Q_NULLPTR),
    pDelegate(Q_NULLPTR),
    pEventFilter(Q_NULLPTR)
{
    pLayout = new QHBoxLayout();
    //pBtnContainer = new QFrame(this);
    pTabBar = new ColoredTabWidget(this);
    //pAddButton = new QPushButton(this);
    TablesDockWidget *tablesWidget = new TablesDockWidget();
    pWidget.append(tablesWidget);

    //pTabBar->setTabShape(QTabWidget::Triangular);
    pTabBar->setTabsClosable(true);
    pTabBar->setDocumentMode(true);
    pTabBar->addTab(tablesWidget, tr("Фильтр"));
    //pTabBar->setAutoFillBackground(false);
    //pTabBar->tabBar()->setAutoFillBackground(false);
    setObjectName("TablesDock");
    setWidget(pTabBar);

    //pAddButton->setStyleSheet(AddTabButtonCss());
    //pAddButton->setToolTip(tr("Новый фильтр"));

    /*pLayout = new QHBoxLayout();
    pLayout->addWidget(pAddButton);
    pLayout->setContentsMargins(0, 0, 0, 2);
    pBtnContainer->setLayout(pLayout);*/
    QTabBar *tabBar = pTabBar->findChild<QTabBar*>();
    tabBar->setTabButton(0, QTabBar::RightSide, Q_NULLPTR);
    //pTabBar->setCornerWidget(pBtnContainer);
    //pAddButton->setEnabled(false);

    connect(tablesWidget, SIGNAL(tableDbClicked(quint32)), SIGNAL(tableDbClicked(quint32)));
    //connect(pAddButton, &QPushButton::clicked, this, &TablesDock::addFilterTab);
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

    pTabBar->setStartColor(model->connection()->color());
}

void TablesDock::closeConnection()
{
    for (int i = pWidget.size() - 1; i >= 1; i--)
    {
        TablesDockWidget* widget = pWidget.takeAt(i);
        delete widget;
        pTabBar->removeTab(i);
    }

    pInfo = Q_NULLPTR;
    setModel(Q_NULLPTR);
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

    //pAddButton->setEnabled(true);
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
