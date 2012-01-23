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

    enum NodeTags
    {
        NODE_PLAYERSHIP = 1,
        NODE_LEVEL,
        NODE_BULLET,
        NODE_ENEMY,
        NODE_ENEMY_SENSOR       //Used for starting enemy ships
    };

    Game();
    virtual ~Game();

    void init();

    void run();

    void processKeyEvents( QKeyEvent *evt, bool pressed );

    //frames per sec
    static const int frameRate;
    //maximum frames to be skipped on slow down
    static const int maxFrameSkip;
    //length of one frame
    static const int frameLength;
    //for calculations of speed, acceleration etc.
    static const float frameRateMultiplicator;

protected:
    bool mActionTriggered[PlayerShip::ACTION_COUNT];
    int mActionMap[PlayerShip::ACTION_COUNT];

    GLCameraNode *mCamera;

    Level *mActiveLevel;

    int mNextFrame;
};

#endif // GAME_H
