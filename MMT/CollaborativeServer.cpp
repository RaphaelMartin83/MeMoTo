#include "CollaborativeServer.h"

CollaborativeServer::CollaborativeServer():
    QTcpServer()
  , m_Clients()
  , m_Socket()
  , m_Listener(nullptr)
  , m_CurrentSessionData()
{
    connect(this, &QTcpServer::newConnection, this, &CollaborativeServer::newClientConnected);
}

void CollaborativeServer::registerListener(I_ConnectionListener* p_Listener)
{
    m_Listener = p_Listener;
}

void CollaborativeServer::newClientConnected()
{
    QTcpSocket* l_newClient = this->nextPendingConnection();
    while(nullptr != l_newClient)
    {
        m_Clients.append(l_newClient);
        connect(l_newClient, &QTcpSocket::disconnected, this, &CollaborativeServer::clientDisconnected);
        connect(l_newClient, &QTcpSocket::readyRead, this, &CollaborativeServer::dataReady);
        l_newClient->write(m_CurrentSessionData);
        l_newClient = this->nextPendingConnection();
    }
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
        QTcpSocket* l_cclient = m_Clients[i_clients];
        l_cclient->write(p_Data);
    }
}
void CollaborativeServer::clientDisconnected()
{
    for( unsigned int i_clients = 0U; i_clients < m_Clients.count(); i_clients++ )
    {
        QTcpSocket* l_cclient = m_Clients[i_clients];
        if( QAbstractSocket::SocketState::ConnectedState!= l_cclient->state() )
        {
            disconnect(l_cclient, &QTcpSocket::disconnected, this, &CollaborativeServer::clientDisconnected);
            disconnect(l_cclient, &QTcpSocket::readyRead, this, &CollaborativeServer::dataReady);
            l_cclient->close();
            m_Clients.removeAt(i_clients);
        }
    }
}
void CollaborativeServer::dataReady()
{
    for( unsigned int i_clients = 0U; i_clients < m_Clients.count(); i_clients++ )
    {
        // Find the sender
        QTcpSocket* l_cclient = m_Clients[i_clients];
        if( 0 != l_cclient->bytesAvailable() )
        {
            QByteArray l_Data = l_cclient->readAll();
            // Sender found, give to listener
            if( nullptr != m_Listener )
            {
                m_Listener->dataChanged(l_Data);
            }

            // Also send to all others
            for( unsigned int i_others = 0U; i_others < m_Clients.count(); i_others++ )
            {
                if( i_clients != i_others )
                {
                    QTcpSocket* l_cother = m_Clients[i_others];
                    l_cother->write(l_Data);
                }
            }
        }
    }
}
