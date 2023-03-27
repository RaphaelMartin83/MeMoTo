#ifndef STATEGRAPHICSITEM_H
#define STATEGRAPHICSITEM_H

#include <QPoint>

#include "I_SquarishGraphicsItem.h"
#include "I_Configurable.h"
#include "I_Serializable.h"
#include "I_ToolListener.h"

#include "StateConfiguration.h"

static const unsigned short DEFAULT_STATE_WIDTH =
        static_cast<unsigned short>(GridReferential::getGridDistance(140U));
static const unsigned short DEFAULT_STATE_HEIGHT =
        static_cast<unsigned short>(GridReferential::getGridDistance(140U));

class StateGraphicsItem:
        public I_SquarishGraphicsItem,
        public I_Configurable
{
public:
    StateGraphicsItem(QPointF p_Pos,
                      unsigned short p_Width = DEFAULT_STATE_WIDTH,
                      unsigned short p_Height = DEFAULT_STATE_HEIGHT);
    StateGraphicsItem(const QJsonObject& p_JSon);
    ~StateGraphicsItem();

    // Override to add specs
    void refreshDisplay();

    // Access / mute
    void setEntryAction(const QString& p_EntryAction);
    const QString& getEntryAction() const;

    void setExitAction(const QString& p_ExitAction);
    const QString& getExitAction() const;

    void setStereotype(const QString& p_Stereotype);
    const QString& getStereotype() const;

    // I_Connectable
    QString getConnectableType() const;

    // I_Selectable overrides
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

    // I_Serializable overrides
    QJsonObject toJson();
    void fromJson(QJsonObject p_Json);
    QString getSerializableName() const;

    static const char* SERIALIZABLE_NAME;

private:
    // My Business
    void resizeTo(const QPoint& p_Pos);

    QGraphicsTextItem* m_EntryActionGraphicsItem;
    QGraphicsTextItem* m_ExitActionGraphicsItem;
    QGraphicsTextItem* m_StereotypeGraphicsItem;

    QString m_EntryAction;
    QString m_ExitAction;
    QString m_Stereotype;
};

#endif // STATEGRAPHICSITEM_H
