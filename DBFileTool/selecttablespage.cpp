#include "selecttablespage.h"
#include "tablesselectwidget.h"
#include "toolsruntime.h"
#include <connectioninfo.h>
#include "fmtcore.h"
#include "dbttoolwizard.h"
#include "selectfolderdlg.h"
#include <QDir>
#include <QMessageBox>
#include <QSettings>

SelectTablesPage::SelectTablesPage(QWidget *parent) :
    QWizardPage(parent),
    m_pTables(nullptr)
{
    setTitle(tr("Выбор таблиц"));
    pLayout = new QVBoxLayout(this);
    m_pEditLayout = new QHBoxLayout();

    m_PathLabel = new QLabel(tr("Путь, куда выгружать:"), this);
    m_pPathEdit = new QLineEdit(this);
    m_pSelectFolder = new QToolButton(this);
    m_pSelectFolder->setIcon(QIcon(":/img/openfolderHS.png"));

    registerField("ExportPath", m_pPathEdit, "text", SIGNAL(textChanged(QString)));

    m_pHorLine = new QFrame();
    m_pHorLine->setFrameShape(QFrame::HLine);

    m_pEditLayout->addWidget(m_pPathEdit);
    m_pEditLayout->addWidget(m_pSelectFolder);

    pLayout->addWidget(m_PathLabel);
    pLayout->addLayout(m_pEditLayout);
    pLayout->addWidget(m_pHorLine);

    connect(m_pSelectFolder, &QToolButton::clicked, [=]()
    {
        QSettings settings(toolFullFileNameFromDir("fmtopt.ini"), QSettings::IniFormat);
        SelectFolderDlg dlg(&settings, RsExpUnlDirContext, this);

        if (dlg.exec() == QDialog::Accepted)
        {
            m_pPathEdit->setText(dlg.selectedPath());
        }
    });
//m_pConnection->close();
    connect(m_pPathEdit, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));

    /*connect(wizard()->button(QWizard::NextButton), &QAbstractButton::clicked, [=]()
    {
        m_pConnection->close();
    });*/
}

SelectTablesPage::~SelectTablesPage()
{

}

bool SelectTablesPage::AddFunc(const QString &str)
{
    DbtToolWizard *pWizard = qobject_cast<DbtToolWizard*>(wizard());

    QStringList lst;
    QVariant val = pWizard->userField("TableList");

    if (val.isValid())
        lst = pWizard->userField("TableList").toStringList();

    lst.append(str);
    pWizard->addUserField("TableList", lst);
    emit completeChanged();

    return true;
}

bool SelectTablesPage::RemoveFunc(const QString &str)
{
    DbtToolWizard *pWizard = qobject_cast<DbtToolWizard*>(wizard());

    QStringList lst;
    QVariant val = pWizard->userField("TableList");

    if (val.isValid())
        lst = pWizard->userField("TableList").toStringList();

    lst.removeOne(str);
    pWizard->addUserField("TableList", lst);
    emit completeChanged();

    return true;
}

void SelectTablesPage::initializePage()
{
    if (m_pTables)
    {
        pLayout->removeWidget(m_pTables);
        delete m_pTables;
    }

    m_pConnection.reset(new ConnectionInfo());

    QString error;
    if (!m_pConnection->open(QRSD_DRIVER, wizard()->field("User").toString(), wizard()->field("Password").toString(), wizard()->field("Service").toString(), "", &error))
    {
        m_pConnection->close();
        return;
    }

    m_pTables = new TablesSelectWidget(m_pConnection.data(), this);
    m_pTables->layout()->setMargin(0);
    pLayout->addWidget(m_pTables);

    m_pTables->setAddFunc(std::bind(&SelectTablesPage::AddFunc, this, std::placeholders::_1));
    m_pTables->setRemFunc(std::bind(&SelectTablesPage::RemoveFunc, this, std::placeholders::_1));
}

bool SelectTablesPage::isComplete() const
{
    QStringList lst;
    DbtToolWizard *pWizard = qobject_cast<DbtToolWizard*>(wizard());
    QVariant val = pWizard->userField("TableList");

    if (val.isValid())
        lst = pWizard->userField("TableList").toStringList();

    bool hr = !m_pPathEdit->text().isEmpty() && !lst.isEmpty();
    return hr;
}

bool SelectTablesPage::validatePage()
{
    QDir dir(m_pPathEdit->text());

    if (!dir.exists())
    {
        QMessageBox::critical(this, tr("Ошибка!"), tr("Не найден каталог выгрузки %1").arg(dir.path()));
        return false;
    }

    return true;
}

int SelectTablesPage::nextId() const
{
    return DbtToolWizard::eExportPage;
}

void SelectTablesPage::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
        return;

    return QWizardPage::keyPressEvent(event);
}
