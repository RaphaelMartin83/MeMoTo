#ifndef I_DATAMANAGER_H
#define I_DATAMANAGER_H

#include <QJsonObject>

class I_DataManager
{
public:
    virtual void getApplicationData(QJsonObject& p_rData) const = 0;
    virtual void setApplicationData(const QJsonObject& p_Data) = 0;
};

#endif // I_DATAMANAGER_H
