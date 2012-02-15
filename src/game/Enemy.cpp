#include "Enemy.h"
#include "Path.h"
#include "Game.h"

#include "../core/Clock.h"
#include "../core/Utility.h"

#include "../ui/GLRenderer.h"
#include "../scene/GLNode.h"
#include "../scene/BMDImport.h"
#include "../scene/CollisionShape.h"

#include "../game/Weapon.h"

Enemy::Enemy( std::string filename )
    :  mStart( false ), mHitPoints( 3 )
{
    mEnemyShip = new GLNode( GLRenderer::getRootNode() );
    BMDImport::loadFromFile( mEnemyShip, "data/Models/" + filename );
    mEnemyShip->setTag( Game::NODE_ENEMY );

    mEnemyShip->setCollisionShape( CollisionShape::newSphereShape( mEnemyShip, 4.f ) );

    mEnemyShip->setPosition( glm::vec3( 0, 10000, 0 ) );

    mStartTrigger = new Node( GLRenderer::getRootNode() );
    mStartTrigger->setCollisionShape(
                CollisionShape::newSphereShape( mStartTrigger, 100.f ) );
    mStartTrigger->setTag( Game::NODE_ENEMY_SENSOR );
}

Enemy::~Enemy()
{
    delete mEnemyShip;
    delete mPath;
}

void Enemy::update()
{
    if( mHitPoints <= 0  || !mEnemyShip || !mPath )
        return;

    if( mStart )
    {
        glm::vec3 cur = mPath->getCurrentPosition();
        glm::vec3 next = mPath->getNextPosition();
        mEnemyShip->setPosition( cur );
        mEnemyShip->setRotation( Utility::directionToQuat( next - cur ) );
    }

    std::vector<CollisionShape*> shapes =
            CollisionShape::shapesCollidingWith( mEnemyShip->getCollisionShape() );
    for( unsigned int x = 0; x < shapes.size(); x++ )
    {
        Node *node = shapes.at( x )->getNode();
        if( node->getTag() == Game::NODE_BULLET )
        {
            mHitPoints -= Weapon::getDamage( dynamic_cast<GLNode*>( node ) );
            mFlashTime = Clock::getTime();
            mEnemyShip->setColor( 0.1, 0.1, 0.1, 1 );
            mEnemyShip->setTexEnvMode( GL_ADD );
            mFlashing = true;

            if( mHitPoints <= 0 )
                mEnemyShip->setPosition( glm::vec3( 0, 10000, 0 ) );
        }
    }

    if( Clock::getTime() > mFlashTime + Clock::ticksPerSecond / 2 && mFlashing )
    {
        mFlashing = false;
        mEnemyShip->setColor( 1, 1, 1, 1 );
        mEnemyShip->setTexEnvMode( GL_MODULATE );
    }

    shapes =
            CollisionShape::shapesCollidingWith( mStartTrigger->getCollisionShape() );
    for( unsigned int x = 0; x < shapes.size(); x++ )
    {
        unsigned int tag = shapes.at( x )->getNode()->getTag();
        if( tag == Game::NODE_PLAYERSHIP )
            mStart = true;
    }

    if( mHitPoints <= 0 )
        mEnemyShip->hide();
}

void Enemy::setPath(Path *path)
{
    mPath = path;
    mStartTrigger->setPosition( mPath->getCurrentPosition() );
}

void Enemy::setStartRadius(float rad)
{
    mStartTrigger->getCollisionShape()->setSphereRadius( rad );
}

void Enemy::setStartPosition(glm::vec3 pos)
{
    mStartTrigger->setPosition( pos );
}
