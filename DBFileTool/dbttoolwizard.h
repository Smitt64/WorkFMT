#ifndef DBTTOOLWIZARD_H
#define DBTTOOLWIZARD_H

#include <QWizard>
#include <QMap>
#include <QTranslator>

class SelectActionPage;
class OraConnectionPage;
class SelectTablesPage;
class ExportPage;
class DbtToolWizard : public QWizard
{
    Q_OBJECT
public:
    enum PagesEnum
    {
        eSelectActionPage,
        eConnectionPage,
        eSelectTablesPage,
        eExportPage,
        /*eImpFileSelectPage,
        eSummaryPage,
        eActionLogPage,
        eExpParamPage,
        eExpPgParamPage,
        eImpPgParamPage*/
    };
    DbtToolWizard();
    virtual ~DbtToolWizard();

    void addUserField(const QString &name, const QVariant &value);
    const QVariant &userField(const QString &name);

private:
    SelectActionPage *m_pActionPage;
    OraConnectionPage *m_pOraConnectionPage;
    SelectTablesPage *m_pSelectTablesPage;
    ExportPage *m_pExportPage;

    QMap<QString, QVariant> m_Fields;
    QTranslator qt_translator;
};

#endif // DBTTOOLWIZARD_H
