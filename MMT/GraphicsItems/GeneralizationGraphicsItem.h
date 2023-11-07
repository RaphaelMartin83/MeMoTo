#ifndef GENERALIZATIONGRAPHICSITEM_H
#define GENERALIZATIONGRAPHICSITEM_H

#include <Engine/I_Configurable.h>

#include <CommonGraphics/GraphicConnector.h>

class GeneralizationGraphicsItem:
        public GraphicConnector,
        public I_Configurable
{
public:
    GeneralizationGraphicsItem(
            const I_Connectable* p_ConnectFrom,
            const I_Connectable* p_ConnectTo,
            const QPoint& p_fromPoint,
            const QPoint& p_toPoint,
            I_ConnectableContainer* p_Container);
    GeneralizationGraphicsItem(
            const I_Connectable* p_ConnectFrom,
            const I_Connectable* p_ConnectTo,
            const QPoint& p_fromPoint,
            const QPoint& p_toPoint,
            I_ConnectableContainer* p_Container,
            const QList<QPoint>& p_ForcedPath);
    GeneralizationGraphicsItem(const QJsonObject& p_JsonObject, I_ConnectableContainer* p_Container);

    ~GeneralizationGraphicsItem();

    void reroute();

    // I_Serializable overloads
    QString getSerializableName() const;

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

    static const char* SERIALIZABLE_NAME;

private:
    void setupArrow();
    void refreshDisplay();

    void clear();

    QList<QGraphicsLineItem*> m_Arrow;
};

#endif // GENERALIZATIONGRAPHICSITEM_H
