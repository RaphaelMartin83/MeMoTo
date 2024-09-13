#ifndef AGGREGATIONGRAPHICSITEM_H
#define AGGREGATIONGRAPHICSITEM_H

#include <QGraphicsTextItem>

#include <CommonGraphics/GraphicConnector.h>
#include <Engine/I_Configurable.h>
#include <ConfigurationContexts/AggregationConfiguration.h>

class AggregationGraphicsItem:
        public GraphicConnector,
        public I_Configurable
{
public:
    AggregationGraphicsItem(
            const I_Connectable* p_ConnectFrom,
            const I_Connectable* p_ConnectTo,
            const QPoint& p_fromPoint,
            const QPoint& p_toPoint,
            I_ConnectableContainer* p_Container);
    AggregationGraphicsItem(
            const I_Connectable* p_ConnectFrom,
            const I_Connectable* p_ConnectTo,
            const QPoint& p_fromPoint,
            const QPoint& p_toPoint,
            I_ConnectableContainer* p_Container,
            const QList<QPoint>& p_ForcedPath);
    AggregationGraphicsItem(const QJsonObject& p_JsonObject,
                            I_ConnectableContainer* p_Container);

    void setAttributeNames(QList<QString> p_AttributeNames);
    const QList<QString>& getAttributesNames() const;

    void reroute();

    // I_Selectable
    void select();
    void select(QPoint p_Pos);
    void unselect();
    QString getSelectableType() const;
    QStringList getSearchFields() const;
    QString getDataFromField(const QString& p_FieldName) const;

    // I_Configurable
    void openConfiguration();
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

#endif // AGGREGATIONGRAPHICSITEM_H
