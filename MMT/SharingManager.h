#ifndef SHARINGMANAGER_H
#define SHARINGMANAGER_H

#include "I_DiagramContainer.h"
#include "I_DataManager.h"
#include "CollaborativeServer.h"
#include "CollaborativeClient.h"

#include "I_SharingConfigurationListener.h"
#include "I_ConnectionListener.h"

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

    void pushModifications();

    void registerDataManager(I_DataManager* p_Manager);

    // I_SharingConfigurationListener
    virtual void sharingHostSelected(const QString& p_Place,
                                      const quint16& p_Port);
    virtual void sharingSessionCreationSelected(const quint16& p_Port);
    virtual void sharingStopped();
    virtual void sharingCanceled();

    // I_ConnectionListener
    void dataChanged(const QByteArray& p_Data);

private:
    void setData(const QJsonObject& p_Data);

    static SharingManager* m_Me;
    bool m_isInited;
    bool m_isDatatoPush;

    I_DataManager* m_Manager;

    CollaborativeServer* m_Server;
    CollaborativeClient* m_Client;
};

#endif // SHARINGMANAGER_H
