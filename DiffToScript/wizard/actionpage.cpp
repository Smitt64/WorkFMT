#include "actionpage.h"
#include "ui_actionpage.h"
#include "svnsatatusmodel.h"
#include "diffwizard.h"
#include "svnlogdlg.h"
#include <selectfolderdlg.h>
#include <QButtonGroup>
#include <QSpinBox>
#include <QFileDialog>
#include <QSettings>

DatSatatusModel::DatSatatusModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{

}

Qt::ItemFlags DatSatatusModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags f = QSortFilterProxyModel::flags(index);

    return f | Qt::ItemIsUserCheckable;
}

QVariant DatSatatusModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::CheckStateRole)
    {
        SvnSatatusModel *src = qobject_cast<SvnSatatusModel*>(sourceModel());
        const SvnSatatusElement &element = src->element(mapToSource(index).row());

        if (!m_CheckSate.contains(element.path))
            return Qt::Unchecked;

        return m_CheckSate[element.path];
    }

    return QSortFilterProxyModel::data(index, role);
}

bool DatSatatusModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::CheckStateRole)
    {
        SvnSatatusModel *src = qobject_cast<SvnSatatusModel*>(sourceModel());
        const SvnSatatusElement &element = src->element(mapToSource(index).row());
        m_CheckSate[element.path] = value.value<Qt::CheckState>();

        emit checkChanged();

        return true;
    }

    return false;
}

bool DatSatatusModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    SvnSatatusModel *src = qobject_cast<SvnSatatusModel*>(sourceModel());
    const SvnSatatusElement &element = src->element(source_row);

    return element.path.contains(".dat", Qt::CaseInsensitive);
}

QStringList DatSatatusModel::files()
{
    QStringList lst;
    QMapIterator<QString, Qt::CheckState> iter(m_CheckSate);

    while (iter.hasNext())
    {
        iter.next();

        if (iter.value() == Qt::Checked)
            lst.append(iter.key());
    }

    return lst;
}

void DatSatatusModel::resetCheckstate()
{
    m_CheckSate.clear();
}

// ---------------------------------------------------------------------------

ActionPage::ActionPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ActionPage)
{
    ui->setupUi(this);
    ui->widget->layout()->setMargin(0);

    m_pModel = new SvnSatatusModel(this);
    m_pStatusModel = new DatSatatusModel(this);
    m_pStatusModel->setSourceModel(m_pModel);

    ui->listView->setModel(m_pStatusModel);
    ui->listView->setModelColumn(SvnSatatusModel::fld_FileName);

    ui->localRadio->setChecked(true);

    ui->widget->setVisible(false);
    //ui->revisionRadio->setEnabled(false);

    fakeBtn = new QSpinBox();
    m_pGroup = new QButtonGroup(this);
    m_pGroup->addButton(ui->localRadio, 0);
    m_pGroup->addButton(ui->revisionRadio, 1);

    setTitle(tr("Параметры репозитория"));
    registerField("Action", fakeBtn, "value");
    registerField("Path", ui->pathEdit);
    registerField("Revision", ui->revisionEdit);

    registerField("OraScript", ui->oraCheck);
    registerField("PgScript", ui->pgCheck);

    connect(m_pGroup, QOverload<int, bool>::of(&QButtonGroup::buttonToggled), [=](int id, bool checked)
    {
        if (checked)
            setField("Action", id);

        if (checked && id == 1)
            ui->widget->setVisible(true);
        else
        {
            ui->revisionEdit->setText("");
            ui->widget->setVisible(false);
        }

        m_pModel->setPath(ui->pathEdit->text(), ui->revisionEdit->text());
    });

    connect(ui->revisionEdit, &QLineEdit::editingFinished, [=]()
    {
        m_pModel->setPath(ui->pathEdit->text(), ui->revisionEdit->text());
        emit completeChanged();
    });

    connect(m_pStatusModel, &DatSatatusModel::checkChanged, this, &ActionPage::completeChanged);
    connect(ui->oraCheck, &QCheckBox::toggled, this, &ActionPage::completeChanged);
    connect(ui->pgCheck, &QCheckBox::toggled, this, &ActionPage::completeChanged);
}

ActionPage::~ActionPage()
{
    delete ui;
}

void ActionPage::initializePage()
{
    m_pStatusModel->setSourceModel(nullptr);

    if (m_pModel)
        delete m_pModel;

    m_pModel = new SvnSatatusModel(this);
    m_pStatusModel->setSourceModel(m_pModel);
    m_pStatusModel->resetCheckstate();

    ui->listView->setModel(m_pStatusModel);
    ui->listView->setModelColumn(SvnSatatusModel::fld_FileName);

    wizard()->button(QWizard::CustomButton2)->setVisible(false);
}

void ActionPage::on_selFolderBtn_clicked()
{
    QSettings settings("DiffToScript.ini", QSettings::IniFormat);

    SelectFolderDlg filderDir(&settings, "SvnRepoList", this);

    if (filderDir.exec() == QDialog::Accepted)
    {
        ui->pathEdit->setText(filderDir.selectedPath());
        m_pModel->setPath(ui->pathEdit->text(), ui->revisionEdit->text());
        emit completeChanged();
    }
}

bool ActionPage::isComplete() const
{
    if (ui->pathEdit->text().isEmpty())
        return false;

    if (ui->oraCheck->isChecked() == ui->pgCheck->isChecked() && !ui->oraCheck->isChecked())
        return false;

    QStringList lst = m_pStatusModel->files();
    if (lst.isEmpty())
        return false;

    DiffWizard *wzrd = qobject_cast<DiffWizard*>(wizard());
    wzrd->addUserField("Files", lst);

    return true;
}

void ActionPage::on_logButton_clicked()
{
    SvnLogDlg dlg(this);
    dlg.setPath(ui->pathEdit->text());
    if (dlg.exec() == QDialog::Accepted)
    {
        ui->revisionEdit->blockSignals(true);
        ui->revisionEdit->setText(dlg.revision());
        ui->revisionEdit->blockSignals(false);

        m_pModel->setPath(ui->pathEdit->text(), ui->revisionEdit->text());
        emit completeChanged();
    }
}

