#ifndef FMTIMPORTFLDSOURCEPAGE_H
#define FMTIMPORTFLDSOURCEPAGE_H

#include <QWizardPage>
#include <QStyledItemDelegate>

namespace Ui {
class FmtImportFldSourcePage;
}

class QTreeView;
class QTextDocument;
class FilteredTableWidget;
class FmtSourceModel;
class SourceComboBoxDelegate;
class FieldsTableViewFilterController;
class FmtImportFldSourcePage : public QWizardPage
{
    Q_OBJECT

public:
    explicit FmtImportFldSourcePage(QWidget *parent = nullptr);
    ~FmtImportFldSourcePage();

    FmtSourceModel *sourceModel();
    virtual void initializePage() Q_DECL_OVERRIDE;

private:
    void extractFirstThreeRowsWithHeader(QTextDocument *m_pRichText);
    FmtSourceModel *m_pModel;
    SourceComboBoxDelegate *m_pDelegate;

    QTreeView *m_pTreeView;
    FilteredTableWidget *m_pFilteredWidget;
    FieldsTableViewFilterController *m_pFilteredWidgetController;
    Ui::FmtImportFldSourcePage *ui;
};

class FilteredTableWidget;
class SourceComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    SourceComboBoxDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

    void setSourceCount(const qint16 &Count);

private:
    static QStringList sourceNames(const qint16 &Type, const int &size, const qint16 &Count = 0);

    qint16 SourceCount = 0;
};

#endif // FMTIMPORTFLDSOURCEPAGE_H
