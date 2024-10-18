#ifndef I_CONFIGURABLE_H
#define I_CONFIGURABLE_H

#include <Engine/I_ConfigurationContent.h>
#include <CommonGraphics/ConfigWidget.h>

class I_Configurable
{
private:
    template <typename SPECIALIZED_CONFIGURATION> void createConfig()
    {
        Q_ASSERT(nullptr == m_ConfigContent);

        m_ConfigContent = new SPECIALIZED_CONFIGURATION();

        Q_ASSERT(nullptr != m_ConfigContent);
    }
public:
    template <typename SPECIALIZED_CONFIGURATION> SPECIALIZED_CONFIGURATION& getConfig()
    {
        if( nullptr == m_ConfigContent)
        {
            this->createConfig<SPECIALIZED_CONFIGURATION>();
        }

        // Downcasting and checking if create and get have been called with different templates
        SPECIALIZED_CONFIGURATION* specializedConfig = dynamic_cast<SPECIALIZED_CONFIGURATION*>(m_ConfigContent);
        Q_ASSERT(nullptr != specializedConfig);

        return *specializedConfig;
    }
    virtual void openConfiguration() = 0;
    virtual void applyConfiguration() = 0;
    virtual void closeConfiguration()
    {
        if( nullptr != m_ConfigContent)
        {
            ConfigWidget::close();

            m_ConfigContent->close();
            delete m_ConfigContent;
            m_ConfigContent = nullptr;
        }

        Q_ASSERT(nullptr == m_ConfigContent);
    }

private:
    I_ConfigurationContent* m_ConfigContent = nullptr;
};

#endif // I_CONFIGURABLE_H
