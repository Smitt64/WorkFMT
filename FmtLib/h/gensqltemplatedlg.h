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
    };
    explicit GenSqlTemplateDlg(QSharedPointer<FmtTable> &table, QWidget *parent = nullptr);
    ~GenSqlTemplateDlg();

    QString templateName() const;
    QString sqlTemplate() const;
    SqlTemplate templateID() const;

private slots:
    void mainFieldsBtnClick();

private:
    Ui::GenSqlTemplateDlg *ui;
    QSharedPointer<FmtTable> pTable;
    QList<FmtField*> m_FldList;
};

#endif // GENSQLTEMPLATEDLG_H
