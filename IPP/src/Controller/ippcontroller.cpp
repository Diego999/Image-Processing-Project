#include "include/Controller/ippcontroller.h"
#include "include/Picture/PictureController.h"

#include <iostream>
#include <sstream>

#include <QPointF>
#include <QDir>
#include <QFile>

IPPController::IPPController(GraphicsScene& graphicsScene) :
    m_annController(nullptr),
    m_graphicsScene(&graphicsScene)
{
    m_graphicsScene->ippController(this);
}

IPPController::~IPPController()
{
    if(m_annController != nullptr)
        m_annController->stopTraining();
    thread->join();
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
    return m_annController->test(set);
}

const std::vector<std::vector<double>> IPPController::generateTargets(const std::vector<std::string>& filepaths)
{
    std::vector<std::vector<double>> targets;
    for(auto& file : filepaths)
    {
        std::vector<double> target;
        std::vector<std::string> tokens;
        std::istringstream f(file);
        std::string s;
        while(std::getline(f, s, '_'))
            tokens.push_back(s);
        target.push_back(tokens[tokens.size() - 2]=="open" ? 0.1 : 0.9);
        targets.push_back(target);
    }
    return targets;
}

void IPPController::importANN(QString annPath)
{
    m_annController = std::make_shared<ANNController>(annPath.toStdString());
}

void IPPController::exportANN(QString annPath)
{
    m_annController->exportANN(annPath.toStdString());;
}

void IPPController::configANN(const std::vector<int>& nbNeuronsPerHiddenLayer, double learningRate, double momentum, double error, bool kFoldCrossValidation, unsigned int k)
{
     m_nbNeuronsPerHiddenLayer = nbNeuronsPerHiddenLayer;
     m_learningRate = learningRate;
     m_momentum = momentum;
     m_error = error;
     m_kFoldCrossValidation = kFoldCrossValidation;
     m_k = k;
}

void IPPController::setTrainingSetPath(QString trainingSetPath)
{
    std::vector<std::string> currentFilePaths;
    QFile file(trainingSetPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::cout << "[Can't open file] -> " << trainingSetPath.toStdString() << std::endl;
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd())
        currentFilePaths.push_back(QFileInfo(trainingSetPath).absoluteDir().absolutePath().toStdString()+in.readLine().toStdString());

    std::vector<std::vector<double>> inputsTrainingSet = PictureController::loadPictures(currentFilePaths);
    std::vector<std::vector<double>> targetsTrainingSet = generateTargets(currentFilePaths);

    m_trainingSet.clear();
    for(size_t i = 0; i < inputsTrainingSet.size(); ++i)
        m_trainingSet.push_back({inputsTrainingSet[i], targetsTrainingSet[i]});
}

void IPPController::setValidationSetPath(QString validationSetPath)
{
    std::vector<std::string> currentFilePaths;
    QFile file(validationSetPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::cout << "[Can't open file] -> " << validationSetPath.toStdString() << std::endl;
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd())
        currentFilePaths.push_back(QFileInfo(validationSetPath).absoluteDir().absolutePath().toStdString()+in.readLine().toStdString());

    std::vector<std::vector<double>> inputsValidationSet = PictureController::loadPictures(currentFilePaths);
    std::vector<std::vector<double>> targetsValidationSet = generateTargets(currentFilePaths);

    m_validationSet.clear();
    for(size_t i = 0; i < inputsValidationSet.size(); ++i)
        m_validationSet.push_back({inputsValidationSet[i], targetsValidationSet[i]});
}

void IPPController::startTraining()
{
    if(!m_trainingSet.empty())
    {
        if(!m_validationSet.empty())
            m_annController = std::make_shared<ANNController>(m_nbNeuronsPerHiddenLayer, m_learningRate, m_momentum, m_trainingSet, m_validationSet);
        else
            m_annController = std::make_shared<ANNController>(m_nbNeuronsPerHiddenLayer, m_learningRate, m_momentum, m_trainingSet);
    }

    m_annController->error(m_error);

    std::function<void(void)> didFinish = [&](void)
    {
        m_graphicsScene->trainingDidFinish();
    };

    if(m_kFoldCrossValidation)
    {
        std::function<void(long, std::vector<double>&, std::vector<double>&)> callback = [&](long i, std::vector<double> &errT, std::vector<double>& errV)
        {
            std::vector<std::vector<QPointF>> points;
            auto it1 = errT.begin();
            auto it2 = errV.begin();

            while(it1 != errT.end() && it2 != errV.end())
            {
                points.push_back({QPointF(i, *it1), QPointF(i, *it2)});
                ++it1;
                ++it2;
            }
            m_graphicsScene->addPointKFoldCrossValidation(points);
        };
        std::function<void(long, double)> callbackFinalANN = [&](long i, double err)
            {
                m_graphicsScene->addPoint({QPointF(i, err)});
            };

        std::cout << "[Training start]" << std::endl;
        thread = std::shared_ptr<std::thread>(new std::thread([=](){
            m_annController->kFoldCrossValidation(callback, callbackFinalANN, m_k, didFinish);
        }));
    }
    else
    {
        std::function<void(long, double, double)> callback = [&](long iteration, double trainingError, double testingError)
        {
            if(testingError < 0)
                m_graphicsScene->addPoint({QPointF(iteration,trainingError)});
            else
                m_graphicsScene->addPoint({QPointF(iteration,trainingError), QPointF(iteration,testingError)});
        };

        std::cout << "[Training start]" << std::endl;
        thread = std::shared_ptr<std::thread>(new std::thread([=](){
            m_annController->train(callback, didFinish);
        }));
    }
}

std::vector<double> IPPController::feed(const std::vector<std::string>& filepaths)
{
    std::vector<std::vector<double>> pictures = PictureController::loadPictures(filepaths, true);
    std::vector<std::vector<double>> fullResults = m_annController->feedForward(pictures);
    std::vector<double> results;
    for(auto result : fullResults)
        results.push_back(result[0]);
    return results;
}

void IPPController::reset()
{
    if(m_annController == nullptr) return;
    m_annController->stopTraining();
    while(m_annController->isTraining()) ;
    m_annController.reset();
    thread->join();
}
