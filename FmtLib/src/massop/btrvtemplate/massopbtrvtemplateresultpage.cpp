#include "massopbtrvtemplateresultpage.h"
#include "ui_massopbtrvtemplateresultpage.h"
#include "errordlg.h"
#include "fmterrors.h"
#include "fmttable.h"
#include "massoperationwizard.h"
#include "../btrvtemplate/massopbtrvtemplate.h"
#include "fmtgencpptemplate.h"
#include "codeeditor.h"
#include <QAbstractButton>
#include <QThreadPool>
#include <QThread>
#include <QTextStream>

MassOpBtrvTemplateResultPageRun::MassOpBtrvTemplateResultPageRun(MassOpBtrvTemplate *Interface, QObject *parent) :
    QObject(parent),
    QRunnable (),
    pInterface(Interface)
{
    setAutoDelete(true);
}

MassOpBtrvTemplateResultPageRun::~MassOpBtrvTemplateResultPageRun()
{

}

void MassOpBtrvTemplateResultPageRun::setTables(const QStringList &list)
{
    m_Tables = list;
}

void MassOpBtrvTemplateResultPageRun::run()
{
    try {
        QScopedPointer<FmtGenCppTemplate> gen(new FmtGenCppTemplate());
        QList<QSharedPointer<FmtTable>> tables;
        int size = m_Tables.size();

        emit message(QString("Загрузка таблиц"));

        gen->initSettings();
        for (int i = 0; i < size; i++)
        {
            QSharedPointer<FmtTable> table(new FmtTable(pInterface->connectionInfo()));

            if (!table->load(m_Tables[i]))
                emit error(tr("Не удалось загрузить таблицу %1").arg(m_Tables[i]));
            else
            {
                emit message(QString("Таблица '%1' загружена").arg(m_Tables[i]));
                gen->CreateBlocks(table);
                tables.append(table);
            }
        }

        QSharedPointer<QByteArray> m_Structs(new QByteArray());
        QSharedPointer<QByteArray> m_FuncDecl(new QByteArray());
        QSharedPointer<QByteArray> m_BfCpp(new QByteArray());
        QSharedPointer<QByteArray> m_SkfCpp(new QByteArray());
        QSharedPointer<QByteArray> m_FindCpp(new QByteArray());
        m_Data[BFS] = m_Structs;
        m_Data[BF] = m_FuncDecl;
        m_Data[BFCPP] = m_BfCpp;
        m_Data[FINDCPP] = m_FindCpp;
        m_Data[SKFCPP] = m_SkfCpp;

        QTextStream structsStream(m_Structs.data());
        QTextStream funcdeclStream(m_FuncDecl.data());
        QTextStream bfCppStream(m_BfCpp.data());
        QTextStream skfCppStream(m_SkfCpp.data());
        QTextStream findCppStream(m_FindCpp.data());

        for (int i = 0; i < size; i++)
        {
            gen->createStruct(tables[i], structsStream);
            structsStream << endl;

            gen->createKeysUnion(tables[i], structsStream);
            structsStream << endl;

            gen->createKeysEnum(tables[i], structsStream);
            structsStream << endl;

            gen->createOpenFuncDecl(tables[i], bfCppStream, true);

            gen->WriteTableComment(tables[i], skfCppStream);
            gen->createSkfDeclFunctions(tables[i], skfCppStream, FmtGenCppTemplate::SkfMode_Create);
            gen->createSkfFunctions(tables[i], skfCppStream);
            skfCppStream << endl;

            gen->WriteTableComment(tables[i], findCppStream);
            gen->createFindFunctions(tables[i], findCppStream);
        }

        bfCppStream << endl;

        for (int i = 0; i < size; i++)
        {
            gen->createOpenFunc(tables[i], bfCppStream);
            bfCppStream << endl << endl;
            
            gen->createDeclExtern(tables[i], funcdeclStream);
            funcdeclStream << endl;
        }

    } catch (...) {
        emit error(tr("Exception handled"));
    }
    emit finished(m_Data);
}

// ============================================================================

MassOpBtrvTemplateResultPage::MassOpBtrvTemplateResultPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::MassOpBtrvTemplateResultPage)
{
    ui->setupUi(this);

    setTitle(tr("Результат"));
    pErrors = new FmtErrors(this);
    pErrDlg = new ErrorDlg(ErrorDlg::mode_Widget);
    pErrDlg->setErrors(pErrors);

    qRegisterMetaType<GenDataMap>();
    addPage(pErrDlg, "Отчет");
}

MassOpBtrvTemplateResultPage::~MassOpBtrvTemplateResultPage()
{
    delete ui;
}

void MassOpBtrvTemplateResultPage::addPage(QWidget *widget, const QString &title)
{
    ui->tabWidget->addTab(widget, title);
}

void MassOpBtrvTemplateResultPage::initializePage()
{
    fIsComplete = false;
    MassOperationWizard *wzrd = qobject_cast<MassOperationWizard*>(wizard());
    MassOpBtrvTemplate *pInterface = qobject_cast<MassOpBtrvTemplate*>(wzrd->getInterface());
    wzrd->button(QWizard::BackButton)->setEnabled(false);

    qDeleteAll(m_Highlighter);
    m_Highlighter.clear();
    for (int i = ui->tabWidget->count() - 1; i > 1; i--)
    {
        QWidget *w = ui->tabWidget->widget(i);
        ui->tabWidget->removeTab(i);
        delete w;
    }

    ui->progressBar->setMaximum(wzrd->tables().size());
    pErrors->clear();

    MassOpBtrvTemplateResultPageRun *run = new MassOpBtrvTemplateResultPageRun(pInterface);
    run->setTables(wzrd->tables());

    connect(run, &MassOpBtrvTemplateResultPageRun::progress, ui->progressBar, &QProgressBar::setValue);
    connect(run, SIGNAL(message(QString)), pErrors, SLOT(appendMessage(QString)));
    connect(run, SIGNAL(error(QString)), pErrors, SLOT(appendError(QString)));
    connect(run, &MassOpBtrvTemplateResultPageRun::finished, this, &MassOpBtrvTemplateResultPage::finished);
    QThreadPool::globalInstance()->start(run);
}

void MassOpBtrvTemplateResultPage::addPage(const QString &title, const QString &data)
{
    CodeEditor *pEditor = new CodeEditor;
    pEditor->setPlainText(data);
    pEditor->setReadOnly(true);
    Highlighter *hgltr = new Highlighter(pEditor->document());
    m_Highlighter.append(hgltr);
    addPage(pEditor, title);
}

void MassOpBtrvTemplateResultPage::finished(const GenDataMap &data)
{
    fIsComplete = true;

    QMapIterator<QString, QSharedPointer<QByteArray>> iter(data);
    while(iter.hasNext())
    {
        iter.next();
        QSharedPointer<QByteArray> dt = iter.value();
        addPage(iter.key(), QString::fromLocal8Bit(*dt.data()));
    }
    emit completeChanged();
}
