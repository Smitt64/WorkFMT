#ifndef FMTWORKWNDGEN_H
#define FMTWORKWNDGEN_H

#include <QtWidgets>
#include "src/widgets/fmtworkwindow/fmtwindowtabinterface.h"
#include "fmtgeninterface.h"

class GenInterfaceFactoryModel;
class CodeEditor;
class Highlighter;
class GeneratorsProxyModel;
class QMdiSubWindow;
class FmtWorkWndGen : public FmtWindowTabInterface
{
    Q_OBJECT

public:
    explicit FmtWorkWndGen(QWidget *parent = Q_NULLPTR);
    virtual ~FmtWorkWndGen();

    void generate();
    void setTable(QSharedPointer<FmtTable> table);
    void setInterfaceID(const QString &id);

    const QString &interfaceId() const;

private slots:
    void interfaceComboSelected(const QString &value);
    void onFinish(const QMap<QString, QByteArray> &data);
    void onSave();
    void onProperty();

private:
    void UpdateSaveAction();

    QString m_InterfaceId;
    FmtGenInterface *pInterface;
    QMdiArea *pContainer;
    CodeEditor *pEditor;
    /*QString getInterfaceId() const;
    QComboBox *pGenType;
    GenInterfaceFactoryModel *pGenModel;
    GeneratorsProxyModel *pProxyModel;*/

    /*CodeEditor *pEditor;
    QAction *pActionRun, *pActionProperty, *pActionSave;*/

    //QMap<QString, FmtGenInterface*> m_Interfaces;
    QSharedPointer<FmtTable> pTable;
    QMap<QString, QMdiSubWindow*> m_pWindows;
    QList<QMdiSubWindow*> m_pWindowsList;

    //Highlighter *pCurrentHighlighter;
};

#endif // FMTWORKWNDGEN_H
