#ifndef I_GRAPHICSITEM_H
#define I_GRAPHICSITEM_H

#include <QPoint>
#include <QGraphicsItem>
#include <QVector2D>
#include <QUuid>

#include "StackingDefinitions.h"
#include "GridReferential.h"

#include "I_Serializable.h"

class I_DiagramContainer;

class I_GraphicsItem:
        public QGraphicsItemGroup,
        public I_Serializable
{
public:
    I_GraphicsItem(const QPoint& p_Pos, bool p_byPassGrid=false):
        m_Plan(MAX_PLAN)
      , m_ID(QUuid::createUuid())
      , m_Pos(p_byPassGrid?p_Pos:GridReferential::getPointOnGrid(p_Pos))
    {
    }
    I_GraphicsItem(const QJsonObject& p_JSon):
        m_Plan(MAX_PLAN)
    {
        I_GraphicsItem::fromJson(p_JSon);
    }

    // I_Serializable
    virtual QJsonObject toJson() const
    {
        QJsonObject l_Ret;

        l_Ret.insert("ID", this->getID().toString());
        l_Ret.insert("PositionX", this->getPos().x());
        l_Ret.insert("PositionY", this->getPos().y());

        return l_Ret;
    }
    virtual void fromJson(const QJsonObject& p_Json)
    {
        this->setID(p_Json.find("ID")->toString());
        m_Pos = QPoint(p_Json.find("PositionX")->toInt(),
                         p_Json.find("PositionY")->toInt());
    }

    virtual QPoint getPos() const
    {
        return m_Pos;
    }
    virtual void setPos(const QPoint& p_Pos, bool p_byPassGrid = false)
    {
        QPoint l_Before = this->getPos();
        if( !p_byPassGrid )
        {
            m_Pos = GridReferential::getPointOnGrid(p_Pos);
        }
        else
        {
            m_Pos = p_Pos;
        }
        QPoint l_After = this->getPos();

        this->moveChildren(l_Before, l_After);

        this->refreshDisplay();
    }
    virtual void moveChildren(const QPoint& p_Before,
                              const QPoint& p_After) {}

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

    QUuid getID() const
    {
        return m_ID;
    }
    void setID(const QString& p_StringID)
    {
        m_ID = QUuid::fromString(p_StringID);
    }
    void rerollID()
    {
        m_ID = QUuid::createUuid();
    }

private:
    unsigned int m_Plan;
    QUuid m_ID;
    QPoint m_Pos;

    I_DiagramContainer* m_Container;
};

#endif // I_GRAPHICSITEM_H
