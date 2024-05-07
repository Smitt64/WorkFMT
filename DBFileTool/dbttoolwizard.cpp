#include "dbttoolwizard.h"
#include "selectactionpage.h"
#include "oraconnectionpage.h"
#include "selecttablespage.h"
#include "exportpage.h"
#include <QVariant>
#include <QDir>
#include <QTranslator>
#include <QApplication>

DbtToolWizard::DbtToolWizard() :
    QWizard()
{
    setTitleFormat(Qt::RichText);
    setWindowTitle(tr("Мастер импорта/экспорта таблиц"));
    setMinimumSize(QSize(800, 600));

    m_pActionPage = new SelectActionPage(this);
    m_pOraConnectionPage = new OraConnectionPage(this);
    m_pSelectTablesPage = new SelectTablesPage(this);
    m_pExportPage = new ExportPage(this);

    addPage(m_pActionPage);
    addPage(m_pOraConnectionPage);
    addPage(m_pSelectTablesPage);
    addPage(m_pExportPage);

    QDir trDir(qApp->applicationDirPath());
    if (trDir.cd("translations"))
    {
        QString translatorFile = QString("qt_%1").arg("ru");
        if (qt_translator.load(translatorFile, trDir.absolutePath()))
            qApp->installTranslator(&qt_translator);
    }
}

DbtToolWizard::~DbtToolWizard()
{

}

void DbtToolWizard::addUserField(const QString &name, const QVariant &value)
{
    m_Fields.insert(name, value);
}

const QVariant &DbtToolWizard::userField(const QString &name)
{
    if (!m_Fields.contains(name))
        return QVariant();

    return m_Fields[name];
}
