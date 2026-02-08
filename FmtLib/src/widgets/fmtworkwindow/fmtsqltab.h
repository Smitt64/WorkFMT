#ifndef FMTSQLTAB_H
#define FMTSQLTAB_H

#include "fmtcodetabbase.h"

class FmtSqlTab : public FmtCodeTabBase
{
    Q_OBJECT
public:
    FmtSqlTab(QWidget *parent = nullptr);
    virtual ~FmtSqlTab();
};

#endif // FMTSQLTAB_H
