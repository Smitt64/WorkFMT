#include "taskoptionscontainer.h"

TaskOptionsContainer::TaskOptionsContainer(const TaskOptions &options, QObject *parent) :
    QObject(parent),
    m_options(options)
{

}

TaskOptionsContainer::~TaskOptionsContainer()
{

}

bool TaskOptionsContainer::isSetInsert() const { return m_options[ctoInsert].isSet; }
bool TaskOptionsContainer::isSetDelete() const { return m_options[ctoDelete].isSet; }
bool TaskOptionsContainer::isSetUpdate() const { return m_options[ctoUpdate].isSet; }
bool TaskOptionsContainer::isSetInput() const { return m_options[ctoInput].isSet; }
bool TaskOptionsContainer::isSetOutput() const { return m_options[ctoOutput].isSet; }
bool TaskOptionsContainer::isSetAutoInc() const { return m_options[ctoAutoInc].isSet; }
bool TaskOptionsContainer::isSetTableInfo() const { return m_options[ctoTableInfo].isSet; }
bool TaskOptionsContainer::isSetIndex() const { return m_options[ctoIndex].isSet; }
bool TaskOptionsContainer::isSetVerbose() const { return m_options[ctoVerbose].isSet; }
bool TaskOptionsContainer::isSetLog() const { return m_options[ctoLog].isSet; }
bool TaskOptionsContainer::isSetOracle() const { return m_options[ctoOracle].isSet; }
bool TaskOptionsContainer::isSetPostgres() const { return m_options[ctoPostgres].isSet; }
bool TaskOptionsContainer::isSetConnectionString() const { return m_options[ctoConnectionString].isSet; }
bool TaskOptionsContainer::isSetConnectionUnicode() const { return m_options[ctoConnectionUnicode].isSet; }
bool TaskOptionsContainer::isSetDiffInfoMode() const { return m_options[ctoDiffInfoMode].isSet; }
bool TaskOptionsContainer::isSetXml() const { return m_options[ctoXml].isSet; }
bool TaskOptionsContainer::isSetDatFile() const { return m_options[ctoDatFile].isSet; }
bool TaskOptionsContainer::isSetNormalFileList() const { return m_options[ctoNormalFileList].isSet; }
bool TaskOptionsContainer::isSetUpdateTable() const { return m_options[ctoUpdateTable].isSet; }

QString TaskOptionsContainer::getInsert() const { return m_options[ctoInsert].value; }
QString TaskOptionsContainer::getDelete() const { return m_options[ctoDelete].value; }
QString TaskOptionsContainer::getUpdate() const { return m_options[ctoUpdate].value; }
QString TaskOptionsContainer::getInput() const { return m_options[ctoInput].value; }
QString TaskOptionsContainer::getOutput() const { return m_options[ctoOutput].value; }
QString TaskOptionsContainer::getAutoInc() const { return m_options[ctoAutoInc].value; }
QString TaskOptionsContainer::getTableInfo() const { return m_options[ctoTableInfo].value; }
QString TaskOptionsContainer::getIndex() const { return m_options[ctoIndex].value; }
QString TaskOptionsContainer::getVerbose() const { return m_options[ctoVerbose].value; }
QString TaskOptionsContainer::getLog() const { return m_options[ctoLog].value; }
QString TaskOptionsContainer::getOracle() const { return m_options[ctoOracle].value; }
QString TaskOptionsContainer::getPostgres() const { return m_options[ctoPostgres].value; }
QString TaskOptionsContainer::getConnectionString() const { return m_options[ctoConnectionString].value; }
QString TaskOptionsContainer::getConnectionUnicode() const { return m_options[ctoConnectionUnicode].value; }
QString TaskOptionsContainer::getDiffInfoMode() const { return m_options[ctoDiffInfoMode].value; }
QString TaskOptionsContainer::getXml() const { return m_options[ctoXml].value; }
QString TaskOptionsContainer::getDatFile() const { return m_options[ctoDatFile].value; }
QString TaskOptionsContainer::getNormalFileList() const { return m_options[ctoNormalFileList].value; }
QString TaskOptionsContainer::getUpdateTable() const { return m_options[ctoUpdateTable].value; }
