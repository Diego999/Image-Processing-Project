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
    void configANN(const std::vector<int>& nbNeuronsPerHiddenLayer, double learningRate, double momentum, double error, bool kFoldCrossValidation, unsigned int k);
    int setTrainingSetPath(QString trainingSetPath);
    int setValidationSetPath(QString validationSetPath);
    void startTraining();
    std::vector<double> feed(const std::vector<std::string>& filepaths);
    static const std::vector<std::vector<double>> generateTargets(const std::vector<std::string>& filepaths);
    bool hasValidationSet() const { return !m_validationSet.empty(); }
    void reset();

private:
    std::shared_ptr<ANNController> m_annController;
    std::shared_ptr<std::thread> thread;
    std::shared_ptr<GraphicsScene> m_graphicsScene;
    std::vector<int> m_nbNeuronsPerHiddenLayer;
    double m_learningRate;
    double m_momentum;
    double m_error;
    bool m_kFoldCrossValidation;
    unsigned int m_k;
    std::vector<std::pair<std::vector<double>, std::vector<double>>> m_trainingSet;
    std::vector<std::pair<std::vector<double>, std::vector<double>>> m_validationSet;
};

#endif // IPPCONTROLLER_H
