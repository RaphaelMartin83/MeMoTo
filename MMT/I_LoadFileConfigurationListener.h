#ifndef I_LOADFILECONFIGURATIONLISTENER_H
#define I_LOADFILECONFIGURATIONLISTENER_H

#include <QString>

class I_LoadFileConfigurationListener
{
public:
    virtual void fileSelectedForLoading(QString p_File) = 0;
    virtual void fileLoadingCanceled() = 0;
};

#endif // I_LOADFILECONFIGURATIONLISTENER_H
