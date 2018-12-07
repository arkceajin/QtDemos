#-------------------------------------------------
#
# Project created by QtCreator 2018-12-07T16:32:52
#
#-------------------------------------------------

QT       += core gui quick multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtOpenCVCam
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

OPENCV_PATH = D:/OpenCV/opencv-4.0.0-build/install

INCLUDEPATH += $${OPENCV_PATH}/include
LIBS += -L$${OPENCV_PATH}/x86/mingw/bin \
        -lopencv_core400 \
        -lopencv_imgproc400 \
        -lopencv_videoio400

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    qtopencvcam.cpp

HEADERS += \
        mainwindow.h \
    qtopencvcam.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    qml.qrc
