#include "Path.h"

Path::Path()
    : mCurPathIndex( 0 ), mCurPosition( 0.f )
{
}

Path::~Path()
{
}

glm::vec3 Path::getNextPosition()
{
    if( mCurPosition >= 1.f )
    {
        if( mCurPathIndex < mPathNodes.size() - 4 )
            mCurPathIndex++;
        else
            mCurPathIndex = 0;
        mCurPosition = 0.f;
    }

    mCurPosition += 0.001f;

    return calculateBezierCurvePoint( mCurPathIndex, mCurPosition );
}

glm::vec3 Path::calculateBezierCurvePoint(int startIndex, float fraction)
{
    //Source: http://de.wikipedia.org/wiki/Bezierkurve

    glm::vec3 p0 = mPathNodes.at( startIndex + 0 );
    glm::vec3 p1 = mPathNodes.at( startIndex + 1 );
    glm::vec3 p2 = mPathNodes.at( startIndex + 2 );
    glm::vec3 p3 = mPathNodes.at( startIndex + 3 );

    glm::vec3 part1 = ( -p0 + 3.f * p1 - 3.f * p2 + p3 ) *
            fraction * fraction * fraction;
    glm::vec3 part2 = ( 3.f * p0 - 6.f * p1 + 3.f * p2 )
            * fraction * fraction;
    glm::vec3 part3 = ( -3.f * p0 + 3.f * p1 )
            * fraction;

    return part1 + part2 + part3 +p0;
}
