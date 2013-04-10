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
    mCollisionTris.clear();
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
                for( std::vector<CollisionTriangle>::iterator x = mCollisionTris.begin();
                     x != mCollisionTris.end(); x++ )
                {
                    CollisionTriangle &tri = *x;
                    tri.aabb.aabbMin -= other->mSphereRadius;
                    tri.aabb.aabbMax += other->mSphereRadius;

                    if( ( nodePos.x >= tri.aabb.aabbMin.x &&
                          nodePos.x <= tri.aabb.aabbMax.x ) &&
                        ( nodePos.y >= tri.aabb.aabbMin.y &&
                          nodePos.y <= tri.aabb.aabbMax.y ) &&
                        ( nodePos.z >= tri.aabb.aabbMin.z &&
                          nodePos.z <= tri.aabb.aabbMax.z ) )
                    {
                        glm::vec3 normal = glm::cross( tri.a - tri.b, tri.b - tri.c );
                        if( glm::dot( normal + nodePos - nodePos, mNode->getPosition() - nodePos ) <= 0.f )
                            return true;
                    }
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
    mCollisionTris.clear();

    glm::vec3 position = mNode->getPosition();

    for( unsigned int x = 0; x < mMeshVertices.size() - 3; x += 3 )
    {
        glm::vec3 a = mMeshVertices.at( x + 0 ) + position;
        glm::vec3 b = mMeshVertices.at( x + 1 ) + position;
        glm::vec3 c = mMeshVertices.at( x + 2 ) + position;

        CollisionTriangle tri;

        tri.aabb.aabbMin.y = glm::min( a.y, b.y, c.y );
        tri.aabb.aabbMin.x = glm::min( a.x, b.x, c.x );
        tri.aabb.aabbMin.z = glm::min( a.z, b.z, c.z );

        tri.aabb.aabbMax.x = glm::max( a.x, b.x, c.x );
        tri.aabb.aabbMax.y = glm::max( a.y, b.y, c.y );
        tri.aabb.aabbMax.z = glm::max( a.z, b.z, c.z );

        mObjectAabb.aabbMin = glm::min( mObjectAabb.aabbMin, tri.aabb.aabbMin );
        mObjectAabb.aabbMax = glm::max( mObjectAabb.aabbMax, tri.aabb.aabbMax );

        mCollisionTris.push_back( tri );
    }
}
