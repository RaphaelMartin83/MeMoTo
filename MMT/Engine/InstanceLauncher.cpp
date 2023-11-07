#include <QProcess>
#include <QApplication>

#include "InstanceLauncher.h"

InstanceLauncher::InstanceLauncher()
{

}

InstanceLauncher& InstanceLauncher::getInstance()
{
    static InstanceLauncher ls_InstanceLauncher;

    return ls_InstanceLauncher;
}

void InstanceLauncher::execute(QStringList p_Arguments)
{
    QProcess l_Proc;
    l_Proc.setProgram(QApplication::applicationFilePath());
    l_Proc.setArguments(p_Arguments);
    l_Proc.startDetached();
}
