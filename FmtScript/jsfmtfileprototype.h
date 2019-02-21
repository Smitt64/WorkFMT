#ifndef JSFMTFILEPROTOTYPE_H
#define JSFMTFILEPROTOTYPE_H

#include <QtScript>
#include <QObject>

class FmtFile;
class JsFmtFilePrototype : public QObject, public QScriptable
{
    Q_OBJECT
public:
    JsFmtFilePrototype(QObject *parent = nullptr);
    virtual ~JsFmtFilePrototype();

private:
    FmtFile *thisByteArray() const;
};

#endif // JSFMTFILEPROTOTYPE_H
