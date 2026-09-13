#include "fmtfieldstableview.h"
#include "fmtcore.h"
#include "fmtfildsmodel.h"
#include <QAbstractItemModel>
#include <QPainter>
#include <QStyleOptionButton>
#include <QApplication>

#define BTN_SIZE 22
#define REM_BTN_X_OFFSET 8
#define ADD_BTN_X_OFFSET 4

FmtFieldsTableView::FmtFieldsTableView(QWidget *parent) :
    QTableView(parent),
    pattern(new QPixmap(8, 8)),
    m_fDrawDeleteMark(false),
    m_fDrawInsertMark(false),
    m_fButtonsEnabled(true)
{
    pattern->fill(Qt::transparent);

    QPainter patternPainter(pattern.data());
    patternPainter.setPen(QPen(QColor(215, 0, 0, 160), 1));
    patternPainter.drawLine(0, 8, 8, 0);
    patternPainter.end();

    setEditTriggers(QAbstractItemView::AllEditTriggers);
    setMouseTracking(true);

    pInsertButton = new QPushButton(this);
    pInsertButton->setGeometry(0, 0, BTN_SIZE, BTN_SIZE);
    pInsertButton->setVisible(false);
    pInsertButton->setCursor(Qt::PointingHandCursor);

    // Стиль для кнопки добавления
    pInsertButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #217346;"
        "   border: 1px solid #1a5c38;"
        "   border-radius: 3px;"
        "   color: white;"
        "   font-weight: bold;"
        "   font-size: 14px;"
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

    pRemoveButton = new QPushButton(this);
    pRemoveButton->setGeometry(0, 0, BTN_SIZE, BTN_SIZE);
    pRemoveButton->setVisible(false);
    pRemoveButton->setCursor(Qt::PointingHandCursor);

    // Стиль для кнопки удаления
    pRemoveButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #d32f2f;"
        "   border: 1px solid #b71c1c;"
        "   border-radius: 3px;"
        "   color: white;"
        "   font-weight: bold;"
        "   font-size: 14px;"
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
    penDeleteMark.setColor(QColor(215, 0, 0)); // Ярко-красный

    penInsertMark.setWidth(2);
    penInsertMark.setColor(QColor(33, 115, 70)); // Основной зеленый цвет темы

    pInsertButton->setText("+");
    pRemoveButton->setText("−");

    connect(pInsertButton, SIGNAL(clicked(bool)), SLOT(InsertClicked()));
    connect(pRemoveButton, SIGNAL(clicked(bool)), SLOT(RemoveClicked()));
}

FmtFieldsTableView::~FmtFieldsTableView()
{

}

void FmtFieldsTableView::MoveButton()
{
    FmtFildsModel *m = (FmtFildsModel*)model();

    if (!m)
        return;

    qint32 columnCount = m->columnCount();
    quint32 rowCount = m->rowCount();
    int row = m_Index = rowAt(m_Point.y());

    if (rowCount == 1 || row < 0)
    {
        pInsertButton->setVisible(false);
        pRemoveButton->setVisible(false);
        return;
    }

    if (row == rowCount - 1)
    {
        pInsertButton->setVisible(false);
        pRemoveButton->setVisible(false);
        return;
    }

    pInsertButton->setVisible(true);
    pRemoveButton->setVisible(true);

    qint32 colWidth = 0, rHeight = 0;

    for (int i = 0; i < columnCount; i++)
        colWidth += columnWidth(i);
    for (int i = 0; i < row; i++)
        rHeight += rowHeight(i);

    QPoint posInsert, posRemove;
    posInsert.setX(colWidth - horizontalHeader()->offset() - pInsertButton->width() / 2 + ADD_BTN_X_OFFSET);
    posInsert.setY(rHeight + pInsertButton->height() / 2 - verticalHeader()->offset() - horizontalHeader()->height());

    posRemove.setX(colWidth - horizontalHeader()->offset() + REM_BTN_X_OFFSET);
    posRemove.setY(rHeight + (rowHeight(row) - pRemoveButton->height()) / 2 + rowHeight(row) - verticalHeader()->offset() - horizontalHeader()->height());

    pInsertButton->move(viewport()->mapToParent(posInsert));
    pRemoveButton->move(viewport()->mapToParent(posRemove));
}

void FmtFieldsTableView::setButtonsVisible(bool value)
{
    pInsertButton->setVisible(value);
    pRemoveButton->setVisible(value);
    m_fButtonsEnabled = value;
}

void FmtFieldsTableView::mouseMoveEvent(QMouseEvent *event)
{
    m_Point = event->pos();

    if (m_fButtonsEnabled)
        MoveButton();

    QTableView::mouseMoveEvent(event);
}

