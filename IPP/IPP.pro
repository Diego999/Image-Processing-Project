QT       += core gui widgets

TARGET = Test
TEMPLATE = app

win32 {
    INCLUDEPATH += D:\\opencv\\build-mingw\\install\\include

    LIBS += -LD:\\opencv\\build-mingw\\bin \
         -lopencv_core248 \
         -lopencv_imgproc248 \
         -lopencv_features2d248\
         -lopencv_highgui248
}

macx {
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
    src/Tests/tests.cpp

HEADERS += \
    include/Utils/utils.h \
    include/NeuralNetwork/neuron.h \
    include/NeuralNetwork/neuronlayer.h \
    include/Utils/settings.h \
    include/NeuralNetwork/artificialneuralnetwork.h \
    include/Tests/tests.h
