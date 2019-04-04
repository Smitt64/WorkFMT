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

/*
bfopctrl.c

typedef struct {
	OPF_UNI openOON_PT_ADR;
	OPF_UNI openOON_PT_ADR_R;
	OPF_UNI openOON_PT;
	OPF_UNI openOON_PT_R;
	OPF_UNI openOON_PTENT_NAME;
	OPF_UNI openOON_PTENT_NAME_R;
	OPF_UNI openOON_PTIND_BDT;
	OPF_UNI openOON_PTIND_BDT_R;
	OPF_UNI openOON_PTIND_BPL;
	OPF_UNI openOON_PTIND_BPL_R;
	OPF_UNI openOON_PTIND_DOC;
	OPF_UNI openOON_PTIND_DOC_R;
	OPF_UNI openOON_PTIND_NAME;
	OPF_UNI openOON_PTIND_NAME_R;

	void* prev;
} T_FM_OPENFILESOON_FILES;

typedef struct {
	void *prev;          // ссылка на предыдущий вызов открывашки
	T_FM_OPENFILESOON_FILES flag;
} FMOpenFilesOONFlags;

static FMOpenFilesOONFlags      *currFilesOON;

void FM_CloseFilesOON()
{
	FMOpenFilesOONFlags *old;
	if (currFilesOON)
	{
		TBfErrorInfo BfError;
		BfSaveError(&BfError);

		if (bfstat == 0) 
			BfRestoreError(&BfError);

		BankCloseFileUNI(&FileOON_PT_ADR, &currFilesOON->flag.openOON_PT_ADR);
		BankCloseFileUNI(&FileOON_PT_ADR_R, &currFilesOON->flag.openOON_PT_ADR_R);
		BankCloseFileUNI(&FileOON_PT, &currFilesOON->flag.openOON_PT);
		BankCloseFileUNI(&FileOON_PT_R, &currFilesOON->flag.openOON_PT_R);
		BankCloseFileUNI(&FileOON_PTENT_NAME, &currFilesOON->flag.openOON_PTENT_NAME);
		BankCloseFileUNI(&FileOON_PTENT_NAME_R, &currFilesOON->flag.openOON_PTENT_NAME_R);
		BankCloseFileUNI(&FileOON_PTIND_BDT, &currFilesOON->flag.openOON_PTIND_BDT);
		BankCloseFileUNI(&FileOON_PTIND_BDT_R, &currFilesOON->flag.openOON_PTIND_BDT_R);
		BankCloseFileUNI(&FileOON_PTIND_BPL, &currFilesOON->flag.openOON_PTIND_BPL);
		BankCloseFileUNI(&FileOON_PTIND_BPL_R, &currFilesOON->flag.openOON_PTIND_BPL_R);
		BankCloseFileUNI(&FileOON_PTIND_DOC, &currFilesOON->flag.openOON_PTIND_DOC);
		BankCloseFileUNI(&FileOON_PTIND_DOC_R, &currFilesOON->flag.openOON_PTIND_DOC_R);
		BankCloseFileUNI(&FileOON_PTIND_NAME, &currFilesOON->flag.openOON_PTIND_NAME);
		BankCloseFileUNI(&FileOON_PTIND_NAME_R, &currFilesOON->flag.openOON_PTIND_NAME_R);

		old = currFilesOON;
		currFilesOON = (FMOpenFilesOONFlags*)currFilesOON->prev;
		FreeMem((void**)&old);
	}
}

int FM_OpenFilesOON(int fmode, int WhatToSave)
{
	int stat = 0;

	FMOpenFilesOONFlags *newF;

	newF = (FMOpenFilesOONFlags*)Allocate(sizeof(FMOpenFilesOONFlags));
	if (!newF)
		return OUT_OF_MEMORY;

	newF->prev = (void*)currFilesOON;
	currFilesOON = newF;

    if (!stat) stat = BankOpenFileUNI(FileOON_PT_ADR, iOpenOON_PT_ADR, fmode, OPF_SaveFilters, &newF->flag.openOON_PT_ADR);
	if (!stat) stat = BankOpenFileUNI(FileOON_PT_ADR_R, iOpenOON_PT_ADR_R, fmode, OPF_SaveFilters, &newF->flag.openOON_PT_ADR_R);
	if (!stat) stat = BankOpenFileUNI(FileOON_PT, iOpenOON_PT, fmode, OPF_SaveFilters, &newF->flag.openOON_PT);
	if (!stat) stat = BankOpenFileUNI(FileOON_PT_R, iOpenOON_PT_R, fmode, OPF_SaveFilters, &newF->flag.openOON_PT_R);
	if (!stat) stat = BankOpenFileUNI(FileOON_PTENT_NAME, iOpenOON_PTENT_NAME, fmode, OPF_SaveFilters, &newF->flag.openOON_PTENT_NAME);
	if (!stat) stat = BankOpenFileUNI(FileOON_PTENT_NAME_R, iOpenOON_PTENT_NAME_R, fmode, OPF_SaveFilters, &newF->flag.openOON_PTENT_NAME_R);
	if (!stat) stat = BankOpenFileUNI(FileOON_PTIND_BDT, iOpenOON_PTIND_BDT, fmode, OPF_SaveFilters, &newF->flag.openOON_PTIND_BDT);
	if (!stat) stat = BankOpenFileUNI(FileOON_PTIND_BDT_R, iOpenOON_PTIND_BDT_R, fmode, OPF_SaveFilters, &newF->flag.openOON_PTIND_BDT_R);
	if (!stat) stat = BankOpenFileUNI(FileOON_PTIND_BPL, iOpenOON_PTIND_BPL, fmode, OPF_SaveFilters, &newF->flag.openOON_PTIND_BPL);
	if (!stat) stat = BankOpenFileUNI(FileOON_PTIND_BPL_R, iOpenOON_PTIND_BPL_R, fmode, OPF_SaveFilters, &newF->flag.openOON_PTIND_BPL_R);
	if (!stat) stat = BankOpenFileUNI(FileOON_PTIND_DOC, iOpenOON_PTIND_DOC, fmode, OPF_SaveFilters, &newF->flag.openOON_PTIND_DOC);
	if (!stat) stat = BankOpenFileUNI(FileOON_PTIND_DOC_R, iOpenOON_PTIND_DOC_R, fmode, OPF_SaveFilters, &newF->flag.openOON_PTIND_DOC_R);
	if (!stat) stat = BankOpenFileUNI(FileOON_PTIND_NAME, iOpenOON_PTIND_NAME, fmode, OPF_SaveFilters, &newF->flag.openOON_PTIND_NAME);
	if (!stat) stat = BankOpenFileUNI(FileOON_PTIND_NAME_R, iOpenOON_PTIND_NAME_R, fmode, OPF_SaveFilters, &newF->flag.openOON_PTIND_NAME_R);

	if (stat)
		FM_CloseFilesOON();
	return stat;
}
*/

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

            if (tables[i]->hasNonUniqueIndexes())
            {
                gen->WriteTableComment(tables[i], skfCppStream);
                gen->createSkfDeclFunctions(tables[i], skfCppStream, FmtGenCppTemplate::SkfMode_Create);
                gen->createSkfFunctions(tables[i], skfCppStream);
                skfCppStream << endl;
            }

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
