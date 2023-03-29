#ifndef CONTAINERTOOL_H
#define CONTAINERTOOL_H

#include "I_Tool.h"
#include "I_DiagramContainer.h"

template <class Container>
class ContainerTool:public I_Tool
{
public:
    ContainerTool()
    {

    }

    virtual bool createsContainers() const
    {
        return true;
    }
    virtual bool createsConnectors() const
    {
        return false;
    }

    void use(const QJsonObject& p_JsonObject, I_ToolListener* p_Target)
    {
        Container* l_NewContainer = new Container(p_JsonObject);
        l_NewContainer->registerDiagramContainer(dynamic_cast<I_DiagramContainer*>(p_Target));
        p_Target->addContainer(l_NewContainer);
    }

    void paste(const QJsonObject& p_JsonObject, QPoint p_middlePos, I_ToolListener* p_Target)
    {
        // ToolListener here has to be a I_DiagramContainer
        I_DiagramContainer* l_TargetAsContainer = dynamic_cast<I_DiagramContainer*>(p_Target);
        if( nullptr == l_TargetAsContainer )
        {
            return;
        }

        // Create with 0,0 position temporarily
        Container* l_NewContainer= new Container(QPoint());
        l_NewContainer->registerDiagramContainer(l_TargetAsContainer);
        l_NewContainer->fromJson(p_JsonObject);

        // Set position according to current cursor position
        QPoint l_JsonPos = l_NewContainer->getPos();
        // Normalize position with middle pos
        l_JsonPos.setX(l_JsonPos.x() - p_middlePos.x());
        l_JsonPos.setY(l_JsonPos.y() - p_middlePos.y());
        // Set new position
        QPoint l_CursorPosition = l_TargetAsContainer->getCurrentCursorPosition();
        l_JsonPos += l_CursorPosition;
        l_NewContainer->setPos(l_JsonPos);

        p_Target->addContainer(l_NewContainer);
        p_Target->changed(nullptr);
    }

    void use(QPoint p_Pos,
            I_ToolListener* p_Target,
            Qt::MouseButtons p_How)
    {
        if( Qt::MouseButton::LeftButton == p_How )
        {
            Container* l_NewContainer = new Container(p_Pos);
            l_NewContainer->registerDiagramContainer(dynamic_cast<I_DiagramContainer*>(p_Target));
            p_Target->addContainer(l_NewContainer);
            p_Target->changed(nullptr);
        }
    }

    QString getItemName() const
    {
        return Container::SERIALIZABLE_NAME;
    }
};

#endif // CONTAINERTOOL_H
