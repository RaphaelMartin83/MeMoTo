#ifndef I_CONNECTOR_H
#define I_CONNECTOR_H

#include "I_Connectable.h"
#include "I_Selectable.h"

class I_Connector: public I_Selectable
{
public:
    I_Connector():
        I_Selectable(QPoint())
    {

    }
    I_Connector(const QJsonObject& p_Json):
        I_Selectable(p_Json)
    {

    }
    virtual bool needReroute() const = 0;
    virtual void reroute() = 0;
    virtual const I_Connectable* const getConnectFrom() const = 0;
    virtual void setConnectFrom(const I_Connectable* p_ConnectFrom) = 0;
    virtual const I_Connectable* const getConnectTo() const = 0;
    virtual void setConnectTo(const I_Connectable* p_ConnectTo) = 0;
};

#endif // I_CONNECTOR_H
