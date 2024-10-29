#ifndef TASKOPTIONS_H
#define TASKOPTIONS_H

#include <array>
#include <utility>

#include <QString>

enum CmdTaskOption
{
    ctoInsert,
    ctoDelete,
    ctoUpdate,
    ctoInput,
    ctoOutput,
    ctoAutoInc,
    ctoTableInfo,
    ctoIndex,
    ctoVerbose,
    ctoLog,
    ctoOracle,
    ctoPostgres,
    ctoConnectionString,
    ctoConnectionUnicode,

    toLast
};

struct TaskOption
{
    bool isSet;
    QString value;
};

// Храняться параметры вызова из cmd
typedef std::array<TaskOption, toLast> TaskOptions;


#endif // TASKOPTIONS_H
