#ifndef COLLISIONSHAPE_H
#define COLLISIONSHAPE_H

#include <set>
#include <map>
#include <vector>

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

    CollisionShape( ShapeType shape );

    virtual ~CollisionShape();

    void setSphereRadius( float radius )
    {
        mSphereRadius = radius;
    }

    bool testCollision( CollisionShape *other );

    void setNode( Node *node )
    {
        mNode = node;
    }

    const Node *getNode()
    {
        return mNode;
    }

    static CollisionShape *newSpehereShape( Node *node, float radius );

    static void findCollisionPairs();

    static bool collisionOccured( CollisionShape *shape1,
                                  CollisionShape *shape2 );

    static std::vector<CollisionShape*> shapesCollidingWith(
            CollisionShape *shape );

protected:
    Node *mNode;

    ShapeType mShapeType;

    float mSphereRadius;

    static std::set<CollisionShape*> sShapes;
    static std::multimap<CollisionShape*, CollisionShape*> sShapePairs;
};

#endif // COLLISIONSHAPE_H
