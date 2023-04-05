#include <QCommandLineParser>

#include "MeMoToApplication.h"

QString MeMoToApplication::sm_DefaultDiagram("");
QString MeMoToApplication::sm_OutputString("");
QString MeMoToApplication::sm_FileToOpen("");
QString MeMoToApplication::sm_FocusOn("");
quint16 MeMoToApplication::sm_CollaborativePort(11310);

MainWindow* MeMoToApplication::sm_MW = nullptr;

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

    QCommandLineOption l_FocusOption("focus-on",
                                     "MeMoTo will open the application on the first occurence of given value in default diagram displayed (see default-diagram option)",
                                     "Focus on item",
                                     "");
    l_Parser.addOption(l_FocusOption);

    l_Parser.process(*this);

    // Retrieve value arguments
    sm_OutputString = l_Parser.value(l_PngOutputOption);
    sm_DefaultDiagram = l_Parser.value(l_DefaultDiagram);
    sm_FocusOn = l_Parser.value(l_FocusOption);

    // Retrieve positionnal arguments
    QStringList l_PositionalArguments = l_Parser.positionalArguments();
    if( l_PositionalArguments.count() >= 1 )
    {
        sm_FileToOpen = l_PositionalArguments[0];
    }
}

int MeMoToApplication::exec()
{
    int l_Ret = -1;

    if( false == MeMoToApplication::isHeadless())
    {
        // Regular run
        sm_MW->show();
        if( "" != sm_FocusOn )
        {
            sm_MW->getCurrentDiagram()->focusOnItem("", "", sm_FocusOn, 0U, false, true);
        }
        l_Ret = QApplication::exec();
    }
    else
    {
        // Headless run
        if( "" != MeMoToApplication::getPNGToCreate() )
        {
            // Need to generate PNG file
            l_Ret = sm_MW->getCurrentDiagram()->printPressed(MeMoToApplication::getPNGToCreate());
        }
    }
    return l_Ret;
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

const quint16& MeMoToApplication::getCollaborativePort()
{
    return sm_CollaborativePort;
}

void MeMoToApplication::registerMainWindow(MainWindow* p_MW)
{
    sm_MW = p_MW;
}
