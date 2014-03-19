#ifndef IPPCONTROLLER_H
#define IPPCONTROLLER_H

#include "include/NeuralNetwork/anncontroller.h"
#include "include/GUI/graphicsscene.h"

class IPPController
{
public:
    IPPController(GraphicsScene& graphicsScene);
private:
    ANNController* annController;

};

#endif // IPPCONTROLLER_H
