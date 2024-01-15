#include "fmtindextreeview.h"
#include "fmtindecesdelegate.h"
#include "fmtcore.h"

FmtIndexTreeView::FmtIndexTreeView(QWidget *parent)
    :QTreeView(parent),
      pFmtTable(Q_NULLPTR),
      m_fButtonsEnabled(true)
{
    setItemsExpandable(false);
    setRootIsDecorated(false);
    setUniformRowHeights(true);
    setMouseTracking(true);
    setIndentation(BTNS_OFFSET);
    setEditTriggers(QAbstractItemView::AllEditTriggers);

    pIndecesDelegate = new FmtIndecesDelegate;
    setItemDelegate(pIndecesDelegate);

    pRemoveButton = new QPushButton(this);
    pInsertButton = new QPushButton(this);

    pRemoveButton->setGeometry(0, 0, 25, 25);
    pRemoveButton->setStyleSheet(MinusButtonCss());
    pRemoveButton->setObjectName("pRemoveButton");
    pRemoveButton->setVisible(false);

    pInsertButton->setGeometry(0, 0, 25, 25);
    pInsertButton->setStyleSheet(PlusButtonCss());
    pInsertButton->setObjectName("pInsertButton");
    pInsertButton->setVisible(false);

    connect(pRemoveButton, SIGNAL(clicked(bool)), SLOT(RemoveButton()));
    connect(pInsertButton, SIGNAL(clicked(bool)), SLOT(InsertButton()));
}

void FmtIndexTreeView::MoveButton()
{
    QModelIndex index = indexAt(m_Point);

    if (!index.isValid())
    {
        pRemoveButton->setVisible(false);
        pInsertButton->setVisible(false);
        return;
    }

    pRemoveButton->setVisible(true);
    pInsertButton->setVisible(true);
    QRect rect = visualRect(index);

    QPoint RemovePos;
    QPoint InsertPos;

    RemovePos.setY(rect.y());
    InsertPos.setX(pInsertButton->width() / 2 + 2);
    InsertPos.setY(rect.y() + pInsertButton->height() / 2);

    pRemoveButton->move(viewport()->mapToParent(RemovePos));
    pInsertButton->move(viewport()->mapToParent(InsertPos));

    m_Index = indexAt(RemovePos);
}

void FmtIndexTreeView::setButtonsVisible(bool value)
{
    pRemoveButton->setVisible(value);
    pInsertButton->setVisible(value);

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

void FmtIndexTreeView::RemoveButton()
{
    model()->removeRow(m_Index.row(), m_Index.parent());

    if (m_fButtonsEnabled)
        MoveButton();

    expandAll();
}

void FmtIndexTreeView::InsertButton()
{
    model()->insertRow(m_Index.row(), m_Index);

    if (m_fButtonsEnabled)
        MoveButton();

    expandAll();
}
