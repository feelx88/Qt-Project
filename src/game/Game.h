#ifndef GAME_H
#define GAME_H

#include "PlayerShip.h"
#include "../core/Clock.h"

class QKeyEvent;
class QSettings;

class GLCameraNode;

class Level;

class Game
{
public:

    Game();

    void init();

    void run();

    void processKeyEvents( QKeyEvent *evt, bool pressed );

protected:
    bool mActionTriggered[PlayerShip::ACTION_COUNT];
    int mActionMap[PlayerShip::ACTION_COUNT];

    GLCameraNode *mCamera;

    Level *mActiveLevel;
    PlayerShip *mActiveShip;

    int mNextFrame;

    const int mFrameRate; //frames per sec
    const int mMaxFrameSkip; //maximum frames to be skipped on slow down
    const int mFrameLength;
};

#endif // GAME_H
