#ifndef SHARINGCONFIGURATION_H
#define SHARINGCONFIGURATION_H

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include "ConfigLineEdit.h"

#include "I_ConfigurationContent.h"
#include "I_ConfigLineEditListener.h"
#include "I_SharingConfigurationListener.h"

class SharingConfiguration:
        public I_ConfigurationContent,
        public I_ConfigLineEditListener
{
    Q_OBJECT
public:
    SharingConfiguration();

    QString getFilePathAndName();
    void setFilePathAndName(const QString& p_File);

    void setFileSharingRunning(bool p_isFileSharingRunning);

    void registerConfigListener(I_SharingConfigurationListener* p_Listener);

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

    QLabel* m_ContextLabel;
    QLabel* m_FileNameLabel;
    ConfigLineEdit* m_FileNameEdit;

    QPushButton* m_OKButton;
    QPushButton* m_CancelButton;

    QWidget* m_emptySpace;

    I_SharingConfigurationListener* m_Listener;
};

#endif // SHARINGCONFIGURATION_H
