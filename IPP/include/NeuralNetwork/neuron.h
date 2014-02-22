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

    void loadNewWeights(const std::vector<double>& inputs);

private:
    static const double MINIMUM_WEIGHT;
    static const double MAXIMUM_WEIGHT;

    void initializeWeights();
    void initializePrevWeights();

    int m_nbInputs;

    std::vector<double> m_weights;
    std::vector<double> m_prevWeights; // For the momentum
};

#endif // NEURON_H
