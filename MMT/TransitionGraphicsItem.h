#ifndef TRANSITIONGRAPHICSITEM_H
#define TRANSITIONGRAPHICSITEM_H

#include <QGraphicsItemGroup>

#include "GraphicConnector.h"
#include "I_Configurable.h"
#include "TransitionGraphicsLabel.h"
#include "I_Serializable.h"
#include "I_ConnectableContainer.h"

class TransitionGraphicsItem:
        public GraphicConnector,
        public I_Configurable
{
public:
    TransitionGraphicsItem(
            const I_Connectable* p_ConnectFrom,
            const I_Connectable* p_ConnectTo,
            const QPoint& p_fromPoint,
            const QPoint& p_toPoint,
            I_ConnectableContainer* p_Container);
    TransitionGraphicsItem(
            const I_Connectable* p_ConnectFrom,
            const I_Connectable* p_ConnectTo,
            const QPoint& p_fromPoint,
            const QPoint& p_toPoint,
            I_ConnectableContainer* p_Container,
            const QList<QPoint>& p_ForcedPath);
    TransitionGraphicsItem(const QJsonObject& p_JsonObject, I_ConnectableContainer* p_Container);

    ~TransitionGraphicsItem();

    void setEvent(const QString& p_Event);
    const QString& getEvent() const;

    void setAction(const QString& p_Action);
    const QString& getAction() const;

    void setGuard(const QString& p_Guard);
    const QString& getGuard() const;

    void setStereotype(const QString& p_Stereotype);
    const QString& getStereotype() const;

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
    virtual QJsonObject toJson() const;
    void fromJson(const QJsonObject& p_Json);
    QString getSerializableName() const;

    static const char* SERIALIZABLE_NAME;

private:
    void setupArrow();
    void setupLabel();
    void refreshDisplay();

    void clear();

    QString m_Guard;
    QString m_Event;
    QString m_Action;
    QString m_Stereotype;

    TransitionGraphicsLabel* m_Text;
    QPoint m_LabelPosition;
    unsigned short m_LabelWidth;

    QList<QGraphicsLineItem*> m_Arrow;
};

#endif // TRANSITIONGRAPHICSITEM_H
