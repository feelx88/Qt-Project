#include "Weapon.h"

#include "../ui/GLRenderer.h"
#include "../scene/GLNode.h"

Weapon::Weapon( int bulletCount, std::string bulletModelFileName )
    : mBulletCount( bulletCount )
{
    mBullets = new GLNode*[mBulletCount];
    for( int x = 0; x < mBulletCount; x++ )
        mBullets[x] = new GLNode( GLRenderer::getRootNode() );
}
