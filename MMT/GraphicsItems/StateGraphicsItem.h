#ifndef STATEGRAPHICSITEM_H
#define STATEGRAPHICSITEM_H

#include <QPoint>

#include <Engine/I_Configurable.h>
#include <Engine/I_Serializable.h>
#include <Engine/I_ToolListener.h>

#include <CommonGraphics/I_SquarishGraphicsItem.h>

#include <ConfigurationContexts/StateConfiguration.h>

static const unsigned short DEFAULT_STATE_WIDTH =
        static_cast<unsigned short>(GridReferential::getGridDistance(160U));
static const unsigned short DEFAULT_STATE_HEIGHT =
        static_cast<unsigned short>(GridReferential::getGridDistance(160U));

class StateGraphicsItem:
        public I_SquarishGraphicsItem,
        public I_Configurable
{
public:
    StateGraphicsItem(const QPoint& p_Pos,
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
    void applyConfiguration();

    // I_Serializable overrides
    virtual QJsonObject toJson() const;
    void fromJson(const QJsonObject& p_Json);
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
