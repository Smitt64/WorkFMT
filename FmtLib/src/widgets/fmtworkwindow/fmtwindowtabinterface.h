#ifndef FMTWINDOWTABINTERFACE_H
#define FMTWINDOWTABINTERFACE_H

#include <QWidget>

class FmtWindowTabInterface : public QWidget
{
    Q_OBJECT
public:
    explicit FmtWindowTabInterface(QWidget *parent = nullptr);

signals:
};

#endif // FMTWINDOWTABINTERFACE_H
