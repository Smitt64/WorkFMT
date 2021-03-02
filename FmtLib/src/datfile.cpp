#include "datfile.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <fmtcore.h>
#include <fmtfield.h>

void DatFileHandlerBase::setDatFile(DatFile *file)
{
    m_pFile = file;
}

DatFile::DatFile(QObject *parent):
    QObject(parent),
    m_pHandler(nullptr)
{

}

DatFile::~DatFile()
{
    qDeleteAll(m_Fields);
    m_Fields.clear();
}

bool DatFile::read(const QString &filename)
{
    bool result = true;

    m_File.reset(new QFile(filename));
    result = m_File->open(QIODevice::ReadOnly | QIODevice::Text);

    if (result)
    {
        bool isDataSection = false, isFieldsSection = false;
        QTextStream stream(m_File.get());
        stream.setCodec("IBM 866");
        while (!stream.atEnd())
        {
            QString line = stream.readLine();

            if (!isDataSection)
            {
                if (!isFieldsSection)
                {
                    if (line[0] == "(")
                    {
                        isFieldsSection = true;
                        isFieldsSection = readField(line.mid(1));
                    }
                }
                else
                {
                    isFieldsSection = readField(line);

                    if (!isFieldsSection && m_pHandler)
                        m_pHandler->OnEndReadHeader();
                }

                if (line.contains("BEGINDATA", Qt::CaseInsensitive))
                {
                    isFieldsSection = false;
                    isDataSection = true;
                }

                if (m_pHandler)
                    m_pHandler->OnReadHeader(line);
            }
            else
            {
                readDataLine(line);
            }
        }
        m_File->close();
    }

    return result;
}

QChar DatFile::quote() const
{
    return QChar('\'');
}

void DatFile::readDataLine(const QString &row)
{
    DatFileRow fields;
    readCSVRow(row, fields, quote());

    //qDebug() << fields;

    if (m_pHandler)
        m_pHandler->OnReadRow(row, fields);
}

bool DatFile::readField(const QString &row)
{
    bool islast = row.back() != ")";

    // (\w+)(\s+(\w*)\s*\(\s*(\d+)\s*\))*
    QRegExp rx("(\\w+)(\\s+(\\w*)\\s*\\(\\s*(\\d+)\\s*\\))*");

    if (rx.indexIn(row, 0) != -1)
    {
        FmtField *field = new FmtField();
        if (rx.captureCount() == 1)
        {
            if (row.contains("chr", Qt::CaseInsensitive))
                field->setType(fmtt_CHR);
            else
                field->setType(fmtt_LONG);

            field->setName(rx.cap(1));
        }
        else
        {
            QString type = rx.cap(3);

            if (type == "CHAR")
            {
                field->setType(fmtt_STRING);
                field->setSize(rx.cap(4).toInt());
            }
            else if (type == "DATE")
            {
                field->setType(fmtt_DATE);
            }

            field->setName(rx.cap(1));
        }

        //qDebug() << field->name() << rx.cap(3) << rx.cap(4);
    }

    return islast;
}

const QList<FmtField*> &DatFile::getFields() const
{
    return m_Fields;
}
