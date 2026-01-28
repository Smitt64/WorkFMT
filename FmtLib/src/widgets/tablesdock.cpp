#include "tablesdock.h"
#include "fmtcore.h"
#include "fmttablesmodel.h"
#include "tablesdockwidget.h"
#include <QtSql>
#include <QToolButton>
#include <mdiproxystyle.h>

#define FILTER_TITLE "Флтр#"
#pragma once

#include <QToolButton>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QStyleOptionToolButton>

class AddTabToolButton : public QToolButton
{
public:
    explicit AddTabToolButton(QWidget *parent = nullptr)
        : QToolButton(parent)
        , m_hovered(false)
        , m_pressed(false)
    {
        setCursor(Qt::PointingHandCursor);
        //setFixedSize(16, 16);
        setToolTip(tr("Новый фильтр"));

        // Отключаем стандартную рамку фокуса
        setFocusPolicy(Qt::NoFocus);

        setAttribute(Qt::WA_TranslucentBackground);
        setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
        setAutoFillBackground(false);
    }

    virtual ~AddTabToolButton() = default;

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
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);

        // Прозрачный фон
        painter.fillRect(rect(), Qt::transparent);

        // Рисуем цветную линию по нижнему краю
        if (m_startColor.isValid() && m_startColor != Qt::transparent)
        {
            painter.setPen(QPen(m_startColor, 3));
            painter.drawLine(rect().bottomLeft(), rect().bottomRight());
        }
        else
        {
            painter.setPen(QPen(Qt::white, 3));
            painter.drawLine(rect().bottomLeft(), rect().bottomRight());
        }

        // Область для кнопки (немного меньше, чтобы была рамка)
        QRect buttonRect = rect().adjusted(1, 1, -1, -2);

        if (m_pressed) {
            // Нажатое - темно-зеленый фон, белый плюс
            painter.fillRect(buttonRect, QColor(33, 115, 70)); // #217346
            drawPlusIcon(painter, buttonRect, Qt::white);
        }
        else if (m_hovered) {
            // Наведение - светло-зеленый фон, темный плюс
            painter.fillRect(buttonRect, QColor(213, 240, 226)); // #d5f0e2
            painter.setPen(QPen(QColor(33, 115, 70), 1)); // #217346 рамка
            painter.drawRect(buttonRect.adjusted(0, 0, -1, -1));
            drawPlusIcon(painter, buttonRect, QColor(33, 115, 70)); // #217346
        }
        else {
            // Нормальное - прозрачный фон с серой рамкой и серым плюсом
            if (m_startColor.isValid())
                painter.setPen(QPen(m_startColor, 1));
            else
                painter.setPen(QPen(QColor(197, 210, 224), 1)); // #c5d2e0

            painter.drawRect(buttonRect.adjusted(0, 0, -1, -1));
            drawPlusIcon(painter, buttonRect, QColor(102, 102, 102)); // #666666
        }
    }

    void enterEvent(QEvent *event) Q_DECL_OVERRIDE
    {
        Q_UNUSED(event);
        m_hovered = true;
        update();
    }

    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE
    {
        Q_UNUSED(event);
        m_hovered = false;
        m_pressed = false;
        update();
    }

    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE
    {
        if (event->button() == Qt::LeftButton) {
            m_pressed = true;
            update();
        }
        QToolButton::mousePressEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE
    {
        if (event->button() == Qt::LeftButton) {
            m_pressed = false;
            update();
        }
        QToolButton::mouseReleaseEvent(event);
    }

private:
    QColor m_startColor;

    void drawNormalBackground(QPainter &painter, const QRect &rect)
    {
        // Градиент как в Office 2013
        QLinearGradient gradient(rect.topLeft(), rect.bottomLeft());
        gradient.setColorAt(0, QColor(248, 248, 248));
        gradient.setColorAt(1, QColor(240, 240, 240));

        painter.setPen(QPen(QColor(200, 200, 200), 1));
        painter.setBrush(gradient);
        painter.drawRect(rect.adjusted(0, 0, -1, -1));
    }

    void drawHoveredBackground(QPainter &painter, const QRect &rect)
    {
        // Градиент при наведении - светло-голубой
        QLinearGradient gradient(rect.topLeft(), rect.bottomLeft());
        gradient.setColorAt(0, QColor(235, 244, 253));
        gradient.setColorAt(1, QColor(216, 235, 253));

        painter.setPen(QPen(QColor(126, 180, 234), 1));
        painter.setBrush(gradient);
        painter.drawRect(rect.adjusted(0, 0, -1, -1));
    }

    void drawPressedBackground(QPainter &painter, const QRect &rect)
    {
        // Градиент при нажатии - более темный голубой
        QLinearGradient gradient(rect.topLeft(), rect.bottomLeft());
        gradient.setColorAt(0, QColor(203, 228, 253));
        gradient.setColorAt(1, QColor(175, 210, 243));

        painter.setPen(QPen(QColor(86, 157, 229), 1));
        painter.setBrush(gradient);
        painter.drawRect(rect.adjusted(0, 0, -1, -1));
    }

    void drawPlusIcon(QPainter &painter, const QRect &rect, const QColor &color)
    {
        painter.save();

        painter.setPen(QPen(color, 2));
        painter.setBrush(Qt::NoBrush);

        // Центрируем плюсик
        int centerX = rect.center().x();
        int centerY = rect.center().y();
        int lineLength = 8; // Длина линий плюса

        // Горизонтальная линия
        painter.drawLine(centerX - lineLength / 2, centerY,
                         centerX + lineLength / 2, centerY);

        // Вертикальная линия
        painter.drawLine(centerX, centerY - lineLength / 2,
                         centerX, centerY + lineLength / 2);

        painter.restore();
    }

private:
    bool m_hovered;
    bool m_pressed;
};

