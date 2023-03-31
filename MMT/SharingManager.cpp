#include <QtSql/QSqlQuery>
#include <QtSql/QSqlResult>
#include <QtSql/QSqlError>
#include <QJsonObject>
#include <QFile>

#include "SharingManager.h"
#include "SharingConfiguration.h"
#include "MeMoToLoader.h"
#include "ErrorDisplayer.h"
#include "ConfigWidget.h"

static SharingConfiguration* s_ConfigurationContext = nullptr;

static const int POLL_PERIOD = 500;

SharingManager* SharingManager::m_Me = nullptr;

SharingManager::SharingManager():
    m_DataBaseHandle(QSqlDatabase::addDatabase("QSQLITE")),
    m_FileSystemWatcher(this),
    m_isInited(false),
    m_Manager(nullptr)
{
    connect(&m_FileSystemWatcher, &QFileSystemWatcher::fileChanged, this, &SharingManager::fileChanged);
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
    s_ConfigurationContext->setFileSharingRunning(m_isInited);
    ConfigWidget::open(s_ConfigurationContext);
}

void SharingManager::sharingPlaceSelected(const QString& p_Place)
{
    ConfigWidget::close();

    if( m_isInited )
    {
        // Stop sharing
        m_FileSystemWatcher.removePath(m_DataBaseHandle.databaseName());
        m_isInited = false;
    }
    else
    {
        QFile l_File(p_Place);

        if( false == l_File.open(QIODevice::ReadWrite) )
        {
            ErrorDisplayer::displayError("ERROR: Cannot share!",
                                         "The given file path for sharing does not exist or doesn't have RW permissions.\n"
                                         "Given file path: " + p_Place +"\n");
        }
        else
        {
            if( 0 == l_File.peek(1).size() )
            {
                l_File.close();
                l_File.remove();
            }
            else
            {
                l_File.close();
            }
            m_DataBaseHandle.setDatabaseName(p_Place);
            if( !l_File.exists() )
            {
                this->initDB();
            }
            else
            {
                this->fileChanged();
            }
            m_FileSystemWatcher.addPath(p_Place);
            m_isInited = true;
        }
    }
}

void SharingManager::sharingCanceled()
{
    ConfigWidget::close();
}

void SharingManager::fileChanged()
{
    QSqlQuery l_Query;
    m_DataBaseHandle.open();
    m_FileSystemWatcher.removePath(m_DataBaseHandle.databaseName());
    l_Query.exec("select data from dataTable");
    m_FileSystemWatcher.addPath(m_DataBaseHandle.databaseName());
    m_DataBaseHandle.close();

    if(l_Query.lastError().type() == QSqlError::NoError)
    {
        l_Query.next();
        QByteArray l_Array = l_Query.value(0).toByteArray();
        QJsonObject l_DataFromDB = MeMoToLoader::loadFromArray(l_Array);

        m_Manager->setApplicationData(l_DataFromDB);
    }
}

void SharingManager::initDB()
{
    m_DataBaseHandle.open();
    QSqlQuery l_Query(m_DataBaseHandle);
    l_Query.exec("create table dataTable "
          "(id integer primary key, "
          "data varchar(200000))");
    m_DataBaseHandle.close();
    QJsonObject l_Data;
    m_Manager->getApplicationData(l_Data);
    this->setData(l_Data, true);
}

void SharingManager::setData(const QJsonObject& p_Data, bool p_first)
{
    QByteArray l_JSonText;
    MeMoToLoader::JsonToArray(p_Data, l_JSonText);

    m_DataBaseHandle.open();
    // During set data, deactivate file system monitoring
    m_FileSystemWatcher.removePath(m_DataBaseHandle.databaseName());
    QSqlQuery l_Query(m_DataBaseHandle);
    if( p_first )
    {
        l_Query.exec("insert into dataTable (data) values ('" + l_JSonText + "')");
    }
    else
    {
        l_Query.exec("update dataTable set data = '" + l_JSonText + "'");
    }
    m_FileSystemWatcher.addPath(m_DataBaseHandle.databaseName());
    m_DataBaseHandle.close();
}

void SharingManager::pushModifications()
{
    if(!m_isInited) {return;}

    QJsonObject l_ObjectToWrite;
    m_Manager->getApplicationData(l_ObjectToWrite);
    this->setData(l_ObjectToWrite, false);
}

void SharingManager::registerDataManager(I_DataManager* p_Manager)
{
    m_Manager = p_Manager;
}
