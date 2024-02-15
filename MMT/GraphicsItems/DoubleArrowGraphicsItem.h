#ifndef DOUBLEARROWGRAPHICSITEM_H
#define DOUBLEARROWGRAPHICSITEM_H

#include <QGraphicsItemGroup>
#include <QGraphicsLineItem>

typedef enum
{
    eArrowDirection_Horizontal,
    eArrowDirection_Vertical
}eArrowDirection;

class DoubleArrowGraphicsItem: public QGraphicsItemGroup
{
public:
    DoubleArrowGraphicsItem();
    DoubleArrowGraphicsItem(eArrowDirection p_Direction,
                            unsigned short p_Length,
                            unsigned short p_Width,
                            QPoint p_Position,
                            Qt::GlobalColor p_Color);

    void setDirection(eArrowDirection p_DIrection);
    void setLength(unsigned short p_Length);
    void setWidth(unsigned short p_Width);
    void setPosition(QPoint p_Pos);
    void setColor(Qt::GlobalColor p_Color);

    void refreshDisplay();

private:
    QGraphicsLineItem* m_MainLine;
    QGraphicsLineItem* m_Arrows[4U];

    unsigned short m_Direction;
    unsigned short m_Length;
    unsigned short m_Width;
    QPoint m_Position;
    Qt::GlobalColor m_Color;
};

#endif // DOUBLEARROWGRAPHICSITEM_H
