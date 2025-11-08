#ifndef REGPARMWIZARD_H
#define REGPARMWIZARD_H

#include <QWizard>
#include <QMap>

class QMenu;
class ActionPage;
class ConnactionPage;
class RegParmWizard : public QWizard
{
    Q_OBJECT

public:
    enum PageId {
        PageAction = 0,
        PageConnection,
        PageScripts
    };

    enum ActionType {
        ActionGenerateScript = 0,
        ActionExecuteScript,
        ActionCompareVersions
    };

    explicit RegParmWizard(QWidget *parent = nullptr);
    virtual ~RegParmWizard();

    void addUserField(const QString &name, const QVariant &value);
    const QVariant &userField(const QString &name);
    QString userValName(const QVariant &value) const;

    ActionType selectedAction() const;

private slots:
    void onSettingsClicked();
    void onRestartClicked();

private:
    void setupUi();
    void setupConnections();

    ActionPage *m_pActionPage;
    ConnactionPage *m_pConnectionPage;

    QMap<QString, QVariant> m_Fields;
    QMenu *m_pHelpMenu;
};

#endif // REGPARMWIZARD_H
