#ifndef I_PRINTIMAGECONFIGURATIONLISTENER_H
#define I_PRINTIMAGECONFIGURATIONLISTENER_H

#include <QString>

class I_PrintImageConfigurationListener
{
public:
    virtual void imageSelectedForPrinting(QString p_File) = 0;
    virtual void imagePrintingCanceled() = 0;
};

#endif // I_PRINTIMAGECONFIGURATIONLISTENER_H
