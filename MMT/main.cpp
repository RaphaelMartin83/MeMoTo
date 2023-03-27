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

    MainWindow* l_MainWindow = nullptr;
    if( argc > 1 )
    {
        l_MainWindow = new MainWindow(argv[1]);
    }
    else
    {
        l_MainWindow = new MainWindow();
    }
    l_MainWindow->show();

    // Does not delete the main window because the process is ending anyway
    return l_Application.exec();
}
