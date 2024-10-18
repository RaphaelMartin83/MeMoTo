#ifndef EXTERNALCLASSGRAPHICSITEM_H
#define EXTERNALCLASSGRAPHICSITEM_H

#include <Engine/I_ClassGraphicsItem.h>
#include <Engine/I_Configurable.h>

class ExternalClassGraphicsItem:
        public I_ClassGraphicsItem,
        public I_Configurable
{
public:
    ExternalClassGraphicsItem(const QPoint& p_Pos,
                            unsigned short p_Width = DEFAULT_CLASS_WIDTH,
                            unsigned short p_Height = DEFAULT_CLASS_HEIGHT);
    ExternalClassGraphicsItem(const QJsonObject& p_JSon);
    ~ExternalClassGraphicsItem();

    // I_Selectable overrides
    void select();
    void select(QPoint p_Pos);
    void unselect();
    QString getSelectableType() const;

    // I_Configurable
    void openConfiguration();
    void applyConfiguration();

    // I_Connectable
    QString getConnectableType() const;

    // I_Serializable overloads
    QJsonObject toJson() const;
    void fromJson(const QJsonObject& p_Json);
    QString getSerializableName() const;

    const QString& getRoot() const;
    const QString& getPath() const;
    void setRootAndPath(const QString& p_Root,
                        const QString& p_Path);
    QString getResolvedFullPath();
    void refreshDisplay();

    void fromExternalJson(QJsonObject p_Json);

    static const char* SERIALIZABLE_NAME;

private:
    QString m_Root;
    QString m_Path;
};

#endif // EXTERNALCLASSGRAPHICSITEM_H
