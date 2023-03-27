#include "ConfigLineEdit.h"

ConfigLineEdit::ConfigLineEdit(I_ConfigLineEditListener* p_Listener):
    QLineEdit()
{
    m_Listener = p_Listener;
}

void ConfigLineEdit::keyPressEvent(QKeyEvent* p_Event)
{
    if( (nullptr != m_Listener) &&
            ((p_Event->key() == Qt::Key::Key_Enter) || (p_Event->key() == Qt::Key::Key_Return)) )
    {
        m_Listener->enterPressed();
    }
    else if( (nullptr != m_Listener) &&
            (p_Event->key() == Qt::Key::Key_Escape) )
    {
        m_Listener->escapePressed();
    }
    else if( p_Event->key() == Qt::Key::Key_Delete)
    {
        // Ignores event to let user delete the object
        p_Event->ignore();
    }
    else if( p_Event->modifiers() == Qt::AltModifier )
    {
        // Ignores event to let user switch context while configuring
        p_Event->ignore();
    }
    else
    {
        QLineEdit::keyPressEvent(p_Event);
    }
}
