#include <qmath.h>

#include "GridReferential.h"

qint64 GridReferential::getGridDistance(const qint64& p_DistToTransform,
                                        GridReferential::roundness p_roundness)
{
    if( 0 == (p_DistToTransform % s_SPAN) )
    {
        // Already on the spot, make computing simpler
        return p_DistToTransform;
    }

    qint64 l_Ret;
    switch(p_roundness)
    {
    case GridReferential::eFindClosest:
    {
        qreal l_RatioInSquare = qreal(p_DistToTransform) / qreal(s_SPAN);
        l_RatioInSquare = l_RatioInSquare - qFloor(l_RatioInSquare);

        if( l_RatioInSquare > 0.5 )
        {
            l_Ret = ((p_DistToTransform / s_SPAN)+1) * s_SPAN;
        }
        else
        {
            l_Ret = (p_DistToTransform / s_SPAN) * s_SPAN;
        }
        break;
    }
    case GridReferential::eFindDirectDown:
    {
        l_Ret = (p_DistToTransform / s_SPAN) * s_SPAN;
        break;
    }
    case GridReferential::eFindDirectUp:
    {
        l_Ret = ((p_DistToTransform / s_SPAN)+1) * s_SPAN;
        break;
    }
    }
    return l_Ret;
}
QPoint GridReferential::getPointOnGrid(const QPoint& p_PointToTransform,
                                       GridReferential::roundness p_roundness)
{
    QPoint l_Ret;

    l_Ret.setX(static_cast<int>(getGridDistance(p_PointToTransform.x(), p_roundness)));
    l_Ret.setY(static_cast<int>(getGridDistance(p_PointToTransform.y(), p_roundness)));

    return l_Ret;
}
