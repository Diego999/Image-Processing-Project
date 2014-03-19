#include "include/Controller/ippcontroller.h"

IPPController::IPPController(GraphicsScene& graphicsScene)
{
    std::vector<std::pair<std::vector<double>, std::vector<double>>> testSets;

    testSets.push_back({{0,0},{0.1}});
    testSets.push_back({{0,1},{0.9}});
    testSets.push_back({{1,0},{0.9}});
    testSets.push_back({{1,1},{0.1}});
    annController = new ANNController(2, 1, {3}, 0.5, 0.5, testSets);

    annController->error(0.0001);

    std::function<void(long, double, double)> callback = [&](long iteration, double trainingError, double testingError)
    {
        graphicsScene.addPoint({QPointF(iteration,trainingError)});
    };

    annController->train(callback);
}
