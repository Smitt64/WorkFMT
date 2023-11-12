#ifndef SUMMARYPAGE_H
#define SUMMARYPAGE_H

#include <QWizardPage>
#include <QTextStream>

namespace Ui {
class SummaryPage;
}

class SummaryPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit SummaryPage(QWidget *parent = nullptr);
    ~SummaryPage();

    virtual void initializePage();

private:
    void AddParam(const QString &key, const QString &value);

    Ui::SummaryPage *ui;
    QString m_Text;
    QScopedPointer<QTextStream> m_Stream;
};

#endif // SUMMARYPAGE_H
