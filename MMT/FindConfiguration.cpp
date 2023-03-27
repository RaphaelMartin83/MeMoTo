#include <QMimeData>

#include "FindConfiguration.h"

FindConfiguration::FindConfiguration()
{
    m_SearchLabel = new QLabel("Find in diagram:");
    m_SearchLabel->setFixedHeight(30);
    m_SearchEdit = new ConfigLineEdit(this);

    m_ItemTypeLabel = new QLabel("Item type:");
    m_ItemTypeLabel->setFixedHeight(30);
    m_ItemTypeComboBox = new QComboBox();

    m_ItemAttributeLabel = new QLabel("Item attribute:");
    m_ItemAttributeLabel->setFixedHeight(30);
    m_ItemAttributeComboBox = new QComboBox();

    m_OccurencesLabel = new QLabel("Occurences:");
    m_OccurencesLabel->setFixedHeight(30);
    m_OccurencesList = new ConfigList(this);

    m_OKButton = new QPushButton("OK");
    m_CancelButton = new QPushButton("Cancel");

    // Layout
    m_Layout = new QGridLayout();
    unsigned short l_row = 0U;
    m_Layout->addWidget(m_SearchLabel, l_row++, 0, 1, -1, Qt::Alignment());
    m_Layout->addWidget(m_SearchEdit, l_row++, 0, 1, -1, Qt::Alignment());
    m_Layout->addWidget(m_ItemTypeLabel, l_row++, 0, 1, -1, Qt::Alignment());
    m_Layout->addWidget(m_ItemTypeComboBox, l_row++, 0, 1, -1, Qt::Alignment());
    m_Layout->addWidget(m_ItemAttributeLabel, l_row++, 0, 1, -1, Qt::Alignment());
    m_Layout->addWidget(m_ItemAttributeComboBox, l_row++, 0, 1, -1, Qt::Alignment());
    m_Layout->addWidget(m_OccurencesLabel, l_row++, 0, 1, -1, Qt::Alignment());
    m_Layout->addWidget(m_OccurencesList, l_row++, 0, 1, -1, Qt::Alignment());
    m_Layout->addWidget(m_OKButton, l_row, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_CancelButton, l_row++, 1, 1, 1, Qt::Alignment());

    this->setVisible(false);

    this->setLayout(m_Layout);

    // Setup events
    connect(m_OKButton, &QPushButton::clicked,
            this, &FindConfiguration::OKPressed);
    connect(m_CancelButton, &QPushButton::clicked,
            this, &FindConfiguration::CancelPressed);
    connect(m_ItemTypeComboBox, &QComboBox::currentTextChanged,
            this, &FindConfiguration::selectedTypeChanged);
    connect(m_ItemAttributeComboBox, &QComboBox::currentTextChanged,
            this, &FindConfiguration::selectedFieldChanged);
    connect(m_OccurencesList, &ConfigList::clicked,
            this, &FindConfiguration::selectedItemChanged);
    connect(m_OccurencesList, &ConfigList::pressed,
            this, &FindConfiguration::selectedItemChanged);
    connect(m_OccurencesList, &ConfigList::doubleClicked,
            this, &FindConfiguration::OKPressed);
    connect(m_SearchEdit, &QLineEdit::textEdited,
            this, &FindConfiguration::researchChanged);
}

void FindConfiguration::registerConfigListener(
        I_FindConfigurationListener* p_Listener)
{
    m_Listener = p_Listener;
}

void FindConfiguration::setSelectableTypes(const QStringList& p_Types)
{
    m_ItemTypeComboBox->clear();

    m_ItemTypeComboBox->addItems(p_Types);

    this->updateOccurencesList();
}

void FindConfiguration::setFocusOnFirstZone()
{
    m_SearchEdit->setFocus();
    m_SearchEdit->selectAll();
}
void FindConfiguration::enterPressed()
{
    this->OKPressed();
}
void FindConfiguration::escapePressed()
{
    this->CancelPressed();
}
void FindConfiguration::OKPressed()
{
    m_Listener->findConfigurationValidated(m_ItemTypeComboBox->currentText(),
                                           m_ItemAttributeComboBox->currentText(),
                                           m_OccurencesList->currentItem()->text(),
                                           this->getCurrentSelectionOffset());
}
void FindConfiguration::CancelPressed()
{
    m_Listener->findConfigurationCanceled();
}
void FindConfiguration::selectedItemChanged()
{
    m_Listener->focusOnItem(m_ItemTypeComboBox->currentText(),
                            m_ItemAttributeComboBox->currentText(),
                            m_OccurencesList->currentItem()->text(),
                            this->getCurrentSelectionOffset());
}
void FindConfiguration::researchChanged()
{
    this->updateOccurencesList();
}
void FindConfiguration::selectedTypeChanged(const QString& p_Type)
{
    m_ItemAttributeComboBox->clear();

    m_ItemAttributeComboBox->addItems(m_Listener->getFieldsOfType(p_Type));

    this->updateOccurencesList();
}
void FindConfiguration::selectedFieldChanged()
{
    this->updateOccurencesList();
}

void FindConfiguration::updateOccurencesList()
{
    m_OccurencesList->clear();

    QStringList l_FullData = m_Listener->getAllDataOfTypeAndField(m_ItemTypeComboBox->currentText(),
                                                               m_ItemAttributeComboBox->currentText());

    for( unsigned int i_occurences = 0U; i_occurences < l_FullData.count(); i_occurences++ )
    {
        // Force everything to lowercase to ignore case into research
        QString l_CurrentData = l_FullData[i_occurences].toLower();

        if( l_CurrentData.contains(m_SearchEdit->text().toLower()) )
        {
            m_OccurencesList->addItem(l_FullData[i_occurences]);
        }
    }

    m_OccurencesList->sortItems();
}

QString FindConfiguration::getCurentType() const
{
    return m_ItemTypeComboBox->currentText();
}
QString FindConfiguration::getCurrentField() const
{
    return m_ItemAttributeComboBox->currentText();
}
QString FindConfiguration::getCurrentData() const
{
    return m_OccurencesList->currentItem()->text();
}
unsigned int FindConfiguration::getCurrentSelectionOffset() const
{
    QStringList l_CurrentList;
    for( unsigned int i_items = 0U; i_items < m_OccurencesList->count(); i_items++ )
    {
        l_CurrentList.append(m_OccurencesList->item(i_items)->text());
    }

    unsigned int l_SelectionOffset = 0U;
    unsigned int l_SelectedItem = m_OccurencesList->currentRow();
    bool l_found = false;

    do
    {
        if( 0U == (l_SelectedItem - l_SelectionOffset) ||
                (l_CurrentList[l_SelectedItem] != l_CurrentList[l_SelectedItem - l_SelectionOffset - 1U]) )
        {
            l_found = true;
        }
        else
        {
            l_SelectionOffset++;
        }
    }while(false == l_found);

    return l_SelectionOffset;
}
