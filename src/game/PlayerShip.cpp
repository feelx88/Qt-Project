#include "PlayerShip.h"

#include "../ui/GLRenderer.h"
#include "../scene/GLNode.h"
#include "../scene/GLCameraNode.h"
#include "../scene/BMDImport.h"

PlayerShip::PlayerShip( Node *parent, std::string fileName, GLCameraNode *camera )
    : Node( parent ), mCamera( camera ), rot( false )
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
    mShipModel->setRotation( glm::quat() );
    glm::vec3 position = mShipModel->getPosition();
    glm::quat rotation = mShipModel->getRotation();

    switch( action )
    {
    case ACTION_MOVE_FASTER:
        break;
    case ACTION_MOVE_SLOWER:
        break;
    case ACTION_MOVE_UP:
        position += glm::vec3( 0, 0.01, 0 );
        curRot = glm::gtc::quaternion::rotate( rotation, 10.f, glm::vec3( 1, 0, 0 ) );
        rot = true;
        break;
    case ACTION_MOVE_DOWN:
        position += glm::vec3( 0, -0.01, 0 );
        curRot = glm::gtc::quaternion::rotate( rotation, -10.f, glm::vec3( 1, 0, 0 ) );
        rot = true;
        break;
    case ACTION_MOVE_LEFT:
        position += glm::vec3( -0.01, 0, 0 );
        curRot = glm::gtc::quaternion::rotate( rotation, 20.f, glm::vec3( 0, 0, 1 ) );
        rot = true;
        break;
    case ACTION_MOVE_RIGHT:
        position += glm::vec3( 0.01, 0, 0 );
        curRot = glm::gtc::quaternion::rotate( rotation, -20.f, glm::vec3( 0, 0, 1 ) );
        rot = true;
        break;
    case ACTION_FIRE_PRIMARY:
        break;
    case ACTION_FIRE_SECONDARY:
        break;
    }

    mShipModel->setPosition( position );
    mShipModel->setRotation( rotation );
}

void PlayerShip::update()
{
    glm::vec3 position = mShipModel->getPosition();

    if( position.z < -200 )
        position.z = 0;

    if( rot )
    {
        mShipModel->setRotation( curRot );
        rot = false;
    }
    else
        mShipModel->setRotation( glm::quat() );

    mShipModel->setPosition( position + glm::vec3( 0, 0, -0.01 ) );

    mCamera->setLookAt( position );
    mCamera->setPosition( position + glm::vec3( 0, 0, 10 ) );
}
