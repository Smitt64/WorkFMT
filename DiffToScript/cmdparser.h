#ifndef CMDPARSER_H
#define CMDPARSER_H
#include <map>

#include <QApplication>

#include "taskoptions.h"

struct CommandLineParseResult
{
    enum class Status {
        Ok,
        Error,
        VersionRequested,
        HelpRequested
    };
    Status statusCode = Status::Ok;
    QString errorString;
};

class CmdParser
{
public:
    CmdParser(QApplication* app);
    CommandLineParseResult parse();
    std::map<CmdTaskOption, QString> argumentWithValue;
    TaskOptions opts;
private:
    QApplication* _app;
};

#endif // CMDPARSER_H
