#ifndef MEMOTOAPPLICATION_H
#define MEMOTOAPPLICATION_H

#include <QApplication>
#include <QString>
#include <QIcon>
#include <QList>

#include <Engine/I_DiagramContainer.h>

#include <MainWindow.h>

class MeMoToApplication: public QApplication
{
public:
    MeMoToApplication(int& argc, char** argv);

    // QApplication override
    static int exec();

    static QString FileExtension();
    static QString SharedFileExtension();

    static bool isHeadless();
    static const QString& getDefaultDiagarm();
    static const QString& getPNGToCreate();
    static const QString& getServerIP();
    static const quint16& getServerPort();
    static const quint16& getCollaborativePort();

    static QIcon getLogo();

    static void startSharing();

    static void getApplicationData(QJsonObject& p_rData);
    static void setApplicationData(const QJsonObject& p_Data);

    static void diagramChanged();

    static bool hasChangesUnsaved();

    static void saveDiagrams();
    static void loadDiagrams();

    static void setFileName(const QString& p_FileName);
    static const QString& getFileName();

private:
    static QString sm_OutputString;
    static QString sm_DefaultDiagram;
    static QString sm_FocusOn;
    static QString sm_ServerIP;
    static quint16 sm_ServerPort;

    static quint16 sm_CollaborativePort;

    static MainWindow* sm_MW;

    static QList<I_DiagramContainer*> sm_Diagrams;

    static bool sm_hasChangesUnsaved;

    static bool sm_isHeadless;

    static QString sm_FileName;
};

#endif // MEMOTOAPPLICATION_H
