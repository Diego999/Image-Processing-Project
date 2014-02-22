#ifndef NEURON_LAYER_H
#define NEURON_LAYER_H

#include <vector>

class Neuron;

class NeuronLayer
{
public:
    NeuronLayer(int nbNeurons, int nbInputs);
    ~NeuronLayer();

    int nbNeurons() const {return m_nbNeurons;}
    std::vector<std::shared_ptr<Neuron>> neurons() const {return m_neurons;}

    const std::shared_ptr<Neuron>& neuron(int i) const {return m_neurons[i];}
    const std::shared_ptr<Neuron>& operator[](int i) const {return m_neurons[i];}

private:
    int m_nbNeurons;
    std::vector<std::shared_ptr<Neuron>> m_neurons;
};

#endif // NEURON_LAYER_H
