#ifndef I_CONNECTIONLISTENER_H
#define I_CONNECTIONLISTENER_H

#include <QByteArray>

class I_ConnectionListener
{
public:
    virtual void dataChanged(const QByteArray& p_Data) = 0;
    virtual void newClientConnected() = 0;
    virtual void connectionClosed() = 0;
};

#endif // I_CONNECTIONLISTENER_H
