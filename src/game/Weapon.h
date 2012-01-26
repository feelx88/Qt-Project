#ifndef WEAPON_H
#define WEAPON_H

#include <string>
#include <vector>
#include <map>

class GLNode;

class Weapon
{
public:
    Weapon( GLNode *shipNode, int coolDownTime, float bulletSpeed,
            int bulletCount, std::string bulletModelFileName );

    virtual ~Weapon();

    void shoot();

    void incrementAmmo( int amount = 1 )
    {
        mAmmoLevel += amount;
    }

    void setInfiniteAmmo( bool on )
    {
        mInfiniteAmmo = on;
    }

    void setDamage( int damage );
    static int getDamage( GLNode *node );

    void update();

protected:
    int mBulletCount, mCurBullet;
    GLNode *mShipNode;
    std::vector<GLNode*> mBullets;

    int mCoolDownTime, mAmmoLevel;
    bool mInfiniteAmmo;

    float mCoolDownLevel, mBulletSpeed;

    int mDamage;
    static std::map<GLNode*, int> sDamageMap;
};

#endif // WEAPON_H
