#include <QGuiApplication>
#include <QScreen>
#include <QLocale>
#include <QTranslator>

#include "MainWindow.h"

#include "MeMoToApplication.h"

int main(int argc, char** argv)
{
    MeMoToApplication l_Application(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "MMT_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            l_Application.installTranslator(&translator);
            break;
        }
    }

    bool l_isHeadless = l_Application.isHeadless();

    MainWindow* l_MainWindow = new MainWindow();

    if( false == l_isHeadless )
    {
        l_MainWindow->show();
        return l_Application.exec();
    }
    else
    {
        if( "" != MeMoToApplication::getPNGToCreate() )
        {
            // Need to generate PNG file
            l_MainWindow->getCurrentDiagram()->printPressed(MeMoToApplication::getPNGToCreate());
        }
    }
}
