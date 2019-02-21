#include "lineeditaction.h"
#include "fmtfildsmodel.h"
#include <QLineEdit>

LineEditAction::LineEditAction(QObject *parent) :
    QWidgetAction(parent),
    pLineEdit(Q_NULLPTR)
{

}

LineEditAction::~LineEditAction()
{

}

QWidget *LineEditAction::createWidget(QWidget *parent)
{
    pLineEdit = new QLineEdit(parent);
    pLineEdit->setClearButtonEnabled(true);
    pLineEdit->setMaximumWidth(300);
    connect(pLineEdit, SIGNAL(textChanged(QString)), SIGNAL(textChanged(QString)));

    return pLineEdit;
}

void LineEditAction::deleteWidget(QWidget *widget)
{
    widget->deleteLater();
}

void LineEditAction::setFocus()
{
    if (pLineEdit != Q_NULLPTR)
        pLineEdit->setFocus();
}
