#include "CollisionShape.h"
#include "Node.h"

CollisionShape::CollisionShape( ShapeType shape, Node *node )
    : mNode( node ), mShapeType( shape ), mSphereRadius( 0.f )
{
}

CollisionShape::~CollisionShape()
{
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
    CollisionShape *shape = new CollisionShape( COLLISION_SPHERE, node );
    shape->setSphereRadius( radius );
    return shape;
}
