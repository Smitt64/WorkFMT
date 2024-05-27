#include "actionpage.h"
#include "ui_actionpage.h"
#include "svnsatatusmodel.h"
#include "diffwizard.h"
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

// ---------------------------------------------------------------------------

ActionPage::ActionPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ActionPage)
{
    ui->setupUi(this);
    ui->stackedWidget->widget(0)->layout()->setMargin(0);

    m_pModel = new SvnSatatusModel(this);
    m_pStatusModel = new DatSatatusModel(this);
    m_pStatusModel->setSourceModel(m_pModel);

    ui->listView->setModel(m_pStatusModel);
    ui->listView->setModelColumn(SvnSatatusModel::fld_FileName);

    ui->localRadio->setChecked(true);
    ui->revisionRadio->setEnabled(false);

    fakeBtn = new QSpinBox();
    m_pGroup = new QButtonGroup(this);
    m_pGroup->addButton(ui->localRadio, 0);
    m_pGroup->addButton(ui->revisionRadio, 1);

    setTitle(tr("Параметры репозитория"));
    registerField("Action", fakeBtn, "value");
    registerField("Path", ui->pathEdit);

    connect(m_pGroup, QOverload<int, bool>::of(&QButtonGroup::buttonToggled), [=](int id, bool checked)
    {
        if (checked)
            setField("Action", id);
    });

    connect(m_pStatusModel, &DatSatatusModel::checkChanged, this, &ActionPage::completeChanged);
}

ActionPage::~ActionPage()
{
    delete ui;
}

void ActionPage::on_selFolderBtn_clicked()
{
    QSettings settings("DiffToScript.ini", QSettings::IniFormat);

    SelectFolderDlg filderDir(&settings, "SvnRepoList", this);

    if (filderDir.exec() == QDialog::Accepted)
    {
        ui->pathEdit->setText(filderDir.selectedPath());
        m_pModel->setPath(ui->pathEdit->text());
        emit completeChanged();
    }
}

bool ActionPage::isComplete() const
{
    if (ui->pathEdit->text().isEmpty())
        return false;

    QStringList lst = m_pStatusModel->files();
    if (lst.isEmpty())
        return false;

    DiffWizard *wzrd = qobject_cast<DiffWizard*>(wizard());
    wzrd->addUserField("Files", lst);

    return true;
}
