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

    double testValidity(const std::vector<std::string>& filepaths) const;
private:
    std::vector<std::vector<double>> generateTargets(const std::vector<std::string> &) const;
    ANNController* annController;
    std::shared_ptr<std::thread> thread;
};

#endif // IPPCONTROLLER_H
