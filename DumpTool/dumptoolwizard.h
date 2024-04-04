#ifndef DUMPTOOLWIZARD_H
#define DUMPTOOLWIZARD_H

#include <QWizard>
#include <QObject>
#include <QMap>
#include <QVariant>
#include <QSettings>

class UserConnectionPage;
class SelectActionPage;
class ImpFileSelectPage;
class SummaryPage;
class ActionLogPage;
class ExpParamPage;
class ExpPgParamPage;
class PgImpParamPage;
class DumpToolWizard : public QWizard
{
    Q_OBJECT
public:
    enum PagesEnum
    {
        eSelectActionPage,
        eConnectionPage,
        eImpFileSelectPage,
        eSummaryPage,
        eActionLogPage,
        eExpParamPage,
        eExpPgParamPage,
        eImpPgParamPage
    };

    DumpToolWizard();
    virtual ~DumpToolWizard();

    void addUserField(const QString &name, const QVariant &value);
    const QVariant &userField(const QString &name);

    QString userValName(const QString &value) const;

    QSettings *settings();

private:
    UserConnectionPage *m_ConnectionPage;
    SelectActionPage *m_SelectActionPage;
    ImpFileSelectPage *m_ImpFileSelectPage;
    SummaryPage *m_pSummaryPage;
    ActionLogPage *m_pActionLogPage;
    ExpParamPage *m_pExpParamPage;
    ExpPgParamPage *m_pExpPgParamPage;
    PgImpParamPage *m_pPgImpParamPage;

    QAbstractButton *m_HelpButton;

    QMap<QString, QVariant> m_Fields;
    QSettings *m_pSettings;
};

#endif // DUMPTOOLWIZARD_H
