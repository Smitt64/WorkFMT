#include "cmdparser.h"
#include <QCommandLineParser>

CmdParser::CmdParser(QApplication* app): _app(app)
{
    std::fill(opts.begin(), opts.end(), TaskOption{false, ""});
}

CommandLineParseResult CmdParser::parse()
{
    const QString oracleParm = "ora";
    const QString postgresParm = "pg";
    const QString diffParm = "diff";
    const QString xmlParm = "xml";

    using Status = CommandLineParseResult::Status;

    QCommandLineParser parser;

    parser.setApplicationDescription("Программа создаёт SQL скрипты на основе изменений (различий) в *.dat файлах.\n"
                                     "Изменения должны иметь формат, который возврщает команда \"svn diff\".\n"
                                     "По умолчанию работает со стандартными входным и выходным потоками.\n\n"
                                     "Например, во воходной поток подается результат svn diff и созданый скрипт выводится на консоль:\n"
                                     "\tsvn diff DREGPARM_DBT.dat | DiffToScript\n"
                                     "Пример со всеми параметрами:\n"
                                     "\tDiffToScript --input d:\\1.txt --output d:\\2.txt --delete --insert --update --pg --log=*.waring=true,DiffToScript.Task.info=true");

    QCommandLineOption deleteScriptOption("delete", QCoreApplication::translate("main", "Создать sql скрипт для удаления."));
    parser.addOption(deleteScriptOption);

    QCommandLineOption insertScriptOption("insert", QCoreApplication::translate("main", "Создать sql скрипт для вставки."));
    parser.addOption(insertScriptOption);

    QCommandLineOption updateScriptOption("update", QCoreApplication::translate("main", "Создать sql скрипт для вставки."));
    parser.addOption(updateScriptOption);

    QCommandLineOption targetInputOption("input", "Файл, который содержит исходный данные.", "input_file");
    parser.addOption(targetInputOption);

    QCommandLineOption targetOutputOption("output", "Файл, в который нужно сохранить результат", "output_file");
    parser.addOption(targetOutputOption);

    QCommandLineOption datfileOption("dat", "Dat файл, который содержит исходный данные.", "dat_file");
    parser.addOption(datfileOption);

    QCommandLineOption connectionStringOption(QStringList() << "cs" << "con-str",
                                              QApplication::translate("main", "Подключиться к базе данных использую строку подключения <constring>"),
                                              QApplication::translate("main", "constring"));
    parser.addOption(connectionStringOption);

    QCommandLineOption unicodeConnectionOption("unicodedb", "Признак unicode базы данных.", "");
    parser.addOption(unicodeConnectionOption);

//    QCommandLineOption autoIncOption("autoinc", "0 - исключает автоинкрементные поля из результата, 1 - включает (по умолчанию)", "0|1");
//    parser.addOption(autoIncOption);

//    QCommandLineOption tableInfoOption("tabinfo", "Вывести информацию о таблице");
//    parser.addOption(tableInfoOption);

//    QCommandLineOption indexOption("index", "Номер индекса таблицы, по которому будет идентифицироваться запись. Должен быть уникальным", "number");
//    parser.addOption(indexOption);

//    QCommandLineOption verboseOption("verbose", "Вывод дополнительной информации");
//    parser.addOption(verboseOption);

    QCommandLineOption logOption("log", "Настройка вывода логов", "категория1=(true|false),категория2=(true|false),...");
    parser.addOption(logOption);

    QCommandLineOption oracleOption(oracleParm, "Sql скрипт для БД oracle");
    parser.addOption(oracleOption);

    QCommandLineOption postgresOption(postgresParm, "Sql скрипт для БД postgres");
    parser.addOption(postgresOption);

    QCommandLineOption diffOption(diffParm, "Данные различий svn diff");
    parser.addOption(diffOption);

    QCommandLineOption xmlOption(xmlParm, "Возвращать результат в формате xml");
    parser.addOption(xmlOption);

    const QCommandLineOption helpOption = parser.addHelpOption();

    parser.process(*_app);

    if (!parser.parse(QCoreApplication::arguments()))
        return { Status::Error, parser.errorText() };

    if (parser.isSet(helpOption))
        return { Status::HelpRequested };

    std::map<QString, QString> args;
    const QStringList positionalArguments = parser.positionalArguments();

    opts[ctoDelete].isSet = parser.isSet(deleteScriptOption);
    opts[ctoInsert].isSet = parser.isSet(insertScriptOption);
    opts[ctoUpdate].isSet = parser.isSet(updateScriptOption);
    opts[ctoInput].isSet = parser.isSet(targetInputOption);
    opts[ctoDatFile].isSet = parser.isSet(datfileOption);
    opts[ctoOutput].isSet = parser.isSet(targetOutputOption);
//    opts[ctoAutoInc].isSet = parser.isSet(autoIncOption);
//    opts[ctoTableInfo].isSet = parser.isSet(tableInfoOption);
//    opts[ctoIndex].isSet = parser.isSet(indexOption);
//    opts[ctoVerbose].isSet = parser.isSet(verboseOption);
    opts[ctoLog].isSet = parser.isSet(logOption);
    opts[ctoOracle].isSet = parser.isSet(oracleOption);
    opts[ctoPostgres].isSet = parser.isSet(postgresOption);
    opts[ctoConnectionString].isSet = parser.isSet(connectionStringOption);
    opts[ctoConnectionUnicode].isSet = parser.isSet(unicodeConnectionOption);
    opts[ctoDiffInfoMode].isSet = parser.isSet(diffOption);
    opts[ctoXml].isSet = parser.isSet(xmlOption);

    if (opts[ctoOracle].isSet & opts[ctoPostgres].isSet)
    {
        return { Status::Error, QString("Ошибка! Выбраны две взаимоисключающие опции: --%1, --%2").arg(oracleParm, postgresParm) };
    }

    if (opts[ctoInput].isSet)
        opts[ctoInput].value = parser.value(targetInputOption);

    if (opts[ctoOutput].isSet)
        opts[ctoOutput].value = parser.value(targetOutputOption);

    if (opts[ctoDatFile].isSet)
        opts[ctoDatFile].value = parser.value(datfileOption);
//    if (opts[ctoAutoInc].isSet)
//        opts[ctoAutoInc].value = parser.value(autoIncOption);
//    if (opts[ctoIndex].isSet)
//        opts[ctoIndex].value = parser.value(indexOption);
    if (opts[ctoLog].isSet)
        opts[ctoLog].value = parser.value(logOption);

    if (opts[ctoConnectionString].isSet)
        opts[ctoConnectionString].value = parser.value(connectionStringOption);

    // Значения по умолчанию
    if (!opts[ctoDelete].isSet && !opts[ctoInsert].isSet && !opts[ctoUpdate].isSet)
    {
        opts[ctoDelete].isSet = true;
        opts[ctoInsert].isSet = true;
        opts[ctoUpdate].isSet = true;
    }

    return { Status::Ok };
}

