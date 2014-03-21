#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include "include/GUI/anngraphics.h"

#include <QGraphicsScene>
#include <QtWidgets>
#include <QQueue>
#include <QVector>
#include <QPointF>
#include <QMutex>

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GraphicsScene(qreal width, qreal height);

    void createUI();
    void addPoint(const std::vector<QPointF>& point);

public slots:
    void updatePoints();

private:
    QTimer *timer;
    ANNGraphics* annGraphics;
    QQueue<std::vector<QPointF>> futurePoints;
    QMutex mutex;
};

#endif // GRAPHICSSCENE_H
