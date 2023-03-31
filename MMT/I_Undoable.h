#ifndef I_UNDOABLE_H
#define I_UNDOABLE_H

#include "I_ContainersContainer.h"

static const unsigned short UNDO_STACK_SIZE = 30U;

class I_Undoable: public I_ContainersContainer
{
public:
    I_Undoable():
        m_UndoStack()
      , m_StackPtr(0U)
      , m_LatestChange(0U)
      , m_OldestChange(0U)
    {

    }

    virtual void saveUndoState()
    {
        m_LatestChange = ++m_StackPtr;
        if( m_LatestChange >= UNDO_STACK_SIZE )
        {
            m_LatestChange = 0U;
            m_StackPtr = 0U;
        }
        if( m_LatestChange == m_OldestChange )
        {
            // Wraping, increase oldestChange
            m_OldestChange++;
            if( m_OldestChange >= UNDO_STACK_SIZE )
            {
                m_OldestChange = 0U;
            }
        }
        m_UndoStack[m_LatestChange] = this->toJson();
    }
    virtual void undo()
    {
        if( m_StackPtr != m_OldestChange )
        {
            // Decrease stack pointer
            if( m_StackPtr == 0U )
            {
                m_StackPtr = UNDO_STACK_SIZE - 1U;
            }
            else
            {
                m_StackPtr--;
            }
            this->fromJson(m_UndoStack[m_StackPtr]);
        }
    }
    virtual void redo()
    {
        if( m_StackPtr != m_LatestChange )
        {
            m_StackPtr++;
            if( m_StackPtr >= UNDO_STACK_SIZE )
            {
                m_StackPtr = 0U;
            }
            this->fromJson(m_UndoStack[m_StackPtr]);
        }
    }

private:
    QJsonObject m_UndoStack[UNDO_STACK_SIZE];
    unsigned short m_StackPtr;
    unsigned short m_LatestChange;
    unsigned short m_OldestChange;
};

#endif // I_UNDOABLE_H
