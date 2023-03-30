#ifndef I_CONTAINERGRAPHICSITEM_H
#define I_CONTAINERGRAPHICSITEM_H

#include "I_Connectable.h"
#include "I_ToolListener.h"

class I_ContainerGraphicsItem:
        public I_Connectable,
        public I_ContainersContainer
{
public:
    I_ContainerGraphicsItem(const QPoint& p_Pos);
    I_ContainerGraphicsItem(const QJsonObject& p_Json);

    // I_Serializabre overrides
    virtual QJsonObject toJson() const
    {
        QJsonObject l_Ret = I_Connectable::toJson();
        l_Ret.insert(QString("Contained"), I_ContainersContainer::toJson());

        return l_Ret;
    }
    virtual void fromJson(QJsonObject p_Json)
    {
        I_Connectable::fromJson(p_Json);
    }

    void moveChildren(
            const QPoint& p_PosBefore, const QPoint& p_PosAfter);

    bool selectChild(const QPoint& p_Pos);
    void unselectChild();
    bool moveChild(const QPoint& p_Pos);

    virtual void resizeToFitContent() = 0;

    virtual void addSelectable(I_Selectable* p_Selectable)
    {
        if( 1U < this->getPlan() )
        {
            p_Selectable->setPlan(this->getPlan() - 1U);
            I_ContainersContainer::addSelectable(p_Selectable);
            resizeToFitContent();
        }
    }

    virtual void addConnectable(I_Connectable* p_Connectable)
    {
        if( 1U < this->getPlan() )
        {
            p_Connectable->setPlan(this->getPlan() - 1U);
            I_ContainersContainer::addConnectable(p_Connectable);
            resizeToFitContent();
        }
    }

    virtual void addContainer(I_ContainersContainer* p_Container)
    {
        if( 1U < this->getPlan() )
        {
            dynamic_cast<I_ContainerGraphicsItem*>(p_Container)->setPlan(this->getPlan() - 1U);
            I_ContainersContainer::addContainer(p_Container);
            resizeToFitContent();
        }
    }

    virtual I_Selectable* getSelectable(const QString& p_SelectableType, const QUuid& p_selectableID) const;
    virtual const I_Connectable* getConnectable(const QString& p_ConnectableType, const QUuid& p_connectableID) const;

    // I_ConnectableContainer
    virtual QList<I_Connector*> getAllConnectors() const;

private:
};

#endif // I_CONTAINERGRAPHICSITEM_H
