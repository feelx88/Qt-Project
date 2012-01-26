#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include "PlayerShip.h"

class Node;
class GLNode;
class GLCameraNode;

class Enemy;

class Level
{
public:

    struct DirectionChanger
    {
        glm::vec3 position, direction;
        Node *node;
    };

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
    std::vector<DirectionChanger> mDirectionChangers;

    PlayerShip *mPlayerShip;
    GLCameraNode *mCamera;
};

#endif // LEVEL_H
