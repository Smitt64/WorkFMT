#include "diffwizard.h"
#include "wizard/actionpage.h"
#include "wizard/connactionpage.h"
#include "wizard/scriptspage.h"
#include <QIcon>
#include <QAbstractButton>
#include <QDesktopServices>
#include <QUrl>
#include <QApplication>
#include <QDir>
#include <QTranslator>
#include <QMessageBox>

DiffWizard::DiffWizard(QWidget *parent) :
    QWizard(parent)
{
    setWindowIcon(QIcon(":/img/DiffToScript.png"));
    setFixedSize(800, 600);

    m_pActionPage = new ActionPage(this);
    m_pConnactionPage = new ConnactionPage(this);
    m_pScriptsPage = new ScriptsPage(this);

    setWindowTitle(tr("Diff to Script"));
    addPage(m_pActionPage);
    addPage(m_pConnactionPage);
    addPage(m_pScriptsPage);

    QAbstractButton *btn = button(QWizard::HelpButton);
    setOption(QWizard::HaveHelpButton);
    setOption(QWizard::HelpButtonOnRight, false);

    connect(btn, &QAbstractButton::clicked, [=]()
    {
        QDesktopServices::openUrl(QUrl("https://confluence.softlab.ru/pages/viewpage.action?pageId=609715197"));
    });
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
