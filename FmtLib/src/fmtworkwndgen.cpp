#include "h/fmtworkwndgen.h"
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
#include <QMdiArea>

FmtWorkWndGen::FmtWorkWndGen(QWidget *parent) :
    FmtWindowTabInterface(parent),
    pInterface(nullptr),
    pContainer(nullptr),
    pEditor(nullptr)
{
    /*pGenType = new QComboBox(this);
    pActionProperty = ui->toolBar->addAction(QIcon(":/img/Properties.png"), tr("Параметры"));
    pActionSave = ui->toolBar->addAction(QIcon(":/save"), tr("Сохранить"));
    ui->toolBar->addSeparator();
    pActionRun = ui->toolBar->addAction(QIcon(":/img/FormRunHS.png"), tr("Сгенерировать"));*/

    //connect(pGenType, SIGNAL(currentIndexChanged(QString)), SLOT(interfaceComboSelected(QString)));
    //connect(pActionRun, SIGNAL(triggered(bool)), SLOT(generate()));

    /*pGenModel = new GenInterfaceFactoryModel(this);
    pProxyModel = new GeneratorsProxyModel(this);
    pProxyModel->setSourceModel(pGenModel);*/

    /*pGenType->setModel(pProxyModel);//->addItems(fmtGenInterfaces());
    pGenType->setModelColumn(GenInterfaceFactoryModel::FieldAlias);*/

    /*pEditor = new CodeEditor(this);
    pEditor->setReadOnly(true);
    pEditor->setWordWrapMode(QTextOption::NoWrap);*/

    //setCentralWidget(pEditor);
    //UpdateSaveAction();
    //ToolApplyHighlighter(pEditor, HighlighterCpp);
    //pEditor->rehighlight();

    //connect(pActionSave, SIGNAL(triggered(bool)), SLOT(onSave()));
    //connect(pActionProperty, SIGNAL(triggered(bool)), SLOT(onProperty()));

    //setInterfaceID("FmtGenCppTemplate");
}

FmtWorkWndGen::~FmtWorkWndGen()
{
    qDeleteAll(m_pWindowsList);
    m_pWindowsList.clear();
    m_pWindows.clear();
    delete pInterface;
}

void FmtWorkWndGen::setTable(QSharedPointer<FmtTable> table)
{
    pTable = table;
}

const QString &FmtWorkWndGen::interfaceId() const
{
    return m_InterfaceId;
}

void FmtWorkWndGen::setInterfaceID(const QString &id)
{
    m_InterfaceId = id;

    if (!id.startsWith("macro:"))
        pInterface = fmtGenInterfaceCreate(id);
    else
    {
        GeneratorsProxyModel model;
        GeneratorsMacroElement element = model.getMacroElement(m_InterfaceId);
        GetGenMacroExecutor(&element, &pInterface);
    }

    auto AddTab = [=](const QString &tabname) -> QMdiSubWindow*
    {
        CodeEditor *pCode = new CodeEditor(this);
        pCode->setReadOnly(true);

        QMdiSubWindow *mdi = pContainer->addSubWindow(pCode);
        mdi->setWindowTitle(tabname);
        mdi->setAttribute(Qt::WA_DeleteOnClose);
        mdi->setWindowIcon(QIcon::fromTheme("Code"));
        mdi->showMaximized();

        return mdi;
    };

    pContainer = new QMdiArea(this);
    pContainer->setDocumentMode(true);
    pContainer->setViewMode(QMdiArea::TabbedView);
    pContainer->setTabsClosable(false);
    //pContainer->setTabPosition(QTabWidget::South);
    setCentralWidget(pContainer);

    QStringList tabsNames = pInterface->tabs();
    if (!tabsNames.isEmpty())
    {
        for (const auto &tab : tabsNames)
        {
            QMdiSubWindow *window = AddTab(tab);

            m_pWindows.insert(tab, window);
            m_pWindowsList.append(window);
        }
    }
    else
    {
        QString tab = fmtGenInterfaceAlias(m_InterfaceId);
        QMdiSubWindow *window = AddTab(tab);

        m_pWindows.insert(tab, window);
        m_pWindowsList.append(window);
    }

    connect(pInterface, &FmtGenInterface::finish, this, &FmtWorkWndGen::onFinish);
}

