#ifndef TRANSITIONGRAPHICSLABEL_H
#define TRANSITIONGRAPHICSLABEL_H

#include <QGraphicsTextItem>

class TransitionGraphicsLabel: public QGraphicsTextItem
{
public:
    TransitionGraphicsLabel();

    QRectF boundingRect() const;

    void setEvent(const QString&);
    void setAction(const QString&);
    void setGuard(const QString&);
    void setStereotype(const QString&);

private:
    QString getDisplayedString() const;

    QString m_Event;
    QString m_Action;
    QString m_Guard;
    QString m_Stereotype;
};

#endif // TRANSITIONGRAPHICSLABEL_H
