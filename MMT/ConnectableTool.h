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

    QString getItemName() const
    {
        return Connectable::SERIALIZABLE_NAME;
    }

};

#endif // CONNECTABLETOOL_H
