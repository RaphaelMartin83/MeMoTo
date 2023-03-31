#ifndef SHARINGMANAGER_H
#define SHARINGMANAGER_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QFileSystemWatcher>

#include "I_DiagramContainer.h"
#include "I_DataManager.h"

#include "I_SharingConfigurationListener.h"

class SharingManager:
        public QObject,
        public I_SharingConfigurationListener
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
    void sharingPlaceSelected(const QString& p_Place);
    void sharingCanceled();

public slots:
    void fileChanged();

private:
    void initDB();
    void setData(const QJsonObject& p_Data, bool p_first = false);

    QSqlDatabase m_DataBaseHandle;
    QFileSystemWatcher m_FileSystemWatcher;

    static SharingManager* m_Me;
    bool m_isInited;
    bool m_isDatatoPush;

    I_DataManager* m_Manager;
};

#endif // SHARINGMANAGER_H
