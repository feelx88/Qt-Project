#include "Weapon.h"

#include "../ui/GLRenderer.h"
#include "../scene/GLNode.h"
#include "../scene/BMDImport.h"

Weapon::Weapon( GLNode *shipNode, int coolDownTime,
                int bulletCount, std::string bulletModelFileName )
    : mBulletCount( bulletCount ), mShipNode( shipNode ),
      mCoolDownTime( coolDownTime ), mCoolDownLevel( 100 )
{
    mBullets = new GLNode*[mBulletCount];
    for( int x = 0; x < mBulletCount; x++ )
    {
        mBullets[x] = new GLNode( GLRenderer::getRootNode() );
        BMDImport::loadFromFile( mBullets[x], bulletModelFileName );
        mBullets[x]->hide();
    }
}

void Weapon::shoot()
{
    if( mAmmoLevel > 0 )
        mAmmoLevel--;
}

void Weapon::update()
{
}
