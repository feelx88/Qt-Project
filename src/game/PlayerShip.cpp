#include "PlayerShip.h"

#include "../core/Clock.h"

#include "../ui/GLRenderer.h"
#include "../scene/GLNode.h"
#include "../scene/GLCameraNode.h"
#include "../scene/BMDImport.h"
#include "../scene/CollisionShape.h"

#include "Game.h"
#include "Weapon.h"

PlayerShip::PlayerShip( std::string fileName, GLCameraNode *camera )
    : mCamera( camera ), mSideAcceleration( 30.f ), mSideMinMaxSpeed( 10.f ),
      mForwardAcceleration( 10.f ), mForwardMinSpeed( -5.f ),
      mForwardMaxSpeed( 15.f ), mShipTiltAngle( glm::vec3( 10.f, -10.f, -30.f ) ),
      mCurAcceleration( 0.f ), mMode( MODE_FIXED_DIRECTION )
{
    mShipNode = new GLNode( GLRenderer::getRootNode() );
    BMDImport::loadFromFile( mShipNode, fileName );

    mShipNode->setTag( Game::NODE_PLAYERSHIP );

    mShipNode->setPosition( glm::vec3( 0, 10, 50 ) );

    mShipNode->setCollisionShape( CollisionShape::newSphereShape( mShipNode, 1 ) );

    glm::vec3 position = mShipNode->getPosition();

    mCamera->setLookAt( position );
    mCamera->setPosition( position + glm::vec3( 0, 10, 10 ) );

    mPrimaryWeapon = new Weapon( mShipNode, 150, 20.f, 30,
                                 "data/Models/PrimaryWeapon/primaryWeaponRay1.bmd" );
    mPrimaryWeapon->setDamage( 1 );
    mSecondaryWeapon = new Weapon( mShipNode, 2000, 10.f, 2,
                                   "data/Models/SecondaryWeapon/secondaryWeaponBomb1.bmd" );
    mSecondaryWeapon->setDamage( 5 );

    mSecondaryWeapon->setInfiniteAmmo( false );
    mSecondaryWeapon->incrementAmmo( 5 );

    mCrosshairBack = new GLNode( GLRenderer::getRootNode() );
    mCrosshairFront = new GLNode( GLRenderer::getRootNode() );

    BMDImport::loadFromFile( mCrosshairFront, "data/Models/Crosshair/crosshair.bmd" );
    BMDImport::loadFromFile( mCrosshairBack, "data/Models/Crosshair/crosshair.bmd" );

    mCrosshairFront->setAlwasDrawToFront( true );
    mCrosshairBack->setAlwasDrawToFront( true );

    setDirection( glm::vec3( 0.f, 0.f, -1.f ) );
}

PlayerShip::~PlayerShip()
{
    delete mShipNode;
    delete mPrimaryWeapon;
    delete mSecondaryWeapon;
    delete mCrosshairFront;
    delete mCrosshairBack;
}

void PlayerShip::action( PlayerShip::SHIP_ACTIONS action )
{
    const float timeFactor = Game::frameRateMultiplicator;

    const float xyAcc = mSideAcceleration * timeFactor;
    const float zAcc = mForwardAcceleration * timeFactor;

    switch( action )
    {
    case ACTION_MOVE_FASTER:
        mCurAcceleration += zAcc;
        break;
    case ACTION_MOVE_SLOWER:
        mCurAcceleration -= zAcc;
        break;
    case ACTION_MOVE_UP:
        mCurRotation += glm::vec3( xyAcc, 0, 0 );
        mCurRotationMinus.x = 0;
        break;
    case ACTION_MOVE_DOWN:
        mCurRotation -= glm::vec3( xyAcc, 0, 0 );
        mCurRotationMinus.x = 0;
        break;
    case ACTION_MOVE_LEFT:
        mCurRotation += glm::vec3( 0, xyAcc, xyAcc * 5 );
        mCurRotationMinus.y = 0;
        break;
    case ACTION_MOVE_RIGHT:
        mCurRotation -= glm::vec3( 0, xyAcc, xyAcc * 5 );
        mCurRotationMinus.y = 0;
        break;
    case ACTION_FIRE_PRIMARY:
        if( mPrimaryWeapon )
            mPrimaryWeapon->shoot();
        break;
    case ACTION_FIRE_SECONDARY:
        if( mSecondaryWeapon )
            mSecondaryWeapon->shoot();
        break;
    case ACTION_COUNT:
    default:
        break;
    }
}

