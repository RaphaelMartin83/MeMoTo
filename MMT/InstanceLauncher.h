#ifndef INSTANCELAUNCHER_H
#define INSTANCELAUNCHER_H

#include <QString>
#include <QStringList>

class InstanceLauncher
{
public:
    static InstanceLauncher& getInstance();

    void execute(QStringList p_Arguments);

private:
    InstanceLauncher();

    QString m_Executable;
};

#endif // INSTANCELAUNCHER_H
