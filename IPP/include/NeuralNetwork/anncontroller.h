#ifndef ANNCONTROLLER_H
#define ANNCONTROLLER_H

#include <vector>
#include <string>
#include <memory>
#include <functional>

class ArtificialNeuralNetwork;

class ANNController
{
public:
    ANNController(int nbInputs, int nbOutputs, const std::vector<int>& nbNeuronsPerHiddenLayer, double learningRate, double momentum);
    ANNController(int nbInputs, int nbOutputs, const std::vector<int>& nbNeuronsPerHiddenLayer, double learningRate, double momentum, const std::vector<std::pair<std::vector<double>, std::vector<double>>>& trainingSet);
    ANNController(int nbInputs, int nbOutputs, const std::vector<int>& nbNeuronsPerHiddenLayer, double learningRate, double momentum, const std::vector<std::pair<std::vector<double>, std::vector<double>>>& trainingSet, const std::vector<std::pair<std::vector<double>, std::vector<double>>>& testSet);
    ANNController(const std::string& filepath);
    ANNController(const std::string& filepath, const std::vector<std::pair<std::vector<double>, std::vector<double>>>& trainingSet);
    ANNController(const std::string& filepath, const std::vector<std::pair<std::vector<double>, std::vector<double>>>& trainingSet, const std::vector<std::pair<std::vector<double>, std::vector<double>>>& testSet);

    void train(const std::function<void(long, double, double)> &callback);
    const std::vector<double>& feedForward(const std::vector<double>& dataInputs);
    const std::vector<std::vector<double>>& feedForward(const std::vector<std::vector<double>>& dataInputs);
    std::vector<double> weights(int numLayer, int numNeuron) const;
    void exportANN(const std::string& filepath);

    double error() const { return m_error; }

    void error(double error) { m_error = error; }

    std::string log() const;

private:
    void importANN(const std::string& filepath);
    void createANN(int nbInputs, int nbOutputs, const std::vector<int>& nbNeuronsPerHiddenLayer, double learningRate, double momentum);

    std::shared_ptr<ArtificialNeuralNetwork> m_ann;

    std::vector<std::pair<std::vector<double>, std::vector<double>>> m_trainingSet;
    std::vector<std::pair<std::vector<double>, std::vector<double>>> m_testSet;

    double m_error;
};

#endif // ANNCONTROLLER_H
