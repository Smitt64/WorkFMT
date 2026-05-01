#include "filelistdialog.h"
#include "ui_filelistdialog.h"
#include <QFileSystemModel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include <QRegExp>
#include <QStringListModel>
#include <QAuthenticator>

FileListDialog::FileListDialog(const QString &path, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileListDialog),
    m_pModel(nullptr),
    m_nam(nullptr),
    m_reply(nullptr),
    m_isFtpMode(false),
    m_originalPath(path)
{
    ui->setupUi(this);

    // Парсим путь для получения хоста и папки (но не подключаемся)
    parseFtpPath(path);

    // По умолчанию работаем в локальном режиме
    setupLocalMode();
}

FileListDialog::~FileListDialog()
{
    if (m_reply) {
        m_reply->abort();
        m_reply->deleteLater();
    }
    if (m_nam) {
        delete m_nam;
    }
    delete ui;
}

void FileListDialog::parseFtpPath(const QString &path)
{
    m_ftpHost.clear();
    m_ftpCurrentPath.clear();
    m_localPath = path;

    // Путь вида "\\tst-9718.com\dump\impdp"
    QStringList spl = path.split("\\");

    if (spl.size() > 3)
    {
        m_ftpHost = spl[2];
        spl.pop_front();
        spl.pop_front();
        spl.pop_front();

        m_ftpCurrentPath = "/" + spl.join("/");
    }
}

void FileListDialog::setupLocalMode()
{
    // Очищаем FTP данные если были
    clearFtpFileList();

    if (m_nam) {
        delete m_nam;
        m_nam = nullptr;
    }

    if (m_reply) {
        m_reply->abort();
        m_reply->deleteLater();
        m_reply = nullptr;
    }

    m_isFtpMode = false;

    // Создаем или пересоздаем локальную модель
    if (m_pModel) {
        delete m_pModel;
    }

    m_pModel = new QFileSystemModel(this);
    m_pModel->setNameFilters(QStringList() << "*.dmp");
    m_pModel->setFilter(QDir::Files);
    m_pModel->setNameFilterDisables(false);

    // Используем локальный путь
    QString localPath = m_localPath;
    QModelIndex root = m_pModel->setRootPath(localPath);
    ui->listView->setModel(m_pModel);
    ui->listView->setRootIndex(root);
    ui->dirLabel->setText(localPath);

    // Переподключаем сигналы
    disconnect(ui->listView, &QListView::doubleClicked, nullptr, nullptr);
    disconnect(ui->listView, &QListView::clicked, nullptr, nullptr);

    connect(ui->listView, &QListView::doubleClicked, [=](const QModelIndex &index)
    {
        if (index.isValid())
            accept();
    });

    connect(ui->listView, &QListView::clicked, [=](const QModelIndex &index)
    {
        if (index.isValid())
            ui->lineEdit->setText(m_pModel->fileName(index));
    });
}

void FileListDialog::setLocalMode()
{
    setupLocalMode();
}

bool FileListDialog::setFtpMode(int port, const QString &user, const QString &password)
{
    // Проверяем, что хост был распарсен из пути
    if (m_ftpHost.isEmpty()) {
        QMessageBox::warning(this, "Ошибка",
                            "Путь не содержит информацию о FTP сервере.\n"
                            "Используйте формат: \\\\сервер\\папка");
        return false;
    }

    m_ftpPort = port;
    m_ftpUser = user;
    m_ftpPassword = password;

    setupFtpMode(port, user, password);
    return true;
}

void FileListDialog::showFtpAuthDialog(int &port, QString &user, QString &password)
{
    bool ok;

    // Запрашиваем порт
    port = QInputDialog::getInt(this, "FTP Порт",
                                QString("Введите порт для %1:").arg(m_ftpHost),
                                21, 1, 65535, 1, &ok);

    if (!ok) {
        port = 21;
    }

    // Запрашиваем пользователя
    user = QInputDialog::getText(this, "FTP Авторизация",
                                 QString("Введите имя пользователя для %1:").arg(m_ftpHost),
                                 QLineEdit::Normal, "anonymous", &ok);

    if (ok && !user.isEmpty()) {
        password = QInputDialog::getText(this, "FTP Авторизация",
                                        QString("Введите пароль для %1:").arg(user),
                                        QLineEdit::Password, "", &ok);
    }

    if (!ok) {
        user.clear();
        password.clear();
    }
}

