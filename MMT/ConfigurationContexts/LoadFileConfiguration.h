#ifndef LOADFILECONFIGURATION_H
#define LOADFILECONFIGURATION_H

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QTreeView>
#include <QTreeWidgetItem>

#include <Engine/I_ConfigurationContent.h>
#include <Engine/I_LoadFileConfigurationListener.h>
#include <Engine/I_ConfigLineEditListener.h>

#include <CommonGraphics/ConfigLineEdit.h>

class LoadFileConfiguration:
        public I_ConfigurationContent,
        public I_ConfigLineEditListener
{
    Q_OBJECT

public:
    LoadFileConfiguration();

    QString getFilePathAndName();
    void setFilePathAndName(const QString& p_File);

    void registerConfigListener(I_LoadFileConfigurationListener* p_Listener);

    // I_ConfigurationContent
    void setFocusOnFirstZone();

    // I_ConfigLineEditListener
    void enterPressed();
    void escapePressed();

public slots:
    void OKPressed();
    void CancelPressed();
    void selectedPathChanged(const QModelIndex& p_Index);
    void writtenPathChanged();

private:
    bool currentFileIsOK() const;

    QGridLayout* m_Layout;

    QLabel* m_ContextLabel;
    QLabel* m_FileNameLabel;
    ConfigLineEdit* m_FileNameEdit;

    QPushButton* m_OKButton;
    QPushButton* m_CancelButton;

    // File browsing mecanism
    QFileSystemModel* m_FileSystemModel;
    QFileIconProvider* m_FileIconProvider;
    QTreeView* m_TreeView;

    I_LoadFileConfigurationListener* m_Listener;
};

#endif // LOADFILECONFIGURATION_H
