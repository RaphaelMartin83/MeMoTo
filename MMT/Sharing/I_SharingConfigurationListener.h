#ifndef I_SHARINGCONFIGURATIONLISTENER_H
#define I_SHARINGCONFIGURATIONLISTENER_H

#include <QString>

class I_SharingConfigurationListener
{
public:
    virtual void sharingHostSelected(const QString& p_Place,
                                      const quint16& p_Port) = 0;
    virtual void sharingSessionCreationSelected(const quint16& p_Port) = 0;
    virtual void sharingStopped() = 0;
    virtual void sharingCanceled() = 0;
};

#endif // I_SHARINGCONFIGURATIONLISTENER_H
