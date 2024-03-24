#include "task.h"
#include "diffparser.h"
#include "sqlbuilder.h"
#include "tableinfo.h"

#include <QTextStream>
#include <QFile>

Task::Task(QObject *parent) : QObject(parent)
{

}


void Task::run()
{
    QFile data("d:\\1.txt");
    data.open(QFile::ReadOnly);

    QString buff = QTextStream(&data).readAll();
    QTextStream is(&buff);

    DiffParser dp;
    dp.parseDoc(is);

    TableInfo ti;
    ti.load("dopusymb_dbt");

    QTextStream os(stdout);
    SqlBuilder sb;
    sb.buildInsert(os, dp.getInsList(), dp.getTableName());

    emit finished();
}
