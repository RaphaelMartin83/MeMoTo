#include <QApplication>
#include <QWheelEvent>
#include <QScroller>

#include "DiagramGraphicsView.h"

DiagramGraphicsView::DiagramGraphicsView()
{
    QScroller::grabGesture(this, QScroller::TouchGesture);
    QScroller::grabGesture(this, QScroller::MiddleMouseButtonGesture);
    QScrollerProperties l_Properties;
    l_Properties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy,
                                 QScrollerProperties::OvershootAlwaysOff);
    l_Properties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy,
                                 QScrollerProperties::OvershootAlwaysOff);
    // We don't want momentum
    // I guess nobody will move faster than light, right?
    l_Properties.setScrollMetric(QScrollerProperties::MinimumVelocity,
                                 3e8);
    QScroller* l_Scroller = QScroller::scroller(this);
    l_Scroller->setScrollerProperties(l_Properties);
}

void DiagramGraphicsView::wheelEvent(QWheelEvent* p_Event)
{
    if( Qt::ControlModifier == QApplication::keyboardModifiers() )
    {
        if( p_Event->angleDelta().y() > 0 )
        {
            QGraphicsView::scale(1.1, 1.1);
        }
        else
        {
            QGraphicsView::scale(0.9, 0.9);
        }
        QGraphicsView::update();
    }
    else
    {
        QGraphicsView::wheelEvent(p_Event);
    }
}
