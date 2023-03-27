#include "ClassConfiguration.h"
#include "StackingDefinitions.h"

ClassConfiguration::ClassConfiguration()
{
    // Init widgets
    m_OKButton = new QPushButton("OK");
    m_CancelButton = new QPushButton("Cancel");

    m_NameLabel = new QLabel("Name:");
    m_NameLabel->setFixedHeight(30);
    m_NameEdit = new ConfigLineEdit(static_cast<I_ConfigLineEditListener*>(this));

    m_ColorLabel = new QLabel("Color:");
    m_ColorSelection = new QComboBox();
    m_ColorSelection->addItems(QColor::colorNames());

    m_MethodsLabel = new QLabel("Methods:");
    m_MethodsLabel->setFixedHeight(30);
    m_MethodsList = new ConfigList(this);
    m_MethodsList->setMinimumHeight(200);
    this->methodsChanged();

    m_AttributesLabel = new QLabel("Attributes:");
    m_AttributesLabel->setFixedHeight(30);
    m_AttributesList = new ConfigList(this);
    m_AttributesList->setMinimumHeight(200);
    this->attributesChanged();

    m_Layout = new QGridLayout();

    // Setup layout
    unsigned short l_row = 0U;
    m_Layout->addWidget(m_NameLabel, l_row++, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_NameEdit, l_row++, 0, 1, -1, Qt::Alignment());

    m_Layout->addWidget(m_ColorLabel, l_row++, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_ColorSelection, l_row++, 0, 1, -1, Qt::Alignment());

    m_Layout->addWidget(m_MethodsLabel, l_row++, 0, 1, -1, Qt::Alignment());
    m_Layout->addWidget(m_MethodsList, l_row++, 0, 1, -1, Qt::Alignment());

    m_Layout->addWidget(m_AttributesLabel, l_row++, 0, 1, -1, Qt::Alignment());
    m_Layout->addWidget(m_AttributesList, l_row++, 0, 1, -1, Qt::Alignment());

    m_Layout->addWidget(m_CancelButton, l_row, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_OKButton, l_row, 1, 1, 1, Qt::Alignment());

    this->setVisible(false);

    this->setWidth(400U);
    this->setLayout(m_Layout);

    // Setup events
    connect(m_OKButton, &QPushButton::clicked, this,
            &ClassConfiguration::OKPressed);
    connect(m_CancelButton, &QPushButton::clicked, this,
            &ClassConfiguration::CancelPressed);
    connect(m_MethodsList, &QListWidget::itemChanged, this,
            &ClassConfiguration::methodsChanged);
    connect(m_AttributesList, &QListWidget::itemChanged, this,
            &ClassConfiguration::attributesChanged);
}

QString ClassConfiguration::getName() const
{
    return m_NameEdit->text();
}
void ClassConfiguration::setName(const QString& p_Name)
{
    m_NameEdit->setText(p_Name);
}

QList<QString> ClassConfiguration::getMethods() const
{
    QList<QString> l_Methods;

    for( unsigned short i_methods = 0U; i_methods < m_MethodsList->count(); i_methods++ )
    {
        if( "" != m_MethodsList->item(i_methods)->text() )
        {
            l_Methods.append(m_MethodsList->item(i_methods)->text());
        }
    }

    return l_Methods;
}

void ClassConfiguration::setMethods(const QList<QString>& p_Methods)
{
    m_MethodsList->clear();

    QListWidgetItem* l_item;
    for( unsigned short i_methods = 0U; i_methods < p_Methods.count(); i_methods++ )
    {
        l_item = new QListWidgetItem(p_Methods[i_methods]);
        l_item->setFlags(l_item->flags() | Qt::ItemIsEditable);
        m_MethodsList->addItem(l_item);
    }
    l_item = new QListWidgetItem("");
    l_item->setFlags(l_item->flags() | Qt::ItemIsEditable);
    m_MethodsList->addItem(l_item);
}

QList<QString> ClassConfiguration::getAttributes() const
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

void ClassConfiguration::setAttributes(const QList<QString>& p_Attributes)
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

QColor ClassConfiguration::getColor() const
{
    QString l_ColorString = this->m_ColorSelection->currentText();
    QColor l_Ret;
    l_Ret.setNamedColor(l_ColorString);

    return l_Ret;
}
void ClassConfiguration::setColor(QString p_Color)
{
    m_ColorSelection->setCurrentText(p_Color);
}

// I_ConfigurationContent
void ClassConfiguration::setFocusOnFirstZone()
{
    m_NameEdit->setFocus();
    m_NameEdit->selectAll();
}

// I_ConfigLineEditListener
void ClassConfiguration::enterPressed()
{
    this->OKPressed();
}
void ClassConfiguration::escapePressed()
{
    this->CancelPressed();
}

// Slots
void ClassConfiguration::OKPressed()
{
    I_Configurable* l_Listener = this->getListener();
    if( nullptr != l_Listener)
    {
        l_Listener->applyConfiguration();
    }
}
void ClassConfiguration::CancelPressed()
{
    I_Configurable* l_Listener = this->getListener();
    if( nullptr != l_Listener)
    {
        l_Listener->closeConfiguration();
    }
}

void ClassConfiguration::methodsChanged()
{
    // Check if an item has to be removed
    for( unsigned short i_items=0U; i_items<m_MethodsList->count(); i_items++ )
    {
        if( m_MethodsList->item(i_items)->text() == "" )
        {
            delete m_MethodsList->item(i_items);
        }
    }

    // If needed, add an empty item in the end
    if( (0 == m_MethodsList->count()) ||
            ("" != m_MethodsList->item(m_MethodsList->count() - 1)->text()) )
    {
        // MethodsList will handle the memory
        QListWidgetItem* l_item = new QListWidgetItem(QString(""));
        l_item->setFlags(l_item->flags() | Qt::ItemIsEditable);
        m_MethodsList->addItem(l_item);
    }
}


void ClassConfiguration::attributesChanged()
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
