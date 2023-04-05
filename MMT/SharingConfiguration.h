#ifndef SHARINGCONFIGURATION_H
#define SHARINGCONFIGURATION_H

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QHostAddress>

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

    void registerConfigListener(I_SharingConfigurationListener* p_Listener);

    QString getHostName() const;
    void setHostName(const QString& p_Host);

    quint16 getHostPort() const;
    void setHostPort(const quint16& p_Port);

    QString getSessionAddress() const;
    void setSessionAddress(const QHostAddress& p_Address);

    quint16 getSessionPort() const;
    void setSessionPort(const quint16& p_Port);

    // I_ConfigurationContent
    void setFocusOnFirstZone();
    void OKPressed() {}

    // I_ConfigLineEditListener
    void enterPressed();
    void escapePressed();

public slots:
    void clientStartPressed();
    void clientStopPressed();
    void serverStartPressed();
    void serverStopPressed();

    void cancelPressed();

private:
    QGridLayout* m_Layout;

    QLabel* m_ClientLabel;
    QLabel* m_HostLabel;
    ConfigLineEdit* m_HostNameEdit;
    QLabel* m_PortLabel;
    QSpinBox* m_PortEdit;
    QPushButton* m_StartClientButton;
    QPushButton* m_StopClientButton;

    QLabel* m_ServerLabel;
    QLabel* m_HostLabelServer;
    ConfigLineEdit* m_HostEditServer;
    QLabel* m_PortLabelServer;
    QSpinBox* m_PortEditServer;
    QPushButton* m_StartServerButton;
    QPushButton* m_StopServerButton;

    QPushButton* m_CancelButton;

    QWidget* m_emptySpace;

    I_SharingConfigurationListener* m_Listener;
};

#endif // SHARINGCONFIGURATION_H
