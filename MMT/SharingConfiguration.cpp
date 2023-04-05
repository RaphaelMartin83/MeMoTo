#include "SharingConfiguration.h"
#include "MeMoToApplication.h"

static const char* s_TitleIfNotSharing = "Enter file sharing path:";
static const char* s_TitleIfSharing = "Sharing...";

static const char* s_OkButtonIfNotSharing = "Start";
static const char* s_OkButtonIfSharing = "Stop";

SharingConfiguration::SharingConfiguration()
{
    // Init widgets (Client)
    m_ClientLabel = new QLabel("Connect to host");
    m_ClientLabel->setFixedHeight(30);
    m_HostLabel = new QLabel("Host: ");
    m_HostLabel->setFixedHeight(30);
    m_HostNameEdit = new ConfigLineEdit(this);
    m_HostNameEdit->setText("");
    m_PortLabel = new QLabel("Port: ");
    m_PortLabel->setFixedHeight(30);
    m_PortEdit = new QSpinBox();
    m_PortEdit->setMinimum(1024);
    m_PortEdit->setMaximum(65535);
    m_StartClientButton = new QPushButton("Start");
    m_StopClientButton = new QPushButton("Stop");

    // Init widgets (Server)
    m_ServerLabel = new QLabel("Open session");
    m_ServerLabel->setFixedHeight(30);
    m_HostLabelServer = new QLabel("Your address: ");
    m_HostLabelServer->setFixedHeight(30);
    m_HostEditServer = new ConfigLineEdit(this);
    m_HostEditServer->setText("");
    m_PortLabelServer = new QLabel("Port: ");
    m_PortEditServer = new QSpinBox();
    m_PortEditServer->setMinimum(1024);
    m_PortEditServer->setMaximum(65535);
    m_StartServerButton = new QPushButton("Start");
    m_StopServerButton = new QPushButton("Stop");

    m_CancelButton = new QPushButton("Cancel");
    m_emptySpace = new QWidget();

    // Layout
    m_Layout = new QGridLayout();
    unsigned short l_row = 0U;
    // Client side
    m_Layout->addWidget(m_ClientLabel, l_row++, 0, 1, -1, Qt::Alignment());
    m_Layout->addWidget(m_HostLabel, l_row, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_HostNameEdit, l_row++, 1, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_PortLabel, l_row, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_PortEdit, l_row++, 1, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_StartClientButton, l_row, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_StopClientButton, l_row++, 1, 1, 1, Qt::Alignment());

    // Server side
    m_Layout->addWidget(m_ServerLabel, l_row++, 0, 1, -1, Qt::Alignment());
    m_Layout->addWidget(m_HostLabelServer, l_row, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_HostEditServer, l_row++, 1, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_PortLabelServer, l_row, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_PortEditServer, l_row++, 1, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_StartServerButton, l_row, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_StopServerButton, l_row++, 1, 1, 1, Qt::Alignment());

    m_Layout->addWidget(m_CancelButton, l_row++, 0, 1, -1, Qt::Alignment());

    m_Layout->addWidget(m_emptySpace, l_row++, 0, -1, 1, Qt::Alignment());

    this->setVisible(false);

    this->setLayout(m_Layout);

    // Setup events
    connect(m_StartClientButton, &QPushButton::clicked, this, &SharingConfiguration::clientStartPressed);
    connect(m_StopClientButton, &QPushButton::clicked, this, &SharingConfiguration::clientStopPressed);
    connect(m_StartServerButton, &QPushButton::clicked, this, &SharingConfiguration::serverStartPressed);
    connect(m_StopServerButton, &QPushButton::clicked, this, &SharingConfiguration::serverStopPressed);
    connect(m_CancelButton, &QPushButton::clicked, this, &SharingConfiguration::cancelPressed);
}

void SharingConfiguration::registerConfigListener(I_SharingConfigurationListener* p_Listener)
{
    m_Listener = p_Listener;
}

QString SharingConfiguration::getHostName() const
{
    return m_HostNameEdit->text();
}
void SharingConfiguration::setHostName(const QString& p_Host)
{
    m_HostNameEdit->setText(p_Host);
}

quint16 SharingConfiguration::getHostPort() const
{
    return m_PortEdit->value();
}
void SharingConfiguration::setHostPort(const quint16& p_Port)
{
    m_PortEdit->setValue(p_Port);
}
QString SharingConfiguration::getSessionAddress() const
{
    return m_HostEditServer->text();
}
void SharingConfiguration::setSessionAddress(const QHostAddress& p_Address)
{
    QHostAddress l_Address(p_Address);
    m_HostEditServer->setText(l_Address.toString());
}

quint16 SharingConfiguration::getSessionPort() const
{
    return m_PortEditServer->value();
}
void SharingConfiguration::setSessionPort(const quint16& p_Port)
{
    m_PortEditServer->setValue(p_Port);
}

// I_ConfigurationContent
void SharingConfiguration::setFocusOnFirstZone()
{
    m_HostNameEdit->setFocus();
}

// I_ConfigLineEditListener
void SharingConfiguration::enterPressed()
{
}
void SharingConfiguration::escapePressed()
{
    this->cancelPressed();
}

void SharingConfiguration::clientStartPressed()
{
    if( (nullptr != m_Listener) )
    {
        m_Listener->sharingHostSelected(m_HostNameEdit->text(),
                         m_PortEdit->value());
    }
}
void SharingConfiguration::clientStopPressed()
{
    if( nullptr != m_Listener )
    {
        m_Listener->sharingStopped();
    }
}
void SharingConfiguration::serverStartPressed()
{
    if( (nullptr != m_Listener) )
    {
        m_Listener->sharingSessionCreationSelected(m_PortEdit->value());
    }
}
void SharingConfiguration::serverStopPressed()
{
    if( nullptr != m_Listener )
    {
        m_Listener->sharingStopped();
    }
}
void SharingConfiguration::cancelPressed()
{
    m_Listener->sharingCanceled();
}
