#include "tablecomboaction.h"
#include "toolbtncombo.h"
#include <QComboBox>
#include <QMenu>
#include <QDebug>

TableComboAction::TableComboAction(QObject *parent)
    : QWidgetAction(parent)
{
    m_Col = 0;
}

void TableComboAction::deleteWidget(QWidget *widget)
{
    widget->deleteLater();
}

QWidget *TableComboAction::createWidget(QWidget *parent)
{
    QComboBox *b = new ToolBtnCombo(parent);
    connect(b, SIGNAL(activated(int)), SIGNAL(activated(int)));
    connect(b, SIGNAL(activated(int)), SLOT(onComboSelected()));

    if (pModel)
    {
        b->setModel(pModel);
        b->setModelColumn(m_Col);
    }
    return b;
}

void TableComboAction::setModel(QAbstractItemModel *model)
{
    pModel = model;
}

void TableComboAction::setDisplayColumn(const quint32 &col)
{
    m_Col = col;
}

void TableComboAction::onComboSelected()
{
    QComboBox *combo = qobject_cast<QComboBox*>(sender());

    if (combo)
    {
        QMenu *parent = qobject_cast<QMenu*>(combo->parent());
        if (parent)
            parent->close();
    }
}
