#-------------------------------------------------
#
# Project created by QtCreator 2011-12-01T13:42:39
#
#-------------------------------------------------

QT       += core gui opengl xml

TARGET = Qt-Project
TEMPLATE = app

INCLUDEPATH += ./include

SOURCES += src/main.cpp \
    src/ui/MainWindow.cpp \
    src/ui/GLRenderer.cpp \
    src/scene/BMDImport.cpp \
    src/scene/Node.cpp \
    src/scene/GLNode.cpp \
    src/scene/GLCameraNode.cpp \
    src/game/Game.cpp \
    src/game/Level.cpp \
    src/game/PlayerShip.cpp \
    src/core/Clock.cpp \
    src/game/Weapon.cpp \

HEADERS  += src/ui/MainWindow.h \
    src/ui/GLRenderer.h \
    src/scene/BMDImport.h \
    src/scene/Node.h \
    src/scene/GLNode.h \
    src/core/Buffer.h \
    src/scene/GLCameraNode.h \
    src/game/Game.h \
    src/game/Level.h \
    src/game/PlayerShip.h \
    src/core/Clock.h \
    src/game/Weapon.h

FORMS    += src/ui/MainWindow.ui

OTHER_FILES += \
    src/blender-export/exporter.py \
    README \
    raw/vadertie.blend \
    raw/x-wing.blend \
    raw/vader_tie_fighter.blend \
    raw/tie_fighter.blend

LIBS += -lGLU




























