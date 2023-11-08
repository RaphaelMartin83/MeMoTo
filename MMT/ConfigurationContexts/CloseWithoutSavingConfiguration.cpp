#include "CloseWithoutSavingConfiguration.h"

CloseWithoutSavingConfiguration::CloseWithoutSavingConfiguration()
{
    // Init widgets
    m_AreYouSureLabel = new QLabel("You have unsaved changes!");

    m_SaveButton = new QPushButton("Save");
    m_CloseButton = new QPushButton("Close anyway");

    m_Layout = new QGridLayout();

    // Setup Layout
    unsigned short i_row = 0U;
    m_Layout->addWidget(m_AreYouSureLabel, i_row++, 0);
    m_Layout->addWidget(m_SaveButton, i_row++, 0);
    m_Layout->addWidget(m_CloseButton, i_row++, 0);

    this->setVisible(false);
    this->setLayout(m_Layout);

    connect(m_SaveButton, &QPushButton::clicked,
            this, &CloseWithoutSavingConfiguration::savePressed);
    connect(m_CloseButton, &QPushButton::clicked,
            this, &CloseWithoutSavingConfiguration::closeAnywayPressed);
}

void CloseWithoutSavingConfiguration::setFocusOnFirstZone()
{
    m_SaveButton->setFocus();
}
void CloseWithoutSavingConfiguration::OKPressed()
{
    this->savePressed();
}

void CloseWithoutSavingConfiguration::registerListener(
    I_CloseWithoutSavingConfigurationListener* p_Listener)
{
    m_Listener = p_Listener;
}

// Slots
void CloseWithoutSavingConfiguration::savePressed()
{
    if( nullptr != m_Listener)
    {
        m_Listener->saveBeforeClosing();
    }
}
void CloseWithoutSavingConfiguration::closeAnywayPressed()
{
    if( nullptr != m_Listener)
    {
        m_Listener->closeAndDropChanges();
    }
}
