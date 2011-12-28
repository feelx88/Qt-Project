#ifndef WEAPON_H
#define WEAPON_H

#include <string>

class GLNode;

class Weapon
{
public:
    Weapon( int bulletCount, std::string bulletModelFileName );

protected:
    int mBulletCount;
    GLNode **mBullets;
};

#endif // WEAPON_H
