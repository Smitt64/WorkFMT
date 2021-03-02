#ifndef HOTFIXWIZARD_H
#define HOTFIXWIZARD_H

#include <QWizard>
#include <fmtlibfactory.h>
#include "fileeditorpageinterface.h"

class SelDirsPage;
class FileListPage;
class FileTypesPage;
class ConnectionInfo;
class HotfixWizard : public QWizard
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
    ~HotfixWizard() Q_DECL_FINAL;

    void setFileListText(const QString &text);
    QString fileListText() const;

    bool isSetContentFlag(const ContentFlag &flag);

    ConnectionInfo *connection();

private:
    void setContentFlag(const QString &filename);
    SelDirsPage *m_pSelDirsPage;
    FileListPage *m_FileListPage;
    FileTypesPage *m_FileTypesPage;

    ConnectionInfo *pConnetion;
    FmtLibFactory<FileEditorPageInterface, QString> m_PageFactory;
    QStringList m_FileList;

    QSet<ContentFlag> m_Contents;
};

inline uint qHash(const HotfixWizard::ContentFlag &key, uint seed)
{
    return qHash(static_cast<quint8>(key), seed);
}

#endif // HOTFIXWIZARD_H
