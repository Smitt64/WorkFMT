#include "dumptoolwizard.h"
#include "userconnectionpage.h"
#include "selectactionpage.h"
#include "impfileselectpage.h"
#include "summarypage.h"
#include "actionlogpage.h"
#include "expparampage.h"

DumpToolWizard::DumpToolWizard()
{
    m_ConnectionPage = new UserConnectionPage(this);
    m_SelectActionPage = new SelectActionPage(this);
    m_ImpFileSelectPage = new ImpFileSelectPage(this);
    m_pSummaryPage = new SummaryPage(this);
    m_pActionLogPage = new ActionLogPage(this);
    m_pExpParamPage = new ExpParamPage(this);

    addPage(m_SelectActionPage);
    addPage(m_ConnectionPage);
    addPage(m_ImpFileSelectPage);
    addPage(m_pSummaryPage);
    addPage(m_pActionLogPage);
    addPage(m_pExpParamPage);

    setTitleFormat(Qt::RichText);
    setWindowTitle(tr("Мастер импорта/экспорта файла дампа"));
    setMinimumSize(QSize(800, 600));

    setWindowIcon(QIcon(":/img/VCProject.dll_I000d_0409.ico"));
}

DumpToolWizard::~DumpToolWizard()
{

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
