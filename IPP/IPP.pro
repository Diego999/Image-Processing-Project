QT       += core gui widgets

TARGET = Test
TEMPLATE = app

win32 {
    include (D:\qwt-6.1.0\features\qwt.prf)

    INCLUDEPATH += D:\\opencv\\build-mingw\\install\\include

    LIBS += -LD:\\opencv\\build-mingw\\bin \
         -lopencv_core248 \
         -lopencv_imgproc248 \
         -lopencv_features2d248\
         -lopencv_highgui248
}

macx {
    include (/usr/local/qwt-6.1.0/features/qwt.prf)

    INCLUDEPATH += /usr/local/include

    LIBS += -L/usr/local/lib \
         -lopencv_core \
         -lopencv_imgproc \
         -lopencv_features2d\
         -lopencv_highgui
    QMAKE_CXXFLAGS = -mmacosx-version-min=10.7 -std=gnu0x -stdlib=libc+
}

CONFIG +=c++11

SOURCES += src/main.cpp \
    src/Utils/utils.cpp \
    src/NeuralNetwork/neuron.cpp \
    src/NeuralNetwork/neuronlayer.cpp \
    src/Utils/settings.cpp \
    src/NeuralNetwork/artificialneuralnetwork.cpp \
    src/Tests/tests.cpp \
    src/Picture/PictureController.cpp \
    src/NeuralNetwork/anncontroller.cpp \
    src/GUI/graphicsview.cpp \
    src/GUI/graphicsscene.cpp \
    src/GUI/pixmap.cpp \
    src/GUI/anngraphics.cpp \
    src/Controller/ippcontroller.cpp \
    src/GUI/pushbutton.cpp

HEADERS += \
    include/Utils/utils.h \
    include/NeuralNetwork/neuron.h \
    include/NeuralNetwork/neuronlayer.h \
    include/Utils/settings.h \
    include/NeuralNetwork/artificialneuralnetwork.h \
    include/Tests/tests.h \
    include/Picture/PictureController.h \
    include/NeuralNetwork/anncontroller.h \
    include/GUI/graphicsview.h \
    include/GUI/graphicsscene.h \
    include/GUI/pixmap.h \
    include/GUI/anngraphics.h \
    include/Controller/ippcontroller.h \
    include/GUI/pushbutton.h

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    resources/images/bg1.png \
    resources/images/bg2.png \
    resources/images/bg3.png \
    resources/images/bg4.png \
    resources/images/draghere.png \
    resources/images/error.png \
    resources/images/success.png
