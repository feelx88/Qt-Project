#ifndef PLAYERSHIP_H
#define PLAYERSHIP_H

#include "../scene/Node.h"

#include <string>

class GLNode;
class GLCameraNode;

class PlayerShip : public Node
{
public:
    enum SHIP_ACTIONS
    {
        ACTION_MOVE_FASTER = 0,
        ACTION_MOVE_SLOWER,
        ACTION_MOVE_UP,
        ACTION_MOVE_DOWN,
        ACTION_MOVE_LEFT,
        ACTION_MOVE_RIGHT,
        ACTION_FIRE_PRIMARY,
        ACTION_FIRE_SECONDARY,
        ACTION_COUNT
    };

    PlayerShip( Node *parent, std::string fileName, GLCameraNode *camera );

    void action( SHIP_ACTIONS action );

    void update( int deltaNSec );

protected:
    GLNode *mShipModel;
    GLCameraNode *mCamera;

    glm::vec3 delta, movement, rotation;
};

#endif // PLAYERSHIP_H
