#ifndef I_SAVEFILECONFIGURATIONLISTENER_H
#define I_SAVEFILECONFIGURATIONLISTENER_H

#include <QString>

class I_SaveFileConfigurationListener
{
public:
    virtual void fileSelectedForSaving(QString p_File) = 0;
    virtual void fileSavingCanceled() = 0;
};

#endif // I_SAVEFILECONFIGURATIONLISTENER_H
