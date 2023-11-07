#ifndef SMTOOLBOXSCENE_H
#define SMTOOLBOXSCENE_H

#include <QGraphicsScene>

#include <Engine/I_ToolBox.h>

// A scene dispaying the tools to design state machines
class SMToolBoxScene: public I_ToolBox
{
public:
    SMToolBoxScene();

    // The Graphics selection
    void mousePressEvent(QGraphicsSceneMouseEvent* p_Event);

    // I_ToolBox
    unsigned short getToolsQuantity() const;
    const QList<I_Tool*>& getToolsList() const;

    // Overrides
    void selectByID(unsigned short p_ID);

private:
    QList<I_Tool*> m_ToolsList;

    /*
     * 6 items total for 600 pixels of widget minimum height
     * Span for each tool is 100 pixels
     * Thus, the implementation in mousepressed event callback
     */
    QGraphicsRectItem* m_StateItem;
    QGraphicsPolygonItem* m_choiceItem;
    QGraphicsLineItem* m_transitionItemLine;
    QGraphicsLineItem* m_transitionItemArrowLeft;
    QGraphicsLineItem* m_transitionItemArrowRight;
    QGraphicsEllipseItem* m_InitItem;
    QGraphicsEllipseItem* m_EntryPointItem;

    QGraphicsRectItem* m_SelectionRect;
};

#endif // SMTOOLBOXSCENE_H
