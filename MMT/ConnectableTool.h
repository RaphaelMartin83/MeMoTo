#ifndef CONNECTABLETOOL_H
#define CONNECTABLETOOL_H

#include "I_Tool.h"
#include "I_DiagramContainer.h"

template <class Connectable>
class ConnectableTool: public I_Tool
{
public:
    ConnectableTool()
    {

    }

    virtual bool createsContainers() const
    {
        return false;
    }
    virtual bool createsConnectors() const
    {
        return false;
    }

    void use(const QJsonObject& p_JsonObject, I_ToolListener* p_Target)
    {
        Connectable* l_NewConnector = new Connectable(p_JsonObject);
        l_NewConnector->registerDiagramContainer(dynamic_cast<I_DiagramContainer*>(p_Target));
        p_Target->addConnectable(l_NewConnector);
    }

    void use(QPoint p_Pos,
                 I_ToolListener* p_Target,
                 Qt::MouseButtons p_How)
    {
        if( Qt::MouseButton::LeftButton == p_How )
        {
            Connectable* l_NewConnectable = new Connectable(p_Pos);
            l_NewConnectable->registerDiagramContainer(dynamic_cast<I_DiagramContainer*>(p_Target));
            p_Target->addConnectable(l_NewConnectable);
            p_Target->changed(nullptr);
        }
    }

    void paste(const QJsonObject& p_JsonObject,
               QPoint p_middlePos, I_ToolListener* p_Target)
    {
        // ToolListener here has to be a I_DiagramContainer
        I_DiagramContainer* l_TargetAsContainer = dynamic_cast<I_DiagramContainer*>(p_Target);
        if( nullptr == l_TargetAsContainer )
        {
            return;
        }

        // Create with 0,0 position temporarily
        Connectable* l_NewConnectable = new Connectable(QPoint());
        l_NewConnectable->registerDiagramContainer(l_TargetAsContainer);
        l_NewConnectable->fromJson(p_JsonObject);

        // Set position according to current cursor position
        QPoint l_JsonPos = l_NewConnectable->getPos();
        // Normalize position with middle pos
        l_JsonPos.setX(l_JsonPos.x() - p_middlePos.x());
        l_JsonPos.setY(l_JsonPos.y() - p_middlePos.y());
        // Set new position
        QPoint l_CursorPosition = l_TargetAsContainer->getCurrentCursorPosition();
        l_JsonPos += l_CursorPosition;
        l_NewConnectable->setPos(l_JsonPos);

        p_Target->addConnectable(l_NewConnectable);
        p_Target->changed(nullptr);
    }

    QString getItemName() const
    {
        return Connectable::SERIALIZABLE_NAME;
    }

};

#endif // CONNECTABLETOOL_H
