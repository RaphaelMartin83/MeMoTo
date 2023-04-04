#include "ConfigList.h"

ConfigList::ConfigList(I_ConfigLineEditListener* p_Listener):
    m_Listener(p_Listener)
{
    this->setDragDropMode(QAbstractItemView::InternalMove);
}

void ConfigList::keyPressEvent(QKeyEvent* p_Event)
{
    if( (p_Event->key() == Qt::Key::Key_Enter) || (p_Event->key() == Qt::Key::Key_Return) )
    {
        // Enter must could be pressed to validate text
        // User has to type enter two times to validate text and then config
        QListWidget::keyPressEvent(p_Event);
    }
    else if( p_Event->key() == Qt::Key::Key_Escape )
    {
        if( nullptr != m_Listener )
        {
            m_Listener->escapePressed();
        }
    }
    else if( p_Event->key() == Qt::Key::Key_Delete )
    {
        p_Event->ignore();
    }
    else
    {
        QListWidget::keyPressEvent(p_Event);
    }
}
