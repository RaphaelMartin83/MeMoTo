#ifndef CHOICECONFIGURATION_H
#define CHOICECONFIGURATION_H

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QKeyEvent>

#include <Engine/I_Configurable.h>
#include <Engine/I_ConfigurationContent.h>
#include <Engine/I_ConfigLineEditListener.h>

#include <CommonGraphics/ConfigLineEdit.h>

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
