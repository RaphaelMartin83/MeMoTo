#include <QGridLayout>
#include <QWidget>

#include <Engine/I_ConfigurationContent.h>

#include "ConfigWidget.h"

QWidget* ConfigWidget::m_Me = nullptr;
QGridLayout* ConfigWidget::m_Layout = nullptr;
QList<I_ConfigurationContent*>* ConfigWidget::m_ContentsStack = new QList<I_ConfigurationContent*>();
bool ConfigWidget::m_isInit = false;

ConfigWidget::ConfigWidget()
{
}

ConfigWidget::~ConfigWidget()
{
}

QWidget& ConfigWidget::getInstance()
{
    if(false == m_isInit)
    {
        m_Layout = new QGridLayout();
        Q_ASSERT(nullptr != m_Layout);
        m_Me = new QWidget();
        Q_ASSERT(nullptr != m_Me);
        m_Me->setLayout(m_Layout);
        m_Me->setVisible(false);

        m_isInit = true;
    }
    return *m_Me;
}
void ConfigWidget::deleteInstance()
{
    if( m_isInit )
    {
        Q_ASSERT(nullptr != m_ContentsStack);
        delete m_ContentsStack;
        m_ContentsStack = nullptr;

        Q_ASSERT(nullptr != m_Layout);
        delete m_Layout;
        m_Layout = nullptr;

        m_isInit = false;
    }
}

void ConfigWidget::open(I_ConfigurationContent* p_Widget)
{
    Q_ASSERT(nullptr != p_Widget);

    // Check if configuration is already open
    bool l_isAlreadyOpened = false;

    Q_ASSERT(nullptr != m_ContentsStack);
    for( unsigned short i_contents = 0U;
         (i_contents < m_ContentsStack->count()) && (false == l_isAlreadyOpened);
         i_contents++ )
    {
        if( p_Widget == m_ContentsStack->at(i_contents) )
        {
            l_isAlreadyOpened = true;
        }
    }

    if( false == l_isAlreadyOpened )
    {
        if( 0 != m_ContentsStack->count() )
        {
            Q_ASSERT(nullptr != m_ContentsStack->last());
            m_ContentsStack->last()->setVisible(false);

            Q_ASSERT(nullptr != m_Layout);
            m_Layout->removeWidget(m_ContentsStack->last());
        }

        ConfigWidget::getInstance().resize(p_Widget->getWidth(),
                                           ConfigWidget::getInstance().height());
        Q_ASSERT(nullptr != m_ContentsStack);
        m_ContentsStack->append(p_Widget);

        Q_ASSERT(nullptr != m_Me);
        m_Me->setVisible(true);

        ConfigWidget::openAgain(p_Widget);
    }
}

void ConfigWidget::openAgain(I_ConfigurationContent* p_Widget)
{
    Q_ASSERT(nullptr != m_Layout);
    m_Layout->addWidget(p_Widget);

    Q_ASSERT(nullptr != m_ContentsStack);
    Q_ASSERT(nullptr != m_ContentsStack->last());
    m_ContentsStack->last()->setVisible(true);
    m_ContentsStack->last()->setFocusOnFirstZone();
}

void ConfigWidget::close()
{
    Q_ASSERT(nullptr != m_ContentsStack);
    if( 0 != m_ContentsStack->count() )
    {
        Q_ASSERT(nullptr != m_ContentsStack->last());
        Q_ASSERT(nullptr != m_Layout);
        m_Layout->removeWidget(m_ContentsStack->last());
        m_ContentsStack->pop_back();
    }

    if( 0 != m_ContentsStack->count() )
    {
        ConfigWidget::openAgain(m_ContentsStack->last());
    }
    else
    {
        if( nullptr != m_Me )
        {
            m_Me->setVisible(false);
        }
    }
}
