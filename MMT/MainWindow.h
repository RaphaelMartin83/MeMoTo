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

class MainWindow : public QMainWindow,
        public I_SaveFileConfigurationListener,
        public I_LoadFileConfigurationListener,
        public I_CloseWithoutSavingConfigurationListener
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    MainWindow(const char* argv, QWidget *parent = nullptr);
    ~MainWindow();

    void show();
    void updateTitle();

    I_DiagramContainer* getCurrentDiagram();
    void addDiagram(I_DiagramContainer* p_Diagram);

    void switchToContext(unsigned short p_ContextID, bool p_Force = false);

    // I_SaveFileConfigurationListener
    virtual void fileSelectedForSaving(QString p_File);
    virtual void fileSavingCanceled();

    // I_LoadFileConfigurationListener
    void fileSelectedForLoading(QString p_File);
    void fileLoadingCanceled();

    // I_CloseWithoutSavingConfigurationListener
    void saveBeforeClosing();
    void closeAndDropChanges();

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

    void keyPressEvent(QKeyEvent* p_Event);
    void savePressed(bool p_alwaysOpen=false);
    void loadPressed(bool p_alwaysOpen=false);

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
};
#endif // MAINWINDOW_H
