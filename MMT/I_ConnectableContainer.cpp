#include "I_ConnectableContainer.h"

QList<I_Selectable*> I_ConnectableContainer::getAllSelectables(bool p_recursive) const
{
    QList<I_Selectable*> l_Ret;

    for( unsigned int i_selectables = 0U;
         i_selectables < m_Selectables.count(); i_selectables++ )
    {
        l_Ret.append(m_Selectables[i_selectables]);
    }
    for( unsigned int i_connectables = 0U;
         i_connectables < m_Connectables.count(); i_connectables++ )
    {
        l_Ret.append(m_Connectables[i_connectables]);
    }
    return l_Ret;
}
const QList<I_Selectable*>& I_ConnectableContainer::getPureSelectables() const
{
    return m_Selectables;
}
I_Selectable* I_ConnectableContainer::getSelectableFromTypeAndID(const QString& p_SelectableType,
                                                 const QUuid& p_SelectableID) const
{
    I_Selectable* l_Ret = nullptr;

    QList<I_Selectable*> l_Selectables = this->getAllSelectables();

    for( unsigned int i_connectables = 0U;
         (i_connectables < l_Selectables.count()) && (nullptr == l_Ret);
         i_connectables++ )
    {
        l_Ret = l_Selectables[i_connectables]->getSelectable(p_SelectableType, p_SelectableID);
    }

    return l_Ret;
}
QList<I_Connectable*> I_ConnectableContainer::getAllConnectables(bool p_recursive) const
{
    return m_Connectables;
}
const QList<I_Connectable*>& I_ConnectableContainer::getPureConnectables() const
{
    return m_Connectables;
}
const I_Connectable* I_ConnectableContainer::getConnectableFromTypeAndID(const QString& p_ConnectableType,
                                                                         const QUuid& p_ConnectableID) const
{
    const I_Connectable* l_Ret = nullptr;

    QList<I_Connectable*> l_Connectables = this->getAllConnectables();

    for( unsigned int i_connectables = 0U;
         (i_connectables < l_Connectables.count()) && (nullptr == l_Ret);
         i_connectables++ )
    {
        l_Ret = l_Connectables[i_connectables]->getConnectable(p_ConnectableType, p_ConnectableID);
    }

    return l_Ret;
}

I_Selectable* I_ConnectableContainer::getSelectable(QPoint p_Pos) const
{
    bool l_isSelectableFound = false;
    QList<I_Selectable*> l_Selectables = this->getAllSelectables();
    QList<I_Selectable*> l_Candidates;
    I_Selectable* l_TheWinner = nullptr;

    for( unsigned int i_selectables = 0U;
         i_selectables < l_Selectables.count();
         i_selectables++ )
    {
        l_isSelectableFound = l_Selectables[i_selectables]->isItYou(p_Pos);
        if( true == l_isSelectableFound )
        {
            l_Candidates.append(l_Selectables[i_selectables]);
        }
    }

    unsigned int l_plan;
    for( unsigned short i_candidates = 0U; i_candidates < l_Candidates.count(); i_candidates++ )
    {
        if( (0U == i_candidates) || (l_Candidates[i_candidates]->getPlan() < l_plan) )
        {
            l_TheWinner = l_Candidates[i_candidates];
            l_plan = l_TheWinner->getPlan();
        }
    }

    return l_TheWinner;
}
I_Selectable* I_ConnectableContainer::getSelectableAt(unsigned int p_index) const
{
    return m_Selectables[p_index];
}
I_Connectable* I_ConnectableContainer::getConnectable(QPoint p_Pos) const
{
    bool l_isConnectableFound = false;
    I_Connectable* l_Ret = nullptr;

    QList<I_Connectable*> l_Connectables = this->getAllConnectables();
    QList<I_Connectable*> l_Candidates;

    for( unsigned int i_connectables = 0U;
         i_connectables < l_Connectables.count();
         i_connectables++ )
    {
        l_isConnectableFound = l_Connectables[i_connectables]->wouldYouConnect(p_Pos);
        if( true == l_isConnectableFound )
        {
            l_Candidates.append(l_Connectables[i_connectables]);
        }
    }

    unsigned int l_plan;
    for( unsigned short i_candidates = 0U; i_candidates < l_Candidates.count(); i_candidates++ )
    {
        if( (0U == i_candidates) || (l_Candidates[i_candidates]->getPlan() < l_plan) )
        {
            l_Ret = l_Candidates[i_candidates];
            l_plan = l_Ret->getPlan();
        }
    }

    return l_Ret;
}
I_Connectable* I_ConnectableContainer::getConnectableAt(unsigned int p_index) const
{
    return m_Connectables[p_index];
}

