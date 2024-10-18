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
quint32 MeMoToApplication::sm_Period(60000);
QString MeMoToApplication::sm_ServerIP("");
quint16 MeMoToApplication::sm_ServerPort = 0U; // Given port in headless mode
quint16 MeMoToApplication::sm_CollaborativePort(11310); // Default port on GUI
QList<I_DiagramContainer*> MeMoToApplication::sm_Diagrams;
bool MeMoToApplication::sm_hasChangesUnsaved = false;
bool MeMoToApplication::sm_isServer = false;
bool MeMoToApplication::sm_isInDisplayMode = false;
bool MeMoToApplication::sm_isReadOnly = false;
bool MeMoToApplication::sm_isAutoconnect = false;
QString MeMoToApplication::sm_FileName("");
QTimer* MeMoToApplication::sm_Timer = nullptr;

MainWindow* MeMoToApplication::sm_MW = nullptr;

MeMoToApplication::MeMoToApplication(int& argc, char** argv):
    QApplication(argc, argv)
{
    QCoreApplication::setApplicationName("MeMoTo");
    QCoreApplication::setApplicationVersion("0.2.0");

    this->createAndHandleArguments();

    sm_Timer = new QTimer(this);
    connect(sm_Timer, &QTimer::timeout, this, &MeMoToApplication::displayModeFileUpdateTick);
}

int MeMoToApplication::exec()
{
    createDiagrams();
    loadFileIfNeeded();

    // Each one of these methods will exit with their own error code
    runPNGDumpIfAsked();
    runAsServerIfAsked();
    runAsGUI();

    return 0;
}

QString MeMoToApplication::FileExtension()
{
    return QString(".memoto");
}
QString MeMoToApplication::SharedFileExtension()
{
    return QString(".memos");
}

bool MeMoToApplication::isServer()
{
    bool l_Ret = false;

    if( sm_isServer )
    {
        l_Ret = true;
    }

    return l_Ret;
}

bool MeMoToApplication::isReadOnly()
{
    return sm_isReadOnly;
}

const QString& MeMoToApplication::getDefaultDiagram()
{
    return sm_DefaultDiagram;
}

unsigned short MeMoToApplication::getDefaultDiagramID()
{
    // Switch to the default diagram
    bool l_Found = false;
    unsigned short l_defaultDiagram = 0U;
    for(unsigned short i_diagrams = 0U;
        (i_diagrams < sm_Diagrams.count()) && (false == l_Found);
        i_diagrams++ )
    {
        if( getDefaultDiagram() == sm_Diagrams[i_diagrams]->getSerializableName() )
        {
            l_defaultDiagram = i_diagrams;
            l_Found = true;
        }
    }

    return l_defaultDiagram;
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

    // After loading from a file, reset position of each diagram
    for( unsigned short i_diagrams = 0U; i_diagrams < sm_Diagrams.count(); i_diagrams++ )
    {
        sm_Diagrams[i_diagrams]->setCurrentPosition(
            sm_Diagrams[i_diagrams]->getStartPosition());
    }

    SharingManager::getInstance().pushModifications();
}

void MeMoToApplication::setFileName(const QString& p_FileName)
{
    sm_FileName = p_FileName;
}
const QString& MeMoToApplication::getFileName()
{
    return sm_FileName;
}

void MeMoToApplication::displayModeFileUpdateTick()
{
    MeMoToApplication::loadDiagrams();
}

