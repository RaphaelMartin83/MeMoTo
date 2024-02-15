#ifndef CLOSEWITHOUTSAVINGCONFIGURATION_H
#define CLOSEWITHOUTSAVINGCONFIGURATION_H

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include <Engine/I_ConfigurationContent.h>
#include <Engine/I_CloseWithoutSavingConfigurationListener.h>

class CloseWithoutSavingConfiguration :
        public I_ConfigurationContent
{
public:
    CloseWithoutSavingConfiguration();

    // I_ConfigurationContent
    void setFocusOnFirstZone();
    void OKPressed();

    void registerListener(I_CloseWithoutSavingConfigurationListener* p_Listener);

public slots:
    void savePressed();
    void closeAnywayPressed();

private:
    QGridLayout* m_Layout;

    QLabel* m_AreYouSureLabel;

    QPushButton* m_SaveButton;
    QPushButton* m_CloseButton;

    I_CloseWithoutSavingConfigurationListener* m_Listener;
};

#endif // CLOSEWITHOUTSAVINGCONFIGURATION_H
