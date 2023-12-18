#include <QCommandLineParser>
#include <QFile>

#include <Engine/MeMoToLoader.h>
#include <Sharing/SharingManager.h>

#include <DiagramScenes/ClassDiagramScene.h>
#include <DiagramScenes/SMDiagramScene.h>

#include "MeMoToApplication.h"

QString MeMoToApplication::sm_DefaultDiagram("");
QString MeMoToApplication::sm_OutputString("");
QString MeMoToApplication::sm_FocusOn("");
QString MeMoToApplication::sm_ServerIP("");
quint16 MeMoToApplication::sm_ServerPort = 0U; // Given port in headless mode
quint16 MeMoToApplication::sm_CollaborativePort(11310); // Default port on GUI
QList<I_DiagramContainer*> MeMoToApplication::sm_Diagrams;
bool MeMoToApplication::sm_hasChangesUnsaved = false;
bool MeMoToApplication::sm_isHeadless = false;
QString MeMoToApplication::sm_FileName("");

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

    QCommandLineOption l_HeadlessOption("server", "MeMoTo will run headless and automatically open a session on given IP:PORT",
                                        "Activate server mode");
    l_Parser.addOption(l_HeadlessOption);

    QCommandLineOption l_ServerInterfaceOption("interface", "Mandatory if server is selected, will listen on the given IP address",
                                               "The IP address to listen to", "IPAddress");
    l_Parser.addOption(l_ServerInterfaceOption);

    QCommandLineOption l_ServerPortOption("port", "If server is selected, will listen on the given port",
                                          "The port to listen to, default is " + QString::number(sm_CollaborativePort), "Port");
    l_Parser.addOption(l_ServerPortOption);

    l_Parser.process(*this);

    // Retrieve value arguments
    sm_OutputString = l_Parser.value(l_PngOutputOption);
    sm_DefaultDiagram = l_Parser.value(l_DefaultDiagram);
    sm_FocusOn = l_Parser.value(l_FocusOption);

    // Get into this loop if it's server mode
    if(l_Parser.isSet(l_HeadlessOption))
    {
        sm_isHeadless = true;
        if(!l_Parser.isSet(l_ServerInterfaceOption))
        {
            qDebug("\nWhen activating server option, an interface IP address is needed");
            l_Parser.showHelp();
            exit(-1);
        }
        else
        {
            sm_ServerIP = l_Parser.value(l_ServerInterfaceOption);
            if(l_Parser.isSet(l_ServerPortOption))
            {
                sm_ServerPort = l_Parser.value(l_ServerPortOption).toInt();
            }
            else
            {
                sm_ServerPort = sm_CollaborativePort;
            }
        }
    }

    // Retrieve positionnal arguments
    QStringList l_PositionalArguments = l_Parser.positionalArguments();
    if( l_PositionalArguments.count() >= 1 )
    {
        sm_FileName = l_PositionalArguments[0];
    }
}

