#ifndef ARTIFICIAL_NEURAL_NETWORK_H
#define ARTIFICIAL_NEURAL_NETWORK_H

#include <vector>

class NeuronLayer;

class ArtificialNeuralNetwork
{
public:
    ArtificialNeuralNetwork(int nbInputs, int nbOutputs, int nbHiddenLayers, int nbNeuronsPerHiddenLayer, double learningRate, double momentum);
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
    int nbNeuronsPerHiddenLayer() const {return m_nbNeuronsPerHiddenLayer;}

    const std::vector<double>& weight(int numLayer, int numNeuron) const;

    void inputs(const std::vector<double>& dataInputs);
    void targets(const std::vector<double>& dataTargets);

    const std::vector<double>& inputs() const {return m_inputs[0];}
    const std::vector<double>& outputs() const {return m_inputs.back();}
    const std::vector<double>& delta() const {return m_delta;}

private:
    static double squashingFunction(double x);

    void layerForward();

    int m_nbInputs;
    int m_nbOutputs;
    int m_nbHiddenLayers;
    int m_nbNeuronsPerHiddenLayer;

    std::vector<std::shared_ptr<NeuronLayer>> m_layers;
    std::vector<std::vector<double>> m_inputs;
    std::vector<double> m_targets;
    std::vector<double> m_delta;

    double m_learningRate;
    double m_momentum;
};

#endif // ARTIFICIAL_NEURAL_NETWORK_H
