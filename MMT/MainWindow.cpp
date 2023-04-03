#include <QGraphicsView>
#include <QKeyEvent>
#include <QJsonObject>
#include <QFile>

#include "MainWindow.h"
#include "DiagramGraphicsView.h"
#include "ConfigWidget.h"
#include "SaveFileConfiguration.h"
#include "LoadFileConfiguration.h"
#include "SharingManager.h"
#include "MeMoToApplication.h"
#include "MeMoToLoader.h"

#include "SMDiagramScene.h"
#include "ClassDiagramScene.h"

static const char* s_ProgramName = "MeMoTo";

static SaveFileConfiguration* s_SaveFileConfiguration = nullptr;
static LoadFileConfiguration* s_LoadFileConfiguration = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_mainLayout(nullptr)
    , m_centralWidget(nullptr)
    , m_ToolBoxView(nullptr)
    , m_DiagramView(nullptr)
    , m_Diagrams()
    , m_CurrentDiagramID(0U)
    , m_SerializablesIndexes()
    , m_FileName()
{
    this->initGUI();

    this->initDiagrams();

    if( "" != MeMoToApplication::getFileToOpen() )
    {
        MainWindow::fileSelectedForLoading(MeMoToApplication::getFileToOpen());
    }
}

MainWindow::MainWindow(const char* argv, QWidget *parent)
    : MainWindow(parent)
{
    this->fileSelectedForLoading(QString(argv));
}

MainWindow::~MainWindow()
{
    m_mainLayout->removeWidget(&ConfigWidget::getInstance());
    ConfigWidget::deleteInstance();
}

I_DiagramContainer* MainWindow::getCurrentDiagram()
{
    return m_Diagrams[m_CurrentDiagramID];
}

// I_SaveFileConfigurationListener
void MainWindow::fileSelectedForSaving(QString p_File)
{
    m_FileName = p_File;

    ConfigWidget::close();

    this->saveDiagrams();
    this->updateTitle();
}
void MainWindow::fileSavingCanceled()
{
    ConfigWidget::close();
}

void MainWindow::fileSelectedForLoading(QString p_File)
{
    if( QFile(p_File).exists() )
    {
        m_FileName = p_File;

        ConfigWidget::close();

        this->loadDiagrams();
        this->updateTitle();
    }
}
void MainWindow::fileLoadingCanceled()
{
    ConfigWidget::close();
}

void MainWindow::getApplicationData(QJsonObject& p_rData) const
{
    p_rData = QJsonObject();
    QJsonObject l_JsonDiag;
    for( unsigned short i_diagrams = 0U; i_diagrams < m_Diagrams.count(); i_diagrams++ )
    {
        l_JsonDiag = m_Diagrams[i_diagrams]->toJson();
        if( l_JsonDiag.count() != 0 )
        {
            p_rData.insert(m_SerializablesIndexes[i_diagrams],
            l_JsonDiag);
        }
    }
}
void MainWindow::setApplicationData(const QJsonObject& p_Data)
{
    for( unsigned short i_diagrams = 0U; i_diagrams < m_Diagrams.count(); i_diagrams++ )
    {
        QJsonObject::const_iterator l_foundDiagramJson = p_Data.find(m_SerializablesIndexes[i_diagrams]);
        if( p_Data.end() != l_foundDiagramJson )
        {
            m_Diagrams[i_diagrams]->clearAll();
            m_Diagrams[i_diagrams]->fromJson(l_foundDiagramJson->toObject());
            m_Diagrams[i_diagrams]->saveUndoState();
        }
    }
}

void MainWindow::startSharing()
{
    SharingManager::getInstance().registerDataManager(this);
    SharingManager::getInstance().start();
}