void MeMoToApplication::createAndHandleArguments()
{
    QCommandLineParser l_Parser;
    l_Parser.setApplicationDescription("MErgeable MOdeling TOol: A DevOps oriented modeling tool");
    l_Parser.addHelpOption();
    l_Parser.addVersionOption();

    // The file to open
    l_Parser.addPositionalArgument("file", "File to open");

    // The options
    QCommandLineOption l_SvgOutputOption("output-svg",
                                        "MeMoTo will output a svg file from the given memoto file, use default-diagram option to output needed diagram",
                                        "Output svg file",
                                        "");
    l_Parser.addOption(l_SvgOutputOption);

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

    QCommandLineOption l_HeadlessOption("server",
                                        "MeMoTo will run headless and automatically open a session on given IP:PORT");
    l_Parser.addOption(l_HeadlessOption);

    QCommandLineOption l_DisplayerOption("displayer",
                                         "MeMoTo will run headless as for server, but read only and will refresh from the given file at the given period in milliseconds",
                                         "Activate displayer mode with refresh period, default is " + QString::number(sm_Period),
                                         "Period");
    l_Parser.addOption(l_DisplayerOption);

    QCommandLineOption l_ServerInterfaceOption("interface",
                                               "Mandatory if server is selected, will listen on the given IP address",
                                               "The IP address to listen to",
                                               "IPAddress");
    l_Parser.addOption(l_ServerInterfaceOption);

    QCommandLineOption l_ServerPortOption("port",
                                          "If server is selected, will listen on the given port",
                                          "The port to listen to, default is " + QString::number(sm_CollaborativePort),
                                          QString::number(sm_CollaborativePort));
    l_Parser.addOption(l_ServerPortOption);

    QCommandLineOption l_ClientOption("client",
                                      "MeMoTo will open the GUI and automatically connect to the given IP:PORT"
                                      "Activate client autoconnect");
    l_Parser.addOption(l_ClientOption);

    l_Parser.process(*this);

    // Retrieve value arguments
    sm_OutputString = l_Parser.value(l_SvgOutputOption);
    sm_DefaultDiagram = l_Parser.value(l_DefaultDiagram);
    sm_FocusOn = l_Parser.value(l_FocusOption);

    sm_isServer = l_Parser.isSet(l_HeadlessOption);
    sm_isInDisplayMode = l_Parser.isSet(l_DisplayerOption);
    sm_isAutoconnect = l_Parser.isSet(l_ClientOption);

    // Optionnal values
    sm_ServerIP = l_Parser.value(l_ServerInterfaceOption);
    sm_ServerPort = l_Parser.value(l_ServerPortOption).toInt();

    // Retrieve positionnal arguments
    QStringList l_PositionalArguments = l_Parser.positionalArguments();
    if( l_PositionalArguments.count() >= 1 )
    {
        sm_FileName = l_PositionalArguments[0];
    }

    // Check if optionnal values have been given depending on context
    if( sm_isServer || sm_isInDisplayMode || sm_isAutoconnect )
    {
        if( "" == sm_ServerIP )
        {
            qCritical("\nWhen activating server or client option, an interface IP address is needed");
            l_Parser.showHelp();
            exit(-1);
        }
    }

    if( sm_isInDisplayMode )
    {
        bool l_isPeriodOK = false;
        sm_Period = l_Parser.value(l_DisplayerOption).toUInt(&l_isPeriodOK);
        if(!l_isPeriodOK)
        {
            qCritical("Bad period given");
            l_Parser.showHelp();
            exit(-1);
        }
    }
}

void MeMoToApplication::loadFileIfNeeded()
{
    if( "" != sm_FileName )
    {
        loadDiagrams();
    }
}

void MeMoToApplication::focusItemIfAsked()
{
    if( "" == sm_FocusOn )
    {
        return;
    }

    Q_ASSERT(sm_MW);
    sm_MW->getCurrentDiagram();

    I_DiagramContainer* currentDiagram = sm_MW->getCurrentDiagram();
    Q_ASSERT(nullptr != currentDiagram);

    currentDiagram->focusOnItem("", "", sm_FocusOn, 0U, false, true);
}

void MeMoToApplication::autoConnectIfAsked()
{
    if(!sm_isAutoconnect)
    {
        return;
    }

    SharingManager::getInstance().sharingHostSelected(sm_ServerIP, sm_ServerPort);
}

void MeMoToApplication::runAsServerIfAsked()
{
    if( !isServer() )
    {
        return;
    }

    // Run the server mode
    QString l_debugString("Will start server at " + QString(sm_ServerIP) + " on server port " + QString::number(sm_ServerPort));
    qDebug() << l_debugString;
    SharingManager::getInstance().start(sm_ServerIP, sm_ServerPort);

    if( sm_isInDisplayMode )
    {
        Q_ASSERT(nullptr != sm_Timer);
        sm_Timer->start(sm_Period);
    }

    exit(QApplication::exec());
}

void MeMoToApplication::runAsGUI()
{
    // Fusion style is system style sensitive (dark or light)
    QApplication::setStyle("Fusion");

    initWindow();

    Q_ASSERT(nullptr != sm_MW);
    sm_MW->switchToContext(getDefaultDiagramID(), true);

    focusItemIfAsked();

    autoConnectIfAsked();

    // Regular run
    sm_MW->show();

    exit(QApplication::exec());
}

void MeMoToApplication::runPNGDumpIfAsked()
{
    if( "" == MeMoToApplication::getPNGToCreate() )
    {
        return;
    }

    // Need to generate PNG file
    exit(sm_Diagrams.at(getDefaultDiagramID())->printPressed(MeMoToApplication::getPNGToCreate()));
}

void MeMoToApplication::createDiagrams()
{
    // Creates the diagrams
    sm_Diagrams.append(new ClassDiagramScene());
    sm_Diagrams.append(new SMDiagramScene());
}

void MeMoToApplication::initWindow()
{
    Q_ASSERT(nullptr == sm_MW);

    // Initialize the main window
    sm_MW = new MainWindow();
    for(unsigned short i_diagrams = 0U; i_diagrams < sm_Diagrams.count(); i_diagrams++)
    {
        sm_MW->addDiagram(sm_Diagrams[i_diagrams]);
    }
}
