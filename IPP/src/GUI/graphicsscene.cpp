#include "include/Controller/ippcontroller.h"
#include "include/GUI/graphicsscene.h"
#include "include/Utils/settings.h"
#include "include/Picture/PictureController.h"

#include <qwt_symbol.h>

#include <tuple>
#include <iostream>
#include <cmath>

#include <QMutex>

#define BORDER 5

#define START_MENU 0
#define SELECT_SETS_MENU 1
#define TRAINING_VIEW 2
#define MENU_MENU 3
#define IMAGE_WIDHT 320
#define IMAGE_HEIGHT 300

GraphicsScene::GraphicsScene(const QSize &size) : QGraphicsScene(0, 0, size.width(), size.height()),
    // Text edit and button
    m_newButton(tr("New")),
    m_importButton(tr("Import")),
    m_exportButton(tr("Export")),
    m_trainingButton(tr("Select training set")),
    m_validationButton(tr("Select validation set")),
    m_startTrainingButton(tr("Start training")),
    m_nextButton("->"),
    m_backButton("<-"),
    // the graphics
    m_annGraphics("Number of iterations", "Error"),
    //the backgrounds
    m_bg1(QPixmap(":images/bg1")),
    m_bg2(QPixmap(":images/bg2")),
    m_bg3(QPixmap(":images/bg3")),
    m_bg4(QPixmap(":images/bg4")),
    m_draggedImage(QPixmap(":images/draghere")),
    m_currentState(0),
    m_finished(false)
{
    setBackgroundBrush(this->palette().window());
    m_errorsLabel.setStyleSheet("* {background-color: #55FFFFFF; font-size: 15px; padding: 5px 9px;}");
    m_errorsLabel.resize(220, 64); // Values from test
    m_resultLabel.setStyleSheet("* {background-color: #55FFFFFF; font-size: 15px; padding: 5px 9px;}");
    m_resultLabel.resize(IMAGE_WIDHT, 64); // Values from test
}

