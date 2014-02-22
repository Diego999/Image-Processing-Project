#include "include/NeuralNetwork/artificialneuralnetwork.h"
#include "include/NeuralNetwork/neuronlayer.h"
#include "include/NeuralNetwork/neuron.h"

#include <iostream>
#include <cmath>
#include <memory>

double ArtificialNeuralNetwork::squashingFunction(double x)
{
    return 1.0/(1.0+exp(x));
}

ArtificialNeuralNetwork::ArtificialNeuralNetwork(int nbInputs, int nbOutputs, int nbHiddenLayers, int nbNeuronsPerHiddenLayer, double learningRate, double momentum)
    :m_nbInputs(nbInputs), m_nbOutputs(nbOutputs), m_nbHiddenLayers(nbHiddenLayers), m_nbNeuronsPerHiddenLayer(nbNeuronsPerHiddenLayer), m_learningRate(learningRate), m_momentum(momentum)
{
    m_inputs.push_back(std::vector<double>(m_nbInputs+1)); //+1 -> threshold input (=1.0)
    m_inputs[0][0] = 1.0;

    for(int i = 0; i < m_nbHiddenLayers; ++i)
    {
        m_layers.push_back(std::shared_ptr<NeuronLayer>(new NeuronLayer(m_nbNeuronsPerHiddenLayer, m_nbInputs)));
        m_delta.push_back(0);

        m_inputs.push_back(std::vector<double>(m_nbNeuronsPerHiddenLayer+1, 0)); //+1 -> threshold input (=1.0)
        m_inputs[i+1][0] = 1.0;
    }
    m_layers.push_back(std::shared_ptr<NeuronLayer>(new NeuronLayer(m_nbOutputs, m_nbNeuronsPerHiddenLayer))); // Output is considered as a layer, contrary the inputs
    m_inputs.push_back(std::vector<double>(m_nbOutputs, 0)); //To simplify the feedforward, the output of the ith-1 layer are the inputs of the ith layer. For the last layer (outputs), it is only outputs
    m_delta.push_back(0); //outputs

    for(int i = 0; i < m_nbOutputs; ++i)
        m_targets.push_back(0);
}

ArtificialNeuralNetwork::~ArtificialNeuralNetwork()
{

}

const std::vector<double>& ArtificialNeuralNetwork::feedForward(const std::vector<double>& dataInputs)
{
    inputs(dataInputs);
    return feedForward();
}

double ArtificialNeuralNetwork::train(const std::vector<double>& dataInputs, const std::vector<double>& dataTargets)
{
    inputs(dataInputs);
    targets(dataTargets);
    return train();
}

double ArtificialNeuralNetwork::train()
{
    return 0;
}

const std::vector<double>& ArtificialNeuralNetwork::feedForward()
{
    layerForward();
    return outputs();
}

void ArtificialNeuralNetwork::layerForward()
{
    for(size_t i = 0; i < m_layers.size(); ++i)
    {
        int j = 1; //To fill the real values and not the thresold
        for(auto& neuron : m_layers[i]->neurons())
        {
            double sum = 0.0;
            for(auto& input : m_inputs[i])
                sum += neuron->weight(i)*input;
            // i+1 -> Next layer
            m_inputs[i+1][j++] = ArtificialNeuralNetwork::squashingFunction(sum);
        }
    }
}

const std::vector<double>& ArtificialNeuralNetwork::weight(int numLayer, int numNeuron) const
{
    return (*m_layers[numLayer])[numNeuron]->weights();
}

void ArtificialNeuralNetwork::inputs(const std::vector<double>& dataInputs)
{
    if(dataInputs.size() != static_cast<size_t>(m_nbInputs))
        std::cerr << "[ArtificialNeuralNetwork::inputs] dataInputs don't have the same size that inputs" << std::endl;

    m_inputs[0].clear();
    for(auto& val : dataInputs)
        m_inputs[0].push_back(val);
}

void ArtificialNeuralNetwork::targets(const std::vector<double>& dataTargets)
{
    if(dataTargets.size() != static_cast<size_t>(m_nbOutputs))
        std::cerr << "[ArtificialNeuralNetwork::targs] dataTargets don't have the same size that outputs" << std::endl;
    m_targets.clear();
    for(auto& val : dataTargets)
        m_targets.push_back(val);
}
