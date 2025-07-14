#ifndef PRINTIMAGECONFIGURATION_H
#define PRINTIMAGECONFIGURATION_H

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QTreeView>
#include <QTreeWidgetItem>

#include <Engine/I_ConfigurationContent.h>
#include <Engine/I_PrintImageConfigurationListener.h>
#include <Engine/I_ConfigLineEditListener.h>

#include <CommonGraphics/ConfigLineEdit.h>

class PrintImageConfiguration:
        public I_ConfigurationContent,
        public I_ConfigLineEditListener
{
    Q_OBJECT

public:
    PrintImageConfiguration();

    QString getFilePathAndName();
    void setFileName(const QString& p_FileName);
    void setPath(const QString& p_Path);

    void registerConfigListener(I_PrintImageConfigurationListener* p_Listener);

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

    I_PrintImageConfigurationListener* m_Listener;
};

#endif // PRINTIMAGECONFIGURATION_H
