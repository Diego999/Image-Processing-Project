#include "include/Tests/tests.h"
#include "include/NeuralNetwork/artificialneuralnetwork.h"
#include "include/NeuralNetwork/anncontroller.h"

#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include <cmath>

struct TestSet{
    std::string op;
    double input1, input2;
    double target;
};

void testsANN()
{
    std::vector<TestSet> testSets;

    testSets.push_back(TestSet{"AND",0,0,0.1});
    testSets.push_back(TestSet{"AND",0,1,0.1});
    testSets.push_back(TestSet{"AND",1,0,0.1});
    testSets.push_back(TestSet{"AND",1,1,0.9});

    testSets.push_back(TestSet{"NAND",0,0,0.9});
    testSets.push_back(TestSet{"NAND",0,1,0.9});
    testSets.push_back(TestSet{"NAND",1,0,0.9});
    testSets.push_back(TestSet{"NAND",1,1,0.1});

    testSets.push_back(TestSet{"OR",0,0,0.1});
    testSets.push_back(TestSet{"OR",0,1,0.9});
    testSets.push_back(TestSet{"OR",1,0,0.9});
    testSets.push_back(TestSet{"OR",1,1,0.9});

    testSets.push_back(TestSet{"NOR",0,0,0.9});
    testSets.push_back(TestSet{"NOR",0,1,0.1});
    testSets.push_back(TestSet{"NOR",1,0,0.1});
    testSets.push_back(TestSet{"NOR",1,1,0.1});

    testSets.push_back(TestSet{"XOR",0,0,0.1});
    testSets.push_back(TestSet{"XOR",0,1,0.9});
    testSets.push_back(TestSet{"XOR",1,0,0.9});
    testSets.push_back(TestSet{"XOR",1,1,0.1});

    testSets.push_back(TestSet{"NXOR",0,0,0.9});
    testSets.push_back(TestSet{"NXOR",0,1,0.1});
    testSets.push_back(TestSet{"NXOR",1,0,0.1});
    testSets.push_back(TestSet{"NXOR",1,1,0.9});

    int k = 0;
    for(size_t i = 0; i < testSets.size(); i+=4)
    {
        ArtificialNeuralNetwork ann(2, 1, {3}, 0.1, 0.1);
        double err = 0;
        size_t j = 0;
        k=0;
        do
        {
            j = 0;
            err = 0;
            while(j++<4)
            {
                TestSet ts = testSets[i+j%4];
                err += ann.train({ts.input1, ts.input2}, {ts.target});
            }
            ++k;
        }while(err >= 0.001);

        for(j = i; j < i+4; ++j)
        {
            TestSet ts = testSets[j];
            double out = ann.feedForward({ts.input1, ts.input2})[0];
            std::cout << static_cast<int>(ts.input1) << " " << ts.op << " " << static_cast<int>(ts.input2) << " -> " << out << " (" << ts.target << ")" << std::endl;
            assert(fabs(out-ts.target) < 0.4);
        }
        std::cout << "Error : " << err << std::endl << "iterations " << k << std::endl << std::endl;
    }
}

void testsANNController()
{
    std::string filepath = "testsANNController.ann";
    std::vector<std::pair<std::vector<double>, std::vector<double>>> testSets;

    testSets.push_back({{0,0},{0.1}});
    testSets.push_back({{0,1},{0.9}});
    testSets.push_back({{1,0},{0.9}});
    testSets.push_back({{1,1},{0.1}});
    ANNController* annc = new ANNController({3}, 0.5, 0.5,testSets, testSets);
    annc->error(0.01);
    std::cout << "Lambda function: " << std::endl;
    std::function<void(long, double, double)> callback = [&](long iteration, double trainingError, double testingError)
    {
        std::cout << "iteration: " << iteration << " " << trainingError << " - " << testingError << std::endl;
    };
    std::vector<std::vector<double>> validationSets;

    validationSets.push_back({0,0});
    validationSets.push_back({0,1});
    validationSets.push_back({1,0});
    validationSets.push_back({1,1});

    annc->train(callback);
    annc->exportANN(filepath);
    for(auto& result : annc->feedForward(validationSets))
        std::cout << result[0] << std::endl;

    ANNController* annc2 = new ANNController(filepath);
    for(auto& result : annc2->feedForward(validationSets))
        std::cout << result[0] << std::endl;

    std::cout << annc->log();
    std::cout << annc2->log();
    delete annc;
    delete annc2;
}
