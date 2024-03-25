#include "streamcontrol.h"
#include "difflogging.h"
#include <QTextCodec>

Q_LOGGING_CATEGORY(logStreamControl, "StreamControl")

QTextStream defOutput;

QMap<int, QString> typeMap = {
    {  QtDebugMsg, "debug"},
    {  QtWarningMsg, "warning"},
    {  QtCriticalMsg, "critical"},
    {  QtFatalMsg, "fatal"},
    {  QtInfoMsg, "info"},
    {  QtSystemMsg, "system"}
};


void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
//    QTextCodec *codec1 = QTextCodec::codecForName( "CP866" );
//    QByteArray text = codec1->fromUnicode( msg );

    //Эта штука не работает, если считывать из файла русские символы и выводить их через
    defOutput << "[" << context.category << "." << typeMap[type] << "] " << msg << Qt::endl;
//    if (type == QtWarningMsg)
        defOutput.flush();
}


StreamControl::StreamControl()
{
    _oldMsgHandler = qInstallMessageHandler(myMessageOutput);
}

StreamControl::~StreamControl()
{
    qInstallMessageHandler(_oldMsgHandler);
}

QIODevice* StreamControl::makeOutputDevice(QString fileName)
{
    if (fileName != "")
    {
        _outputFile.setFileName(fileName);
        _outputFile.open(QFile::WriteOnly);
        defOutput.setDevice(&_outputFile);
        qCInfo(logDiff) << "Output is " << fileName;
    }
    else
    {
        _outputFile.open(stdout, QFile::WriteOnly);
        defOutput.setDevice(&_outputFile);
        qCInfo(logDiff) << "Output is stdout";
    }
    return &_outputFile;
}

QString& StreamControl::getInput(QString fileName)
{
    _buff.clear();
    if (fileName != "")
    {
        _inputFile.setFileName(fileName);
        if (!_inputFile.open(QFile::ReadOnly))
            qCritical(logStreamControl) << "file " + fileName + " not opened";
        _buff = QTextStream(&_inputFile).readAll();
        qCInfo(logDiff) << "Input is " << fileName;
    }
    else
    {
        qCInfo(logDiff) << "Input is stdin";
        _buff = QTextStream(stdin).readAll();
    }
    return _buff;
}
