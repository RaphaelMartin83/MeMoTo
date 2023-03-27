#ifndef I_SELECTABLE_H
#define I_SELECTABLE_H

#include <QGraphicsItem>

#include "I_GraphicsItem.h"

class I_Selectable:
        public I_GraphicsItem
{
public:
    virtual void select() = 0;
    virtual void select(QPoint p_Pos) = 0;
    virtual void unselect() = 0;
    virtual void move(QPoint p_Pos) = 0;
    virtual bool isItYou(QPoint p_Pos) const = 0;
    virtual bool isFullySelected() const = 0;
    virtual QPoint getFocusPosition() const = 0;
    virtual QString getSelectableType() const = 0;
    virtual QStringList getSearchFields() const = 0;
    virtual QString getDataFromField(const QString& p_FieldName) const = 0;
    virtual bool isConnectable() const {return false;};
    virtual QRect getCollisionRectangle() const = 0;
    virtual I_Selectable* getSelectable(const QString& p_SelectableType, const unsigned int& p_SelectableID) const
    {
        if( (this->getSelectableType() == p_SelectableType) &&
                (this->getID() == p_SelectableID) )
        {
            return const_cast<I_Selectable*>(this);
        }
        else
        {
            return nullptr;
        }
    }
};

#endif // I_SELECTABLE_H
