#ifndef COLLISIONSHAPE_H
#define COLLISIONSHAPE_H

#include <set>
#include <map>
#include <vector>
#include <glm/glm.hpp>

class Node;

class CollisionShape
{
public:
    enum ShapeType
    {
        COLLISION_SPHERE = 0,
        COLLISION_BOX,
        COLLISION_MESH
    };

    struct AABB
    {
        glm::vec3 aabbMin, aabbMax;
    };

    struct CollisionTriangle
    {
        glm::vec3 a, b, c;
        AABB aabb;
    };

    CollisionShape( ShapeType shape );

    virtual ~CollisionShape();

    void setSphereRadius( float radius )
    {
        mSphereRadius = radius;
    }

    void setMeshVertices( std::vector<glm::vec3> verts )
    {
        mMeshVertices = verts;
    }

    bool testCollision( CollisionShape *other );

    void setNode( Node *node )
    {
        mNode = node;
    }

    Node *getNode()
    {
        return mNode;
    }

    AABB getObjectAABB()
    {
        return mObjectAabb;
    }

    static CollisionShape *newSphereShape( Node *node, float radius );
    static CollisionShape *newMeshShape( Node *node, std::vector<glm::vec3> verts );

    static void findCollisionPairs();

    static bool collisionOccured( CollisionShape *shape1,
                                  CollisionShape *shape2 );

    static std::vector<CollisionShape*> shapesCollidingWith(
            CollisionShape *shape );

protected:
    void recalculateAABBs();

    Node *mNode;

    ShapeType mShapeType;

    float mSphereRadius;

    std::vector<CollisionTriangle> mCollisionTris;
    std::vector<glm::vec3> mMeshVertices;
    AABB mObjectAabb;

    static std::set<CollisionShape*> sShapes;
    static std::multimap<CollisionShape*, CollisionShape*> sShapePairs;
};

#endif // COLLISIONSHAPE_H
