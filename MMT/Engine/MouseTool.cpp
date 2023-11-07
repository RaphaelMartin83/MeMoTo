#include <QApplication>

#include "MouseTool.h"

MouseTool::MouseTool()
{

}

void MouseTool::use(
        QPoint p_Pos,
        I_ToolListener* p_Target,
        Qt::MouseButtons p_How)
{
    bool l_isControlPressed = ((QApplication::keyboardModifiers() & Qt::ControlModifier) != 0);

    if( Qt::MouseButton::LeftButton == p_How )
    {
        if( false == l_isControlPressed )
        {
            p_Target->unselectAll();
        }
        I_Selectable* l_Selected = p_Target->getSelectable(p_Pos);
        if( nullptr != l_Selected )
        {
            l_Selected->select(p_Pos);
        }
    }
    else if( Qt::MouseButton::RightButton == p_How )
    {
        QList<I_Selectable*> l_Selected = p_Target->getCurrentSelected(false);
        if( 1U == l_Selected.count() )
        {
            l_Selected[0U]->move(p_Pos);
            if( true == l_Selected[0U]->isConnectable() )
            {
                p_Target->rerouteConnectableConnections(static_cast<I_Connectable*>(l_Selected[0U]));
            }
            p_Target->changed(nullptr);
        }
        else if( 1U < l_Selected.count() )
        {
            // Take middle point of selection and translate everyone
            QPoint l_middlePoint = p_Target->getSelectionCoord();
            unsigned long l_dX = p_Pos.x() - l_middlePoint.x();
            unsigned long l_dY = p_Pos.y() - l_middlePoint.y();

            for(unsigned int i_items=0U; i_items < l_Selected.count(); i_items++ )
            {
                QPoint l_targetPoint = l_Selected[i_items]->getPos();
                l_targetPoint.setX(l_targetPoint.x() + l_dX);
                l_targetPoint.setY(l_targetPoint.y() + l_dY);
                l_Selected[i_items]->move(l_targetPoint);
            }
            for(unsigned int i_items=0U; i_items < l_Selected.count(); i_items++ )
            {
                if( true == l_Selected[i_items]->isConnectable() )
                {
                    p_Target->rerouteConnectableConnections(static_cast<I_Connectable*>(l_Selected[0U]));
                }
            }
            p_Target->changed(nullptr);
        }
    }
}

QString MouseTool::getItemName() const
{
    return "Mouse";
}
