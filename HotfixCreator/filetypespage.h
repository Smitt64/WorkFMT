#ifndef FILETYPESPAGE_H
#define FILETYPESPAGE_H

#include <QWizardPage>
#include <QAbstractTableModel>
#include "hotfixwizard.h"

namespace Ui {
class FileTypesPage;
}

class HotfixWizard;
class FileTypesModel Q_DECL_FINAL : public QAbstractTableModel
{
    Q_OBJECT

public:
    FileTypesModel(HotfixWizard *wizard, QObject *parent = nullptr);

    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

private:
    HotfixWizard *pWizard;
    QMap<QString, HotfixWizard::ContentFlag> m_FlagData;
};

class FileTypesPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit FileTypesPage(QWidget *parent = nullptr);
    ~FileTypesPage();

private:
    Ui::FileTypesPage *ui;
    FileTypesModel *m_pModel;
};

#endif // FILETYPESPAGE_H
