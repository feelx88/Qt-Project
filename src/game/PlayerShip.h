#ifndef PLAYERSHIP_H
#define PLAYERSHIP_H

#include <string>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class GLNode;
class GLCameraNode;

class Weapon;

class PlayerShip
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

    PlayerShip( std::string fileName, GLCameraNode *camera );

    void action( SHIP_ACTIONS action );

    void update();

protected:
    GLNode *mShipModel;
    GLCameraNode *mCamera;

    Weapon *mPrimaryWeapon, *mSecondaryWeapon;

    float mSideAcceleration, mSideMinMaxSpeed;
    float mForwardAcceleration, mForwardMinSpeed, mForwardMaxSpeed;

    glm::vec3 mShipTiltAngle;

    glm::vec3 mCurAcceleration;
};

#endif // PLAYERSHIP_H
