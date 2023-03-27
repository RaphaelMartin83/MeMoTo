#ifndef GENERALIZATIONCONFIGURATION_H
#define GENERALIZATIONCONFIGURATION_H

#include <QGridLayout>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>

#include "I_ConfigurationContent.h"
#include "I_Configurable.h"

class GeneralizationConfiguration:
        public I_ConfigurationContent
{
    Q_OBJECT

public:
    GeneralizationConfiguration();

    bool getAutoRoute() const;
    void setAutoRoute(bool p_isAutoRoute);

    // I_Selectable overloads
    void select(QPoint p_Pos);
    void unselect();

    // I_ConfigurationContent
    void setFocusOnFirstZone();

public slots:
    void OKPressed();
    void CancelPressed();

private:
    QGridLayout* m_Layout;

    QLabel* m_AutoRouteLabel;
    QCheckBox* m_AutoRouteCheckBox;

    QPushButton* m_OKButton;
    QPushButton* m_CancelButton;

    // Display
    QWidget* m_emptySpace;
};

#endif // GENERALIZATIONCONFIGURATION_H
