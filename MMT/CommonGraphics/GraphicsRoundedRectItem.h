#ifndef GRAPHICSROUNDEDRECTITEM_H
#define GRAPHICSROUNDEDRECTITEM_H

#include <QGraphicsRectItem>

class GraphicsRoundedRectItem: public QGraphicsRectItem
{
public:
    GraphicsRoundedRectItem(QRect p_Rect);

    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};

#endif // GRAPHICSROUNDEDRECTITEM_H
