#ifndef SMDIAGRAMSCENE_H
#define SMDIAGRAMSCENE_H

#include "I_DiagramContainer.h"
#include "SMToolBoxScene.h"

class SMDiagramScene:
        public I_DiagramContainer
{
public:
    SMDiagramScene();

    // I_DiagramContainer
    I_ToolBox* getToolBox();
    QString getDiagramString();
    QPoint getStartPosition() const;
    unsigned int getDiagramMaxWidth() const;
    unsigned int getDiagramMaxHeight() const;

    // I_Serializable
    QString getSerializableName() const;
private:
    SMToolBoxScene* m_ToolBox;
};

#endif // SMDIAGRAMSCENE_H