void MainWindow::initGUI()
{
    QIcon l_ApplicationIcon = QIcon("logo.ico");
    this->setWindowIcon(l_ApplicationIcon);

    m_mainLayout = new QGridLayout();

    m_ToolBoxView = new QGraphicsView();
    m_ToolBoxView->setFixedWidth(100);
    m_DiagramView = new DiagramGraphicsView();
    m_DiagramView->setMinimumWidth(700);
    m_DiagramView->setMinimumHeight(700);
    m_DiagramView->setMouseTracking(true);

    ConfigWidget::getInstance().setFixedWidth(200);

    m_mainLayout->addWidget(m_ToolBoxView, 0, 0, 1, 1, Qt::Alignment());
    m_mainLayout->addWidget(m_DiagramView, 0, 1, 1, 1, Qt::Alignment());
    m_mainLayout->addWidget(&ConfigWidget::getInstance(), 0, 2, 1, 1, Qt::Alignment());

    m_centralWidget = new QWidget();
    m_centralWidget->setLayout(m_mainLayout);

    this->setCentralWidget(m_centralWidget);
    this->setWindowTitle(s_ProgramName);
    this->move(300, 150);
}
void MainWindow::updateTitle()
{
    static const unsigned short MAX_DISPLAYED_NAME_SIZE = 100;
    QString l_newTitle = s_ProgramName;
    if( "" != m_FileName )
    {
        l_newTitle += " on ";
        if( m_FileName.size() > MAX_DISPLAYED_NAME_SIZE )
        {
            l_newTitle += ".." + m_FileName.last(MAX_DISPLAYED_NAME_SIZE);
        }
        else
        {
            l_newTitle += m_FileName;
        }
    }
    l_newTitle += " (" + m_Diagrams[m_CurrentDiagramID]->getDiagramString() + ")";
    this->setWindowTitle(l_newTitle);
}
void MainWindow::initDiagrams()
{
    // Initialize all the diagrams
    m_Diagrams.append(new ClassDiagramScene);
    m_Diagrams.last()->registerDiagramView(m_DiagramView);
    m_SerializablesIndexes.append(m_Diagrams.last()->getSerializableName());
    m_Diagrams.append(new SMDiagramScene);
    m_Diagrams.last()->registerDiagramView(m_DiagramView);
    m_SerializablesIndexes.append(m_Diagrams.last()->getSerializableName());

    // Displays the default diagram
    bool l_Found = false;
    for(unsigned short i_diagrams = 0U;
        (i_diagrams < m_Diagrams.count()) && (false == l_Found);
        i_diagrams++ )
    {
        if( MeMoToApplication::getDefaultDiagarm() == m_Diagrams[i_diagrams]->getSerializableName() )
        {
            this->switchToContext(i_diagrams, true);
            l_Found = true;
        }
    }
    if( !l_Found )
    {
        this->switchToContext(0U, true);
    }
}

void MainWindow::saveDiagrams()
{
    QJsonObject l_JSonGlobal;
    this->getApplicationData(l_JSonGlobal);

    QFile l_File(m_FileName);
    MeMoToLoader::saveToFile(l_File, l_JSonGlobal);
}

void MainWindow::loadDiagrams()
{
    QFile l_File(m_FileName);

    QJsonObject l_JsonObject = MeMoToLoader::loadFromFile(l_File);

    MainWindow::setApplicationData(l_JsonObject);
}

void MainWindow::switchToContext(unsigned short p_ContextID, bool p_Force)
{
    if( (true == p_Force) || (p_ContextID != m_CurrentDiagramID) )
    {
        m_Diagrams[m_CurrentDiagramID]->setUnfocused();

        if( p_ContextID < m_Diagrams.count() )
        {
            m_CurrentDiagramID = p_ContextID;
        }
        else
        {
            m_CurrentDiagramID = m_Diagrams.count() - 1U;
        }

        m_Diagrams[m_CurrentDiagramID]->showDiagram();
        m_ToolBoxView->setScene(m_Diagrams[m_CurrentDiagramID]->getToolBox());

        this->updateTitle();
    }
    else
    {
        // Do nothing :)
    }
}

