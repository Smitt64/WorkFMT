#include "task.h"
#include "dbttoolwizard.h"
#include <exportobject.h>
#include <QApplication>

Task::Task(int argc, char *argv[], QObject *parent) : QObject(parent)
{
    args.reserve(argc);
    std::copy(argv + 0, argv + argc, std::back_inserter(args));

    parser.addHelpOption();
    parser.addVersionOption();

    connectionStringOption.reset(new QCommandLineOption(QStringList() << "cs" << "con-str",
                                                QApplication::translate("main", "Connect to database by connection string <constring>"),
                                                QApplication::translate("main", "constring")));
    connectionUnicode.reset(new QCommandLineOption(QStringList() << "uc" << "unicode",
                                                QApplication::translate("main", "Connect to unicode database"),
                                                QApplication::translate("main", "unicode")));

    exportOption.reset(new QCommandLineOption(QStringList() << "e" << "export",
                                              QApplication::translate("main", "Export tables mode")));
    importOption.reset(new QCommandLineOption(QStringList() << "i" << "import",
                                              QApplication::translate("main", "Import tables mode")));
    dirOption.reset(new QCommandLineOption(QStringList() << "d" << "edir",
                                   QApplication::translate("main", "Export to directory <exportdir>"),
                                   QApplication::translate("main", "exportdir")));
    dbtOption.reset(new QCommandLineOption(QStringList() << "dbt",
                                   QApplication::translate("main", "Tables to export <dbt>"),
                                   QApplication::translate("main", "dbt")));
    useOld.reset(new QCommandLineOption(QStringList() << "old",
                                   QApplication::translate("main", "Use old algorithm"),
                                   QApplication::translate("main", "old")));
    clobMode.reset(new QCommandLineOption(QStringList() << "clob",
                                          QApplication::translate("main", "Clob mode: 0 - Simplified (default), 1 - SimplifiedTrimmed, 2 - SplitFile"),
                                          QApplication::translate("main", "clob")));

    parser.addOption(*connectionUnicode.data());
    parser.addOption(*connectionStringOption.data());
    parser.addOption(*exportOption.data());
    parser.addOption(*importOption.data());
    parser.addOption(*dirOption.data());
    parser.addOption(*dbtOption.data());
    parser.addOption(*useOld.data());
    parser.addOption(*clobMode.data());

    sterr.reset(new QTextStream(stderr, QIODevice::WriteOnly));
    stout.reset(new QTextStream(stdout, QIODevice::WriteOnly));

    sterr->setCodec("cp866");
    stout->setCodec("cp866");
}

void Task::processError(const QString &str)
{
    *sterr << str << Qt::endl;
    sterr->flush();
}

void Task::processInfo(const QString &str)
{
    *stout << str << Qt::endl;
    stout->flush();
}

void Task::exportTableStart(const QString &str)
{
    *stout << tr("Начат экспорт таблицы %1")
           .arg(str) << Qt::endl;
    stout->flush();
}

void Task::importTableStart(const QString &str)
{
    *stout << tr("Начат импорт таблицы %1")
           .arg(str) << Qt::endl;
    stout->flush();
}

void Task::run()
{
    parser.process(args);

    bool fExport = parser.isSet(*exportOption.data());
    bool fImport = parser.isSet(*importOption.data());
    bool fOld = parser.isSet(*useOld.data());

    if (!fExport && !fImport)
    {
        if (fOld)
        {
            w = new DbMainWindow();
            w->setAttribute(Qt::WA_DeleteOnClose);
            connect(w, SIGNAL(destroyed(QObject*)), SIGNAL(finished()));
            w->show();
        }
        else
        {
            DbtToolWizard *w = new DbtToolWizard();
            w->setAttribute(Qt::WA_DeleteOnClose);
            connect(w, SIGNAL(destroyed(QObject*)), SIGNAL(finished()));
            w->show();
        }
    }
    else if (fExport)
    {
        exportTable();
        emit finished();
    }
    else if (fImport)
    {
        importTable();
        emit finished();
    }
}

