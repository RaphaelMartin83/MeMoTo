#ifndef GRIDREFERENTIAL_H
#define GRIDREFERENTIAL_H

#include <QPoint>

class GridReferential
{
public:
    typedef enum
    {
        eFindClosest,
        eFindDirectUp,
        eFindDirectDown
    }roundness;

    static qint64 getGridDistance(const qint64& p_DistToTransform,
                                  GridReferential::roundness p_roundness=eFindClosest);
    static QPoint getPointOnGrid(const QPoint& p_PointToTransform,
                                 GridReferential::roundness p_roundness=eFindClosest);

private:
    static const unsigned short s_SPAN= 20U;
};

#endif // GRIDREFERENTIAL_H
