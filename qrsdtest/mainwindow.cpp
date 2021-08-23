#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql>
#include <QSqlQuery>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QDir current(QDir::current());
    qApp->addLibraryPath(QFileInfo(QCoreApplication::applicationFilePath()).path());
    qApp->addLibraryPath(current.absolutePath());
    qApp->addLibraryPath(current.absoluteFilePath("sqldrivers"));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    for (const QString &drv : QSqlDatabase::drivers())
        ui->plainTextEdit->appendPlainText(drv);

    QSqlDatabase db = QSqlDatabase::addDatabase("qrsd");
    //db.setDatabaseName(ui->lineEdit->text());
    db.setUserName("SERP_3168");
    db.setPassword("SERP_3168");
    db.setDatabaseName("THOR_DB12DEV1");

    ui->plainTextEdit->appendPlainText("QSqlDatabase::addDatabase(qrsd)");

    if (db.open())
    {
        ui->plainTextEdit->appendPlainText("db.open() = true");

        QSqlQuery querycount(db);
        querycount.exec("select count(*) from fmt_names");
        querycount.next();
        qDebug() << "count: " << querycount.value(0).toInt();

        QSqlQuery query(db);
        //query.prepare("select * from fmt_names");//select * from fmt_names where t_name = 'dacb_group_tmp'
        //query.prepare("select /*t_name, t_oldparol, t_ctypeperson,*/ t_datelastmistake, t_timelastmistake from dperson_dbt");
        //query.prepare("select t_acctrnid, t_date_carry, t_ground, T_SUM_NATCUR from DACCTRN_DBT where t_acctrnid = ?");
        //query.addBindValue(47);
        query.prepare("select * from dperson_dbt "); // where t_datelastmistake = ?
        //query.addBindValue(QDate::fromString("09.02.2021", "dd.MM.yyyy"));

        if (query.exec())
        {
            ui->plainTextEdit->appendPlainText("query.exec() = true");
            int count = 0;
            while (query.next())
            {
                count++;
                ui->plainTextEdit->appendPlainText(query.value("t_name").toString());
                ui->plainTextEdit->appendPlainText(query.value("t_oldparol").toByteArray().toHex());
                //*/ui->plainTextEdit->appendPlainText(query.value("t_comment").toString());*/
                ui->plainTextEdit->appendPlainText(query.value("t_ctypeperson").toChar());
                ui->plainTextEdit->appendPlainText(query.value("t_datelastmistake").toDate().toString());
                ui->plainTextEdit->appendPlainText(query.value("t_timelastmistake").toTime().toString());
                /*/*ui->plainTextEdit->appendPlainText(QString::number(query.value("t_acctrnid").toInt()));
                ui->plainTextEdit->appendPlainText(query.value("t_date_carry").toDate().toString("dd.MM.yyyy"));
                ui->plainTextEdit->appendPlainText(query.value("t_ground").toString());
                ui->plainTextEdit->appendPlainText(QString::number(query.value("t_sum_natcur").toDouble()));*/
                ui->plainTextEdit->appendPlainText("-------------------------------------------");
            }
            qDebug() << "CalcCount: " << count;
        }
        else
        {
            ui->plainTextEdit->appendPlainText("query.exec() = false");
            ui->plainTextEdit->appendPlainText(query.lastError().text());
        }
    }
    else
    {
        ui->plainTextEdit->appendPlainText("db.open() = false");
        ui->plainTextEdit->appendPlainText(db.lastError().text());
    }
}
