#include "MouseToolPolygon.h"

MouseToolPolygon::MouseToolPolygon(unsigned short p_Size,
                                   unsigned short p_WidthToCenter,
                                   unsigned short p_HeightToCenter):
    QPolygonF()
{
    qreal dx = p_WidthToCenter/2 - p_Size/2;
    qreal dy = p_HeightToCenter/2 - p_Size/2;
    *this << QPointF(dx, dy+p_Size)
           << QPointF(dx+p_Size/2, dy+p_Size/2)
           << QPointF(dx+p_Size/3, dy+p_Size/3)
           << QPointF(dx+p_Size, dy)
           << QPointF(dx+2*p_Size/3, dy+2*p_Size/3)
           << QPointF(dx+p_Size/2, dy+p_Size/2);
}
