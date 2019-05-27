#include "expsettingspage.h"
#include "exporttoxmlwizard.h"
#include "tablesdockwidget.h"
#include "fmttablesmodel.h"
#include "tablesselectwidget.h"
#include "fmtapplication.h"
#include "fmtimpexpwrp.h"
#include <QFileDialog>
#include <QProgressDialog>
#include <functional>

ExpSettingsPage::ExpSettingsPage(ExportToXmlWizard *parent)
    : QWizardPage(parent)
{
    pWizard = parent;

    verticalLayout_2 = new QVBoxLayout;
    verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));

    label = new QLabel(this);
    label->setObjectName(QStringLiteral("label"));
    label->setText(tr("Каталог выгрузки:"));

    horizontalLayout_2->addWidget(label);

    dirComboBox = new QComboBox(this);
    dirComboBox->setObjectName(QStringLiteral("dirComboBox"));

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(dirComboBox->sizePolicy().hasHeightForWidth());
    dirComboBox->setSizePolicy(sizePolicy);

    horizontalLayout_2->addWidget(dirComboBox);
    verticalLayout_2->addLayout(horizontalLayout_2);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));

    pSelectWidget = new TablesSelectWidget(pWizard->pConnection, this);
    pSelectWidget->setAddFunc(std::bind(&ExpSettingsPage::AddTable, this, std::placeholders::_1));
    pSelectWidget->setRemFunc(std::bind(&ExpSettingsPage::RemTable, this, std::placeholders::_1));
    horizontalLayout->addWidget(pSelectWidget);

    verticalLayout_2->addLayout(horizontalLayout);
    setLayout(verticalLayout_2);

    registerField("dir", dirComboBox, "currentText");

    UpdateDirCombo();
    UpdateButtons();

    setButtonText(QWizard::NextButton, tr("Начать"));

    connect(dirComboBox, SIGNAL(activated(int)), this, SLOT(dirComboItemActivated(int)));
}

void ExpSettingsPage::UpdateDirCombo()
{
    dirComboBox->clear();
    dirComboBox->addItem(QIcon(":/img/openfolderHS.png"), tr("<Выбрать>"));

    pWizard->m_pPrm->beginGroup("FmtXml");
    QStringList list = pWizard->m_pPrm->value("ExportDirs").toStringList();
    dirComboBox->addItems(list);
    pWizard->m_pPrm->endGroup();

    if (!list.size())
        dirComboBox->setCurrentIndex(-1);
    else
       dirComboBox->setCurrentIndex(1);
}

void ExpSettingsPage::dirComboItemActivated(int index)
{
    if (index == 0)
    {
        pWizard->m_pPrm->beginGroup("FmtXml");
        QString dir = QFileDialog::getExistingDirectory(this, tr("Каталог экспорта"));

        if (!dir.isEmpty())
        {
            QStringList list = pWizard->m_pPrm->value("ExportDirs").toStringList();
            if (!list.contains(dir))
            {
                if (list.size() > 10)
                    list.removeLast();
                list.prepend(dir);
            }
            pWizard->m_pPrm->setValue("ExportDirs", list);
        }
        pWizard->m_pPrm->endGroup();
        UpdateDirCombo();
    }
}

void ExpSettingsPage::UpdateButtons()
{
    emit QWizardPage::completeChanged();
}

bool ExpSettingsPage::AddTable(const QString table)
{
    bool result = false;
    if (!pWizard->pExp->isExists(table))
    {
        pWizard->pExp->addTable(table);
        UpdateButtons();
        result = true;
    }
    return result;
}

bool ExpSettingsPage::RemTable(const QString table)
{
    bool result = true;
    pWizard->pExp->removeTable(table);
    UpdateButtons();
    return result;
}

bool ExpSettingsPage::isComplete() const
{
    bool hr = (pWizard->pExp->tablesCount() && !dirComboBox->currentText().isEmpty());
    return hr;
}
