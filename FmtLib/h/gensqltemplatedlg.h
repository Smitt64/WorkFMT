#ifndef GENSQLTEMPLATEDLG_H
#define GENSQLTEMPLATEDLG_H

#include <QDialog>

namespace Ui {
class GenSqlTemplateDlg;
}

class FmtTable;
class FmtField;
class GenSqlTemplateDlg : public QDialog
{
    Q_OBJECT

public:
    enum SqlTemplate
    {
        sqlInsertWithDefaults = 0,
        sqlInsertWithPlaceholders,
        sqlInsertForRsdCommand,
        sqlUpdateWithDefaults,
        sqlUpdateWithPlaceholders,
        sqlUpdateForRsdCommand
    };
    explicit GenSqlTemplateDlg(QSharedPointer<FmtTable> &table, QWidget *parent = nullptr);
    ~GenSqlTemplateDlg();

    QString templateName() const;
    QString sqlTemplate() const;
    SqlTemplate templateID() const;

private slots:
    void mainFieldsBtnClick();
    void otherFieldsBtnClick();
    void currentIndexChanged(const int &index);

private:
    Ui::GenSqlTemplateDlg *ui;
    QSharedPointer<FmtTable> pTable;
    QList<FmtField*> m_FldList, m_OtherFldList;
};

#endif // GENSQLTEMPLATEDLG_H
