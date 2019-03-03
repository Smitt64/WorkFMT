#include "h/fmtworkwndgen.h"
#include "ui_fmtworkwndgen.h"
#include "fmtgeninterface.h"
#include "highlighter.h"
#include "codeeditor.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

FmtWorkWndGen::FmtWorkWndGen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FmtWorkWndGen),
    pCurrentHighlighter(NULL)
{
    ui->setupUi(this);

    pGenType = new QComboBox(this);
    ui->toolBar->addWidget(pGenType);

    pActionProperty = ui->toolBar->addAction(QIcon(":/img/Properties.png"), tr("Параметры"));
    pActionSave = ui->toolBar->addAction(QIcon(":/save"), tr("Сохранить"));
    ui->toolBar->addSeparator();
    pActionRun = ui->toolBar->addAction(QIcon(":/img/FormRunHS.png"), tr("Сгенерировать"));

    connect(pGenType, SIGNAL(currentIndexChanged(QString)), SLOT(interfaceComboSelected(QString)));
    connect(pActionRun, SIGNAL(triggered(bool)), SLOT(generate()));
    pGenType->addItems(fmtGenInterfaces());

    qDebug() << fmtGenAliases();

    pEditor = new CodeEditor(this);
    pEditor->setReadOnly(true);
    setCentralWidget(pEditor);
    UpdateSaveAction();

    connect(pActionSave, SIGNAL(triggered(bool)), SLOT(onSave()));
    connect(pActionProperty, SIGNAL(triggered(bool)), SLOT(onProperty()));
}

FmtWorkWndGen::~FmtWorkWndGen()
{
    delete ui;
}

void FmtWorkWndGen::setTable(QSharedPointer<FmtTable> table)
{
    pTable = table;
}

void FmtWorkWndGen::onFinish(const QByteArray &data)
{
    if (pCurrentHighlighter)
        delete pCurrentHighlighter;

    FmtGenInterface *pInterface = m_Interfaces[pGenType->currentText()];
    switch(pInterface->getContentType())
    {
    case FmtGenInterface::GenSql:
        pCurrentHighlighter = new SqlHighlighter(pEditor->document());
        break;
    case FmtGenInterface::GenCpp:
        pCurrentHighlighter = new Highlighter(pEditor->document());
        break;
    }

    pEditor->setPlainText(QString::fromLocal8Bit(data));
    pActionRun->setEnabled(true);
    UpdateSaveAction();
}

void FmtWorkWndGen::interfaceComboSelected(const QString &value)
{
    FmtGenInterface *pInterface = NULL;

    if (m_Interfaces.contains(value))
        pInterface = m_Interfaces[value];
    else
    {
        pInterface = fmtGenInterfaceCreate(value);
        m_Interfaces[value] = pInterface;
    }
    connect(pInterface, SIGNAL(finish(QByteArray)), SLOT(onFinish(QByteArray)));
    pActionProperty->setEnabled(pInterface->hasPropertes());
}

void FmtWorkWndGen::generate()
{
    m_Interfaces[pGenType->currentText()]->start(pTable);
    pActionRun->setEnabled(false);
}

void FmtWorkWndGen::UpdateSaveAction()
{
    if (pEditor->toPlainText().isEmpty())
        pActionSave->setEnabled(false);
    else
        pActionSave->setEnabled(true);
}

void FmtWorkWndGen::onProperty()
{
    m_Interfaces[pGenType->currentText()]->propertyEditor(this);
}

void FmtWorkWndGen::onSave()
{
    QString filter;
    FmtGenInterface *pInterface = m_Interfaces[pGenType->currentText()];

    switch(pInterface->getContentType())
    {
    case FmtGenInterface::GenSql:
        filter = "Sql files(*.sql)";
        break;
    case FmtGenInterface::GenCpp:
        filter = "C++ files(*.c *.cpp *.h *.hpp)";
        break;
    default:
        filter = "Text files(*.txt)";
    }

    QString fname = QFileDialog::getSaveFileName(this, QString(), QString(), filter);

    if (fname.isEmpty())
        return;

    QFile f(fname);
    if (f.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&f);
        stream.setCodec("IBM 866");
        stream << pEditor->toPlainText();
        f.close();
    }
}
