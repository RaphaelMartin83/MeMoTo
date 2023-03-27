#ifndef SAVEFILECONFIGURATION_H
#define SAVEFILECONFIGURATION_H

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QTreeView>
#include <QTreeWidgetItem>

#include "ConfigLineEdit.h"

#include "I_ConfigurationContent.h"
#include "I_SaveFileConfigurationListener.h"
#include "I_ConfigLineEditListener.h"

class SaveFileConfiguration:
        public I_ConfigurationContent,
        public I_ConfigLineEditListener
{
    Q_OBJECT

public:
    SaveFileConfiguration();

    QString getFilePathAndName();
    void setFileName(const QString& p_FileName);
    void setPath(const QString& p_Path);

    void registerConfigListener(I_SaveFileConfigurationListener* p_Listener);

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
    QGridLayout* m_Layout;

    QLabel* m_ContextLabel;
    QLabel* m_FilePathLabel;
    ConfigLineEdit* m_FilePathEdit;
    QLabel* m_FileNameLabel;
    ConfigLineEdit* m_FileNameEdit;

    QPushButton* m_OKButton;
    QPushButton* m_CancelButton;

    // File browsing mecanism
    QFileSystemModel* m_FileSystemModel;
    QFileIconProvider* m_FileIconProvider;
    QTreeView* m_TreeView;

    I_SaveFileConfigurationListener* m_Listener;
};

#endif // SAVEFILECONFIGURATION_H
