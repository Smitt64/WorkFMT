#ifndef EXPRESULTPAGE_H
#define EXPRESULTPAGE_H

#include <QtWidgets>

class ExportToXmlWizard;
class ErrorDlg;
class ExpResultPage : public QWizardPage
{
    Q_OBJECT
public:
    ExpResultPage(ExportToXmlWizard *parent = 0);
    virtual bool isComplete() const;

public slots:
    void prepare();
    void exportFinished(int code);

private slots:
    void openExportFolder();

private:
    ExportToXmlWizard *pWizard;

    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QProgressBar *progressBar;
    QPushButton *openFolderButton;
    //QPushButton *cancelButton;

    ErrorDlg *logTableView;

    bool fFinished;
};

#endif // EXPRESULTPAGE_H
