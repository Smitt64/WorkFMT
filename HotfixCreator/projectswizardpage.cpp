#include "projectswizardpage.h"
#include "ui_projectswizardpage.h"
#include "hotfixcontentmodel.h"
#include "hotfixwizard.h"
#include "modulelistmodel.h"
#include "projectloader.h"
#include <QSortFilterProxyModel>

ProjectsWizardPage::ProjectsWizardPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ProjectsWizardPage)
{
    ui->setupUi(this);

    setTitle("Проекты для сборки");

    m_Proxy.reset(new QSortFilterProxyModel());
    m_pModel.reset(new ModuleListModel());

    m_Proxy->setSourceModel(m_pModel.data());
    ui->treeView->setModel(m_Proxy.data());

    m_Proxy->setSortRole(Qt::DisplayRole);
    m_Proxy->setSortCaseSensitivity(Qt::CaseInsensitive);

    connect(m_pModel.data(), &ModuleListModel::dataChanged, this, &ProjectsWizardPage::onDataChanged);
}

ProjectsWizardPage::~ProjectsWizardPage()
{
    delete ui;
}

void ProjectsWizardPage::onDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
    Q_UNUSED(topLeft);
    Q_UNUSED(bottomRight);
    Q_UNUSED(roles);

    HotfixWizard *wzrd = (HotfixWizard*)wizard();
    HotfixContentModel *pModel = wzrd->structModel();

    // Получаем список выбранных проектов из модели
    QSet<QString> selectedProjects = m_pModel->getSelectedModules();
    QStringList projects = pModel->projects();

    // Получаем список модулей для выбранных проектов
    QStringList modules = selectedProjects.values();

    // Генерируем текст с помощью ProjectLoader
    QString generatedText = pModel->projectLoader()->generateTextForProjects(projects, modules);

    // Обновляем текстовое поле
    ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText(generatedText);
}

void ProjectsWizardPage::initializePage()
{
    HotfixWizard *wzrd = (HotfixWizard*)wizard();
    HotfixContentModel *pModel = wzrd->structModel();

    QStringList projectNames = pModel->projects();
    QSet<QString> modules = pModel->projectLoader()->getModulesFromNames(projectNames);
    //m_pModel->setModules(modules);

    // Пример: получаем данные о проектах
    ProjectsTuple projects;
    for (const QString& name : qAsConst(modules))
    {
        QString vcxproj, dir, targetName, targetExt;
        if (pModel->projectLoader()->getProjectInfo(name, vcxproj, dir, targetName, targetExt))
            projects[name] = std::make_tuple(targetName, targetExt, vcxproj);
    }

    m_pModel->setModules(projects);
    m_Proxy->sort(0);

    QString text = pModel->projectLoader()->generateTextForProjects(projectNames, QStringList());
    ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText(text);
}
