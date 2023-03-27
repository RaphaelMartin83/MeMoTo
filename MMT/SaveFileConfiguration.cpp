#include <QScroller>
#include <QStandardPaths>

#include "SaveFileConfiguration.h"

#include "ConfigLineEdit.h"
#include "MeMoToApplication.h"

SaveFileConfiguration::SaveFileConfiguration():
    m_Listener(nullptr)
{
    // Init widgets
    m_ContextLabel = new QLabel("Enter file saving path:");
    m_ContextLabel->setFixedHeight(30);

    m_FileNameLabel = new QLabel("Name: (without extension)");
    m_FileNameEdit = new ConfigLineEdit(this);
    m_FilePathLabel = new QLabel("Path:");
    m_FilePathEdit = new ConfigLineEdit(this);
    m_FilePathEdit->setText(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + QDir::separator());

    // File browsing mecanism
    m_FileSystemModel = new QFileSystemModel();
    m_FileIconProvider = new QFileIconProvider();
    m_FileSystemModel->setIconProvider(m_FileIconProvider);
    m_FileSystemModel->setRootPath(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    m_FileSystemModel->setReadOnly(true);
    m_FileSystemModel->setOption(QFileSystemModel::DontUseCustomDirectoryIcons);
    m_FileSystemModel->setOption(QFileSystemModel::DontWatchForChanges);
    m_FileSystemModel->setFilter(QDir::Filter::Dirs);
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
    m_Layout->addWidget(m_FilePathLabel, l_row++, 0, 1, -1, Qt::Alignment());
    m_Layout->addWidget(m_FilePathEdit, l_row++, 0, 1, -1, Qt::Alignment());
    m_Layout->addWidget(m_TreeView, l_row++, 0, 1, -1, Qt::Alignment());
    m_Layout->addWidget(m_OKButton, l_row, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_CancelButton, l_row++, 1, 1, 1, Qt::Alignment());

    this->setVisible(false);

    this->setLayout(m_Layout);

    // Setup events
    connect(m_OKButton, &QPushButton::clicked, this, &SaveFileConfiguration::OKPressed);
    connect(m_CancelButton, &QPushButton::clicked, this, &SaveFileConfiguration::CancelPressed);
    connect(m_TreeView, &QTreeView::clicked,
            this, &SaveFileConfiguration::selectedPathChanged);
    connect(m_TreeView, &QTreeView::pressed,
            this, &SaveFileConfiguration::selectedPathChanged);
    connect(m_TreeView, &QTreeView::doubleClicked,
            this, &SaveFileConfiguration::OKPressed);
    connect(m_FilePathEdit, &QLineEdit::textEdited,
            this, &SaveFileConfiguration::writtenPathChanged);
}

QString SaveFileConfiguration::getFilePathAndName()
{
    return m_FilePathEdit->text() + QDir::separator() + m_FileNameEdit->text() + MeMoToApplication::FileExtension();
}
void SaveFileConfiguration::setFileName(const QString& p_FileName)
{
    m_FileNameEdit->setText(p_FileName);
}
void SaveFileConfiguration::setPath(const QString& p_Path)
{
    m_FilePathEdit->setText(p_Path);
}

void SaveFileConfiguration::registerConfigListener(I_SaveFileConfigurationListener* p_Listener)
{
    m_Listener = p_Listener;
}

void SaveFileConfiguration::selectedPathChanged(const QModelIndex& p_Index)
{
    m_FilePathEdit->setText(m_FileSystemModel->filePath(p_Index));
}

void SaveFileConfiguration::writtenPathChanged()
{
    QDir l_Dir(m_FilePathEdit->text());

    if( true == l_Dir.exists() )
    {
        m_FilePathEdit->setStyleSheet("color: black");
        m_TreeView->setCurrentIndex(m_FileSystemModel->index(m_FilePathEdit->text()));
        m_TreeView->expand(m_TreeView->currentIndex());
    }
    else
    {
        m_FilePathEdit->setStyleSheet("color: red");
    }
}

// I_ConfigurationContent
void SaveFileConfiguration::setFocusOnFirstZone()
{
    m_FileNameEdit->setFocus();
}

// I_ConfigLineEditListener
void SaveFileConfiguration::enterPressed()
{
    this->OKPressed();
}
void SaveFileConfiguration::escapePressed()
{
    this->CancelPressed();
}

void SaveFileConfiguration::OKPressed()
{
    if( (nullptr != m_Listener) &&
            (QDir(m_FilePathEdit->text()).exists()) )
    {
        m_Listener->fileSelectedForSaving(this->getFilePathAndName());
    }
}
void SaveFileConfiguration::CancelPressed()
{
    if( nullptr != m_Listener )
    {
        m_Listener->fileSavingCanceled();
    }
}
