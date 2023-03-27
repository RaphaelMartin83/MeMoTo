#ifndef CLASSDIAGRAMSCENE_H
#define CLASSDIAGRAMSCENE_H

#include "I_DiagramContainer.h"

#include "ClassGraphicsItem.h"
#include "ClassToolBoxScene.h"
#include "ExternalClassGraphicsItem.h"
#include "GeneralizationGraphicsItem.h"
#include "CompositionGraphicsItem.h"
#include "AggregationGraphicsItem.h"
#include "ImplementationGraphicsItem.h"

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
