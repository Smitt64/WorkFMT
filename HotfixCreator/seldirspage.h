#ifndef SELDIRSPAGE_H
#define SELDIRSPAGE_H

#include <QWizardPage>
#include <QLineEdit>
#include <QFileDialog>
#include <QToolButton>
#include <QScopedPointer>

namespace Ui {
class SelDirsPage;
}

class SelectDirWrp Q_DECL_FINAL: public QObject
{
    Q_OBJECT
public:
    SelectDirWrp(QLineEdit *edit, QWidget *parent = nullptr) :
        QObject(parent)
    {
        m_widget = edit;
    }

public slots:
    void selectDirSlot()
    {
        QWizardPage *page = qobject_cast<QWizardPage*>(parent());
        QString dir = QFileDialog::getExistingDirectory(page, m_widget->toolTip(), m_widget->text());

        if (!dir.isEmpty())
        {
            m_widget->setText(dir);
            page->completeChanged();
        }
    }

private:
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
