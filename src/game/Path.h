#ifndef PATH_H
#define PATH_H

#include <vector>
#include <glm/glm.hpp>

class Path
{
public:
    Path();

    virtual ~Path();

    void addNode( glm::vec3 node )
    {
        mPathNodes.push_back( node );
    }

    glm::vec3 getNextPosition();
protected:

    glm::vec3 calculateBezierCurvePoint( int startIndex, float fraction );

    std::vector<glm::vec3> mPathNodes;
    int mCurPathIndex;
    float mCurPosition;
};

#endif // PATH_H
