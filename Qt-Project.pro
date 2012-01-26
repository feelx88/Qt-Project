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
    src/scene/CollisionShape.cpp \
    src/game/Enemy.cpp \
    src/game/Path.cpp

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
    src/game/Weapon.h \
    src/scene/CollisionShape.h \
    src/game/Enemy.h \
    src/game/Path.h

FORMS    += src/ui/MainWindow.ui

OTHER_FILES += \
    README \
    raw/vadertie.blend \
    raw/x-wing.blend \
    raw/vader_tie_fighter.blend \
    raw/tie_fighter.blend \
    src/blender-export/bmd-export.py \
    src/blender-export/level-export.py

unix {
LIBS += -lGLU
}
win32 {
LIBS += -lglu32
}

QMAKE_CXXFLAGS_WARN_ON += -Wno-unknown-pragmas




























