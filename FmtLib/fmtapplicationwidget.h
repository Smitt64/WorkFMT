#ifndef FMTAPPLICATIONWIDGET_H
#define FMTAPPLICATIONWIDGET_H

#include <ribbon/applicationwidgetbase.h>

class FmtApplicationWidget : public ApplicationWidgetBase
{
    Q_OBJECT
public:
    FmtApplicationWidget(SARibbonMainWindow *parent = nullptr);
    virtual ~FmtApplicationWidget();
};

#endif // FMTAPPLICATIONWIDGET_H
