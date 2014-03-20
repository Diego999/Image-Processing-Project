#ifndef IPPCONTROLLER_H
#define IPPCONTROLLER_H

#include "include/NeuralNetwork/anncontroller.h"
#include "include/GUI/graphicsscene.h"

#include <thread>
#include <memory>

class IPPController
{
public:
    IPPController(GraphicsScene& graphicsScene);
    ~IPPController();

private:
    std::vector<std::vector<double>> generateTargets(std::vector<std::string>);
    ANNController* annController;
    std::shared_ptr<std::thread> thread;
};

#endif // IPPCONTROLLER_H
