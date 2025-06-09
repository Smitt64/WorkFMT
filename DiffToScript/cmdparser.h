#ifndef CMDPARSER_H
#define CMDPARSER_H

#include <map>
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

class QApplication;
class CmdParser
{
public:
    CmdParser();
    CommandLineParseResult parse(QApplication* app);
    CommandLineParseResult parse(const QStringList &arguments);
    std::map<CmdTaskOption, QString> argumentWithValue;
    TaskOptions opts;
};

#endif // CMDPARSER_H