class ColoredTabBar : public QTabBar
{
    QColor m_startColor;
public:
    ColoredTabBar(QWidget *parent = nullptr) :
        QTabBar(parent)
    {
        setAttribute(Qt::WA_TranslucentBackground);
        setAutoFillBackground(false);
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
        QWidget *tabWidget = qobject_cast<QWidget*>(parent());
        painter.fillRect(tabWidget->rect(), Qt::white);

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
        setAutoFillBackground(false);

        setStyleSheet(
            "QTabWidget::pane {"
            "    border: 1px solid #c5d2e0;"
            "    border-top: none;"
            "    background: white;"
            "    margin: 0px;"
            "    padding: 0px;"
            "}"
            "QTabBar::tab {"
            "    background: #f8f8f8;"
            "    color: #333333;"
            "    border: 1px solid #c5d2e0;"
            "    border-bottom: none;"
            "    padding: 4px 10px;"
            "    margin-right: 1px;"
            "    font-size: 8pt;"
            "    font-family: 'Segoe UI';"
            "}"
            "QTabBar::tab:selected {"
            "    background: white;"
            "    color: #217346;"
            "    border: 1px solid #217346;"
            "    border-bottom: 1px solid white;"
            "}"
            "QTabBar::tab:hover:!selected {"
            "    background: #e6f2fc;"
            "    color: #217346;"
            "}"
            "QTabBar::close-button {"
            "    image: url(://img/svg/Close_16x.svg);"
            "    border: 1px solid transparent;"
            "    border-radius: 2px;"
            "    margin: 2px;"
            "}"
            "QTabBar::close-button:hover {"
            "    background: #d5f0e2;"
            "    border: 1px solid #c2e8d1;"
            "    image: url(://img/svg/Close_red_16x.svg);"
            "}"
            );
    }

    void setStartColor(const QColor &color)
    {
        if (m_startColor != color)
        {
            m_startColor = color;
            m_pTabBar->setStartColor(m_startColor);

            AddTabToolButton *pAddButton = dynamic_cast<AddTabToolButton*>(cornerWidget());

            if (pAddButton)
                pAddButton->setStartColor(m_startColor);

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
    pTabBar = new ColoredTabWidget(this);
    pAddButton = new AddTabToolButton(this);

    TablesDockWidget *tablesWidget = new TablesDockWidget();
    pWidget.append(tablesWidget);

    pTabBar->setTabsClosable(true);
    pTabBar->setDocumentMode(true);
    pTabBar->addTab(tablesWidget, tr("Фильтр"));

    setObjectName("TablesDock");
    setWidget(pTabBar);

    pAddButton->setToolTip(tr("Новый фильтр"));

    QTabBar *tabBar = pTabBar->findChild<QTabBar*>();
    tabBar->setTabButton(0, QTabBar::RightSide, Q_NULLPTR);
    pTabBar->setCornerWidget(pAddButton);
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

    if (model)
        pTabBar->setStartColor(model->connection()->color());
    else
        pTabBar->setStartColor(QColor());

    pTabBar->update();
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

    pAddButton->setStartColor(pInfo->color());
    pTabBar->setStartColor(pInfo->color());
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
