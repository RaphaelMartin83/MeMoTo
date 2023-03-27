#include <QPen>

#include "DoubleArrowGraphicsItem.h"

static const unsigned short ARROW_THICKNESS = 2U;

DoubleArrowGraphicsItem::DoubleArrowGraphicsItem():
    m_Direction(eArrowDirection_Horizontal)
  , m_Length(20U)
  , m_Width(6U)
  , m_Position(QPoint(0,0))
  , m_Color(Qt::black)
{
    // The main line
    m_MainLine = new QGraphicsLineItem();
    this->addToGroup(m_MainLine);

    for( unsigned short i_arrows=0U; i_arrows < 4U; i_arrows++ )
    {
        m_Arrows[i_arrows] = new QGraphicsLineItem();
        this->addToGroup(m_Arrows[i_arrows]);
    }

    this->refreshDisplay();
}
DoubleArrowGraphicsItem::DoubleArrowGraphicsItem(eArrowDirection p_Direction,
                            unsigned short p_Length,
                            unsigned short p_Width,
                            QPoint p_Position,
                            Qt::GlobalColor p_Color):
    m_Direction(p_Direction)
  , m_Length(p_Length)
  , m_Width(p_Width)
  , m_Position(p_Position)
  , m_Color(p_Color)
{
    // The main line
    m_MainLine = new QGraphicsLineItem();
    this->addToGroup(m_MainLine);

    for( unsigned short i_arrows=0U; i_arrows < 4U; i_arrows++ )
    {
        m_Arrows[i_arrows] = new QGraphicsLineItem();
        this->addToGroup(m_Arrows[i_arrows]);
    }

    this->refreshDisplay();
}

void DoubleArrowGraphicsItem::setDirection(eArrowDirection p_Direction)
{
    m_Direction = p_Direction;
    this->refreshDisplay();
}
void DoubleArrowGraphicsItem::setLength(unsigned short p_Length)
{
    m_Length = p_Length;
    this->refreshDisplay();
}
void DoubleArrowGraphicsItem::setWidth(unsigned short p_Width)
{
    m_Width = p_Width;
    this->refreshDisplay();
}
void DoubleArrowGraphicsItem::setPosition(QPoint p_Pos)
{
    m_Position = p_Pos;
    this->refreshDisplay();
}
void DoubleArrowGraphicsItem::setColor(Qt::GlobalColor p_Color)
{
    m_Color = p_Color;
    this->refreshDisplay();
}

void DoubleArrowGraphicsItem::refreshDisplay()
{
    switch(m_Direction)
    {
    case eArrowDirection_Horizontal:
    {
        m_MainLine->setLine(QLineF(m_Position.x() - m_Length/2,
                                                  m_Position.y(),
                                                  m_Position.x() + m_Length/2,
                                                  m_Position.y()));

        m_Arrows[0U]->setLine(QLineF(m_Position.x() - m_Length/2,
                                                    m_Position.y(),
                                                    m_Position.x() - m_Length/2 + m_Width/2,
                                                    m_Position.y() - m_Width/2));
        m_Arrows[1U]->setLine(QLineF(m_Position.x() - m_Length/2,
                                                    m_Position.y(),
                                                    m_Position.x() - m_Length/2 + m_Width/2,
                                                    m_Position.y() + m_Width/2));
        m_Arrows[2U]->setLine(QLineF(m_Position.x() + m_Length/2,
                                                    m_Position.y(),
                                                    m_Position.x() + m_Length/2 - m_Width/2,
                                                    m_Position.y() - m_Width/2));
        m_Arrows[3U]->setLine(QLineF(m_Position.x() + m_Length/2,
                                                    m_Position.y(),
                                                    m_Position.x() + m_Length/2 - m_Width/2,
                                                    m_Position.y() + m_Width/2));
        break;
    }
    case eArrowDirection_Vertical:
    {
        m_MainLine->setLine(QLineF(m_Position.x(),
                                   m_Position.y() - m_Length/2,
                                   m_Position.x(),
                                   m_Position.y() + m_Length/2));

        m_Arrows[0U]->setLine(QLineF(m_Position.x(),
                                     m_Position.y() - m_Length/2,
                                     m_Position.x() - m_Width/2,
                                     m_Position.y() - m_Length/2 + m_Width/2));
        m_Arrows[1U]->setLine(QLineF(m_Position.x(),
                                     m_Position.y() - m_Length/2,
                                     m_Position.x() + m_Width/2,
                                     m_Position.y() - m_Length/2 + m_Width/2));
        m_Arrows[2U]->setLine(QLineF(m_Position.x(),
                                     m_Position.y() + m_Length/2,
                                     m_Position.x() - m_Width/2,
                                     m_Position.y() + m_Length/2 - m_Width/2));
        m_Arrows[3U]->setLine(QLineF(m_Position.x(),
                                     m_Position.y() + m_Length/2,
                                     m_Position.x() + m_Width/2,
                                     m_Position.y() + m_Length/2 - m_Width/2));
        break;
    }
    }
    m_MainLine->setPen(QPen(m_Color, ARROW_THICKNESS));
    for( unsigned short i_arrows=0U; i_arrows < 4U; i_arrows++ )
    {
        m_Arrows[i_arrows]->setPen(QPen(m_Color, ARROW_THICKNESS));
    }
}