void FileListDialog::setupFtpMode(int port, const QString &user, const QString &password)
{
    // Очищаем локальную модель если была
    if (m_pModel) {
        delete m_pModel;
        m_pModel = nullptr;
    }

    m_isFtpMode = true;
    m_ftpPort = port;
    m_ftpUser = user;
    m_ftpPassword = password;

    if (m_nam) {
        delete m_nam;
    }
    m_nam = new QNetworkAccessManager(this);

    // Формируем отображаемый путь
    QString displayPath = QString("\\\\%1\\%2").arg(m_ftpHost).arg(m_ftpCurrentPath.mid(1).replace('/', '\\'));
    ui->dirLabel->setText(displayPath);

    // Если не переданы учетные данные, запрашиваем их
    /*if (m_ftpUser.isEmpty()) {
        int inputPort;
        QString inputUser, inputPassword;
        showFtpAuthDialog(inputPort, inputUser, inputPassword);

        if (inputUser.isEmpty()) {
            reject();
            return;
        }

        m_ftpPort = inputPort;
        m_ftpUser = inputUser;
        m_ftpPassword = inputPassword;
    }*/

    // Формируем URL с логином и паролем в строке
    /*QString urlString = QString("ftp://%1:%2@%3:%4%5")
        .arg(QString::fromLocal8Bit(QUrl::toPercentEncoding(m_ftpUser)))
        .arg(QString::fromLocal8Bit(QUrl::toPercentEncoding(m_ftpPassword)))
        .arg(m_ftpHost)
        .arg(m_ftpPort)
        .arg(m_ftpCurrentPath);*/
    QString urlString = QString("ftp://%1:%2%3")
            .arg(m_ftpHost)
            .arg(m_ftpPort)
            .arg(m_ftpCurrentPath);

    QUrl url(urlString);

    qDebug() << "Connecting to:" << url.toString(QUrl::RemoveUserInfo); // Без пароля для лога
    qDebug() << "Username:" << m_ftpUser;
    qDebug() << "Path:" << m_ftpCurrentPath;

    QNetworkRequest request(url);

    // Устанавливаем таймаут
    request.setTransferTimeout(30000);

    // Очищаем старые данные
    clearFtpFileList();
    m_ftpDataBuffer.clear();

    // Отправляем запрос на получение списка файлов
    if (m_reply) {
        m_reply->abort();
        m_reply->deleteLater();
    }

    m_reply = m_nam->get(request);

    // Подключаем сигналы
    connect(m_reply, &QNetworkReply::finished, this, &FileListDialog::onFtpListFinished);
    connect(m_reply, &QNetworkReply::readyRead, this, &FileListDialog::onFtpListReadyRead);
    connect(m_reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            this, &FileListDialog::onFtpError);

    // Правильное подключение authenticationRequired - сигнал QNetworkAccessManager, не QNetworkReply
    connect(m_nam, &QNetworkAccessManager::authenticationRequired,
            this, &FileListDialog::onAuthenticationRequired);

    // Настраиваем обработку выбора в списке
    disconnect(ui->listView, &QListView::doubleClicked, nullptr, nullptr);
    disconnect(ui->listView, &QListView::clicked, nullptr, nullptr);

    connect(ui->listView, &QListView::doubleClicked, [=](const QModelIndex &index)
    {
        if (index.isValid())
            accept();
    });

    connect(ui->listView, &QListView::clicked, [=](const QModelIndex &index)
    {
        if (index.isValid()) {
            m_selectedFileName = ui->listView->model()->data(index).toString();
            ui->lineEdit->setText(m_selectedFileName);
        }
    });

    ui->dirLabel->setText(displayPath + " (подключение...)");
}

void FileListDialog::onRefreshFtpList()
{
    if (m_isFtpMode && m_nam) {
        setupFtpMode(m_ftpPort, m_ftpUser, m_ftpPassword);
    }
}

void FileListDialog::onFtpListFinished()
{
    if (!m_reply) return;

    if (m_reply->error() != QNetworkReply::NoError) {
        onFtpError();
        return;
    }

    // Парсим полученные данные
    parseFtpListData(m_ftpDataBuffer);
    updateFtpFileList();

    m_reply->deleteLater();
    m_reply = nullptr;
}

void FileListDialog::onFtpListReadyRead()
{
    if (m_reply) {
        m_ftpDataBuffer.append(m_reply->readAll());
    }
}

