#ifndef HANDLEGRAPHICSITEM_H
#define HANDLEGRAPHICSITEM_H

#include <QGraphicsItem>

#include "ConnectionDefinitions.h"
#include "I_Selectable.h"

class HandleGraphicsItem: public I_Selectable
{
public:
    HandleGraphicsItem(const QPoint& p_Position,
                       const eConnectDirection& p_HandlePosition,
                       bool p_VisibleUnselected = false);
    ~HandleGraphicsItem();

    void setHandlePosition(const eConnectDirection& p_HandlePos);
    const eConnectDirection& getHandlePosition() const;
    QPoint getHandleCoordinates() const;
    void selectForConnection();

    // I_Selectable
    void select();
    void select(QPoint p_Pos);
    void unselect();
    void move(QPoint p_Pos);
    bool isItYou(QPoint p_Pos) const;
    virtual bool isFullySelected() const;
    QPoint getFocusPosition() const;
    virtual void refreshDisplay();
    QString getSelectableType() const;
    QStringList getSearchFields() const;
    QString getDataFromField(const QString& p_FieldName) const;
    virtual QRect getCollisionRectangle() const { return QRect(); }

    // I_Serializable because selectable but useless here
    virtual QJsonObject toJson() {return QJsonObject();}
    virtual void fromJson(QJsonObject p_Json) {}
    virtual QString getSerializableName() const {return "";}

private:
    eConnectDirection m_HandlePosition;
    unsigned short m_diameter;

    QGraphicsEllipseItem* m_Circle;

    bool m_isSelected;
    bool m_isVisibleUnselected;
};

#endif // HANDLEGRAPHICSITEM_H
