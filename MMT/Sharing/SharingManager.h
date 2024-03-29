#ifndef SHARINGMANAGER_H
#define SHARINGMANAGER_H

#include <Engine/I_DiagramContainer.h>
#include <Engine/I_ConnectionListener.h>

#include <Sharing/CollaborativeServer.h>
#include <Sharing/CollaborativeClient.h>
#include <Sharing/I_SharingConfigurationListener.h>

class SharingManager:
        public QObject,
        public I_SharingConfigurationListener,
        public I_ConnectionListener
{
    Q_OBJECT
private:
    SharingManager();
public:
    static SharingManager& getInstance();

    void start();
    void start(const QString& p_Place, const quint16& p_Port);

    void pushModifications();

    // I_SharingConfigurationListener
    virtual void sharingHostSelected(const QString& p_Place,
                                      const quint16& p_Port);
    virtual void sharingSessionCreationSelected(const quint16& p_Port);
    virtual void sharingStopped();
    virtual void sharingCanceled();

    // I_ConnectionListener
    void dataChanged(const QByteArray& p_Data);
    void newClientConnected();
    void connectionClosed();

private:
    void setData(const QJsonObject& p_Data);

    static SharingManager* m_Me;
    bool m_isInited;
    bool m_isDatatoPush;

    CollaborativeServer* m_Server;
    CollaborativeClient* m_Client;
};

#endif // SHARINGMANAGER_H
