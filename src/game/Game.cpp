#include "Game.h"

#include <QKeyEvent>
#include <QSettings>
#include "../ui/GLRenderer.h"

#include "../scene/Node.h"
#include "../scene/GLCameraNode.h"
#include "../scene/GLNode.h"
#include "../scene/BMDImport.h"

#include "Level.h"
#include "PlayerShip.h"

#include <iostream>

Game::Game()
{
    for( int x = 0; x < PlayerShip::ACTION_COUNT; x++ )
    {
        mActionMap[x] = 0;
        mActionTriggered[x] = false;
    }
}

void Game::init()
{
    QSettings settings( "raw/config.ini", QSettings::IniFormat );

    mActionMap[PlayerShip::ACTION_MOVE_UP] =
            settings.value( "ActionMoveUp", Qt::Key_Up ).toInt();
    mActionMap[PlayerShip::ACTION_MOVE_DOWN] =
            settings.value( "ActionMoveDown", Qt::Key_Down ).toInt();
    mActionMap[PlayerShip::ACTION_MOVE_LEFT] =
            settings.value( "ActionMoveLeft", Qt::Key_Left ).toInt();
    mActionMap[PlayerShip::ACTION_MOVE_RIGHT] =
            settings.value( "ActionMoveRight", Qt::Key_Right ).toInt();

    mCamera = new GLCameraNode( GLRenderer::getRootNode(), glm::vec3( 0, 20, 10 ),
                      glm::quat() );

    mActiveShip = new PlayerShip( "raw/ship.bmd", mCamera );
    mActiveLevel = new Level( "raw/testlevel.bmd" );
}

void Game::run()
{
    for( int x = 0; x < PlayerShip::ACTION_COUNT; x++ )
    {
        if( mActionTriggered[x] )
            mActiveShip->action( (PlayerShip::SHIP_ACTIONS)x );
    }
}

void Game::processKeyEvents( QKeyEvent *evt, bool pressed )
{
    for( int x = 0 ; x < PlayerShip::ACTION_COUNT; x++ )
    {
        if( evt->key() == mActionMap[x] )
            mActionTriggered[x] = pressed;
    }
}