#ifndef GRAPHICSBEZIERCURVE_H
#define GRAPHICSBEZIERCURVE_H

#include <QPen>
#include <QPoint>
#include <QList>
#include <QGraphicsItem>

class GraphicsBezierCurve: public QGraphicsItem
{
public:
    GraphicsBezierCurve(const QList<QPoint>& p_Points,
                        QGraphicsItem* p_Parent = nullptr);

    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    QRectF boundingRect() const override;

    void setPen(QPen p_Pen);

private:
    QList<QPoint> m_Points;
    QPen m_Pen;
};

#endif // GRAPHICSBEZIERCURVE_H
