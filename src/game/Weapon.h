#ifndef WEAPON_H
#define WEAPON_H

#include <string>
#include <deque>

class GLNode;

class Weapon
{
public:
    Weapon( GLNode *shipNode, int coolDownTime, float bulletSpeed,
            int bulletCount, std::string bulletModelFileName );

    ~Weapon();

    void shoot();

    void incrementAmmo( int amount = 1 )
    {
        //AmmoLevel = -1 => infinite ammo
        if( mAmmoLevel > 0 )
            mAmmoLevel += amount;
    }

    void update();

protected:
    int mBulletCount, mCurBullet;
    GLNode *mShipNode;
    std::deque<GLNode*> mBullets;

    int mCoolDownTime, mAmmoLevel;

    float mCoolDownLevel, mBulletSpeed;
};

#endif // WEAPON_H