void GraphicsScene::createUI()
{
    // First add background
    addItem(&m_bg1);
    addItem(&m_bg2);
    addItem(&m_bg3);
    addItem(&m_bg4);

    m_bg1.setPos(-5,-5);
    m_bg2.setPos(-5,-5);
    m_bg3.setPos(-5,-5);
    m_bg4.setPos(-5,-5);

    /**
      http://qt-project.org/doc/qt-5/statemachine-api.html
    **/

    connect(&m_newButton, &QPushButton::clicked, this, &GraphicsScene::createANN);
    connect(&m_importButton, &QPushButton::clicked, this, &GraphicsScene::importANN);
    connect(&m_exportButton, &QPushButton::clicked, this, &GraphicsScene::exportANN);
    connect(&m_trainingButton, &QPushButton::clicked, this, &GraphicsScene::selectTrainingSet);
    connect(&m_validationButton, &QPushButton::clicked, this, &GraphicsScene::selectValidationSet);
    connect(&m_startTrainingButton, &QPushButton::clicked, this, &GraphicsScene::startTraining);
    connect(&m_nextButton, &QPushButton::clicked, this, &GraphicsScene::goToNextState);
    connect(&m_backButton, &QPushButton::clicked, this, &GraphicsScene::backToStartMenu);

    m_annGraphics.resize(550, 550);

    QGraphicsProxyWidget *newButtonProxy = new QGraphicsProxyWidget;
    QGraphicsProxyWidget *importButtonProxy = new QGraphicsProxyWidget;
    QGraphicsProxyWidget *exportButtonProxy = new QGraphicsProxyWidget;
    QGraphicsProxyWidget *trainingButtonProxy = new QGraphicsProxyWidget;
    QGraphicsProxyWidget *testButtonProxy = new QGraphicsProxyWidget;
    QGraphicsProxyWidget *startTrainingButtonProxy = new QGraphicsProxyWidget;
    QGraphicsProxyWidget *nextButtonProxy = new QGraphicsProxyWidget;
    QGraphicsProxyWidget *backButtonProxy = new QGraphicsProxyWidget;

    QGraphicsProxyWidget *errorsLabelProxy = new QGraphicsProxyWidget;
    QGraphicsProxyWidget *resultsLabelProxy = new QGraphicsProxyWidget;

    QGraphicsProxyWidget *annGraphicsProxy = new QGraphicsProxyWidget;

    newButtonProxy->setWidget(&m_newButton);
    importButtonProxy->setWidget(&m_importButton);
    exportButtonProxy->setWidget(&m_exportButton);
    trainingButtonProxy->setWidget(&m_trainingButton);
    testButtonProxy->setWidget(&m_validationButton);
    startTrainingButtonProxy->setWidget(&m_startTrainingButton);
    nextButtonProxy->setWidget(&m_nextButton);
    backButtonProxy->setWidget(&m_backButton);

    errorsLabelProxy->setWidget(&m_errorsLabel);
    resultsLabelProxy->setWidget(&m_resultLabel);

    annGraphicsProxy->setWidget(&m_annGraphics);

    addItem(&m_draggedImage);

    addItem(newButtonProxy);
    addItem(importButtonProxy);
    addItem(exportButtonProxy);
    addItem(trainingButtonProxy);
    addItem(testButtonProxy);
    addItem(startTrainingButtonProxy);
    addItem(backButtonProxy);
    addItem(nextButtonProxy);

    addItem(errorsLabelProxy);
    addItem(resultsLabelProxy);

    addItem(annGraphicsProxy);

    QStateMachine *machine = new QStateMachine;
    QState *stateStart = new QState(machine);
    QState *stateSets = new QState(machine);
    QState *stateTesting = new QState(machine);
    QState *stateMain = new QState(machine);
    machine->setInitialState(stateStart);
    connect(this, &GraphicsScene::goToNextState, this, &GraphicsScene::nextState);
    connect(this, &GraphicsScene::goToPreviousState, this, &GraphicsScene::previousState);

    qreal width = this->width();
    qreal height = this->height();

    // State Start
    // bottom buttons
    stateStart->assignProperty(backButtonProxy, "opacity", qreal(0));
    stateStart->assignProperty(nextButtonProxy, "opacity", qreal(0));
    stateStart->assignProperty(startTrainingButtonProxy, "opacity", qreal(0));
    stateStart->assignProperty(backButtonProxy, "pos", QPointF(BORDER, height - m_backButton.height() - BORDER));
    stateStart->assignProperty(nextButtonProxy, "pos", QPointF(2 * width + width - m_nextButton.width() - BORDER, height - m_nextButton.height() - BORDER));
    stateStart->assignProperty(startTrainingButtonProxy, "pos", QPointF(width - m_startTrainingButton.width() - BORDER, height - m_startTrainingButton.height() - BORDER));

    stateStart->assignProperty(newButtonProxy, "pos", QPointF(width / 2 - m_newButton.width() / 2, height * 0.4));
    stateStart->assignProperty(importButtonProxy, "pos", QPointF(width / 2 - m_importButton.width() / 2, height * 0.6));
    stateStart->assignProperty(trainingButtonProxy, "pos", QPointF(width + width / 2 - m_trainingButton.width() / 2, height * 0.4));
    stateStart->assignProperty(testButtonProxy, "pos", QPointF(width + width / 2 - m_validationButton.width() / 2, height / 2));

    stateStart->assignProperty(annGraphicsProxy, "pos", QPointF(2 * width + width / 2 - m_annGraphics.width() / 2, height / 2 - m_annGraphics.height() / 2));
    stateStart->assignProperty(errorsLabelProxy, "pos", QPointF(2 * width + width / 2 - m_errorsLabel.width() / 2, height / 2 + m_annGraphics.height() / 2 + 10));

    qreal topBotSpace = height / 2 - m_draggedImage.height() / 2;

    stateStart->assignProperty(exportButtonProxy, "pos", QPointF(3 * width + width / 2 - m_exportButton.width() / 2, topBotSpace / 2 - m_exportButton.height() / 2));
    stateStart->assignProperty(&m_draggedImage, "pos", QPointF(3 * width + width / 2 - m_draggedImage.width() / 2, height / 2 - m_draggedImage.height() / 2));
    stateStart->assignProperty(resultsLabelProxy, "pos", QPointF(3 * width + width / 2 - m_resultLabel.width() / 2, height - topBotSpace / 2 - m_resultLabel.height() / 2));

    stateStart->assignProperty(&m_bg1, "opacity", qreal(1));
    stateStart->assignProperty(&m_bg2, "opacity", qreal(0));
    stateStart->assignProperty(&m_bg3, "opacity", qreal(0));
    stateStart->assignProperty(&m_bg4, "opacity", qreal(0));

    // State Sets
    // bottom buttons
    stateSets->assignProperty(backButtonProxy, "opacity", qreal(1));
    stateSets->assignProperty(nextButtonProxy, "opacity", qreal(0));
    stateSets->assignProperty(startTrainingButtonProxy, "opacity", qreal(1));
    stateSets->assignProperty(nextButtonProxy, "pos", QPointF(width + width - m_nextButton.width() - BORDER, height - m_nextButton.height() - BORDER));

    stateSets->assignProperty(newButtonProxy, "pos", QPointF(-(width + width / 2 - m_newButton.width() / 2), height * 0.4));
    stateSets->assignProperty(importButtonProxy, "pos", QPointF(-(width + width / 2 - m_importButton.width() / 2), height * 0.6));
    stateSets->assignProperty(trainingButtonProxy, "pos", QPointF(width /2 - m_trainingButton.width() / 2, height * 0.4));
    stateSets->assignProperty(testButtonProxy, "pos", QPointF(width / 2 - m_validationButton.width() / 2, height * 0.6));

    stateSets->assignProperty(annGraphicsProxy, "pos", QPointF(width + width / 2 - m_annGraphics.width() / 2, height / 2 - m_annGraphics.height() / 2));
    stateSets->assignProperty(errorsLabelProxy, "pos", QPointF(width + width / 2 - m_errorsLabel.width() / 2, height / 2 + m_annGraphics.height() / 2 + 10));

    stateSets->assignProperty(exportButtonProxy, "pos", QPointF(2 * width + width / 2 - m_exportButton.width() / 2, topBotSpace / 2 - m_exportButton.height() / 2));
    stateSets->assignProperty(&m_draggedImage, "pos", QPointF(2 * width + width / 2 - m_draggedImage.width() / 2, height / 2 - m_draggedImage.height() / 2));
    stateSets->assignProperty(resultsLabelProxy, "pos", QPointF(2 * width + width / 2 - m_resultLabel.width() / 2, height - topBotSpace / 2 - m_resultLabel.height() / 2));

    stateSets->assignProperty(&m_bg1, "opacity", qreal(0));
    stateSets->assignProperty(&m_bg2, "opacity", qreal(1));
    stateSets->assignProperty(&m_bg3, "opacity", qreal(0));
    stateSets->assignProperty(&m_bg4, "opacity", qreal(0));

    // State Testing
    // bottom buttons
    stateTesting->assignProperty(backButtonProxy, "opacity", qreal(1));
    stateTesting->assignProperty(nextButtonProxy, "opacity", qreal(1));
    stateTesting->assignProperty(startTrainingButtonProxy, "opacity", qreal(0));
    stateTesting->assignProperty(nextButtonProxy, "pos", QPointF(width - m_nextButton.width() - BORDER, height - m_nextButton.height() - BORDER));

    stateTesting->assignProperty(newButtonProxy, "pos", QPointF(-(3 * width + width / 2 - m_newButton.width() / 2), height * 0.4));
    stateTesting->assignProperty(importButtonProxy, "pos", QPointF(-(3 * width + width / 2 - m_importButton.width() / 2), height * 0.6));
    stateTesting->assignProperty(trainingButtonProxy, "pos", QPointF(-(2 * width + width / 2 - m_trainingButton.width() / 2), height / 2));
    stateTesting->assignProperty(testButtonProxy, "pos", QPointF(-(2 * width + width / 2 - m_validationButton.width() / 2), height / 2));

    stateTesting->assignProperty(annGraphicsProxy, "pos", QPointF(width / 2 - m_annGraphics.width() / 2, height / 2 - m_annGraphics.height() / 2));
    stateTesting->assignProperty(errorsLabelProxy, "pos", QPointF(width / 2 - m_errorsLabel.width() / 2, height / 2 + m_annGraphics.height() / 2 + 10));

    stateTesting->assignProperty(exportButtonProxy, "pos", QPointF(width + width / 2 - m_exportButton.width() / 2, topBotSpace / 2 - m_exportButton.height() / 2));
    stateTesting->assignProperty(&m_draggedImage, "pos", QPointF(width + width / 2 - m_draggedImage.width() / 2, height / 2 - m_draggedImage.height() / 2));
    stateTesting->assignProperty(resultsLabelProxy, "pos", QPointF(width + width / 2 - m_resultLabel.width() / 2, height - topBotSpace / 2 - m_resultLabel.height() / 2));

    stateTesting->assignProperty(&m_bg1, "opacity", qreal(0));
    stateTesting->assignProperty(&m_bg2, "opacity", qreal(0));
    stateTesting->assignProperty(&m_bg3, "opacity", qreal(1));
    stateTesting->assignProperty(&m_bg4, "opacity", qreal(0));

    // State Main
    // bottom buttons
    stateMain->assignProperty(backButtonProxy, "opacity", qreal(1));
    stateMain->assignProperty(nextButtonProxy, "opacity", qreal(0));
    stateMain->assignProperty(startTrainingButtonProxy, "opacity", qreal(0));

    stateMain->assignProperty(nextButtonProxy, "pos", QPointF(-width + width - m_nextButton.width() - BORDER, height - m_nextButton.height() - BORDER));

    stateMain->assignProperty(newButtonProxy, "pos", QPointF(-(4 * width + width / 2 - m_newButton.width() / 2), height * 0.4));
    stateMain->assignProperty(importButtonProxy, "pos", QPointF(-(4 * width + width / 2 - m_importButton.width() / 2), height * 0.6));
    stateMain->assignProperty(trainingButtonProxy, "pos", QPointF(-(3 * width + width / 2 - m_trainingButton.width() / 2), height / 2));
    stateMain->assignProperty(testButtonProxy, "pos", QPointF(-(3 * width + width / 2 - m_validationButton.width() / 2), height / 2));

    stateMain->assignProperty(annGraphicsProxy, "pos", QPointF(-(width + width / 2 - m_annGraphics.width() / 2), height / 2 - m_annGraphics.height() / 2));
    stateMain->assignProperty(errorsLabelProxy, "pos", QPointF(-(width + width / 2 - m_errorsLabel.width() / 2), height / 2 + m_annGraphics.height() / 2 + 10));

    stateMain->assignProperty(exportButtonProxy, "pos", QPointF(width / 2 - m_exportButton.width() / 2, topBotSpace / 2 - m_exportButton.height() / 2));
    stateMain->assignProperty(&m_draggedImage, "pos", QPointF(width / 2 - m_draggedImage.width() / 2, height / 2 - m_draggedImage.height() / 2));
    stateMain->assignProperty(resultsLabelProxy, "pos", QPointF(width / 2 - m_resultLabel.width() / 2, height - topBotSpace / 2 - m_resultLabel.height() / 2));

    stateMain->assignProperty(&m_bg1, "opacity", qreal(0));
    stateMain->assignProperty(&m_bg2, "opacity", qreal(0));
    stateMain->assignProperty(&m_bg3, "opacity", qreal(0));
    stateMain->assignProperty(&m_bg4, "opacity", qreal(1));


    QAbstractTransition *t1 = stateStart->addTransition(this, SIGNAL(goToNextState()), stateSets);
    t1->addAnimation(new QPropertyAnimation(&m_bg1, "opacity"));
    t1->addAnimation(new QPropertyAnimation(&m_bg2, "opacity"));
    t1->addAnimation(new QPropertyAnimation(&m_bg3, "opacity"));
    t1->addAnimation(new QPropertyAnimation(&m_bg4, "opacity"));

    t1->addAnimation(new QPropertyAnimation(backButtonProxy, "opacity"));
    t1->addAnimation(new QPropertyAnimation(nextButtonProxy, "opacity"));
    t1->addAnimation(new QPropertyAnimation(startTrainingButtonProxy, "opacity"));

    t1->addAnimation(new QPropertyAnimation(backButtonProxy, "pos"));
    t1->addAnimation(new QPropertyAnimation(nextButtonProxy, "pos"));
    t1->addAnimation(new QPropertyAnimation(startTrainingButtonProxy, "pos"));
    t1->addAnimation(new QPropertyAnimation(newButtonProxy, "pos"));
    t1->addAnimation(new QPropertyAnimation(importButtonProxy, "pos"));
    t1->addAnimation(new QPropertyAnimation(trainingButtonProxy, "pos"));
    t1->addAnimation(new QPropertyAnimation(testButtonProxy, "pos"));

    t1->addAnimation(new QPropertyAnimation(annGraphicsProxy, "pos"));

    t1->addAnimation(new QPropertyAnimation(errorsLabelProxy, "pos"));

    t1->addAnimation(new QPropertyAnimation(exportButtonProxy, "pos"));
    t1->addAnimation(new QPropertyAnimation(&m_draggedImage, "pos"));
    t1->addAnimation(new QPropertyAnimation(resultsLabelProxy, "pos"));

    QAbstractTransition *t2 = stateSets->addTransition(this, SIGNAL(goToPreviousState()), stateStart);
    t2->addAnimation(new QPropertyAnimation(&m_bg1, "opacity"));
    t2->addAnimation(new QPropertyAnimation(&m_bg2, "opacity"));
    t2->addAnimation(new QPropertyAnimation(&m_bg3, "opacity"));
    t2->addAnimation(new QPropertyAnimation(&m_bg4, "opacity"));

    t2->addAnimation(new QPropertyAnimation(backButtonProxy, "opacity"));
    t2->addAnimation(new QPropertyAnimation(nextButtonProxy, "opacity"));
    t2->addAnimation(new QPropertyAnimation(startTrainingButtonProxy, "opacity"));

    t2->addAnimation(new QPropertyAnimation(backButtonProxy, "pos"));
    t2->addAnimation(new QPropertyAnimation(nextButtonProxy, "pos"));
    t2->addAnimation(new QPropertyAnimation(startTrainingButtonProxy, "pos"));
    t2->addAnimation(new QPropertyAnimation(newButtonProxy, "pos"));
    t2->addAnimation(new QPropertyAnimation(importButtonProxy, "pos"));
    t2->addAnimation(new QPropertyAnimation(trainingButtonProxy, "pos"));
    t2->addAnimation(new QPropertyAnimation(testButtonProxy, "pos"));

    t2->addAnimation(new QPropertyAnimation(annGraphicsProxy, "pos"));

    t2->addAnimation(new QPropertyAnimation(errorsLabelProxy, "pos"));

    t2->addAnimation(new QPropertyAnimation(exportButtonProxy, "pos"));
    t2->addAnimation(new QPropertyAnimation(&m_draggedImage, "pos"));
    t2->addAnimation(new QPropertyAnimation(resultsLabelProxy, "pos"));

    QAbstractTransition *t3 = stateSets->addTransition(this, SIGNAL(goToNextState()), stateTesting);
    t3->addAnimation(new QPropertyAnimation(&m_bg1, "opacity"));
    t3->addAnimation(new QPropertyAnimation(&m_bg2, "opacity"));
    t3->addAnimation(new QPropertyAnimation(&m_bg3, "opacity"));
    t3->addAnimation(new QPropertyAnimation(&m_bg4, "opacity"));

    t3->addAnimation(new QPropertyAnimation(backButtonProxy, "opacity"));
    t3->addAnimation(new QPropertyAnimation(nextButtonProxy, "opacity"));
    t3->addAnimation(new QPropertyAnimation(startTrainingButtonProxy, "opacity"));

    t3->addAnimation(new QPropertyAnimation(backButtonProxy, "pos"));
    t3->addAnimation(new QPropertyAnimation(nextButtonProxy, "pos"));
    t3->addAnimation(new QPropertyAnimation(startTrainingButtonProxy, "pos"));
    t3->addAnimation(new QPropertyAnimation(newButtonProxy, "pos"));
    t3->addAnimation(new QPropertyAnimation(importButtonProxy, "pos"));
    t3->addAnimation(new QPropertyAnimation(trainingButtonProxy, "pos"));
    t3->addAnimation(new QPropertyAnimation(testButtonProxy, "pos"));

    t3->addAnimation(new QPropertyAnimation(annGraphicsProxy, "pos"));

    t3->addAnimation(new QPropertyAnimation(errorsLabelProxy, "pos"));

    t3->addAnimation(new QPropertyAnimation(exportButtonProxy, "pos"));
    t3->addAnimation(new QPropertyAnimation(&m_draggedImage, "pos"));
    t3->addAnimation(new QPropertyAnimation(resultsLabelProxy, "pos"));

    QAbstractTransition *t6 = stateTesting->addTransition(this, SIGNAL(goToPreviousState()), stateSets);
    t6->addAnimation(new QPropertyAnimation(&m_bg1, "opacity"));
    t6->addAnimation(new QPropertyAnimation(&m_bg2, "opacity"));
    t6->addAnimation(new QPropertyAnimation(&m_bg3, "opacity"));
    t6->addAnimation(new QPropertyAnimation(&m_bg4, "opacity"));

    t6->addAnimation(new QPropertyAnimation(backButtonProxy, "opacity"));
    t6->addAnimation(new QPropertyAnimation(nextButtonProxy, "opacity"));
    t6->addAnimation(new QPropertyAnimation(startTrainingButtonProxy, "opacity"));

    t6->addAnimation(new QPropertyAnimation(backButtonProxy, "pos"));
    t6->addAnimation(new QPropertyAnimation(nextButtonProxy, "pos"));
    t6->addAnimation(new QPropertyAnimation(startTrainingButtonProxy, "pos"));
    t6->addAnimation(new QPropertyAnimation(newButtonProxy, "pos"));
    t6->addAnimation(new QPropertyAnimation(importButtonProxy, "pos"));
    t6->addAnimation(new QPropertyAnimation(trainingButtonProxy, "pos"));
    t6->addAnimation(new QPropertyAnimation(testButtonProxy, "pos"));

    t6->addAnimation(new QPropertyAnimation(annGraphicsProxy, "pos"));

    t6->addAnimation(new QPropertyAnimation(errorsLabelProxy, "pos"));

    t6->addAnimation(new QPropertyAnimation(exportButtonProxy, "pos"));
    t6->addAnimation(new QPropertyAnimation(&m_draggedImage, "pos"));
    t6->addAnimation(new QPropertyAnimation(resultsLabelProxy, "pos"));

    QAbstractTransition *t7 = stateTesting->addTransition(this, SIGNAL(goToNextState()), stateMain);
    t7->addAnimation(new QPropertyAnimation(&m_bg1, "opacity"));
    t7->addAnimation(new QPropertyAnimation(&m_bg2, "opacity"));
    t7->addAnimation(new QPropertyAnimation(&m_bg3, "opacity"));
    t7->addAnimation(new QPropertyAnimation(&m_bg4, "opacity"));

    t7->addAnimation(new QPropertyAnimation(backButtonProxy, "opacity"));
    t7->addAnimation(new QPropertyAnimation(nextButtonProxy, "opacity"));
    t7->addAnimation(new QPropertyAnimation(startTrainingButtonProxy, "opacity"));

    t7->addAnimation(new QPropertyAnimation(backButtonProxy, "pos"));
    t7->addAnimation(new QPropertyAnimation(nextButtonProxy, "pos"));
    t7->addAnimation(new QPropertyAnimation(startTrainingButtonProxy, "pos"));
    t7->addAnimation(new QPropertyAnimation(newButtonProxy, "pos"));
    t7->addAnimation(new QPropertyAnimation(importButtonProxy, "pos"));
    t7->addAnimation(new QPropertyAnimation(trainingButtonProxy, "pos"));
    t7->addAnimation(new QPropertyAnimation(testButtonProxy, "pos"));

    t7->addAnimation(new QPropertyAnimation(annGraphicsProxy, "pos"));

    t7->addAnimation(new QPropertyAnimation(errorsLabelProxy, "pos"));

    t7->addAnimation(new QPropertyAnimation(exportButtonProxy, "pos"));
    t7->addAnimation(new QPropertyAnimation(&m_draggedImage, "pos"));
    t7->addAnimation(new QPropertyAnimation(resultsLabelProxy, "pos"));

    QAbstractTransition *t8 = stateMain->addTransition(this, SIGNAL(goToPreviousState()), stateTesting);
    t8->addAnimation(new QPropertyAnimation(&m_bg1, "opacity"));
    t8->addAnimation(new QPropertyAnimation(&m_bg2, "opacity"));
    t8->addAnimation(new QPropertyAnimation(&m_bg3, "opacity"));
    t8->addAnimation(new QPropertyAnimation(&m_bg4, "opacity"));

    t8->addAnimation(new QPropertyAnimation(backButtonProxy, "opacity"));
    t8->addAnimation(new QPropertyAnimation(nextButtonProxy, "opacity"));
    t8->addAnimation(new QPropertyAnimation(startTrainingButtonProxy, "opacity"));

    t8->addAnimation(new QPropertyAnimation(backButtonProxy, "pos"));
    t8->addAnimation(new QPropertyAnimation(nextButtonProxy, "pos"));
    t8->addAnimation(new QPropertyAnimation(startTrainingButtonProxy, "pos"));
    t8->addAnimation(new QPropertyAnimation(newButtonProxy, "pos"));
    t8->addAnimation(new QPropertyAnimation(importButtonProxy, "pos"));
    t8->addAnimation(new QPropertyAnimation(trainingButtonProxy, "pos"));
    t8->addAnimation(new QPropertyAnimation(testButtonProxy, "pos"));

    t8->addAnimation(new QPropertyAnimation(annGraphicsProxy, "pos"));

    t8->addAnimation(new QPropertyAnimation(errorsLabelProxy, "pos"));

    t8->addAnimation(new QPropertyAnimation(exportButtonProxy, "pos"));
    t8->addAnimation(new QPropertyAnimation(&m_draggedImage, "pos"));
    t8->addAnimation(new QPropertyAnimation(resultsLabelProxy, "pos"));

    machine->start();

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &GraphicsScene::checkStatus);
    m_timer->start();
}

