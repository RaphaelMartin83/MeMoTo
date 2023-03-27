#ifndef CLASSCONFIGURATION_H
#define CLASSCONFIGURATION_H

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>

#include "ConfigLineEdit.h"
#include "ConfigList.h"

#include "I_ConfigurationContent.h"
#include "I_ConfigLineEditListener.h"
#include "I_Configurable.h"

class ClassConfiguration:
        public I_ConfigurationContent,
        public I_ConfigLineEditListener
{
    Q_OBJECT

public:
    ClassConfiguration();

    QString getName() const;
    void setName(const QString& p_Name);
    QList<QString> getMethods() const;
    void setMethods(const QList<QString>& p_Methods);
    QList<QString> getAttributes() const;
    void setAttributes(const QList<QString>& p_Attributes);
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
    void methodsChanged();
    void attributesChanged();

private:
    QGridLayout* m_Layout;

    QPushButton* m_OKButton;
    QPushButton* m_CancelButton;

    // Name
    QLabel* m_NameLabel;
    ConfigLineEdit* m_NameEdit;

    // Color
    QLabel* m_ColorLabel;
    QComboBox* m_ColorSelection;

    // Methods
    QLabel* m_MethodsLabel;
    ConfigList* m_MethodsList;

    // Attributes
    QLabel* m_AttributesLabel;
    ConfigList* m_AttributesList;
};

#endif // CLASSCONFIGURATION_H
