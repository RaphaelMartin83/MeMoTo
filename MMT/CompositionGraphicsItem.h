#ifndef COMPOSITIONGRAPHICSITEM_H
#define COMPOSITIONGRAPHICSITEM_H

#include <QGraphicsTextItem>

#include "GraphicConnector.h"
#include "I_Configurable.h"

class CompositionGraphicsItem:
        public GraphicConnector,
        public I_Configurable
{
public:
    CompositionGraphicsItem(
            const I_Connectable* p_ConnectFrom,
            const I_Connectable* p_ConnectTo,
            const QPoint& p_fromPoint,
            const QPoint& p_toPoint,
            I_ConnectableContainer* p_Container);
    CompositionGraphicsItem(
            const I_Connectable* p_ConnectFrom,
            const I_Connectable* p_ConnectTo,
            const QPoint& p_fromPoint,
            const QPoint& p_toPoint,
            I_ConnectableContainer* p_Container,
            const QList<QPoint>& p_ForcedPath);
    CompositionGraphicsItem(const QJsonObject& p_JsonObject,
                            I_ConnectableContainer* p_Container);

    void setAttributeNames(QList<QString> p_AttributeNames);
    const QList<QString>& getAttributesNames() const;

    void reroute();

    // I_Selectable overloads
    void select();
    void select(QPoint p_Pos);
    void unselect();
    QString getSelectableType() const;
    QStringList getSearchFields() const;
    QString getDataFromField(const QString& p_FieldName) const;

    // I_Configurable
    void openConfiguration();
    void closeConfiguration();
    void applyConfiguration();

    // I_Serializable
    QJsonObject toJson() const;
    void fromJson(const QJsonObject& p_Json);
    QString getSerializableName() const;

    static const char* SERIALIZABLE_NAME;

private:
    void setupTip();
    void setupLabel();
    void refreshDisplay();

    void clear();

    QList<QString> m_AttributeNames;

    QGraphicsTextItem* m_Text;
    QPoint m_LabelPosition;
    unsigned short m_LabelWidth;

    QGraphicsPolygonItem* m_Tip;
};

#endif // COMPOSITIONGRAPHICSITEM_H
