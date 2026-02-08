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
#include <SARibbon.h>

FmtWorkWndGen::FmtWorkWndGen(QWidget *parent) :
    FmtCodeTabBase(parent),
    pInterface(nullptr),
    m_pUpdateScripts(nullptr)
{
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

    QStringList tabsNames = pInterface->tabs();
    if (!tabsNames.isEmpty())
    {
        for (const auto &tab : tabsNames)
            AddTab(tab);
    }
    else
    {
        QString tab = fmtGenInterfaceAlias(m_InterfaceId);
        AddTab(tab);
    }

    connect(pInterface, &FmtGenInterface::finish, this, &FmtWorkWndGen::onFinish);

    updateRibbonState();
}

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

        setHighlighter(pCode, pInterface->getContentType());

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
                SetCodeToTab(m_pWindows[TabName], item.value());
        }
    }
    else
        SetCodeToTab(m_pWindowsList[0], data[QString()]);

    pContainer->setActiveSubWindow(m_pWindowsList[0]);
    m_pUpdateScripts->setEnabled(true);

    updateRibbonState();
}

void FmtWorkWndGen::generate()
{
    m_pUpdateScripts->setEnabled(false);
    pInterface->start(pTable);
}

void FmtWorkWndGen::onProperty()
{
    /*const QString id = getInterfaceId();

    if (!id.startsWith("macro:"))
    {
        m_Interfaces[id]->propertyEditor(this);
    }*/
}

QString FmtWorkWndGen::ribbonCategoryName() const
{
    return tr("Код");
}

void FmtWorkWndGen::initRibbonPanels()
{
    FmtCodeTabBase::initRibbonPanels();
}

void FmtWorkWndGen::activateRibbon()
{
    FmtCodeTabBase::activateRibbon();
    updateRibbonState();
}

void FmtWorkWndGen::deactivateRibbon()
{
    FmtCodeTabBase::deactivateRibbon();
}

void FmtWorkWndGen::updateRibbonState()
{
    FmtCodeTabBase::updateRibbonState();
}

void FmtWorkWndGen::setupRibbonActions()
{
    m_pActionPannel->addSeparator();
    m_pUpdateScripts = createAction(tr("Обновить"), "UpdatedScript");
    m_pActionPannel->addLargeAction(m_pUpdateScripts);
    m_pUpdateScripts->setEnabled(false);

    connect(m_pUpdateScripts, &QAction::triggered, [=]()
    {
        generate();
    });
}