void FmtFieldsTableView::InsertClicked()
{
    model()->insertRow(m_Index);

    // Подсветка добавленной строки
    if (model() && m_Index < model()->rowCount())
    {
        QModelIndex idx = model()->index(m_Index, 0);
        if (idx.isValid())
        {
            selectionModel()->clearSelection();
            selectionModel()->select(idx, QItemSelectionModel::Select | QItemSelectionModel::Rows);
            scrollTo(idx);
        }
    }

    if (m_fButtonsEnabled)
        MoveButton();
}

void FmtFieldsTableView::RemoveClicked()
{
    // Запрос подтверждения удаления
    /*QMessageBox::StandardButton reply = QMessageBox::question(this, "Подтверждение",
                                                              "Удалить выбранную строку?",
                                                              QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        model()->removeRow(m_Index);

        if (m_fButtonsEnabled)
            MoveButton();
    }*/

    model()->removeRow(m_Index);

    if (m_fButtonsEnabled)
        MoveButton();
}

bool FmtFieldsTableView::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == pRemoveButton)
    {
        if (e->type() == QEvent::HoverEnter)
        {
            m_fDrawDeleteMark = true;
        }
        else if (e->type() == QEvent::HoverLeave)
        {
            m_fDrawDeleteMark = false;
        }
        viewport()->update();
    }
    else if (obj == pInsertButton)
    {
        if (e->type() == QEvent::HoverEnter)
        {
            m_fDrawInsertMark = true;
        }
        else if (e->type() == QEvent::HoverLeave)
        {
            m_fDrawInsertMark = false;
        }
        viewport()->update();
    }
    return QTableView::eventFilter(obj, e);
}

void FmtFieldsTableView::paintDeleteMark()
{
    QPainter p(viewport());
    p.setRenderHint(QPainter::Antialiasing);

    if (m_fDrawDeleteMark)
    {
        int btnY = pRemoveButton->y() - horizontalHeader()->height();
        int row = rowAt(btnY);

        if (row >= 0 && row < model()->rowCount())
        {
            QRect first = visualRect(model()->index(row, 0));
            QRect last = visualRect(model()->index(row, model()->columnCount() - 1));

            // Область выделения
            QRect selection(first.x(), first.y() - horizontalHeader()->offset(),
                            last.right() - first.x() + 1, last.height());
            selection.adjust(1, 1, -1, -1);

            // Создаем кисть с диагональным паттерном
            QBrush hatchBrush(QColor(215, 0, 0, 40)); // Базовый цвет

            // Красный полупрозрачный фон (как было)
            p.fillRect(selection, QColor(215, 0, 0, 40));

            // Устанавливаем кисть с паттерном
            hatchBrush.setTexture(*pattern.data());

            // Заливаем область паттерном
            p.fillRect(selection, hatchBrush);

            // Красная рамка
            p.setPen(penDeleteMark);
            p.drawRect(selection);
        }
    }

    if (m_fDrawInsertMark)
    {
        int btnY = pInsertButton->y() - horizontalHeader()->height();
        int row = rowAt(btnY);

        p.setPen(penInsertMark);

        if (row != -1)
        {
            QRect first = visualRect(model()->index(row, 0));
            QRect last = visualRect(model()->index(row, model()->columnCount() - 1));

            // Рисуем толстую зеленую линию под строкой
            int lineY = last.bottom() + 2;
            p.setPen(QPen(QColor(33, 115, 70), 3));
            p.drawLine(QLine(first.x(), lineY, last.right(), lineY));
        }
        else
        {
            QRect last = visualRect(model()->index(0, model()->columnCount() - 1));
            p.setPen(QPen(QColor(33, 115, 70), 3));
            p.drawLine(QLine(0, 2, last.right(), 2));
        }
    }
}

void FmtFieldsTableView::paintEvent(QPaintEvent *event)
{
    QTableView::paintEvent(event);
    paintDeleteMark();
}

void FmtFieldsTableView::leaveEvent(QEvent *event)
{
    // Скрываем кнопки при уходе мыши с таблицы
    if (m_fButtonsEnabled && !pInsertButton->underMouse() && !pRemoveButton->underMouse())
    {
        pInsertButton->setVisible(false);
        pRemoveButton->setVisible(false);
        m_fDrawDeleteMark = false;
        m_fDrawInsertMark = false;
        viewport()->update();
    }
    QTableView::leaveEvent(event);
}

void FmtFieldsTableView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Insert)
    {
        InsertClicked();
    }
    else if (event->key() == Qt::Key_Delete)
    {
        RemoveClicked();
    }
    else
    {
        QTableView::keyPressEvent(event);
    }
}
