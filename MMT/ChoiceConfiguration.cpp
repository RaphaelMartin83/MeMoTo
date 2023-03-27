#include "ChoiceConfiguration.h"

ChoiceConfiguration::ChoiceConfiguration()
{
    // Init widgets
    m_OKButton = new QPushButton("OK");
    m_CancelButton = new QPushButton("Cancel");

    m_NameLabel = new QLabel("Name:");
    m_NameLabel->setFixedHeight(30);
    m_NameEdit = new ConfigLineEdit(static_cast<I_ConfigLineEditListener*>(this));

    m_EmptySpace = new QWidget();

    m_Layout = new QGridLayout();

    // Setup layout
    m_Layout->addWidget(m_NameLabel, 0, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_NameEdit, 1, 0, 1, -1, Qt::Alignment());

    m_Layout->addWidget(m_CancelButton, 2, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_OKButton, 2, 1, 1, 1, Qt::Alignment());

    m_Layout->addWidget(m_EmptySpace, 4, 0, -1, -1, Qt::Alignment());

    this->setVisible(false);

    this->setLayout(m_Layout);

    // Setup events
    connect(m_OKButton, &QPushButton::clicked, this, &ChoiceConfiguration::OKPressed);
    connect(m_CancelButton, &QPushButton::clicked, this, &ChoiceConfiguration::CancelPressed);
}

QString ChoiceConfiguration::getName() const
{
    return m_NameEdit->text();
}
void ChoiceConfiguration::setName(const QString& p_Name)
{
    m_NameEdit->setText(p_Name);
}

void ChoiceConfiguration::setFocusOnFirstZone()
{
    m_NameEdit->setFocus();
    m_NameEdit->selectAll();
}

void ChoiceConfiguration::enterPressed()
{
    this->OKPressed();
}

void ChoiceConfiguration::escapePressed()
{
    this->CancelPressed();
}

// Slots
void ChoiceConfiguration::OKPressed()
{
    I_Configurable* l_Listener = this->getListener();
    if( nullptr != l_Listener)
    {
        l_Listener->applyConfiguration();
    }
}

void ChoiceConfiguration::CancelPressed()
{
    I_Configurable* l_Listener = this->getListener();
    if( nullptr != l_Listener)
    {
        l_Listener->closeConfiguration();
    }
}
