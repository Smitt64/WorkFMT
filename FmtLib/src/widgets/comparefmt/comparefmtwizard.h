#ifndef COMPAREFMTWIZARD_H
#define COMPAREFMTWIZARD_H

#include <QWizard>

class CompareParamsPage;
class CompareFmtWizard : public QWizard
{
    Q_OBJECT
public:
    CompareFmtWizard(QWidget *parent = nullptr);
    virtual ~CompareFmtWizard();

private:
    CompareParamsPage *m_ParamsPage;
};

#endif // COMPAREFMTWIZARD_H