void GraphicsScene::checkStatus()
{
    m_mutex.lock();
    if(!m_futurePoints.isEmpty())
    {
        std::vector<QPointF> points = m_futurePoints.dequeue();
        m_annGraphics.addPoint(points);
        m_errorsLabel.setText(GraphicsScene::formatErrorsLabel(points));
    }
    // Enable next button if training is finished
    if(m_finished)
    {
        m_nextButton.setVisible(m_finished);
        m_timer->stop();
    }
    m_mutex.unlock();
}

void GraphicsScene::addPoint(const std::vector<QPointF>& points)
{
    m_mutex.lock();
    m_futurePoints.enqueue(points);
    m_mutex.unlock();
}

QString GraphicsScene::formatErrorsLabel(const std::vector<QPointF>& points)
{
    if(points.size() > 1)
        return tr("Iteration: %1\nTraining Set Error: %2\nValidation Set Error: %3").arg(QString::number(points[0].x()), QString::number(points[0].y(), 'g', 5), QString::number(points[1].y(), 'g', 5));
    else
        return tr("Iteration: %1\nTraining Set Error: %2").arg(QString::number(points[0].x()), QString::number(points[0].y(), 'g', 5));
}

bool GraphicsScene::isDropAllow()
{
    return m_currentState == MENU_MENU;
}

