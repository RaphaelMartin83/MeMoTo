#include "MouseTool.h"

MouseTool::MouseTool()
{

}

void MouseTool::use(
        QPoint p_Pos,
        I_ToolListener* p_Target,
        Qt::MouseButtons p_How)
{
    if( Qt::MouseButton::LeftButton == p_How )
    {
        if( nullptr != p_Target->getCurrentSelected() )
        {
            p_Target->getCurrentSelected()->unselect();
        }
        I_Selectable* l_Selected = p_Target->getSelectable(p_Pos);
        p_Target->setCurrentSelected(l_Selected);
        if( nullptr != l_Selected )
        {
            l_Selected->select(p_Pos);
        }
    }
    else if( Qt::MouseButton::RightButton == p_How )
    {
        I_Selectable* l_Selected = p_Target->getCurrentSelected();
        if( nullptr != l_Selected )
        {
            l_Selected->move(p_Pos);
            if( true == l_Selected->isConnectable() )
            {
                p_Target->rerouteConnectableConnections(static_cast<I_Connectable*>(l_Selected));
            }
            p_Target->changed(nullptr);
        }
    }
}

QString MouseTool::getItemName() const
{
    return "Mouse";
}
