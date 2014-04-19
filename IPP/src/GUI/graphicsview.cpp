#include "include/GUI/graphicsview.h"

#include <iostream>

GraphicsView::GraphicsView() { }

GraphicsView::GraphicsView(GraphicsScene *scene) : QGraphicsView(scene)
{
    viewport()->setAcceptDrops(true);
}

void GraphicsView::resizeEvent(QResizeEvent *)
{
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}

void GraphicsView::dragEnterEvent(QDragEnterEvent* event)
{
    GraphicsScene* graphicsScene = (GraphicsScene*)scene();
    if(graphicsScene->isDropAllow())
        event->accept();
    else
        event->ignore();
}

void GraphicsView::dragMoveEvent(QDragMoveEvent* event)
{
    GraphicsScene* graphicsScene = (GraphicsScene*)scene();
    if(graphicsScene->isDropAllow())
        event->accept();
    else
        event->ignore();
}

void GraphicsView::dropEvent(QDropEvent* event)
{
    std::vector<std::string> filePathList;
    if (event->mimeData()->hasUrls())
    {
        foreach (QUrl url, event->mimeData()->urls())
        {
            filePathList.push_back(url.toLocalFile().toStdString());
        }
    }
    GraphicsScene* graphicsScene = (GraphicsScene*)scene();
    graphicsScene->dropData(filePathList);
}
