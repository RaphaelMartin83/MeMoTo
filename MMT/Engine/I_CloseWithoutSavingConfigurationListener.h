#ifndef I_CLOSEWITHOUTSAVINGCONFIGURATIONLISTENER_H
#define I_CLOSEWITHOUTSAVINGCONFIGURATIONLISTENER_H

class I_CloseWithoutSavingConfigurationListener
{
public:
    virtual void saveBeforeClosing() = 0;
    virtual void closeAndDropChanges() = 0;
};

#endif // I_CLOSEWITHOUTSAVINGCONFIGURATIONLISTENER_H
