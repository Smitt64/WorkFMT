#include "regparmoptionsdlg.h"
#include <QSettings>
#include <QDir>
#include <QApplication>

RegparmOptionsDlg::RegparmOptionsDlg(QSettings *settings, QWidget *parent) :
    OptionsDlg(settings, parent)
{
    setDefaultStyle("windowsvista");

    addRslPage();
    addLogPage("regparmtool");
    addUpdatePage();
}

RegparmOptionsDlg::~RegparmOptionsDlg()
{

}

QSharedPointer<QSettings> regparmGetSettings()
{
    QDir settingsDir = QDir(qApp->applicationDirPath());

    QSharedPointer<QSettings> pSettings(new QSettings(settingsDir.absoluteFilePath("regparmtool.ini"), QSettings::IniFormat));
    return pSettings;
}
