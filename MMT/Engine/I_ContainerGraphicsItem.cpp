#include <QVector2D>

#include "I_ContainerGraphicsItem.h"

I_ContainerGraphicsItem::I_ContainerGraphicsItem(const QPoint& p_Pos):
    I_Connectable(p_Pos)
{

}

I_ContainerGraphicsItem::I_ContainerGraphicsItem(const QJsonObject& p_Json):
    I_Connectable(p_Json)
{

}

void I_ContainerGraphicsItem::moveChildren(
        const QPoint& p_PosBefore, const QPoint& p_PosAfter)
{
    // Retrieve all selectables
    QList<I_Selectable*> l_Selectables = this->getAllSelectables();

    int l_dx = p_PosAfter.x() - p_PosBefore.x();
    int l_dy = p_PosAfter.y() - p_PosBefore.y();

    for( unsigned int i_items = 0U; i_items < l_Selectables.count(); i_items++ )
    {
        l_Selectables[i_items]->translate(l_dx, l_dy);
    }
}

bool I_ContainerGraphicsItem::selectChild(const QPoint& p_Pos)
{
    bool l_Ret = false;

    QList<I_Selectable*> l_Selectables = this->getAllSelectables();

    for(unsigned int i_sel = 0U; i_sel < l_Selectables.count(); i_sel++)
    {
        l_Ret = l_Selectables[i_sel]->isItYou(p_Pos);
        if(l_Ret)
        {
            l_Selectables[i_sel]->select(p_Pos);
            break;
        }
    }

    return l_Ret;
}

void I_ContainerGraphicsItem::unselectChild()
{
    QList<I_Selectable*> l_Selectables = this->getAllSelectables();

    for(unsigned int i_sel = 0U; i_sel < l_Selectables.count(); i_sel++)
    {
        if( l_Selectables[i_sel]->isSelected() )
        {
            l_Selectables[i_sel]->unselect();
        }
    }
}

bool I_ContainerGraphicsItem::moveChild(const QPoint& p_Pos)
{
    bool l_Ret = false;

    // If click is into the container
    if(this->isItYou(p_Pos))
    {
        QList<I_Selectable*> l_Selectables = this->getAllSelectables();

        for(unsigned int i_sel = 0U; i_sel < l_Selectables.count(); i_sel++)
        {
            l_Selectables[i_sel]->move(p_Pos);
        }
    }

    return l_Ret;
}

I_Selectable* I_ContainerGraphicsItem::getSelectable(const QString& p_SelectableType, const QUuid& p_selectableID) const
{
    I_Selectable* l_Ret = I_Selectable::getSelectable(p_SelectableType, p_selectableID);

    QList<I_Selectable*> l_Selectables = this->getAllSelectables();
    for(unsigned int i_con = 0U;
        (i_con < l_Selectables.count()) && (nullptr == l_Ret);
        i_con++)
    {
        l_Ret = l_Selectables[i_con]->getSelectable(p_SelectableType, p_selectableID);
    }

    return l_Ret;
}

const I_Connectable* I_ContainerGraphicsItem::getConnectable(const QString& p_ConnectableType, const QUuid& p_connectableID) const
{
    const I_Connectable* l_Ret = I_Connectable::getConnectable(p_ConnectableType, p_connectableID);

    QList<I_Connectable*> l_Connectables = this->getAllConnectables();
    for(unsigned int i_con = 0U;
        (i_con < l_Connectables.count()) && (nullptr == l_Ret);
        i_con++)
    {
        l_Ret = l_Connectables[i_con]->getConnectable(p_ConnectableType, p_connectableID);
    }

    return l_Ret;
}

QList<I_Connector*> I_ContainerGraphicsItem::getAllConnectors() const
{
    return QList<I_Connector*>();
}
