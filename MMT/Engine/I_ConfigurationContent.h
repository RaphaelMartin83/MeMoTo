#ifndef I_CONFIGURATIONCONTENT_H
#define I_CONFIGURATIONCONTENT_H

#include <QWidget>
#include <QKeyEvent>

#include "I_Configurable.h"

#include "I_Selectable.h"

class I_DiagramContainer;

class I_ConfigurationContent: public QWidget
{
public:
    virtual void setFocusOnFirstZone() = 0;
    virtual void OKPressed() = 0;

    virtual void setWidth(unsigned int p_Width)
    {
        m_Width = p_Width;
    }

    virtual unsigned int getWidth() const
    {
        return m_Width;
    }

    virtual void keyPressEvent(QKeyEvent* p_Event)
    {
        if( p_Event->key() == Qt::Key::Key_Enter || p_Event->key() == Qt::Key::Key_Return )
        {
            this->OKPressed();
        }
        else
        {
            p_Event->ignore();
        }
    }

    void setListener(const QString& p_Type, const QUuid& p_ID)
    {
        m_ListenerType = p_Type;
        m_ListenerID = p_ID;
    }

    I_Configurable* getListener() const;

    void registerDiagram(I_DiagramContainer* p_Diagram)
    {
        m_Diagram = p_Diagram;
    }

private:
    QString m_ListenerType;
    QUuid m_ListenerID;

    unsigned int m_Width = 200U;

    I_DiagramContainer* m_Diagram;
};

#endif // I_CONFIGURATIONCONTENT_H
