#include "triggerpage.h"
#include "buildtrigwizard.h"
#include "repopage.h"
#include "errordlg.h"
#include "errorsmodel.h"
#include <QVBoxLayout>
#include <QProgressBar>
#include <QThreadPool>
#include <QAbstractButton>
#include <QProcess>
#include <QDir>
#include <QFile>
#include <QFileInfo>

// Перевод известных сообщений MacTriggerGit.exe на русский язык.
// Русские литералы только через fromUtf8 (как tr() в остальном проекте):
// файл в UTF-8 без BOM, QStringLiteral исказит кириллицу.
static QString translateTriggerMessage(const QString &text)
{
    static const QList<QPair<QString, QString>> translations = {
        { QStringLiteral("there is no macro header or it has an invalid format."),
          QString::fromUtf8("отсутствует заголовок макроса или он имеет неверный формат") },
        { QStringLiteral("in header should be fields $Module and $Description"),
          QString::fromUtf8("в заголовке должны быть поля $Module и $Description") },
        { QStringLiteral("macro name should be equal to the field $Name"),
          QString::fromUtf8("имя макроса должно совпадать со значением поля $Name") },
        { QStringLiteral("macro contains debugbreak"),
          QString::fromUtf8("макрос содержит debugbreak") },
        { QStringLiteral("failed access"),
          QString::fromUtf8("нет доступа к файлу") }
    };

    const QString trimmed = text.trimmed();

    for (const auto &pair : translations)
    {
        if (trimmed.endsWith(pair.first, Qt::CaseInsensitive))
            return trimmed.left(trimmed.length() - pair.first.length()) + pair.second;
    }

    return trimmed;
}

TriggerOperation::TriggerOperation(QObject *parent) :
    QObject(parent),
    QRunnable()
{
    setAutoDelete(true);
}

TriggerOperation::~TriggerOperation()
{

}

void TriggerOperation::run()
{
    const QString triggerPath = QDir(repoPath).absoluteFilePath(QStringLiteral("Build/Trig/MacTriggerGit.exe"));

    if (!QFile::exists(triggerPath))
    {
        emit appendError(tr("Не найден файл триггера: %1")
                         .arg(QDir::toNativeSeparators(triggerPath)), ErrorsModel::TypeError);
        emit operationFinished(true);
        return;
    }

    bool hasErrors = false;
    const int total = files.size();

    for (int i = 0; i < total; ++i)
    {
        const QFileInfo fi(files.at(i));

        // Рабочий каталог: репозиторий + каталог из пути макроса,
        // в приложение передаётся только имя файла с расширением
        QProcess proc;
        proc.setWorkingDirectory(QDir(repoPath).absoluteFilePath(fi.path()));
        proc.setProgram(triggerPath);
        proc.setArguments(QStringList() << fi.fileName());

        proc.start();

        if (!proc.waitForStarted())
        {
            hasErrors = true;

            emit appendError(tr("Не удалось запустить %1: %2")
                             .arg(QDir::toNativeSeparators(triggerPath), proc.errorString()),
                             ErrorsModel::TypeError);
            emit progressValue(i + 1);
            continue;
        }

        proc.waitForFinished(-1);

        const QString stdOut = QString::fromLocal8Bit(proc.readAllStandardOutput());
        const QString stdErr = QString::fromLocal8Bit(proc.readAllStandardError());

        // Триггер сам указывает имя файла в сообщениях ("<file> - <message>"),
        // префикс добавляем только если его нет. Строки-маркеры "ERROR!" и
        // "SUCCESS!" в лог не попадают: "ERROR!" помечает предыдущее
        // сообщение как ошибку.
        auto makeMsg = [this](int idx, const QString &text) -> QString
        {
            const QString relPath = files.at(idx);
            const QString fileName = QFileInfo(relPath).fileName();

            // Триггер может вывести как относительный путь, так и только имя файла
            if (text.startsWith(relPath, Qt::CaseInsensitive) ||
                text.startsWith(fileName, Qt::CaseInsensitive))
                return text;

            return QStringLiteral("%1: %2").arg(relPath, text);
        };

        QList<QPair<QString, int>> messages;

        for (const QString &line : stdOut.split('\n', Qt::SkipEmptyParts))
        {
            const QString text = line.trimmed();

            if (text.isEmpty())
                continue;

            if (!text.compare(QLatin1String("ERROR!"), Qt::CaseInsensitive))
            {
                hasErrors = true;

                if (!messages.isEmpty() && messages.last().second != ErrorsModel::TypeError)
                    messages.last().second = ErrorsModel::TypeError;

                continue;
            }

            if (!text.compare(QLatin1String("SUCCESS!"), Qt::CaseInsensitive))
                continue;

            // Служебные строки триггера в лог не попадают
            if (text.startsWith(QLatin1String("CurDir"), Qt::CaseInsensitive) ||
                text.startsWith(QLatin1String("FoundMacName="), Qt::CaseInsensitive) ||
                text.startsWith(QLatin1String("macname="), Qt::CaseInsensitive))
                continue;

            int type = ErrorsModel::TypeInfo;

            if (text.contains(QLatin1String("ошибка"), Qt::CaseInsensitive) ||
                text.contains(QLatin1String("error"), Qt::CaseInsensitive))
            {
                type = ErrorsModel::TypeError;
                hasErrors = true;
            }
            else if (text.contains(QLatin1String("предупреждение"), Qt::CaseInsensitive) ||
                     text.contains(QLatin1String("warning"), Qt::CaseInsensitive))
            {
                type = ErrorsModel::TypeWarning;
            }

            messages.append(qMakePair(makeMsg(i, translateTriggerMessage(text)), type));
        }

        for (const QString &line : stdErr.split('\n', Qt::SkipEmptyParts))
        {
            const QString text = line.trimmed();

            if (text.isEmpty() ||
                !text.compare(QLatin1String("ERROR!"), Qt::CaseInsensitive) ||
                !text.compare(QLatin1String("SUCCESS!"), Qt::CaseInsensitive))
                continue;

            hasErrors = true;

            messages.append(qMakePair(makeMsg(i, translateTriggerMessage(text)), ErrorsModel::TypeError));
        }

        for (const auto &msg : messages)
            emit appendError(msg.first, msg.second);

        if (proc.exitStatus() == QProcess::CrashExit)
        {
            hasErrors = true;

            emit appendError(tr("%1: процесс аварийно завершился").arg(files.at(i)), ErrorsModel::TypeError);
        }
        else if (proc.exitCode() != 0)
        {
            hasErrors = true;

            emit appendError(tr("%1: код завершения %2").arg(files.at(i)).arg(proc.exitCode()),
                             ErrorsModel::TypeError);
        }

        emit progressValue(i + 1);
    }

    emit operationFinished(hasErrors);
}

