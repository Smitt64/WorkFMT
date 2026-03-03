#include "fmtindextreeview.h"
#include "fmtindecesdelegate.h"
#include "fmtcore.h"
#include <QPainter>
#include <QMessageBox>

FmtIndexTreeView::FmtIndexTreeView(QWidget *parent)
    :QTreeView(parent),
    pattern(new QPixmap(8, 8)),
    pFmtTable(Q_NULLPTR),
    m_fDrawDeleteMark(false),
    m_fDrawInsertMark(false),
    m_fButtonsEnabled(true)
{
    // Создаем паттерн для диагональной штриховки
    pattern->fill(Qt::transparent);
    QPainter patternPainter(pattern.data());
    patternPainter.setPen(QPen(QColor(215, 0, 0, 160), 1));
    patternPainter.drawLine(0, 8, 8, 0);
    patternPainter.end();

    setItemsExpandable(false);
    setRootIsDecorated(false);
    setUniformRowHeights(true);
    setMouseTracking(true);
    setIndentation(BTNS_OFFSET);
    setEditTriggers(QAbstractItemView::AllEditTriggers);

    pIndecesDelegate = new FmtIndecesDelegate;
    setItemDelegate(pIndecesDelegate);

    // Кнопка добавления
    pInsertButton = new QPushButton(this);
    pInsertButton->setGeometry(0, 0, 25, 25);
    pInsertButton->setVisible(false);
    pInsertButton->setCursor(Qt::PointingHandCursor);
    pInsertButton->setToolTip("Добавить индекс");

    QPixmap plusPixmap(19, 19);
    plusPixmap.fill(Qt::transparent);
    QPainter plusPainter(&plusPixmap);
    plusPainter.setRenderHint(QPainter::Antialiasing);
    plusPainter.setPen(QPen(Qt::white, 2));
    plusPainter.drawLine(4, plusPixmap.height()/2, plusPixmap.width()-4, plusPixmap.height()/2);
    plusPainter.drawLine(plusPixmap.width()/2, 4, plusPixmap.width()/2, plusPixmap.height()-4);
    plusPainter.end();

    pInsertButton->setIcon(QIcon(plusPixmap));
    pInsertButton->setIconSize(QSize(19, 19));

    pInsertButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #217346;"
        "   border: 1px solid #1a5c38;"
        "   border-radius: 3px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2e8b57;"
        "   border: 1px solid #217346;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #1a5c38;"
        "   border: 1px solid #0f4229;"
        "}"
        );

    // Кнопка удаления
    pRemoveButton = new QPushButton(this);
    pRemoveButton->setGeometry(0, 0, 25, 25);
    pRemoveButton->setVisible(false);
    pRemoveButton->setCursor(Qt::PointingHandCursor);
    pRemoveButton->setToolTip("Удалить индекс");

    QPixmap minusPixmap(19, 19);
    minusPixmap.fill(Qt::transparent);
    QPainter minusPainter(&minusPixmap);
    minusPainter.setRenderHint(QPainter::Antialiasing);
    minusPainter.setPen(QPen(Qt::white, 2));
    minusPainter.drawLine(4, minusPixmap.height()/2, minusPixmap.width()-4, minusPixmap.height()/2);
    minusPainter.end();

    pRemoveButton->setIcon(QIcon(minusPixmap));
    pRemoveButton->setIconSize(QSize(19, 19));

    pRemoveButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #d32f2f;"
        "   border: 1px solid #b71c1c;"
        "   border-radius: 3px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #f44336;"
        "   border: 1px solid #d32f2f;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #b71c1c;"
        "   border: 1px solid #8e1a1a;"
        "}"
        );

    pInsertButton->installEventFilter(this);
    pRemoveButton->installEventFilter(this);

    // Настройка перьев для маркеров
    penDeleteMark.setWidth(2);
    penDeleteMark.setColor(QColor(215, 0, 0));

    penInsertMark.setWidth(2);
    penInsertMark.setColor(QColor(33, 115, 70));

    connect(pInsertButton, SIGNAL(clicked(bool)), SLOT(InsertButton()));
    connect(pRemoveButton, SIGNAL(clicked(bool)), SLOT(RemoveButton()));
}

FmtIndexTreeView::~FmtIndexTreeView()
{

}

void FmtIndexTreeView::MoveButton()
{
    QModelIndex index = indexAt(m_Point);

    if (!index.isValid())
    {
        pRemoveButton->setVisible(false);
        pInsertButton->setVisible(false);
        m_fDrawInsertMark = false;
        m_fDrawDeleteMark = false;
        return;
    }

    // Проверяем, можно ли удалять этот элемент
    bool canDelete = true; // Здесь можно добавить логику проверки

    pRemoveButton->setVisible(canDelete);
    pInsertButton->setVisible(true);

    QRect rect = visualRect(index);

    QPoint RemovePos;
    QPoint InsertPos;

    // Кнопка удаления - прикреплена к левому краю, Y совпадает с верхом элемента
    RemovePos.setX(0); // Левый край
    RemovePos.setY(rect.y());

    // Кнопка добавления - центр по X смещен, Y по центру кнопки относительно верха элемента
    InsertPos.setX(pInsertButton->width() / 2 + 2);
    InsertPos.setY(rect.y() + pInsertButton->height() / 2);

    pRemoveButton->move(viewport()->mapToParent(RemovePos));
    pInsertButton->move(viewport()->mapToParent(InsertPos));

    m_Index = index;
}

void FmtIndexTreeView::setButtonsVisible(bool value)
{
    pInsertButton->setVisible(value);
    pRemoveButton->setVisible(value);
    m_fButtonsEnabled = value;
}