// Mappings for all the application's shorcuts
void MainWindow::keyPressEvent(QKeyEvent* p_Event)
{
    switch(p_Event->key())
    {
    case Qt::Key::Key_Tab:
        if( p_Event->modifiers() == Qt::ControlModifier )
        {
            unsigned short l_DiagramToDisplay = m_CurrentDiagramID;
            l_DiagramToDisplay++;
            if( l_DiagramToDisplay >= m_Diagrams.count() )
            {
                l_DiagramToDisplay = 0U;
            }
            this->switchToContext(l_DiagramToDisplay);
        }
        break;
    case Qt::Key::Key_F1:
        if( p_Event->modifiers() == Qt::AltModifier )
        {
            this->switchToContext(0U);
        }
        else
        {
            m_Diagrams[m_CurrentDiagramID]->selectToolByID(0U);
        }
        break;
    case Qt::Key::Key_F2:
        if( p_Event->modifiers() == Qt::AltModifier )
        {
            this->switchToContext(1U);
        }
        else
        {
            m_Diagrams[m_CurrentDiagramID]->selectToolByID(1U);
        }
        break;
    case Qt::Key::Key_F3:
        if( p_Event->modifiers() == Qt::AltModifier )
        {
            this->switchToContext(2U);
        }
        else
        {
            m_Diagrams[m_CurrentDiagramID]->selectToolByID(2U);
        }
        break;
    case Qt::Key::Key_F4:
        if( p_Event->modifiers() == Qt::AltModifier )
        {
            this->switchToContext(3U);
        }
        else
        {
            m_Diagrams[m_CurrentDiagramID]->selectToolByID(3U);
        }
        break;
    case Qt::Key::Key_F5:
        if( p_Event->modifiers() == Qt::AltModifier )
        {
            this->switchToContext(4U);
        }
        else
        {
            m_Diagrams[m_CurrentDiagramID]->selectToolByID(4U);
        }
        break;
    case Qt::Key::Key_F6:
        if( p_Event->modifiers() == Qt::AltModifier )
        {
            this->switchToContext(5U);
        }
        else
        {
            m_Diagrams[m_CurrentDiagramID]->selectToolByID(5U);
        }
        break;
    case Qt::Key::Key_F7:
        if( p_Event->modifiers() == Qt::AltModifier )
        {
            this->switchToContext(6U);
        }
        else
        {
            m_Diagrams[m_CurrentDiagramID]->selectToolByID(6U);
        }
        break;
    case Qt::Key::Key_F8:
        if( p_Event->modifiers() == Qt::AltModifier )
        {
            this->switchToContext(7U);
        }
        else
        {
            m_Diagrams[m_CurrentDiagramID]->selectToolByID(7U);
        }
        break;
    case Qt::Key::Key_Delete:
        m_Diagrams[m_CurrentDiagramID]->deletePressed();
        break;
    case Qt::Key::Key_Escape:
        m_Diagrams[m_CurrentDiagramID]->escapePressed();
        break;
    default:
        break;
    }

    if( p_Event->matches(QKeySequence::Save) )
    {
        this->savePressed();
    }
    else if( p_Event->matches(QKeySequence::SaveAs) )
    {
        this->savePressed(true);
    }
    else if( (p_Event->key() == Qt::Key_O) && (p_Event->modifiers() == Qt::ControlModifier) )
    {
        this->loadPressed();
    }
    else if( (p_Event->key() == Qt::Key_O) && (p_Event->modifiers() && Qt::ControlModifier) && (p_Event->modifiers() && Qt::ShiftModifier) )
    {
        this->loadPressed(true);
    }
    else if( p_Event->matches(QKeySequence::Print) )
    {
        m_Diagrams[m_CurrentDiagramID]->printPressed("");
    }
    else if( p_Event->matches(QKeySequence::Copy) )
    {
        m_Diagrams[m_CurrentDiagramID]->copyPressed();
    }
    else if( p_Event->matches(QKeySequence::Paste) )
    {
        m_Diagrams[m_CurrentDiagramID]->pastePressed();
    }
    else if( p_Event->matches(QKeySequence::Undo) )
    {
        m_Diagrams[m_CurrentDiagramID]->clearAll();
        m_Diagrams[m_CurrentDiagramID]->undo();
    }
    else if( p_Event->matches(QKeySequence::Redo) )
    {
        m_Diagrams[m_CurrentDiagramID]->clearAll();
        m_Diagrams[m_CurrentDiagramID]->redo();
    }
    else if( p_Event->matches(QKeySequence::Find) )
    {
        m_Diagrams[m_CurrentDiagramID]->find();
    }
    else if( (p_Event->key() == Qt::Key_Space) && (p_Event->modifiers() == Qt::ControlModifier) )
    {
        this->startSharing();
    }
}

void MainWindow::savePressed(bool p_alwaysOpen)
{
    if( ("" != m_FileName) && (false == p_alwaysOpen) )
    {
        this->saveDiagrams();
    }
    else
    {
        // Open save configuration
        if( nullptr == s_SaveFileConfiguration )
        {
            s_SaveFileConfiguration = new SaveFileConfiguration();
            s_SaveFileConfiguration->registerConfigListener(this);
        }
        s_SaveFileConfiguration->setFileName("MyClass");
        ConfigWidget::open(s_SaveFileConfiguration);
    }
}

void MainWindow::loadPressed(bool p_alwaysOpen)
{
    if( ("" != m_FileName) && (false == p_alwaysOpen) )
    {
        this->loadDiagrams();
    }
    else
    {
        // Open load configuration
        if( nullptr == s_LoadFileConfiguration )
        {
            s_LoadFileConfiguration = new LoadFileConfiguration();
            s_LoadFileConfiguration->registerConfigListener(this);
        }
        ConfigWidget::open(s_LoadFileConfiguration);
    }
}
