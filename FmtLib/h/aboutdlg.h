#ifndef ABOUTDLG_H
#define ABOUTDLG_H

#include <QDialog>
#include <QLocale>
#include <QFileInfo>
#include <QMap>

namespace Ui {
class AboutDlg;
}

class QDomElement;
class QStandardItemModel;
class QWebEngineView;
class AboutDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDlg(QWidget *parent = 0);
    ~AboutDlg();

private slots:
    void urlChanged(const QUrl &url);

private:
    void PutVersion();
    void ReadComponents();
    void ReadComponentPackage(QDomElement *docElem);
    void ReadVersionsTitles();
    void RenderHtmlForProject(const QString &project);
    Ui::AboutDlg *ui;
    QStandardItemModel *pComponentsModel;
    QLocale currentLocale;
    QWebEngineView *m_WebView;

    QMap<QString,QFileInfo> m_Projects;
};

#endif // ABOUTDLG_H
