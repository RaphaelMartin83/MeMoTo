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
};

#endif // MEMOTOAPPLICATION_H
