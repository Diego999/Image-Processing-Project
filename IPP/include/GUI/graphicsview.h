#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

class GraphicsView : public QGraphicsView
{
public:
    GraphicsView ();
    GraphicsView(QGraphicsScene *scene);

    virtual void resizeEvent(QResizeEvent *);
};

#endif // GRAPHICSVIEW_H
