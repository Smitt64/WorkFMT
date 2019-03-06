#ifndef FMRICHTEXTWIDGET_H
#define FMRICHTEXTWIDGET_H

#include <QtWidgets>
#include <fmttable.h>

#define CALC_SIZE -2
typedef struct
{
    int insertBefore;
    bool readComment, readTableName, ignoreFirstRow;
}FmRichTextReadParam;

typedef struct
{
    FmtFldType type;
    int size;
}FmRichTextCreateFieldParam;

typedef QList<FmRichTextCreateFieldParam> CreateFieldParamList;

class FmRichTextWidget : public QWizardPage
{
    Q_OBJECT
public:
    FmRichTextWidget(QWidget *parent = nullptr);
    void setDocumentMinimumSize(const QSize &size);
    void ReadConetent(FmtSharedTablePtr &pTable, const FmRichTextReadParam &prm);
    void AddCustomWidget(const QString &label, QWidget *widget);

signals:

private slots:
    void onTextChanged();

private:
    CreateFieldParamList GetFieldsToCreate(const QString &str, const QString &comment = QString());
    void AddToCreateFieldParamList(CreateFieldParamList &list, const FmtFldType &type, const int &size = CALC_SIZE);
    void TryAutoMapFields();
    void SetAutoMapField(QComboBox *pCombo, const QString &name);
    QCheckBox *m_IngnoreNumbersCol, *m_IgnoreTitleRow;
    QLabel *pNameLabel, *pTypeLabel, *pCommentLabel;
    QComboBox *pNameCombo, *pTypeCombo, *pCommentCombo;
    QCheckBox *pStringCheck;
    QStandardItemModel m_ColumnsItems;

    QTextEdit *pDocument;
    QHBoxLayout *grid;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *pLabelspaces[2];
    QList<QLabel*> m_CustomLabels;
};

#endif // FMRICHTEXTWIDGET_H
