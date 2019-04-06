#include "treecombobox.h"
#include <QTreeView>
#include <QHeaderView>
#include <QMouseEvent>

TreeComboBox::TreeComboBox(QWidget *parent) :
    QComboBox(parent)
{
    pView = new QTreeView(this);
    pView->setRootIsDecorated(false);
    pView->setUniformRowHeights(true);
    pView->setExpandsOnDoubleClick(false);

    setView(pView);
    pView->header()->hide();
    pView->viewport()->installEventFilter(this);
    setMaxVisibleItems(20);
}

bool TreeComboBox::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress && object == view()->viewport())
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QModelIndex index = view()->indexAt(mouseEvent->pos());
        if (!view()->visualRect(index).contains(mouseEvent->pos()))
            skipNextHide = true;
    }
    return false;
}

void TreeComboBox::showPopup()
{
    setRootModelIndex(QModelIndex());
    pView->expandAll();
    QComboBox::showPopup();
}

void TreeComboBox::hidePopup()
{
    QModelIndex index = view()->currentIndex();

    if (index.isValid() && (index.flags() & Qt::ItemIsSelectable) == Qt::ItemIsSelectable)
    {
        setRootModelIndex(view()->currentIndex().parent());
        QComboBox::setCurrentIndex(view()->currentIndex().row());
        emit modelIndexChanged(index);
    }

    if (skipNextHide)
        skipNextHide = false;
    else
        QComboBox::hidePopup();
}

void TreeComboBox::setCurrentIndex(const QModelIndex &index)
{
    if (index.isValid() && (index.flags() & Qt::ItemIsSelectable) == Qt::ItemIsSelectable)
    {
        setRootModelIndex(index.parent());
        QComboBox::setCurrentIndex(index.row());
    }
}
