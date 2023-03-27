#ifndef CONFIGWIDGET_H
#define CONFIGWIDGET_H

#include <QGridLayout>

#include "I_ConfigurationContent.h"

class ConfigWidget
{
public:
    static QWidget& getInstance();
    static void deleteInstance();

    ~ConfigWidget();

    static void open(I_ConfigurationContent* p_Widget);
    static void close();

private:
    ConfigWidget();

    static void openAgain(I_ConfigurationContent* p_Widget);

    static QWidget* m_Me;
    static QGridLayout* m_Layout;
    static QList<I_ConfigurationContent*>* m_ContentsStack;

    static bool m_isInit;
};

#endif // CONFIGWIDGET_H