void Task::exportTable()
{
    int stat = 0;
    *stout << tr("Запуск экспорта таблиц...") << Qt::endl;
    stout->flush();

    if (!parser.isSet(*connectionStringOption.data()))
    {
        *sterr << tr("Не задана строка подключения.") << Qt::endl;
        sterr->flush();
        return;
    }

    QString dsn;
    QString user;
    QString pswd;
    QString connectionString = parser.value(*connectionStringOption.data());
    stat = !ParseConnectionString(connectionString, user, pswd, dsn);

    if (!stat && !parser.isSet(*dirOption.data()))
    {
        *sterr << tr("Не указан каталог экспорта") << Qt::endl;
        sterr->flush();
        stat = 1;
    }

    if (!stat && !parser.isSet(*dbtOption.data()))
    {
        *sterr << tr("Не заданы таблицы для экспорта") << Qt::endl;
        sterr->flush();
        stat = 1;
    }

    if (!stat)
    {
        QString path = parser.value(*dirOption.data());
        *stout << tr("Параметры подключения: %1/%2@%3")
               .arg(user, pswd, dsn) << Qt::endl;
        stout->flush();

        *stout << tr("Каталог экспорта: %1")
               .arg(path) << Qt::endl;
        stout->flush();

        if (parser.isSet(*useOld.data()))
        {
            QScopedPointer<DBFileObject> w(new DBFileObject);
            connect(w.data(), SIGNAL(procError(QString)), SLOT(processError(QString)));
            connect(w.data(), SIGNAL(exportTableStart(QString)), SLOT(exportTableStart(QString)));

            QStringList dbts = parser.value(*dbtOption.data()).split(";");
            w->unload(user, pswd, dsn, path, dbts);

            *stout << tr("Обработка завершена: %1")
                   .arg(parser.value(*dbtOption.data())) << Qt::endl;
            stout->flush();
        }
        else
        {
            ExportObject::ClobMode mode = ExportObject::ClobMode_Simplified;

            if (parser.isSet(*clobMode.data()))
                mode = (ExportObject::ClobMode)parser.value(*clobMode).toInt();

            QStringList dbts = parser.value(*dbtOption.data()).split(";");
            QScopedPointer<ExportObject> w(new ExportObject);
            w->setConnectionInfo(user, pswd, dsn, parser.isSet(*connectionUnicode.data()));
            w->setClobMode(mode);
            w->exportTable(dbts, path);
        }
    }
}

void Task::importTable()
{
    int stat = 0;
    *stout << tr("Запуск импорта таблиц...") << Qt::endl;
    stout->flush();

    if (!parser.isSet(*connectionStringOption.data()))
    {
        *sterr << tr("Не задана строка подключения.") << Qt::endl;
        sterr->flush();
        return;
    }

    QString dsn;
    QString user;
    QString pswd;
    QString connectionString = parser.value(*connectionStringOption.data());
    stat = !ParseConnectionString(connectionString, user, pswd, dsn);

    /*if (!stat && !parser.isSet(*dirOption.data()))
    {
        *sterr << QString("Не указан каталог экспорта") << Qt::endl;
        sterr->flush();
        stat = 1;
    }*/

    if (!stat && !parser.isSet(*dbtOption.data()))
    {
        *sterr << tr("Не заданы файлы таблиц для импорта") << Qt::endl;
        sterr->flush();
        stat = 1;
    }

    if (!stat)
    {
        //QString path = parser.value(*dirOption.data());
        *stout << tr("Параметры подключения: %1/%2@%3")
               .arg(user, pswd, dsn) << Qt::endl;
        stout->flush();

        /**stout << tr("Каталог экспорта: %1")
               .arg(path) << Qt::endl;
        stout->flush();*/

        QScopedPointer<DBFileObject> w(new DBFileObject);
        connect(w.data(), SIGNAL(procError(QString)), SLOT(processError(QString)));
        connect(w.data(), SIGNAL(procInfo(QString)), SLOT(processInfo(QString)));
        connect(w.data(), SIGNAL(importTableStart(QString)), SLOT(importTableStart(QString)));

        QStringList dbts = parser.value(*dbtOption.data()).split(";");
        w->load(user, pswd, dsn, dbts);

        *stout << tr("Обработка завершена: %1")
               .arg(parser.value(*dbtOption.data())) << Qt::endl;
        stout->flush();
    }
}

