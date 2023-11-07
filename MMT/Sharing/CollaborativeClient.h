#ifndef COLLABORATIVECLIENT_H
#define COLLABORATIVECLIENT_H

#include <QTcpSocket>

#include <Engine/I_ConnectionListener.h>

class CollaborativeClient: public QTcpSocket
{
public:
    CollaborativeClient();
    ~CollaborativeClient();

    void start(const QString& p_Host, const quint16& p_Port);
    void stop();
    void updateData(const QByteArray& p_Data);
    void serverDisconnected();
    void dataReady();
    void errorOccurred();

    void registerListener(I_ConnectionListener* p_Listener);

private:
    I_ConnectionListener* m_Listener;
};

#endif // COLLABORATIVECLIENT_H
