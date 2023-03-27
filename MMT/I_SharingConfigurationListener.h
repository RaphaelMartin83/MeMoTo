#ifndef I_SHARINGCONFIGURATIONLISTENER_H
#define I_SHARINGCONFIGURATIONLISTENER_H

#include <QString>

class I_SharingConfigurationListener
{
public:
    virtual void sharingPlaceSelected(const QString& p_Place) = 0;
    virtual void sharingCanceled() = 0;
};

#endif // I_SHARINGCONFIGURATIONLISTENER_H
