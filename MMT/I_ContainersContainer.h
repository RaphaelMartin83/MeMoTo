#ifndef I_CONTAINERSCONTAINER_H
#define I_CONTAINERSCONTAINER_H

#include <QVector2D>
#include <QList>

#include "I_ConnectableContainer.h"

class I_ContainersContainer:
        public I_ConnectableContainer
{
public:
    virtual ~I_ContainersContainer()
    {
        QList<I_Selectable*> l_Items = I_ContainersContainer::getAllSelectables();

        for( unsigned int i_item = 0U; i_item < l_Items.count(); i_item++ )
        {
            delete l_Items[i_item];
        }
    }

    virtual void clearAll()
    {
        I_ConnectableContainer::clearAll();

        while(0U != m_Containers.count())
        {
            delete m_Containers.takeAt(0U);
        }
    }

    virtual void addSelectable(I_Selectable* p_Selectable)
    {
        I_ContainersContainer* l_Container = static_cast<I_ContainersContainer*>(this->getContainerAtPos(
                    dynamic_cast<I_Selectable*>(p_Selectable)->getPos()));

        if( nullptr == l_Container )
        {
            // I'll handle it
            I_ConnectableContainer::addSelectable(p_Selectable);
        }
        else
        {
            l_Container->addSelectable(p_Selectable);
        }
    }

    virtual void addConnectable(I_Connectable* p_Connectable)
    {
        I_ContainersContainer* l_Container = static_cast<I_ContainersContainer*>(this->getContainerAtPos(
                    dynamic_cast<I_Selectable*>(p_Connectable)->getPos()));

        if( nullptr == l_Container )
        {
            // I'll handle it
            I_ConnectableContainer::addConnectable(p_Connectable);
        }
        else
        {
            l_Container->addConnectable(p_Connectable);
        }
    }

    virtual void addContainer(I_ContainersContainer* p_Container)
    {
        I_ContainersContainer* l_Container = static_cast<I_ContainersContainer*>(this->getContainerAtPos(
                    dynamic_cast<I_Selectable*>(p_Container)->getPos()));
        if( nullptr == l_Container )
        {
            // I'll handle it
            m_Containers.append(p_Container);
        }
        else
        {
            l_Container->addContainer(p_Container);
        }
    }
    const QList<I_ContainersContainer*>& getPureContainers() const
    {
        return m_Containers;
    }
    void deleteContainerAt(unsigned int p_index)
    {
        delete m_Containers.takeAt(p_index);
    }
    QList<I_Connectable*> getAllConnectables(bool p_recursive=false) const
    {
        QList<I_Connectable*> l_Ret = I_ConnectableContainer::getAllConnectables();

        for( unsigned int i_connectables = 0U; i_connectables < m_Containers.count(); i_connectables++ )
        {
            // Contained containers are always connectables
            if( p_recursive )
            {
                l_Ret.append(m_Containers[i_connectables]->getAllConnectables(p_recursive));
            }
            l_Ret.append(dynamic_cast<I_Connectable*>(m_Containers[i_connectables]));
        }

        return l_Ret;
    }
    virtual QList<I_Selectable*> getAllSelectables(bool p_recursive=false) const
    {
        QList<I_Selectable*> l_Ret = I_ConnectableContainer::getAllSelectables(p_recursive);

        for( unsigned int i_containers = 0U;
             i_containers < m_Containers.count(); i_containers++ )
        {
            // Contained containers are always selectables
            l_Ret.append(dynamic_cast<I_Selectable*>(m_Containers[i_containers]));
            if(p_recursive)
            {
                l_Ret.append(m_Containers[i_containers]->getAllSelectables(p_recursive));
            }
        }

        return l_Ret;
    }
    I_ContainersContainer* getContainerAtPos(QPoint p_Pos) const
    {
        I_ContainersContainer* l_Ret = nullptr;

        for( unsigned int i_tl = 0U;
             (i_tl < this->getPureContainers().count()) && (nullptr == l_Ret);
             i_tl++ )
        {
            if( dynamic_cast<I_Selectable*>(this->getPureContainers()[i_tl])->isItYou(p_Pos) )
            {
                l_Ret = static_cast<I_ContainersContainer*>(
                            this->getPureContainers()[i_tl]);
            }
        }
        return l_Ret;
    }
    virtual I_Connectable* getConnectable(QPoint p_Pos) const
    {
        I_Connectable* l_Ret = nullptr;

        I_ContainersContainer* l_Container = static_cast<I_ContainersContainer*>(
                    this->getContainerAtPos(p_Pos));
        if( nullptr == l_Container )
        {
            // I'll handle it
            l_Ret = I_ConnectableContainer::getConnectable(p_Pos);
        }
        else
        {
            l_Ret = l_Container->getConnectable(p_Pos);
            I_Connectable* l_ContainerAsConnectable = dynamic_cast<I_Connectable*>(l_Container);
            if((nullptr == l_Ret) &&
                    (nullptr != l_ContainerAsConnectable) &&
                    (l_ContainerAsConnectable->wouldYouConnect(p_Pos)))
            {
                l_Ret = dynamic_cast<I_Connectable*>(l_Container);
            }
        }
        return l_Ret;
    }
    virtual I_Selectable* getChildSelected() const
    {
        I_Selectable* l_Ret = nullptr;

        for(unsigned int i_child = 0U;
            (i_child < this->getPureSelectables().count()) && (nullptr == l_Ret);
             i_child++ )
        {
            if(this->getPureSelectables()[i_child]->isFullySelected())
            {
                l_Ret = this->getPureSelectables()[i_child];
            }
        }

        for(unsigned int i_child = 0U;
            (i_child < this->getPureConnectables().count()) && (nullptr == l_Ret);
             i_child++ )
        {
            if(this->getPureConnectables()[i_child]->isFullySelected())
            {
                l_Ret = this->getPureConnectables()[i_child];
            }
        }

        for(unsigned int i_child = 0U;
            (i_child < this->getPureContainers().count()) && (nullptr == l_Ret);
             i_child++ )
        {
            if(dynamic_cast<I_Selectable*>(this->getPureContainers()[i_child])->isFullySelected())
            {
                l_Ret = dynamic_cast<I_Selectable*>(this->getPureContainers()[i_child]);
            }
            else
            {
                l_Ret = this->getPureContainers()[i_child]->getChildSelected();
            }
        }

        return l_Ret;
    }
    virtual void deleteSelectedChild()
    {
        bool l_isFound = false;

        for(unsigned int i_child = 0U;
            (i_child < this->getPureSelectables().count()) && (false == l_isFound);
             i_child++ )
        {
            l_isFound = this->getPureSelectables()[i_child]->isFullySelected();
            if(l_isFound)
            {
                this->deleteSelectableAt(i_child);
            }
        }

        for(unsigned int i_child = 0U;
            (i_child < this->getPureConnectables().count()) && (false == l_isFound);
             i_child++ )
        {
            l_isFound = this->getPureConnectables()[i_child]->isFullySelected();
            if(l_isFound)
            {
                this->deleteConnectableAt(i_child);
            }
        }

        for(unsigned int i_child = 0U;
            (i_child < this->getPureContainers().count()) && (false == l_isFound);
             i_child++ )
        {
            if(dynamic_cast<I_Selectable*>(this->getPureContainers()[i_child])->isFullySelected())
            {
                delete m_Containers.takeAt(i_child);
                l_isFound = true;
            }
            else if(nullptr != this->getPureContainers()[i_child]->getChildSelected())
            {
                this->getPureContainers()[i_child]->deleteSelectedChild();
                l_isFound = true;
            }
        }
    }
    virtual bool isItYourChild(const I_Connectable* p_Connectable) const
    {
        bool l_Ret = I_ConnectableContainer::isItYourChild(p_Connectable);

        for( unsigned int i_cont = 0U;
             (i_cont < m_Containers.count()) && (false == l_Ret);
             i_cont++)
        {
            l_Ret = ( m_Containers[i_cont]->isItYourChild(p_Connectable) ||
                     (p_Connectable == dynamic_cast<const I_Connectable*>(m_Containers[i_cont])) );
        }

        return l_Ret;
    }
    virtual QJsonObject toJson() const
    {
        QJsonObject l_MyJson = I_ConnectableContainer::toJson();

        for( unsigned int i_con = 0U; i_con < m_Containers.count(); i_con++ )
        {
            I_ContainersContainer* l_CurrentSelectable = m_Containers[i_con];

            QJsonObject::iterator l_ArrayFound = l_MyJson.find(l_CurrentSelectable->getSerializableName());
            QJsonArray l_Array;
            if( l_MyJson.end() != l_ArrayFound )
            {
                l_Array = l_ArrayFound->toArray();
            }
            QJsonObject l_CurrentObject = l_CurrentSelectable->toJson();
            l_Array.append(l_CurrentSelectable->toJson());
            l_MyJson.insert(l_CurrentSelectable->getSerializableName(), l_Array);
        }

        return l_MyJson;
    }

private:
    QList<I_ContainersContainer*> m_Containers;
};

#endif // I_CONTAINERSCONTAINER_H
