#ifndef FMTAPPLICATIONWIDGET_H
#define FMTAPPLICATIONWIDGET_H

#include <ribbon/applicationwidgetbase.h>

class GeneralOptions;
class CategoryContentWidget;
class ExternalToolsPage;
class ConnectionInfo;
class TableGroupOptions;
class AppOptionsContentWidget;
class SARibbonActionsManager;
class RslExecutor;

class FmtApplicationWidget : public ApplicationWidgetBase
{
    Q_OBJECT
public:
    FmtApplicationWidget(SARibbonMainWindow *parent = nullptr);
    virtual ~FmtApplicationWidget();

    void setCurrentConnection(ConnectionInfo *info);
    void initMacroCommands();

protected:
    void setupTabs();

private slots:
    void onTabChanged(int index);
    void onBackButtonClicked();

private:
    AppOptionsContentWidget *m_pOptionsWidget;
    void InitGeneralOptions();
    void InitExternalToolsPage();

    // examples
    void createFileTab();      // Вкладка с LayoutListDetail
    void createHomeTab();      // Вкладка с LayoutGroups
    void createInsertTab();    // Вкладка с LayoutLeftPanel (пример с произвольными виджетами)
    void createViewTab();      // Вкладка с LayoutFullWidget

    // Виджеты для вкладок
    CategoryContentWidget *m_fileWidget;
    CategoryContentWidget *m_homeWidget;
    CategoryContentWidget *m_insertWidget;
    CategoryContentWidget *m_viewWidget;

    QListWidgetItem *m_TableGroupItem;
    ExternalToolsPage *m_pExternalToolsPage;
    GeneralOptions *m_pGeneralOptions;
    TableGroupOptions *m_pTableGroupOptions;
    SARibbonActionsManager *m_actionsManager;
    RslExecutor *m_executor;
};

#endif // FMTAPPLICATIONWIDGET_H