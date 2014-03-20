#ifndef IPPCONTROLLER_H
#define IPPCONTROLLER_H

#include "include/NeuralNetwork/anncontroller.h"
#include "include/GUI/graphicsscene.h"

#include <QPointF>
#include <QObject>

#include <thread>


class IPPController : QObject
{
    Q_OBJECT
public:
    IPPController(GraphicsScene& graphicsScene);
private:
    ANNController* annController;
    std::thread t;

};

#endif // IPPCONTROLLER_H
