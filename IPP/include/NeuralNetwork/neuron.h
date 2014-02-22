#ifndef NEURON_H
#define NEURON_H

#include <vector>

class Neuron
{
public:
    Neuron(int nbInputs);
    ~Neuron();

    int inputNb() const {return m_nbInputs;}

    std::vector<double> weights() const {return m_weights;}
    std::vector<double> prevWeights() const {return m_prevWeights;}
    std::vector<double> deltas() const {return m_deltas;}

    double weight(int i) const {return m_weights[i];}
    const double& operator[](int i) const {return m_weights[i];}

    void loadNewWeights(const std::vector<double>& inputs);

private:
    static const double MINIMUM_WEIGHT;
    static const double MAXIMUM_WEIGHT;

    void initializeWeights();
    void initializePrevWeightsAndDelta();

    int m_nbInputs;

    std::vector<double> m_weights;
    std::vector<double> m_prevWeights; // For the momentum
    std::vector<double> m_deltas;
};

#endif // NEURON_H
