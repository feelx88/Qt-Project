#ifndef GAME_H
#define GAME_H

#include "PlayerShip.h"

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
};

#endif // GAME_H
