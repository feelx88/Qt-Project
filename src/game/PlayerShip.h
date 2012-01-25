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

    enum CONTROL_MODE
    {
        MODE_FREEFLIGHT = 0,
        MODE_FIXED_DIRECTION
    };

    PlayerShip( std::string fileName, GLCameraNode *camera );

    virtual ~PlayerShip();

    void action( SHIP_ACTIONS action );

    void update();

    void setPosition( glm::vec3 position );

    void setDirection( glm::vec3 direction );

protected:
    GLNode *mShipModel;
    GLCameraNode *mCamera;

    Weapon *mPrimaryWeapon, *mSecondaryWeapon;

    GLNode *mCrosshairFront, *mCrosshairBack;

    float mSideAcceleration, mSideMinMaxSpeed;
    float mForwardAcceleration, mForwardMinSpeed, mForwardMaxSpeed;

    glm::vec3 mShipTiltAngle;

    float mCurAcceleration;
    glm::vec3 mCurRotation, mCurRotationMinus;

    glm::vec3 mShipDirection, mShipUp, mShipLeft;
    glm::quat mShipDirectionQuat;

    CONTROL_MODE mMode;
};

#endif // PLAYERSHIP_H
