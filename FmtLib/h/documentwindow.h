#ifndef DOCUMENTWINDOW_H
#define DOCUMENTWINDOW_H

#include <QWidget>
#include "fmtlib_global.h"

namespace Ui {
class DocumentWindow;
}

class FMTLIBSHARED_EXPORT DocumentWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DocumentWindow(QWidget *parent = 0);
    ~DocumentWindow();

private:
    Ui::DocumentWindow *ui;
};

#endif // DOCUMENTWINDOW_H
