#ifndef EXTERNALCLASSCONFIGURATION_H
#define EXTERNALCLASSCONFIGURATION_H

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>

#include <CommonGraphics/ConfigLineEdit.h>

#include <Engine/I_ConfigurationContent.h>
#include <Engine/I_ConfigLineEditListener.h>
#include <Engine/I_Configurable.h>

class ExternalClassConfiguration: public I_ConfigurationContent,
        public I_ConfigLineEditListener
{
    Q_OBJECT

public:
    ExternalClassConfiguration();

    QString getName() const;
    void setName(const QString& p_Name);

    QString getRoot() const;
    void setRoot(const QString& p_Root);

    QString getPath() const;
    void setPath(const QString& p_Path);

    QColor getColor() const;
    void setColor(QString p_Color);

    bool isContentToHide() const;
    void setContentToHide(bool p_isContentToHide);

    static QString getResolvedPath(QString p_Root, QString p_Path, QString p_Name);

    // I_ConfigurationContent
    void setFocusOnFirstZone();

    // I_ConfigLineEditListener
    void enterPressed();
    void escapePressed();

public slots:
    void OKPressed();
    void CancelPressed();

    void openPressed();

private:
    QGridLayout* m_Layout;

    // Name
    QLabel* m_NameLabel;
    ConfigLineEdit* m_NameEdit;

    // Root
    QLabel* m_RootLabel;
    ConfigLineEdit* m_RootEdit;

    // Path
    QLabel* m_PathLabel;
    ConfigLineEdit* m_PathEdit;

    // Color
    QComboBox* m_ColorSelection;

    QLabel* m_HideContent;
    QCheckBox* m_HideContentCheckBox;

    QPushButton* m_OpenButton;

    QPushButton* m_OKButton;
    QPushButton* m_CancelButton;

    // Empty space
    QWidget* m_EmptySpace;
};

#endif // EXTERNALCLASSCONFIGURATION_H
