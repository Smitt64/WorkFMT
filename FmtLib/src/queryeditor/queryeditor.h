#ifndef QUERYEDITOR_H
#define QUERYEDITOR_H

#include "mdisubinterface.h"

class QueryEditor : public MdiSubInterface
{
public:
    QueryEditor(QWidget *parent = Q_NULLPTR);

    QString makeWindowTitle() Q_DECL_OVERRIDE;
};

#endif // QUERYEDITOR_H
