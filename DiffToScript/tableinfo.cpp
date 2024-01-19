#include "tableinfo.h"
#include "oracleauthdlg.h"
//#include "fmttable.h"
#include "connectioninfo.h"

TableInfo::TableInfo(QObject *parent) : QObject(parent)
{

}

void TableInfo::load(QString tableName)
{
    ConnectionInfo* pTmpInfo = new ConnectionInfo();
    QString user = "DEV_SPIRIN_RSB3184";
    QString pass = "DEV_SPIRIN_RSB3184";
    QString dsn = "RSB19C1";
    QString optionStr;




//    bool hr = pTmpInfo->open("qrsd", user, pass, dsn, optionStr);
//    bool hr = OracleAuthDlg::tryConnect(pTmpInfo, user, pass, dsn, optionStr, nullptr);

//    FmtTable fmt(pTmpInfo);
//    bool b = fmt.load("dopusymb_dbt");

}
