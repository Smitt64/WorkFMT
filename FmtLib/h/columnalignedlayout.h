#ifndef COLUMNALIGNEDLAYOUT_H
#define COLUMNALIGNEDLAYOUT_H

#include <QHBoxLayout>
#include "fmtlib_global.h"

class QHeaderView;
class FMTLIBSHARED_EXPORT ColumnAlignedLayout : public QHBoxLayout
{
    Q_OBJECT
public:
    ColumnAlignedLayout(QWidget *parent = Q_NULLPTR);
    virtual ~ColumnAlignedLayout();

    void setTableColumnsToTrack(QHeaderView *view);

private:
    void setGeometry(const QRect &r) Q_DECL_OVERRIDE;
    QHeaderView *pHeaderView;
};

#endif // COLUMNALIGNEDLAYOUT_H
