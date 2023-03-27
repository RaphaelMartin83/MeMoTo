#include <QApplication>
#include <QWheelEvent>

#include "DiagramGraphicsView.h"

DiagramGraphicsView::DiagramGraphicsView()
{

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
