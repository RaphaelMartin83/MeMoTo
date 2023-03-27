#include "MeMoToApplication.h"

MeMoToApplication::MeMoToApplication(int& argc, char** argv):
    QApplication(argc, argv)
{

}

QString MeMoToApplication::FileExtension()
{
    return QString(".memoto");
}
QString MeMoToApplication::SharedFileExtension()
{
    return QString(".memos");
}
