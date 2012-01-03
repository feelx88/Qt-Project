#include "CollisionShape.h"
#include "Node.h"

std::set<CollisionShape*> CollisionShape::sShapes;
std::multimap<CollisionShape*, CollisionShape*> CollisionShape::sShapePairs;

CollisionShape::CollisionShape( ShapeType shape )
    : mNode( 0 ), mShapeType( shape ), mSphereRadius( 0.f )
{
    sShapes.insert( this );
}

CollisionShape::~CollisionShape()
{
    sShapes.erase( this );
}

bool CollisionShape::testCollision( CollisionShape *other )
{
    if( mShapeType == COLLISION_SPHERE )
    {
        if( other->mShapeType == COLLISION_SPHERE )
        {
            float collisionDistance = mSphereRadius + other->mSphereRadius;
            float centerDistance = glm::distance(
                    mNode->getPosition(), other->mNode->getPosition() );

            if( centerDistance <= collisionDistance )
                return true;
            else
                return false;
        }

        if( other->mShapeType == COLLISION_BOX )
        {
        }

        if( other->mShapeType == COLLISION_MESH )
        {
        }
    }

    if( mShapeType == COLLISION_BOX )
    {
        if( other->mShapeType == COLLISION_SPHERE )
        {
        }

        if( other->mShapeType == COLLISION_BOX )
        {
        }

        if( other->mShapeType == COLLISION_MESH )
        {
        }
    }

    if( mShapeType == COLLISION_MESH )
    {
        if( other->mShapeType == COLLISION_SPHERE )
        {
        }

        if( other->mShapeType == COLLISION_BOX )
        {
        }

        if( other->mShapeType == COLLISION_MESH )
        {
        }
    }
}

CollisionShape *CollisionShape::newSpehereShape( Node *node, float radius )
{
    CollisionShape *shape = new CollisionShape( COLLISION_SPHERE );
    shape->setSphereRadius( radius );
    shape->setNode( node );
    return shape;
}

void CollisionShape::findCollisionPairs()
{
    if( sShapes.size() < 2 )
        return;

    sShapePairs.clear();

    std::set<CollisionShape*>::iterator x, y;

    for( x = sShapes.begin(); x != sShapes.end()--; x++ )
    {
        CollisionShape *shape1 = *x;

        for( y = x, y++; y != sShapes.end(); y++ )
        {
            CollisionShape *shape2 = *y;
            if( shape1->testCollision( shape2 ) )
            {
                sShapePairs.insert( std::make_pair( shape1, shape2 ) );
                sShapePairs.insert( std::make_pair( shape2, shape1 ) );
            }
        }
    }
}

bool CollisionShape::collisionOccured( CollisionShape *shape1,
                                       CollisionShape *shape2 )
{
    for( std::multimap<CollisionShape*, CollisionShape*>::iterator x =
         sShapePairs.begin(); x != sShapePairs.end(); x++ )
    {
        CollisionShape *s1 = x->first;
        CollisionShape *s2 = x->second;
        if( ( shape1 == s1 || shape1 == s2 ) && ( shape2 == s1 || shape2 == s2 ) )
            return true;
    }
}

std::vector<CollisionShape*> CollisionShape::shapesCollidingWith(
        CollisionShape *shape )
{
    std::vector<CollisionShape*> ret;
    for( std::multimap<CollisionShape*, CollisionShape*>::iterator x =
         sShapePairs.begin(); x != sShapePairs.end(); x++ )
    {
        CollisionShape *s1 = x->first;
        CollisionShape *s2 = x->second;
        if( s1 == shape )
            ret.push_back( s2 );
        if( s2 == shape )
            ret.push_back( s1 );
    }

    return ret;
}
