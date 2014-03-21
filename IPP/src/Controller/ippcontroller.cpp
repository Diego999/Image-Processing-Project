#include "include/Controller/ippcontroller.h"
#include "include/Picture/PictureController.h"

#include <iostream>
#include <sstream>

#include <QPointF>
#include <QDir>
#include <QFile>

IPPController::IPPController(GraphicsScene& graphicsScene):annController(nullptr)
{
    QDir dir = QDir::current();

    QStringList filters;
    filters << "*.list";
    dir.setNameFilters(filters);
    QStringList files = dir.entryList();
    std::vector<std::vector<std::string>> filesPath;
    for(QString filePath : files)
    {
        std::vector<std::string> currentFilePaths;
        QFile file(dir.absolutePath().append("/").append(filePath));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            std::cout << "[Can't open file] -> " << filePath.toStdString() << std::endl;
            return;
        }

        QTextStream in(&file);
        while (!in.atEnd())
            currentFilePaths.push_back(dir.absolutePath().append(in.readLine()).toStdString());
        filesPath.push_back(currentFilePaths);
    }

    std::cout << "[File path extract]" << std::endl;

    std::vector<std::vector<double>> inputsTrainingSet = PictureController::loadPictures(filesPath[1]);
    std::vector<std::vector<double>> targetsTrainingSet = generateTargets(filesPath[1]);
    std::vector<std::vector<double>> inputsValidationSet = PictureController::loadPictures(filesPath[0]);
    std::vector<std::vector<double>> targetsValidationSet = generateTargets(filesPath[0]);

    std::cout << "[Picutes loaded]" << std::endl;

    std::vector<std::pair<std::vector<double>, std::vector<double>>> trainingSet, validationSet;

    for(size_t i = 0; i < inputsTrainingSet.size(); ++i)
        trainingSet.push_back({inputsTrainingSet[i], targetsTrainingSet[i]});
    for(size_t i = 0; i < inputsValidationSet.size(); ++i)
        validationSet.push_back({inputsValidationSet[i], targetsValidationSet[i]});


    std::cout << "[Set created]" << std::endl;

    /*
    std::vector<std::pair<std::vector<double>, std::vector<double>>> testSets;

    testSets.push_back({{0,0},{0.1}});
    testSets.push_back({{0,1},{0.9}});
    testSets.push_back({{1,0},{0.9}});
    testSets.push_back({{1,1},{0.1}});*/

    annController = new ANNController({10}, 0.3, 0.3, trainingSet, validationSet);

    annController->error(0.01);

    std::function<void(long, double, double)> callback = [&](long iteration, double trainingError, double testingError)
    {
        graphicsScene.addPoint({QPointF(iteration,trainingError), QPointF(iteration,testingError)});
    };

    std::cout << "[Training start]" << std::endl;
    thread = std::shared_ptr<std::thread>(new std::thread([=](){
        annController->train(callback);
    }));
}

IPPController::~IPPController()
{
    if(annController != nullptr)
        annController->stopTraining();
}

double IPPController::testValidity(const std::vector<std::string>& filepaths) const
{
    //Make it proper
    QString filePath("/Users/diego/test/all_train.list");

    std::vector<std::string> filePaths;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::cout << "[Can't open file] -> " << filePath.toStdString() << std::endl;
        return -1;
    }

    QTextStream in(&file);
    while (!in.atEnd())
        filePaths.push_back(in.readLine().toStdString());

    std::cout << "[File path extract]" << std::endl;

    std::vector<std::vector<double>> inputSet = PictureController::loadPictures(filePaths);
    std::vector<std::vector<double>> targetSet = generateTargets(filePaths);

    std::vector<std::pair<std::vector<double>, std::vector<double>>> set;

    for(size_t i = 0; i < inputSet.size(); ++i)
        set.push_back({inputSet[i], targetSet[i]});
    return annController->test(set);
}

std::vector<std::vector<double>> IPPController::generateTargets(const std::vector<std::string>& files) const
{
    std::vector<std::vector<double>> targets;
    for(auto& file : files)
    {
        std::vector<double> target;
        std::vector<std::string> tokens;
        std::istringstream f(file);
        std::string s;
        while(std::getline(f, s, '_'))
            tokens.push_back(s);
        target.push_back(tokens[3]=="open" ? 0.1 : 0.9);
        targets.push_back(target);
    }
    return targets;
}
