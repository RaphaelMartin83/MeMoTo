#ifndef FINDCONFIGURATION_H
#define FINDCONFIGURATION_H

#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

#include <CommonGraphics/ConfigList.h>
#include <CommonGraphics/ConfigLineEdit.h>

#include <Engine/I_ConfigurationContent.h>
#include <Engine/I_FindConfigurationListener.h>
#include <Engine/I_ConfigLineEditListener.h>

class FindConfiguration:
        public I_ConfigurationContent,
        public I_ConfigLineEditListener
{
    Q_OBJECT

public:
    FindConfiguration();

    void registerConfigListener(I_FindConfigurationListener* p_Listener);

    void setSelectableTypes(const QStringList& p_Types);
    void updateOccurencesList();

    QString getCurentType() const;
    QString getCurrentField() const;
    QString getCurrentData() const;
    unsigned int getCurrentSelectionOffset() const;

    // I_ConfigurationContent
    void setFocusOnFirstZone();

    // I_ConfigLineEditListener
    void enterPressed();
    void escapePressed();

public slots:
    void OKPressed();
    void CancelPressed();
    void selectedItemChanged();
    void researchChanged();
    void selectedTypeChanged(const QString& p_Type);
    void selectedFieldChanged();

private:
    QGridLayout* m_Layout;

    QLabel* m_SearchLabel;
    ConfigLineEdit* m_SearchEdit;

    QLabel* m_ItemTypeLabel;
    QComboBox* m_ItemTypeComboBox;

    QLabel* m_ItemAttributeLabel;
    QComboBox* m_ItemAttributeComboBox;

    QLabel* m_OccurencesLabel;
    ConfigList* m_OccurencesList;

    QPushButton* m_OKButton;
    QPushButton* m_CancelButton;

    // The listener
    I_FindConfigurationListener* m_Listener;
};

#endif // FINDCONFIGURATION_H