/*QString FmtWorkWndGen::getInterfaceId() const
{
    int index = pGenType->currentIndex();
    QString id = pProxyModel->data(pProxyModel->index(index, GenInterfaceFactoryModel::FieldKey), Qt::DisplayRole).toString();
    return id;
}*/

void FmtWorkWndGen::onFinish(const QMap<QString, QByteArray> &data)
{
    QList<QMdiSubWindow*> windows = pContainer->subWindowList();
    for (auto wnd : windows)
    {
        CodeEditor *pCode = qobject_cast<CodeEditor*>(wnd->widget());
        pCode->clear();
    }

    auto SetCodeToTab = [=](QMdiSubWindow *window, const QByteArray &data)
    {
        CodeEditor *pCode = qobject_cast<CodeEditor*>(window->widget());
        pCode->setPlainText(QString::fromLocal8Bit(data));

        ToolApplyHighlighter(pCode, pInterface->getContentType(), "office2013_highlighter");

        if (pCode->highlighter())
        {
            pCode->highlighter()->addType(pTable->name());
            pCode->highlighter()->addHighlightingRules(pInterface->highlightingRuleList());
            pCode->rehighlight();
        }
    };

    if (data.size() > 1 && !data.contains(QString()))
    {
        QMapIterator<QString, QByteArray> iter(data);
        while (iter.hasNext())
        {
            auto item = iter.next();
            QString TabName = item.key();

            if (m_pWindows.contains(TabName))
            {
                SetCodeToTab(m_pWindows[TabName], item.value());
                /*CodeEditor *pCode = qobject_cast<CodeEditor*>(m_pWindows[TabName]->widget());
                pCode->setPlainText(QString::fromLocal8Bit(item.value()));

                ToolApplyHighlighter(pCode, pInterface->getContentType(), "office2013_highlighter");

                if (pCode->highlighter())
                {
                    pCode->highlighter()->addType(pTable->name());
                    pCode->highlighter()->addHighlightingRules(pInterface->highlightingRuleList());
                    pCode->rehighlight();
                }*/
            }
        }
    }
    else
        SetCodeToTab(m_pWindowsList[0], data[QString()]);

    pContainer->setActiveSubWindow(m_pWindowsList[0]);

    /*FmtGenInterface *pInterface = m_Interfaces[id];
    ToolApplyHighlighter(pEditor, pInterface->getContentType());

    if (pEditor->highlighter())
    {
        pEditor->highlighter()->addType(pTable->name());
        pEditor->highlighter()->addHighlightingRules(pInterface->highlightingRuleList());
        pEditor->rehighlight();
    }

    pEditor->setPlainText(QString::fromLocal8Bit(data));
    pActionRun->setEnabled(true);
    UpdateSaveAction();*/
}

void FmtWorkWndGen::interfaceComboSelected(const QString &value)
{
    Q_UNUSED(value);
    /*const QString id = getInterfaceId();

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
    }*/
}

void FmtWorkWndGen::generate()
{
    pInterface->start(pTable);
    /*const QString id = getInterfaceId();

    m_Interfaces[getInterfaceId()]->start(pTable);
    pActionRun->setEnabled(false);*/
}

void FmtWorkWndGen::UpdateSaveAction()
{
    /*if (pEditor->toPlainText().isEmpty())
        pActionSave->setEnabled(false);
    else
        pActionSave->setEnabled(true);*/
}

void FmtWorkWndGen::onProperty()
{
    /*const QString id = getInterfaceId();

    if (!id.startsWith("macro:"))
    {
        m_Interfaces[id]->propertyEditor(this);
    }*/
}

void FmtWorkWndGen::onSave()
{
    /*QString filter;
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
    }*/
}
