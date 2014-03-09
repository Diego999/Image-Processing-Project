#ifndef ARTIFICIAL_NEURAL_NETWORK_H
#define ARTIFICIAL_NEURAL_NETWORK_H

#include <vector>
#include <initializer_list>
#include <memory>

class NeuronLayer;

class ArtificialNeuralNetwork
{
public:
    //The inputs is not consider as a layer !
    ArtificialNeuralNetwork(int nbInputs, int nbOutputs, const std::vector<int>& nbNeuronsPerHiddenLayer, double learningRate, double momentum);
    ~ArtificialNeuralNetwork();

    double learningRate() const {return m_learningRate;}
    double momentum() const {return m_momentum;}

    const std::vector<double>& feedForward(const std::vector<double>& dataInputs);
    double train(const std::vector<double>& dataInputs, const std::vector<double>& dataTargets);

    int nbInputs() const {return m_nbInputs;}
    int nbOutputs() const {return m_nbOutputs;}
    int nbHiddenLayers() const {return m_nbHiddenLayers;}
    const std::vector<int>& nbNeuronsPerHiddenLayer() const {return m_nbNeuronsPerHiddenLayer;}

    double threshold(int numLayer, int numNeuron) const;
    double deltaNeuron(int numLayer, int numNeuron) const;
    std::vector<double> weights(int numLayer, int numNeuron) const;

    void threshold(int numLayer, int numNeuron, double threshold);
    void deltaNeuron(int numLayer, int numNeuron, double deltaNeuron);
    void weights(int numLayer, int numNeuron, int i, double weights);

private:
    void layerForward();
    double computeHiddenError();
    double computeOutputError();
    void adjustWeights();

    int m_nbInputs;
    int m_nbOutputs;
    int m_nbHiddenLayers;
    std::vector<int> m_nbNeuronsPerHiddenLayer;

    std::vector<std::shared_ptr<NeuronLayer>> m_layers;
    std::vector<std::vector<double>> m_inputs;
    std::vector<double> m_targets; //Supposed outputs vector

    double m_learningRate;
    double m_momentum;
};

#endif // ARTIFICIAL_NEURAL_NETWORK_H
