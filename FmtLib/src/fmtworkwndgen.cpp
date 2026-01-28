#include "h/fmtworkwndgen.h"
#include "ui_fmtworkwndgen.h"
#include "fmtgeninterface.h"
#include "geninterfacefactorymodel.h"
#include <codeeditor/codeeditor.h>
#include <codeeditor/codehighlighter.h>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QAbstractItemModel>
#include <QAbstractProxyModel>
#include <QDir>
#include <QRegularExpression>
#include "rslexecutors/generatorrslexecutor.h"
#include "src/models/generatorsproxymodel.h"

class GenMacroExecutor : public FmtGenInterface
{
public:
    GenMacroExecutor(const GeneratorsMacroElement *element) :
        FmtGenInterface()
    {
        m_Data = *element;
    }

    virtual ~GenMacroExecutor()
    {

    }

protected:
    virtual QMap<QString, QByteArray> makeContent(FmtSharedTablePtr pTable)
    {
        GeneratorRslExecutor executor(m_Data.macro, pTable.data());
        executor.execute();

        return QMap<QString, QByteArray>{{QString(), executor.data()}};
    }

private:
    GeneratorsMacroElement m_Data;
};

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
    pProxyModel = new GeneratorsProxyModel(this);
    pProxyModel->setSourceModel(pGenModel);

    pGenType->setModel(pProxyModel);//->addItems(fmtGenInterfaces());
    pGenType->setModelColumn(GenInterfaceFactoryModel::FieldAlias);

    pEditor = new CodeEditor(this);
    pEditor->setReadOnly(true);
    pEditor->setWordWrapMode(QTextOption::NoWrap);

    setCentralWidget(pEditor);
    UpdateSaveAction();
    ToolApplyHighlighter(pEditor, HighlighterCpp);
    pEditor->rehighlight();

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
    QString id = pProxyModel->data(pProxyModel->index(index, GenInterfaceFactoryModel::FieldKey), Qt::DisplayRole).toString();
    return id;
}

void FmtWorkWndGen::onFinish(const QByteArray &data)
{
    QString id = getInterfaceId();
    FmtGenInterface *pInterface = m_Interfaces[id];
    ToolApplyHighlighter(pEditor, pInterface->getContentType());

    if (pEditor->highlighter())
    {
        pEditor->highlighter()->addType(pTable->name());
        pEditor->highlighter()->addHighlightingRules(pInterface->highlightingRuleList());
        pEditor->rehighlight();
    }

    pEditor->setPlainText(QString::fromLocal8Bit(data));
    pActionRun->setEnabled(true);
    UpdateSaveAction();
}

void FmtWorkWndGen::interfaceComboSelected(const QString &value)
{
    Q_UNUSED(value);
    const QString id = getInterfaceId();

    if (!id.startsWith("macro:"))
    {
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
    else
    {
        QModelIndex start = pProxyModel->index(0, 0);
        QModelIndexList lst = pProxyModel->match(start, Qt::DisplayRole, id);

        if (!lst.isEmpty())
        {
            QModelIndex index = lst.first();

            GeneratorsMacroElement element = pProxyModel->getMacroElement(index);

            GenMacroExecutor *pMacroExecutor = Q_NULLPTR;

            if (m_Interfaces.contains(id))
                pMacroExecutor = dynamic_cast<GenMacroExecutor*>(m_Interfaces[id]);
            else
            {
                pMacroExecutor = new GenMacroExecutor(&element);
                m_Interfaces[id] = pMacroExecutor;

                connect(m_Interfaces[id], SIGNAL(finish(QByteArray)), SLOT(onFinish(QByteArray)));
                pActionProperty->setEnabled(m_Interfaces[id]->hasPropertes());
            }
        }
    }
}

void FmtWorkWndGen::generate()
{
    const QString id = getInterfaceId();

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
    const QString id = getInterfaceId();

    if (!id.startsWith("macro:"))
    {
        m_Interfaces[id]->propertyEditor(this);
    }
}

void FmtWorkWndGen::onSave()
{
    QString filter;
    FmtGenInterface *pInterface = m_Interfaces[getInterfaceId()];

    switch(pInterface->getContentType())
    {
    case HighlighterSql:
        filter = "Sql files(*.sql)";
        break;
    case HighlighterCpp:
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
