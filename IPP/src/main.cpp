#include "include/Tests/tests.h"
#include <QtWidgets>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "include/GUI/graphicsscene.h"
#include "include/GUI/graphicsview.h"
#include "include/NeuralNetwork/artificialneuralnetwork.h"
#include "include/NeuralNetwork/anncontroller.h"

int main(int argc, char* argv[])
{
    std::string filepath = "test.txt";
    std::vector<std::pair<std::vector<double>, std::vector<double>>> testSets;

    testSets.push_back({{0,0},{0.1}});
    testSets.push_back({{0,1},{0.9}});
    testSets.push_back({{1,0},{0.9}});
    testSets.push_back({{1,1},{0.1}});
    ANNController* annc = new ANNController(2, 1, {3}, 0.5, 0.5,testSets, testSets);
    annc->error(0.0001);
    std::function<void(double, double)> callback = [&](double trainingError, double testingError)
    {
        std::cout << trainingError << " - " << testingError << std::endl;
    };
    std::vector<std::vector<double>> validationSets;

    validationSets.push_back({0,0});
    validationSets.push_back({0,1});
    validationSets.push_back({1,0});
    validationSets.push_back({1,1});

    annc->train(callback);
    annc->exportANN(filepath);
    for(auto& result : annc->feedForward(validationSets))
        std::cout << result[0] << std::endl;
    /*
    ANNController* annc2 = new ANNController(filepath);
    for(auto& result : annc2->feedForward(validationSets))
        std::cout << result[0] << std::endl;*/

    //testsANN();
    /*
    QApplication app(argc, argv);

    ArtificialNeuralNetwork ann(128*128, 2, 5, {1,2,3,4}, 0.5, 0.4);

    GraphicsScene scene(400, 300);
    scene.createUI();
    GraphicsView view(&scene);

    view.show();

    return app.exec();*/
    return 0;
}
