#include <QGraphicsView>
#include <QKeyEvent>
#include <QJsonObject>
#include <QFile>

#include <MeMoToApplication.h>

#include <Engine/DiagramGraphicsView.h>
#include <Engine/MeMoToLoader.h>
#include <Engine/InstanceLauncher.h>

#include <CommonGraphics/ConfigWidget.h>

#include <ConfigurationContexts/SaveFileConfiguration.h>
#include <ConfigurationContexts/LoadFileConfiguration.h>
#include <ConfigurationContexts/CloseWithoutSavingConfiguration.h>

#include <Sharing/SharingManager.h>

#include <DiagramScenes/SMDiagramScene.h>
#include <DiagramScenes/ClassDiagramScene.h>

#include "MainWindow.h"

static const char* s_ProgramName = "MeMoTo";

static SaveFileConfiguration* s_SaveFileConfiguration = nullptr;
static LoadFileConfiguration* s_LoadFileConfiguration = nullptr;
static CloseWithoutSavingConfiguration* s_CloseConfiguration = nullptr;

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
    , m_hasChangesUnsaved(false)
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
        }
        else
        {
            m_Diagrams[i_diagrams]->clearAll();
        }
        m_Diagrams[i_diagrams]->saveUndoState();
    }
}

void MainWindow::diagramChanged()
{
    m_hasChangesUnsaved = true;
    this->updateTitle();
}

void MainWindow::saveBeforeClosing()
{
    ConfigWidget::close();

    this->savePressed(true);
}
void MainWindow::closeAndDropChanges()
{
    MeMoToApplication::exit();
}

void MainWindow::startSharing()
{
    SharingManager::getInstance().registerDataManager(this);
    SharingManager::getInstance().start();
}

void MainWindow::NextButtonPressed()
{
    unsigned short l_DiagramToDisplay = m_CurrentDiagramID;
    l_DiagramToDisplay++;
    if( l_DiagramToDisplay >= m_Diagrams.count() )
    {
        l_DiagramToDisplay = 0U;
    }
    this->switchToContext(l_DiagramToDisplay);
}
void MainWindow::PrevButtonPressed()
{
    unsigned short l_DiagramToDisplay = m_CurrentDiagramID;
    l_DiagramToDisplay--;
    if( l_DiagramToDisplay >= m_Diagrams.count() )
    {
        l_DiagramToDisplay = m_Diagrams.count() - 1U;
    }
    this->switchToContext(l_DiagramToDisplay);
}
void MainWindow::newMenuClicked()
{
    InstanceLauncher::getInstance().execute(QStringList());
}
void MainWindow::openMenuClicked()
{
    this->loadPressed();
}
void MainWindow::openNewMenuClicked()
{
    this->loadPressed(true);
}
void MainWindow::saveMenuClicked()
{
    this->savePressed();
}
void MainWindow::saveAsMenuClicked()
{
    this->savePressed(true);
}
void MainWindow::undoMenuClicked()
{
    m_Diagrams[m_CurrentDiagramID]->clearAll();
    m_Diagrams[m_CurrentDiagramID]->undo();
}
void MainWindow::redoMenuClicked()
{
    m_Diagrams[m_CurrentDiagramID]->clearAll();
    m_Diagrams[m_CurrentDiagramID]->redo();
}
void MainWindow::copyMenuClicked()
{
    m_Diagrams[m_CurrentDiagramID]->copyPressed();
}
void MainWindow::pasteMenuClicked()
{
    m_Diagrams[m_CurrentDiagramID]->pastePressed();
}
void MainWindow::shareMenuClicked()
{
    this->startSharing();
}
void MainWindow::findMenuClicked()
{
    m_Diagrams[m_CurrentDiagramID]->find();
}
void MainWindow::printMenuClicked()
{
    m_Diagrams[m_CurrentDiagramID]->printPressed("");
}

void MainWindow::closeEvent(QCloseEvent* p_event)
{
    if( m_hasChangesUnsaved )
    {
        p_event->ignore();

        if( nullptr == s_CloseConfiguration )
        {
            s_CloseConfiguration = new CloseWithoutSavingConfiguration();
            s_CloseConfiguration->registerListener(this);
        }
        ConfigWidget::open(s_CloseConfiguration);
    }
}

