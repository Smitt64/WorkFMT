#ifndef SELDIRSPAGE_H
#define SELDIRSPAGE_H

#include <QWizardPage>
#include <QLineEdit>
#include <QFileDialog>
#include <QToolButton>
#include <QScopedPointer>
#include <QSettings>
#include <selectfolderdlg.h>

namespace Ui {
class SelDirsPage;
}

class SelectDirWrp Q_DECL_FINAL: public QObject
{
    Q_OBJECT
public:
    SelectDirWrp(QLineEdit *edit, QSettings *settings, const QString &context, QWidget *parent = nullptr) :
        QObject(parent)
    {
        m_pSettings = settings;
        m_Context = context;
        m_widget = edit;
    }

public slots:
    void selectDirSlot()
    {
        QWizardPage *page = qobject_cast<QWizardPage*>(parent());

        SelectFolderDlg dlg(m_pSettings, m_Context, m_widget);
        if (dlg.exec() == QDialog::Accepted)
        {
            QString dir = dlg.selectedPath();

            if (!dir.isEmpty())
            {
                m_widget->setText(dir);
                emit page->completeChanged();
            }
        }
    }

private:
    QString m_Context;
    QSettings *m_pSettings;
    QLineEdit *m_widget;
};

class SelDirsPage Q_DECL_FINAL: public QWizardPage
{
    Q_OBJECT

public:
    explicit SelDirsPage(QWidget *parent = nullptr);
    ~SelDirsPage() Q_DECL_OVERRIDE;

    bool isComplete() const Q_DECL_OVERRIDE;

private:
    Ui::SelDirsPage *ui;
    SelectDirWrp *m_pSourceWrp;
    SelectDirWrp *m_pHotfixWrp;
};

#endif // SELDIRSPAGE_H
