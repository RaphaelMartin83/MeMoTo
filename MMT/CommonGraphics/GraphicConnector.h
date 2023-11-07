#ifndef GRAPHICCONNECTOR_H
#define GRAPHICCONNECTOR_H

#include <Engine/Connector.h>
#include <GraphicsItems/HandleGraphicsItem.h>
#include <GraphicsItems/TransitionGraphicsLabel.h>
#include <CommonGraphics/GraphicsBezierCurve.h>

class GraphicConnector:
        public Connector
{
public:
    GraphicConnector(
            const I_Connectable* p_ConnectFrom,
            const I_Connectable* p_ConnectTo,
            const QPoint& p_fromPoint,
            const QPoint& p_toPoint,
            I_ConnectableContainer* p_Container);
    GraphicConnector(
            const I_Connectable* p_ConnectFrom,
            const I_Connectable* p_ConnectTo,
            const QPoint& p_fromPoint,
            const QPoint& p_toPoint,
            I_ConnectableContainer* p_Container,
            const QList<QPoint>& p_ForcedPath);
    GraphicConnector(const QJsonObject& p_JsonObject,
                     I_ConnectableContainer* p_Container);

    bool isCurved() const;
    void setCurved(bool p_isCurved);

    // I_Selectable
    void select();
    void select(QPoint p_Pos);
    void unselect();
    void move(QPoint p_Pos);
    QPoint getPos() const;
    bool isItYou(QPoint p_Pos) const;
    bool isSelected() const;
    bool isFullySelected() const;
    QPoint getFocusPosition() const;
    virtual QRect getCollisionRectangle() const { return QRect(); }

    void route(QPoint p_From, QPoint p_To);
    virtual void reroute();

protected:
    void clear();
    void setupLines();
    void setupCurve();
    void setupHandles();
    void deleteHandles();
    virtual void refreshDisplay();
    void selectAllHandles();
    qreal getLastLineAngle() const;
    void setLinesPen(QPen p_Pen);
    unsigned short getThickness() const;

private:
    QList<QGraphicsLineItem*> m_Lines;
    GraphicsBezierCurve* m_Curve;
    QList<HandleGraphicsItem*> m_Handles;

    int m_SelectedHandleID;
    bool m_isFullySelected;
    bool m_isCurved;
};

#endif // GRAPHICCONNECTOR_H
