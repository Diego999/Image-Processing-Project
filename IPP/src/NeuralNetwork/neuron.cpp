#include "include/NeuralNetwork/neuron.h"
#include "include/Utils/utils.h"

const double Neuron::MAXIMUM_WEIGHT = 1.0;
const double Neuron::MINIMUM_WEIGHT = -1.0;

Neuron::Neuron(int nbInputs):m_nbInputs(nbInputs)
{
    initializeWeights();
    initializePrevWeightsAndDelta();
}

Neuron::~Neuron()
{

}

void Neuron::loadNewWeights(const std::vector<double>& inputs)
{
    m_nbInputs = inputs.size()-1;

    m_weights.clear();
    for(auto& val : inputs)
        m_weights.push_back(val);

    initializePrevWeightsAndDelta();
}

void Neuron::initializeWeights()
{
    m_weights.clear();
    for(auto& val : ipp_random::randDoubleList(MINIMUM_WEIGHT, MAXIMUM_WEIGHT, m_nbInputs+1))
        m_weights.push_back(val);
}

void Neuron::initializePrevWeightsAndDelta()
{
    m_prevWeights.clear();
    m_deltas.clear();
    for(int i = 0; i < m_nbInputs+1; ++i)
    {
        m_prevWeights.push_back(0);
        m_deltas.push_back(0);
    }
}

