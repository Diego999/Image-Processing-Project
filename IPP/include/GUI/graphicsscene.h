#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include "include/GUI/anngraphics.h"
#include "include/GUI/pixmap.h"
#include "include/GUI/pushbutton.h"

#include <memory>

#include <QGraphicsScene>
#include <QtWidgets>
#include <QQueue>
#include <QVector>
#include <QPointF>
#include <QMutex>

class IPPController;

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GraphicsScene(const QSize& size);

    void createUI();
    void addPoint(const std::vector<QPointF>& points);
    void addPointKFoldCrossValidation(const std::vector<std::vector<QPointF>>& points);

    void ippController(IPPController* ippController) { m_ippController = ippController; }

    void trainingDidFinish() { m_finished = true; }

    bool isDropAllow();
    void dropData(const std::vector<std::string>& filepaths);

public slots:
    void checkStatus();

signals:
    void goToNextState();
    void goToPreviousState();

private slots:
    void createANN();
    void importANN();
    void exportANN();
    void selectTrainingSet();
    void selectValidationSet();
    void nextState();
    void previousState();
    void backToStartMenu();
    void startTraining();
    void stopTraining();

private:
    // Tools
    IPPController* m_ippController;
    QQueue<std::vector<QPointF>> m_futurePoints;
    QQueue<std::vector<std::vector<QPointF>>> m_futurePointsKFoldCrossValidation;
    qint32 m_currentState;
    unsigned int m_k;
    bool m_finished;
    bool m_kFoldCrossValidation;
    int m_trainingQuantity;
    int m_validationQuantity;

    QTimer *m_timer;

    // GUI
    ANNGraphics m_annGraphics;
    QMutex m_mutex;
    QPixmap m_successImage;
    QPixmap m_errorImage;
    PushButton m_newButton;
    PushButton m_importButton;
    PushButton m_exportButton;
    PushButton m_trainingButton;
    PushButton m_validationButton;
    PushButton m_startTrainingButton;
    PushButton m_stopTrainingButton;
    PushButton m_nextButton;
    PushButton m_backButton;
    QLabel m_errorsLabel;
    QLabel m_resultLabel;
    std::shared_ptr<Pixmap> m_bg1;
    std::shared_ptr<Pixmap> m_bg2;
    std::shared_ptr<Pixmap> m_bg3;
    std::shared_ptr<Pixmap> m_bg4;
    std::shared_ptr<Pixmap> m_draggedImage;
    std::shared_ptr<Pixmap> m_resultImage;

    void stateChange();

    QString formatErrorsLabel(const std::vector<QPointF>& points);
};

#endif // GRAPHICSSCENE_H
