#include "Enemy.h"
#include "Path.h"

#include "../ui/GLRenderer.h"
#include "../scene/GLNode.h"
#include "../scene/BMDImport.h"

Enemy::Enemy( std::string filename )
{
    mEnemyShip = new GLNode( GLRenderer::getRootNode() );
}

Enemy::~Enemy()
{
}

void Enemy::update()
{
}
