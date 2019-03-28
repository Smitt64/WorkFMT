#ifndef MASSINITTABLEPARAMS_H
#define MASSINITTABLEPARAMS_H

#include <QWizardPage>

namespace Ui {
class MassInitTableParams;
}

class MassInitTablesParamModel;
class MassInitTableParams : public QWizardPage
{
    Q_OBJECT

public:
    explicit MassInitTableParams(QWidget *parent = nullptr);
    ~MassInitTableParams();
    void setTables(const QStringList &list);

    virtual void initializePage() Q_DECL_OVERRIDE;

private slots:
    void selectAllCreateTables();
    void selectAllCreateIndex();

private:
    Ui::MassInitTableParams *ui;
    MassInitTablesParamModel *pModel;
};

#endif // MASSINITTABLEPARAMS_H
