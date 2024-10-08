#include <QGraphicsView>
#include <QKeyEvent>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QStandardPaths>

#include <MeMoToApplication.h>

#include <Engine/DiagramGraphicsView.h>
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
{
    this->initGUI();
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
void MainWindow::addDiagram(I_DiagramContainer* p_Diagram)
{
    m_Diagrams.append(p_Diagram);
    m_Diagrams.last()->registerDiagramView(m_DiagramView);
}

// I_SaveFileConfigurationListener
void MainWindow::fileSelectedForSaving(QString p_File)
{
    MeMoToApplication::setFileName(p_File);

    ConfigWidget::close();

    MeMoToApplication::saveDiagrams();
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
        MeMoToApplication::setFileName(p_File);

        ConfigWidget::close();

        MeMoToApplication::loadDiagrams();
        this->updateTitle();
    }
}
void MainWindow::fileLoadingCanceled()
{
    ConfigWidget::close();
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
    MeMoToApplication::startSharing();
}
void MainWindow::findMenuClicked()
{
    m_Diagrams[m_CurrentDiagramID]->find();
}
void MainWindow::printMenuClicked()
{
    m_Diagrams[m_CurrentDiagramID]->printPressed(MeMoToApplication::getFileName());
}

void MainWindow::closeEvent(QCloseEvent* p_event)
{
    if( MeMoToApplication::hasChangesUnsaved() )
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
    Q_ASSERT(nullptr != m_mainLayout);

    m_PrevDiagButton = new QPushButton("<-");
    Q_ASSERT(nullptr != m_PrevDiagButton);
    m_PrevDiagButton->setFixedWidth(47);
    m_NextDiagButton = new QPushButton("->");
    Q_ASSERT(nullptr != m_NextDiagButton);
    m_NextDiagButton->setFixedWidth(47);

    m_ToolBoxView = new QGraphicsView();
    Q_ASSERT(nullptr != m_ToolBoxView);
    m_ToolBoxView->setFixedWidth(100);
    m_DiagramView = new DiagramGraphicsView();
    Q_ASSERT(nullptr != m_DiagramView);
    m_DiagramView->setMinimumWidth(700);
    m_DiagramView->setMinimumHeight(700);
    m_DiagramView->setMouseTracking(true);

    m_mainLayout->addWidget(m_ToolBoxView, 0, 0, 1, 2, Qt::Alignment());
    m_mainLayout->addWidget(m_PrevDiagButton, 1, 0, 1, 1, Qt::Alignment());
    m_mainLayout->addWidget(m_NextDiagButton, 1, 1, 1, 1, Qt::Alignment());

    m_splitter = new QSplitter();
    Q_ASSERT(nullptr != m_splitter);
    m_splitter->addWidget(m_DiagramView);
    m_splitter->addWidget(&ConfigWidget::getInstance());
    m_mainLayout->addWidget(m_splitter, 0, 2, -1, 1, Qt::Alignment());

    m_centralWidget = new QWidget();
    Q_ASSERT(nullptr != m_centralWidget);
    m_centralWidget->setLayout(m_mainLayout);

    this->setCentralWidget(m_centralWidget);
    this->setWindowTitle(s_ProgramName);
    this->move(300, 150);

    m_MenuBar = this->menuBar();
    m_FileMenu = new QMenu("File");
    Q_ASSERT(nullptr != m_FileMenu);
    QList<QAction*> l_ActionsListFile;
    l_ActionsListFile.append(new QAction("New"));
    l_ActionsListFile.append(new QAction("Open"));
    l_ActionsListFile.append(new QAction("Open new"));
    l_ActionsListFile.append(new QAction("Save"));
    l_ActionsListFile.append(new QAction("Save as"));
    m_FileMenu->addActions(l_ActionsListFile);
    m_EditMenu = new QMenu("Edit");
    Q_ASSERT(nullptr != m_EditMenu);
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
    QString l_FileName = MeMoToApplication::getFileName();
    if( "" != l_FileName )
    {
        l_newTitle += " on ";
        if( l_FileName.size() > MAX_DISPLAYED_NAME_SIZE )
        {
            l_newTitle += ".." + l_FileName.last(MAX_DISPLAYED_NAME_SIZE);
        }
        else
        {
            l_newTitle += l_FileName;
        }
    }
    l_newTitle += " (" + m_Diagrams[m_CurrentDiagramID]->getDiagramString() + ")";

    if( MeMoToApplication::hasChangesUnsaved() )
    {
        l_newTitle += "*";
    }

    this->setWindowTitle(l_newTitle);
}
void MainWindow::show()
{
    QMainWindow::show();
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
        this->printMenuClicked();
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
        MeMoToApplication::startSharing();
    }
}

void MainWindow::savePressed(bool p_alwaysOpen)
{
    if( ("" != MeMoToApplication::getFileName()) && (false == p_alwaysOpen) )
    {
        MeMoToApplication::saveDiagrams();
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
    if( ("" != MeMoToApplication::getFileName()) && (false == p_alwaysOpen) )
    {
        MeMoToApplication::loadDiagrams();
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
