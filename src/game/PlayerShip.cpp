#include "PlayerShip.h"

#include "../core/Clock.h"

#include "../ui/GLRenderer.h"
#include "../scene/GLNode.h"
#include "../scene/GLCameraNode.h"
#include "../scene/BMDImport.h"

PlayerShip::PlayerShip( Node *parent, std::string fileName, GLCameraNode *camera )
    : Node( parent ), mCamera( camera ), delta( glm::vec3( 0, 0, -0.5 ) )
{
    mShipModel = new GLNode( this );
    BMDImport::loadFromFile( mShipModel, fileName );

    mShipModel->setPosition( glm::vec3( 0, 10, 0 ) );

    glm::vec3 position = mShipModel->getPosition();

    mCamera->setLookAt( position );
    mCamera->setPosition( position + glm::vec3( 0, 10, 10 ) );
}

void PlayerShip::action( PlayerShip::SHIP_ACTIONS action )
{
    switch( action )
    {
    case ACTION_MOVE_FASTER:
        break;
    case ACTION_MOVE_SLOWER:
        break;
    case ACTION_MOVE_UP:
        delta += glm::vec3( 0, 1, 0 );
        break;
    case ACTION_MOVE_DOWN:
        delta -= glm::vec3( 0, 1, 0 );
        break;
    case ACTION_MOVE_LEFT:
        delta -= glm::vec3( 1, 0, 0 );
        break;
    case ACTION_MOVE_RIGHT:
        delta += glm::vec3( 1, 0, 0 );
        break;
    case ACTION_FIRE_PRIMARY:
        break;
    case ACTION_FIRE_SECONDARY:
        break;
    }

    delta = glm::clamp( delta, glm::vec3( -1 ), glm::vec3( 1 ) );
}

void PlayerShip::update( int deltaNSec )
{
    Node::update( deltaNSec );

    glm::vec3 position = mShipModel->getPosition();

    float frameFactor = (float)deltaNSec / (float)Clock::ticksPerSecond;

    movement += ( delta * 0.5f ) * frameFactor;

    movement = glm::clamp( movement, glm::vec3( -2.f * frameFactor ), glm::vec3( 2.f * frameFactor ) );

    position += movement;

    if( position.z < -200 )
        position.z = 20;

    mShipModel->setPosition( position );

    movement *= 0.99f;
    delta = glm::vec3( 0, 0, -0.5 );

    mCamera->setLookAt( position );
    mCamera->setPosition( position + glm::vec3( 0, 2, 10 ) );
}
