#ifndef CHOICEGRAPHICSITEM_H
#define CHOICEGRAPHICSITEM_H

#include <QGraphicsItemGroup>
#include "I_Selectable.h"
#include "I_Connectable.h"
#include "HandleGraphicsItem.h"
#include "I_Configurable.h"

class ChoiceGraphicsItem:
        public I_Connectable,
        public I_Configurable
{
public:
    ChoiceGraphicsItem(const QPoint& p_Pos);
    ChoiceGraphicsItem(const QJsonObject& p_JsonObject);
    ~ChoiceGraphicsItem();

    void setName(const QString& p_Name);
    const QString& getName() const;

    void refreshDisplay();
    void translateTo(QPoint p_Pos);

    // I_Selectable interface
    void select();
    void select(QPoint p_Pos);
    void unselect();
    void move(QPoint p_Pos);
    bool isItYou(QPoint p_Pos) const;
    virtual bool isFullySelected() const;
    QPoint getFocusPosition() const;
    QString getSelectableType() const;
    QStringList getSearchFields() const;
    QString getDataFromField(const QString& p_FieldName) const;

    // I_Connectable interface
    virtual QString getConnectableType() const;
    virtual const QList<HandleGraphicsItem*>& getConnectableHandles() const;
    virtual QRect getCollisionRectangle() const;
    virtual QString getConnectableName() const;

    // I_Configurable
    void openConfiguration();
    void closeConfiguration();
    void applyConfiguration();

    // I_Serializable
    QJsonObject toJson() const;
    void fromJson(QJsonObject p_Json);
    QString getSerializableName() const;

    static const char* SERIALIZABLE_NAME;

private:
    void setupHandles();
    void deleteHandles();
    void resizeTo(QPoint p_Pos);
    void selectAllHandles();
    void selectHandlesOnSide(eConnectDirection p_Side);

    unsigned short m_Width;
    unsigned short m_Height;

    bool m_isFullySelected;
    eConnectDirection m_SelectedHandlesDirection;

    QGraphicsPolygonItem* m_Polygon;
    QGraphicsTextItem* m_NameGraphicsItem;
    QList<HandleGraphicsItem*> m_Handles;

    HandleGraphicsItem* m_SelectedHandle;
    HandleGraphicsItem* m_SelectedHandleForConnectionFrom;
    HandleGraphicsItem* m_SelectedHandleForConnectionTo;

    QString m_Name;
};

#endif // CHOICEGRAPHICSITEM_H
