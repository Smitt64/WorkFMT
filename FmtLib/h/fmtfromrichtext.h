#ifndef FMTFROMRICHTEXT_H
#define FMTFROMRICHTEXT_H

#include <QObject>
#include <QtWidgets>
#include <QSqlDatabase>
#include "fmtlib_global.h"

class QTextEdit;
class FmtWorkWindow;
class FmtTable;
class ConnectionInfo;
class FMTLIBSHARED_EXPORT FmtFromRichText : public QWizard
{
    Q_OBJECT
public:
    FmtFromRichText(ConnectionInfo *connection, QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~FmtFromRichText();

    bool isOpenInWorkspace() { return OpenCheck->isChecked(); }
    bool isCreateTable() { return CreateTableCheck->isChecked(); }
    bool isCreateIndex() { return CreateIndexCheck->isChecked(); }

    QSharedPointer<FmtTable> fmtTable() { return pFmtTable; }

private slots:
    void onPageChanged(const int &id);
    void onTextChanged();

private:
    int GetTypeSize(const QString &str, int *size);
    QWizardPage *createRichTextPage();
    QWizardPage *createFmtWorkPage();
    QWizardPage *createFinishPage();

    QWizardPage *pRichTextPage;
    QWizardPage *pFmtWorkPage;
    QWizardPage *pFinishPage;

    QTextEdit *pDocument;
    QSharedPointer<FmtTable> pFmtTable;
    FmtWorkWindow *pWorkWnd;

    QCheckBox *OpenCheck;
    QCheckBox *CreateTableCheck;
    QCheckBox *CreateIndexCheck;

    QCheckBox *m_IngnoreNumbersCol, *m_IgnoreTitleRow;
    QLabel *pNameLabel, *pTypeLabel, *pCommentLabel;
    QComboBox *pNameCombo, *pTypeCombo, *pCommentCombo;

    QStandardItemModel m_ColumnsItems;
};

#endif // FMTFROMRICHTEXT_H
