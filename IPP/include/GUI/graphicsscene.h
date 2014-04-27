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
    std::shared_ptr<ANNGraphics> m_annGraphics;
    QMutex m_mutex;
    std::shared_ptr<QPixmap> m_successImage;
    std::shared_ptr<QPixmap> m_errorImage;
    std::shared_ptr<PushButton> m_newButton;
    std::shared_ptr<PushButton> m_importButton;
    std::shared_ptr<PushButton> m_exportButton;
    std::shared_ptr<PushButton> m_trainingButton;
    std::shared_ptr<PushButton> m_validationButton;
    std::shared_ptr<PushButton> m_startTrainingButton;
    std::shared_ptr<PushButton> m_stopTrainingButton;
    std::shared_ptr<PushButton> m_nextButton;
    std::shared_ptr<PushButton> m_backButton;
    std::shared_ptr<QLabel> m_errorsLabel;
    std::shared_ptr<QLabel> m_resultLabel;
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
