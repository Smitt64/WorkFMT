#ifndef HOTFIXWIZARD_H
#define HOTFIXWIZARD_H

#include <QWizard>
#include <fmtlibfactory.h>

class SelDirsPage;
class StructSettingsPage;
class ConnectionInfo;
class QSettings;
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
    HotfixWizard();
    virtual ~HotfixWizard();

    void setFileListText(const QString &text);
    QString fileListText() const;

    bool isSetContentFlag(const ContentFlag &flag);

    ConnectionInfo *connection();
    QSettings *settings();

private:
    void setContentFlag(const QString &filename);
    SelDirsPage *m_pSelDirsPage;
    StructSettingsPage *m_pStructPage;

    ConnectionInfo *pConnetion;
    QStringList m_FileList;

    QSet<ContentFlag> m_Contents;
    QScopedPointer<QSettings> m_pSettings;
};

inline uint qHash(const HotfixWizard::ContentFlag &key, uint seed)
{
    return qHash(static_cast<quint8>(key), seed);
}

#endif // HOTFIXWIZARD_H
