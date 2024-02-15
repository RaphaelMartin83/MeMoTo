#include "I_ConfigurationContent.h"

#include "I_DiagramContainer.h"

I_Configurable* I_ConfigurationContent::getListener() const
{
    I_Configurable* l_Configurable = nullptr;

    if( nullptr != m_Diagram )
    {
        I_Selectable* l_Selectable = m_Diagram->getSelectableFromTypeAndID(m_ListenerType, m_ListenerID);

        l_Configurable = dynamic_cast<I_Configurable*>(l_Selectable);
    }

    return l_Configurable;
}
