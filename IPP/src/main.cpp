#include <iostream>
#include "include/NeuralNetwork/artificialneuralnetwork.h"

int main(int argc, char* argv[])
{
    std::vector<std::vector<double>> inputs(1);
    inputs[0].push_back(0);
    inputs[0].push_back(0);

   /* inputs[1].push_back(0);
    inputs[1].push_back(1);

    inputs[2].push_back(1);
    inputs[2].push_back(0);

    inputs[3].push_back(1);
    inputs[3].push_back(1);*/

    std::vector<std::vector<double>> targets(1);
    targets[0].push_back(0);
    /*targets[1].push_back(0.9);
    targets[2].push_back(0.9);
    targets[3].push_back(0.1);*/

    ArtificialNeuralNetwork ann(2, 1, 1, {3}, 0.3, 0.3);
    for(int i = 0; i < 1000; ++i)
    {
        for(int j = 0; j < targets.size(); ++j)
                for(auto& x : ann.feedForward(inputs[j]))
                    std::cout << j << " " << x << std::endl;
        std::cout << std::endl;
        ann.train(inputs[0], targets[0]);
    }

    return 0;
}
