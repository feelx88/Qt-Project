#include "Game.h"

#include <QKeyEvent>
#include <QSettings>
#include "../ui/GLRenderer.h"

#include "../scene/Node.h"
#include "../scene/GLCameraNode.h"
#include "../scene/GLNode.h"
#include "../scene/BMDImport.h"
#include "../scene/CollisionShape.h"

#include "Level.h"
#include "PlayerShip.h"

#include <iostream>

const int Game::frameRate = 60;
const int Game::maxFrameSkip = 5;
const int Game::frameLength = Clock::ticksPerSecond / Game::frameRate;
const float Game::frameRateMultiplicator = 1.f / (float)Game::frameRate;

Game::Game()
    : mCamera( 0 ), mActiveLevel( 0 ),
      mNextFrame( 0 )
{
    for( int x = 0; x < PlayerShip::ACTION_COUNT; x++ )
    {
        mActionMap[x] = 0;
        mActionTriggered[x] = false;
    }
}

Game::~Game()
{
    delete mActiveLevel;

    delete mCamera;

    GLNode::clearTextures();
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
    mActionMap[PlayerShip::ACTION_MOVE_FASTER] =
            settings.value( "ActionMoveFaster", Qt::Key_W ).toInt();
    mActionMap[PlayerShip::ACTION_MOVE_SLOWER] =
            settings.value( "ActionMoveSlower", Qt::Key_S ).toInt();
    mActionMap[PlayerShip::ACTION_FIRE_PRIMARY] =
            settings.value( "ActionFirePrimary", Qt::Key_Q ).toInt();
    mActionMap[PlayerShip::ACTION_FIRE_SECONDARY] =
            settings.value( "ActionFireSecondary", Qt::Key_E ).toInt();

    mCamera = new GLCameraNode( GLRenderer::getRootNode(),
                                glm::vec3( 0, 20, 10 ), glm::quat() );

    mActiveLevel = new Level( mCamera );
    mActiveLevel->loadLevel( "raw/testlevel2/testlevel2.xml" );

    mNextFrame = Clock::getTime();
}

void Game::run()
{
    int framesSkipped = 0;

    while( Clock::getTime() > mNextFrame && framesSkipped < maxFrameSkip )
    {
        CollisionShape::findCollisionPairs();
        for( int x = 0; x < PlayerShip::ACTION_COUNT; x++ )
        {
            if( mActionTriggered[x] )
                mActiveLevel->action( (PlayerShip::SHIP_ACTIONS)x );
        }

        mActiveLevel->update();

        mNextFrame += frameLength;
        framesSkipped++;
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
