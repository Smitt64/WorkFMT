#include "generatorpage.h"
#include "errordlg.h"
#include "errorsmodel.h"
#include "hotfixcontentmodel.h"
#include <QVBoxLayout>
#include <QProgressBar>
#include <QThreadPool>
#include "hotfixwizard.h"

GeneratorOp::GeneratorOp(QObject *parent) :
    QObject(parent),
    QRunnable()
{
    setAutoDelete(true);
}

GeneratorOp::~GeneratorOp()
{

}

void GeneratorOp::run()
{
    UsrMakeHandle func = [=](const qint16 &action, ContentTreeItem *item)
    {
        /*if (action == UsrMakeBegin)
            m_Mutex.lock();
        else
            m_Mutex.unlock();*/
    };

    m_pModel->makeHotFix(m_pLog, func);
}

// --------------------------------------------------------------------------

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

void GeneratorPage::initializePage()
{
    HotfixWizard *wzrd = (HotfixWizard*)wizard();
    HotfixContentModel *pModel = wzrd->structModel();

    GeneratorOp *op = new GeneratorOp();
    op->m_pLog = m_Log.data();
    op->m_pModel = pModel;

    QThreadPool::globalInstance()->start(op);
}
