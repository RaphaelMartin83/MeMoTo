#include <QGuiApplication>
#include <QScreen>
#include <QLocale>
#include <QTranslator>

#include "MainWindow.h"

#include "MeMoToApplication.h"

int main(int argc, char** argv)
{
    MeMoToApplication l_Application(argc, argv);

    // Note: Translator is not used yet
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "MMT_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            l_Application.installTranslator(&translator);
            break;
        }
    }

    MainWindow* l_MainWindow = new MainWindow();
    MeMoToApplication::registerMainWindow(l_MainWindow);

    return MeMoToApplication::exec();
}
