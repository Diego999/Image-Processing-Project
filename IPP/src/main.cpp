#include "include/Tests/tests.h"
#include "include/Controller/ippcontroller.h"
#include "include/GUI/graphicsscene.h"
#include "include/GUI/graphicsview.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    bool test = false;
    if(test)
    {
        testsANN();
        testsANNController();

        return 0;
    }
    else
    {
        const QSize size(800, 700);
        QApplication app(argc, argv);
        GraphicsScene scene(size);
        scene.createUI();
        GraphicsView view(&scene);
        view.resize(size);
        view.setFixedSize(size);
        view.show();

        IPPController ippc(scene);

        return app.exec();
    }
}
