#include "h/fmtworkwndgen.h"
#include "ui_fmtworkwndgen.h"
#include "fmtgeninterface.h"
#include "geninterfacefactorymodel.h"
#include "highlighter.h"
#include "codeeditor.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

FmtWorkWndGen::FmtWorkWndGen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FmtWorkWndGen),
    pCurrentHighlighter(nullptr)
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

    pGenModel = new GenInterfaceFactoryModel(this);
    pGenType->setModel(pGenModel);//->addItems(fmtGenInterfaces());
    pGenType->setModelColumn(GenInterfaceFactoryModel::FieldAlias);

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

QString FmtWorkWndGen::getInterfaceId() const
{
    int index = pGenType->currentIndex();
    QString id = pGenModel->data(pGenModel->index(index, GenInterfaceFactoryModel::FieldKey), Qt::DisplayRole).toString();
    return id;
}

void FmtWorkWndGen::onFinish(const QByteArray &data)
{
    if (pCurrentHighlighter)
        delete pCurrentHighlighter;

    FmtGenInterface *pInterface = m_Interfaces[getInterfaceId()];
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
    Q_UNUSED(value);
    const QString id = getInterfaceId();
    FmtGenInterface *pInterface = Q_NULLPTR;

    if (m_Interfaces.contains(id))
        pInterface = m_Interfaces[id];
    else
    {
        pInterface = fmtGenInterfaceCreate(id);
        m_Interfaces[id] = pInterface;
    }
    connect(pInterface, SIGNAL(finish(QByteArray)), SLOT(onFinish(QByteArray)));
    pActionProperty->setEnabled(pInterface->hasPropertes());
}

void FmtWorkWndGen::generate()
{
    m_Interfaces[getInterfaceId()]->start(pTable);
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
    m_Interfaces[getInterfaceId()]->propertyEditor(this);
}

void FmtWorkWndGen::onSave()
{
    QString filter;
    FmtGenInterface *pInterface = m_Interfaces[getInterfaceId()];

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
