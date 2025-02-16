#include "generatorpage.h"
#include "errordlg.h"
#include "errorsmodel.h"
#include <QVBoxLayout>
#include <QProgressBar>

GeneratorPage::GeneratorPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle("Создание хотфикса");
    m_Layout.reset(new QVBoxLayout());
    m_Progress.reset(new QProgressBar());
    m_LogWidget.reset(new ErrorDlg(ErrorDlg::ModeWidget));
    m_Log.reset(new ErrorsModel());

    m_Progress->setTextVisible(false);

    m_Layout->addWidget(m_LogWidget.data());
    m_Layout->addWidget(m_Progress.data());

    m_LogWidget->layout()->setMargin(0);
    m_LogWidget->setErrors(m_Log.data());

    setLayout(m_Layout.data());
}

GeneratorPage::~GeneratorPage()
{

}
