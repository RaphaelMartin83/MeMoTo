#ifndef IMPLEMENTATIONGRAPHICSITEM_H
#define IMPLEMENTATIONGRAPHICSITEM_H

#include <CommonGraphics/GraphicConnector.h>

#include <Engine/I_Configurable.h>

class ImplementationGraphicsItem:
        public GraphicConnector,
        public I_Configurable
{
public:
    ImplementationGraphicsItem(
            const I_Connectable* p_ConnectFrom,
            const I_Connectable* p_ConnectTo,
            const QPoint& p_fromPoint,
            const QPoint& p_toPoint,
            I_ConnectableContainer* p_Container);
    ImplementationGraphicsItem(
            const I_Connectable* p_ConnectFrom,
            const I_Connectable* p_ConnectTo,
            const QPoint& p_fromPoint,
            const QPoint& p_toPoint,
            I_ConnectableContainer* p_Container,
            const QList<QPoint>& p_ForcedPath);
    ImplementationGraphicsItem(const QJsonObject& p_JsonObject, I_ConnectableContainer* p_Container);

    ~ImplementationGraphicsItem();

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

#endif // IMPLEMENTATIONGRAPHICSITEM_H
