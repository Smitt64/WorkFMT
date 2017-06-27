#include "tablecomboaction.h"
#include "toolbtncombo.h"
#include <QComboBox>

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
