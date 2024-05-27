#include "diffwizard.h"
#include "wizard/actionpage.h"
#include "wizard/connactionpage.h"
#include "wizard/scriptspage.h"

DiffWizard::DiffWizard(QWidget *parent) :
    QWizard(parent)
{
    setFixedSize(800, 600);

    m_pActionPage = new ActionPage(this);
    m_pConnactionPage = new ConnactionPage(this);
    m_pScriptsPage = new ScriptsPage(this);

    setWindowTitle(tr("Diff to Script"));
    addPage(m_pActionPage);
    addPage(m_pConnactionPage);
    addPage(m_pScriptsPage);
}

DiffWizard::~DiffWizard()
{

}

void DiffWizard::addUserField(const QString &name, const QVariant &value)
{
    m_Fields.insert(name, value);
}

const QVariant &DiffWizard::userField(const QString &name)
{
    return m_Fields[name];
}

QString DiffWizard::userValName(const QString &value) const
{
    return m_Fields.key(value);
}