void PlayerShip::update()
{
    const float timeFactor = Game::frameRateMultiplicator;

    const glm::vec3 xDir = glm::vec3( 1.f, 0.f, 0.f );
    const glm::vec3 yDir = glm::vec3( 0.f, 1.f, 0.f );
    const glm::vec3 zDir = glm::vec3( 0.f, 0.f, 1.f );

    glm::vec3 position = mShipNode->getPosition();
    glm::quat rotation;

    if( mMode == MODE_FIXED_DIRECTION )
        rotation = mShipDirectionQuat;

    if( mMode == MODE_FREEFLIGHT )
    {
        mCurRotation.z = glm::clamp( mCurRotation.z, -50.f, 50.f );
        mCurRotation.x = glm::clamp( mCurRotation.x, -50.f, 50.f );
    }
    else
    {
        mCurRotation = glm::clamp( mCurRotation,
                                   glm::vec3( -20.f, -40.f, -50.f ),
                                   glm::vec3( 20.f, 40.f, 50.f ) );
    }

    rotation = glm::gtc::quaternion::rotate( rotation, mCurRotation.y, yDir );
    rotation = glm::gtc::quaternion::rotate( rotation, mCurRotation.x, xDir );

    mCurAcceleration = glm::clamp( mCurAcceleration,
                                   mForwardMinSpeed, mForwardMaxSpeed );

    mShipNode->move( glm::vec3( 0, 0, -mCurAcceleration * timeFactor ) );

    rotation = glm::gtc::quaternion::rotate( rotation, mCurRotation.z, zDir );

    mShipNode->setRotation( rotation );

    if( mMode == MODE_FREEFLIGHT )
        mCurRotation.z -= mCurRotationMinus.z * timeFactor;
    else
        mCurRotation -= mCurRotationMinus * timeFactor;

    mCurRotationMinus = mCurRotation;

    if( mMode == MODE_FREEFLIGHT )
    {
        mCamera->setLookAt( position );
        mCamera->setPosition( position );
        mCamera->setRotation( rotation );
        mCamera->move( glm::vec3( 0, 1, 10 ) );
    }
    else //MODE_FIXED_DIRECTION
    {
        mCamera->setLookAt( position );
        mCamera->setPosition( position );
        mCamera->setRotation( mShipDirectionQuat );
        mCamera->move( glm::vec3( 0, 1, 10 ) );
    }

    if( mPrimaryWeapon )
        mPrimaryWeapon->update();
    if( mSecondaryWeapon )
        mSecondaryWeapon->update();

    mCrosshairFront->setPosition( position );
    mCrosshairFront->setRotation( rotation );
    mCrosshairFront->move( glm::vec3( 0, 0, -200 ) );
    mCrosshairFront->setScale( glm::vec3( 3 ) );

    mCrosshairBack->setPosition( position );
    mCrosshairBack->setRotation( rotation );
    mCrosshairBack->move( glm::vec3( 0, 0, -20 ) );
}

void PlayerShip::setPosition( glm::core::type::vec3 position )
{
    if( mShipNode )
        mShipNode->setPosition( position );
}

void PlayerShip::setDirection(glm::core::type::vec3 direction)
{
    direction *= -1;
    mShipDirection = direction;
    mShipLeft = glm::cross( direction, glm::vec3( 0.f, -1.f, 0.f ) );
    mShipUp = glm::cross( direction, mShipLeft );

    mShipDirection = glm::normalize( mShipDirection );
    mShipUp = glm::normalize( mShipUp );
    mShipLeft = glm::normalize( mShipLeft );

    glm::mat3x3 directionMatrix( mShipLeft.x, mShipUp.x, mShipDirection.x,
                                 mShipLeft.y, mShipUp.y, mShipDirection.y,
                                 mShipLeft.z, mShipUp.z, mShipDirection.z );

    mShipDirectionQuat = glm::toQuat( directionMatrix );
    glm::normalize( mShipDirectionQuat );
    mShipDirectionQuat = glm::inverse( mShipDirectionQuat );
}
