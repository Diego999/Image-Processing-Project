#include "include/GUI/graphicsscene.h"
#include "include/GUI/pixmap.h"

#include <qwt_symbol.h>

#include <tuple>
#include <iostream>
#include <QMutex>


GraphicsScene::GraphicsScene(qreal width, qreal height) : QGraphicsScene(0, 0, width, height)
{
    setBackgroundBrush(this->palette().window());
}

void GraphicsScene::createUI()
{
    /**
      http://qt-project.org/doc/qt-5/statemachine-api.html
      **/
    // Text edit and button
    QPushButton *newButton = new QPushButton(tr("New"));
    QPushButton *importButton = new QPushButton(tr("Import..."));
    QPushButton *trainingButton = new QPushButton(tr("Train"));
    QPushButton *testButton = new QPushButton(tr("Test"));
    QPushButton *nextButton = new QPushButton("N");
    QPushButton *backButton = new QPushButton("B");

    std::vector<std::tuple<std::string, QPen>> curves;
    curves.push_back(std::make_tuple("Training Set Error", QPen(Qt::blue, 3)));
    curves.push_back(std::make_tuple("Validation Set Error", QPen(Qt::red, 3)));
    annGraphics =  new ANNGraphics(curves, "Number of iterations", "Error");

    QGraphicsProxyWidget *newButtonProxy = new QGraphicsProxyWidget;
    QGraphicsProxyWidget *importButtonProxy = new QGraphicsProxyWidget;
    QGraphicsProxyWidget *trainingButtonProxy = new QGraphicsProxyWidget;
    QGraphicsProxyWidget *testButtonProxy = new QGraphicsProxyWidget;
    QGraphicsProxyWidget *nextButtonProxy = new QGraphicsProxyWidget;
    QGraphicsProxyWidget *backButtonProxy = new QGraphicsProxyWidget;

    QGraphicsProxyWidget *annGraphicsProxy = new QGraphicsProxyWidget;

    newButtonProxy->setWidget(newButton);
    importButtonProxy->setWidget(importButton);
    trainingButtonProxy->setWidget(trainingButton);
    testButtonProxy->setWidget(testButton);
    nextButtonProxy->setWidget(nextButton);
    backButtonProxy->setWidget(backButton);

    annGraphicsProxy->setWidget(annGraphics);

    addItem(newButtonProxy);
    addItem(importButtonProxy);
    addItem(trainingButtonProxy);
    addItem(testButtonProxy);
    addItem(nextButtonProxy);
    addItem(backButtonProxy);

    addItem(annGraphicsProxy);

    QStateMachine *machine = new QStateMachine;
    QState *stateStart = new QState(machine);
    QState *stateTraining = new QState(machine);
    QState *stateTest = new QState(machine);
    QState *stateMain = new QState(machine);
    machine->setInitialState(stateStart);

    qreal width = this->width();
    qreal height = this->height();

    // State Start
    // bottom buttons
    stateStart->assignProperty(backButtonProxy, "opacity", qreal(0));
    stateStart->assignProperty(nextButtonProxy, "opacity", qreal(1));
    stateStart->assignProperty(backButtonProxy, "pos", QPointF(5, height - backButton->height() - 5));
    stateStart->assignProperty(nextButtonProxy, "pos", QPointF(width - nextButton->width() - 5, height - nextButton->height() - 5));

    stateStart->assignProperty(newButtonProxy, "pos", QPointF(width / 2, height / 4));
    stateStart->assignProperty(importButtonProxy, "pos", QPointF(width / 2, height * (3/4)));
    stateStart->assignProperty(trainingButtonProxy, "pos", QPointF(width + width /2, height / 2));
    stateStart->assignProperty(testButtonProxy, "pos", QPointF(2 * width + width / 2, height / 2));

    // State Training
    // bottom buttons
    stateTraining->assignProperty(backButtonProxy, "opacity", qreal(1));
    stateTraining->assignProperty(nextButtonProxy, "opacity", qreal(1));

    stateTraining->assignProperty(newButtonProxy, "pos", QPointF(-(width + width / 2), height / 4));
    stateTraining->assignProperty(importButtonProxy, "pos", QPointF(-(width + width / 2), height * (3/4)));
    stateTraining->assignProperty(trainingButtonProxy, "pos", QPointF(width /2, height / 2));
    stateTraining->assignProperty(testButtonProxy, "pos", QPointF(width + width / 2, height / 2));

    // State Test
    // bottom buttons
    stateTest->assignProperty(backButtonProxy, "opacity", qreal(1));
    stateTest->assignProperty(nextButtonProxy, "opacity", qreal(1));

    stateTest->assignProperty(newButtonProxy, "pos", QPointF(-(2 * width + width / 2), height / 4));
    stateTest->assignProperty(importButtonProxy, "pos", QPointF(-(2 * width + width / 2), height * (3/4)));
    stateTest->assignProperty(trainingButtonProxy, "pos", QPointF(-(width + width / 2), height / 2));
    stateTest->assignProperty(testButtonProxy, "pos", QPointF(width / 2, height / 2));

    // State Main
    // bottom buttons
    stateMain->assignProperty(backButtonProxy, "opacity", qreal(1));
    stateMain->assignProperty(nextButtonProxy, "opacity", qreal(0));

    stateMain->assignProperty(newButtonProxy, "pos", QPointF(width / 2, height / 4));
    stateMain->assignProperty(importButtonProxy, "pos", QPointF(width / 2, height * (3/4)));
    stateMain->assignProperty(trainingButtonProxy, "pos", QPointF(width + width /2, height / 2));
    stateMain->assignProperty(testButtonProxy, "pos", QPointF(2 * width + width / 2, height / 2));

    QAbstractTransition *t1 = stateStart->addTransition(nextButton, SIGNAL(clicked()), stateTraining);
    t1->addAnimation(new QPropertyAnimation(backButtonProxy, "opacity"));
    t1->addAnimation(new QPropertyAnimation(nextButtonProxy, "opacity"));

    t1->addAnimation(new QPropertyAnimation(backButtonProxy, "pos"));
    t1->addAnimation(new QPropertyAnimation(nextButtonProxy, "pos"));
    t1->addAnimation(new QPropertyAnimation(newButtonProxy, "pos"));
    t1->addAnimation(new QPropertyAnimation(importButtonProxy, "pos"));
    t1->addAnimation(new QPropertyAnimation(trainingButtonProxy, "pos"));
    t1->addAnimation(new QPropertyAnimation(testButtonProxy, "pos"));

    QAbstractTransition *t2 = stateTraining->addTransition(backButton, SIGNAL(clicked()), stateStart);
    t2->addAnimation(new QPropertyAnimation(backButtonProxy, "opacity"));
    t2->addAnimation(new QPropertyAnimation(nextButtonProxy, "opacity"));

    t2->addAnimation(new QPropertyAnimation(backButtonProxy, "pos"));
    t2->addAnimation(new QPropertyAnimation(nextButtonProxy, "pos"));
    t2->addAnimation(new QPropertyAnimation(newButtonProxy, "pos"));
    t2->addAnimation(new QPropertyAnimation(importButtonProxy, "pos"));
    t2->addAnimation(new QPropertyAnimation(trainingButtonProxy, "pos"));
    t2->addAnimation(new QPropertyAnimation(testButtonProxy, "pos"));

    QAbstractTransition *t3 = stateTraining->addTransition(nextButton, SIGNAL(clicked()), stateTest);
    t3->addAnimation(new QPropertyAnimation(backButtonProxy, "opacity"));
    t3->addAnimation(new QPropertyAnimation(nextButtonProxy, "opacity"));

    t3->addAnimation(new QPropertyAnimation(backButtonProxy, "pos"));
    t3->addAnimation(new QPropertyAnimation(nextButtonProxy, "pos"));
    t3->addAnimation(new QPropertyAnimation(newButtonProxy, "pos"));
    t3->addAnimation(new QPropertyAnimation(importButtonProxy, "pos"));
    t3->addAnimation(new QPropertyAnimation(trainingButtonProxy, "pos"));
    t3->addAnimation(new QPropertyAnimation(testButtonProxy, "pos"));

    QAbstractTransition *t4 = stateTest->addTransition(backButton, SIGNAL(clicked()), stateTraining);
    t4->addAnimation(new QPropertyAnimation(backButtonProxy, "opacity"));
    t4->addAnimation(new QPropertyAnimation(nextButtonProxy, "opacity"));

    t4->addAnimation(new QPropertyAnimation(backButtonProxy, "pos"));
    t4->addAnimation(new QPropertyAnimation(nextButtonProxy, "pos"));
    t4->addAnimation(new QPropertyAnimation(newButtonProxy, "pos"));
    t4->addAnimation(new QPropertyAnimation(importButtonProxy, "pos"));
    t4->addAnimation(new QPropertyAnimation(trainingButtonProxy, "pos"));
    t4->addAnimation(new QPropertyAnimation(testButtonProxy, "pos"));

    QAbstractTransition *t5 = stateTest->addTransition(nextButton, SIGNAL(clicked()), stateMain);
    t5->addAnimation(new QPropertyAnimation(backButtonProxy, "opacity"));
    t5->addAnimation(new QPropertyAnimation(nextButtonProxy, "opacity"));

    t5->addAnimation(new QPropertyAnimation(backButtonProxy, "pos"));
    t5->addAnimation(new QPropertyAnimation(nextButtonProxy, "pos"));
    t5->addAnimation(new QPropertyAnimation(newButtonProxy, "pos"));
    t5->addAnimation(new QPropertyAnimation(importButtonProxy, "pos"));
    t5->addAnimation(new QPropertyAnimation(trainingButtonProxy, "pos"));
    t5->addAnimation(new QPropertyAnimation(testButtonProxy, "pos"));

    QAbstractTransition *t6 = stateMain->addTransition(backButton, SIGNAL(clicked()), stateTest);
    t6->addAnimation(new QPropertyAnimation(backButtonProxy, "opacity"));
    t6->addAnimation(new QPropertyAnimation(nextButtonProxy, "opacity"));

    t6->addAnimation(new QPropertyAnimation(backButtonProxy, "pos"));
    t6->addAnimation(new QPropertyAnimation(nextButtonProxy, "pos"));
    t6->addAnimation(new QPropertyAnimation(newButtonProxy, "pos"));
    t6->addAnimation(new QPropertyAnimation(importButtonProxy, "pos"));
    t6->addAnimation(new QPropertyAnimation(trainingButtonProxy, "pos"));
    t6->addAnimation(new QPropertyAnimation(testButtonProxy, "pos"));

    machine->start();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePoints()));
    timer->start();
}

void GraphicsScene::updatePoints()
{
    mutex.lock();
    if(!futurePoints.isEmpty())
        annGraphics->addPoint(futurePoints.dequeue());
    mutex.unlock();
}

void GraphicsScene::addPoint(const std::vector<QPointF>& point)
{
    mutex.lock();
    futurePoints.enqueue(point);
    mutex.unlock();
}
