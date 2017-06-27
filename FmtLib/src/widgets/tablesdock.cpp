#include "tablesdock.h"
#include "fmtcore.h"
#include "fmttablesmodel.h"
#include "tablesdockwidget.h"
#include <QtSql>

TablesDock::TablesDock(const QString &title, QWidget *parent, Qt::WindowFlags flags) :
    QDockWidget(title, parent, flags)
{
    pWidget = new TablesDockWidget();
    setObjectName("TablesDock");
    setWidget(pWidget);

    connect(pWidget, SIGNAL(tableDbClicked(quint32)), SIGNAL(tableDbClicked(quint32)));
}

TablesDock::~TablesDock()
{
}

void TablesDock::setModel(FmtTablesModel *model)
{
    pWidget->setModel(model);
}

TablesDockWidget *TablesDock::tablesWidget()
{
    return pWidget;
}
