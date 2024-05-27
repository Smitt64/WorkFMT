#ifndef SVNTOOL_H
#define SVNTOOL_H

#include <QObject>

class SvnTool : public QObject
{
    Q_OBJECT
public:
    explicit SvnTool(QObject *parent = nullptr);
    virtual ~SvnTool();

signals:

};

#endif // SVNTOOL_H
