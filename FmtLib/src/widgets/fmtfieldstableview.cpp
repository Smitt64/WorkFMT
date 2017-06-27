#include "fmtfieldstableview.h"
#include "fmtcore.h"
#include "fmtfildsmodel.h"
#include <QAbstractItemModel>
#include <QPainter>

#define BTN_SIZE 25
#define REM_BTN_X_OFFSET 10
#define ADD_BTN_X_OFFSET 5

FmtFieldsTableView::FmtFieldsTableView(QWidget *parent) :
    QTableView(parent),
    m_fDrawDeleteMark(false),
    m_fDrawInsertMark(false)
{
    setEditTriggers(QAbstractItemView::AllEditTriggers);
    setMouseTracking(true);

    pInsertButton = new QPushButton(this);
    pInsertButton->setGeometry(0, 0, BTN_SIZE, BTN_SIZE);
    pInsertButton->setVisible(false);
    pInsertButton->setStyleSheet(PlusButtonCss());

    pRemoveButton = new QPushButton(this);
    pRemoveButton->setGeometry(0, 0, BTN_SIZE, BTN_SIZE);
    pRemoveButton->setVisible(false);
    pRemoveButton->setStyleSheet(MinusButtonCss());

    pRemoveButton->installEventFilter(this);
    pInsertButton->installEventFilter(this);

    penDeleteMark.setWidth(2);
    penDeleteMark.setColor(QColor(Qt::red).darker());

    penInsertMark.setWidth(2);
    penInsertMark.setColor(QColor(Qt::blue).darker());

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

    //qDebug() << colWidth << verticalHeader()->offset();

    QPoint posInsert, posRemove;
    posInsert.setX(colWidth - horizontalHeader()->offset() - pInsertButton->width() / 2 + ADD_BTN_X_OFFSET);
    posInsert.setY(rHeight + pInsertButton->height() / 2 - verticalHeader()->offset() - horizontalHeader()->height());

    posRemove.setX(colWidth - horizontalHeader()->offset() + REM_BTN_X_OFFSET);
    posRemove.setY(rHeight + (rowHeight(row) - pRemoveButton->height()) / 2 + rowHeight(row) - verticalHeader()->offset() - horizontalHeader()->height());

    pInsertButton->move(viewport()->mapToParent(posInsert));
    pRemoveButton->move(viewport()->mapToParent(posRemove));
}

void FmtFieldsTableView::mouseMoveEvent(QMouseEvent *event)
{
    m_Point = event->pos();
    MoveButton();
    QTableView::mouseMoveEvent(event);
}

void FmtFieldsTableView::InsertClicked()
{
    model()->insertRow(m_Index);
    MoveButton();
}

void FmtFieldsTableView::RemoveClicked()
{
    model()->removeRow(m_Index);
    MoveButton();
}

bool FmtFieldsTableView::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == pRemoveButton)
    {
        if (e->type() == QEvent::HoverEnter)
            m_fDrawDeleteMark = true;
        else if (e->type() == QEvent::HoverLeave)
            m_fDrawDeleteMark = false;
        viewport()->update();
    }
    else if (obj == pInsertButton)
    {
        if (e->type() == QEvent::HoverEnter)
            m_fDrawInsertMark = true;
        else if (e->type() == QEvent::HoverLeave)
            m_fDrawInsertMark = false;
        viewport()->update();
    }
    return QTableView::eventFilter(obj, e);
}

void FmtFieldsTableView::paintDeleteMark()
{
    QPainter p(viewport());

    if (m_fDrawDeleteMark)
    {
        int btnY = pRemoveButton->y() - horizontalHeader()->height();
        int row = rowAt(btnY);
        QRect first = visualRect(model()->index(row, 0));
        QRect last = visualRect(model()->index(row, model()->columnCount() - 1));

        p.setPen(penDeleteMark);
        QRect selection(first.x() + 2, first.y() + 2 - horizontalHeader()->offset(), last.right() - 2, last.height() - 2);
        p.drawRect(selection);
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
            p.drawLine(QLine(first.x(), first.bottom() + 2, last.right(), last.bottom() + 2));
        }
        else
        {
            QRect last = visualRect(model()->index(0, model()->columnCount() - 1));
            p.drawLine(QLine(0, 2, last.right(), 2));
        }
    }
}

void FmtFieldsTableView::paintEvent(QPaintEvent *event)
{
    QTableView::paintEvent(event);
    paintDeleteMark();
}