int MeMoToApplication::exec()
{
    int l_Ret = -1;

    // Creates the diagrams
    sm_Diagrams.append(new ClassDiagramScene());
    sm_Diagrams.append(new SMDiagramScene());

    if( "" != sm_FileName )
    {
        loadDiagrams();
    }

    // Switch to the default diagram
    bool l_Found = false;
    unsigned short l_defaultDiagram = 0U;
    for(unsigned short i_diagrams = 0U;
        (i_diagrams < sm_Diagrams.count()) && (false == l_Found);
        i_diagrams++ )
    {
        if( getDefaultDiagarm() == sm_Diagrams[i_diagrams]->getSerializableName() )
        {
            l_defaultDiagram = i_diagrams;
            l_Found = true;
        }
    }

    if( !MeMoToApplication::isHeadless() )
    {
        // Initialize the main window
        sm_MW = new MainWindow();
        for(unsigned short i_diagrams = 0U; i_diagrams < sm_Diagrams.count(); i_diagrams++)
        {
            sm_MW->addDiagram(sm_Diagrams[i_diagrams]);
        }

        sm_MW->switchToContext(l_defaultDiagram, true);
        if( "" != sm_FocusOn )
        {
            sm_MW->getCurrentDiagram()->focusOnItem("", "", sm_FocusOn, 0U, false, true);
        }

        // Regular run
        sm_MW->show();

        l_Ret = QApplication::exec();
    }
    else
    {
        // Headless run
        if( "" != MeMoToApplication::getPNGToCreate() )
        {
            // Need to generate PNG file
            l_Ret = sm_Diagrams.at(l_defaultDiagram)->printPressed(MeMoToApplication::getPNGToCreate());
        }

        if( isHeadless() )
        {
            // Run the server mode
            QString l_debugString("Will start server at " + QString(sm_ServerIP) + " on server port " + QString::number(sm_ServerPort));
            SharingManager::getInstance().start(sm_ServerIP, sm_ServerPort);
            //SharingManager::getInstance().start("127.0.0.1", 4242);
            l_Ret = QApplication::exec();
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

    if( QString("") != sm_OutputString || sm_isHeadless )
    {
        l_Ret = true;
    }

    return l_Ret;
}

const QString& MeMoToApplication::getDefaultDiagarm()
{
    return sm_DefaultDiagram;
}

const QString& MeMoToApplication::getPNGToCreate()
{
    return sm_OutputString;
}

const QString& MeMoToApplication::getServerIP()
{
    return sm_ServerIP;
}

const quint16& MeMoToApplication::getServerPort()
{
    return sm_ServerPort;
}

const quint16& MeMoToApplication::getCollaborativePort()
{
    return sm_CollaborativePort;
}

QIcon MeMoToApplication::getLogo()
{
    QIcon l_Ret = QIcon(QApplication::applicationDirPath() + "/MeMoTo-icon.png");

    return l_Ret;
}

void MeMoToApplication::getApplicationData(QJsonObject& p_rData)
{
    p_rData = QJsonObject();
    QJsonObject l_JsonDiag;
    for( unsigned short i_diagrams = 0U; i_diagrams < sm_Diagrams.count(); i_diagrams++ )
    {
        l_JsonDiag = sm_Diagrams[i_diagrams]->toJson();
        if( l_JsonDiag.count() != 0 )
        {
            p_rData.insert(sm_Diagrams[i_diagrams]->getSerializableName(),
            l_JsonDiag);
        }
    }
}
void MeMoToApplication::setApplicationData(const QJsonObject& p_Data)
{
    for( unsigned short i_diagrams = 0U; i_diagrams < sm_Diagrams.count(); i_diagrams++ )
    {
        QJsonObject::const_iterator l_foundDiagramJson = p_Data.find(sm_Diagrams[i_diagrams]->getSerializableName());
        if( p_Data.end() != l_foundDiagramJson )
        {
            sm_Diagrams[i_diagrams]->clearAll();
            sm_Diagrams[i_diagrams]->fromJson(l_foundDiagramJson->toObject());
        }
        else
        {
            sm_Diagrams[i_diagrams]->clearAll();
        }
        sm_Diagrams[i_diagrams]->saveUndoState();
    }
}

void MeMoToApplication::diagramChanged()
{
    sm_hasChangesUnsaved = true;
    sm_MW->updateTitle();
}

bool MeMoToApplication::hasChangesUnsaved()
{
    return sm_hasChangesUnsaved;
}

void MeMoToApplication::startSharing()
{
    SharingManager::getInstance().start();
}

void MeMoToApplication::saveDiagrams()
{
    QJsonObject l_JSonGlobal;
    MeMoToApplication::getApplicationData(l_JSonGlobal);

    QFile l_File(sm_FileName);
    MeMoToLoader::saveToFile(l_File, l_JSonGlobal);

    sm_hasChangesUnsaved = false;

    sm_MW->updateTitle();
}

void MeMoToApplication::loadDiagrams()
{
    QFile l_File(sm_FileName);

    QJsonObject l_JsonObject = MeMoToLoader::loadFromFile(l_File);

    setApplicationData(l_JsonObject);
}

void MeMoToApplication::setFileName(const QString& p_FileName)
{
    sm_FileName = p_FileName;
}
const QString& MeMoToApplication::getFileName()
{
    return sm_FileName;
}
