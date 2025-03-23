#include "generatorpage.h"
#include "errordlg.h"
#include "errorsmodel.h"
#include "hotfixcontentmodel.h"
#include "model/contenttreeitem.h"
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
    int progress = 0;
    UsrMakeHandle func = [&progress, this](const qint16 &action, ContentTreeItem *item)
    {
        emit progressChanged(++progress);
        /*if (action == UsrMakeBegin)
            m_Mutex.lock();
        else
            m_Mutex.unlock();*/
    };

    MakeParams params;
    params[PARAM_USER] = wzrd->field("User");
    params[PARAM_PASSWORD] = wzrd->field("Password");
    params[PARAM_SERVICE] = wzrd->field("Service");
    params[PARAM_IS_UNICODE] = wzrd->field("IsUnicode");

    params[PARAM_SOURCE_DIR] = wzrd->field("sourceEdit");
    params[PARAM_HOTFIX_DIR] = wzrd->field("hotfixEdit");
    params[PARAM_HOTFIX_NAME] = wzrd->field("hotfixName");
    params[PARAM_ORA_PG] = wzrd->field("checkOraPg");
    params[PARAM_UNPACKDBEXE] = wzrd->field("unpackDbExe");
    params[PARAM_BUILDINSTRUCTION] = wzrd->field("BuildInstruction");

    m_pModel->makeHotFix(m_pLog, params, func);
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

    //m_Progress->setTextVisible(false);

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
    op->wzrd = dynamic_cast<HotfixWizard*>(wizard());
    m_Progress->setMaximum(pModel->totalChildCount());

    connect(op, &GeneratorOp::progressChanged, m_Progress.data(), &QProgressBar::setValue);
    QThreadPool::globalInstance()->start(op);
}
