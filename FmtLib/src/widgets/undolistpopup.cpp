#include "undolistpopup.h"
#include <QDebug>
#include <QUndoView>

UndoListPopup::UndoListPopup(QObject *parent) :
    QWidgetAction(parent),
    pStack(NULL)
{
}

UndoListPopup::~UndoListPopup()
{

}

void UndoListPopup::deleteWidget(QWidget *widget)
{
    widget->deleteLater();
}

QWidget *UndoListPopup::createWidget(QWidget *parent)
{
    QUndoView *b = new QUndoView(parent);
    if (pStack) b->setStack(pStack);
    b->show();
    return b;
}

void UndoListPopup::setStack(QUndoStack *stack)
{
    pStack = stack;
}
