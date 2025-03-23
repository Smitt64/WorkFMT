#include "fmtcontenttreeitem.h"
#include "hotfixcontentmodel.h"
#include "toolsruntime.h"
#include <QIcon>
#include <QFileInfo>
#include <QTextStream>
#include <QDir>
#include <QFile>
#include <QTemporaryFile>
#include <QProcess>
#include <QInputDialog>

#define TableComment "COMMENT ON TABLE"
#define ColumnComment "COMMENT ON COLUMN"

QString generateIndent(int spaces)
{
    return QString(spaces, ' ');
}

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

FmtContentTreeItem::FmtContentTreeItem(const QString &file, ContentTreeItem *parentItem) :
    FileContentTreeItem(file, parentItem),
    m_Elem(ElementTable),
    m_Width(0)
{
}

FmtContentTreeItem::~FmtContentTreeItem()
{

}

void FmtContentTreeItem::setElementType(const FmtElement &type)
{
    m_Elem = type;
}

void FmtContentTreeItem::setWidth(const int &w)
{
    m_Width = w;
}

QString FmtContentTreeItem::removeComments(const QString &input)
{
    QString result;
    int length = input.length();
    bool inSingleLineComment = false; // Флаг для однострочного комментария
    bool inMultiLineComment = false;  // Флаг для многострочного комментария

    for (int i = 0; i < length; ++i)
    {
        // Проверяем начало многострочного комментария /*
        if (!inSingleLineComment && !inMultiLineComment && input[i] == '/' && i + 1 < length && input[i + 1] == '*')
        {
            inMultiLineComment = true;
            i++; // Пропускаем следующий символ '*'
            continue;
        }

        // Проверяем конец многострочного комментария */
        if (inMultiLineComment && input[i] == '*' && i + 1 < length && input[i + 1] == '/')
        {
            inMultiLineComment = false;
            i++; // Пропускаем следующий символ '/'
            continue;
        }

        // Проверяем начало однострочного комментария --
        if (!inSingleLineComment && !inMultiLineComment && input[i] == '-' && i + 1 < length && input[i + 1] == '-')
        {
            inSingleLineComment = true;
            i++; // Пропускаем следующий символ '-'
            continue;
        }

        // Проверяем конец однострочного комментария (новая строка)
        if (inSingleLineComment && input[i] == '\n')
        {
            inSingleLineComment = false;
        }

        // Если мы не внутри комментария, добавляем символ к результату
        if (!inSingleLineComment && !inMultiLineComment)
        {
            result += input[i];
        }
    }

    return result;
}

QString FmtContentTreeItem::WrapCreateTableBlock(const QString &sql)
{
    QString result;
    QTextStream stream(&result);
    QString clearedSql = removeComments(sql).trimmed().replace("\r\n", "\n").replace("\t", generateIndent(4));

    int posTableComment = clearedSql.indexOf(TableComment, 0, Qt::CaseInsensitive);
    int posColumnComment = clearedSql.indexOf(ColumnComment, 0, Qt::CaseInsensitive);

    int maxLength = 0;
    int pos = qMin(posTableComment, posColumnComment);
    QString tableScript;

    if (pos != -1)
        tableScript = clearedSql.mid(0, pos).trimmed();

    if (tableScript.back() == ";")
        tableScript.chop(1);

    QStringList table = tableScript.split("\n");
    for (const QString &str : qAsConst(table))
        maxLength = qMax(maxLength, str.length()); // Используем qMax для сравнения

    stream << "DECLARE" << Qt::endl;
    stream << generateIndent(4) << "e_object_exists EXCEPTION;" << Qt::endl;
    stream << generateIndent(4) << "PRAGMA EXCEPTION_INIT(e_object_exists, -955);" << Qt::endl;
    stream << "BEGIN" << Qt::endl;
    stream << generateIndent(4) << "EXECUTE IMMEDIATE " << Qt::endl;

    for (int i = 0; i < table.size(); ++i)
    {
        // Обрамляем строку в одинарные кавычки
        QString formattedString = "'" + escapeSqlString(table[i]);

        // Выравниваем строку по ширине maxLength
        formattedString = formattedString.leftJustified(maxLength + 2, ' ') + "'"; // +2 для учета кавычек

        // Добавляем " ||" в конце, если это не последняя строка
        if (i < table.size() - 1)
            formattedString += " ||";
        else
            formattedString += ";";

        // Записываем строку в QTextStream
        stream << generateIndent(4 * 2) << formattedString << Qt::endl;
    }

    if (pos != -1)
    {
        QString commentScript = clearedSql.mid(pos).trimmed();
        stream << Qt::endl;

        QStringList comments = commentScript.split("\n");

        for (const QString &str : qAsConst(comments))
        {
            stream << generateIndent(4) << "EXECUTE IMMEDIATE '";

            if (str.trimmed().back() == ";")
                stream << escapeSqlString(str.mid(0, str.trimmed().size() - 1));
            else
                stream << escapeSqlString(str.trimmed());

            stream << "';" << Qt::endl;
        }

        stream << Qt::endl;
    }

    stream << "EXCEPTION" << Qt::endl;
    stream << generateIndent(4) << "WHEN e_object_exists THEN NULL;" << Qt::endl;
    stream << "END;" << Qt::endl << "/";

    return result;
}

