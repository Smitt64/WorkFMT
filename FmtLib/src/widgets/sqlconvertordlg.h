#ifndef SQLCONVERTORDLG_H
#define SQLCONVERTORDLG_H

#include <QDialog>

namespace Ui {
class SqlConvertorDlg;
}

class QSplitter;
class CodeEditor;
class QPlainTextEdit;
class QLineEdit;
class QLabel;
class SqlConvertorDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SqlConvertorDlg(QWidget *parent = nullptr);
    ~SqlConvertorDlg();

    void setUserName(const QString &name);

private slots:
    void onConvertClicked();
    void onClearLogClicked();

private:
    Ui::SqlConvertorDlg *ui;

    QSplitter *m_mainSplitter;
    QSplitter *m_editorSplitter;
    CodeEditor *m_pSource;
    CodeEditor *m_pResult;
    QPlainTextEdit *m_pLog;

    QLineEdit *m_usernameEdit;  // Поле для ввода имени пользователя
    QLabel *m_usernameLabel;    // Метка "Пользователь:"
};

#endif // SQLCONVERTORDLG_H
