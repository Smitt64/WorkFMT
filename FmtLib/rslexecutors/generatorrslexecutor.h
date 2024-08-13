#ifndef GENERATORRSLEXECUTOR_H
#define GENERATORRSLEXECUTOR_H

#include <QObject>
#include <rslexecutor.h>

class FmtTable;
class GeneratorRslExecutor : public RslExecutor
{
    Q_OBJECT
public:
    GeneratorRslExecutor(const QString &filename, FmtTable *pTable);

    void execute();
    const QByteArray &data() const;

protected:
    virtual void PlayRepProc() Q_DECL_OVERRIDE;

private:
    QByteArray m_Data;
    QString m_Filename;
    FmtTable *m_pTable;
};

#endif // GENERATORRSLEXECUTOR_H
