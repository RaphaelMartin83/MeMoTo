#ifndef CLASSTOOLBOXSCENE_H
#define CLASSTOOLBOXSCENE_H

#include "I_ToolBox.h"

class ClassToolBoxScene: public I_ToolBox
{
public:
    ClassToolBoxScene();

    // The Graphics selection
    void mousePressEvent(QGraphicsSceneMouseEvent* p_Event);

    // I_ToolBox
    unsigned short getToolsQuantity() const;
    const QList<I_Tool*>& getToolsList() const;

    // Overrides
    void selectByID(unsigned short p_ID);

private:
    QList<I_Tool*> m_ToolsList;

    QGraphicsRectItem* m_SelectionRect;
};

#endif // CLASSTOOLBOXSCENE_H
