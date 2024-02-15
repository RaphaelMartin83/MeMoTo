#include "ImplementationConfiguration.h"

ImplementationConfiguration::ImplementationConfiguration()
{
    // Init widgets
    m_AutoRouteLabel = new QLabel("Auto route:");
    m_AutoRouteCheckBox = new QCheckBox();
    m_OKButton = new QPushButton("OK");
    m_CancelButton = new QPushButton("Cancel");

    m_emptySpace = new QWidget();

    m_Layout = new QGridLayout();

    // Setup layout
    unsigned short l_row = 0U;

    m_Layout->addWidget(m_AutoRouteLabel, l_row, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_AutoRouteCheckBox, l_row++, 1, 1, 1, Qt::Alignment());

    m_Layout->addWidget(m_CancelButton, l_row, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_OKButton, l_row++, 1, 1, 1, Qt::Alignment());

    m_Layout->addWidget(m_emptySpace, l_row, 1, 1, 1, Qt::Alignment());

    this->setVisible(false);

    this->setLayout(m_Layout);

    // Setup events
    connect(m_OKButton, &QPushButton::clicked, this,
            &ImplementationConfiguration::OKPressed);
    connect(m_CancelButton, &QPushButton::clicked, this,
            &ImplementationConfiguration::CancelPressed);
}

bool ImplementationConfiguration::getAutoRoute() const
{
    return m_AutoRouteCheckBox->isChecked();
}
void ImplementationConfiguration::setAutoRoute(bool p_isAutoRoute)
{
    m_AutoRouteCheckBox->setChecked(p_isAutoRoute);
}

// I_ConfigurationContent
void ImplementationConfiguration::setFocusOnFirstZone()
{
    m_AutoRouteCheckBox->setFocus();
}

// Slots
void ImplementationConfiguration::OKPressed()
{
    I_Configurable* l_Listener = this->getListener();
    if( nullptr != l_Listener)
    {
        l_Listener->applyConfiguration();
    }
}
void ImplementationConfiguration::CancelPressed()
{
    I_Configurable* l_Listener = this->getListener();
    if( nullptr != l_Listener)
    {
        l_Listener->closeConfiguration();
    }
}
