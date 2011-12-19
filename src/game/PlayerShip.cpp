#include "PlayerShip.h"

#include "../ui/GLRenderer.h"
#include "../scene/GLNode.h"
#include "../scene/GLCameraNode.h"
#include "../scene/BMDImport.h"

PlayerShip::PlayerShip( Node *parent, std::string fileName, GLCameraNode *camera )
    : Node( parent ), mCamera( camera )
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
    glm::vec3 position = mShipModel->getPosition();

    switch( action )
    {
    case ACTION_MOVE_FASTER:
        break;
    case ACTION_MOVE_SLOWER:
        break;
    case ACTION_MOVE_UP:
        mShipModel->setPosition( position + glm::vec3( 0, 0.01, 0 ) );
        break;
    case ACTION_MOVE_DOWN:
        mShipModel->setPosition( position + glm::vec3( 0, -0.01, 0 ) );
        break;
    case ACTION_MOVE_LEFT:
        mShipModel->setPosition( position + glm::vec3( -0.01, 0, 0 ) );
        break;
    case ACTION_MOVE_RIGHT:
        mShipModel->setPosition( position + glm::vec3( 0.01, 0, 0 ) );
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

    if( position.z < -200 )
        position.z = 0;

    mShipModel->setPosition( position + glm::vec3( 0, 0, -0.05 ) );

    mCamera->setLookAt( position );
    mCamera->setPosition( position + glm::vec3( 0, 0, 10 ) );
}
