#include "include/NeuralNetwork/anncontroller.h"
#include "include/NeuralNetwork/artificialneuralnetwork.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define DEFAULT_ERROR 0.001
#define SPACE " "

ANNController::ANNController(int nbInputs, int nbOutputs, const std::vector<int>& nbNeuronsPerHiddenLayer, double learningRate, double momentum)
{
    createANN(nbInputs, nbOutputs, nbNeuronsPerHiddenLayer, learningRate, momentum);
    m_error = DEFAULT_ERROR;
}

ANNController::ANNController(int nbInputs, int nbOutputs, const std::vector<int>& nbNeuronsPerHiddenLayer, double learningRate, double momentum, const std::vector<std::pair<std::vector<double>, std::vector<double>>>& trainingSet) :
    ANNController(nbInputs, nbOutputs, nbNeuronsPerHiddenLayer, learningRate, momentum)
{
    m_trainingSet = trainingSet;
}

ANNController::ANNController(int nbInputs, int nbOutputs, const std::vector<int>& nbNeuronsPerHiddenLayer, double learningRate, double momentum, const std::vector<std::pair<std::vector<double>, std::vector<double>>>& trainingSet, const std::vector<std::pair<std::vector<double>, std::vector<double>>>& testSet) :
    ANNController(nbInputs, nbOutputs, nbNeuronsPerHiddenLayer, learningRate, momentum, trainingSet)
{
    m_testSet = testSet;
}

ANNController::ANNController(const std::string& filepath)
{
    importANN(filepath);
    m_error = DEFAULT_ERROR;
}

ANNController::ANNController(const std::string& filepath, const std::vector<std::pair<std::vector<double>, std::vector<double>>>& trainingSet) :
    ANNController(filepath)
{
    m_trainingSet = trainingSet;
}

ANNController::ANNController(const std::string& filepath, const std::vector<std::pair<std::vector<double>, std::vector<double>>>& trainingSet, const std::vector<std::pair<std::vector<double>, std::vector<double>>>& testSet) :
    ANNController(filepath, trainingSet)
{
    m_testSet = testSet;
}

void ANNController::train(const std::function<void(double, double)> &callback)
{
    const size_t trainingQuantity = m_trainingSet.size();
    const size_t testQuantity = m_testSet.size();
    size_t j;
    double totalCurrentErrorTraining;
    double totalCurrentErrorTest;
    do
    {
        j = -1;
        totalCurrentErrorTraining = 0.0;
        totalCurrentErrorTest = 0.0;

        while(++j < trainingQuantity)
        {
            std::pair<std::vector<double>, std::vector<double>> training = m_trainingSet[j];
            totalCurrentErrorTraining += m_ann->train(training.first, training.second);
        }
        j = -1;
        while(++j < testQuantity)
        {
            std::pair<std::vector<double>, std::vector<double>> test = m_testSet[j];
            totalCurrentErrorTest += m_ann->train(test.first, test.second);
        }

        callback(totalCurrentErrorTraining, totalCurrentErrorTest);
    } while(totalCurrentErrorTraining >= m_error);
}

const std::vector<double>&  ANNController::feedForward(const std::vector<double>& dataInputs)
{
    return m_ann->feedForward(dataInputs);
}

const std::vector<std::vector<double>>&  ANNController::feedForward(const std::vector<std::vector<double>>& dataInputs)
{
    std::vector<std::vector<double>>* results = new std::vector<std::vector<double>>();
    for(size_t i = 0; i < dataInputs.size(); ++i)
        results->push_back(m_ann->feedForward(dataInputs[i]));
    return *results;
}

std::vector<double> ANNController::weights(int numLayer, int numNeuron) const
{
    return m_ann->weights(numLayer, numNeuron);
}