// ---------------------------------------------------------------------------

TriggerPage::TriggerPage(QWidget *parent) :
    QWizardPage(parent),
    m_Finished(false)
{
    setTitle(tr("Запуск проверки"));

    m_Layout.reset(new QVBoxLayout());
    m_Progress.reset(new QProgressBar());
    m_LogWidget.reset(new ErrorDlg(ErrorDlg::ModeWidget));
    m_Log.reset(new ErrorsModel());

    m_Layout->addWidget(m_LogWidget.data());
    m_Layout->addWidget(m_Progress.data());

    m_LogWidget->layout()->setMargin(0);
    m_LogWidget->setErrors(m_Log.data());
    m_LogWidget->setMessage(tr("Запуск MacTriggerGit.exe для выбранных макросов"));

    setLayout(m_Layout.data());
}

TriggerPage::~TriggerPage()
{

}

bool TriggerPage::isComplete() const
{
    return m_Finished;
}

void TriggerPage::initializePage()
{
    m_Finished = false;
    m_Log->clear();
    m_Progress->setValue(0);
    emit completeChanged();

    BuildTrigWizard *wzrd = dynamic_cast<BuildTrigWizard*>(wizard());

    TriggerOperation *op = new TriggerOperation();
    op->repoPath = field("Path").toString();
    op->files = wzrd->repoPage()->selectedFiles();

    m_Progress->setMaximum(qMax(op->files.size(), 1));

    connect(op, &TriggerOperation::appendError, this, [this](const QString &text, int type)
    {
        m_Log->appendError(text, type);
    });

    connect(op, &TriggerOperation::progressValue, m_Progress.data(), &QProgressBar::setValue);
    connect(op, &TriggerOperation::operationFinished, this, &TriggerPage::onOperationFinished);

    wizard()->button(QWizard::BackButton)->setEnabled(false);

    QThreadPool::globalInstance()->start(op);
}

void TriggerPage::onOperationFinished(bool hasErrors)
{
    wizard()->button(QWizard::BackButton)->setEnabled(true);

    if (hasErrors)
        m_Log->appendError(tr("Проверка завершена с ошибками"), ErrorsModel::TypeError);
    else
        m_Log->appendMessage(tr("Проверка успешно завершена"));

    m_Finished = true;
    emit completeChanged();
}