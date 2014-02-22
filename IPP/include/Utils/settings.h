#ifndef SETTINGS_H
#define SETTINGS_H

class SettingsNeuralNetwork
{
    static int nbInputs;
    static int nbOutputs;
    static int nbHiddenLayers;
    static int nbNeuronsPerHiddenLayer;

    static double learningRate;
    static double momentum;
};

#endif // SETTINGS_H
