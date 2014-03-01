#include "include/NeuralNetwork/neuron.h"
#include "include/Utils/utils.h"

#include <cassert>
#include <cmath>

const double Neuron::MAXIMUM_WEIGHT = 1.0;
const double Neuron::MINIMUM_WEIGHT = -1.0;

double Neuron::squashingFunction(double x)
{
    return 1.0/(1.0+exp(-x));
}

Neuron::Neuron(int nbInputs):m_nbInputs(nbInputs), m_delta(0), m_output(0), m_weights(nbInputs+1), m_prevWeights(nbInputs+1)
{
    assert(nbInputs > 0);
    initializeWeights();
    initializePrevWeights();
}

Neuron::~Neuron()
{

}

double Neuron::computeOutput(const std::vector<double>& inputs)
{
    double sum = 0.0;
    for(size_t i = 0; i < inputs.size(); ++i)
        sum += m_weights[i+1]*inputs[i]; // i+1 -> avoid threshold
    sum -= m_weights[0]; //xi = 1.0

    m_output = squashingFunction(sum);
    return m_output;
}

void Neuron::initializeWeights()
{
    m_weights.clear();
    for(auto& val : ipp_random::randDoubleList(MINIMUM_WEIGHT, MAXIMUM_WEIGHT, m_nbInputs+1))
        m_weights.push_back(val);
}

void Neuron::initializePrevWeights()
{
    m_prevWeights.clear();
    for(size_t i = 0; i < m_weights.size(); ++i)
        m_prevWeights.push_back(0);
}

void Neuron::changeWeights(const std::vector<double>& newWeights)
{
    assert(m_weights.size()-1 == newWeights.size());

    m_prevWeights[0] = m_weights[0];
    for(size_t i = 1; i < m_weights.size(); ++i)
    {
        m_prevWeights[i] = m_weights[i];
        m_weights[i] = newWeights[i-1];
    }
}

void Neuron::loadNewWeights(const std::vector<double>& inputs)
{
    m_nbInputs = inputs.size()-1;
    m_delta = m_output = 0;

    m_weights.clear();
    for(auto& val : inputs)
        m_weights.push_back(val);

    initializePrevWeights();
}
