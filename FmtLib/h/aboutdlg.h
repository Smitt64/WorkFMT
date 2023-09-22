#ifndef ABOUTDLG_H
#define ABOUTDLG_H

#include <QDialog>
#include <QLocale>

namespace Ui {
class AboutDlg;
}

class QDomElement;
class AboutTextBrowser;
class QStandardItemModel;
class AboutDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDlg(QWidget *parent = 0);
    ~AboutDlg();

private:
    void PutVersion();
    void ReadComponents();
    void ReadComponentPackage(QDomElement *docElem);
    Ui::AboutDlg *ui;
    QStandardItemModel *pComponentsModel;
    QLocale currentLocale;

    AboutTextBrowser *m_pTextBrowser;
};

#endif // ABOUTDLG_H
