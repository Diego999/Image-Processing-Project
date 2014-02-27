#include <iostream>
#include "include/NeuralNetwork/artificialneuralnetwork.h"

int main(int argc, char* argv[])
{
    std::vector<std::vector<double>> inputs(1);
    //inputs[0].push_back(0);
    //inputs[0].push_back(0);

    //inputs[1].push_back(0);
    //inputs[1].push_back(1);

    //inputs[2].push_back(1);
    //inputs[2].push_back(0);

    inputs[0].push_back(0);
    inputs[0].push_back(1);

    std::vector<std::vector<double>> targets(1);
    //targets[0].push_back(0.1);
    //targets[1].push_back(0.9);
    //targets[2].push_back(0.9);
    targets[0].push_back(1);

    ArtificialNeuralNetwork ann(2, 1, 1, {3}, 0.5, 0);
    double err = 0;
    do
    {
        //for(size_t j = 0; j < targets.size(); ++j)
        //        for(auto& x : ann.feedForward(inputs[j]))
        //            std::cout << j << " " << x << std::endl;
        err =  ann.train(inputs[0], targets[0]);
        std::cout << "Err : " << err << std::endl;
        std::cout << std::endl;
    }while(err >= 0.001);
    std::cout << ann.feedForward(inputs[0])[0] << std::endl;

    return 0;
}
