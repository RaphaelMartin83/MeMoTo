#ifndef MOUSETOOL_H
#define MOUSETOOL_H

#include "I_Tool.h"

#include "I_ToolListener.h"

class MouseTool: public I_Tool
{
public:
    MouseTool();

    void use(const QJsonObject& p_JsonObject, I_ToolListener* p_Target) {}
    void use(QPoint p_Pos, I_ToolListener* p_Target, Qt::MouseButtons p_How);
    virtual void paste(const QJsonObject& p_JsonObject,
                       QPoint p_middlePos,
                       I_ToolListener* p_Target,
                       QUuid* p_rIDPasted = nullptr,
                       QUuid* p_rIDRerolled = nullptr) {}
    QString getItemName() const;
    bool createsContainers() const { return false; }
    bool createsConnectors() const { return false; }
};


#endif // MOUSETOOL_H
