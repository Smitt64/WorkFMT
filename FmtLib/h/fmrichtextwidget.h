#ifndef FMRICHTEXTWIDGET_H
#define FMRICHTEXTWIDGET_H

#include <QtWidgets>
#include <fmttable.h>

typedef struct
{
    int insertBefore;
    bool readComment, readTableName, ignoreFirstRow;
}FmRichTextReadParam;

class FmRichTextWidget : public QWizardPage
{
    Q_OBJECT
public:
    explicit FmRichTextWidget(QWidget *parent = nullptr);
    void setDocumentMinimumSize(const QSize &size);
    void ReadConetent(FmtSharedTablePtr &pTable, const FmRichTextReadParam &prm);
    void AddCustomWidget(const QString &label, QWidget *widget);

signals:

private slots:
    void onTextChanged();

private:
    FmtFldType GetTypeSize(const QString &str, int *size, const QString &comment = QString());
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
