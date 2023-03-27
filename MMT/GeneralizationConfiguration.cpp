#include "GeneralizationConfiguration.h"

GeneralizationConfiguration::GeneralizationConfiguration()
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
            &GeneralizationConfiguration::OKPressed);
    connect(m_CancelButton, &QPushButton::clicked, this,
            &GeneralizationConfiguration::CancelPressed);
}

bool GeneralizationConfiguration::getAutoRoute() const
{
    return m_AutoRouteCheckBox->isChecked();
}
void GeneralizationConfiguration::setAutoRoute(bool p_isAutoRoute)
{
    m_AutoRouteCheckBox->setChecked(p_isAutoRoute);
}

// I_ConfigurationContent
void GeneralizationConfiguration::setFocusOnFirstZone()
{
    m_AutoRouteCheckBox->setFocus();
}

// Slots
void GeneralizationConfiguration::OKPressed()
{
    I_Configurable* l_Listener = this->getListener();
    if( nullptr != l_Listener)
    {
        l_Listener->applyConfiguration();
    }
}
void GeneralizationConfiguration::CancelPressed()
{
    I_Configurable* l_Listener = this->getListener();
    if( nullptr != l_Listener)
    {
        l_Listener->closeConfiguration();
    }
}