void GraphicsScene::dropData(const std::vector<std::string>& filepaths)
{
    std::vector<double> results = m_ippController->feed(filepaths);
    static std::vector<std::vector<double>> pictures = PictureController::loadPictures(filepaths);
    QImage image = PictureController::create(pictures.back(), 32);
    QPixmap pixmap = QPixmap::fromImage(image).scaled(IMAGE_WIDHT, IMAGE_HEIGHT, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    m_draggedImage.pixmap(pixmap);
    m_draggedImage.update();
    if(results.back() < 0.5)
        m_resultLabel.setText(tr("Open\nWith error: %1").arg(QString::number(fabs(results.back() - 0.1), 'g', 5)));
    else
        m_resultLabel.setText(tr("Sunglasses\nWith error: %1").arg(QString::number(fabs(0.9 - results.back()), 'g', 5)));
}

void GraphicsScene::createANN()
{
    QDialog dialog(0, Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    dialog.setWindowTitle(tr("New artificial neural network"));
    QFormLayout form(&dialog);

    // Add some text above the fields
    form.addRow(new QLabel(tr("Config your artificial neural network")));

    QLineEdit hiddenLayerLineEdit(&dialog);
    hiddenLayerLineEdit.setText("30;20;10");
    QString hiddenLayerLabel = tr("Hidden layer (30;20;10)");
    form.addRow(hiddenLayerLabel, &hiddenLayerLineEdit);

    QDoubleSpinBox learningRateSpinBox(&dialog);
    learningRateSpinBox.setValue(0.5D);
    learningRateSpinBox.setMinimum(0.001D);
    learningRateSpinBox.setMaximum(1.0D);
    learningRateSpinBox.setSingleStep(learningRateSpinBox.minimum());
    learningRateSpinBox.setDecimals(3);
    QString learningRateLabel = tr("Learning rate");
    form.addRow(learningRateLabel, &learningRateSpinBox);

    QDoubleSpinBox momentumSpinBox(&dialog);
    momentumSpinBox.setValue(0.5D);
    momentumSpinBox.setMinimum(0.001D);
    momentumSpinBox.setMaximum(1.0D);
    momentumSpinBox.setSingleStep(momentumSpinBox.minimum());
    momentumSpinBox.setDecimals(3);
    QString momentumLabel = tr("Momentum");
    form.addRow(momentumLabel, &momentumSpinBox);

    QDoubleSpinBox errorSpinBox(&dialog);
    errorSpinBox.setValue(0.0001D);
    errorSpinBox.setMinimum(0.00001D);
    errorSpinBox.setSingleStep(errorSpinBox.minimum());
    errorSpinBox.setDecimals(5);
    QString errorLabel = tr("Error");
    form.addRow(errorLabel, &errorSpinBox);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted)
    {
        std::vector<int> nbNeuronsPerHiddenLayer;
        for(QString nbNeurons : hiddenLayerLineEdit.text().split(";"))
        {
            int neurons = nbNeurons.toInt();
            if (neurons == 0)
                neurons++;
            nbNeuronsPerHiddenLayer.push_back(neurons);
        }
        m_ippController->configANN(nbNeuronsPerHiddenLayer, learningRateSpinBox.value(), momentumSpinBox.value(), errorSpinBox.value());
        emit goToNextState();
    }
}

void GraphicsScene::importANN()
{
    QString fileName = QFileDialog::getOpenFileName(&m_importButton,
                                                    tr("Open Existing ANN"),
                                                    "",
                                                    tr(QString("ANN (*.").append(SettingsNeuralNetwork::extension.c_str()).append(")").toStdString().c_str()));

    if(!fileName.isNull())
    {
        m_ippController->importANN(fileName);
        emit goToNextState();
        emit goToNextState();
        emit goToNextState();
    }
}

void GraphicsScene::exportANN()
{
    QString fileName = QFileDialog::getSaveFileName(&m_importButton,
                                                    tr("Save ANN"),
                                                    "",
                                                    tr(QString("ANN (*.").append(SettingsNeuralNetwork::extension.c_str()).append(")").toStdString().c_str()));

    if(!fileName.isNull())
    {
        m_ippController->exportANN(fileName);
    }
}

void GraphicsScene::selectTrainingSet()
{
    QString fileName = QFileDialog::getOpenFileName(&m_trainingButton,
                                                    tr("Open Training Set"),
                                                    "",
                                                    tr(QString("Training Set (*)").toStdString().c_str()));
    if(!fileName.isNull())
    {
        m_annGraphics.addCurve(std::make_tuple("Training Set Error", QPen(Qt::blue, 3)));
        m_ippController->setTrainingSetPath(fileName);
        m_startTrainingButton.setVisible(true);
    }
}

void GraphicsScene::selectValidationSet()
{
    QString fileName = QFileDialog::getOpenFileName(&m_validationButton,
                                                    tr("Open Validation Set"),
                                                    "",
                                                    tr(QString("Validation Set (*)").toStdString().c_str()));
    if(!fileName.isNull())
    {
        m_annGraphics.addCurve(std::make_tuple("Validation Set Error", QPen(Qt::red, 3)));
        m_ippController->setValidationSetPath(fileName);
    }
}

void GraphicsScene::nextState()
{
    m_currentState++;
    stateChange();
}

void GraphicsScene::previousState()
{
    m_currentState--;
    stateChange();
}

void GraphicsScene::backToStartMenu()
{
    while(m_currentState != START_MENU)
    {
        emit goToPreviousState();
    }
}

void GraphicsScene::stateChange()
{
    switch (m_currentState) {
    case START_MENU: // Start menu
        break;
    case SELECT_SETS_MENU: // Select sets menu
        m_startTrainingButton.setVisible(false);
        break;
    case TRAINING_VIEW: // Training view (with graph)
        m_nextButton.setVisible(false);
        break;
    case MENU_MENU: // Main menu
        break;
    default:
        break;
    }
}

void GraphicsScene::startTraining()
{
    m_ippController->startTraining();
    emit goToNextState();
}
