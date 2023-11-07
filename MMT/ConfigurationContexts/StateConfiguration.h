#ifndef STATECONFIGURATION_H
#define STATECONFIGURATION_H

#include <QGridLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>

#include <Engine/I_ConfigurationContent.h>
#include <Engine/I_ConfigLineEditListener.h>

#include <CommonGraphics/ConfigLineEdit.h>

class StateConfiguration: public I_ConfigurationContent,
        public I_ConfigLineEditListener
{
    Q_OBJECT

public:
    StateConfiguration();

    QString getName() const;
    void setName(const QString& p_Name);

    QString getEntryAction() const;
    void setEntryAction(const QString& p_EntryAction);

    QString getExitAction() const;
    void setExitAction(const QString& p_ExitAction);

    QString getStereotype() const;
    void setStereotype(const QString& p_Stereotype);

    QColor getColor() const;
    void setColor(QString p_Color);

    // I_ConfigurationContent
    void setFocusOnFirstZone();

    // I_ConfigLineEditListener
    void enterPressed();
    void escapePressed();

public slots:
    void OKPressed();
    void CancelPressed();

private:
    QPushButton* m_OKButton;
    QPushButton* m_CancelButton;

    // Name
    QLabel* m_NameLabel;
    ConfigLineEdit* m_NameEdit;

    // Entry Action
    QLabel* m_EntryActionLabel;
    ConfigLineEdit* m_EntryActionEdit;

    // Exit Action
    QLabel* m_ExitActionLabel;
    ConfigLineEdit* m_ExitActionEdit;

    // Stereotype
    QLabel* m_StereotypeLabel;
    ConfigLineEdit* m_StereotypeEdit;

    // Color
    QComboBox* m_ColorSelection;

    // Display
    QWidget* m_emptySpace;

    // The layout
    QGridLayout* m_Layout;
};

#endif // STATECONFIGURATION_H