QVariant FmtContentTreeItem::data(const int &column, const int &role) const
{
    if (column == HotfixContentModel::ColumnName)
    {
        if (role == Qt::DecorationRole)
        {
            if (m_Elem == ElementTable)
                return QIcon("://img/fmtable.png");
            else
                return QIcon("://img/index.png");
        }
        else if (role == Qt::DisplayRole)
        {
            if (m_Elem == ElementTable)
            {
                QFileInfo fi(fileName());
                return fi.baseName().toUpper();
            }
        }
    }
    else if (column == HotfixContentModel::ColumnAction)
    {
        if (role == Qt::DisplayRole)
        {
            if (m_Elem == ElementIndex)
                return QObject::tr("create index");
            else
            {
                if (svnAction() == "modified")
                    return "alter table";
                else
                    return "create table";
            }
        }
    }

    return FileContentTreeItem::data(column, role);
}

MakeResult FmtContentTreeItem::makeTable(const MakeAction &action, QString &msg, const MakeParams &params) const
{
    if (action == ActionMake )
    {
        if (svnAction() == "modified")
        {
            QStringList args = { "diff", fullFileName() };

            QTemporaryFile tmp;
            if (!tmp.open())
            {
                msg = "Ошибка создания временного файла с результатом <b>svn diff</b>";
                return ResultFail;
            }

            QScopedPointer<QProcess> process(new QProcess());
            process->setWorkingDirectory(params[PARAM_SOURCE_DIR].toString());
            toolStartProcess(process.data(), "svn.exe", args, true, true);

            tmp.write(process->readAllStandardOutput());
            tmp.close();

            QStringList argtmpl;
            argtmpl << "--updtbl"
                    << "--delete"
                    << "--insert"
                    << "--update"
                    << "--input"
                    << tmp.fileName();

            QScopedPointer<QProcess> proc(new QProcess);
            int ExitCode = toolStartProcess(proc.data(), "DiffToScript.exe", argtmpl, true, true);

            if (ExitCode)
            {
                QByteArray errdata = proc->readAllStandardError();
                QTextStream stream(&errdata);
                stream.setCodec("IBM 866");

                msg = stream.readAll();
            }
            else
            {
                QByteArray errdata = proc->readAllStandardOutput();

                QFile f(fileName() + ".sql");
                f.open(QIODevice::WriteOnly);
                f.write(errdata);
                f.close();
            }
        }
        else
        {
            QString inscript = toolReadTextFileContent(fullFileName(), "IBM-866");
            if (inscript.isEmpty())
            {
                msg = QString("Ошибка сохранения скрипта <b>%1</b> создания таблицы");
                return ResultFail;
            }

            QFile f(fileName() + ".sql");
            if (!f.open(QIODevice::WriteOnly))
            {
                msg = QString("Ошибка сохранения скрипта <b>%1</b> создания таблицы");
                return ResultFail;
            }

            QTextStream stream(&f);
            stream.setCodec("IBM 866");

            QString sql = WrapCreateTableBlock(inscript);
            stream << sql;
            f.close();

        }
    }
    return ResultSuccess;
}

MakeResult FmtContentTreeItem::make(const MakeAction &action, QString &msg, const MakeParams &params) const
{
    if (checkable() && checkState() != Qt::Checked && checkState() != Qt::PartiallyChecked)
        return ResultSuccess;

    if (m_Elem == ElementTable)
        return makeTable(action, msg, params);

    if (action == ActionMake)
    {
        QDir d(fileName());
        QFileInfo fi(fileName());

        QString dbtname = fi.baseName().toLower();
        dbtname = dbtname.mid(1);

        int pos = dbtname.lastIndexOf("_");
        if (pos != -1)
            dbtname = dbtname.replace(pos, 1, ".");

        d.cdUp();
        QFile file(d.absoluteFilePath("create_index.cmd"));
        if (!file.open(QIODevice::Append | QIODevice::Text))
        {
            msg = QString("Не удалось добавить таблицу <b>%1</b> в файл <b>%2</b>").arg(dbtname, file.fileName());
            return ResultWarning;
        }

        QTextStream stream(&file);
        stream.setCodec("IBM 866");
        stream.setFieldAlignment(QTextStream::AlignLeft);

        stream << "dbinit %user%:%pass%@%dsn% -dt:ora -log:\".\\log\\";
        stream.setFieldWidth(m_Width + 5);
        stream << dbtname + ".log\"";
        stream.setFieldWidth(0);
        stream << " -op:idx -tsn:%indx_tsn% -tbli:";
        stream.setFieldWidth(m_Width);
        stream << dbtname;
        stream.setFieldWidth(0);
        stream << " >> create_index.log" << Qt::endl;

        file.close();
    }

    return ResultSuccess;
}
