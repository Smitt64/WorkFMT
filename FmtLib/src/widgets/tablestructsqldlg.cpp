#include "tablestructsqldlg.h"
#include "ui_tablestructsqldlg.h"
#include "src/models/tableobjectsmodel.h"
#include "codeeditor/codeeditor.h"
#include "codeeditor/codehighlighter.h"
#include <QSettings>
#include "fmtapplication.h"

TableStructSqlDlg::TableStructSqlDlg(ConnectionInfo *connection, const QString &tableName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableStructSqlDlg),
    m_pConnection(connection)
{
    ui->setupUi(this);

    // Загрузка настроек
    FmtApplication *fmtapp = qobject_cast<FmtApplication*>(qApp);
    if (fmtapp)
    {
        QSettings *settings = fmtapp->settings();
        if (settings)
        {
            // Загружаем состояние чекбоксов
            ui->checkSimplified->setChecked(settings->value("TableStructSqlDlg/simplified", false).toBool());
            ui->checkWordWrap->setChecked(settings->value("TableStructSqlDlg/wordWrap", true).toBool());

            // Применяем перенос строк
            ui->plainTextEdit->setWordWrapMode(ui->checkWordWrap->isChecked() ? QTextOption::WrapAtWordBoundaryOrAnywhere : QTextOption::NoWrap);
        }
    }

    m_pModel.reset(new TableObjectsModel(connection, tableName));
    ui->treeView->setModel(m_pModel.data());

    ToolApplyHighlighter(ui->plainTextEdit, HighlighterSql);

    ui->treeView->expandAll();
    connect(ui->treeView, &QTreeView::clicked, this, &TableStructSqlDlg::onTreeItemClicked);

    // Подключаем сигналы чекбоксов
    connect(ui->checkSimplified, &QCheckBox::stateChanged, this, &TableStructSqlDlg::saveSettings);
    connect(ui->checkWordWrap, &QCheckBox::stateChanged, this, [this](int state) {
        ui->plainTextEdit->setWordWrapMode(state ? QTextOption::WrapAtWordBoundaryOrAnywhere : QTextOption::NoWrap);
        saveSettings();
    });
}

TableStructSqlDlg::~TableStructSqlDlg()
{
    delete ui;
}

void TableStructSqlDlg::saveSettings()
{
    FmtApplication *fmtapp = qobject_cast<FmtApplication*>(qApp);
    if (fmtapp)
    {
        QSettings *settings = fmtapp->settings();
        if (settings)
        {
            // Сохраняем состояния чекбоксов
            settings->setValue("TableStructSqlDlg/simplified", ui->checkSimplified->isChecked());
            settings->setValue("TableStructSqlDlg/wordWrap", ui->checkWordWrap->isChecked());
        }
    }
}

void TableStructSqlDlg::onTreeItemClicked(const QModelIndex &index)
{
    QString sql = m_pModel->getSqlForIndex(index, ui->checkSimplified->isChecked());
    ui->plainTextEdit->setPlainText(sql);
}
