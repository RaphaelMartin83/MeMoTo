#include <QCommandLineParser>

#include "MeMoToApplication.h"

QString MeMoToApplication::sm_DefaultDiagram("");
QString MeMoToApplication::sm_OutputString("");
QString MeMoToApplication::sm_FileToOpen("");

MeMoToApplication::MeMoToApplication(int& argc, char** argv):
    QApplication(argc, argv)
{
    QCoreApplication::setApplicationName("MeMoTo");
    QCoreApplication::setApplicationVersion("0.2.0");

    QCommandLineParser l_Parser;
    l_Parser.setApplicationDescription("MErgeable MOdeling TOol: A DevOps oriented modeling tool");
    l_Parser.addHelpOption();
    l_Parser.addVersionOption();

    // The file to open
    l_Parser.addPositionalArgument("file", "File to open");

    // The options
    QCommandLineOption l_PngOutputOption("output-png",
                                        "MeMoTo will output a png file from the given memoto file, use default-diagram option to output needed diagram",
                                        "Output png file",
                                        "");
    l_Parser.addOption(l_PngOutputOption);

    QCommandLineOption l_DefaultDiagram("default-diagram",
                                        "MeMoTo will open on the given diagram, can be ClassDiagram or StateMachine",
                                        "Default diagram",
                                        "ClassDiagram");
    l_Parser.addOption(l_DefaultDiagram);

    l_Parser.process(*this);

    // Retrieve value arguments
    sm_OutputString = l_Parser.value(l_PngOutputOption);
    sm_DefaultDiagram = l_Parser.value(l_DefaultDiagram);

    // Retrieve positionnal arguments
    QStringList l_PositionalArguments = l_Parser.positionalArguments();
    if( l_PositionalArguments.count() >= 1 )
    {
        sm_FileToOpen = l_PositionalArguments[0];
    }
}

QString MeMoToApplication::FileExtension()
{
    return QString(".memoto");
}
QString MeMoToApplication::SharedFileExtension()
{
    return QString(".memos");
}

bool MeMoToApplication::isHeadless()
{
    bool l_Ret = false;

    if( QString("") != sm_OutputString )
    {
        l_Ret = true;
    }

    return l_Ret;
}

const QString& MeMoToApplication::getDefaultDiagarm()
{
    return sm_DefaultDiagram;
}

const QString& MeMoToApplication::getFileToOpen()
{
    return sm_FileToOpen;
}

const QString& MeMoToApplication::getPNGToCreate()
{
    return sm_OutputString;
}
