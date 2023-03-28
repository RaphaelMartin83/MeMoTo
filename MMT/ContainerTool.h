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

    void paste(const QJsonObject& p_JsonObject, QPoint p_middlePoint, I_ToolListener* p_Target)
    {

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
