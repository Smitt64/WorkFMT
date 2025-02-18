#ifndef HOTFIXWIZARD_H
#define HOTFIXWIZARD_H

#include <QWizard>
#include <fmtlibfactory.h>

class SelDirsPage;
class StructSettingsPage;
class GeneratorPage;
class ConnectionInfo;
class QSettings;
class HotfixContentModel;
class ProjectsWizardPage;
class ConnactionPage;
class HotfixWizard Q_DECL_FINAL : public QWizard
{
    Q_OBJECT
public:
    enum class ContentFlag : quint8
    {
        ContentSource,
        ContentLbr,
        ContentDatFile,
        ContentMac,
        ContentFmt,
        ContentCreateTables,
    };

    enum Pages
    {
        PageSelDirs = 0,
        PageConnaction,
        PageStructSettings,
        PageProjectsWizard,
        PageGenerate
    };
    HotfixWizard();
    virtual ~HotfixWizard();

    void setFileListText(const QString &text);
    QString fileListText() const;

    bool isSetContentFlag(const ContentFlag &flag);

    ConnectionInfo *connection();
    QSettings *settings();

    HotfixContentModel *structModel();

private:
    void setContentFlag(const QString &filename);
    SelDirsPage *m_pSelDirsPage;
    StructSettingsPage *m_pStructPage;
    ProjectsWizardPage *m_pProjects;
    GeneratorPage *m_pGeneratorPage;
    ConnactionPage *m_pConnactionPage;

    ConnectionInfo *pConnetion;
    QStringList m_FileList;

    QSet<ContentFlag> m_Contents;
    QScopedPointer<QSettings> m_pSettings;

    QScopedPointer<HotfixContentModel> m_pStrucModel;
};

inline uint qHash(const HotfixWizard::ContentFlag &key, uint seed)
{
    return qHash(static_cast<quint8>(key), seed);
}

#endif // HOTFIXWIZARD_H
