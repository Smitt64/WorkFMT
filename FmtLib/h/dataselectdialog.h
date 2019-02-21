#ifndef DATASELECTDIALOG_H
#define DATASELECTDIALOG_H

#include <QtWidgets>
#include "fmtlib_global.h"

namespace Ui {
class DataSelectDialog;
}

class CodeEditor;
class SqlHighlighter;
class QSqlDatabase;
class FMTLIBSHARED_EXPORT DataSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataSelectDialog(QWidget *parent = 0);
    virtual ~DataSelectDialog();

    void setQueryString(const QString &str, QSqlDatabase *db);
    void setQuery(const QString &tableName, const QString &fields, QSqlDatabase *db);

    QVariant selectedValue(int role = Qt::DisplayRole);

private slots:
    void OnRefrash();
    void OnDoubleClicked(const QModelIndex &index);

private:
    Ui::DataSelectDialog *ui;
    CodeEditor *pEditor;
    SqlHighlighter *pHighlighter;
    QAbstractItemModel *pModel;
    QSqlDatabase *pdb;
};

#endif // DATASELECTDIALOG_H
