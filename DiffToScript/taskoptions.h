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
    ctoDiffInfoMode,

    toLast
};

struct TaskOption
{
    bool isSet;
    QString value;
};

// Храняться параметры вызова из cmd
using TaskOptions = std::array<TaskOption, toLast>;


#endif // TASKOPTIONS_H
