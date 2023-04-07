#ifndef I_TOOLBOX_H
#define I_TOOLBOX_H

#include <QGraphicsScene>

#include "MeMoToThemeDefinition.h"
#include "I_ToolSwitcherListener.h"
#include "I_Connectable.h"
#include "I_Tool.h"

class I_ToolBox: public QGraphicsScene
{
public:

    I_ToolBox()
    {
        this->setBackgroundBrush(SCENES_BACKGROUND_COLOR);
    }

    virtual unsigned short getToolsQuantity() const = 0;
    virtual const QList<I_Tool*>& getToolsList() const = 0;

    virtual void selectByID(unsigned short p_ID)
    {
        if( this->getToolsQuantity() <= p_ID )
        {
            if( m_CurrentTool != (this->getToolsQuantity() - 1U) )
            {
                m_CurrentTool = this->getToolsQuantity() - 1U;
                if( nullptr != m_Listener )
                {
                    m_Listener->toolChanged();
                }
            }
        }
        else
        {
            if( m_CurrentTool != p_ID )
            {
                m_CurrentTool = p_ID;
                if( nullptr != m_Listener )
                {
                    m_Listener->toolChanged();
                }
            }
        }
    }
    unsigned short getCurrentToolID() const
    {
        return m_CurrentTool;
    }
    void useTool(QPoint p_Position, Qt::MouseButton p_Button)
    {
        this->getToolsList().at(m_CurrentTool)->use(p_Position, m_Listener, p_Button);
    }
    void registerToolBoxListener(I_ToolSwitcherListener* p_Listener)
    {
        m_Listener = p_Listener;
    }

protected:
    void informToolChanged()
    {
        if( nullptr != m_Listener )
        {
            m_Listener->toolChanged();
        }
    }

private:
    I_ToolSwitcherListener* m_Listener = nullptr;
    unsigned short m_CurrentTool = 0U;
};

#endif // I_TOOLBOX_H
