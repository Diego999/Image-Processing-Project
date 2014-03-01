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
    const std::vector<std::shared_ptr<Neuron>>& neurons() const {return m_neurons;}
    size_t size() const {return m_neurons.size();}
    double weightNeuron(int neuron, int i) const;
    double threshold(int neuron) const;
    double prevThreshold(int neuron) const;
    double prevWeightNeuron(int neuron, int i) const;
    double deltaNeuron(int neuron) const;
    double outputNeuron(int neuron) const;

    void weightNeuron(int neuron, int i, double v);
    void prevWeightNeuron(int neuron, int i, double v);
    void deltaNeuron(int neuron, double d);
    void updateWeightNeuron(int neuron, int i, double v);
    void threshold(int neuron, double v);
    void prevThreshold(int neuron, double v);

    const std::shared_ptr<Neuron>& neuron(int i) const {return m_neurons[i];}
    std::shared_ptr<Neuron>& operator[](int i)  {return m_neurons[i];}
    const std::shared_ptr<Neuron>& operator[](int i) const {return m_neurons[i];}

private:
    int m_nbNeurons;
    std::vector<std::shared_ptr<Neuron>> m_neurons;
};

#endif // NEURON_LAYER_H
