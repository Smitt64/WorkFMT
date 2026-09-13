#ifndef REGPARMOPTIONSDLG_H
#define REGPARMOPTIONSDLG_H

#include <optionsdlg/optionsdlg.h>
#include <QSharedPointer>

class QSettings;

class RegparmOptionsDlg : public OptionsDlg
{
    Q_OBJECT
public:
    RegparmOptionsDlg(QSettings *settings, QWidget *parent = nullptr);
    virtual ~RegparmOptionsDlg();
};

QSharedPointer<QSettings> regparmGetSettings();

#endif // REGPARMOPTIONSDLG_H
