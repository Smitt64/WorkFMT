#ifndef FILELISTPAGE_H
#define FILELISTPAGE_H

#include <QWizardPage>

namespace Ui {
class FileListPage;
}

class FileListPage Q_DECL_FINAL: public QWizardPage
{
    Q_OBJECT

public:
    explicit FileListPage(QWidget *parent = nullptr);
    ~FileListPage() Q_DECL_OVERRIDE;

    bool isComplete() const Q_DECL_OVERRIDE;
    void cleanupPage() Q_DECL_OVERRIDE;
    bool validatePage() Q_DECL_OVERRIDE;
    void initializePage() Q_DECL_OVERRIDE;
    //int nextId() const Q_DECL_OVERRIDE;

private slots:
    void currentIdChanged(int id);

private:
    Ui::FileListPage *ui;
};

#endif // FILELISTPAGE_H
