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

    void reset();

    glm::vec3 getNextPosition();
    glm::vec3 getCurrentPosition();

    void setLooping( bool looping )
    {
        mLooping = looping;
    }

protected:

    glm::vec3 calculateBezierCurvePoint( int startIndex, float fraction );

    std::vector<glm::vec3> mPathNodes;
    unsigned int mCurPathIndex;
    float mCurFraction;
    glm::vec3 mCurPosition;
    bool mRecalcCurPosition;

    bool mLooping;
};

#endif // PATH_H
