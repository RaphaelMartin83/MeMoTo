#ifndef CLASSDIAGRAMSCENE_H
#define CLASSDIAGRAMSCENE_H

#include <Engine/I_DiagramContainer.h>

#include <ToolBoxScenes/ClassToolBoxScene.h>

#include <GraphicsItems/ClassGraphicsItem.h>
#include <GraphicsItems/ExternalClassGraphicsItem.h>
#include <GraphicsItems/GeneralizationGraphicsItem.h>
#include <GraphicsItems/CompositionGraphicsItem.h>
#include <GraphicsItems/AggregationGraphicsItem.h>
#include <GraphicsItems/ImplementationGraphicsItem.h>

class ClassDiagramScene: public I_DiagramContainer
{
public:
    ClassDiagramScene();

    // I_DiagramContainer
    I_ToolBox* getToolBox();
    QString getDiagramString();
    QPoint getStartPosition() const;
    unsigned int getDiagramMaxWidth() const;
    unsigned int getDiagramMaxHeight() const;

    // I_Serializable
    QString getSerializableName() const;
private:
    ClassToolBoxScene* m_ToolBox;
};

#endif // CLASSDIAGRAMSCENE_H
