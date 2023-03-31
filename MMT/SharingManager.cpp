#include <QtSql/QSqlQuery>
#include <QtSql/QSqlResult>
#include <QtSql/QSqlError>
#include <QJsonObject>
#include <QFile>

#include "ConfigWidget.h"

#include "SharingManager.h"
#include "SharingConfiguration.h"
#include "MeMoToLoader.h"

static SharingConfiguration* s_ConfigurationContext = nullptr;

static const int POLL_PERIOD = 500;

SharingManager* SharingManager::m_Me = nullptr;

SharingManager::SharingManager():
    m_DataBaseHandle(QSqlDatabase::addDatabase("QSQLITE")),
    m_Timer(this),
    m_isInited(false),
    m_Manager(nullptr),
    m_isDatatoPush(false),
    m_DataBaseRequestsThread(),
    m_DataBaseRequester(m_DataBaseHandle),
    m_ApplicationData()
{
    m_DataBaseRequester.moveToThread(&m_DataBaseRequestsThread);
    connect(&m_Timer, &QTimer::timeout, this, &SharingManager::pollEvents);
    connect(this, &SharingManager::getData, &m_DataBaseRequester, &DataBaseRequester::retrieveData);
    connect(&m_DataBaseRequester, &DataBaseRequester::dataRetrieved, this, &SharingManager::dataRetrieved);
    m_DataBaseRequestsThread.start();
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
    ConfigWidget::open(s_ConfigurationContext);
}

void SharingManager::sharingPlaceSelected(const QString& p_Place)
{
    ConfigWidget::close();

    QFile l_File(p_Place);
    m_DataBaseHandle.setDatabaseName(p_Place);
    if( !l_File.exists() )
    {
        this->initDB();
    }

    m_Manager->getApplicationData(m_ApplicationData);
    m_isInited = true;

    m_Timer.start(POLL_PERIOD);
}
void SharingManager::sharingCanceled()
{
    ConfigWidget::close();
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
    QSqlQuery l_Query(m_DataBaseHandle);
    if( p_first )
    {
        l_Query.exec("insert into dataTable (data) values ('" + l_JSonText + "')");
    }
    else
    {
        l_Query.exec("update dataTable set data = '" + l_JSonText + "'");
    }
    m_DataBaseHandle.close();
}

void SharingManager::dataRetrieved(QSqlQuery& p_Query)
{
    if(p_Query.lastError().type() == QSqlError::NoError)
    {
        p_Query.next();
        QByteArray l_Array = p_Query.value(0).toByteArray();
        QJsonObject l_DataFromDB = MeMoToLoader::loadFromArray(l_Array);

        if( l_DataFromDB != m_ApplicationData )
        {
            m_Manager->setApplicationData(l_DataFromDB);
            m_ApplicationData = l_DataFromDB;
        }
    }
}

void SharingManager::pollEvents()
{
    if(m_isDatatoPush)
    {
        m_isDatatoPush = false;
        m_Manager->getApplicationData(m_ApplicationData);
        this->setData(m_ApplicationData);
    }
    else
    {
        emit getData();
    }
}

void SharingManager::pushModifications()
{
    if(!m_isInited) {return;}

    m_isDatatoPush = true;
}

void SharingManager::registerDataManager(I_DataManager* p_Manager)
{
    m_Manager = p_Manager;
}
