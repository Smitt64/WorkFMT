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
#include <toolsruntime.h>

FmtWorkWndGen::FmtWorkWndGen(QWidget *parent) :
    FmtCodeTabBase(parent),
    pInterface(nullptr),
    m_pUpdateScripts(nullptr)
{
    pContainer->setToolTip(tr("Область отображения сгенерированного кода"));
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

    QString interfaceDescription;
    if (!id.startsWith("macro:"))
    {
        pInterface = fmtGenInterfaceCreate(id);
        interfaceDescription = tr("Генератор кода: %1").arg(fmtGenInterfaceAlias(id));
    }
    else
    {
        GeneratorsProxyModel model;
        GeneratorsMacroElement element = model.getMacroElement(m_InterfaceId);
        GetGenMacroExecutor(&element, &pInterface);
        interfaceDescription = tr("Макрос: %1").arg(element.alias);
    }

    setToolTip(interfaceDescription);

    QStringList tabsNames = pInterface->tabs();
    if (!tabsNames.isEmpty())
    {
        for (const auto &tab : tabsNames)
        {
            QMdiSubWindow *wnd = AddTab(tab);
            wnd->setToolTip(tr("Результат генерации: %1 - %2").arg(interfaceDescription, tab));
        }
    }
    else
    {
        QString tab = fmtGenInterfaceAlias(m_InterfaceId);
        QMdiSubWindow *wnd = AddTab(tab);
        wnd->setToolTip(tr("Результат генерации: %1").arg(interfaceDescription));
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
        QString codeText = QString::fromLocal8Bit(data);
        pCode->setPlainText(codeText);

        setHighlighter(pCode, pInterface->getContentType());

        if (pCode->highlighter())
        {
            pCode->highlighter()->addType(pTable->name());
            pCode->highlighter()->addHighlightingRules(pInterface->highlightingRuleList());
            pCode->rehighlight();
        }

        // Добавляем информацию о размере сгенерированного кода
        int lineCount = codeText.count('\n') + 1;
        int charCount = codeText.length();
        pCode->setToolTip(tr("Сгенерированный код: %1 строк, %2 символов").arg(lineCount).arg(charCount));
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

    // Показываем уведомление об успешной генерации
    QToolTip::showText(QCursor::pos(),
                       tr("Генерация кода завершена успешно"),
                       this, QRect(), 2000);

    updateRibbonState();
}

void FmtWorkWndGen::generate()
{
    m_pUpdateScripts->setEnabled(false);

    // Показываем уведомление о начале генерации
    QToolTip::showText(QCursor::pos(),
                       tr("Генерация кода..."),
                       this, QRect(), 1000);

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

    // Добавляем описание для категории
    if (m_pRibbonCategory)
    {
        QString categoryDescription;
        if (!m_InterfaceId.startsWith("macro:"))
            categoryDescription = tr("Инструменты для работы с генератором кода: %1").arg(fmtGenInterfaceAlias(m_InterfaceId));
        else
        {
            GeneratorsProxyModel model;
            GeneratorsMacroElement element = model.getMacroElement(m_InterfaceId);
            categoryDescription = tr("Инструменты для работы с макросом: %1").arg(element.alias);
        }
        m_pRibbonCategory->setToolTip(categoryDescription);
    }
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

    // Обновляем состояние кнопки обновления
    if (m_pUpdateScripts)
    {
        bool hasActiveWindow = (pContainer->currentSubWindow() != nullptr);
        m_pUpdateScripts->setEnabled(hasActiveWindow);
    }
}

void FmtWorkWndGen::setupRibbonActions()
{
    m_pActionPannel->addSeparator();

    m_pUpdateScripts = createAction(tr("Обновить"), "UpdatedScript");
    toolAddActionWithTooltip(m_pUpdateScripts,
                             tr("Заново сгенерировать код на основе текущей структуры таблицы"),
                             QKeySequence::Refresh);
    m_pActionPannel->addLargeAction(m_pUpdateScripts);
    m_pUpdateScripts->setEnabled(false);

    // Добавляем информацию о текущем генераторе в панель
    if (!m_InterfaceId.isEmpty())
    {
        QString generatorInfo;
        if (!m_InterfaceId.startsWith("macro:"))
            generatorInfo = tr("Генератор: %1").arg(fmtGenInterfaceAlias(m_InterfaceId));
        else
        {
            GeneratorsProxyModel model;
            GeneratorsMacroElement element = model.getMacroElement(m_InterfaceId);
            generatorInfo = tr("Макрос: %1").arg(element.alias);
        }

        // Создаем информационную метку
        QLabel *infoLabel = new QLabel(generatorInfo, this);
        infoLabel->setToolTip(tr("Текущий активный генератор/макрос"));
        infoLabel->setStyleSheet("QLabel { padding: 5px; background-color: #f0f0f0; border-radius: 3px; }");
        m_pActionPannel->addWidget(infoLabel, SARibbonPannelItem::Large);
    }

    connect(m_pUpdateScripts, &QAction::triggered, [=]()
    {
        generate();
    });
}
