#include "CollaborativeClient.h"

#include "MeMoToApplication.h"
#include "ErrorDisplayer.h"

CollaborativeClient::CollaborativeClient():
    QTcpSocket()
{
    connect(this, &QTcpSocket::disconnected, this, &CollaborativeClient::serverDisconnected);
    connect(this, &QTcpSocket::readyRead, this, &CollaborativeClient::dataReady);
    connect(this, &QTcpSocket::errorOccurred, this, &CollaborativeClient::errorOccurred);
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
void CollaborativeClient::errorOccurred()
{
    ErrorDisplayer::displayError("Connection error", this->errorString());
    this->close();
    m_Listener->connectionClosed();
}

void CollaborativeClient::registerListener(I_ConnectionListener* p_Listener)
{
    m_Listener = p_Listener;
}
