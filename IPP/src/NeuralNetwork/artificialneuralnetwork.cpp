#include "include/NeuralNetwork/artificialneuralnetwork.h"
#include "include/NeuralNetwork/neuronlayer.h"
#include "include/NeuralNetwork/neuron.h"

#include <cmath>
#include <cassert>

ArtificialNeuralNetwork::ArtificialNeuralNetwork(int nbInputs, int nbOutputs, int nbHiddenLayers,  const std::vector<int>& nbNeuronsPerHiddenLayer, double learningRate, double momentum)
    :m_nbInputs(nbInputs), m_nbOutputs(nbOutputs), m_nbHiddenLayers(nbHiddenLayers), m_nbNeuronsPerHiddenLayer(nbNeuronsPerHiddenLayer), m_learningRate(learningRate), m_momentum(momentum)
{
    m_inputs.push_back(std::vector<double>(m_nbInputs));

    for(auto it = m_nbNeuronsPerHiddenLayer.begin(); it != m_nbNeuronsPerHiddenLayer.end(); ++it)
    {
        m_layers.push_back(std::shared_ptr<NeuronLayer>(new NeuronLayer(*it, m_nbInputs)));
        m_inputs.push_back(std::vector<double>(*it, 0));
    }
    m_layers.push_back(std::shared_ptr<NeuronLayer>(new NeuronLayer(m_nbOutputs, *(m_nbNeuronsPerHiddenLayer.end()-1)))); // Output is considered as a layer, contrary to the inputs
    m_inputs.push_back(std::vector<double>(m_nbOutputs, 0)); //To simplify the feedforward, the output of the ith-1 layer are the inputs of the ith layer. For the last layer (outputs), it is only outputs

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
    layerForward();
    double errorTot = 0;
    errorTot += computeOutputError();
    errorTot += computeHiddenError();
    adjustWeights();
    return errorTot;
}

const std::vector<double>& ArtificialNeuralNetwork::feedForward()
{
    layerForward();
    return outputs();
}

void ArtificialNeuralNetwork::layerForward()
{
    for(size_t i = 0; i < m_layers.size(); ++i)
        for(size_t j = 0; j < m_layers[i]->size(); ++j)
        {
            double output = m_layers[i]->neuron(j)->computeOutput(m_inputs[i]);
            m_inputs[i+1][j] = output;
        }
}

double ArtificialNeuralNetwork::computeOutputError()
{
    double totErr = 0.0;
    for(size_t i = 0; i < m_layers.back()->size(); ++i)
        for(int j = 0; j < m_nbOutputs; ++j)
        {
            double o = m_inputs.back()[j];
            double t = m_targets[j];
            double delta = o*(1.0-o)*(t-o);
            m_layers.back()->deltaNeuron(j, delta);
            totErr += abs(delta);
        }
    return totErr;
}

double ArtificialNeuralNetwork::computeHiddenError()
{
    double totErrAllHiddenLayer = 0.0;
    for(size_t i = m_layers.size()-1; i > 0 ; --i)
    {
        double totErrHiddenLayer = 0.0;
        for(size_t j = 0; j < m_layers[i-1]->size(); ++j)
        {
            double sum = 0.0;
            for(size_t k = 0; k < m_layers[i]->size(); ++k)
                sum += m_layers[i]->deltaNeuron(k)*m_layers[i]->weightNeuron(k, j);

            double h = m_layers[i-1]->outputNeuron(j);
            double delta = h*(1.0-h)*sum;
            m_layers[i-1]->deltaNeuron(j, delta);
            totErrHiddenLayer += abs(delta);
        }
        totErrAllHiddenLayer += totErrHiddenLayer;
    }
    return totErrAllHiddenLayer;
}

void ArtificialNeuralNetwork::adjustWeights()
{
    for(size_t i = 0; i < m_layers.size(); ++i)
        for(auto& neuron : m_layers[i]->neurons())
            for(size_t j = 0; j < m_inputs[i].size(); ++j)
                neuron->updateWeight(j, m_learningRate*neuron->delta()*m_inputs[i][j] + m_momentum*neuron->prevWeight(j));
}

std::vector<double> ArtificialNeuralNetwork::weights(int numLayer, int numNeuron) const
{
    return m_layers[numLayer]->neuron(numNeuron)->weights();
}

void ArtificialNeuralNetwork::inputs(const std::vector<double>& dataInputs)
{
    assert(dataInputs.size() == static_cast<size_t>(m_nbInputs));

    m_inputs[0].clear();
    for(auto& val : dataInputs)
        m_inputs[0].push_back(val);
}

void ArtificialNeuralNetwork::targets(const std::vector<double>& dataTargets)
{
    assert(dataTargets.size() == static_cast<size_t>(m_nbOutputs));

    m_targets.clear();
    for(auto& val : dataTargets)
        m_targets.push_back(val);
}