void FileListDialog::onFtpError()
{
    if (!m_reply) return;

    QString errorMsg = m_reply->errorString();
    QMessageBox::critical(this, "Ошибка FTP",
                         QString("Ошибка подключения к %1:%2\n%3")
                         .arg(m_ftpHost).arg(m_ftpPort).arg(errorMsg));

    // Возвращаемся в локальный режим при ошибке
    setupLocalMode();
}

void FileListDialog::onAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    qDebug() << "Authentication required for:" << reply->url().toString();

    if (!m_ftpUser.isEmpty()) {
        authenticator->setUser(m_ftpUser);
        authenticator->setPassword(m_ftpPassword);
        qDebug() << "Setting credentials:" << m_ftpUser;
    } else {
        // Запрашиваем учетные данные
        bool ok;
        QString user = QInputDialog::getText(this, "FTP Авторизация",
                                            QString("Введите имя пользователя для %1:").arg(m_ftpHost),
                                            QLineEdit::Normal, "anonymous", &ok);
        if (ok && !user.isEmpty()) {
            QString password = QInputDialog::getText(this, "FTP Авторизация",
                                                    QString("Введите пароль для %1:").arg(user),
                                                    QLineEdit::Password, "", &ok);
            if (ok) {
                authenticator->setUser(user);
                authenticator->setPassword(password);
                m_ftpUser = user;
                m_ftpPassword = password;
            }
        }
    }
}

void FileListDialog::parseFtpListData(const QByteArray &data)
{
    m_ftpFileList.clear();
    qDebug() << data;
    QStringList lines = QString(data).split('\n', Qt::SkipEmptyParts);

    // Регулярные выражения для парсинга LIST вывода
    QRegExp unixRegex("^([-dl])([rwx-]{9})\\s+\\d+\\s+\\S+\\s+\\S+\\s+(\\d+)\\s+(\\w+\\s+\\d+\\s+\\d+:?\\d*)\\s+(.+)$");
    QRegExp dosRegex("^(\\d{2}-\\d{2}-\\d{2})\\s+(\\d{2}:\\d{2}[AP]M)\\s+(<DIR>|\\d+)\\s+(.+)$");

    for (const QString &line : lines) {
        QString trimmed = line.trimmed();
        if (trimmed.isEmpty()) continue;

        QString fileName;

        // Пробуем Unix формат
        if (unixRegex.indexIn(trimmed) != -1) {
            fileName = unixRegex.cap(6).trimmed();
        }
        // Пробуем DOS формат
        else if (dosRegex.indexIn(trimmed) != -1) {
            fileName = dosRegex.cap(4).trimmed();
            if (fileName == "." || fileName == "..") continue;
        }
        // Если не распарсилось, пробуем взять последнее слово
        else {
            QStringList parts = trimmed.split(' ', Qt::SkipEmptyParts);
            if (!parts.isEmpty()) {
                fileName = parts.last();
            }
        }

        // Фильтруем только файлы .dmp
        if (!fileName.isEmpty() && fileName.endsWith(".dmp", Qt::CaseInsensitive)) {
            m_ftpFileList.append(fileName);
        }
    }

    qDebug() << "Found" << m_ftpFileList.size() << ".dmp files";
}

void FileListDialog::updateFtpFileList()
{
    // Сортируем список
    m_ftpFileList.sort();

    // Создаем модель для отображения списка файлов
    QStringListModel *model = new QStringListModel(this);
    model->setStringList(m_ftpFileList);
    ui->listView->setModel(model);

    // Обновляем отображение пути
    QString displayPath = QString("\\\\%1\\%2").arg(m_ftpHost).arg(m_ftpCurrentPath.mid(1).replace('/', '\\'));

    if (m_ftpFileList.isEmpty()) {
        ui->dirLabel->setText(displayPath + " (нет файлов .dmp)");
    } else {
        ui->dirLabel->setText(QString("%1 (файлов: %2)").arg(displayPath).arg(m_ftpFileList.size()));
    }
}

void FileListDialog::clearFtpFileList()
{
    m_ftpFileList.clear();
    m_selectedFileName.clear();
    ui->lineEdit->clear();
}

QString FileListDialog::fileName() const
{
    if (m_isFtpMode) {
        return m_selectedFileName;
    } else if (m_pModel) {
        return m_pModel->fileName(ui->listView->currentIndex());
    }
    return QString();
}
