#ifndef CHOICECONFIGURATION_H
#define CHOICECONFIGURATION_H

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QKeyEvent>

#include "I_Configurable.h"
#include "I_ConfigurationContent.h"
#include "ConfigLineEdit.h"
#include "I_ConfigLineEditListener.h"

class ChoiceConfiguration: public I_ConfigurationContent,
        public I_ConfigLineEditListener
{
    Q_OBJECT

public:
    ChoiceConfiguration();

    QString getName() const;
    void setName(const QString& p_Name);

    // I_ConfigurationContent
    void setFocusOnFirstZone();

    // I_ConfigLineEditListener
    void enterPressed();
    void escapePressed();

public slots:
    void OKPressed();
    void CancelPressed();

private:
    QGridLayout* m_Layout;

    QPushButton* m_OKButton;
    QPushButton* m_CancelButton;

    // Name
    QLabel* m_NameLabel;
    ConfigLineEdit* m_NameEdit;

    // Empty space
    QWidget* m_EmptySpace;
};

#endif // CHOICECONFIGURATION_H
