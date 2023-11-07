#ifndef CONNECTORTOOL_H
#define CONNECTORTOOL_H

#include "I_Tool.h"
#include "I_DiagramContainer.h"

template <class ConnectorTemplate>
class ConnectorTool: public I_Tool
{
public:
    ConnectorTool():
        m_OnGoingPath()
    {
    }

    virtual bool createsContainers() const
    {
        return false;
    }
    virtual bool createsConnectors() const
    {
        return true;
    }

    void use(const QJsonObject& p_JsonObject, I_ToolListener* p_Target)
    {
        ConnectorTemplate* l_NewConnector = new ConnectorTemplate(p_JsonObject, p_Target);
        l_NewConnector->registerDiagramContainer(dynamic_cast<I_DiagramContainer*>(p_Target));
        p_Target->addConnector(l_NewConnector);
    }

    void use(QPoint p_Pos,
                 I_ToolListener* p_Target,
                 Qt::MouseButtons p_How)
    {
        if( Qt::MouseButton::LeftButton == p_How )
        {
            if( nullptr != p_Target->getFromConnectable() )
            {
                p_Target->getFromConnectable()->unselectForConnection();
            }
            p_Target->setFromConnectable(p_Target->getConnectable(p_Pos));
            if( nullptr != p_Target->getFromConnectable() )
            {
                const I_Connectable* l_FromConnectable = p_Target->getFromConnectable();
                l_FromConnectable->selectForConnection(p_Pos, eConnectionPoint_From);
                m_OnGoingPath.clear();
            }
        }
        else if( Qt::MouseButton::RightButton == p_How )
        {
            if( nullptr != p_Target->getFromConnectable() )
            {
                const I_Connectable* l_ToConnectable = p_Target->getConnectable(p_Pos);
                if( nullptr != l_ToConnectable )
                {
                    l_ToConnectable->selectForConnection(p_Pos, eConnectionPoint_To);
                    ConnectorTemplate* l_NewConnector = nullptr;
                    if( 0U == m_OnGoingPath.count() )
                    {
                        // Will auto route
                        l_NewConnector = new ConnectorTemplate(p_Target->getFromConnectable(),
                             l_ToConnectable,
                             p_Target->getFromConnectable()->getSelectedConnectionPositionFrom(),
                             l_ToConnectable->getSelectedConnectionPositionTo(),
                             p_Target);
                    }
                    else
                    {
                        // Will force path
                        l_NewConnector = new ConnectorTemplate(p_Target->getFromConnectable(),
                             l_ToConnectable,
                             p_Target->getFromConnectable()->getSelectedConnectionPositionFrom(),
                             l_ToConnectable->getSelectedConnectionPositionTo(),
                             p_Target, m_OnGoingPath);
                    }

                    l_NewConnector->registerDiagramContainer(dynamic_cast<I_DiagramContainer*>(p_Target));
                    p_Target->addConnector(l_NewConnector);

                    p_Target->getFromConnectable()->unselectForConnection();
                    p_Target->setFromConnectable(nullptr);
                    l_ToConnectable->unselectForConnection();
                    p_Target->changed(nullptr);
                }
                else
                {
                    m_OnGoingPath.append(p_Pos);
                }
            }
        }
    }

    void paste(const QJsonObject& p_JsonObject,
               QPoint p_middlePos,
               I_ToolListener* p_Target,
               QUuid* p_rIDPasted = nullptr, QUuid* p_rIDRerolled = nullptr)
    {
        ConnectorTemplate* l_NewConnector = new ConnectorTemplate(p_JsonObject, p_Target);
        l_NewConnector->registerDiagramContainer(dynamic_cast<I_DiagramContainer*>(p_Target));
        l_NewConnector->rerollID();

        p_Target->addConnector(l_NewConnector);
        p_Target->changed(nullptr);
    }

    QString getItemName() const
    {
        return ConnectorTemplate::SERIALIZABLE_NAME;
    }
private:
    QList<QPoint> m_OnGoingPath;
};


#endif // CONNECTORTOOL_H
