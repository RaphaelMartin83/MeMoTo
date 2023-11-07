#include "StateConfiguration.h"

StateConfiguration::StateConfiguration()
{
    // Init widgets
    m_OKButton = new QPushButton("OK");
    m_CancelButton = new QPushButton("Cancel");

    m_NameLabel = new QLabel("Name:");
    m_NameLabel->setFixedHeight(30);
    m_NameEdit = new ConfigLineEdit(this);

    m_EntryActionLabel = new QLabel("Entry Action:");
    m_EntryActionLabel->setFixedHeight(30);
    m_EntryActionEdit = new ConfigLineEdit(this);

    m_ExitActionLabel = new QLabel("Exit Action:");
    m_ExitActionLabel->setFixedHeight(30);
    m_ExitActionEdit = new ConfigLineEdit(this);

    m_StereotypeLabel = new QLabel("Stereotype:");
    m_StereotypeLabel->setFixedHeight(30);
    m_StereotypeEdit = new ConfigLineEdit(this);

    m_ColorSelection = new QComboBox();
    m_ColorSelection->addItems(QColor::colorNames());

    m_emptySpace = new QWidget();

    m_Layout = new QGridLayout();

    // Setup layout
    m_Layout->addWidget(m_NameLabel, 0, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_NameEdit, 1, 0, 1, -1, Qt::Alignment());

    m_Layout->addWidget(m_EntryActionLabel, 2, 0, 1, -1, Qt::Alignment());
    m_Layout->addWidget(m_EntryActionEdit, 3, 0, 1, -1, Qt::Alignment());

    m_Layout->addWidget(m_ExitActionLabel, 4, 0, 1, -1, Qt::Alignment());
    m_Layout->addWidget(m_ExitActionEdit, 5, 0, 1, -1, Qt::Alignment());

    m_Layout->addWidget(m_StereotypeLabel, 6, 0, 1, -1, Qt::Alignment());
    m_Layout->addWidget(m_StereotypeEdit, 7, 0, 1, -1, Qt::Alignment());

    m_Layout->addWidget(m_ColorSelection, 8, -1, 1, -1, Qt::Alignment());

    m_Layout->addWidget(m_CancelButton, 9, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_OKButton, 9, 1, 1, 1, Qt::Alignment());

    m_Layout->addWidget(m_emptySpace, 10, 0, -1, -1, Qt::Alignment());

    this->setVisible(false);

    this->setLayout(m_Layout);

    // Setup events
    connect(m_OKButton, &QPushButton::clicked, this, &StateConfiguration::OKPressed);
    connect(m_CancelButton, &QPushButton::clicked, this, &StateConfiguration::CancelPressed);
}

QString StateConfiguration::getName() const
{
    return m_NameEdit->text();
}
void StateConfiguration::setName(const QString& p_Name)
{
    m_NameEdit->setText(p_Name);
}

QString StateConfiguration::getEntryAction() const
{
    return m_EntryActionEdit->text();
}
void StateConfiguration::setEntryAction(const QString& p_EntryAction)
{
    m_EntryActionEdit->setText(p_EntryAction);
}

QString StateConfiguration::getExitAction() const
{
    return m_ExitActionEdit->text();
}
void StateConfiguration::setExitAction(const QString& p_ExitAction)
{
    m_ExitActionEdit->setText(p_ExitAction);
}

QString StateConfiguration::getStereotype() const
{
    return m_StereotypeEdit->text();
}
void StateConfiguration::setStereotype(const QString& p_Stereotype)
{
    m_StereotypeEdit->setText(p_Stereotype);
}

QColor StateConfiguration::getColor() const
{
    QString l_ColorString = this->m_ColorSelection->currentText();
    QColor l_Ret;
    l_Ret.setNamedColor(l_ColorString);

    return l_Ret;
}
void StateConfiguration::setColor(QString p_Color)
{
    m_ColorSelection->setCurrentText(p_Color);
}

void StateConfiguration::setFocusOnFirstZone()
{
    m_NameEdit->setFocus();
    m_NameEdit->selectAll();
}

void StateConfiguration::enterPressed()
{
    this->OKPressed();
}

void StateConfiguration::escapePressed()
{
    this->CancelPressed();
}

// Slots
void StateConfiguration::OKPressed()
{
    I_Configurable* l_Listener = this->getListener();
    if( nullptr != l_Listener)
    {
        l_Listener->applyConfiguration();
    }
}

void StateConfiguration::CancelPressed()
{
    I_Configurable* l_Listener = this->getListener();
    if( nullptr != l_Listener)
    {
        l_Listener->closeConfiguration();
    }
}
