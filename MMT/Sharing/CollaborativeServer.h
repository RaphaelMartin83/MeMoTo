#ifndef COLLABORATIVESERVER_H
#define COLLABORATIVESERVER_H

#include <QWebSocketServer>
#include <QWebSocket>

#include <Engine/I_ConnectionListener.h>

class CollaborativeServer: public QWebSocketServer
{
    Q_OBJECT
public:
    CollaborativeServer();
    ~CollaborativeServer();

    void start(const QString& p_Host, const quint16& p_Port);
    void stop();
    void updateData(const QByteArray& p_Data);
    void clientDisconnected();
    void dataReady(QByteArray);

    void registerListener(I_ConnectionListener* p_Listener);

private slots:
    void newClientConnected();

private:

    QWebSocket m_Socket;

    QList<QWebSocket*> m_Clients;

    I_ConnectionListener* m_Listener;
};

#endif // COLLABORATIVESERVER_H
