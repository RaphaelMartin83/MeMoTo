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
    Q_ASSERT(nullptr != m_Layout);
    Q_ASSERT(nullptr != p_Widget);
    Q_ASSERT(nullptr != m_ContentsStack);

    if( m_Layout->widget() != p_Widget )
    {
        if( 0 != m_ContentsStack->count() )
        {
            ConfigWidget::hideTopWidget();
        }

        ConfigWidget::getInstance().resize(p_Widget->getWidth(),
                                           ConfigWidget::getInstance().height());
        Q_ASSERT(nullptr != m_ContentsStack);
        m_ContentsStack->append(p_Widget);

        ConfigWidget::displayTopWidget();
    }
}

void ConfigWidget::displayTopWidget()
{
    Q_ASSERT(nullptr != m_Layout);
    Q_ASSERT(0 == m_Layout->count());
    Q_ASSERT(nullptr != m_ContentsStack);
    Q_ASSERT(nullptr != m_ContentsStack->last());
    Q_ASSERT(0 < m_ContentsStack->count());

    m_Layout->addWidget(m_ContentsStack->last());

    m_ContentsStack->last()->setVisible(true);
    m_ContentsStack->last()->setFocusOnFirstZone();

    Q_ASSERT(nullptr != m_Me);
    m_Me->setVisible(true);
}

void ConfigWidget::hideTopWidget()
{
    Q_ASSERT(nullptr != m_ContentsStack->last());
    Q_ASSERT(nullptr != m_Layout);
    Q_ASSERT(0 != m_Layout->count());
    Q_ASSERT(0 < m_ContentsStack->count());

	m_ContentsStack->last()->setVisible(false);
	m_Layout->removeWidget(m_ContentsStack->last());
}

void ConfigWidget::close()
{
    Q_ASSERT(nullptr != m_ContentsStack);
    if( 0 != m_ContentsStack->count() )
    {
        ConfigWidget::hideTopWidget();

        Q_ASSERT(nullptr != m_ContentsStack->last());
        m_ContentsStack->pop_back();
    }

    if( 0 != m_ContentsStack->count() )
    {
        ConfigWidget::displayTopWidget();
    }
    else
    {
        if( nullptr != m_Me )
        {
            m_Me->setVisible(false);
        }
    }
}
