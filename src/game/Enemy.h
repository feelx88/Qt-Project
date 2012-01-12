#ifndef ENEMY_H
#define ENEMY_H

#include <string>

class GLNode;
class Path;

class Enemy
{
public:
    Enemy( std::string filename );

    virtual ~Enemy();

    void update();

    void setPath( Path *path )
    {
        mPath = path;
    }

protected:
    GLNode *mEnemyShip;
    Path *mPath;
};

#endif // ENEMY_H
