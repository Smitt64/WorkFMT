#include "buildtrigwizard.h"
#include "repopage.h"
#include "triggerpage.h"

BuildTrigWizard::BuildTrigWizard(QWidget *parent) :
    QWizard(parent)
{
    setWindowTitle(tr("BuildTrig"));
    setFixedSize(800, 600);

    m_pRepoPage = new RepoPage(this);
    m_pTriggerPage = new TriggerPage(this);

    addPage(m_pRepoPage);
    addPage(m_pTriggerPage);

    setOption(QWizard::NoCancelButtonOnLastPage);
    setOption(QWizard::DisabledBackButtonOnLastPage);
}

BuildTrigWizard::~BuildTrigWizard()
{

}

RepoPage *BuildTrigWizard::repoPage() const
{
    return m_pRepoPage;
}

void BuildTrigWizard::addUserField(const QString &name, const QVariant &value)
{
    m_Fields.insert(name, value);
}

const QVariant &BuildTrigWizard::userField(const QString &name)
{
    return m_Fields[name];
}