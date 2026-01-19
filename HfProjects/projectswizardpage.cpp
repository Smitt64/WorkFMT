#include "projectswizardpage.h"
#include "ui_projectswizardpage.h"
#include "modulelistmodel.h"
#include "projectloader.h"
#include <QSortFilterProxyModel>

QStringList extractProjectNames(const QString& input)
{
    QStringList lines = input.split('\n', Qt::SkipEmptyParts);
    QStringList projectNames;
    QSet<QString> seen;

    for (const QString& line : qAsConst(lines))
    {
        QString trimmed = line.trimmed();
        if (trimmed.isEmpty())
            continue;

        // Разбиваем строку по '/'
        QStringList parts = trimmed.split('/', Qt::SkipEmptyParts);

        // Проверяем: минимум 3 части (Source, <project>, <file>)
        if (parts.size() >= 3 && parts[0].compare(QStringLiteral("Source"), Qt::CaseInsensitive) == 0)
        {
            QString projectName = parts[1];
            if (!seen.contains(projectName))
            {
                projectNames.append(projectName);
                seen.insert(projectName);
            }
        }
    }

    qDebug() << projectNames;

    return projectNames;
}

ProjectsWizardPage::ProjectsWizardPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ProjectsWizardPage)
{
    ui->setupUi(this);

    m_ProjectLoader.reset(new ProjectLoader(":/res/projects.json"));
    setTitle("Проекты для сборки");
    registerField("BuildInstruction", ui->plainTextEdit, "plainText");

    m_Proxy.reset(new QSortFilterProxyModel());
    m_pModel.reset(new ModuleListModel());

    m_Proxy->setSourceModel(m_pModel.data());
    ui->treeView->setModel(m_Proxy.data());

    m_Proxy->setSortRole(Qt::DisplayRole);
    m_Proxy->setSortCaseSensitivity(Qt::CaseInsensitive);

    connect(m_pModel.data(), &ModuleListModel::dataChanged, this, &ProjectsWizardPage::onDataChanged);
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &ProjectsWizardPage::updateFilter);
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

    // Получаем список выбранных проектов из модели
    QSet<QString> selectedProjects = m_pModel->getSelectedModules();
    QStringList projects = extractProjectNames(field("Files").toString());

    // Получаем список модулей для выбранных проектов
    QStringList modules = selectedProjects.values();

    // Генерируем текст с помощью ProjectLoader
    QString generatedText = m_ProjectLoader->generateTextForProjects(projects, modules);

    // Обновляем текстовое поле
    ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText(generatedText);
}

void ProjectsWizardPage::initializePage()
{
    /*HotfixWizard *wzrd = (HotfixWizard*)wizard();
    HotfixContentModel *pModel = wzrd->structModel();*/

    QStringList projectNames = extractProjectNames(field("Files").toString());
    QSet<QString> modules = m_ProjectLoader->getModulesFromNames(projectNames);
    //m_pModel->setModules(modules);

    // Пример: получаем данные о проектах
    ProjectsTuple projects;
    for (const QString& name : qAsConst(modules))
    {
        QString vcxproj, dir, targetName, targetExt;
        if (m_ProjectLoader->getProjectInfo(name, vcxproj, dir, targetName, targetExt))
            projects[name] = std::make_tuple(targetName, targetExt, vcxproj);
    }

    m_pModel->setModules(projects);
    m_Proxy->sort(0);

    QString text = m_ProjectLoader->generateTextForProjects(projectNames, QStringList());
    ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText(text);
}

void ProjectsWizardPage::updateFilter(const QString& text)
{
    // Устанавливаем фильтр для QSortFilterProxyModel
    m_Proxy->setFilterCaseSensitivity(Qt::CaseInsensitive); // Без учета регистра
    m_Proxy->setFilterFixedString(text); // Фильтр по введенному тексту

    // Фильтрация по всем колонкам
    m_Proxy->setFilterKeyColumn(-1); // -1 означает фильтрацию по всем колонкам

    // Обновляем подсветку в делегате
    //m_highlightDelegate->setFilter(text);
    ui->treeView->viewport()->update(); // Принудительно обновляем отображение
}
