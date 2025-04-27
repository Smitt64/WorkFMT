#ifndef TASKOPTIONSCONTAINER_H
#define TASKOPTIONSCONTAINER_H

#include <QObject>
#include "taskoptions.h"

class TaskOptionsContainer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isSetInsert READ isSetInsert CONSTANT)
    Q_PROPERTY(bool isSetDelete READ isSetDelete CONSTANT)
    Q_PROPERTY(bool isSetUpdate READ isSetUpdate CONSTANT)
    Q_PROPERTY(bool isSetInput READ isSetInput CONSTANT)
    Q_PROPERTY(bool isSetOutput READ isSetOutput CONSTANT)
    Q_PROPERTY(bool isSetAutoInc READ isSetAutoInc CONSTANT)
    Q_PROPERTY(bool isSetTableInfo READ isSetTableInfo CONSTANT)
    Q_PROPERTY(bool isSetIndex READ isSetIndex CONSTANT)
    Q_PROPERTY(bool isSetVerbose READ isSetVerbose CONSTANT)
    Q_PROPERTY(bool isSetLog READ isSetLog CONSTANT)
    Q_PROPERTY(bool isSetOracle READ isSetOracle CONSTANT)
    Q_PROPERTY(bool isSetPostgres READ isSetPostgres CONSTANT)
    Q_PROPERTY(bool isSetConnectionString READ isSetConnectionString CONSTANT)
    Q_PROPERTY(bool isSetConnectionUnicode READ isSetConnectionUnicode CONSTANT)
    Q_PROPERTY(bool isSetDiffInfoMode READ isSetDiffInfoMode CONSTANT)
    Q_PROPERTY(bool isSetXml READ isSetXml CONSTANT)
    Q_PROPERTY(bool isSetDatFile READ isSetDatFile CONSTANT)
    Q_PROPERTY(bool isSetNormalFileList READ isSetNormalFileList CONSTANT)
    Q_PROPERTY(bool isSetUpdateTable READ isSetUpdateTable CONSTANT)

    Q_PROPERTY(QString Insert READ getInsert CONSTANT)
    Q_PROPERTY(QString Delete READ getDelete CONSTANT)
    Q_PROPERTY(QString Update READ getUpdate CONSTANT)
    Q_PROPERTY(QString Input READ getInput CONSTANT)
    Q_PROPERTY(QString Output READ getOutput CONSTANT)
    Q_PROPERTY(QString AutoInc READ getAutoInc CONSTANT)
    Q_PROPERTY(QString TableInfo READ getTableInfo CONSTANT)
    Q_PROPERTY(QString Index READ getIndex CONSTANT)
    Q_PROPERTY(QString Verbose READ getVerbose CONSTANT)
    Q_PROPERTY(QString Log READ getLog CONSTANT)
    Q_PROPERTY(QString Oracle READ getOracle CONSTANT)
    Q_PROPERTY(QString Postgres READ getPostgres CONSTANT)
    Q_PROPERTY(QString ConnectionString READ getConnectionString CONSTANT)
    Q_PROPERTY(QString ConnectionUnicode READ getConnectionUnicode CONSTANT)
    Q_PROPERTY(QString DiffInfoMode READ getDiffInfoMode CONSTANT)
    Q_PROPERTY(QString Xml READ getXml CONSTANT)
    Q_PROPERTY(QString DatFile READ getDatFile CONSTANT)
    Q_PROPERTY(QString NormalFileList READ getNormalFileList CONSTANT)
    Q_PROPERTY(QString UpdateTable READ getUpdateTable CONSTANT)
public:
    TaskOptionsContainer(const TaskOptions &options, QObject *parent = nullptr);
    virtual ~TaskOptionsContainer();

    bool isSetInsert() const;
    bool isSetDelete() const;
    bool isSetUpdate() const;
    bool isSetInput() const;
    bool isSetOutput() const;
    bool isSetAutoInc() const;
    bool isSetTableInfo() const;
    bool isSetIndex() const;
    bool isSetVerbose() const;
    bool isSetLog() const;
    bool isSetOracle() const;
    bool isSetPostgres() const;
    bool isSetConnectionString() const;
    bool isSetConnectionUnicode() const;
    bool isSetDiffInfoMode() const;
    bool isSetXml() const;
    bool isSetDatFile() const;
    bool isSetNormalFileList() const;
    bool isSetUpdateTable() const;

    QString getInsert() const;
    QString getDelete() const;
    QString getUpdate() const;
    QString getInput() const;
    QString getOutput() const;
    QString getAutoInc() const;
    QString getTableInfo() const;
    QString getIndex() const;
    QString getVerbose() const;
    QString getLog() const;
    QString getOracle() const;
    QString getPostgres() const;
    QString getConnectionString() const;
    QString getConnectionUnicode() const;
    QString getDiffInfoMode() const;
    QString getXml() const;
    QString getDatFile() const;
    QString getNormalFileList() const;
    QString getUpdateTable() const;

private:
    TaskOptions m_options;
};

#endif // TASKOPTIONSCONTAINER_H
