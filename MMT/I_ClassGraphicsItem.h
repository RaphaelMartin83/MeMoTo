#ifndef I_CLASSGRAPHICSITEM_H
#define I_CLASSGRAPHICSITEM_H

#include <QPointF>
#include <QJsonObject>

#include <I_SquarishGraphicsItem.h>

static const unsigned short DEFAULT_CLASS_WIDTH = 250U;
static const unsigned short DEFAULT_CLASS_HEIGHT = 300U;

class I_ClassGraphicsItem:
        public I_SquarishGraphicsItem
{
public:
    I_ClassGraphicsItem(const QPoint& p_Pos, const QString& p_Name,
                            unsigned short p_Width = DEFAULT_CLASS_WIDTH,
                            unsigned short p_Height = DEFAULT_CLASS_HEIGHT);
    I_ClassGraphicsItem(const QJsonObject& p_JSon);

    ~I_ClassGraphicsItem();

    virtual void refreshDisplay();

    void setMethodsList(const QList<QString>& p_Methods);
    const QList<QString>& getMethodsList();
    void setAttributesList(const QList<QString>& p_Attributes);
    const QList<QString>& getAttributesList();

    // I_Selectable
    QStringList getSearchFields() const;
    QString getDataFromField(const QString& p_FieldName) const;

    // I_Serializable overloads
    QJsonObject toJson();
    void fromJson(QJsonObject p_Json);

private:
    void refreshText();

    QGraphicsTextItem* m_ClassContentToDisplay;

    QList<QString> m_Methods;
    QList<QString> m_Attributes;
};

#endif // I_CLASSGRAPHICSITEM_H
