#include "include/NeuralNetwork/artificialneuralnetwork.h"
#include "include/NeuralNetwork/neuronlayer.h"
#include "include/NeuralNetwork/neuron.h"

#include <iostream>
#include <cmath>

double squashingFunction(double x)
{
    return 1.0/(1.0+exp(x));
}

ArtificialNeuralNetwork::ArtificialNeuralNetwork(int nbInputs, int nbOutputs, int nbHiddenLayers, int nbNeuronsPerHiddenLayer, double learningRate, double momentum)
    :m_nbInputs(nbInputs), m_nbOutputs(nbOutputs), m_nbHiddenLayers(nbHiddenLayers), m_nbNeuronsPerHiddenLayer(nbNeuronsPerHiddenLayer), m_learningRate(learningRate), m_momentum(momentum)
{
    for(int i = 0; i < m_nbInputs; ++i)
        m_inputs.push_back(0);

    for(int i = 0; i < m_nbOutputs; ++i)
    {
        m_outputs.push_back(0);
        m_targets.push_back(0);
    }

    for(int i = 0; i < m_nbHiddenLayers; ++i)
    {
        m_layers.push_back(std::shared_ptr<NeuronLayer>(new NeuronLayer(m_nbNeuronsPerHiddenLayer, m_nbInputs)));
        m_delta.push_back(0);
    }

    m_delta.push_back(0); //outputs
}

ArtificialNeuralNetwork::~ArtificialNeuralNetwork()
{

}

const std::vector<double>& ArtificialNeuralNetwork::weight(int numLayer, int numNeuron) const
{
    return (*m_layers[numLayer])[numNeuron]->weights();
}

const std::vector<double>& ArtificialNeuralNetwork::feedforward()
{
    return feedforward(m_inputs);
}

const std::vector<double>& ArtificialNeuralNetwork::feedforward(const std::vector<double>& dataInputs)
{

}

double ArtificialNeuralNetwork::train()
{
    return train(m_inputs, m_targets);
}

double ArtificialNeuralNetwork::train(const std::vector<double>& dataInputs, const std::vector<double>& dataTargets)
{

}

void ArtificialNeuralNetwork::inputs(const std::vector<double>& dataInputs)
{
    if(dataInputs.size() != static_cast<size_t>(m_nbInputs))
        std::cerr << "[ArtificialNeuralNetwork::inputs] dataInputs don't have the same size that inputs" << std::endl;

    m_inputs.clear();
    for(auto& val : dataInputs)
        m_targets.push_back(val);
}

void ArtificialNeuralNetwork::targets(const std::vector<double>& dataTargets)
{
    if(dataTargets.size() != static_cast<size_t>(m_nbOutputs))
        std::cerr << "[ArtificialNeuralNetwork::targs] dataTargets don't have the same size that outputs" << std::endl;
    m_targets.clear();
    for(auto& val : dataTargets)
        m_targets.push_back(val);
}
