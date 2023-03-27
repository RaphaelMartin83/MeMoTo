#ifndef I_CONFIGLINEEDITLISTENER_H
#define I_CONFIGLINEEDITLISTENER_H

class I_ConfigLineEditListener
{
public:
    virtual void enterPressed() = 0;
    virtual void escapePressed() = 0;
};

#endif // I_CONFIGLINEEDITLISTENER_H
