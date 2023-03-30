#ifndef CLASSGRAPHICSITEM_H
#define CLASSGRAPHICSITEM_H

#include "I_ClassGraphicsItem.h"
#include "I_Configurable.h"

class ClassGraphicsItem:
        public I_ClassGraphicsItem,
        public I_Configurable
{
public:
    ClassGraphicsItem(const QPoint& p_Pos,
                            unsigned short p_Width = DEFAULT_CLASS_WIDTH,
                            unsigned short p_Height = DEFAULT_CLASS_HEIGHT);
    ClassGraphicsItem(const QJsonObject& p_JSon);
    ~ClassGraphicsItem();

    // I_Selectable overrides
    void select();
    void select(QPoint p_Pos);
    void unselect();
    QString getSelectableType() const;

    // I_Configurable
    void openConfiguration();
    void closeConfiguration();
    void applyConfiguration();

    // I_Connectable
    QString getConnectableType() const;

    // I_Serializable
    QString getSerializableName() const;

    static const char* SERIALIZABLE_NAME;
};

#endif // CLASSGRAPHICSITEM_H
