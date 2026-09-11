#include "repopage.h"
#include "ui_repopage.h"
#include <svn/svnstatusmodel.h>
#include <toolsruntime.h>
#include <selectfolderdlg.h>
#include "svnlogdlg.h"
#include <QSettings>
#include <QButtonGroup>
#include <QFileInfo>

MacSatatusModel::MacSatatusModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{

}

Qt::ItemFlags MacSatatusModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags f = QSortFilterProxyModel::flags(index);

    return f | Qt::ItemIsUserCheckable;
}

QVariant MacSatatusModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        SvnStatusModel *src = qobject_cast<SvnStatusModel*>(sourceModel());
        const SvnSatatusElement &element = src->element(mapToSource(index).row());

        return QFileInfo(element.path).fileName();
    }

    if (role == Qt::CheckStateRole)
    {
        SvnStatusModel *src = qobject_cast<SvnStatusModel*>(sourceModel());
        const SvnSatatusElement &element = src->element(mapToSource(index).row());

        if (!m_CheckSate.contains(element.path))
            return Qt::Unchecked;

        return m_CheckSate[element.path];
    }

    return QSortFilterProxyModel::data(index, role);
}

bool MacSatatusModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::CheckStateRole)
    {
        SvnStatusModel *src = qobject_cast<SvnStatusModel*>(sourceModel());
        const SvnSatatusElement &element = src->element(mapToSource(index).row());
        m_CheckSate[element.path] = value.value<Qt::CheckState>();

        emit checkChanged();

        return true;
    }

    return false;
}

bool MacSatatusModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    SvnStatusModel *src = qobject_cast<SvnStatusModel*>(sourceModel());
    const SvnSatatusElement &element = src->element(source_row);

    return element.path.contains(".mac", Qt::CaseInsensitive);
}

QStringList MacSatatusModel::files()
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

void MacSatatusModel::resetCheckstate()
{
    m_CheckSate.clear();
}

// ---------------------------------------------------------------------------

RepoPage::RepoPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::RepoPage)
{
    ui->setupUi(this);
    ui->listView->setSelectionMode(QAbstractItemView::NoSelection);

    m_pModel = new SvnStatusModel(this);
    m_pStatusModel = new MacSatatusModel(this);
    m_pStatusModel->setSourceModel(m_pModel);

    ui->listView->setModel(m_pStatusModel);
    ui->listView->setModelColumn(SvnStatusModel::fld_FileName);

    setTitle(tr("Параметры репозитория"));

    registerField("Path", ui->pathEdit);
    registerField("Revision", ui->revisionEdit);

    m_pGroup = new QButtonGroup(this);
    m_pGroup->addButton(ui->localRadio, 0);
    m_pGroup->addButton(ui->revisionRadio, 1);

    ui->localRadio->setChecked(true);
    ui->widget->setVisible(false);

    connect(m_pStatusModel, &MacSatatusModel::checkChanged, this, &RepoPage::completeChanged);

    connect(m_pGroup, QOverload<int, bool>::of(&QButtonGroup::buttonToggled), this, [this](int id, bool checked)
    {
        if (checked && id == 1)
        {
            ui->widget->setVisible(true);
        }
        else
        {
            ui->revisionEdit->setText("");
            ui->widget->setVisible(false);
        }

        m_pModel->setPath(ui->pathEdit->text(), ui->revisionEdit->text());
    });

    connect(ui->revisionEdit, &QLineEdit::editingFinished, this, [this]()
    {
        m_pModel->setPath(ui->pathEdit->text(), ui->revisionEdit->text());
        emit completeChanged();
    });
}

RepoPage::~RepoPage()
{
    delete ui;
}

void RepoPage::initializePage()
{
    m_pStatusModel->setSourceModel(nullptr);

    if (m_pModel)
        delete m_pModel;

    m_pModel = new SvnStatusModel(this);
    m_pStatusModel->setSourceModel(m_pModel);
    m_pStatusModel->resetCheckstate();

    ui->listView->setModel(m_pStatusModel);
    ui->listView->setModelColumn(SvnStatusModel::fld_FileName);
}

void RepoPage::on_selFolderBtn_clicked()
{
    QSettings settings("BuildTrig.ini", QSettings::IniFormat);

    SelectFolderDlg filderDir(&settings, "SvnRepoList", this);

    if (filderDir.exec() == QDialog::Accepted)
    {
        ui->pathEdit->setText(filderDir.selectedPath());
        m_pModel->setPath(ui->pathEdit->text(), ui->revisionEdit->text());
        emit completeChanged();

        VcsType type = toolDetectVcsType(ui->pathEdit->text());

        if (type == VcsType::Git)
            ui->versIcon->setPixmap(QPixmap("://res/gitlogo.svg"));
        else
            ui->versIcon->setPixmap(QPixmap("://res/svnlogo.svg"));
    }
}

void RepoPage::on_logButton_clicked()
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

QStringList RepoPage::selectedFiles() const
{
    QStringList lst;
    const QString base = field("Path").toString();

    for (const QString &file : const_cast<MacSatatusModel*>(m_pStatusModel)->files())
        lst.append(QDir::toNativeSeparators(QDir(base).relativeFilePath(file)));

    return lst;
}

bool RepoPage::isComplete() const
{
    if (ui->pathEdit->text().isEmpty())
        return false;

    QStringList lst = m_pStatusModel->files();
    if (lst.isEmpty())
        return false;

    return true;
}