void MainWindow::initGUI()
{
    this->setWindowIcon(MeMoToApplication::getLogo());

    m_mainLayout = new QGridLayout();

    m_PrevDiagButton = new QPushButton("<-");
    m_PrevDiagButton->setFixedWidth(47);
    m_NextDiagButton = new QPushButton("->");
    m_NextDiagButton->setFixedWidth(47);

    m_ToolBoxView = new QGraphicsView();
    m_ToolBoxView->setFixedWidth(100);
    m_DiagramView = new DiagramGraphicsView();
    m_DiagramView->setMinimumWidth(700);
    m_DiagramView->setMinimumHeight(700);
    m_DiagramView->setMouseTracking(true);

    ConfigWidget::getInstance().setFixedWidth(200);

    m_mainLayout->addWidget(m_ToolBoxView, 0, 0, 1, 2, Qt::Alignment());
    m_mainLayout->addWidget(m_PrevDiagButton, 1, 0, 1, 1, Qt::Alignment());
    m_mainLayout->addWidget(m_NextDiagButton, 1, 1, 1, 1, Qt::Alignment());
    m_mainLayout->addWidget(m_DiagramView, 0, 2, -1, 1, Qt::Alignment());
    m_mainLayout->addWidget(&ConfigWidget::getInstance(), 0, 3, -1, 1, Qt::Alignment());

    m_centralWidget = new QWidget();
    m_centralWidget->setLayout(m_mainLayout);

    this->setCentralWidget(m_centralWidget);
    this->setWindowTitle(s_ProgramName);
    this->move(300, 150);

    m_MenuBar = this->menuBar();
    m_FileMenu = new QMenu("File");
    QList<QAction*> l_ActionsListFile;
    l_ActionsListFile.append(new QAction("New"));
    l_ActionsListFile.append(new QAction("Open"));
    l_ActionsListFile.append(new QAction("Open new"));
    l_ActionsListFile.append(new QAction("Save"));
    l_ActionsListFile.append(new QAction("Save as"));
    m_FileMenu->addActions(l_ActionsListFile);
    m_EditMenu = new QMenu("Edit");
    QList<QAction*> l_ActionsListEdit;
    l_ActionsListEdit.append(new QAction("Undo"));
    l_ActionsListEdit.append(new QAction("Redo"));
    l_ActionsListEdit.append(new QAction("Copy"));
    l_ActionsListEdit.append(new QAction("Paste"));
    l_ActionsListEdit.append(new QAction("Share"));
    l_ActionsListEdit.append(new QAction("Find"));
    l_ActionsListEdit.append(new QAction("Print"));;
    m_EditMenu->addActions(l_ActionsListEdit);
    m_MenuBar->addMenu(m_FileMenu);
    m_MenuBar->addMenu(m_EditMenu);

    connect(m_PrevDiagButton, &QPushButton::pressed, this, &MainWindow::PrevButtonPressed);
    connect(m_NextDiagButton, &QPushButton::pressed, this, &MainWindow::NextButtonPressed);

    connect(m_FileMenu->actions().at(0), &QAction::triggered, this, &MainWindow::newMenuClicked);
    connect(m_FileMenu->actions().at(1), &QAction::triggered, this, &MainWindow::openMenuClicked);
    connect(m_FileMenu->actions().at(2), &QAction::triggered, this, &MainWindow::openNewMenuClicked);
    connect(m_FileMenu->actions().at(3), &QAction::triggered, this, &MainWindow::saveMenuClicked);
    connect(m_FileMenu->actions().at(4), &QAction::triggered, this, &MainWindow::saveAsMenuClicked);

    connect(m_EditMenu->actions().at(0), &QAction::triggered, this, &MainWindow::undoMenuClicked);
    connect(m_EditMenu->actions().at(1), &QAction::triggered, this, &MainWindow::redoMenuClicked);
    connect(m_EditMenu->actions().at(2), &QAction::triggered, this, &MainWindow::copyMenuClicked);
    connect(m_EditMenu->actions().at(3), &QAction::triggered, this, &MainWindow::pasteMenuClicked);
    connect(m_EditMenu->actions().at(4), &QAction::triggered, this, &MainWindow::shareMenuClicked);
    connect(m_EditMenu->actions().at(5), &QAction::triggered, this, &MainWindow::findMenuClicked);
    connect(m_EditMenu->actions().at(6), &QAction::triggered, this, &MainWindow::printMenuClicked);
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

    if( m_hasChangesUnsaved )
    {
        l_newTitle += "*";
    }

    this->setWindowTitle(l_newTitle);
}
void MainWindow::initDiagrams()
{
    // Initialize all the diagrams
    m_Diagrams.append(new ClassDiagramScene);
    m_Diagrams.last()->registerDiagramView(m_DiagramView);
    m_Diagrams.last()->registerDiagramListener(static_cast<I_DiagramListener*>(this));
    m_SerializablesIndexes.append(m_Diagrams.last()->getSerializableName());
    m_Diagrams.append(new SMDiagramScene);
    m_Diagrams.last()->registerDiagramView(m_DiagramView);
    m_Diagrams.last()->registerDiagramListener(static_cast<I_DiagramListener*>(this));
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

    m_hasChangesUnsaved = false;

    this->updateTitle();
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
            this->NextButtonPressed();
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
    else if( p_Event->matches(QKeySequence::New) )
    {
        this->newMenuClicked();
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
