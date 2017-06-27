#ifndef FMTWORKWNDGEN_H
#define FMTWORKWNDGEN_H

#include <QtWidgets>
#include "fmtgeninterface.h"

namespace Ui {
class FmtWorkWndGen;
}

class CodeEditor;
class FmtWorkWndGen : public QMainWindow
{
    Q_OBJECT

public:
    explicit FmtWorkWndGen(QWidget *parent = 0);
    ~FmtWorkWndGen();

    void setTable(QSharedPointer<FmtTable> table);

private slots:
    void interfaceComboSelected(const QString &value);
    void generate();
    void onFinish(const QByteArray &data);
    void onSave();
    void onProperty();

private:
    void UpdateSaveAction();
    Ui::FmtWorkWndGen *ui;
    QComboBox *pGenType;

    CodeEditor *pEditor;
    QAction *pActionRun, *pActionProperty, *pActionSave;

    QMap<QString, FmtGenInterface*> m_Interfaces;
    QSharedPointer<FmtTable> pTable;

    QSyntaxHighlighter *pCurrentHighlighter;
};

#endif // FMTWORKWNDGEN_H
