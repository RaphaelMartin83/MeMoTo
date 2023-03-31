#ifndef ERRORDISPLAYER_H
#define ERRORDISPLAYER_H

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QString>

#include "I_ConfigurationContent.h"

class ErrorDisplayer: public I_ConfigurationContent
{
    Q_OBJECT
public:
    ErrorDisplayer();
    static void displayError(const QString& p_ErrorTitle,
                             const QString& p_ErrorText);

public slots:
    void setFocusOnFirstZone();
    void OKPressed();
    void CopyPressed();

private:
    static QGridLayout* m_Layout;

    static QPushButton* m_CopyButton;
    static QPushButton* m_OKButton;

    // Name
    static QLabel* m_NameLabel;

    // Text
    static QLabel* m_Text;
};

#endif // ERRORDISPLAYER_H
