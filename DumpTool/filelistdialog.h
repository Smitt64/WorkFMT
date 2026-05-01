#ifndef FILELISTDIALOG_H
#define FILELISTDIALOG_H

#include <QDialog>
#include <QAuthenticator>

namespace Ui {
class FileListDialog;
}

class QFileSystemModel;
class QNetworkAccessManager;
class QNetworkReply;

class FileListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileListDialog(const QString &path, QWidget *parent = nullptr);
    ~FileListDialog();

    QString fileName() const;

    // Функции для управления режимами
    void setLocalMode();                                      // Переключиться в локальный режим
    bool setFtpMode(int port = 21, const QString &user = "anonymous",
                    const QString &password = "");            // Переключиться в FTP режим
    bool isFtpMode() const { return m_isFtpMode; }

    // Получить информацию о подключении
    QString getFtpHost() const { return m_ftpHost; }
    QString getFtpPath() const { return m_ftpCurrentPath; }

private slots:
    void onFtpListFinished();
    void onFtpListReadyRead();
    void onFtpError();
    void onRefreshFtpList();
    void onAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);

private:
    void setupLocalMode();
    void setupFtpMode(int port, const QString &user, const QString &password);
    void parseFtpPath(const QString &path);
    void parseFtpListData(const QByteArray &data);
    void updateFtpFileList();
    void clearFtpFileList();
    void showFtpAuthDialog(int &port, QString &user, QString &password);

    Ui::FileListDialog *ui;

    // Local mode
    QFileSystemModel *m_pModel;
    QString m_localPath;

    // FTP mode
    QNetworkAccessManager *m_nam;
    QNetworkReply *m_reply;
    QString m_ftpHost;
    QString m_ftpUser;
    QString m_ftpPassword;
    QString m_ftpCurrentPath;
    int m_ftpPort;
    QStringList m_ftpFileList;
    bool m_isFtpMode;
    QString m_selectedFileName;
    QByteArray m_ftpDataBuffer;
    QString m_originalPath;  // Сохраняем исходный путь из конструктора
};

#endif // FILELISTDIALOG_H
