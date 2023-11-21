#include <MeMoToApplication.h>

#include <Engine/ErrorDisplayer.h>
#include <Sharing/CollaborativeClient.h>

CollaborativeClient::CollaborativeClient():
    QWebSocket()
{
    connect(this, &QWebSocket::disconnected, this, &CollaborativeClient::serverDisconnected);
    connect(this, &QWebSocket::binaryMessageReceived, this, &CollaborativeClient::dataReady);
    connect(this, &QWebSocket::errorOccurred, this, &CollaborativeClient::errorOccurred);
}

CollaborativeClient::~CollaborativeClient()
{
    disconnect(this, &QWebSocket::disconnected, this, &CollaborativeClient::serverDisconnected);
    disconnect(this, &QWebSocket::binaryMessageReceived, this, &CollaborativeClient::dataReady);
    disconnect(this, &QWebSocket::errorOccurred, this, &CollaborativeClient::errorOccurred);
}

void CollaborativeClient::start(const QString& p_Host, const quint16& p_Port)
{
    QUrl l_URL("ws://" + p_Host + ":" + QString::number(p_Port));
    this->open(l_URL);
}
void CollaborativeClient::stop()
{
    this->close();
}
void CollaborativeClient::updateData(const QByteArray& p_Data)
{
    this->sendBinaryMessage(p_Data);
}
void CollaborativeClient::serverDisconnected()
{
    this->close();
}
void CollaborativeClient::dataReady(QByteArray p_Message)
{
    m_Listener->dataChanged(p_Message);
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
