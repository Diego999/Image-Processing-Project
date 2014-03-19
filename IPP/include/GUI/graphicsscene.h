#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include "include/GUI/anngraphics.h"

#include <QGraphicsScene>
#include <QtWidgets>

class GraphicsScene : public QGraphicsScene
{
public:
    GraphicsScene(qreal width, qreal height);

    void createUI();
    void addPoint(const std::vector<QPointF>& point);

private:
    ANNGraphics* annGraphics;
};

#endif // GRAPHICSSCENE_H
