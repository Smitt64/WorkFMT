#include "tablestructsqldlg.h"
#include "ui_tablestructsqldlg.h"
#include "src/models/tableobjectsmodel.h"
#include "codeeditor/codeeditor.h"
#include "codeeditor/codehighlighter.h"
#include <QSettings>
#include <QTextStream>
#include "fmtapplication.h"

QString escapeSqlString(const QString &input)
{
    QString escapedString = input;

    // Экранируем одинарные кавычки (удваиваем их)
    escapedString.replace("'", "''");

    // Дополнительно можно экранировать другие символы, если это необходимо
    // Например, обратный слэш:
    escapedString.replace("\\", "\\\\");

    return escapedString;
}

QString generateIndent(int spaces)
{
    return QString(spaces, ' ');
}

TableStructSqlDlg::TableStructSqlDlg(ConnectionInfo *connection, const QString &tableName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableStructSqlDlg),
    m_pConnection(connection)
{
    ui->setupUi(this);

    // Загрузка настроек
    FmtApplication *fmtapp = qobject_cast<FmtApplication*>(qApp);
    if (fmtapp)
    {
        QSettings *settings = fmtapp->settings();
        if (settings)
        {
            // Загружаем состояние чекбоксов
            ui->checkSimplified->setChecked(settings->value("TableStructSqlDlg/simplified", false).toBool());
            ui->checkWordWrap->setChecked(settings->value("TableStructSqlDlg/wordWrap", true).toBool());

            // Применяем перенос строк
            ui->plainTextEdit->setWordWrapMode(ui->checkWordWrap->isChecked() ? QTextOption::WrapAtWordBoundaryOrAnywhere : QTextOption::NoWrap);
        }
    }

    m_pModel.reset(new TableObjectsModel(connection, tableName));
    ui->treeView->setModel(m_pModel.data());

    ToolApplyHighlighter(ui->plainTextEdit, HighlighterSql);

    ui->treeView->expandAll();
    connect(ui->treeView, &QTreeView::clicked, this, &TableStructSqlDlg::onTreeItemClicked);

    // Подключаем сигналы чекбоксов
    connect(ui->checkSimplified, &QCheckBox::stateChanged, this, &TableStructSqlDlg::saveSettings);
    connect(ui->checkWordWrap, &QCheckBox::stateChanged, this, [this](int state) {
        ui->plainTextEdit->setWordWrapMode(state ? QTextOption::WrapAtWordBoundaryOrAnywhere : QTextOption::NoWrap);
        saveSettings();
    });
}

TableStructSqlDlg::~TableStructSqlDlg()
{
    delete ui;
}

void TableStructSqlDlg::saveSettings()
{
    FmtApplication *fmtapp = qobject_cast<FmtApplication*>(qApp);
    if (fmtapp)
    {
        QSettings *settings = fmtapp->settings();
        if (settings)
        {
            // Сохраняем состояния чекбоксов
            settings->setValue("TableStructSqlDlg/simplified", ui->checkSimplified->isChecked());
            settings->setValue("TableStructSqlDlg/wordWrap", ui->checkWordWrap->isChecked());
        }
    }
}

void TableStructSqlDlg::onTreeItemClicked(const QModelIndex &index)
{
    QString sql = m_pModel->getSqlForIndex(index, ui->checkSimplified->isChecked());
    ui->plainTextEdit->setPlainText(sql);
}

QString TableStructSqlDlg::getObjectsSql()
{
    QString result;
    QTextStream stream(&result);
    TableObjectsInfoList list = m_pModel->getCheckedObjectsWithSql(ui->checkSimplified->isChecked());

    for (auto item : list)
    {
        if (item.itemType == RootItem)
            stream << wrapCreateTableExecuteImmediate(item.sql) << Qt::endl << Qt::endl;
        else if (item.itemType == IndexItem)
            stream << wrapIndexDdlInExecuteImmediate(item.sql) << Qt::endl << Qt::endl;
        else if (item.itemType == SequenceItem)
            stream << wrapSequenceDdlInExecuteImmediate(item.sql) << Qt::endl << Qt::endl;
        else if (item.itemType == TriggerItem)
            stream << wrapTriggerDdlInExecuteImmediate(item.sql) << Qt::endl << Qt::endl;
        else
            stream << item.sql << Qt::endl << Qt::endl;
    }

    return result;
}

