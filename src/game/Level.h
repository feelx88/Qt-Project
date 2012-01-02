#ifndef LEVEL_H
#define LEVEL_H

#include <string>

class GLNode;

class Level
{
public:
    Level( std::string fileName );
    virtual ~Level();

    void update();

protected:
    GLNode *mLevelModel;
};

#endif // LEVEL_H
