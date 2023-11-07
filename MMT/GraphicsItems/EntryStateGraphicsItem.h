#ifndef ENTRYSTATEGRAPHICSITEM_H
#define ENTRYSTATEGRAPHICSITEM_H

#include <QGraphicsItemGroup>

#include <Engine/I_Selectable.h>
#include <Engine/I_Connectable.h>

#include <GraphicsItems/HandleGraphicsItem.h>

class EntryStateGraphicsItem:
        public I_Connectable
{
public:
    EntryStateGraphicsItem(const QPoint& p_Pos);
    EntryStateGraphicsItem(const QJsonObject& p_JsonObject);
    ~EntryStateGraphicsItem();

    void setDiameter(unsigned short p_Diameter);

    void refreshDisplay();
    void translateTo(QPoint p_Pos);

    // I_Selectable interface
    void select();
    void select(QPoint p_Pos);
    void unselect();
    void move(QPoint p_Pos);
    bool isItYou(QPoint p_Pos) const;
    bool isSelected() const;
    bool isFullySelected() const;
    QPoint getFocusPosition() const;
    QString getSelectableType() const;
    QStringList getSearchFields() const;
    QString getDataFromField(const QString& p_FieldName) const;

    // I_Connectable interface
    virtual QString getConnectableType() const;
    virtual const QList<HandleGraphicsItem*>& getConnectableHandles() const;
    virtual QRect getCollisionRectangle() const;
    virtual QString getConnectableName() const;

    // I_Serializable
    QJsonObject toJson() const;
    void fromJson(const QJsonObject& p_Json);
    QString getSerializableName() const;

    static const char* SERIALIZABLE_NAME;

private:
    void setupHandles();
    void deleteHandles();

    void selectAllHandles();
    void selectHandlesOnSide(eConnectDirection p_Side);
    void resizeTo(QPoint p_Pos);

    unsigned short m_Diameter;

    bool m_isFullySelected;
    eConnectDirection m_SelectedHandlesDirection;

    QGraphicsEllipseItem* m_Circle;
    QList<HandleGraphicsItem*> m_Handles;

    HandleGraphicsItem* m_SelectedHandleForConnectionFrom;
    HandleGraphicsItem* m_SelectedHandleForConnectionTo;

    QString m_Name;
};

#endif // ENTRYSTATEGRAPHICSITEM_H
