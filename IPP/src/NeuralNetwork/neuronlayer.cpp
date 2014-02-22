#include "include/NeuralNetwork/neuronlayer.h"
#include "include/NeuralNetwork/neuron.h"

NeuronLayer::NeuronLayer(int nbNeurons, int nbInputs):m_nbNeurons(nbNeurons)
{
    for(int i = 0; i < m_nbNeurons; ++i)
        m_neurons.push_back(std::shared_ptr<Neuron>(new Neuron(nbInputs)));
}

NeuronLayer::~NeuronLayer()
{
    //Pointers will be deleted when there won't be any references
}

