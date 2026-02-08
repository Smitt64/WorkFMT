#ifndef FMTWORKWNDGEN_H
#define FMTWORKWNDGEN_H

#include <QtWidgets>
#include "src/widgets/fmtworkwindow/fmtcodetabbase.h"
#include "fmtgeninterface.h"

class GenInterfaceFactoryModel;
class CodeEditor;
class Highlighter;
class GeneratorsProxyModel;
class QMdiSubWindow;
class SARibbonCategory;
class SARibbonPannel;
class FmtWorkWndGen : public FmtCodeTabBase
{
    Q_OBJECT

public:
    explicit FmtWorkWndGen(QWidget *parent = Q_NULLPTR);
    virtual ~FmtWorkWndGen();

    void generate();
    void setTable(QSharedPointer<FmtTable> table);
    void setInterfaceID(const QString &id);

    const QString &interfaceId() const;

    virtual QString ribbonCategoryName() const override;
    virtual void initRibbonPanels() override;
    virtual void activateRibbon() override;
    virtual void deactivateRibbon() override;

protected:
    virtual void updateRibbonState() override;

private slots:
    void onFinish(const QMap<QString, QByteArray> &data);
    void onProperty();

private:
    virtual void setupRibbonActions() override;

    QString m_InterfaceId;
    FmtGenInterface *pInterface;

    QSharedPointer<FmtTable> pTable;

    QAction *m_pUpdateScripts;
};

#endif // FMTWORKWNDGEN_H
