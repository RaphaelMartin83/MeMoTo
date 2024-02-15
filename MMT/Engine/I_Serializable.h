#ifndef I_SERIALISABLE_H
#define I_SERIALISABLE_H

#include <QJsonObject>
#include <QJsonArray>

class I_Serializable
{
public:
    virtual QJsonObject toJson() const = 0;
    virtual void fromJson(const QJsonObject& p_Json) = 0;
    virtual QString getSerializableName() const = 0;
};

#endif // I_SERIALISABLE_H
