#ifndef I_FINDCONFIGURATIONLISTENER_H
#define I_FINDCONFIGURATIONLISTENER_H

#include <QStringList>

class I_FindConfigurationListener
{
public:
    virtual void findConfigurationValidated(QString p_Type,
                                            QString p_Field,
                                            QString p_Data,
                                            unsigned int p_selectionOffset) = 0;
    virtual void findConfigurationCanceled() = 0;
    virtual QStringList getFieldsOfType(QString p_Type) = 0;
    virtual QStringList getAllDataOfTypeAndField(QString p_Type, QString p_Field) = 0;
    virtual void focusOnItem(QString p_Type, QString p_Field, QString p_Data,
                             unsigned short p_findOffset,
                             bool p_Select = false, bool p_CaseSensitive = true) = 0;
};

#endif // I_FINDCONFIGURATIONLISTENER_H