void I_ConnectableContainer::addSelectable(I_Selectable* p_Selectable)
{
    m_Selectables.append(p_Selectable);
}
void I_ConnectableContainer::addConnectable(I_Connectable* p_Connectable)
{
    m_Connectables.append(p_Connectable);
}
void I_ConnectableContainer::deleteSelectableAt(unsigned int p_index)
{
    delete m_Selectables.takeAt(p_index);
}
void I_ConnectableContainer::deleteConnectableAt(unsigned int p_index)
{
    delete m_Connectables.takeAt(p_index);
}

bool I_ConnectableContainer::isItYourChild(const I_Connectable* p_Connectable) const
{
    bool l_Ret = false;

    for( unsigned int i_connect = 0U;
         (i_connect < m_Connectables.count()) && (false == l_Ret);
          i_connect++ )
    {
        if( p_Connectable == m_Connectables[i_connect] )
        {
            l_Ret = true;
        }
    }

    return l_Ret;
}

QPoint I_ConnectableContainer::getSelectionCoord()
{
    unsigned long long l_XSum = 0U;
    unsigned long long l_YSum = 0U;

    for( unsigned int i_items = 0U; i_items < m_CurrentSelectedType.count(); i_items++ )
    {
        I_Selectable* l_Selectable = this->getSelectableFromTypeAndID(m_CurrentSelectedType[i_items],
                                                                      m_CurrentSelectedID[i_items]);
        l_XSum += l_Selectable->getPos().x();
        l_YSum += l_Selectable->getPos().y();
    }

    QPoint l_Ret;
    if( 0U != m_CurrentSelectedType.count() )
    {
        l_Ret.setX(l_XSum / m_CurrentSelectedType.count());
        l_Ret.setY(l_YSum / m_CurrentSelectedType.count());
    }

    return l_Ret;
}

void I_ConnectableContainer::unselectAll()
{
    for( unsigned int i_items = 0U; i_items < m_CurrentSelectedType.count(); i_items++ )
    {
        I_Selectable* l_FoundSelectable = this->getSelectableFromTypeAndID(
                    m_CurrentSelectedType[i_items], m_CurrentSelectedID[i_items]);
        if( nullptr != l_FoundSelectable )
        {
            l_FoundSelectable->unselect();
        }
    }

    m_CurrentSelectedType.clear();
    m_CurrentSelectedID.clear();
}

QJsonObject I_ConnectableContainer::toJson() const
{
    QJsonObject l_MyJson;

    for( unsigned int i_sel = 0U; i_sel < m_Selectables.count(); i_sel++ )
    {
        QJsonObject::iterator l_TempContainedArray = l_MyJson.find(m_Selectables[i_sel]->getSerializableName());
        QJsonArray l_Array;
        if( l_MyJson.end() != l_TempContainedArray )
        {
            l_Array = l_TempContainedArray->toArray();
        }
        l_Array.append(m_Selectables[i_sel]->toJson());
        l_MyJson.insert(m_Selectables[i_sel]->getSerializableName(), l_Array);
    }

    for( unsigned int i_con = 0U; i_con < m_Connectables.count(); i_con++ )
    {
        QJsonObject::iterator l_TempContainedArray = l_MyJson.find(m_Connectables[i_con]->getSerializableName());
        QJsonArray l_Array;
        if( l_MyJson.end() != l_TempContainedArray )
        {
            l_Array = l_TempContainedArray->toArray();
        }
        l_Array.append(m_Connectables[i_con]->toJson());
        l_MyJson.insert(m_Connectables[i_con]->getSerializableName(), l_Array);
    }

    return l_MyJson;
}
