#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

class SettingsNeuralNetwork
{
public:
    static const int nbInputs;
    static const int nbOutputs;
    static const int nbHiddenLayers;
    static const int nbNeuronsPerHiddenLayer;

    static const double learningRate;
    static const double momentum;

    static std::string extension;
};

#endif // SETTINGS_H
