#include <QtSql/QSqlQuery>
#include <QtSql/QSqlResult>
#include <QtSql/QSqlError>
#include <QJsonObject>
#include <QNetworkInterface>

#include <MeMoToApplication.h>

#include <Engine/MeMoToLoader.h>
#include <Engine/ErrorDisplayer.h>

#include <CommonGraphics/ConfigWidget.h>
#include <ConfigurationContexts/SharingConfiguration.h>
#include <Sharing/SharingManager.h>

static SharingConfiguration* s_ConfigurationContext = nullptr;

static const int POLL_PERIOD = 100;

SharingManager* SharingManager::m_Me = nullptr;

SharingManager::SharingManager():
    m_isInited(false),
    m_Server(nullptr),
    m_Client(nullptr)
{
}

SharingManager& SharingManager::getInstance()
{
    if( nullptr == m_Me )
    {
        m_Me = new SharingManager();
    }
    return *m_Me;
}

void SharingManager::start()
{
    if( nullptr == s_ConfigurationContext )
    {
        s_ConfigurationContext = new SharingConfiguration();
        s_ConfigurationContext->registerConfigListener(m_Me);
    }
    s_ConfigurationContext->setHostName("");
    s_ConfigurationContext->setHostPort(MeMoToApplication::getCollaborativePort());

    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    QHostAddress l_myAddress;
#ifndef __EMSCRIPTEN__
    for( const QHostAddress &i_address: QNetworkInterface::allAddresses() )
    {
        if( (i_address.protocol() == QAbstractSocket::IPv4Protocol) && (i_address != localhost) )
        {
            l_myAddress = i_address;
        }
    }
#endif
    s_ConfigurationContext->setSessionAddress(l_myAddress);
    s_ConfigurationContext->setSessionPort(MeMoToApplication::getCollaborativePort());
    s_ConfigurationContext->setRunning(m_isInited);
    ConfigWidget::open(s_ConfigurationContext);
}

void SharingManager::start(const QString& p_Place, const quint16& p_Port)
{
    m_Server = new CollaborativeServer();
    m_Server->registerListener(this);
    m_Server->start(p_Place, p_Port);
    m_isInited = true;
}

void SharingManager::sharingHostSelected(const QString& p_Place,
                                  const quint16& p_Port)
{
    if( (nullptr == m_Server) && (nullptr == m_Client) )
    {
        m_Client = new CollaborativeClient();
        m_Client->registerListener(this);
        m_Client->start(p_Place, p_Port);
        ConfigWidget::close();
        m_isInited = true;
    }
}
void SharingManager::sharingSessionCreationSelected(const quint16& p_Port)
{
    if( (nullptr == m_Server) && (nullptr == m_Client) )
    {
        m_Server = new CollaborativeServer();
        m_Server->registerListener(this);
        m_Server->start(s_ConfigurationContext->getSessionAddress(), p_Port);
        ConfigWidget::close();
        m_isInited = true;
    }
}
void SharingManager::sharingStopped()
{
    if( nullptr != m_Server )
    {
        m_Server->close();
        delete m_Server;
        m_Server = nullptr;
    }

    if( nullptr != m_Client )
    {
        m_Client->close();
        delete m_Client;
        m_Client = nullptr;
    }
    m_isInited = false;

    ConfigWidget::close();
}
void SharingManager::sharingCanceled()
{
    ConfigWidget::close();
}

void SharingManager::dataChanged(const QByteArray& p_Data)
{
    QJsonObject l_Json = MeMoToLoader::loadFromArray(qUncompress(p_Data));
    MeMoToApplication::setApplicationData(l_Json);
}
void SharingManager::newClientConnected()
{
    this->pushModifications();
}
void SharingManager::connectionClosed()
{
    if( nullptr != m_Server )
    {
        m_Server->deleteLater();
        m_Server = nullptr;
    }

    if( nullptr != m_Client )
    {
        m_Client->deleteLater();
        m_Client = nullptr;
    }
    m_isInited = false;
}

void SharingManager::setData(const QJsonObject& p_Data)
{
    QByteArray l_JSonText;
    MeMoToLoader::JsonToArray(p_Data, l_JSonText);
    QByteArray l_CompressedPayload = qCompress(l_JSonText, 2);

    if( nullptr != m_Server )
    {
        m_Server->updateData(l_CompressedPayload);
    }
    if( nullptr != m_Client )
    {
        m_Client->updateData(l_CompressedPayload);
    }
}

void SharingManager::pushModifications()
{
    if(!m_isInited) {return;}

    QJsonObject l_ObjectToWrite;
    MeMoToApplication::getApplicationData(l_ObjectToWrite);
    this->setData(l_ObjectToWrite);
}
