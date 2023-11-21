#include <QFile>
#include <QProcessEnvironment>
#include <QDir>
#include <QStringList>

#include <MeMoToApplication.h>

#include <Engine/InstanceLauncher.h>

#include <ConfigurationContexts/ExternalClassConfiguration.h>

ExternalClassConfiguration::ExternalClassConfiguration()
{
    // Init widgets
    m_NameLabel = new QLabel("Name:");
    m_NameLabel->setFixedHeight(30);
    m_NameEdit = new ConfigLineEdit(static_cast<I_ConfigLineEditListener*>(this));

    m_RootLabel = new QLabel("Root:");
    m_RootLabel->setFixedHeight(30);
    m_RootEdit = new ConfigLineEdit(static_cast<I_ConfigLineEditListener*>(this));

    m_PathLabel = new QLabel("Path:");
    m_PathLabel->setFixedHeight(30);
    m_PathEdit = new ConfigLineEdit(static_cast<I_ConfigLineEditListener*>(this));

    m_ColorSelection = new QComboBox();
    m_ColorSelection->addItems(QColor::colorNames());

    m_HideContent = new QLabel("Hide content");
    m_HideContentCheckBox = new QCheckBox();

    m_OpenButton = new QPushButton("Open");

    m_OKButton = new QPushButton("OK");
    m_CancelButton = new QPushButton("Cancel");

    m_EmptySpace = new QWidget();

    m_Layout = new QGridLayout();

    // Setup layout
    unsigned short l_row=0;
    m_Layout->addWidget(m_NameLabel, l_row++, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_NameEdit, l_row++, 0, 1, -1, Qt::Alignment());

    m_Layout->addWidget(m_RootLabel, l_row++, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_RootEdit, l_row++, 0, 1, -1, Qt::Alignment());

    m_Layout->addWidget(m_PathLabel, l_row++, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_PathEdit, l_row++, 0, 1, -1, Qt::Alignment());

    m_Layout->addWidget(m_ColorSelection, l_row++, 0, 1, -1, Qt::Alignment());

    m_Layout->addWidget(m_HideContent, l_row, 0, 1, -1, Qt::AlignLeft);
    m_Layout->addWidget(m_HideContentCheckBox, l_row++, 1, 1, -1, Qt::Alignment());

    m_Layout->addWidget(m_OpenButton, l_row++, 0, 1, -1, Qt::Alignment());

    m_Layout->addWidget(m_CancelButton, l_row, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_OKButton, l_row++, 1, 1, 1, Qt::Alignment());

    m_Layout->addWidget(m_EmptySpace, l_row++, 0, -1, -1, Qt::Alignment());

    this->setVisible(false);

    this->setLayout(m_Layout);

    // Setup events
    connect(m_OKButton, &QPushButton::clicked, this, &ExternalClassConfiguration::OKPressed);
    connect(m_CancelButton, &QPushButton::clicked, this, &ExternalClassConfiguration::CancelPressed);
    connect(m_OpenButton, &QPushButton::clicked, this, &ExternalClassConfiguration::openPressed);
}

QString ExternalClassConfiguration::getName() const
{
    return m_NameEdit->text();
}

void ExternalClassConfiguration::setName(const QString& p_Name)
{
    m_NameEdit->setText(p_Name);
}

QString ExternalClassConfiguration::getRoot() const
{
    return m_RootEdit->text();
}

void ExternalClassConfiguration::setRoot(const QString& p_Root)
{
    m_RootEdit->setText(p_Root);
}

QString ExternalClassConfiguration::getPath() const
{
    return m_PathEdit->text();
}

void ExternalClassConfiguration::setPath(const QString & p_Path)
{
    m_PathEdit->setText(p_Path);
}

QColor ExternalClassConfiguration::getColor() const
{
    QString l_ColorString = this->m_ColorSelection->currentText();
    QColor l_Ret(l_ColorString);

    return l_Ret;
}
void ExternalClassConfiguration::setColor(QString p_Color)
{
    m_ColorSelection->setCurrentText(p_Color);
}

bool ExternalClassConfiguration::isContentToHide() const
{
    return m_HideContentCheckBox->isChecked();
}
void ExternalClassConfiguration::setContentToHide(bool p_isContentToHide)
{
    m_HideContentCheckBox->setChecked(p_isContentToHide);
}

QString ExternalClassConfiguration::getResolvedPath(QString p_Root, QString p_Path, QString p_Name)
{
    QString l_resolvedFullPath;

    // If root is empty, nothing to do with root
    if( "" != p_Root )
    {
        QProcessEnvironment l_env(QProcessEnvironment::systemEnvironment());
        l_resolvedFullPath = l_env.value(p_Root);

        // If it's not an environment variable
        if( "" == l_resolvedFullPath )
        {
            l_resolvedFullPath = p_Root;
        }
        l_resolvedFullPath.append(QDir::separator());
    }

    l_resolvedFullPath.append(p_Path);
    if( !l_resolvedFullPath.contains(MeMoToApplication::FileExtension()) )
    {
        l_resolvedFullPath.append(p_Name);
        l_resolvedFullPath.append(MeMoToApplication::FileExtension());
    }

    return l_resolvedFullPath;
}

void ExternalClassConfiguration::setFocusOnFirstZone()
{
    m_NameEdit->setFocus();
    m_NameEdit->selectAll();
}

void ExternalClassConfiguration::enterPressed()
{
    this->OKPressed();
}

void ExternalClassConfiguration::escapePressed()
{
    this->CancelPressed();
}

// Slots
void ExternalClassConfiguration::OKPressed()
{
    I_Configurable* l_Listener = this->getListener();
    if( nullptr != l_Listener)
    {
        l_Listener->applyConfiguration();
    }
}

void ExternalClassConfiguration::CancelPressed()
{
    I_Configurable* l_Listener = this->getListener();
    if( nullptr != l_Listener)
    {
        l_Listener->closeConfiguration();
    }
}

void ExternalClassConfiguration::openPressed()
{
    QStringList l_argsList;
    l_argsList.append(this->getResolvedPath(this->getRoot(), this->getPath(), this->getName()));
    l_argsList.append("--focus-on");
    l_argsList.append(this->getName());
    l_argsList.append("--default-diagram");
    l_argsList.append("ClassDiagram");
    InstanceLauncher::getInstance().execute(QStringList(l_argsList));
}
