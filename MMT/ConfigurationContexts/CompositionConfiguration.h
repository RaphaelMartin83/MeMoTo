#ifndef COMPOSITIONCONFIGURATION_H
#define COMPOSITIONCONFIGURATION_H

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>

#include <CommonGraphics/ConfigLineEdit.h>
#include <CommonGraphics/ConfigList.h>

#include <Engine/I_ConfigurationContent.h>
#include <Engine/I_ConfigLineEditListener.h>
#include <Engine/I_Configurable.h>

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
