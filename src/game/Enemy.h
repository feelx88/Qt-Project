#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include <glm/glm.hpp>

class Node;
class GLNode;
class Path;

class Enemy
{
public:
    Enemy( std::string filename );

    virtual ~Enemy();

    void update();

    void setPath( Path *path );

    void setStartRadius( float rad );
    void setStartPosition( glm::vec3 pos );

protected:
    GLNode *mEnemyShip;
    Path *mPath;

    Node *mStartTrigger;
    bool mStart;

    int mHitPoints;
};

#endif // ENEMY_H
