#-------------------------------------------------
#
# Project created by QtCreator 2011-12-01T13:42:39
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = Qt-Project
TEMPLATE = app

INCLUDEPATH += ./include

SOURCES += src/main.cpp \
    src/ui/MainWindow.cpp \
    src/GraphicsEngine.cpp \
    src/ui/GLRenderer.cpp \
    src/scene/BMDImport.cpp \
    src/scene/GLNode.cpp \
    src/scene/GLCameraNode.cpp

HEADERS  += src/ui/MainWindow.h \
    src/GraphicsEngine.h \
    src/ui/GLRenderer.h \
    src/scene/BMDImport.h \
    src/scene/Node.h \
    src/scene/GLNode.h \
    src/core/Buffer.h \
    src/scene/GLCameraNode.h

FORMS    += src/ui/MainWindow.ui

OTHER_FILES += \
    src/blender-export/exporter.py \
    README \
    raw/vadertie.blend \
    raw/x-wing.blend \
    raw/vader_tie_fighter.blend \
    raw/tie_fighter.blend




























