#include <QPainter>

#include "GraphicsRoundedRectItem.h"

GraphicsRoundedRectItem::GraphicsRoundedRectItem(QRect p_Rect):
    QGraphicsRectItem(p_Rect)
{

}

QPainterPath GraphicsRoundedRectItem::shape() const
{
    QPainterPath l_Ret;

    l_Ret.moveTo(this->rect().topLeft());
    l_Ret.addRoundedRect(this->rect(), 10, 10, Qt::RelativeSize);

    return l_Ret;
}

void GraphicsRoundedRectItem::paint(QPainter* painter,
                                    const QStyleOptionGraphicsItem* option,
                                    QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPen l_Pen(Qt::black);
    l_Pen.setWidth(1.0);

    painter->setPen(l_Pen);
    painter->setBrush(this->brush());
    painter->drawPath(this->shape());
}
