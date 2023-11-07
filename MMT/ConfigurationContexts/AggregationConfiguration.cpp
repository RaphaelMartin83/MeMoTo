#include "AggregationConfiguration.h"

AggregationConfiguration::AggregationConfiguration()
{
    // Init widgets
    m_AutoRouteLabel = new QLabel("Auto route:");
    m_AutoRouteCheckBox = new QCheckBox();
    m_OKButton = new QPushButton("OK");
    m_CancelButton = new QPushButton("Cancel");

    m_AttributesLabel = new QLabel("Attributes:");
    m_AttributesLabel->setFixedHeight(30);
    m_AttributesList = new ConfigList(this);
    m_AttributesList->setMinimumHeight(200);
    this->attributesChanged();

    m_Layout = new QGridLayout();

    // Setup layout
    unsigned short l_row = 0U;
    m_Layout->addWidget(m_AttributesLabel, l_row++, 0, 1, -1, Qt::Alignment());
    m_Layout->addWidget(m_AttributesList, l_row++, 0, 1, -1, Qt::Alignment());

    m_Layout->addWidget(m_AutoRouteLabel, l_row, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_AutoRouteCheckBox, l_row++, 1, 1, 1, Qt::Alignment());

    m_Layout->addWidget(m_CancelButton, l_row, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_OKButton, l_row, 1, 1, 1, Qt::Alignment());

    this->setVisible(false);

    this->setLayout(m_Layout);

    // Setup events
    connect(m_OKButton, &QPushButton::clicked, this,
            &AggregationConfiguration::OKPressed);
    connect(m_CancelButton, &QPushButton::clicked, this,
            &AggregationConfiguration::CancelPressed);
    connect(m_AttributesList, &QListWidget::itemChanged, this,
            &AggregationConfiguration::attributesChanged);
}

QList<QString> AggregationConfiguration::getAttributes() const
{
    QList<QString> l_Attributes;

    for( unsigned short i_methods = 0U; i_methods < m_AttributesList->count(); i_methods++ )
    {
        if( "" != m_AttributesList->item(i_methods)->text() )
        {
            l_Attributes.append(m_AttributesList->item(i_methods)->text());
        }
    }

    return l_Attributes;
}

void AggregationConfiguration::setAttributes(const QList<QString>& p_Attributes)
{
    m_AttributesList->clear();

    QListWidgetItem* l_item;
    for( unsigned short i_methods = 0U; i_methods < p_Attributes.count(); i_methods++ )
    {
        l_item = new QListWidgetItem(p_Attributes[i_methods]);
        l_item->setFlags(l_item->flags() | Qt::ItemIsEditable);
        m_AttributesList->addItem(l_item);
    }
    l_item = new QListWidgetItem("");
    l_item->setFlags(l_item->flags() | Qt::ItemIsEditable);
    m_AttributesList->addItem(l_item);
}
bool AggregationConfiguration::getAutoRoute() const
{
    return m_AutoRouteCheckBox->isChecked();
}
void AggregationConfiguration::setAutoRoute(bool p_isAutoRoute)
{
    m_AutoRouteCheckBox->setChecked(p_isAutoRoute);
}

// I_ConfigurationContent
void AggregationConfiguration::setFocusOnFirstZone()
{
    m_AttributesList->setFocus();
}

// I_ConfigLineEditListener
void AggregationConfiguration::enterPressed()
{
    this->OKPressed();
}
void AggregationConfiguration::escapePressed()
{
    this->CancelPressed();
}

// Slots
void AggregationConfiguration::OKPressed()
{
    I_Configurable* l_Listener = this->getListener();
    if( nullptr != l_Listener)
    {
        l_Listener->applyConfiguration();
    }
}
void AggregationConfiguration::CancelPressed()
{
    I_Configurable* l_Listener = this->getListener();
    if( nullptr != l_Listener)
    {
        l_Listener->closeConfiguration();
    }
}

void AggregationConfiguration::attributesChanged()
{
    // Check if an item has to be removed
    for( unsigned short i_items=0U; i_items<m_AttributesList->count(); i_items++ )
    {
        if( m_AttributesList->item(i_items)->text() == "" )
        {
            delete m_AttributesList->item(i_items);
        }
    }

    // If needed, add an empty item in the end
    if( (0 == m_AttributesList->count()) ||
            ("" != m_AttributesList->item(m_AttributesList->count() - 1)->text()) )
    {
        // AttributesList will handle the memory
        QListWidgetItem* l_item = new QListWidgetItem(QString(""));
        l_item->setFlags(l_item->flags() | Qt::ItemIsEditable);
        m_AttributesList->addItem(l_item);
    }
}
