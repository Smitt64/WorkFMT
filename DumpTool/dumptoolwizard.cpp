#include "dumptoolwizard.h"
#include "userconnectionpage.h"
#include "selectactionpage.h"
#include "impfileselectpage.h"
#include "summarypage.h"
#include "toolsruntime.h"
#include "actionlogpage.h"
#include "expparampage.h"
#include "exppgparampage.h"
#include "pgimpparampage.h"
#include <QPushButton>
#include <QApplication>
#include <QDesktopServices>

DumpToolWizard::DumpToolWizard() :
    QWizard()
{
    QDir dir(qApp->applicationDirPath());
    m_pSettings = new QSettings(dir.absoluteFilePath("DumpTool.ini"), QSettings::IniFormat);

    m_ConnectionPage = new UserConnectionPage(this);
    m_SelectActionPage = new SelectActionPage(this);
    m_ImpFileSelectPage = new ImpFileSelectPage(this);
    m_pSummaryPage = new SummaryPage(this);
    m_pActionLogPage = new ActionLogPage(this);
    m_pExpParamPage = new ExpParamPage(this);
    m_pExpPgParamPage = new ExpPgParamPage(this);
    m_pPgImpParamPage = new PgImpParamPage(this);

    addPage(m_SelectActionPage);
    addPage(m_ConnectionPage);
    addPage(m_ImpFileSelectPage);
    addPage(m_pSummaryPage);
    addPage(m_pActionLogPage);
    addPage(m_pExpParamPage);
    addPage(m_pExpPgParamPage);
    addPage(m_pPgImpParamPage);

    setTitleFormat(Qt::RichText);
    setOption(QWizard::HaveHelpButton);
    setWindowTitle(tr("Мастер импорта/экспорта файла дампа"));
    setMinimumSize(QSize(800, 600));

    setWindowIcon(QIcon(":/img/VCProject.dll_I000d_0409.ico"));

    m_HelpButton = button(QWizard::HelpButton);

    connect(m_HelpButton, &QPushButton::clicked, []
    {
        QDesktopServices::openUrl(QUrl("https://confluence.softlab.ru/pages/viewpage.action?pageId=609091997"));
    });
}

DumpToolWizard::~DumpToolWizard()
{
    delete m_pSettings;
}

QSettings *DumpToolWizard::settings()
{
    return m_pSettings;
}

void DumpToolWizard::addUserField(const QString &name, const QVariant &value)
{
    m_Fields.insert(name, value);
}

const QVariant &DumpToolWizard::userField(const QString &name)
{
    return m_Fields[name];
}

QString DumpToolWizard::userValName(const QString &value) const
{
    return m_Fields.key(value);
}
