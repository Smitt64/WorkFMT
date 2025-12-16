#include "wordpreviewregpage.h"
#include "ui_wordpreviewregpage.h"
#include "rsl/extractinfoexecutor.h"
#include "regparmwizard.h"
#include "rsl/reginfoobj.h"
#include <QHeaderView>

WordPreviewRegPage::WordPreviewRegPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::WordPreviewRegPage),
    m_pModel(new RegInfoObjModel())
{
    ui->setupUi(this);

    ui->treeView->setModel(m_pModel.data());

    // Настройка отображения treeView
    setupTreeView();
}

WordPreviewRegPage::~WordPreviewRegPage()
{
    delete ui;
}

QSharedPointer<RegInfoObjModel> WordPreviewRegPage::model()
{
    return m_pModel;
}

void WordPreviewRegPage::setupTreeView()
{
    // Настраиваем заголовки
    QHeaderView *header = ui->treeView->header();

    // Устанавливаем минимальные ширины для булевых колонок
    header->setMinimumSectionSize(30); // Минимальная ширина для чекбоксов

    // Устанавливаем фиксированные ширины для булевых колонок
    ui->treeView->setColumnWidth(RegInfoObjModel::IsGlobalColumn, 30);
    ui->treeView->setColumnWidth(RegInfoObjModel::IsNodeColumn, 30);
    ui->treeView->setColumnWidth(RegInfoObjModel::IsSecurityColumn, 30);

    // Устанавливаем ширину для колонки типа
    ui->treeView->setColumnWidth(RegInfoObjModel::TypeNameColumn, 100);

    // Колонка "Полное имя" растягивается по возможности
    header->setSectionResizeMode(RegInfoObjModel::FullNameColumn, QHeaderView::Stretch);

    // Остальные колонки фиксированного размера
    header->setSectionResizeMode(RegInfoObjModel::TypeNameColumn, QHeaderView::Fixed);
    header->setSectionResizeMode(RegInfoObjModel::IsGlobalColumn, QHeaderView::Fixed);
    header->setSectionResizeMode(RegInfoObjModel::IsNodeColumn, QHeaderView::Fixed);
    header->setSectionResizeMode(RegInfoObjModel::IsSecurityColumn, QHeaderView::Fixed);
    header->setSectionResizeMode(RegInfoObjModel::CommentColumn, QHeaderView::Interactive);
    header->setSectionResizeMode(RegInfoObjModel::DefaultValueColumn, QHeaderView::Interactive);

    // Устанавливаем делегаты для редактирования
    ui->treeView->setItemDelegateForColumn(RegInfoObjModel::TypeNameColumn, new TypeComboBoxDelegate(this));
    ui->treeView->setItemDelegateForColumn(RegInfoObjModel::DefaultValueColumn, new DefaultValueDelegate(this));

    // Настраиваем отображение
    ui->treeView->setAlternatingRowColors(true);
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeView->setRootIsDecorated(false);

    // Включаем редактирование по двойному клику
    ui->treeView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);

    // Скрываем вертикальный заголовок
    ui->treeView->header()->setVisible(true);
}

void WordPreviewRegPage::initializePage()
{
    RegParmWizard *wzrd = (RegParmWizard*)wizard();

    ExtractInfoExecutor executor;
    executor.setDocument(wzrd->wordContentDocument());
    executor.setDebugMacroFlag(true);
    executor.playRep("extract_info.mac");

    m_pModel->setRegInfoObjects(executor.objList());

    // После загрузки данных дополнительно настраиваем ширины
    adjustColumnsAfterDataLoad();
}

void WordPreviewRegPage::adjustColumnsAfterDataLoad()
{
    // Автоподбор ширины для комментария и значения по умолчанию
    ui->treeView->resizeColumnToContents(RegInfoObjModel::CommentColumn);
    ui->treeView->resizeColumnToContents(RegInfoObjModel::DefaultValueColumn);

    // Ограничиваем максимальную ширину для комментария и значения
    int maxWidth = ui->treeView->width() / 3;
    if (ui->treeView->columnWidth(RegInfoObjModel::CommentColumn) > maxWidth) {
        ui->treeView->setColumnWidth(RegInfoObjModel::CommentColumn, maxWidth);
    }
    if (ui->treeView->columnWidth(RegInfoObjModel::DefaultValueColumn) > maxWidth) {
        ui->treeView->setColumnWidth(RegInfoObjModel::DefaultValueColumn, maxWidth);
    }
}
