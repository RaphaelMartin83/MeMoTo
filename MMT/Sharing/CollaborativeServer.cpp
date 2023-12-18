#include <MeMoToApplication.h>

#include "CollaborativeServer.h"

CollaborativeServer::CollaborativeServer():
    QWebSocketServer(QString("MeMoToServer"), SslMode::NonSecureMode)
  , m_Clients()
  , m_Socket()
  , m_Listener(nullptr)
  , m_CurrentSessionData()
{
    connect(this, &QWebSocketServer::newConnection, this, &CollaborativeServer::newClientConnected);
}

CollaborativeServer::~CollaborativeServer()
{
    while( 0 != m_Clients.count() )
    {
        QWebSocket* l_Socket = m_Clients.takeAt(0);
        if( nullptr != l_Socket )
        {
            l_Socket->close();
            l_Socket->deleteLater();
        }
    }
}

void CollaborativeServer::registerListener(I_ConnectionListener* p_Listener)
{
    m_Listener = p_Listener;
}

void CollaborativeServer::newClientConnected()
{
    QWebSocket* l_newClient = this->nextPendingConnection();
    while(nullptr != l_newClient)
    {
        m_Clients.append(l_newClient);
        connect(l_newClient, &QWebSocket::disconnected, this, &CollaborativeServer::clientDisconnected);
        connect(l_newClient, &QWebSocket::binaryMessageReceived, this, &CollaborativeServer::dataReady);
        l_newClient = this->nextPendingConnection();
    }
    m_Listener->newClientConnected();
}
void CollaborativeServer::start(const QString& p_Host, const quint16& p_Port)
{
    QHostAddress l_Address(p_Host);
    this->listen(l_Address, p_Port);
}
void CollaborativeServer::stop()
{
    this->close();
}
void CollaborativeServer::updateData(const QByteArray& p_Data)
{
    m_CurrentSessionData = p_Data;
    for( unsigned int i_clients = 0U; i_clients < m_Clients.count(); i_clients++ )
    {
        QWebSocket* l_cclient = m_Clients[i_clients];
        l_cclient->sendBinaryMessage(p_Data);
    }
}
void CollaborativeServer::clientDisconnected()
{
    for( unsigned int i_clients = 0U; i_clients < m_Clients.count(); i_clients++ )
    {
        QWebSocket* l_cclient = m_Clients[i_clients];
        if( QAbstractSocket::SocketState::ConnectedState!= l_cclient->state() )
        {
            disconnect(l_cclient, &QWebSocket::disconnected, this, &CollaborativeServer::clientDisconnected);
            disconnect(l_cclient, &QWebSocket::binaryMessageReceived, this, &CollaborativeServer::dataReady);
            l_cclient->close();
            l_cclient->deleteLater();
            m_Clients.removeAt(i_clients);
        }
    }
}
void CollaborativeServer::dataReady(QByteArray p_Message)
{
    for( unsigned int i_clients = 0U; i_clients < m_Clients.count(); i_clients++ )
    {
        if( nullptr != m_Listener )
        {
            m_Listener->dataChanged(p_Message);
        }

        // Also send to all others
        for( unsigned int i_others = 0U; i_others < m_Clients.count(); i_others++ )
        {
            if( i_clients != i_others )
            {
                QWebSocket* l_cother = m_Clients[i_others];
                l_cother->sendBinaryMessage(p_Message);
            }
        }
    }
}
