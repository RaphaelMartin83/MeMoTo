#ifndef I_TOOLLISTENER_H
#define I_TOOLLISTENER_H

#include "I_Undoable.h"

#include "GraphicConnector.h"

class I_ContainerGraphicsItem;

class I_ToolListener:
        public I_Undoable
{
public:
    // Elements creation
    virtual void addConnector(GraphicConnector* p_Connector) = 0;
    virtual void changed(I_GraphicsItem* p_WhoChanged) = 0;
};

#endif // I_TOOLLISTENER_H
