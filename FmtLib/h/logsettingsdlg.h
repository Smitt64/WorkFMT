#ifndef LOGSETTINGSDLG_H
#define LOGSETTINGSDLG_H

#include <QDialog>

namespace Ui {
class LogSettingsDlg;
}

class FmtApplication;
class LogSettingsDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LogSettingsDlg(QWidget *parent = nullptr);
    ~LogSettingsDlg();

private slots:
    void OpenLogFile();
    void OnLoggingChack(bool on);
    void ApplyRules();

private:
    void UpdateDailog();
    QString CreateRules();
    Ui::LogSettingsDlg *ui;
    FmtApplication *m_pApp;
};

#endif // LOGSETTINGSDLG_H