void FmtIndexTreeView::mouseMoveEvent(QMouseEvent *event)
{
    m_Point = event->pos();
    pIndecesDelegate->setPoint(m_Point);

    if (m_fButtonsEnabled)
        MoveButton();

    QTreeView::mouseMoveEvent(event);
}

void FmtIndexTreeView::setFmtTable(FmtTable *table)
{
    pFmtTable = table;
    pIndecesDelegate->setFmtTable(table);
}

void FmtIndexTreeView::InsertButton()
{
    if (!m_Index.isValid())
        return;

    model()->insertRow(m_Index.row(), m_Index);

    if (m_fButtonsEnabled)
        MoveButton();

    expandAll();

    if (model() && m_Index.isValid() && m_Index.model()->rowCount(m_Index) > 0)
    {
        QModelIndex newIndex = m_Index.model()->index(0, 0, m_Index);
        if (newIndex.isValid())
        {
            selectionModel()->clearSelection();
            selectionModel()->select(newIndex, QItemSelectionModel::Select | QItemSelectionModel::Rows);
            scrollTo(newIndex);
        }
    }
}

void FmtIndexTreeView::RemoveButton()
{
    if (!m_Index.isValid())
        return;

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Подтверждение",
                                                              "Удалить выбранный индекс?",
                                                              QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        model()->removeRow(m_Index.row(), m_Index.parent());

        if (m_fButtonsEnabled)
            MoveButton();

        expandAll();
    }
}

bool FmtIndexTreeView::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == pInsertButton)
    {
        if (e->type() == QEvent::HoverEnter)
        {
            m_fDrawInsertMark = true;
            viewport()->update();
        }
        else if (e->type() == QEvent::HoverLeave)
        {
            m_fDrawInsertMark = false;
            viewport()->update();
        }
    }
    else if (obj == pRemoveButton)
    {
        if (e->type() == QEvent::HoverEnter)
        {
            m_fDrawDeleteMark = true;
            viewport()->update();
        }
        else if (e->type() == QEvent::HoverLeave)
        {
            m_fDrawDeleteMark = false;
            viewport()->update();
        }
    }
    return QTreeView::eventFilter(obj, e);
}

void FmtIndexTreeView::paintEvent(QPaintEvent *event)
{
    QTreeView::paintEvent(event);
    paintMarkers();
}

void FmtIndexTreeView::paintMarkers()
{
    QPainter p(viewport());
    p.setRenderHint(QPainter::Antialiasing);

    // Маркер удаления - на всю ширину строки
    if (m_fDrawDeleteMark && m_Index.isValid())
    {
        // Получаем регион для всей строки (все колонки)
        QRect rowRect;
        for (int col = 0; col < model()->columnCount(m_Index.parent()); ++col)
        {
            QModelIndex idx = model()->index(m_Index.row(), col, m_Index.parent());
            if (idx.isValid())
            {
                QRect colRect = visualRect(idx);
                if (rowRect.isNull())
                    rowRect = colRect;
                else
                    rowRect = rowRect.united(colRect);
            }
        }

        if (!rowRect.isNull())
        {
            // Красный полупрозрачный фон на всю строку
            p.fillRect(rowRect, QColor(215, 0, 0, 40));

            // Диагональная штриховка на всю строку
            QBrush hatchBrush(QColor(215, 0, 0, 40));
            hatchBrush.setTexture(*pattern.data());
            p.fillRect(rowRect, hatchBrush);

            // Красная рамка вокруг всей строки
            p.setPen(penDeleteMark);
            p.drawRect(rowRect.adjusted(1, 1, -1, -1));
        }
    }

    // Маркер вставки - зеленая линия на всю ширину строки
    if (m_fDrawInsertMark && m_Index.isValid())
    {
        // Получаем правую границу последней колонки
        int rightmost = 0;
        for (int col = 0; col < model()->columnCount(m_Index.parent()); ++col)
        {
            QModelIndex idx = model()->index(m_Index.row(), col, m_Index.parent());
            if (idx.isValid())
            {
                QRect colRect = visualRect(idx);
                rightmost = qMax(rightmost, colRect.right());
            }
        }

        QRect rect = visualRect(m_Index);

        // Зеленая линия под элементом на всю ширину
        int lineY = rect.bottom() + 2;
        p.setPen(QPen(QColor(33, 115, 70), 3));
        p.drawLine(rect.left(), lineY, rightmost, lineY);

        // Треугольник-указатель слева
        p.setPen(QPen(QColor(33, 115, 70), 2));
        p.setBrush(QColor(33, 115, 70));

        QPolygon arrow;
        arrow << QPoint(rect.left() - 10, lineY - 4)
              << QPoint(rect.left() - 2, lineY)
              << QPoint(rect.left() - 10, lineY + 4);
        p.drawPolygon(arrow);
    }
}

void FmtIndexTreeView::leaveEvent(QEvent *event)
{
    if (m_fButtonsEnabled && !pInsertButton->underMouse() && !pRemoveButton->underMouse())
    {
        pInsertButton->setVisible(false);
        pRemoveButton->setVisible(false);
        m_fDrawInsertMark = false;
        m_fDrawDeleteMark = false;
        viewport()->update();
    }
    QTreeView::leaveEvent(event);
}

void FmtIndexTreeView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Insert)
    {
        InsertButton();
    }
    else if (event->key() == Qt::Key_Delete)
    {
        RemoveButton();
    }
    else
    {
        QTreeView::keyPressEvent(event);
    }
}
