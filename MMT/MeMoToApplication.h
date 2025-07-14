#ifndef MEMOTOAPPLICATION_H
#define MEMOTOAPPLICATION_H

#include <QApplication>
#include <QString>
#include <QIcon>
#include <QList>
#include <QTimer>

#include <Engine/I_DiagramContainer.h>
#include <Engine/I_DiagramListener.h>

#include <I_MainWindowListener.h>
#include <MainWindow.h>

class MeMoToApplication: public QApplication,
                          public I_FileManager,
                          public I_MainWindowListener,
                          public I_DiagramListener
{
public:
    MeMoToApplication(int& argc, char** argv);

    // QApplication override
    int exec();

    static QString FileExtension();
    static QString SharedFileExtension();

    static bool isServer();
    static bool isReadOnly();
    static const QString& getDefaultDiagram();
    static unsigned short getDefaultDiagramID();
    static const QString& getPNGToCreate();
    static const QString& getServerIP();
    static const quint16& getServerPort();
    static const quint16& getCollaborativePort();

    static QIcon getLogo();

    static void getApplicationData(QJsonObject& p_rData);
    static void setApplicationData(const QJsonObject& p_Data);

    void diagramChanged();

    bool hasChangesUnsaved() const;

    void saveDiagrams();
    void loadDiagrams();

    void setFileName(const QString& p_FileName);
    const QString& getFileName() const;

    void displayModeFileUpdateTick();

    // I_MainWindowListener
    void closeAsked();

private:
    void createAndHandleArguments();
    void loadFileIfNeeded();
    static void focusItemIfAsked();
    static void autoConnectIfAsked();
    static void runAsServerIfAsked();
    void runAsGUI();
    static void runPNGDumpIfAsked();
    void createDiagrams();
    void initWindow();

    static QString sm_OutputString;
    static QString sm_DefaultDiagram;
    static QString sm_FocusOn;
    static quint32 sm_Period;
    static QString sm_ServerIP;
    static quint16 sm_ServerPort;

    static quint16 sm_CollaborativePort;

    static MainWindow* sm_MW;

    static QList<I_DiagramContainer*> sm_Diagrams;

    bool m_hasChangesUnsaved;

    static bool sm_isServer;
    static bool sm_isInDisplayMode;

    static bool sm_isReadOnly;

    static bool sm_isAutoconnect;

    QString m_FileName;

    static QTimer* sm_Timer;
};

#endif // MEMOTOAPPLICATION_H
