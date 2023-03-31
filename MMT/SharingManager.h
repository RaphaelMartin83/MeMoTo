#ifndef SHARINGMANAGER_H
#define SHARINGMANAGER_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QTimer>
#include <QMutex>
#include <QThread>

#include "I_DiagramContainer.h"
#include "I_DataManager.h"

#include "I_SharingConfigurationListener.h"

class DataBaseRequester: public QObject
{
    Q_OBJECT

public:
    DataBaseRequester(QSqlDatabase& p_Database):
        m_Database(p_Database)
        , m_Query(p_Database)
    {

    }
public slots:
    void retrieveData()
    {
        QString l_Query("select data from dataTable");

        m_Database.open();
        m_Query.exec(l_Query);
        m_Database.close();

        emit dataRetrieved(m_Query);
    }
signals:
    void dataRetrieved(QSqlQuery& p_Query);

private:
    QSqlDatabase& m_Database;
    QSqlQuery m_Query;
};

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
    void pollEvents();

signals:
    void getData();

private:
    void initDB();
    void setData(const QJsonObject& p_Data, bool p_first = false);
    void dataRetrieved(QSqlQuery& p_Query);

    QSqlDatabase m_DataBaseHandle;
    QTimer m_Timer;

    static SharingManager* m_Me;
    bool m_isInited;
    bool m_isDatatoPush;

    QThread m_DataBaseRequestsThread;
    DataBaseRequester m_DataBaseRequester;

    QJsonObject m_ApplicationData;

    I_DataManager* m_Manager;
};

#endif // SHARINGMANAGER_H
