#ifndef MEMOTOAPPLICATION_H
#define MEMOTOAPPLICATION_H

#include <QApplication>
#include <QString>
#include <MainWindow.h>

class MeMoToApplication: public QApplication
{
public:
    MeMoToApplication(int& argc, char** argv);

    // QApplication override
    static int exec();

    static QString FileExtension();
    static QString SharedFileExtension();

    static bool isHeadless();
    static const QString& getDefaultDiagarm();
    static const QString& getFileToOpen();
    static const QString& getPNGToCreate();

    static void registerMainWindow(MainWindow* p_MW);

private:
    static QString sm_OutputString;
    static QString sm_DefaultDiagram;
    static QString sm_FileToOpen;
    static QString sm_FocusOn;

    static MainWindow* sm_MW;
};

#endif // MEMOTOAPPLICATION_H
