#ifndef CREATETABLEFROMSQLDLG_H
#define CREATETABLEFROMSQLDLG_H

#include <QDialog>
#include <QPlainTextEdit>
#include "src/core/createtablesqlparser.h"

class CodeEditor;
class QLineEdit;
class QLabel;
class QPushButton;

/**
 * @brief Диалог создания записи по коду create table.
 *
 * Позволяет вставить SQL-запрос CREATE TABLE, распарсить его,
 * задать имя таблицы, комментарий и создать поля нужных типов и размеров.
 * Созданная таблица открывается в редакторе, но не сохраняется.
 */
class CreateTableFromSqlDlg : public QDialog
{
    Q_OBJECT
public:
    explicit CreateTableFromSqlDlg(QWidget *parent = Q_NULLPTR);
    virtual ~CreateTableFromSqlDlg();

    // Результат парсинга (валиден после exec() == Accepted)
    CreateTableSqlResult result() const;

    // Имя и комментарий с учётом ручной корректировки в диалоге
    QString tableName() const;
    QString tableComment() const;

private slots:
    bool parseSql();
    void onOkClicked();

private:
    void updateFieldsInfo(int count);

    CodeEditor *m_pEditor;
    QLineEdit *m_pNameEdit;
    QLineEdit *m_pCommentEdit;
    QLabel *m_pFieldsLabel;
    QPushButton *m_pParseButton;
    QPushButton *m_pOkButton;
    CreateTableSqlResult m_Result;
};

#endif // CREATETABLEFROMSQLDLG_H