#ifndef DIFFTOSCRIPTTEST_H
#define DIFFTOSCRIPTTEST_H

#include <QObject>

class DiffToScriptTest : public QObject
{
    Q_OBJECT
public:
    explicit DiffToScriptTest(QObject *parent = nullptr);
signals:
private slots:
    void initTestCase();
    void cleanupTestCase();

    void caseLinesType();
    void caseParseStringField();
    void caseJsonLoad();
    void caseDoubleInsert();
};

#endif // DIFFTOSCRIPTTEST_H
