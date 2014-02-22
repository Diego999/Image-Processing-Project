#include "include/NeuralNetwork/neuronlayer.h"
#include "include/NeuralNetwork/neuron.h"

NeuronLayer::NeuronLayer(int nbNeurons, int nbInputs):m_nbNeurons(nbNeurons)
{
    for(int i = 0; i < m_nbNeurons; ++i)
        m_neurons.push_back(std::shared_ptr<Neuron>(new Neuron(nbInputs)));
}

NeuronLayer::~NeuronLayer()
{

}

void NeuronLayer::updateWeightNeuron(int neuron, int i, double v)
{
    return m_neurons[neuron]->updateWeight(i, v);
}

double NeuronLayer::outputNeuron(int neuron) const
{
    return m_neurons[neuron]->output();
}

double NeuronLayer::weightNeuron(int neuron, int i) const
{
    return m_neurons[neuron]->weight(i);
}

void NeuronLayer::weightNeuron(int neuron, int i, double v)
{
    m_neurons[neuron]->weight(i, v);
}

double NeuronLayer::prevWeightNeuron(int neuron, int i) const
{
    return m_neurons[neuron]->prevWeight(i);
}

void NeuronLayer::prevWeightNeuron(int neuron, int i, double v)
{
    m_neurons[neuron]->prevWeight(i, v);
}

double NeuronLayer::deltaNeuron(int neuron) const
{
    return m_neurons[neuron]->delta();
}

void NeuronLayer::deltaNeuron(int neuron, double d)
{
    m_neurons[neuron]->delta(d);
}
