#ifndef NEURON_H
#define NEURON_H

#include <vector>

class Neuron
{
public:
    Neuron(int nbInputs);
    ~Neuron();

    double computeOutput(const std::vector<double>& inputs);

    std::vector<double> weights() const {return std::vector<double>(m_weights.begin()+1, m_weights.end());}
    std::vector<double> prevWeights() const {return std::vector<double>(m_prevWeights.begin()+1, m_prevWeights.end());}

    int inputNb() const {return m_nbInputs;}
    double output() const {return m_output;}

    double threshold() const {return m_weights[0];}
    void threshold(double t) {m_weights[0]=t;}

    double weight(int i) const {return m_weights[i+1];}
    void weight(int i, double v) {m_weights[i+1] = v;}

    double prevWeight(int i) const {return m_prevWeights[i+1];}
    void prevWeight(int i, double v) {m_prevWeights[i+1] = v;}

    double prevThreshold() const {return m_prevWeights[0];}
    void prevThreshold(double v) {m_prevWeights[0] = v;}

    void updateWeight(int i, double v) {m_prevWeights[i+1] = m_weights[i+1]; m_weights[i+1] += v;}
    void updateThreshold(double v) {m_prevWeights[0] = m_weights[0]; m_weights[0] += v;}

    double delta() const {return m_delta;}
    void delta(double d) {m_delta=d;}

    const double& operator[](int i) const {return m_weights[i];}
    double& operator[](int i) {return m_weights[i];}

    void loadNewWeights(const std::vector<double>& inputs);
    void changeWeights(const std::vector<double>& newWeights); //Transfer weights[i] -> oldWeights[i]

private:
    static const double MINIMUM_WEIGHT;
    static const double MAXIMUM_WEIGHT;

    static double squashingFunction(double x);

    void initializeWeights();
    void initializePrevWeights();

    int m_nbInputs;

    std::vector<double> m_weights;
    std::vector<double> m_prevWeights; // For the momentum
    double m_delta;
    double m_output;
};

#endif // NEURON_H
