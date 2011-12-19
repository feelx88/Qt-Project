#include "PlayerShip.h"

#include "../ui/GLRenderer.h"
#include "../scene/GLNode.h"
#include "../scene/GLCameraNode.h"
#include "../scene/BMDImport.h"

PlayerShip::PlayerShip( Node *parent, std::string fileName, GLCameraNode *camera )
    : Node( parent ), mCamera( camera ), delta( glm::vec3() )
{
    mShipModel = new GLNode( GLRenderer::getRootNode() );
    BMDImport::loadFromFile( mShipModel, fileName );

    mShipModel->setPosition( glm::vec3( 0, 10, 0 ) );

    glm::vec3 position = mShipModel->getPosition();

    mCamera->setLookAt( position );
    mCamera->setPosition( position + glm::vec3( 0, 10, 10 ) );
}

void PlayerShip::action(PlayerShip::SHIP_ACTIONS action)
{
    switch( action )
    {
    case ACTION_MOVE_FASTER:
        break;
    case ACTION_MOVE_SLOWER:
        break;
    case ACTION_MOVE_UP:
        delta += glm::vec3( 0, 0.0001, 0 );
        break;
    case ACTION_MOVE_DOWN:
        delta -= glm::vec3( 0, 0.0001, 0 );
        break;
    case ACTION_MOVE_LEFT:
        delta -= glm::vec3( 0.0001, 0, 0 );
        break;
    case ACTION_MOVE_RIGHT:
        delta += glm::vec3( 0.0001, 0, 0 );
        break;
    case ACTION_FIRE_PRIMARY:
        break;
    case ACTION_FIRE_SECONDARY:
        break;
    }
}

void PlayerShip::update()
{
    glm::vec3 position = mShipModel->getPosition();

    glm::quat rotation = glm::gtc::quaternion::rotate( glm::quat(), delta.x * -2000, glm::vec3( 0, 0, 1 )  );
    rotation = glm::gtc::quaternion::rotate( rotation, delta.y * 2000, glm::vec3( 1, 0, 0 )  );
    mShipModel->setRotation( rotation );

    if( position.z < -200 )
        position.z = 0;

    mShipModel->setPosition( position + glm::vec3( delta.x, delta.y, -0.01 ) );

    delta *= 0.99;

    glm::clamp( delta, glm::vec3( -0.1, -0.1, -0.1 ), glm::vec3( 0.1, 0.1, 0.1 ) );

    mCamera->setLookAt( position );
    mCamera->setPosition( position + glm::vec3( 0, 2, 10 ) );
}
