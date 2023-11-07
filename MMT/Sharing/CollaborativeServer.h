#ifndef COLLABORATIVESERVER_H
#define COLLABORATIVESERVER_H

#include <QTcpSocket>
#include <QTcpServer>

#include <Engine/I_ConnectionListener.h>

class CollaborativeServer: public QTcpServer
{
    Q_OBJECT
public:
    CollaborativeServer();
    ~CollaborativeServer();

    void start(const QString& p_Host, const quint16& p_Port);
    void stop();
    void updateData(const QByteArray& p_Data);
    void clientDisconnected();
    void dataReady();

    void registerListener(I_ConnectionListener* p_Listener);

private slots:
    void newClientConnected();

private:

    QTcpSocket m_Socket;

    QByteArray m_CurrentSessionData;

    QList<QTcpSocket*> m_Clients;

    I_ConnectionListener* m_Listener;
};

#endif // COLLABORATIVESERVER_H
