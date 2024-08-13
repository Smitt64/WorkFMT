#ifndef FMTWORKWNDGEN_H
#define FMTWORKWNDGEN_H

#include <QtWidgets>
#include "fmtgeninterface.h"

namespace Ui {
class FmtWorkWndGen;
}

class GenInterfaceFactoryModel;
class CodeEditor;
class Highlighter;
class GeneratorsProxyModel;
class FmtWorkWndGen : public QMainWindow
{
    Q_OBJECT

public:
    explicit FmtWorkWndGen(QWidget *parent = Q_NULLPTR);
    virtual ~FmtWorkWndGen();

    void setTable(QSharedPointer<FmtTable> table);

private slots:
    void interfaceComboSelected(const QString &value);
    void generate();
    void onFinish(const QByteArray &data);
    void onSave();
    void onProperty();

private:
    void UpdateSaveAction();
    QString getInterfaceId() const;
    Ui::FmtWorkWndGen *ui;
    QComboBox *pGenType;
    GenInterfaceFactoryModel *pGenModel;
    GeneratorsProxyModel *pProxyModel;

    CodeEditor *pEditor;
    QAction *pActionRun, *pActionProperty, *pActionSave;

    QMap<QString, FmtGenInterface*> m_Interfaces;
    QSharedPointer<FmtTable> pTable;

    Highlighter *pCurrentHighlighter;
};

#endif // FMTWORKWNDGEN_H
