#ifndef CONFIGLINEEDIT_H
#define CONFIGLINEEDIT_H

#include <QKeyEvent>
#include <QLineEdit>

#include "I_ConfigLineEditListener.h"

class ConfigLineEdit: public QLineEdit
{
public:
    ConfigLineEdit(I_ConfigLineEditListener* p_Listener);

    void keyPressEvent(QKeyEvent* p_Event);

private:
    I_ConfigLineEditListener* m_Listener;
};

#endif // CONFIGLINEEDIT_H
