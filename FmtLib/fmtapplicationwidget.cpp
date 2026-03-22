#include "fmtapplicationwidget.h"

FmtApplicationWidget::FmtApplicationWidget(SARibbonMainWindow *parent) :
    ApplicationWidgetBase(parent)
{
    setMenuPanelColor(QColor(0x217346));

    addTab(tr("Пример"), new QWidget());
}

FmtApplicationWidget::~FmtApplicationWidget()
{

}