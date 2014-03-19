#include "include/Tests/tests.h"
#include "include/Controller/ippcontroller.h"
#include "include/GUI/graphicsscene.h"
#include "include/GUI/graphicsview.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    // testsANN();
    // testsANNController();

    QApplication app(argc, argv);
    GraphicsScene scene(400, 300);
    scene.createUI();
    GraphicsView view(&scene);
    view.show();



    IPPController ippc(scene);

    return app.exec();
}
