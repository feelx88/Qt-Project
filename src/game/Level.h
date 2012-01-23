#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include "PlayerShip.h"

class GLNode;
class GLCameraNode;

class Enemy;

class Level
{
public:
    Level( GLCameraNode *camera );
    virtual ~Level();

    void update();

    void loadLevel( std::string fileName );

    PlayerShip *getPlayerShip()
    {
        return mPlayerShip;
    }

    void action( PlayerShip::SHIP_ACTIONS action );

protected:
    std::vector<GLNode*> mLevelMeshes;
    glm::vec3 mPlayerStart;

    std::vector<Enemy*> mEnemies;

    PlayerShip *mPlayerShip;
    GLCameraNode *mCamera;
};

#endif // LEVEL_H
