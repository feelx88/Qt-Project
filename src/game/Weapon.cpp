#include "Weapon.h"

#include "../ui/GLRenderer.h"
#include "../scene/GLNode.h"
#include "../scene/BMDImport.h"
#include "../scene/CollisionShape.h"
#include "../game/Game.h"

std::map<GLNode*, int> Weapon::sDamageMap;

Weapon::Weapon( GLNode *shipNode, int coolDownTime, float bulletSpeed,
                int bulletCount, std::string bulletModelFileName )
    : mBulletCount( bulletCount ), mCurBullet( 0 ), mShipNode( shipNode ),
      mCoolDownTime( coolDownTime ), mAmmoLevel( 0 ), mInfiniteAmmo( true ),
      mCoolDownLevel( coolDownTime ), mBulletSpeed( bulletSpeed ), mDamage( 1 )
{
    for( int x = 0; x < mBulletCount; x++ )
    {
        GLNode *bullet = new GLNode( GLRenderer::getRootNode() );
        BMDImport::loadFromFile( bullet, bulletModelFileName );
        bullet->hide();

        bullet->setCollisionShape(
                    CollisionShape::newSphereShape( bullet, 1.f ) );

        bullet->setTag( Game::NODE_BULLET );

        mBullets.push_back( bullet );

        sDamageMap.insert( std::make_pair( bullet, mDamage ) );
    }
}

Weapon::~Weapon()
{
    for( int x = 0; x < mBulletCount; x++ )
        delete mBullets[x];
}

void Weapon::shoot()
{
    if( ( !mInfiniteAmmo && mAmmoLevel == 0 ) || mCoolDownLevel < mCoolDownTime )
        return;

    mCoolDownLevel = 0.f;

    if( !mInfiniteAmmo )
        mAmmoLevel--;

    mCurBullet++;

    if( mCurBullet >= mBulletCount )
        mCurBullet = 0;

    GLNode *bullet = mBullets.at( mCurBullet );

    bullet->setPosition( mShipNode->getPosition() );
    bullet->setRotation( mShipNode->getRotation() );

    bullet->show();
}

void Weapon::setDamage( int damage )
{
    for( int x = 0; x < mBulletCount; x++ )
    {
        GLNode *bullet = mBullets.at( x );
        sDamageMap.erase( sDamageMap.find( bullet ) );
        sDamageMap.insert( std::make_pair( bullet, damage  ) );
    }
    mDamage = damage;
}

int Weapon::getDamage( GLNode *node )
{
    int damage = 0;

    std::map<GLNode*, int>::iterator x = sDamageMap.find( node );
    if( x != sDamageMap.end() )
        damage = x->second;

    return damage;
}

void Weapon::update()
{
    mCoolDownLevel += 1000.f / (float)Game::frameRate;

    for( int x = 0; x < mBulletCount; x++ )
    {
        GLNode *bullet = mBullets.at( x );
        bullet->move( glm::vec3( 0.f, 0.f, -mBulletSpeed ) );
    }
}
