#ifndef IMPEXPPARAMS_H
#define IMPEXPPARAMS_H

#include <QDialog>
#include "fmtlib_global.h"

namespace Ui {
class ImpExpParams;
}

class QSettings;
class FMTLIBSHARED_EXPORT ImpExpParams : public QDialog
{
    Q_OBJECT

public:
    explicit ImpExpParams(QWidget *parent = 0);
    ~ImpExpParams();

private slots:
    void OpenFmtXml();
    void OnAccept();

private:
    QSettings *m_pPrm;
    Ui::ImpExpParams *ui;
};

#endif // IMPEXPPARAMS_H
