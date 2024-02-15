#ifndef MOUSETOOLPOLYGON_H
#define MOUSETOOLPOLYGON_H

#include <QPolygonF>

class MouseToolPolygon: public QPolygonF
{
public:
    MouseToolPolygon(unsigned short p_Size,
                       unsigned short p_WidthToCenter,
                       unsigned short p_HeightToCenter);
};

#endif // MOUSETOOLPOLYGON_H
