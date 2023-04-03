#include <QPainterPath>
#include <QPen>
#include <QPainter>

#include "GraphicsBezierCurve.h"

GraphicsBezierCurve::GraphicsBezierCurve(
        const QList<QPoint>& p_Points,
        QGraphicsItem* p_Parent):
    QGraphicsItem(p_Parent)
  , m_Points(p_Points)
{

}

QPainterPath GraphicsBezierCurve::shape() const
{
    QPainterPath l_Ret;

    l_Ret.moveTo(m_Points.first());

    unsigned short i_points = 1U;
    while(i_points < m_Points.count())
    {
        const unsigned short l_pointsToArrival = m_Points.count() - i_points;
        if( l_pointsToArrival >= 3U )
        {
            l_Ret.cubicTo(m_Points[i_points], m_Points[i_points + 1U], m_Points[i_points + 2U]);
        }
        else if( l_pointsToArrival >= 2U )
        {
            l_Ret.quadTo(m_Points[i_points], m_Points[i_points + 1U]);
        }
        else // One point left
        {
            l_Ret.lineTo(m_Points[i_points]);
        }

        i_points += l_pointsToArrival;
    }

    return l_Ret;
}

void GraphicsBezierCurve::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    // Draw curve
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(m_Pen); // curve pen

    painter->drawPath(this->shape());
}

QRectF GraphicsBezierCurve::boundingRect() const
{
    unsigned int l_xMin = m_Points[0].x();
    unsigned int l_xMax = m_Points[0].x();
    unsigned int l_yMin = m_Points[0].y();
    unsigned int l_yMax = m_Points[0].y();

    for(unsigned short i_pts = 1U; i_pts < m_Points.count(); i_pts++ )
    {
        if( l_xMin > m_Points[i_pts].x() )
        {
            l_xMin = m_Points[i_pts].x();
        }
        if( l_xMax < m_Points[i_pts].x() )
        {
            l_xMax = m_Points[i_pts].x();
        }
        if( l_yMin > m_Points[i_pts].y() )
        {
            l_yMin = m_Points[i_pts].y();
        }
        if( l_yMax < m_Points[i_pts].y() )
        {
            l_yMax = m_Points[i_pts].y();
        }
    }

    return QRect(l_xMin, l_yMin, l_xMax - l_xMin, l_yMax - l_yMin);
}

void GraphicsBezierCurve::setPen(QPen p_Pen)
{
    m_Pen = p_Pen;
}
