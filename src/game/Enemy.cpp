#include "Enemy.h"
#include "Path.h"
#include "Game.h"

#include "../ui/GLRenderer.h"
#include "../scene/GLNode.h"
#include "../scene/BMDImport.h"
#include "../scene/CollisionShape.h"

#include "../game/Weapon.h"

Enemy::Enemy( std::string filename )
    :  mStart( false ), mHitPoints( 3 )
{
    mEnemyShip = new GLNode( GLRenderer::getRootNode() );
    BMDImport::loadFromFile( mEnemyShip, filename );
    mEnemyShip->setTag( Game::NODE_ENEMY );

    mEnemyShip->setCollisionShape( CollisionShape::newSphereShape( mEnemyShip, 4.f ) );

    mEnemyShip->setPosition( glm::vec3( 0, 10000, 0 ) );

    mStartTrigger = new Node( GLRenderer::getRootNode() );
    mStartTrigger->setCollisionShape(
                CollisionShape::newSphereShape( mStartTrigger, 150.f ) );
    mStartTrigger->setTag( Game::NODE_ENEMY_SENSOR );
}

Enemy::~Enemy()
{
    delete mEnemyShip;
    delete mPath;
}

void Enemy::update()
{
    if( !mEnemyShip || !mPath )
        return;

    if( mStart )
        mEnemyShip->setPosition( mPath->getNextPosition() );

    std::vector<CollisionShape*> shapes =
            CollisionShape::shapesCollidingWith( mEnemyShip->getCollisionShape() );
    for( unsigned int x = 0; x < shapes.size(); x++ )
    {
        Node *node = shapes.at( x )->getNode();
        if( node->getTag() == Game::NODE_BULLET )
            mHitPoints -= Weapon::getDamage( dynamic_cast<GLNode*>( node ) );
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
