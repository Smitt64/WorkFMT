#ifndef SQLCONVERTORCATEGORY_H
#define SQLCONVERTORCATEGORY_H

#include <QPlainTextEdit>
#include <QLineEdit>
#include <QToolButton>
#include <QLabel>
#include <QSplitter>
#include <ribbon/CategoryContentWidget.h>
#include <toolsqlconverter.h>

class CodeEditor;

class SqlConvertorCategory : public CategoryContentWidget
{
    Q_OBJECT

public:
    explicit SqlConvertorCategory(QWidget *parent = nullptr);
    virtual ~SqlConvertorCategory();

    // Методы доступа
    QString sourceSql() const;
    QString resultSql() const;
    QString logText() const;
    QString userName() const;

    void setSourceSql(const QString &sql);
    void setResultSql(const QString &sql);
    void setLogText(const QString &text);
    void setUserName(const QString &name);

    void clearLog();
    void clearAll();

signals:
    void convertRequested(const QString &sql, const QString &userName);
    void conversionCompleted(const QString &result, const QString &error);

public slots:
    void onConvert();
    void onClearLog();
    void setConversionResult(const QString &result, const QString &error = QString());

private:
    void setupLeftPanel();
    void setupRightPanel();
    void setupButtons();
    void applyButtonStyle(QToolButton *button);
    void addToLog(const QString &message);
    SqlConversionResult performConversion(const QString &sql, const QString &userName);

    // Левая панель - компоненты
    QWidget *m_leftPanelContent;
    QWidget *m_buttonContainer;
    QToolButton *m_convertButton;
    QToolButton *m_clearLogButton;
    QLabel *m_usernameLabel;
    QLineEdit *m_usernameEdit;

    // Правая панель - компоненты
    QSplitter *m_mainSplitter;
    QSplitter *m_editorSplitter;
    CodeEditor *m_sourceEditor;
    CodeEditor *m_resultEditor;
    QPlainTextEdit *m_logEditor;
};

#endif // SQLCONVERTORCATEGORY_H