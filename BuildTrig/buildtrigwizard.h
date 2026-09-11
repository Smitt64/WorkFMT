#ifndef BUILDTRIGWIZARD_H
#define BUILDTRIGWIZARD_H

#include <QVariant>
#include <QWizard>

class RepoPage;
class TriggerPage;
class BuildTrigWizard : public QWizard
{
    Q_OBJECT
public:
    enum
    {
        PageRepo = 0,
        PageTrigger
    };

    BuildTrigWizard(QWidget *parent = nullptr);
    virtual ~BuildTrigWizard();

    void addUserField(const QString &name, const QVariant &value);
    const QVariant &userField(const QString &name);

    RepoPage *repoPage() const;

private:
    RepoPage *m_pRepoPage;
    TriggerPage *m_pTriggerPage;

    QMap<QString, QVariant> m_Fields;
};

#endif // BUILDTRIGWIZARD_H