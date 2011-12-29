#ifndef WEAPON_H
#define WEAPON_H

#include <string>

class GLNode;

class Weapon
{
public:
    Weapon( GLNode *shipNode, int coolDownTime, int bulletCount,
            std::string bulletModelFileName );

    void shoot();

    void incrementAmmo( int amount = 1 )
    {
        //AmmoLevel = -1 => infinite ammo
        if( mAmmoLevel > 0 )
            mAmmoLevel += amount;
    }

protected:
    int mBulletCount;
    GLNode *mShipNode, **mBullets;

    int mCoolDownTime, mCoolDownLevel;

    int mAmmoLevel;
};

#endif // WEAPON_H
