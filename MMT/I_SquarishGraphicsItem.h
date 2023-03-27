#ifndef I_SQUARISHGRAPHICSITEM_H
#define I_SQUARISHGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QPoint>

#include "I_ContainerGraphicsItem.h"

#include "HandleGraphicsItem.h"
#include "DoubleArrowGraphicsItem.h"

#include "ConnectionDefinitions.h"
#include "GridReferential.h"

static const unsigned short DEFAULT_SQUARE_WIDTH =
        static_cast<unsigned short>(GridReferential::getGridDistance(140U));
static const unsigned short DEFAULT_SQUARE_HEIGHT =
        static_cast<unsigned short>(GridReferential::getGridDistance(140U));

class I_SquarishGraphicsItem:
        public I_ContainerGraphicsItem
{
public:
    I_SquarishGraphicsItem(QPointF p_Pos, const QString& p_Name,
                            unsigned int p_ID,
                            unsigned short p_Width = DEFAULT_SQUARE_WIDTH,
                            unsigned short p_Height = DEFAULT_SQUARE_HEIGHT);
    I_SquarishGraphicsItem(const QJsonObject& p_JSon);
    ~I_SquarishGraphicsItem();

    // I'm a graphic class after all
    virtual void refreshDisplay();

    // Squarish business
    void setWidth(unsigned short p_Width);
    void setHeight(unsigned short p_Height);
    unsigned short getWidth();
    unsigned short getHeight();
    QPoint getPos() const;
    void setPos(const QPoint& p_Pos);
    void setName(const QString& p_Name);
    const QString& getName() const;
    void setColor(QColor p_Color);
    QColor getColor() const;
    QString getColorName() const;

    // Handling methods
    void setupHandles();
    void deleteHandles();
    void selectAllHandles();
    void selectHandlesOnSide(eConnectDirection p_Direction);
    void resizeTo(const QPoint& p_Pos);

    void translateTo(const QPoint& p_Pos);
    void resizeToContain(QRect p_Rect);

    // I_ContainerGraphicsItem
    virtual void resizeToFitContent();

    // I_Selectable interface
    void select();
    void select(QPoint p_Pos);
    void unselect();
    void move(QPoint p_Pos);
    bool isItYou(QPoint p_Pos) const;
    bool isFullySelected() const;
    QPoint getFocusPosition() const;

    // I_Connectable
    // NOTE: getConnectableType is not implemented because this object shall not be used
    const QList<HandleGraphicsItem*>& getConnectableHandles() const;
    QRect getCollisionRectangle() const;
    virtual QString getConnectableName() const;

    // I_Serializable
    QJsonObject toJson();
    void fromJson(QJsonObject p_Json);

    // I_GraphicsItem overrides
    virtual void setPlan(unsigned short p_Plan);

private:
    // Square
    QString m_Name;
    QPoint m_Pos;
    unsigned short m_Width;
    unsigned short m_Height;

    // Basic graphics
    QGraphicsRectItem* m_Rect;
    QGraphicsTextItem* m_NameGraphicsItem;
    QGraphicsLineItem* m_LineUnderTitle;
    QList<HandleGraphicsItem*> m_Handles;

    // Selection Items
    DoubleArrowGraphicsItem* m_LeftDoubleArrow;
    DoubleArrowGraphicsItem* m_RightDoubleArrow;
    DoubleArrowGraphicsItem* m_TopDoubleArrow;
    DoubleArrowGraphicsItem* m_BotDoubleArrow;

    // Selection mecanisms
    bool m_isFullySelected = false;
    eConnectDirection m_SelectedHandlesDirection;
    HandleGraphicsItem* m_SelectedHandleForConnectionFrom;
    HandleGraphicsItem* m_SelectedHandleForConnectionTo;
};

#endif // I_SQUARISHGRAPHICSITEM_H
