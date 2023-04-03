#ifndef TRANSITIONCONFIGURATION_H
#define TRANSITIONCONFIGURATION_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QCheckBox>

#include "ConfigLineEdit.h"
#include "I_ConfigLineEditListener.h"
#include "I_Configurable.h"
#include "I_ConfigurationContent.h"

class TransitionConfiguration: public I_ConfigurationContent,
        public I_ConfigLineEditListener
{
    Q_OBJECT

public:
    TransitionConfiguration();

    QString getEvent() const;
    void setEvent(const QString& p_Event);

    QString getAction() const;
    void setAction(const QString& p_Action);

    QString getGuard() const;
    void setGuard(const QString& p_Guard);

    QString getStereotype() const;
    void setStereotype(const QString& p_Stereotype);

    bool getAutoRoute() const;
    void setAutoRoute(bool p_isAutoRoute);

    void setFromInfo(QString p_FromType, QString p_FromName);
    void setToInfo(QString p_ToType, QString p_ToName);

    void setCurvedInfo(bool p_Curved);
    bool getCurvedInfo() const;

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

    // Event
    QLabel* m_EventLabel;
    ConfigLineEdit* m_EventEdit;

    // Action
    QLabel* m_ActionLabel;
    ConfigLineEdit* m_ActionEdit;

    // Guard
    QLabel* m_GuardLabel;
    ConfigLineEdit* m_GuardEdit;

    // Stereotype
    QLabel* m_StereotypeLabel;
    ConfigLineEdit* m_StereotypeEdit;

    // Route
    QLabel* m_AutoRouteLabel;
    QCheckBox* m_AutoRouteCheckBox;

    // Curved?
    QLabel* m_CurvedLabel;
    QCheckBox* m_CurvedCheckBox;

    // Infos
    QLabel* m_ConnectFromLabel;
    QLabel* m_ConnectToLabel;

    // Display
    QWidget* m_emptySpace;

    // The layout
    QGridLayout* m_Layout;
};

#endif // TRANSITIONCONFIGURATION_H
