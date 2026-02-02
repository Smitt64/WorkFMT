#ifndef FMTWINDOWTABINTERFACE_H
#define FMTWINDOWTABINTERFACE_H

#include <QMainWindow>

class FmtWindowTabInterface : public QMainWindow
{
    Q_OBJECT
public:
    explicit FmtWindowTabInterface(QWidget *parent = nullptr);

signals:
};

#endif // FMTWINDOWTABINTERFACE_H
