#include "PlayerShip.h"

#include "../core/Clock.h"

#include "../ui/GLRenderer.h"
#include "../scene/GLNode.h"
#include "../scene/GLCameraNode.h"
#include "../scene/BMDImport.h"

#include "Game.h"
#include "Weapon.h"

PlayerShip::PlayerShip( std::string fileName, GLCameraNode *camera )
    : mCamera( camera ), mCurAcceleration( glm::vec3( 0, 0, -0.5 ) )
{
    mShipModel = new GLNode( GLRenderer::getRootNode() );
    BMDImport::loadFromFile( mShipModel, fileName );

    mShipModel->setPosition( glm::vec3( 0, 10, 0 ) );

    glm::vec3 position = mShipModel->getPosition();

    mCamera->setLookAt( position );
    mCamera->setPosition( position + glm::vec3( 0, 10, 10 ) );

    //Not yet working
    //mPrimaryWeapon = new Weapon( 10, "raw/rayOne.bmd" );
    //mSecondaryWeapon = new Weapon( 10, "raw/rayTwo.bmd" );
}

void PlayerShip::action( PlayerShip::SHIP_ACTIONS action )
{
    const float timeFactor = Game::frameRateMultiplicator;

    switch( action )
    {
    case ACTION_MOVE_FASTER:
        break;
    case ACTION_MOVE_SLOWER:
        break;
    case ACTION_MOVE_UP:
        mCurAcceleration += glm::vec3( 0, 2 * timeFactor, 0 );
        break;
    case ACTION_MOVE_DOWN:
        mCurAcceleration -= glm::vec3( 0, 2 * timeFactor, 0 );
        break;
    case ACTION_MOVE_LEFT:
        mCurAcceleration -= glm::vec3( 2 * timeFactor, 0, 0 );
        break;
    case ACTION_MOVE_RIGHT:
        mCurAcceleration += glm::vec3( 2 * timeFactor, 0, 0 );
        break;
    case ACTION_FIRE_PRIMARY:
        break;
    case ACTION_FIRE_SECONDARY:
        break;
    }

    mCurAcceleration = glm::clamp( mCurAcceleration, glm::vec3( -10 * timeFactor ),
                             glm::vec3( 10 * timeFactor ) );
}

void PlayerShip::update()
{
    const float timeFactor = Game::frameRateMultiplicator;

    glm::vec3 position = mShipModel->getPosition();

    mCurAcceleration += glm::vec3( 0, 0, -1 * timeFactor );

    position += mCurAcceleration;

    glm::quat rotation;
    rotation = glm::gtc::quaternion::rotate( rotation,
                                             mCurAcceleration.y * 20.f *
                                             Game::frameRate / 10.f,
                                             glm::vec3( 1, 0, 0 ) );
    rotation = glm::gtc::quaternion::rotate( rotation,
                                             mCurAcceleration.x * -10.f *
                                             Game::frameRate / 10.f,
                                             glm::vec3( 0, 1, 0 ) );
    rotation = glm::gtc::quaternion::rotate( rotation,
                                             mCurAcceleration.x * -30.f *
                                             Game::frameRate / 10.f,
                                             glm::vec3( 0, 0, 1 ) );

    //Only for testing
    //----------------------
    if( position.z < -200 )
        position.z = 0;
    //----------------------

    mShipModel->setPosition( position );
    mShipModel->setRotation( rotation );

    mCurAcceleration -= mCurAcceleration * timeFactor * 10.f;

    mCamera->setLookAt( position );
    mCamera->setPosition( position + glm::vec3( 0, 2, 10 ) );
}
