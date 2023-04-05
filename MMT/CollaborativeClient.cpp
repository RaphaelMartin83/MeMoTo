#include "CollaborativeClient.h"

#include "MeMoToApplication.h"

CollaborativeClient::CollaborativeClient():
    QTcpSocket()
{
    connect(this, &QTcpSocket::disconnected, this, &CollaborativeClient::serverDisconnected);
    connect(this, &QTcpSocket::readyRead, this, &CollaborativeClient::dataReady);
}

void CollaborativeClient::start(const QString& p_Host, const quint16& p_Port)
{
    this->connectToHost(p_Host, p_Port);
}
void CollaborativeClient::stop()
{
    this->close();
}
void CollaborativeClient::updateData(const QByteArray& p_Data)
{
    this->write(p_Data);
}
void CollaborativeClient::serverDisconnected()
{
    this->close();
}
void CollaborativeClient::dataReady()
{
    m_Listener->dataChanged(this->readAll());
}

void CollaborativeClient::registerListener(I_ConnectionListener* p_Listener)
{
    m_Listener = p_Listener;
}
