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

    void importANN(QString annPath);
    void exportANN(QString annPath);
    void configANN(const std::vector<int>& nbNeuronsPerHiddenLayer, double learningRate, double momentum, double error);
    void setTrainingSetPath(QString trainingSetPath);
    void setValidationSetPath(QString validationSetPath);
    void startTraining();
    void feed(const std::vector<std::string>& filepaths);

private:
    std::vector<std::vector<double>> generateTargets(const std::vector<std::string> &) const;
    ANNController* annController;
    std::shared_ptr<std::thread> thread;
    std::shared_ptr<GraphicsScene> m_graphicsScene;
    std::vector<int> m_nbNeuronsPerHiddenLayer;
    double m_learningRate;
    double m_momentum;
    double m_error;
    std::vector<std::pair<std::vector<double>, std::vector<double>>> m_trainingSet;
    std::vector<std::pair<std::vector<double>, std::vector<double>>> m_validationSet;
};

#endif // IPPCONTROLLER_H
