#ifndef I_TOOL_H
#define I_TOOL_H

#include "I_ToolSwitcherListener.h"

class I_Tool
{
public:
    virtual void use(const QJsonObject& p_JsonObject, I_ToolListener* p_Target) = 0;
    virtual void use(QPoint p_Pos, I_ToolListener* p_Target, Qt::MouseButtons p_How) = 0;
    virtual QString getItemName() const = 0;
    virtual bool createsContainers() const = 0;
    virtual bool createsConnectors() const = 0;
};

#endif // I_TOOL_H
