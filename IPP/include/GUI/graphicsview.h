#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include "include/GUI/graphicsscene.h"

#include <QGraphicsView>
#include <QDragEnterEvent>

class GraphicsView : public QGraphicsView
{
public:
    GraphicsView ();
    GraphicsView(GraphicsScene *scene);

    virtual void resizeEvent(QResizeEvent *);
protected:
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dropEvent(QDropEvent* event);
};

#endif // GRAPHICSVIEW_H
