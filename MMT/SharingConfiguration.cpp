#include "SharingConfiguration.h"
#include "MeMoToApplication.h"

SharingConfiguration::SharingConfiguration()
{
    // Init widgets
    m_ContextLabel = new QLabel("Enter file sharing path:");
    m_ContextLabel->setFixedHeight(30);

    m_FileNameEdit = new ConfigLineEdit(this);
    m_FileNameEdit->setText("");

    m_OKButton = new QPushButton("OK");
    m_CancelButton = new QPushButton("Cancel");

    m_FileNameLabel = new QLabel("NOTE: Enter the path and name of the memoto sharing file.\n"
                                 "Create this file on a shared directory accessible by your coworkers and give them the path.\n"
                                 "If needed, memoto will automatically add " + MeMoToApplication::SharedFileExtension() + " an extension.");
    m_FileNameLabel->setWordWrap(true);
    m_FileNameLabel->setFixedHeight(200);

    m_emptySpace = new QWidget();

    // Layout
    m_Layout = new QGridLayout();
    unsigned short l_row = 0U;
    m_Layout->addWidget(m_ContextLabel, l_row++, 0, 1, -1, Qt::Alignment());
    m_Layout->addWidget(m_FileNameEdit, l_row++, 0, 1, -1, Qt::Alignment());
    m_Layout->addWidget(m_OKButton, l_row, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_CancelButton, l_row++, 1, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_FileNameLabel, l_row++, 0, 1, -1, Qt::Alignment());
    m_Layout->addWidget(m_emptySpace, l_row++, 0, -1, 1, Qt::Alignment());

    this->setVisible(false);

    this->setLayout(m_Layout);

    // Setup events
    connect(m_OKButton, &QPushButton::clicked, this, &SharingConfiguration::OKPressed);
    connect(m_CancelButton, &QPushButton::clicked, this, &SharingConfiguration::CancelPressed);
}

QString SharingConfiguration::getFilePathAndName()
{
    return m_FileNameEdit->text();
}
void SharingConfiguration::setFilePathAndName(const QString& p_FileName)
{
    m_FileNameEdit->setText(p_FileName);
}

void SharingConfiguration::registerConfigListener(I_SharingConfigurationListener* p_Listener)
{
    m_Listener = p_Listener;
}


// I_ConfigurationContent
void SharingConfiguration::setFocusOnFirstZone()
{
    m_FileNameEdit->setFocus();
}

// I_ConfigLineEditListener
void SharingConfiguration::enterPressed()
{
    this->OKPressed();
}
void SharingConfiguration::escapePressed()
{
    this->CancelPressed();
}

void SharingConfiguration::OKPressed()
{
    if( (nullptr != m_Listener) )
    {
        m_Listener->sharingPlaceSelected(m_FileNameEdit->text());
    }
}
void SharingConfiguration::CancelPressed()
{
    if( nullptr != m_Listener )
    {
        m_Listener->sharingCanceled();
    }
}
