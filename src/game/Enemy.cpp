#include "Enemy.h"
#include "Path.h"
#include "Game.h"

#include "../ui/GLRenderer.h"
#include "../scene/GLNode.h"
#include "../scene/BMDImport.h"
#include "../scene/CollisionShape.h"

Enemy::Enemy( std::string filename )
    : mHitPoints( 3 )
{
    mEnemyShip = new GLNode( GLRenderer::getRootNode() );
    BMDImport::loadFromFile( mEnemyShip, filename );
    mEnemyShip->setTag( Game::NODE_ENEMY );

    mEnemyShip->setCollisionShape( CollisionShape::newSphereShape( mEnemyShip, 4.f ) );

    mEnemyShip->setPosition( glm::vec3( 0, 10000, 0 ) );
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

    mEnemyShip->setPosition( mPath->getNextPosition() );

    std::vector<CollisionShape*> shapes =
            CollisionShape::shapesCollidingWith( mEnemyShip->getCollisionShape() );
    for( unsigned int x = 0; x < shapes.size(); x++ )
    {
        if( shapes.at( x )->getNode()->getTag() == Game::NODE_BULLET )
            mHitPoints--;
    }

    if( mHitPoints <= 0 )
        mEnemyShip->hide();
}
