#ifndef I_GRAPHICSITEM_H
#define I_GRAPHICSITEM_H

#include <QPoint>
#include <QGraphicsItem>
#include <QVector2D>

#include "StackingDefinitions.h"

#include "I_Serializable.h"

class I_DiagramContainer;

class I_GraphicsItem:
        public QGraphicsItemGroup,
        public I_Serializable
{
public:
    virtual QPoint getPos() const = 0;
    virtual void setPos(const QPoint&) = 0;

    virtual unsigned int getPlan() const
    {
        return m_Plan;
    }
    virtual void setPlan(unsigned int p_Plan)
    {
        m_Plan = p_Plan;
        this->refreshDisplay();
    }

    virtual void refreshDisplay() = 0;

    virtual void translate(int p_dx, int p_dy)
    {
        QPoint l_Pos = this->getPos();

        l_Pos.setX(l_Pos.x() + p_dx);
        l_Pos.setY(l_Pos.y() + p_dy);

        this->setPos(l_Pos);
    }

    void registerDiagramContainer(I_DiagramContainer* p_Container)
    {
        m_Container = p_Container;
    }
    I_DiagramContainer* getDiagramContainer() const
    {
        return m_Container;
    }

    unsigned long long getID() const
    {
        return m_ID;
    }
    void setID(unsigned long long p_ID)
    {
        m_ID = p_ID;
    }
private:
    unsigned int m_Plan = MAX_PLAN;
    unsigned long long m_ID = 0U;

    I_DiagramContainer* m_Container;
};

#endif // I_GRAPHICSITEM_H
