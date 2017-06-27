#include "documentwindow.h"
#include "ui_documentwindow.h"

DocumentWindow::DocumentWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DocumentWindow)
{
    ui->setupUi(this);
}

DocumentWindow::~DocumentWindow()
{
    delete ui;
}
