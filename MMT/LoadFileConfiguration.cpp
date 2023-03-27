#include <QScroller>
#include <QStandardPaths>
#include <QFileInfo>

#include "LoadFileConfiguration.h"

#include "ConfigLineEdit.h"
#include "MeMoToApplication.h"

LoadFileConfiguration::LoadFileConfiguration():
    m_Listener(nullptr)
{
    // Init widgets
    m_ContextLabel = new QLabel("Enter file loading path:");
    m_ContextLabel->setFixedHeight(30);

    m_FileNameLabel = new QLabel("Path and name: (*" + MeMoToApplication::FileExtension() + ")");
    m_FileNameEdit = new ConfigLineEdit(this);
    m_FileNameEdit->setText(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));

    // File browsing mecanism
    m_FileSystemModel = new QFileSystemModel();
    m_FileIconProvider = new QFileIconProvider();
    m_FileSystemModel->setIconProvider(m_FileIconProvider);
    m_FileSystemModel->setRootPath(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    m_FileSystemModel->setReadOnly(true);
    m_FileSystemModel->setOption(QFileSystemModel::DontUseCustomDirectoryIcons);
    m_FileSystemModel->setOption(QFileSystemModel::DontWatchForChanges);
    QStringList l_FilterTypes(QString("*") + MeMoToApplication::FileExtension());
    m_FileSystemModel->setNameFilterDisables(false);
    m_FileSystemModel->setNameFilters(l_FilterTypes);
    m_TreeView = new QTreeView();
    m_TreeView->setModel(m_FileSystemModel);
    m_TreeView->setIndentation(20);
    m_TreeView->setSortingEnabled(true);
    m_TreeView->hideColumn(1);
    m_TreeView->hideColumn(2);
    m_TreeView->hideColumn(3);
    m_TreeView->setColumnWidth(0, 2000);
    m_TreeView->setRootIndex(m_FileSystemModel->index(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)));
    m_TreeView->setRootIsDecorated(true);
    QScroller::grabGesture(m_TreeView, QScroller::TouchGesture);
    QScroller::grabGesture(m_TreeView, QScroller::MiddleMouseButtonGesture);

    m_OKButton = new QPushButton("OK");
    m_CancelButton = new QPushButton("Cancel");

    // Layout
    m_Layout = new QGridLayout();
    unsigned short l_row = 0U;
    m_Layout->addWidget(m_ContextLabel, l_row++, 0, 1, -1, Qt::Alignment());
    m_Layout->addWidget(m_FileNameLabel, l_row++, 0, 1, -1, Qt::Alignment());
    m_Layout->addWidget(m_FileNameEdit, l_row++, 0, 1, -1, Qt::Alignment());
    m_Layout->addWidget(m_TreeView, l_row++, 0, 1, -1, Qt::Alignment());
    m_Layout->addWidget(m_OKButton, l_row, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_CancelButton, l_row++, 1, 1, 1, Qt::Alignment());

    this->setVisible(false);

    this->setLayout(m_Layout);

    // Setup events
    connect(m_OKButton, &QPushButton::clicked, this, &LoadFileConfiguration::OKPressed);
    connect(m_CancelButton, &QPushButton::clicked, this, &LoadFileConfiguration::CancelPressed);
    connect(m_TreeView, &QTreeView::clicked,
            this, &LoadFileConfiguration::selectedPathChanged);
    connect(m_TreeView, &QTreeView::pressed,
            this, &LoadFileConfiguration::selectedPathChanged);
    connect(m_TreeView, &QTreeView::doubleClicked,
            this, &LoadFileConfiguration::OKPressed);
    connect(m_FileNameEdit, &QLineEdit::textEdited,
            this, &LoadFileConfiguration::writtenPathChanged);
}

QString LoadFileConfiguration::getFilePathAndName()
{
    return m_FileNameEdit->text();
}
void LoadFileConfiguration::setFilePathAndName(const QString& p_FileName)
{
    m_FileNameEdit->setText(p_FileName);
}

void LoadFileConfiguration::registerConfigListener(I_LoadFileConfigurationListener* p_Listener)
{
    m_Listener = p_Listener;
}

void LoadFileConfiguration::selectedPathChanged(const QModelIndex& p_Index)
{
    m_FileNameEdit->setText(m_FileSystemModel->filePath(p_Index));

    if( this->currentFileIsOK() )
    {
        m_FileNameEdit->setStyleSheet("color: black");
    }
    else
    {
        m_FileNameEdit->setStyleSheet("color: red");
    }
}

void LoadFileConfiguration::writtenPathChanged()
{
    QFile l_File(m_FileNameEdit->text());
    if( l_File.exists() )
    {
        m_TreeView->setCurrentIndex(m_FileSystemModel->index(m_FileNameEdit->text()));
        m_TreeView->expand(m_TreeView->currentIndex());
    }
    if( this->currentFileIsOK() )
    {
        m_FileNameEdit->setStyleSheet("color: black");
    }
    else
    {
        m_FileNameEdit->setStyleSheet("color: red");
    }
}

bool LoadFileConfiguration::currentFileIsOK() const
{
    QFile l_File(m_FileNameEdit->text());
    QFileInfo l_FileInfo(m_FileNameEdit->text());

    QString l_suffix = l_FileInfo.suffix();

    return  (l_File.exists()) &&
            (l_FileInfo.isFile()) &&
            (l_FileInfo.isReadable()) &&
            (MeMoToApplication::FileExtension() == ("." + l_FileInfo.suffix()));
}

// I_ConfigurationContent
void LoadFileConfiguration::setFocusOnFirstZone()
{
    m_FileNameEdit->setFocus();
}

// I_ConfigLineEditListener
void LoadFileConfiguration::enterPressed()
{
    this->OKPressed();
}
void LoadFileConfiguration::escapePressed()
{
    this->CancelPressed();
}

void LoadFileConfiguration::OKPressed()
{
    if( (nullptr != m_Listener) &&
            this->currentFileIsOK() )
    {
        m_Listener->fileSelectedForLoading(this->getFilePathAndName());
    }
}
void LoadFileConfiguration::CancelPressed()
{
    if( nullptr != m_Listener )
    {
        m_Listener->fileLoadingCanceled();
    }
}
