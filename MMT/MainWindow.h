#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGridLayout>
#include <QPushButton>
#include <QMenuBar>

#include <Engine/I_DiagramContainer.h>
#include <Engine/DiagramGraphicsView.h>
#include <Engine/I_SaveFileConfigurationListener.h>
#include <Engine/I_LoadFileConfigurationListener.h>
#include <Engine/I_CloseWithoutSavingConfigurationListener.h>
#include <Engine/I_DiagramListener.h>

#include <Sharing/I_DataManager.h>

class MainWindow : public QMainWindow,
        public I_SaveFileConfigurationListener,
        public I_LoadFileConfigurationListener,
        public I_DataManager,
        public I_DiagramListener,
        public I_CloseWithoutSavingConfigurationListener
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    MainWindow(const char* argv, QWidget *parent = nullptr);
    ~MainWindow();

    I_DiagramContainer* getCurrentDiagram();

    // I_SaveFileConfigurationListener
    virtual void fileSelectedForSaving(QString p_File);
    virtual void fileSavingCanceled();

    // I_LoadFileConfigurationListener
    void fileSelectedForLoading(QString p_File);
    void fileLoadingCanceled();

    // I_DataManager
    void getApplicationData(QJsonObject& p_rData) const;
    void setApplicationData(const QJsonObject& p_Data);

    // I_DiagramListener
    void diagramChanged();

    // I_CloseWithoutSavingConfigurationListener
    void saveBeforeClosing();
    void closeAndDropChanges();

    void startSharing();

private slots:
    void NextButtonPressed();
    void PrevButtonPressed();

    void newMenuClicked();
    void openMenuClicked();
    void openNewMenuClicked();
    void saveMenuClicked();
    void saveAsMenuClicked();

    void undoMenuClicked();
    void redoMenuClicked();
    void copyMenuClicked();
    void pasteMenuClicked();
    void shareMenuClicked();
    void findMenuClicked();
    void printMenuClicked();

public slots:
    void closeEvent(QCloseEvent* p_event);

private:
    void initGUI();
    void initDiagrams();

    void updateTitle();

    void saveDiagrams();
    void loadDiagrams();
    void keyPressEvent(QKeyEvent* p_Event);
    void savePressed(bool p_alwaysOpen=false);
    void loadPressed(bool p_alwaysOpen=false);
    void switchToContext(unsigned short p_ContextID, bool p_Force = false);

    QMenuBar* m_MenuBar;
    QMenu* m_FileMenu;
    QMenu* m_EditMenu;

    QGridLayout* m_mainLayout;
    QWidget* m_centralWidget;

    QPushButton* m_PrevDiagButton;
    QPushButton* m_NextDiagButton;

    QGraphicsView* m_ToolBoxView;
    DiagramGraphicsView* m_DiagramView;

    QList<I_DiagramContainer*> m_Diagrams;
    unsigned short m_CurrentDiagramID;

    QList<QString> m_SerializablesIndexes;

    QString m_FileName;

    bool m_hasChangesUnsaved;
};
#endif // MAINWINDOW_H
