#include "TransitionConfiguration.h"

TransitionConfiguration::TransitionConfiguration()
{
    // Init Widgets
    m_OKButton = new QPushButton("OK");
    m_CancelButton = new QPushButton("Cancel");

    m_EventLabel = new QLabel("Event:");
    m_EventLabel->setFixedHeight(30);
    m_EventEdit = new ConfigLineEdit(this);

    m_ActionLabel = new QLabel("Action:");
    m_ActionLabel->setFixedHeight(30);
    m_ActionEdit = new ConfigLineEdit(this);

    m_GuardLabel = new QLabel("Guard:");
    m_GuardLabel->setFixedHeight(30);
    m_GuardEdit = new ConfigLineEdit(this);

    m_StereotypeLabel = new QLabel("Stereotype:");
    m_StereotypeLabel->setFixedHeight(30);
    m_StereotypeEdit = new ConfigLineEdit(this);

    m_AutoRouteLabel = new QLabel("Auto route:");
    m_AutoRouteCheckBox = new QCheckBox();

    m_CurvedLabel = new QLabel("Curved");
    m_CurvedCheckBox = new QCheckBox();

    m_ConnectFromLabel = new QLabel();
    m_ConnectFromLabel->setFixedHeight(40);
    m_ConnectToLabel = new QLabel();
    m_ConnectToLabel->setFixedHeight(40);

    m_emptySpace = new QWidget();

    m_Layout = new QGridLayout();

    // Setup layout
    unsigned short l_row = 0U;
    m_Layout->addWidget(m_EventLabel, l_row++, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_EventEdit, l_row++, 0, 1, -1, Qt::Alignment());

    m_Layout->addWidget(m_ActionLabel, l_row++, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_ActionEdit, l_row++, 0, 1, -1, Qt::Alignment());

    m_Layout->addWidget(m_GuardLabel, l_row++, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_GuardEdit, l_row++, 0, 1, -1, Qt::Alignment());

    m_Layout->addWidget(m_StereotypeLabel, l_row++, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_StereotypeEdit, l_row++, 0, 1, -1, Qt::Alignment());

    m_Layout->addWidget(m_AutoRouteLabel, l_row, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_AutoRouteCheckBox, l_row++, 1, 1, 1, Qt::Alignment());

    m_Layout->addWidget(m_CurvedLabel, l_row, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_CurvedCheckBox, l_row++, 1, 1, 1, Qt::Alignment());

    m_Layout->addWidget(m_ConnectFromLabel, l_row++, -1, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_ConnectToLabel, l_row++, -1, 1, 1, Qt::Alignment());

    m_Layout->addWidget(m_CancelButton, l_row, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_OKButton, l_row++, 1, 1, 1, Qt::Alignment());

    m_Layout->addWidget(m_emptySpace, l_row, 0, -1, -1, Qt::Alignment());

    this->setVisible(false);

    this->setLayout(m_Layout);

    // Setup events
    connect(m_OKButton, &QPushButton::clicked, this, &TransitionConfiguration::OKPressed);
    connect(m_CancelButton, &QPushButton::clicked, this, &TransitionConfiguration::CancelPressed);
}

QString TransitionConfiguration::getEvent() const
{
    return m_EventEdit->text();
}
void TransitionConfiguration::setEvent(const QString& p_Event)
{
    m_EventEdit->setText(p_Event);
}

QString TransitionConfiguration::getAction() const
{
    return m_ActionEdit->text();
}
void TransitionConfiguration::setAction(const QString& p_Action)
{
    m_ActionEdit->setText(p_Action);
}

QString TransitionConfiguration::getGuard() const
{
    return m_GuardEdit->text();
}
void TransitionConfiguration::setGuard(const QString& p_Guard)
{
    m_GuardEdit->setText(p_Guard);
}

QString TransitionConfiguration::getStereotype() const
{
    return m_StereotypeEdit->text();
}
void TransitionConfiguration::setStereotype(const QString& p_Stereotype)
{
    m_StereotypeEdit->setText(p_Stereotype);
}
bool TransitionConfiguration::getAutoRoute() const
{
    return m_AutoRouteCheckBox->isChecked();
}
void TransitionConfiguration::setAutoRoute(bool p_isAutoRoute)
{
    m_AutoRouteCheckBox->setChecked(p_isAutoRoute);
}
void TransitionConfiguration::setFromInfo(QString p_FromType, QString p_FromName)
{
    m_ConnectFromLabel->setText("Connects " + p_FromType + ":\n" + p_FromName);
}
void TransitionConfiguration::setToInfo(QString p_ToType, QString p_ToName)
{
    m_ConnectToLabel->setText("To " + p_ToType + ":\n" + p_ToName);
}
void TransitionConfiguration::setCurvedInfo(bool p_Curved)
{
    m_CurvedCheckBox->setChecked(p_Curved);
}
bool TransitionConfiguration::getCurvedInfo() const
{
    return m_CurvedCheckBox->isChecked();
}

void TransitionConfiguration::setFocusOnFirstZone()
{
    m_EventEdit->setFocus();
    m_EventEdit->selectAll();
}

void TransitionConfiguration::enterPressed()
{
    this->OKPressed();
}

void TransitionConfiguration::escapePressed()
{
    this->CancelPressed();
}

// Slots
void TransitionConfiguration::OKPressed()
{
    I_Configurable* l_Listener = this->getListener();
    if( nullptr != l_Listener)
    {
        l_Listener->applyConfiguration();
    }
}

void TransitionConfiguration::CancelPressed()
{
    I_Configurable* l_Listener = this->getListener();
    if( nullptr != l_Listener)
    {
        l_Listener->closeConfiguration();
    }
}