QString TableStructSqlDlg::wrapCreateTableExecuteImmediate(const QString &sqlScript)
{
    QStringList lines = sqlScript.split('\n', Qt::SkipEmptyParts);
    QStringList executeLines;
    QString tableName;
    QStringList commentLines;
    bool isTemporary = false;
    QString onCommitClause;

    // Разбираем SQL на части
    for (int i = 0; i < lines.size(); ++i)
    {
        QString line = lines[i].trimmed();

        // Определяем временную таблицу
        if (line.startsWith("CREATE GLOBAL TEMPORARY TABLE"))
        {
            isTemporary = true;
            // Извлекаем имя таблицы
            int start = line.indexOf("TABLE") + 6;
            int end = line.indexOf('(', start);
            if (end == -1) end = line.length();
            tableName = line.mid(start, end - start).trimmed();

            // Проверяем ON COMMIT clause в текущей или следующей строке
            for (int j = i; j < lines.size(); ++j) {
                QString checkLine = lines[j].trimmed();
                if (checkLine.contains("ON COMMIT PRESERVE ROWS", Qt::CaseInsensitive)) {
                    onCommitClause = " ON COMMIT PRESERVE ROWS";
                    break;
                }
                else if (checkLine.contains("ON COMMIT DELETE ROWS", Qt::CaseInsensitive)) {
                    onCommitClause = " ON COMMIT DELETE ROWS";
                    break;
                }
            }

            // Формируем EXECUTE IMMEDIATE для CREATE TABLE
            QString createSql = "        'CREATE GLOBAL TEMPORARY TABLE " + tableName + "' || \n";
            createSql += "        '(' || \n";

            // Получаем список колонок
            QStringList columns;
            for (int k = i + 1; k < lines.size(); ++k)
            {
                QString innerLine = lines[k].trimmed();
                if (innerLine.startsWith(")"))
                {
                    // Проверяем ON COMMIT если не нашли ранее
                    if (onCommitClause.isEmpty() && innerLine.contains("ON COMMIT"))
                    {
                        if (innerLine.contains("PRESERVE ROWS", Qt::CaseInsensitive))
                            onCommitClause = " ON COMMIT PRESERVE ROWS";
                        else if (innerLine.contains("DELETE ROWS", Qt::CaseInsensitive))
                            onCommitClause = " ON COMMIT DELETE ROWS";
                    }
                    break;
                }

                if (!innerLine.isEmpty() && !innerLine.startsWith("--"))
                {
                    QString columnDef = innerLine
                            .replace("NULL,", ",")
                            .replace("NULL", "")
                            .replace(" ,", ",")
                            .replace(";", "");
                    if (columnDef.endsWith(","))
                        columnDef.chop(1);
                    columns << escapeSqlString(columnDef.trimmed());
                }
            }

            createSql += "        '" + columns.join(",' || \n        '") + "' || \n";
            createSql += "        ')" + onCommitClause + "'";
            executeLines << createSql;
            break;
        }
        else if (line.startsWith("CREATE TABLE"))
        {
            // Обычная таблица
            int start = line.indexOf("TABLE") + 6;
            int end = line.indexOf('(', start);
            if (end == -1) end = line.length();
            tableName = line.mid(start, end - start).trimmed();

            // Формируем EXECUTE IMMEDIATE
            QString createSql = "        'CREATE TABLE " + tableName + "' || \n";
            createSql += "        '(' || \n";

            QStringList columns;
            for (int k = i + 1; k < lines.size(); ++k) {
                QString innerLine = lines[k].trimmed();
                if (innerLine.startsWith(")")) break;

                if (!innerLine.isEmpty() && !innerLine.startsWith("--"))
                {
                    QString columnDef = innerLine
                            .replace("NULL,", ",")
                            .replace("NULL", "")
                            .replace(" ,", ",")
                            .replace(";", "");
                    if (columnDef.endsWith(","))
                        columnDef.chop(1);
                    columns << escapeSqlString(columnDef.trimmed());
                }
            }

            createSql += "        '" + columns.join(",' || \n        '") + "' || \n";
            createSql += "        ')'";
            executeLines << createSql;
            break;
        }
    }

    // Собираем комментарии
    for (const QString &line : lines)
    {
        QString trimmed = line.trimmed();
        if (trimmed.startsWith("COMMENT ON"))
        {
            commentLines << trimmed.replace(";", "").replace("'", "''");
        }
    }

    // Формируем полный PL/SQL блок
    QString result =
            "DECLARE \n"
            "    e_object_exists EXCEPTION;\n"
            "    PRAGMA EXCEPTION_INIT(e_object_exists, -955); \n"
            "BEGIN\n";

    // Добавляем CREATE TABLE
    if (!executeLines.isEmpty())
        result += "    EXECUTE IMMEDIATE \n" + executeLines.first() + ";\n\n";

    // Добавляем комментарии
    for (const QString &comment : commentLines)
    {
        result += "    EXECUTE IMMEDIATE '" + comment + "';\n";
    }

    result +=
            "\nEXCEPTION \n"
            "    WHEN e_object_exists THEN NULL; \n"
            "END;\n"
            "/";

    return result;
}

