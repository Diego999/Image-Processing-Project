#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QtWidgets>

class GraphicsScene : public QGraphicsScene
{
public:
    GraphicsScene(qreal width, qreal height);

    void createUI();
};

#endif // GRAPHICSSCENE_H
