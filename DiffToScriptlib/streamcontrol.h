#ifndef STREAMCONTROL_H
#define STREAMCONTROL_H

#include <QTextStream>
#include <QFile>
#include <qloggingcategory.h>
#include "DiffToScriptl_global.h"

DIFFTOSCRIPTL_EXPORT Q_DECLARE_LOGGING_CATEGORY(logStreamControl)

class DIFFTOSCRIPTL_EXPORT StreamControl
{
public:
    StreamControl();
    ~StreamControl();
    QIODevice* makeOutputDevice(QString fileName);
    QTextStream *getInput(QString fileName);
private:
    QFile _inputFile;
    QFile _outputFile;
    QString _buff;
    QtMessageHandler _oldMsgHandler;
    QScopedPointer<QTextStream> _stream;
};

#endif // STREAMCONTROL_H
