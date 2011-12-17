#include "Game.h"

#include <QKeyEvent>
#include <QSettings>
#include "../ui/GLRenderer.h"

#include "../scene/Node.h"
#include "../scene/GLCameraNode.h"
#include "../scene/GLNode.h"
#include "../scene/BMDImport.h"

#include <iostream>

Game::Game()
{
}

GLNode *node = 0;
GLCameraNode *cam = 0;

void Game::init()
{
    QSettings settings( "raw/config.ini", QSettings::IniFormat );

    cam = new GLCameraNode( GLRenderer::getRootNode(), glm::vec3( 0, 10, 10 ),
                      glm::quat() );

    node = new GLNode( GLRenderer::getRootNode(), glm::vec3(), glm::quat() );
    BMDImport::loadFromFile( node, "raw/ship.bmd" );
}

void Game::run()
{
    cam->setLookAt( node->getPosition() );

    if( mActionTriggered[ACTION_MOVE_UP] )
        node->setPosition( node->getPosition() + glm::vec3( 0, 0, -0.01 ) );
    if( mActionTriggered[ACTION_MOVE_DOWN] )
        node->setPosition( node->getPosition() + glm::vec3( 0, 0, 0.01 ) );
    if( mActionTriggered[ACTION_MOVE_LEFT] )
        node->setPosition( node->getPosition() + glm::vec3( -0.01, 0, 0 ) );
    if( mActionTriggered[ACTION_MOVE_RIGHT] )
        node->setPosition( node->getPosition() + glm::vec3( 0.01, 0, 0 ) );

}

void Game::processKeyEvents( QKeyEvent *evt, bool pressed )
{
    if( evt->key() == Qt::Key_Up )
        mActionTriggered[ACTION_MOVE_UP] = pressed;
    if( evt->key() == Qt::Key_Down )
        mActionTriggered[ACTION_MOVE_DOWN] = pressed;
    if( evt->key() == Qt::Key_Left )
        mActionTriggered[ACTION_MOVE_LEFT] = pressed;
    if( evt->key() == Qt::Key_Right )
        mActionTriggered[ACTION_MOVE_RIGHT] = pressed;
}
