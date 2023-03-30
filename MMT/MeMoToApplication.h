#ifndef MEMOTOAPPLICATION_H
#define MEMOTOAPPLICATION_H

#include <QApplication>
#include <QString>

class MeMoToApplication: public QApplication
{
public:
    MeMoToApplication(int& argc, char** argv);

    static QString FileExtension();
    static QString SharedFileExtension();

    static bool isHeadless();
    static const QString& getDefaultDiagarm();
    static const QString& getFileToOpen();
    static const QString& getPNGToCreate();

private:
    static QString sm_OutputString;
    static QString sm_DefaultDiagram;
    static QString sm_FileToOpen;
};

#endif // MEMOTOAPPLICATION_H
