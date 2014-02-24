#ifndef ARTIFICIAL_NEURAL_NETWORK_H
#define ARTIFICIAL_NEURAL_NETWORK_H

#include <vector>
#include <initializer_list>

class NeuronLayer;

class ArtificialNeuralNetwork
{
public:
    //The inputs is not consider as a layer !
    ArtificialNeuralNetwork(int nbInputs, int nbOutputs, int nbHiddenLayers, std::initializer_list<int> nbNeuronsPerHiddenLayer, double learningRate, double momentum);
    ~ArtificialNeuralNetwork();

    double learningRate() const {return m_learningRate;}
    double momentum() const {return m_momentum;}

    const std::vector<double>& feedForward(); //Use inputs(...) before and then ouputs
    const std::vector<double>& feedForward(const std::vector<double>& dataInputs);

    double train(); // Use inputs(...), targets(...) before
    double train(const std::vector<double>& dataInputs, const std::vector<double>& dataTargets);

    int nbInputs() const {return m_nbInputs;}
    int nbOutputs() const {return m_nbOutputs;}
    int nbHiddenLayers() const {return m_nbHiddenLayers;}
    std::initializer_list<int> nbNeuronsPerHiddenLayer() const {return m_nbNeuronsPerHiddenLayer;}

    std::vector<double> weights(int numLayer, int numNeuron) const;

    void inputs(const std::vector<double>& dataInputs);
    void targets(const std::vector<double>& dataTargets);

    const std::vector<double>& inputs() const {return m_inputs[0];}
    const std::vector<double>& outputs() const {return m_inputs.back();}

private:
    void layerForward();
    double computeHiddenError();
    double computeOutputError();
    void adjustWeights();

    int m_nbInputs;
    int m_nbOutputs;
    int m_nbHiddenLayers;
    std::initializer_list<int> m_nbNeuronsPerHiddenLayer;

    std::vector<std::shared_ptr<NeuronLayer>> m_layers;
    std::vector<std::vector<double>> m_inputs;
    std::vector<double> m_targets; //Supposed outputs vector

    double m_learningRate;
    double m_momentum;
};

#endif // ARTIFICIAL_NEURAL_NETWORK_H
