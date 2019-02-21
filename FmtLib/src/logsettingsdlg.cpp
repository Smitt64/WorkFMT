#include "logsettingsdlg.h"
#include "ui_logsettingsdlg.h"
#include "loggingcategories.h"
#include "fmtapplication.h"
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>

LogSettingsDlg::LogSettingsDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogSettingsDlg)
{
    ui->setupUi(this);
    m_pApp = (FmtApplication*)qApp;

    const QStringList subcategorynames = QStringList()
            << "debug"
            << "info"
            << "warning"
            << "critical";

    QStringList lst = LoggingCategoryList();

    foreach (const QString &str, lst) {
        QTreeWidgetItem *category = new QTreeWidgetItem();
        category->setText(0, str);
        category->setCheckState(0, Qt::PartiallyChecked);
        ui->treeWidget->addTopLevelItem(category);

        int uncheckCount = 0;
        const QLoggingCategory &lc = LoggingCategory(str);
        if (QString(lc.categoryName()) == str)
        {
            foreach (const QString &sub, subcategorynames) {
                QTreeWidgetItem *subcategory = new QTreeWidgetItem();
                subcategory->setText(0, sub);

                if (sub == "debug")
                    subcategory->setCheckState(0, lc.isDebugEnabled() ? Qt::Checked : Qt::Unchecked);
                if (sub == "info")
                    subcategory->setCheckState(0, lc.isInfoEnabled() ? Qt::Checked : Qt::Unchecked);
                if (sub == "warning")
                    subcategory->setCheckState(0, lc.isWarningEnabled() ? Qt::Checked : Qt::Unchecked);
                if (sub == "critical")
                    subcategory->setCheckState(0, lc.isCriticalEnabled() ? Qt::Checked : Qt::Unchecked);

                if (subcategory->checkState(0) == Qt::Unchecked)
                    uncheckCount ++;

                category->addChild(subcategory);
            }

            if (uncheckCount == category->childCount())
                category->setCheckState(0, Qt::Unchecked);
            else if (uncheckCount == 0)
                category->setCheckState(0, Qt::Checked);
        }
    }
    ui->groupBox->setChecked(m_pApp->isLoggingEnabled());
    UpdateDailog();

    connect(ui->toolButton, SIGNAL(clicked(bool)), SLOT(OpenLogFile()));
    connect(ui->groupBox, SIGNAL(toggled(bool)), SLOT(OnLoggingChack(bool)));
    connect(ui->buttonBox, SIGNAL(accepted()), SLOT(ApplyRules()));
}

LogSettingsDlg::~LogSettingsDlg()
{
    delete ui;
}

void LogSettingsDlg::OpenLogFile()
{
    QDesktopServices::openUrl(QUrl(m_pApp->logginFileName()));
}

void LogSettingsDlg::UpdateDailog()
{
    ui->lineEdit->setToolTip(m_pApp->logginFileName());
    ui->lineEdit->setText(QFileInfo(m_pApp->logginFileName()).fileName());
}

QString LogSettingsDlg::CreateRules()
{
    QString rules;
    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *category = ui->treeWidget->topLevelItem(i);
        for (int j = 0; j < category->childCount(); j++)
        {
            QTreeWidgetItem *subcategory = category->child(j);
            if (!rules.isEmpty())
                rules == ";";
            rules += QString("%1.%2=%3")
                    .arg(category->text(0), subcategory->text(0))
                    .arg(subcategory->checkState(0) == Qt::Checked ? "true" : "false");
        }
    }

    return rules;
}

void LogSettingsDlg::OnLoggingChack(bool on)
{
    if (!on)
        m_pApp->disableLogging();
    else
    {
        if (!m_pApp->initLogging(CreateRules()))
            ui->groupBox->setChecked(false);
    }
    UpdateDailog();
}

void LogSettingsDlg::ApplyRules()
{
    if (m_pApp->isLoggingEnabled())
        m_pApp->setLoggingRules(CreateRules());
}
