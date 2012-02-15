#include "CollisionShape.h"
#include "Node.h"
#include <QtOpenGL>

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
    mMeshAABBs.clear();
}

bool CollisionShape::testCollision( CollisionShape *other )
{
    if( other == this )
        return true;

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

        else if( other->mShapeType == COLLISION_BOX )
        {
        }

        else if( other->mShapeType == COLLISION_MESH )
        {
            return other->testCollision( this );
        }
    }

    if( mShapeType == COLLISION_BOX )
    {
        if( other->mShapeType == COLLISION_SPHERE )
        {
        }

        else if( other->mShapeType == COLLISION_BOX )
        {
        }

        else if( other->mShapeType == COLLISION_MESH )
        {
        }
    }

    if( mShapeType == COLLISION_MESH )
    {
        if( other->mShapeType == COLLISION_SPHERE )
        {
            glm::vec3 nodePos = other->getNode()->getPosition();

            if( ( nodePos.x >= mObjectAabb.aabbMin.x &&
                  nodePos.x <= mObjectAabb.aabbMax.x ) &&
                ( nodePos.y >= mObjectAabb.aabbMin.y &&
                  nodePos.y <= mObjectAabb.aabbMax.y ) &&
                ( nodePos.z >= mObjectAabb.aabbMin.z &&
                  nodePos.z <= mObjectAabb.aabbMax.z ) )
            {
                for( std::vector<AABB>::iterator x = mMeshAABBs.begin();
                     x != mMeshAABBs.end(); x++ )
                {
                    AABB aabb = *x;
                    aabb.aabbMin -= other->mSphereRadius;
                    aabb.aabbMax += other->mSphereRadius;

                    if( ( nodePos.x >= aabb.aabbMin.x &&
                          nodePos.x <= aabb.aabbMax.x ) &&
                        ( nodePos.y >= aabb.aabbMin.y &&
                          nodePos.y <= aabb.aabbMax.y ) &&
                        ( nodePos.z >= aabb.aabbMin.z &&
                          nodePos.z <= aabb.aabbMax.z ) )
                        return true;
                }
            }
        }

        else if( other->mShapeType == COLLISION_BOX )
        {
        }

        else if( other->mShapeType == COLLISION_MESH )
        {
        }
    }
    return false;
}

CollisionShape *CollisionShape::newSphereShape( Node *node, float radius )
{
    CollisionShape *shape = new CollisionShape( COLLISION_SPHERE );
    shape->setSphereRadius( radius );
    shape->setNode( node );
    return shape;
}

CollisionShape *CollisionShape::newMeshShape(
        Node *node, std::vector<glm::vec3> verts )
{
    CollisionShape *shape = new CollisionShape( COLLISION_MESH );
    shape->setMeshVertices( verts );
    shape->setNode( node );
    shape->recalculateAABBs();
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
        if( ( shape1 == s1 && shape2 == s2 ) || ( shape1 == s2 && shape2 == s1 ) )
            return true;
    }
    return false;
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

void CollisionShape::recalculateAABBs()
{
    mMeshAABBs.clear();

    glm::vec3 position = mNode->getPosition();

    for( unsigned int x = 0; x < mMeshVertices.size() - 3; x += 3 )
    {
        glm::vec3 a = mMeshVertices.at( x + 0 ) + position;
        glm::vec3 b = mMeshVertices.at( x + 1 ) + position;
        glm::vec3 c = mMeshVertices.at( x + 2 ) + position;

        AABB aabb;

        aabb.aabbMin.y = glm::min( a.y, b.y, c.y );
        aabb.aabbMin.x = glm::min( a.x, b.x, c.x );
        aabb.aabbMin.z = glm::min( a.z, b.z, c.z );

        aabb.aabbMax.x = glm::max( a.x, b.x, c.x );
        aabb.aabbMax.y = glm::max( a.y, b.y, c.y );
        aabb.aabbMax.z = glm::max( a.z, b.z, c.z );

        mObjectAabb.aabbMin = glm::min( mObjectAabb.aabbMin, aabb.aabbMin );
        mObjectAabb.aabbMax = glm::max( mObjectAabb.aabbMax, aabb.aabbMax );

        mMeshAABBs.push_back( aabb );
    }
}