void ANNController::exportANN(const std::string& filepath)
{
    std::ofstream file(filepath);
    file << m_ann->nbInputs() << SPACE << m_ann->nbOutputs() << SPACE << m_ann->nbHiddenLayers() << SPACE;
    for(auto& nbNeurons : m_ann->nbNeuronsPerHiddenLayer())
        file << nbNeurons << SPACE;
    file << m_ann->learningRate() << SPACE << m_ann->momentum() << SPACE;
    for(int i = 0; i < m_ann->nbHiddenLayers(); ++i)
    {
        for(int j = 0; j < m_ann->nbNeuronsPerHiddenLayer()[i]; ++j)
        {
            for(auto& weight : m_ann->weights(i,j))
                file << weight << SPACE;
            file << m_ann->threshold(i, j) << SPACE << m_ann->deltaNeuron(i, j) << SPACE;
        }
    }
    file.close();
}

void ANNController::importANN(const std::string& filepath)
{
    std::ifstream rfile(filepath);
    std::stringstream buffer;
    buffer << rfile.rdbuf();
    std::string content(buffer.str());
    std::string::size_type fullIdx = 0;
    std::string::size_type idx = 0;
    int nbInputs = std::stoi(content, &idx);
    fullIdx += idx;
    int nbOutputs = std::stoi(content.substr(fullIdx), &idx);
    fullIdx += idx;
    int nbHiddenLayers = std::stoi(content.substr(fullIdx), &idx);
    fullIdx += idx;
    std::vector<int> nbNeuronsPerHiddenLayer;
    for(int i = 0; i < nbHiddenLayers; ++i)
    {
        int nbNeurons = std::stoi(content.substr(fullIdx), &idx);
        fullIdx += idx;
        nbNeuronsPerHiddenLayer.push_back(nbNeurons);
    }

    double learningRate = std::stod(content.substr(fullIdx), &idx);
    fullIdx += idx;
    double momentum = std::stod(content.substr(fullIdx), &idx);
    fullIdx += idx;

    std::vector<std::vector<std::tuple<std::vector<double>, double, double>>> neurons;
    for(int i = 0; i < nbHiddenLayers; ++i)
    {
        std::vector<std::tuple<std::vector<double>, double, double>> layer;
        for(int j = 0; j < nbNeuronsPerHiddenLayer[i]; ++j)
        {
            size_t nbNeuronsParent = (i > 0) ? nbNeuronsPerHiddenLayer[i-1] : nbInputs;
            std::vector<double> weights;
            for(size_t k = 0; k < nbNeuronsParent; ++k)
            {
                double weight = std::stod(content.substr(fullIdx), &idx);
                fullIdx += idx;
                weights.push_back(weight);
            }
            double threshold = std::stod(content.substr(fullIdx), &idx);
            fullIdx += idx;
            double deltaNeuron = std::stod(content.substr(fullIdx), &idx);
            fullIdx += idx;
            std::tuple<std::vector<double>, double, double> neuron(weights, threshold, deltaNeuron);
            layer.push_back(neuron);
        }
        neurons.push_back(layer);
    }

    createANN(nbInputs, nbOutputs, nbNeuronsPerHiddenLayer, learningRate, momentum);

    for(size_t i = 0; i < neurons.size(); ++i)
        for(size_t j = 0; j < neurons[i].size(); ++j)
        {
            for(size_t k = 0; k < std::get<0>(neurons[i][j]).size(); ++k)
                m_ann->weights(i, j, k, std::get<0>(neurons[i][j])[k]);
            m_ann->threshold(i, j, std::get<1>(neurons[i][j]));
            m_ann->deltaNeuron(i, j, std::get<2>(neurons[i][j]));
        }
}

void ANNController::createANN(int nbInputs, int nbOutputs, const std::vector<int>& nbNeuronsPerHiddenLayer, double learningRate, double momentum)
{
    m_ann = std::shared_ptr<ArtificialNeuralNetwork>(new ArtificialNeuralNetwork(nbInputs, nbOutputs, nbNeuronsPerHiddenLayer, learningRate, momentum));
}
