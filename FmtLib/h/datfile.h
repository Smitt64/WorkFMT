#ifndef DATFILE_H
#define DATFILE_H

#include <QObject>
#include "fmtlib_global.h"

typedef QVector<QString> DatFileRow;

class DatFile;
class DatFileHandlerBase
{
    friend class DatFile;
public:
    virtual ~DatFileHandlerBase() {}
    virtual void OnReadHeader(const QString &line) { Q_UNUSED(line) }
    virtual void OnEndReadHeader() { }
    virtual void OnReadRow(const QString &line, const DatFileRow &row) = 0;

private:
    void setDatFile(DatFile *file);

    DatFile *m_pFile;
};

class QFile;
class FmtField;
class FMTLIBSHARED_EXPORT DatFile : public QObject
{
    Q_OBJECT
public:
    DatFile(QObject *parent = nullptr);
    ~DatFile() Q_DECL_OVERRIDE;

    bool read(const QString &filename);
    QChar quote() const;

    const QList<FmtField*> &getFields() const;

private:
    void readDataLine(const QString &row);
    bool readField(const QString &row);
    QScopedPointer<QFile> m_File;
    DatFileHandlerBase *m_pHandler;
    QList<FmtField*> m_Fields;
};

#endif // DATFILE_H
