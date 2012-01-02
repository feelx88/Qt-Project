#include "Weapon.h"

#include "../ui/GLRenderer.h"
#include "../scene/GLNode.h"
#include "../scene/BMDImport.h"
#include "../game/Game.h"

Weapon::Weapon( GLNode *shipNode, int coolDownTime, float bulletSpeed,
                int bulletCount, std::string bulletModelFileName )
    : mBulletCount( bulletCount ), mCurBullet( 0 ), mShipNode( shipNode ),
      mCoolDownTime( coolDownTime ), mAmmoLevel( 0 ), mInfiniteAmmo( true ),
      mCoolDownLevel( coolDownTime ), mBulletSpeed( bulletSpeed )
{
    for( int x = 0; x < mBulletCount; x++ )
    {
        GLNode *bullet = new GLNode( GLRenderer::getRootNode() );
        BMDImport::loadFromFile( bullet, bulletModelFileName );
        bullet->hide();
        mBullets.push_back( bullet );
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

void Weapon::update()
{
    mCoolDownLevel += 1000.f / (float)Game::frameRate;

    for( int x = 0; x < mBulletCount; x++ )
    {
        GLNode *bullet = mBullets.at( x );
        bullet->move( glm::vec3( 0.f, 0.f, -mBulletSpeed ) );
    }
}
