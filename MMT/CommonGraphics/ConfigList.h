#ifndef CONFIGLIST_H
#define CONFIGLIST_H

#include <QListWidget>
#include <QKeyEvent>

#include <Engine/I_ConfigLineEditListener.h>

class ConfigList:
        public QListWidget
{
public:
    ConfigList(I_ConfigLineEditListener* p_Listener);

    void keyPressEvent(QKeyEvent* p_Event);

private:
    I_ConfigLineEditListener* m_Listener;
};

#endif // CONFIGLIST_H
