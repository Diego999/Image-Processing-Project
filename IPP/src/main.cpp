#include <iostream>
#include "include/NeuralNetwork/artificialneuralnetwork.h"

int main(int argc, char* argv[])
{
    ArtificialNeuralNetwork ann(128*128, 2, 5, {1,2,3,4}, 0.5, 0.4);
    std::cout << "Hello IPP !" << std::endl;
    return 0;
}
