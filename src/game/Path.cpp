#include "Path.h"

Path::Path()
    : mCurPathIndex( 0 ), mCurFraction( 0.f ), mRecalcCurPosition( true )
{
}

Path::~Path()
{
}

void Path::reset()
{
    mCurPathIndex = 0;
    mCurFraction = 0.f;
}

glm::vec3 Path::getNextPosition()
{
    mCurFraction += 0.01f;
    mRecalcCurPosition = true;

    return getCurrentPosition();
}

glm::core::type::vec3 Path::getCurrentPosition()
{
    if( mRecalcCurPosition )
    {
        if( mCurFraction > 1.f )
        {
            if( mCurPathIndex < mPathNodes.size() - 4 )
            {
                mCurPathIndex += 4;
                mCurFraction = 0.f;
            }
            else
                mCurFraction = 1.f;
        }
        mRecalcCurPosition = false;
        return calculateBezierCurvePoint( mCurPathIndex, mCurFraction );
    }
    else
        return mCurPosition;
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

    return part1 + part2 + part3 + p0;
}
