#ifndef I_TOOLSWITCHERLISTENER_H
#define I_TOOLSWITCHERLISTENER_H

#include "I_ToolListener.h"

class I_ToolSwitcherListener: public I_ToolListener
{
public:
    // Cleanup if needed
    virtual void toolChanged() = 0;
};

#endif // I_TOOLSWITCHERLISTENER_H
