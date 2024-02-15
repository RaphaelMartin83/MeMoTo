#ifndef DIAGRAMGRAPHICSVIEW_H
#define DIAGRAMGRAPHICSVIEW_H

#include <QGraphicsView>

class DiagramGraphicsView: public QGraphicsView
{
public:
    DiagramGraphicsView();

private slots:
    void wheelEvent(QWheelEvent* p_Event);

};

#endif // DIAGRAMGRAPHICSVIEW_H
