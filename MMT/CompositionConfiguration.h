#ifndef COMPOSITIONCONFIGURATION_H
#define COMPOSITIONCONFIGURATION_H

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>

#include "ConfigLineEdit.h"
#include "ConfigList.h"

#include "I_ConfigurationContent.h"
#include "I_ConfigLineEditListener.h"
#include "I_Configurable.h"

class CompositionConfiguration:
        public I_ConfigurationContent,
        public I_ConfigLineEditListener
{
    Q_OBJECT

public:
    CompositionConfiguration();

    QList<QString> getAttributes() const;
    void setAttributes(const QList<QString>& p_Attributes);

    bool getAutoRoute() const;
    void setAutoRoute(bool p_isAutoRoute);

    // I_ConfigurationContent
    void setFocusOnFirstZone();

    // I_ConfigLineEditListener
    void enterPressed();
    void escapePressed();

public slots:
    void OKPressed();
    void CancelPressed();
    void attributesChanged();

private:
    QGridLayout* m_Layout;

    QLabel* m_AutoRouteLabel;
    QCheckBox* m_AutoRouteCheckBox;

    QPushButton* m_OKButton;
    QPushButton* m_CancelButton;

    // Attributes
    QLabel* m_AttributesLabel;
    ConfigList* m_AttributesList;
};

#endif // COMPOSITIONCONFIGURATION_H