QString TableStructSqlDlg::wrapIndexDdlInExecuteImmediate(const QString &indexDdl)
{
    // Улучшенное регулярное выражение для квотированных имен
    QRegularExpression createIndexRegex(
                R"(CREATE\s+(UNIQUE\s+)?INDEX\s+("?\w+"?)\s+ON\s+("?\w+"?)\s*\(([^)]+)\))",
                QRegularExpression::CaseInsensitiveOption
                );

    QRegularExpressionMatch match = createIndexRegex.match(indexDdl);

    if (!match.hasMatch()) {
        return indexDdl; // Если не удалось распарсить, возвращаем как есть
    }

    QString indexName = match.captured(2);
    QString tableName = match.captured(3);
    QString columnsPart = match.captured(4).trimmed();

    QString tmp = indexDdl;

    // Формируем блоки EXECUTE IMMEDIATE
    QString result =
            QString("BEGIN\n"
                    "  EXECUTE IMMEDIATE 'DROP INDEX %1';\n"
                    "EXCEPTION\n"
                    "  WHEN OTHERS THEN NULL;\n"
                    "END;\n"
                    "/\n\n"
                    "BEGIN\n"
                    "  EXECUTE IMMEDIATE '%2';\n"
                    "EXCEPTION\n"
                    "  WHEN OTHERS THEN NULL;\n"
                    "END;\n"
                    "/")
            .arg(indexName)
            .arg(tmp.replace("'", "''")); // Экранируем кавычки в оригинальном DDL

    return result;
}

QString TableStructSqlDlg::wrapSequenceDdlInExecuteImmediate(const QString &sequenceDdl)
{
    // Регулярное выражение для извлечения имени последовательности и параметров
    QRegularExpression sequenceRegex(
                R"(CREATE\s+SEQUENCE\s+("?[\w_]+\"?)\s*(.*))",
                QRegularExpression::CaseInsensitiveOption
                );

    QRegularExpressionMatch match = sequenceRegex.match(sequenceDdl);

    if (!match.hasMatch())
        return sequenceDdl; // Если не удалось распарсить, возвращаем как есть

    QString sequenceName = match.captured(1);
    QString sequenceParams = match.captured(2).trimmed();

    // Удаляем START WITH из параметров
    sequenceParams.remove(QRegularExpression(
                              R"(\s*START\s+WITH\s+\d+\s*)",
                              QRegularExpression::CaseInsensitiveOption
                              ));

    // Формируем блоки EXECUTE IMMEDIATE
    QString result =
            QString("BEGIN\n"
                    "  EXECUTE IMMEDIATE 'DROP SEQUENCE %1';\n"
                    "EXCEPTION\n"
                    "  WHEN OTHERS THEN NULL;\n"
                    "END;\n"
                    "/\n\n"
                    "BEGIN\n"
                    "  EXECUTE IMMEDIATE 'CREATE SEQUENCE %1 %2';\n"
                    "EXCEPTION\n"
                    "  WHEN OTHERS THEN NULL;\n"
                    "END;\n"
                    "/")
            .arg(sequenceName)
            .arg(sequenceParams.replace("'", "''").trimmed());

    return result;
}

QString TableStructSqlDlg::wrapTriggerDdlInExecuteImmediate(const QString &triggerDdl)
{
    QString result;
    QStringList parts = triggerDdl.split("/", Qt::SkipEmptyParts);

    // Обрабатываем CREATE OR REPLACE TRIGGER
    if (parts.size() > 0)
    {
        QString createTriggerPart = parts[0].trimmed();
        if (!createTriggerPart.isEmpty())
            result += createTriggerPart + "\n/\n";
    }

    // Обрабатываем ALTER TRIGGER (если есть)
    if (parts.size() > 1)
    {
        QString alterTriggerPart = parts[1].trimmed();
        if (alterTriggerPart.startsWith("ALTER TRIGGER", Qt::CaseInsensitive))
        {
            // Извлекаем имя триггера
            QRegularExpression alterRegex(
                        R"(ALTER\s+TRIGGER\s+("?[\w_]+\"?)\s+(ENABLE|DISABLE))",
                        QRegularExpression::CaseInsensitiveOption
                        );

            QRegularExpressionMatch match = alterRegex.match(alterTriggerPart);
            if (match.hasMatch())
            {
                QString triggerName = match.captured(1);
                QString action = match.captured(2).toUpper();

                // Оборачиваем ALTER TRIGGER в EXECUTE IMMEDIATE
                result += QString("\nBEGIN\n"
                                  "  EXECUTE IMMEDIATE 'ALTER TRIGGER %1 %2';\n"
                                  "EXCEPTION\n"
                                  "  WHEN OTHERS THEN NULL;\n"
                                  "END;\n"
                                  "/")
                        .arg(triggerName)
                        .arg(action);
            } else
            {
                // Если не удалось распарсить, добавляем как есть
                result += alterTriggerPart + "\n/";
            }
        }
        else
        {
            // Если это не ALTER TRIGGER, добавляем как есть
            result += alterTriggerPart + "\n/";
        }
    }

    return result.trimmed();
}
