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
    : mCamera( camera ), mCurAcceleration( glm::vec3() ),
      mSideAcceleration( 0.5f ), mSideMinMaxSpeed( 10.f ),
      mForwardAcceleration( 1.f ), mForwardMinSpeed( -15.f ),
      mForwardMaxSpeed( -3.f ), mShipTiltAngle( glm::vec3( 10.f, -10.f, -30.f ) )
{
    mShipModel = new GLNode( GLRenderer::getRootNode() );
    BMDImport::loadFromFile( mShipModel, fileName );

    mShipModel->setPosition( glm::vec3( 0, 10, 50 ) );

    glm::vec3 position = mShipModel->getPosition();

    mCamera->setLookAt( position );
    mCamera->setPosition( position + glm::vec3( 0, 10, 10 ) );

    mPrimaryWeapon = new Weapon( mShipModel, 150, 20.f, 30,
                                 "raw/primaryWeaponRay1.bmd" );
    mSecondaryWeapon = new Weapon( mShipModel, 2000, 10.f, 2,
                                   "raw/secondaryWeaponBomb1.bmd" );

    mSecondaryWeapon->setInfiniteAmmo( false );
    mSecondaryWeapon->incrementAmmo( 3 );

    mCrosshairBack = new GLNode( GLRenderer::getRootNode() );
    mCrosshairFront = new GLNode( GLRenderer::getRootNode() );

    BMDImport::loadFromFile( mCrosshairFront, "raw/crosshair.bmd" );
    BMDImport::loadFromFile( mCrosshairBack, "raw/crosshair.bmd" );

    mCrosshairFront->setAlwasDrawToFront( true );
    mCrosshairBack->setAlwasDrawToFront( true );
}

PlayerShip::~PlayerShip()
{
    delete mShipModel;
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
        mCurAcceleration -= glm::vec3( 0, 0, zAcc );
        break;
    case ACTION_MOVE_SLOWER:
        mCurAcceleration += glm::vec3( 0, 0, zAcc );
        break;
    case ACTION_MOVE_UP:
        mCurAcceleration += glm::vec3( 0, xyAcc, 0 );
        break;
    case ACTION_MOVE_DOWN:
        mCurAcceleration -= glm::vec3( 0, xyAcc, 0 );
        break;
    case ACTION_MOVE_LEFT:
        mCurAcceleration -= glm::vec3( xyAcc, 0, 0 );
        break;
    case ACTION_MOVE_RIGHT:
        mCurAcceleration += glm::vec3( xyAcc, 0, 0 );
        break;
    case ACTION_FIRE_PRIMARY:
        if( mPrimaryWeapon )
            mPrimaryWeapon->shoot();
        break;
    case ACTION_FIRE_SECONDARY:
        if( mSecondaryWeapon )
            mSecondaryWeapon->shoot();
        break;
    }


}

void PlayerShip::update()
{
    const float timeFactor = Game::frameRateMultiplicator;
    const float angleFraction = (float)Game::frameRate / mSideMinMaxSpeed;

    const float xyMinMax = mSideMinMaxSpeed * timeFactor;
    const float zMin = mForwardMinSpeed * timeFactor;
    const float zMax = mForwardMaxSpeed * timeFactor;

    mCurAcceleration = glm::clamp( mCurAcceleration,
                                   glm::vec3( -xyMinMax, -xyMinMax, zMin ),
                                   glm::vec3( +xyMinMax, +xyMinMax, zMax ) );

    glm::vec3 position = mShipModel->getPosition();

    position += mCurAcceleration;

    glm::quat rotation;
    rotation = glm::gtc::quaternion::rotate( rotation, mCurAcceleration.y *
                                             mShipTiltAngle.x * angleFraction,
                                             glm::vec3( 1, 0, 0 ) );
    rotation = glm::gtc::quaternion::rotate( rotation, mCurAcceleration.x *
                                             mShipTiltAngle.y * angleFraction,
                                             glm::vec3( 0, 1, 0 ) );
    rotation = glm::gtc::quaternion::rotate( rotation, mCurAcceleration.x *
                                             mShipTiltAngle.z * angleFraction,
                                             glm::vec3( 0, 0, 1 ) );

    //Only for testing
    //----------------------
    if( position.z < -200 )
        position.z = 50;
    //----------------------

    mShipModel->setPosition( position );
    mShipModel->setRotation( rotation );

    float oldz = mCurAcceleration.z;

    mCurAcceleration -= mCurAcceleration * timeFactor;// * 10.f;

    mCurAcceleration.z = oldz;

    mCamera->setLookAt( position );
    mCamera->setPosition( position + glm::vec3( 0, 0, 10 ) );

    if( mPrimaryWeapon )
        mPrimaryWeapon->update();
    if( mSecondaryWeapon )
        mSecondaryWeapon->update();

    mCrosshairFront->setPosition( mShipModel->getPosition() );
    mCrosshairFront->setRotation( mShipModel->getRotation() );
    mCrosshairFront->move( glm::vec3( 0, 0, -50 ) );

    mCrosshairBack->setPosition( mShipModel->getPosition() );
    mCrosshairBack->setRotation( mShipModel->getRotation() );
    mCrosshairBack->move( glm::vec3( 0, 0, -20 ) );
}
