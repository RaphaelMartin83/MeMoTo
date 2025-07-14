#ifndef CONFIGWIDGET_H
#define CONFIGWIDGET_H

#include<QWidget>

#include<Engine/I_ConfigurationContent.h>

static bool ConfigWidget_isCloseCalled = false;

class ConfigWidget
{
public:
    static QWidget& getInstance()
    {
        static QWidget widget;
        return widget;
    }
    static void deleteInstance()
    {

    }

    ~ConfigWidget();

    static void open(I_ConfigurationContent* p_Widget)
    {

    }
    static void close()
    {
        ConfigWidget_isCloseCalled = true;
    }
};

#endif // CONFIGWIDGET_H
