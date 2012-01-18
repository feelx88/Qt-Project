#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>

#include <glm/glm.hpp>

class GLNode;

class Enemy;

class Level
{
public:
    Level();
    virtual ~Level();

    void update();

    void loadLevel( std::string fileName );

protected:
    std::vector<GLNode*> mLevelMeshes;
    glm::vec3 mPlayerStart;

    std::vector<Enemy*> mEnemies;
};

#endif // LEVEL_H
