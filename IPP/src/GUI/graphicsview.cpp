#include "include/GUI/graphicsview.h"

GraphicsView::GraphicsView()
{

}

GraphicsView::GraphicsView(QGraphicsScene *scene) : QGraphicsView(scene)
{
}

void GraphicsView::resizeEvent(QResizeEvent *)
{
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}
