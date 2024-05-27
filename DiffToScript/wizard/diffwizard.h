#ifndef DIFFWIZARD_H
#define DIFFWIZARD_H

#include <QVariant>
#include <QWizard>

class ActionPage;
class ConnactionPage;
class ScriptsPage;
class DiffWizard : public QWizard
{
    Q_OBJECT
public:
    enum
    {
        PageAction = 0,
        PageConnection,
    };
    DiffWizard(QWidget *parent = nullptr);
    virtual ~DiffWizard();

    void addUserField(const QString &name, const QVariant &value);
    const QVariant &userField(const QString &name);

    QString userValName(const QString &value) const;

private:
    ActionPage *m_pActionPage;
    ConnactionPage *m_pConnactionPage;
    ScriptsPage *m_pScriptsPage;

    QMap<QString, QVariant> m_Fields;
};

#endif // DIFFWIZARD_H
